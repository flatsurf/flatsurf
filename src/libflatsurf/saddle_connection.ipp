/**********************************************************************
 *  This file is part of flatsurf.
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
 *  along with flatsurf. If not, see <https://www.gnu.org/licenses/>.
 *********************************************************************/

#include <climits>
#include <ostream>

#include "flatsurf/bound.hpp"
#include "flatsurf/saddle_connection.hpp"
#include "flatsurf/saddle_connections.hpp"

using std::ostream;

namespace flatsurf {
template <typename Vector, typename AlongTriangulation>
struct SaddleConnection<Vector, AlongTriangulation>::Implementation {
  Implementation(const FlatTriangulation<Vector> &surface, const AlongTriangulation &vector, const HalfEdge &source, const HalfEdge &target)
      : surface(surface), vector(vector), source(source), target(target) {}

  const FlatTriangulation<Vector> &surface;
  AlongTriangulation vector;
  HalfEdge source;
  HalfEdge target;
};

template <typename Vector, typename AlongTriangulation>
ostream &operator<<(ostream &os,
                    const SaddleConnection<Vector, AlongTriangulation> &self) {
  os << "SaddleConnection(" << self.source() << " -> " << self.target()
     << " in direction " << self.vector() << " crossing";

  // We reconstruct the sequence of half edges that this saddle connection
  // crossed. This is expensive (but cheap in terms of the output size.) This
  // information seems to be essential to properly plot a saddle connection.
  auto reconstruction = SaddleConnections<Vector, AlongTriangulation>(
      self.impl->surface, Bound(LLONG_MAX), self.source());
  auto it = reconstruction.begin();
  while (**it != self) {
    auto ccw = (*it)->vector().ccw(self.vector());
    assert(ccw != CCW::COLLINEAR &&
           "There cannot be another saddle connection in exactly the same "
           "direction.");
    it.skipSector(-ccw);
    while (true) {
      auto crossing = it.incrementWithCrossings();
      if (crossing.has_value()) {
        os << " " << *crossing;
      } else {
        break;
      }
    }
  }

  return os << ")";
}

template <typename Vector, typename AlongTriangulation>
bool SaddleConnection<Vector, AlongTriangulation>::operator==(
    const SaddleConnection<Vector, AlongTriangulation> &rhs) const {
  bool ret = &impl->surface == &rhs.impl->surface &&
             static_cast<Vector>(vector()) == static_cast<Vector>(rhs.vector()) &&
             source() == rhs.source();
  assert(!ret || target() == rhs.target());
  return ret;
}

template <typename Vector, typename AlongTriangulation>
HalfEdge SaddleConnection<Vector, AlongTriangulation>::source() const { return impl->source; }

template <typename Vector, typename AlongTriangulation>
HalfEdge SaddleConnection<Vector, AlongTriangulation>::target() const { return impl->target; }

template <typename Vector, typename AlongTriangulation>
const AlongTriangulation &SaddleConnection<Vector, AlongTriangulation>::vector() const { return impl->vector; }
}  // namespace flatsurf

// Instantiations of templates so implementations are generated for the linker
#include <exact-real/number_field_traits.hpp>
#include "flatsurf/forward.hpp"
#include "flatsurf/vector_eantic.hpp"
#include "flatsurf/vector_exactreal.hpp"
#include "flatsurf/vector_longlong.hpp"
using namespace flatsurf;

template struct flatsurf::SaddleConnection<VectorLongLong>;
template ostream &flatsurf::operator<<(
    ostream &, const SaddleConnection<VectorLongLong> &);
template struct flatsurf::SaddleConnection<VectorEAntic>;
template ostream &flatsurf::operator<<(ostream &,
                                       const SaddleConnection<VectorEAntic> &);
template struct flatsurf::SaddleConnection<
    VectorEAntic, VectorAlongTriangulation<VectorEAntic>>;
template ostream &flatsurf::operator<<(
    ostream &,
    const SaddleConnection<VectorEAntic, VectorAlongTriangulation<VectorEAntic>>
        &);
template struct flatsurf::SaddleConnection<
    VectorExactReal<exactreal::NumberFieldTraits>>;
template ostream &flatsurf::operator<<(
    ostream &,
    const SaddleConnection<VectorExactReal<exactreal::NumberFieldTraits>> &);
