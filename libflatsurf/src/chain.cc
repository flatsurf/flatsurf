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

#include "../flatsurf/chain.hpp"

#include <flint/fmpz.h>
#include <flint/fmpz_vec.h>
#include <fmt/format.h>
#include <fmt/ostream.h>
#include <gmp.h>
#include <gmpxx.h>

#include <ostream>

#include "../flatsurf/bound.hpp"
#include "../flatsurf/fmt.hpp"
#include "external/rx-ranges/include/rx/ranges.hpp"
#include "impl/chain.impl.hpp"
#include "impl/chain_iterator.impl.hpp"
#include "util/assert.ipp"
#include "util/hash.ipp"

namespace flatsurf {

template <typename Surface>
Chain<Surface>::Chain(const Surface& surface) :
  self(spimpl::make_impl<ImplementationOf<Chain>>(surface)) {
}

template <typename Surface>
Chain<Surface>::Chain(const Surface& surface, HalfEdge e) :
  self(spimpl::make_impl<ImplementationOf<Chain>>(surface, e)) {
}

template <typename Surface>
const mpz_class& Chain<Surface>::operator[](const Edge& edge) const {
  auto ret = (*self)[edge.index()];
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
Chain<Surface>::operator const Vector<T> &() const {
  return *self;
}

template <typename Surface>
Chain<Surface>::operator const Vector<exactreal::Arb> &() const {
  return *self;
}

template <typename Surface>
Chain<Surface>::operator bool() const {
  return not _fmpz_vec_is_zero(self->coefficients, self->surface->size());
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
  Chain<Surface> ret(self->surface);
  ret -= *this;
  return ret;
}

template <typename Surface>
Chain<Surface>& Chain<Surface>::operator+=(HalfEdge halfEdge) {
  Edge edge(halfEdge);
  fmpz_add_si(self->coefficients + edge.index(), self->coefficients + edge.index(), halfEdge == edge.positive() ? 1 : -1);

  self->vector += halfEdge;
  self->approximateVector += halfEdge;

  return *this;
}

template <typename Surface>
Chain<Surface>& Chain<Surface>::operator-=(HalfEdge halfEdge) {
  return *this += -halfEdge;
}

template <typename Surface>
Chain<Surface>& Chain<Surface>::operator+=(const Chain& rhs) {
  _fmpz_vec_add(self->coefficients, self->coefficients, rhs.self->coefficients, surface().size());

  self->vector += rhs.self->vector;
  self->approximateVector += rhs.self->approximateVector;

  return *this;
}

template <typename Surface>
Chain<Surface>& Chain<Surface>::operator-=(const Chain& rhs) {
  _fmpz_vec_sub(self->coefficients, self->coefficients, rhs.self->coefficients, surface().size());

  self->vector -= rhs.self->vector;
  self->approximateVector -= rhs.self->approximateVector;

  return *this;
}

template <typename Surface>
Chain<Surface>& Chain<Surface>::operator+=(Chain&& rhs) {
  _fmpz_vec_add(self->coefficients, self->coefficients, rhs.self->coefficients, surface().size());

  self->vector += std::move(rhs.self->vector);
  self->approximateVector += std::move(rhs.self->approximateVector);

  return *this;
}

template <typename Surface>
Chain<Surface>& Chain<Surface>::operator-=(Chain&& rhs) {
  _fmpz_vec_sub(self->coefficients, self->coefficients, rhs.self->coefficients, surface().size());

  self->vector -= std::move(rhs.self->vector);
  self->approximateVector -= std::move(rhs.self->approximateVector);

  return *this;
}

template <typename Surface>
bool Chain<Surface>::operator==(const Chain& rhs) const {
  if (surface() != rhs.surface())
    return false;
  // We cannot use _fmpz_vec_equal since our coefficients are not normalized,
  // i.e., fmpz_equal assumes that no fmpz_t is an mpz_t if it still fits into
  // a single limb. However, we promote our entries in the operator[] so we
  // break that rule.
  return !(*this - rhs);
}

template <typename Surface>
Chain<Surface>& Chain<Surface>::operator*=(const mpz_class& c) {
  fmpz_t cc;
  fmpz_init_set_readonly(cc, c.get_mpz_t());
  _fmpz_vec_scalar_mul_fmpz(self->coefficients, self->coefficients, surface().size(), cc);
  fmpz_clear_readonly(cc);
  return *this;
}

template <typename Surface>
const Surface& Chain<Surface>::surface() const {
  return self->surface;
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
ImplementationOf<Chain<Surface>>::ImplementationOf(const Surface& surface) :
  surface(surface),
  coefficients(_fmpz_vec_init(surface.size())),
  vector(this),
  approximateVector(this) {
}

template <typename Surface>
ImplementationOf<Chain<Surface>>::ImplementationOf(const Surface& surface, HalfEdge halfEdge) :
  surface(surface),
  coefficients(_fmpz_vec_init(surface.size())),
  vector(this, this->surface->fromHalfEdge(halfEdge)),
  approximateVector(this, this->surface->fromHalfEdgeApproximate(halfEdge)) {
  Edge edge(halfEdge);
  fmpz_add_si(coefficients + edge.index(), coefficients + edge.index(), halfEdge == edge.positive() ? 1 : -1);
}

template <typename Surface>
ImplementationOf<Chain<Surface>>::ImplementationOf(const ImplementationOf& rhs) :
  surface(rhs.surface),
  coefficients(_fmpz_vec_init(surface->size())),
  vector(this, rhs.vector),
  approximateVector(this, rhs.approximateVector) {
  _fmpz_vec_set(coefficients, rhs.coefficients, surface->size());
}

template <typename Surface>
ImplementationOf<Chain<Surface>>::~ImplementationOf() {
  _fmpz_vec_clear(coefficients, surface->size());
}

template <typename Surface>
std::optional<const mpz_class*> ImplementationOf<Chain<Surface>>::operator[](const size_t index) const {
  if (fmpz_is_zero(&coefficients[index])) return std::nullopt;

  __mpz_struct* promoted = _fmpz_promote_val(&coefficients[index]);
  // This is hack that gmpxx.h is using as well. An mpz_class and a mpz_t are
  // the same in memory so we can cast the latter to the former to get a
  // non-owning mpz_class.
  return reinterpret_cast<const mpz_class*>(promoted);
}

template <typename Surface>
size_t ImplementationOf<Chain<Surface>>::hash(const Chain<Surface>& self) {
  const auto hash_fmpz = [](fmpz_t x) -> size_t {
    if (fmpz_fits_si(x)) {
      return fmpz_get_si(x);
    } else {
      __mpz_struct* y = _fmpz_promote_val(x);
      const size_t lowest_limb = mpz_get_ui(y);
      _fmpz_demote_val(x);
      return lowest_limb;
    }
  };

  size_t ret = hash_fmpz(&self.self->coefficients[0]);
  for (size_t i = 1; i < self.surface().size(); i++)
    ret = hash_combine(ret, hash_fmpz(&self.self->coefficients[i]));

  return ret;
}

template <typename Surface>
ImplementationOf<Chain<Surface>>::operator const Vector<T> &() const {
  return vector;
}

template <typename Surface>
ImplementationOf<Chain<Surface>>::operator const Vector<exactreal::Arb> &() const {
  return approximateVector;
}

}  // namespace flatsurf

namespace std {

using namespace flatsurf;

template <typename Surface>
size_t hash<Chain<Surface>>::operator()(const Chain<Surface>& self) const {
  return ImplementationOf<Chain<Surface>>::hash(self);
}

}  // namespace std

// Instantiations of templates so implementations are generated for the linker
#include "util/instantiate.ipp"

LIBFLATSURF_INSTANTIATE_MANY_WRAPPED((LIBFLATSURF_INSTANTIATE_WITH_IMPLEMENTATION)(LIBFLATSURF_INSTANTIATE_HASH), Chain, LIBFLATSURF_FLAT_TRIANGULATION_TYPES)
