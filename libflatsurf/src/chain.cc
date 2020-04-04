/**********************************************************************
 *  This file is part of flatsurf.
 *
 *        Copyright (C) 2019-2020 Julian Rüth
 *
 *  Flatsurf is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Flatsurf is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with flatsurf. If not, see <https://www.gnu.org/licenses/>.
 *********************************************************************/

#include <flint/fmpz.h>
#include <flint/fmpz_vec.h>
#include <fmt/format.h>
#include <fmt/ostream.h>
#include <gmp.h>
#include <gmpxx.h>

#include "../flatsurf/bound.hpp"
#include "../flatsurf/chain.hpp"
#include "../flatsurf/fmt.hpp"

#include "external/rx-ranges/include/rx/ranges.hpp"

#include "impl/chain.impl.hpp"
#include "impl/chain_iterator.impl.hpp"

#include "util/assert.ipp"
#include "util/hash.ipp"

namespace flatsurf {

template <typename Surface>
Chain<Surface>::Chain(std::shared_ptr<const Surface> surface) :
  impl(spimpl::make_impl<Implementation>(std::move(surface))) {
}

template <typename Surface>
Chain<Surface>::Chain(std::shared_ptr<const Surface> surface, HalfEdge e) :
  Chain(surface) {
  *this += e;
}

template <typename Surface>
const mpz_class& Chain<Surface>::operator[](const Edge& edge) const {
  auto ret = (*impl)[edge.index()];
  static mpz_class zero;
  return ret ? **ret : zero;
}

template <typename Surface>
mpz_class Chain<Surface>::operator[](const HalfEdge& halfEdge) const {
  Edge edge(halfEdge);
  mpz_class positive = (*this)[edge];
  return halfEdge == edge.positive() ? positive : -positive;
}

template <typename Surface>
Chain<Surface>::operator const Vector<T>&() const {
  return *impl;
}

template <typename Surface>
Chain<Surface>::operator const Vector<exactreal::Arb>&() const {
  return *impl;
}

template <typename Surface>
Chain<Surface>::operator bool() const {
  return not _fmpz_vec_is_zero(impl->coefficients, impl->surface->size());
}

template <typename Surface>
bool Chain<Surface>::operator<(const Bound rhs) const {
  const auto approx = static_cast<const Vector<exactreal::Arb>&>(*this) < rhs;
  if (approx)
    return *approx;
  return static_cast<const Vector<T>&>(*this) < rhs;
}

template <typename Surface>
bool Chain<Surface>::operator>(const Bound rhs) const {
  const auto approx = static_cast<const Vector<exactreal::Arb>&>(*this) > rhs;
  if (approx)
    return *approx;
  return static_cast<const Vector<T>&>(*this) > rhs;
}

template <typename Surface>
Chain<Surface> Chain<Surface>::operator-() const {
  Chain<Surface> ret(impl->surface);
  ret -= *this;
  return ret;
}

template <typename Surface>
Chain<Surface>& Chain<Surface>::operator+=(HalfEdge halfEdge) {
  Edge edge(halfEdge);
  fmpz_add_si(impl->coefficients + edge.index(), impl->coefficients + edge.index(), halfEdge == edge.positive() ? 1 : -1);

  impl->vector += halfEdge;
  impl->approximateVector += halfEdge;

  return *this;
}

template <typename Surface>
Chain<Surface>& Chain<Surface>::operator-=(HalfEdge halfEdge) {
  return *this += -halfEdge;
}

template <typename Surface>
Chain<Surface>& Chain<Surface>::operator+=(const Chain& rhs) {
  _fmpz_vec_add(impl->coefficients, impl->coefficients, rhs.impl->coefficients, surface().size());

  impl->vector += rhs.impl->vector;
  impl->approximateVector += rhs.impl->approximateVector;

  return *this;
}

template <typename Surface>
Chain<Surface>& Chain<Surface>::operator-=(const Chain& rhs) {
  _fmpz_vec_sub(impl->coefficients, impl->coefficients, rhs.impl->coefficients, surface().size());

  impl->vector -= rhs.impl->vector;
  impl->approximateVector -= rhs.impl->approximateVector;

  return *this;
}

template <typename Surface>
Chain<Surface>& Chain<Surface>::operator+=(Chain&& rhs) {
  _fmpz_vec_add(impl->coefficients, impl->coefficients, rhs.impl->coefficients, surface().size());

  impl->vector += std::move(rhs.impl->vector);
  impl->approximateVector += std::move(rhs.impl->approximateVector);

  return *this;
}

template <typename Surface>
Chain<Surface>& Chain<Surface>::operator-=(Chain&& rhs) {
  _fmpz_vec_sub(impl->coefficients, impl->coefficients, rhs.impl->coefficients, surface().size());

  impl->vector -= std::move(rhs.impl->vector);
  impl->approximateVector -= std::move(rhs.impl->approximateVector);

  return *this;
}

template <typename Surface>
bool Chain<Surface>::operator==(const Chain& rhs) const {
  return surface() == rhs.surface() && _fmpz_vec_equal(impl->coefficients, rhs.impl->coefficients, surface().size());
}

template <typename Surface>
Chain<Surface>& Chain<Surface>::operator*=(const mpz_class& c) {
  fmpz_t cc;
  fmpz_init_set_readonly(cc, c.get_mpz_t());
  _fmpz_vec_scalar_mul_fmpz(impl->coefficients, impl->coefficients, surface().size(), cc);
  fmpz_clear_readonly(cc);
  return *this;
}

template <typename Surface>
const Surface& Chain<Surface>::surface() const {
  return *impl->surface;
}

template <typename Surface>
ChainIterator<Surface> Chain<Surface>::begin() const {
  return ImplementationOf<ChainIterator<Surface>>::begin(this);
}

template <typename Surface>
ChainIterator<Surface> Chain<Surface>::end() const {
  return ImplementationOf<ChainIterator<Surface>>::end(this);
}

template <typename Surface>
std::ostream& operator<<(std::ostream& os, const Chain<Surface>& chain) {
  return os << fmt::format("[{}]", fmt::join(chain | rx::transform([](const auto& entry) { return fmt::format("{}: {}", entry.first, *entry.second); }) | rx::to_vector(), ", "));
}

template <typename Surface>
ImplementationOf<Chain<Surface>>::ImplementationOf(std::shared_ptr<const Surface> surface) :
  surface(std::move(surface)),
  coefficients(_fmpz_vec_init(this->surface->size())),
  vector(this),
  approximateVector(this) {
}

template <typename Surface>
ImplementationOf<Chain<Surface>>::ImplementationOf(const ImplementationOf& rhs) :
  surface(rhs.surface),
  coefficients(_fmpz_vec_init(this->surface->size())),
  vector(this, rhs.vector),
  approximateVector(this, rhs.approximateVector) {
  _fmpz_vec_add(coefficients, coefficients, rhs.coefficients, this->surface->size());
}

template <typename Surface>
ImplementationOf<Chain<Surface>>::~ImplementationOf() {
  _fmpz_vec_clear(coefficients, surface->size());
}

template <typename Surface>
std::optional<const mpz_class*> ImplementationOf<Chain<Surface>>::operator[](const size_t index) const {
  if (fmpz_is_zero(&coefficients[index])) return {};

  __mpz_struct* promoted = _fmpz_promote_val(&coefficients[index]);
  // This is hack that gmpxx.h is using as well. An mpz_class and a mpz_t are
  // the same in memory so we can cast the latter to the former to get a
  // non-owning mpz_class.
  return reinterpret_cast<const mpz_class*>(promoted);
}

template <typename Surface>
ImplementationOf<Chain<Surface>>::operator const Vector<T>&() const {
  return vector;
}

template <typename Surface>
ImplementationOf<Chain<Surface>>::operator const Vector<exactreal::Arb>&() const {
  return approximateVector;
}

}  // namespace flatsurf

namespace std {

using namespace flatsurf;

template <typename Surface>
size_t hash<Chain<Surface>>::operator()(const Chain<Surface>& self) const noexcept {
  const auto hash_fmpz = [](fmpz_t x) -> size_t {
    if (fmpz_fits_si(x)) {
      return fmpz_get_si(x);
    } else {
      __mpz_struct *y = _fmpz_promote_val(x);
      const size_t lowest_limb = mpz_get_ui(y);
      _fmpz_demote_val(x);
      return lowest_limb;
    }
  };

  size_t ret = hash_fmpz(&self.impl->coefficients[0]);
  for (size_t i = 1; i < self.surface().size(); i++)
    ret = hash_combine(ret, hash_fmpz(&self.impl->coefficients[i]));

  return ret;
}

}  // namespace std

// Instantiations of templates so implementations are generated for the linker
#include "util/instantiate.ipp"

LIBFLATSURF_INSTANTIATE_MANY_WRAPPED((LIBFLATSURF_INSTANTIATE_WITH_IMPLEMENTATION)(LIBFLATSURF_INSTANTIATE_HASH), Chain, LIBFLATSURF_FLAT_TRIANGULATION_TYPES)
