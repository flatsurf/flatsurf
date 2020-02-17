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

#include <flint/fmpz_vec.h>
#include <flint/fmpz.h>
#include <fmt/format.h>
#include <fmt/ostream.h>
#include <gmp.h>
#include <gmpxx.h>

#include "../flatsurf/chain.hpp"
#include "../flatsurf/fmt.hpp"
#include "../flatsurf/bound.hpp"

#include "external/rx-ranges/include/rx/ranges.hpp"

#include "impl/chain.impl.hpp"
#include "impl/chain_iterator.impl.hpp"

#include "util/assert.ipp"

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
  return surface() == rhs.surface()
    && _fmpz_vec_equal(impl->coefficients, rhs.impl->coefficients, surface().size());
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
  return ::flatsurf::Implementation<ChainIterator<Surface>>::begin(this);
}

template <typename Surface>
ChainIterator<Surface> Chain<Surface>::end() const {
  return ::flatsurf::Implementation<ChainIterator<Surface>>::end(this);
}

template <typename Surface>
std::ostream& operator<<(std::ostream& os, const Chain<Surface>& chain) {
  return os << fmt::format("[{}]", fmt::join(chain | rx::transform([](const auto& entry) { return fmt::format("{}: {}", entry.first, *entry.second); }) | rx::to_vector(), ", "));
}

template <typename Surface>
Implementation<Chain<Surface>>::Implementation(std::shared_ptr<const Surface> surface) :
  surface(std::move(surface)),
  coefficients(_fmpz_vec_init(this->surface->size())),
  vector(this),
  approximateVector(this) {
}

template <typename Surface>
Implementation<Chain<Surface>>::Implementation(const Implementation& rhs) :
  surface(rhs.surface),
  coefficients(_fmpz_vec_init(this->surface->size())),
  vector(this, rhs.vector),
  approximateVector(this, rhs.approximateVector) {
  _fmpz_vec_add(coefficients, coefficients, rhs.coefficients, this->surface->size());
}

template <typename Surface>
Implementation<Chain<Surface>>::~Implementation() {
  _fmpz_vec_clear(coefficients, surface->size());
}

template <typename Surface>
std::optional<const mpz_class*> Implementation<Chain<Surface>>::operator[](const size_t index) const {
  if (fmpz_is_zero(&coefficients[index])) return {};

  __mpz_struct* promoted = _fmpz_promote_val(&coefficients[index]);
  // This is hack that gmpxx.h is using as well. An mpz_class and a mpz_t are
  // the same in memory so we can cast the latter to the former to get a
  // non-owning mpz_class.
  return reinterpret_cast<const mpz_class*>(promoted);
}

template <typename Surface>
Implementation<Chain<Surface>>::operator const Vector<T>&() const {
  return vector;
}

template <typename Surface>
Implementation<Chain<Surface>>::operator const Vector<exactreal::Arb>&() const {
  return approximateVector;
}

}

