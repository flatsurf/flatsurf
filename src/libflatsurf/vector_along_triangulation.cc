/**********************************************************************
 *  This file is part of Flatsurf.
 *
 *        Copyright (C) 2019 Julian Rüth
 *
 *  Flatsurf is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  Flatsurf is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Flatsurf. If not, see <https://www.gnu.org/licenses/>.
 *********************************************************************/

#include <ostream>

#include "libflatsurf/bound.hpp"
#include "libflatsurf/flat_triangulation.hpp"
#include "libflatsurf/half_edge.hpp"
#include "libflatsurf/half_edge_map.hpp"
#include "libflatsurf/vector_along_triangulation.hpp"
#include "libflatsurf/vector_eantic.hpp"

using std::ostream;

namespace flatsurf {
template <typename V>
struct VectorAlongTriangulation<V>::Implementation {
  using Surface = FlatTriangulation<V>;

  explicit Implementation(const Surface& surface) : surface(surface), impl() {}

  const Surface& surface;
  V impl;
};

template <typename V>
VectorAlongTriangulation<V>::VectorAlongTriangulation(const Surface& surface)
    : impl(spimpl::make_impl<Implementation>(surface)) {}

template <typename V>
VectorAlongTriangulation<V>::VectorAlongTriangulation(
    const Surface& surface, const HalfEdgeMap<int>& coefficients)
    : VectorAlongTriangulation(surface) {
  coefficients.apply(
      [&](const HalfEdge e, const int coeff) { this->iadd(e, coeff); });
}

template <typename V>
VectorAlongTriangulation<V>& VectorAlongTriangulation<V>::operator+=(
    const HalfEdge halfEdge) {
  this->iadd(halfEdge, 1);
  return *this;
}

template <typename V>
VectorAlongTriangulation<V>& VectorAlongTriangulation<V>::operator-=(
    const HalfEdge halfEdge) {
  this->iadd(halfEdge, -1);
  return *this;
}

template <typename V>
VectorAlongTriangulation<V>& VectorAlongTriangulation<V>::iadd(
    const HalfEdge halfEdge, const int c) {
  if (c != 0) {
    auto v = c * impl->surface.fromEdge(halfEdge);
    impl->impl += v;
  }
  return *this;
}

template <typename V>
CCW VectorAlongTriangulation<V>::ccw(
    const VectorAlongTriangulation<V>& other) const {
  return impl->impl.ccw(other.impl->impl);
}

template <typename V>
bool VectorAlongTriangulation<V>::operator>(const Bound bound) const {
  return impl->impl > bound;
}

template <typename V>
bool VectorAlongTriangulation<V>::operator<(const Bound bound) const {
  return impl->impl < bound;
}

template <typename V>
VectorAlongTriangulation<V>::operator V() const {
  return impl->impl;
}

template <typename V>
ostream& operator<<(ostream& os, const VectorAlongTriangulation<V>& self) {
  return os << self.impl->impl;
}

}  // namespace flatsurf

// Instantiations of templates so implementations are generated for the linker
#include <exact-real/number_field_traits.hpp>
#include "libflatsurf/vector_eantic.hpp"
#include "libflatsurf/vector_exactreal.hpp"
#include "libflatsurf/vector_longlong.hpp"
template struct flatsurf::VectorAlongTriangulation<flatsurf::VectorLongLong>;
template ostream& flatsurf::operator<<(
    ostream&,
    const flatsurf::VectorAlongTriangulation<flatsurf::VectorLongLong>&);
template struct flatsurf::VectorAlongTriangulation<flatsurf::VectorEAntic>;
template ostream& flatsurf::operator<<(
    ostream&,
    const flatsurf::VectorAlongTriangulation<flatsurf::VectorEAntic>&);
template struct flatsurf::VectorAlongTriangulation<
    flatsurf::VectorExactReal<exactreal::NumberFieldTraits>>;
template ostream& flatsurf::operator<<(
    ostream&, const flatsurf::VectorAlongTriangulation<
                  flatsurf::VectorExactReal<exactreal::NumberFieldTraits>>&);
