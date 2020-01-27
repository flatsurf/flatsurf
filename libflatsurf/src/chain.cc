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

#include "../flatsurf/chain.hpp"
#include "../flatsurf/edge.hpp"
#include "../flatsurf/bound.hpp"
#include "../flatsurf/vector.hpp"

#include "impl/chain.impl.hpp"

namespace flatsurf {

template <typename Surface>
Chain<Surface>::Chain() : Chain(std::make_shared<Surface>()) {}

template <typename Surface>
Chain<Surface>::Chain(std::shared_ptr<const Surface> surface) :
  impl(spimpl::make_impl<Implementation>(surface)) {}

template <typename Surface>
Chain<Surface>::operator typename Surface::Vector() const {
  typename Surface::Vector vector;
  impl->coefficients.apply([&](const Edge& edge, const mpz_class& c) {
    vector += c * impl->surface->fromEdge(edge.positive());
  });
  return vector;
}

template <typename Surface>
const Surface& Chain<Surface>::surface() const {
  return *impl->surface;
}

template <typename Surface>
Chain<Surface>& Chain<Surface>::operator+=(const Chain<Surface>& rhs) {
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
  Edge edge(rhs);
  if (rhs == edge.positive())
    impl->coefficients.get(edge)++;
  else
    impl->coefficients.get(edge)--;
  return *this;
}

template <typename Surface>
Chain<Surface>& Chain<Surface>::operator*=(const mpz_class& rhs) {
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
  return static_cast<typename Surface::Vector>(*this) < rhs;
}

template <typename Surface>
bool Chain<Surface>::operator>(const Bound rhs) const {
  return static_cast<typename Surface::Vector>(*this) > rhs;
}

template <typename Surface>
Chain<Surface> Chain<Surface>::operator-() const {
  Chain<Surface> neg(impl->surface);
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
  coefficients(surface.get(), [&](const Edge&) { return 0; }, updateAfterFlip) {}

template <typename Surface>
void Implementation<Chain<Surface>>::updateAfterFlip(EdgeMap<mpz_class>& map, HalfEdge flip) {
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

}

namespace std {

using namespace flatsurf;

template <typename Surface>
size_t hash<Chain<Surface>>::operator()(const Chain<Surface>& self) const noexcept {
  return hash<EdgeMap<mpz_class>>()(self.impl->coefficients);
}

}


// Instantiations of templates so implementations are generated for the linker
#include "util/instantiate.ipp"

LIBFLATSURF_INSTANTIATE_MANY_WRAPPED((LIBFLATSURF_INSTANTIATE_WITH_IMPLEMENTATION)(LIBFLATSURF_INSTANTIATE_HASH), Chain, LIBFLATSURF_FLAT_TRIANGULATION_TYPES)