/*
namespace flatsurf {

template <typename Surface>
Chain<Surface>::operator const Vector<T>&() const {
  if (!impl->vector) {
    ASSERT(impl->pendingVectorMoves.size() == 0, "pendingVectorMoves was not properly cleared when vector was reset");
    impl->vector.emplace();
    impl->coefficients.apply([&](const Edge& edge, const mpz_class& c) {
      *impl->vector += c * impl->surface->fromEdge(edge.positive());
    });
  }

  // TODO: We could simplify the pendingVectorMoves before applying them.
  // Probably by keeping them as a somewhat storage optimized TrackingMap which
  // kills itself and vector upon flip. (Rather, TrackingStorage should be
  // optimized for such cases. The underlying storage primitive should also be
  // used here.)
  for (auto& move : impl->pendingVectorMoves) {
    // TODO: We should only do this if converting the chain is cheapear than
    // converting *this after adding the chain coefficients.
    *impl->vector += move;
  }

  impl->pendingVectorMoves.clear();

  // TODO: Maybe this should also be reset when a lot of error has been accumulated.
  if (!impl->approximateVector)
    impl->approximateVector = static_cast<Vector<exactreal::Arb>>(*impl->vector);

  return *impl->vector;
}

template <typename Surface>
Chain<Surface>::operator const Vector<exactreal::Arb>&() const {
  if (!impl->approximateVector) {
    ASSERT(impl->pendingApproximateVectorMoves.size() == 0, "pendingApproximateVectorMoves was not properly cleared when vector was reset");
    (void)static_cast<const Vector<T>&>(*this);
    ASSERT(impl->approximateVector, "exact vector creation did not set approximate vector");
  }

  // TODO: We could simplify the pendingVectorMoves before applying them.
  // Probably by keeping them as a somewhat storage optimized TrackingMap which
  // kills itself and vector upon flip. (Rather, TrackingStorage should be
  // optimized for such cases. The underlying storage primitive should also be
  // used here.)
  for (auto& move : impl->pendingApproximateVectorMoves) {
    // TODO: We should only do this if converting the chain is cheapear than
    // converting *this after adding the chain coefficients.
    *impl->approximateVector += move;
  }

  impl->pendingApproximateVectorMoves.clear();

  return *impl->approximateVector;
}

template <typename Surface>
const Surface& Chain<Surface>::surface() const {
  return *impl->surface;
}

template <typename Surface>
Chain<Surface>& Chain<Surface>::operator+=(const Chain<Surface>& rhs) {
  impl->recordMove(rhs);

  rhs.impl->coefficients.apply([&](const auto& edge, const auto& c) {
    impl->coefficients.get(edge) += c;
  });

  return *this;
}

template <typename Surface>
Chain<Surface>& Chain<Surface>::operator+=(Chain<Surface>&& rhs) {
  rhs.impl->coefficients.apply([&](const auto& edge, const auto& c) {
    impl->coefficients.get(edge) += c;
  });

  impl->recordMove(std::move(rhs));

  return *this;
}

template <typename Surface>
Chain<Surface>& Chain<Surface>::operator-=(const Chain<Surface>& rhs) {
  return *this += -rhs;
}

template <typename Surface>
Chain<Surface>& Chain<Surface>::operator+=(HalfEdge rhs) {
  impl->recordMove(rhs);

  const Edge edge(rhs);
  if (rhs == edge.positive())
    impl->coefficients.get(edge)++;
  else
    impl->coefficients.get(edge)--;

  return *this;
}

template <typename Surface>
Chain<Surface>& Chain<Surface>::operator*=(const mpz_class& rhs) {
  if (impl->vector) {
    (void)static_cast<const Vector<T>&>(*this);
    ASSERT(impl->pendingVectorMoves.size() == 0, "producing the current vector did not clear pending moves");
    *impl->vector *= rhs;
  }

  if (impl->approximateVector) {
    (void)static_cast<const Vector<exactreal::Arb>&>(*this);
    ASSERT(impl->pendingApproximateVectorMoves.size() == 0, "producing the current approximate vector did not clear pending moves");
    *impl->approximateVector *= rhs;
  }

  impl->coefficients.apply([&](const Edge& edge, const mpz_class& c) {
    impl->coefficients.set(edge, c * rhs);
  });

  return *this;
}

template <typename Surface>
mpz_class Chain<Surface>::operator[](const Edge& edge) const {
  return impl->coefficients.get(edge);
}

template <typename Surface>
Chain<Surface>& Chain<Surface>::operator-=(HalfEdge rhs) {
  return *this += -rhs;
}

template <typename Surface>
bool Chain<Surface>::operator==(const Chain<Surface>& rhs) const {
  return impl->surface == rhs.impl->surface && impl->coefficients == rhs.impl->coefficients;
}

template <typename Surface>
Chain<Surface> Chain<Surface>::operator-() const {
  Chain<Surface> neg(impl->surface);

  if (impl->vector) {
    (void)static_cast<const Vector<T>&>(*this);
    neg.impl->vector = -*impl->vector;
  }

  if (impl->approximateVector) {
    (void)static_cast<const Vector<exactreal::Arb>&>(*this);
    neg.impl->approximateVector = -*impl->approximateVector;
  }

  impl->coefficients.apply([&](const Edge& edge, const mpz_class& c) {
    neg.impl->coefficients.set(edge, -c);
  });

  return neg;
}

template <typename Surface>
Chain<Surface>::operator bool() const {
  bool zero = true;

  impl->coefficients.apply([&](const Edge&, const mpz_class& c) {
    if (c) zero = false;
  });

  return !zero;
}

template <typename Surface>
Implementation<Chain<Surface>>::Implementation(std::shared_ptr<const Surface> surface) :
  surface(std::move(surface)),
  coefficients(
      this->surface.get(), [&](const Edge&) { return mpz_class(); }, updateCoefficientsAfterFlip),
  vector(std::in_place_t()),
  approximateVector(std::in_place_t()) {}

template <typename Surface>
Implementation<Chain<Surface>>::Implementation(std::shared_ptr<const Surface> surface, HalfEdge halfEdge) :
  surface(std::move(surface)),
  coefficients(
      this->surface.get(), [&](const Edge& edge) {
        if (halfEdge == edge.positive())
          return mpz_class(1);
        else if (halfEdge == edge.negative())
          return mpz_class(-1);
        else
          return mpz_class();
      }, updateCoefficientsAfterFlip),
  vector(this->surface->fromEdge(halfEdge)),
  approximateVector(this->surface->fromEdgeApproximate(halfEdge)) {}

template <typename Surface>
void Implementation<Chain<Surface>>::recordMove(HalfEdge edge) const {
  recordMove(Chain(surface, edge));
}

template <typename Surface>
void Implementation<Chain<Surface>>::recordMove(const Chain<Surface>& move) const {
  if (vector) {
    if (pendingVectorMoves.size() < surface->size() && move.impl->vector.has_value()) {
      pendingVectorMoves.push_back(move);
    } else {
      vector = {};
      pendingVectorMoves.clear();
    }
  }

  if (approximateVector) {
    if (pendingApproximateVectorMoves.size() < surface->size() && move.impl->approximateVector.has_value()) {
      pendingApproximateVectorMoves.push_back(move);
    } else {
      approximateVector = {};
      pendingApproximateVectorMoves.clear();
    }
  }
}

template <typename Surface>
void Implementation<Chain<Surface>>::updateCoefficientsAfterFlip(EdgeMap<mpz_class>& map, HalfEdge flip) {
  const auto& parent = map.parent();

  const auto set = [&](HalfEdge halfEdge, const mpz_class& c) {
    if (Edge(halfEdge).positive() == halfEdge)
      map.set(halfEdge, c);
    else
      map.set(halfEdge, -c);
  };

  set(-parent.nextAtVertex(flip), map.get(flip));
  set(-parent.nextInFace(parent.nextInFace(flip)), map.get(flip));
  set(flip, 0);
}

}  // namespace flatsurf

*/

namespace std {

using namespace flatsurf;

template <typename Surface>
size_t hash<Chain<Surface>>::operator()(const Chain<Surface>& self) const noexcept {
  return hash<Vector<typename Surface::Coordinate>>()(static_cast<const Vector<typename Surface::Coordinate>&>(self));
}

}  // namespace std

// Instantiations of templates so implementations are generated for the linker
#include "util/instantiate.ipp"

LIBFLATSURF_INSTANTIATE_MANY_WRAPPED((LIBFLATSURF_INSTANTIATE_WITH_IMPLEMENTATION)(LIBFLATSURF_INSTANTIATE_HASH), Chain, LIBFLATSURF_FLAT_TRIANGULATION_TYPES)
