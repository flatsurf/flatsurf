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

#include <gmpxx.h>

#include "../flatsurf/bound.hpp"
#include "../flatsurf/chain.hpp"
#include "../flatsurf/edge.hpp"
#include "../flatsurf/vector.hpp"

#include "impl/chain.impl.hpp"

#include "util/assert.ipp"

namespace flatsurf {

template <typename Surface>
Chain<Surface>::Chain() :
  Chain(std::make_shared<Surface>()) {}

template <typename Surface>
Chain<Surface>::Chain(std::shared_ptr<const Surface> surface) :
  impl(spimpl::make_impl<Implementation>(surface)) {}

template <typename Surface>
Chain<Surface>::operator const typename Surface::Vector&() const {
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
    if (auto chain = std::get_if<Chain>(&move)) {
      // TODO: We should only do this if converting the chain is cheapear than
      // converting *this after adding the chain coefficients.
      *impl->vector += static_cast<const typename Surface::Vector&>(*chain);
    } else if (auto halfEdge = std::get_if<HalfEdge>(&move)) {
      *impl->vector += impl->surface->fromEdge(*halfEdge);
    } else {
      throw std::logic_error("unsupported primitive in moves");
    }
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
    (void)static_cast<const typename Surface::Vector&>(*this);
    ASSERT(impl->approximateVector, "exact vector creation did not set approximate vector");
  }

  // TODO: We could simplify the pendingVectorMoves before applying them.
  // Probably by keeping them as a somewhat storage optimized TrackingMap which
  // kills itself and vector upon flip. (Rather, TrackingStorage should be
  // optimized for such cases. The underlying storage primitive should also be
  // used here.)
  for (auto& move : impl->pendingApproximateVectorMoves) {
    if (auto chain = std::get_if<Chain>(&move)) {
      // TODO: We should only do this if converting the chain is cheapear than
      // converting *this after adding the chain coefficients.
      *impl->approximateVector += static_cast<const Vector<exactreal::Arb>&>(*chain);
    } else if (auto halfEdge = std::get_if<HalfEdge>(&move)) {
      *impl->approximateVector += impl->surface->fromEdgeApproximate(*halfEdge);
    } else {
      throw std::logic_error("unsupported primitive in moves");
    }
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
    (void)static_cast<const typename Surface::Vector&>(*this);
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
bool Chain<Surface>::operator<(const Bound rhs) const {
  // TODO: Check approximate first.
  return static_cast<typename Surface::Vector>(*this) < rhs;
}

template <typename Surface>
bool Chain<Surface>::operator>(const Bound rhs) const {
  // TODO: Check approximate first.
  return static_cast<typename Surface::Vector>(*this) > rhs;
}

template <typename Surface>
Chain<Surface> Chain<Surface>::operator-() const {
  Chain<Surface> neg(impl->surface);

  if (impl->vector) {
    (void)static_cast<const typename Surface::Vector&>(*this);
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
std::ostream& operator<<(std::ostream& os, const Chain<Surface>& chain) {
  return os << chain.impl->coefficients;
}

template <typename Surface>
Implementation<Chain<Surface>>::Implementation(std::shared_ptr<const Surface> surface) :
  surface(surface),
  coefficients(
      surface.get(), [&](const Edge&) { return 0; }, updateAfterFlip),
  vector(typename Surface::Vector()),
  approximateVector(Vector<exactreal::Arb>()) {}

template <typename Surface>
void Implementation<Chain<Surface>>::recordMove(const Move& move) const {
  if (vector) {
    if (pendingVectorMoves.size() < surface->size()) {
      pendingVectorMoves.push_back(move);
    } else {
      vector = {};
      pendingVectorMoves.clear();
    }
  }

  if (approximateVector) {
    if (pendingApproximateVectorMoves.size() < surface->size()) {
      pendingApproximateVectorMoves.push_back(move);
    } else {
      approximateVector = {};
      pendingApproximateVectorMoves.clear();
    }
  }
}

template <typename Surface>
void Implementation<Chain<Surface>>::updateAfterFlip(EdgeMap<mpz_class>& map, HalfEdge flip) {
  const auto& parent = map.parent();

  // TODO TODO TODO: Chain needs to be tracking, so we trash pendingMoves on flip and everything else.

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

namespace std {

using namespace flatsurf;

template <typename Surface>
size_t hash<Chain<Surface>>::operator()(const Chain<Surface>& self) const noexcept {
  return hash<EdgeMap<mpz_class>>()(self.impl->coefficients);
}

}  // namespace std

// Instantiations of templates so implementations are generated for the linker
#include "util/instantiate.ipp"

LIBFLATSURF_INSTANTIATE_MANY_WRAPPED((LIBFLATSURF_INSTANTIATE_WITH_IMPLEMENTATION)(LIBFLATSURF_INSTANTIATE_HASH), Chain, LIBFLATSURF_FLAT_TRIANGULATION_TYPES)
