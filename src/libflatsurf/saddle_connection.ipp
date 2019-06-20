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

#ifndef LIBFLATSURF_SADDLE_CONNECTION_IPP
#define LIBFLATSURF_SADDLE_CONNECTION_IPP

// SaddleConnection lives in an .ipp header file since
// SaddleConnections::Iterator needs access to its private Implementaion to
// create instances.

#include <climits>
#include <ostream>

#include "flatsurf/flat_triangulation.hpp"
#include "flatsurf/saddle_connection.hpp"
#include "flatsurf/saddle_connections.hpp"
#include "flatsurf/vector.hpp"

#include "util/assert.ipp"

using std::ostream;

namespace flatsurf {
template <typename Surface>
class SaddleConnection<Surface>::Implementation {
 public:
  Implementation(Surface const *surface, const typename Surface::Vector &vector, const HalfEdge &source, const HalfEdge &target)
      : surface(surface), vector(vector), source(source), target(target) {}

  Surface const *surface;
  const typename Surface::Vector vector;
  HalfEdge source;
  HalfEdge target;
};

template <typename Surface>
ostream &operator<<(ostream &os, const SaddleConnection<Surface> &self) {
  os << "SaddleConnection(" << self.source() << " -> " << self.target()
     << " in direction " << self.vector();

  bool crossingAnyHalfEdge = false;

  // We reconstruct the sequence of half edges that this saddle connection
  // crossed. This is expensive (but cheap in terms of the output size.) This
  // information seems to be essential to properly plot a saddle connection.
  auto reconstruction = SaddleConnections<Surface>(self.impl->surface, Bound(LLONG_MAX), self.source());
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
        if (!crossingAnyHalfEdge)
          os << " crossing";
        crossingAnyHalfEdge = true;
        os << " " << *crossing;
      } else {
        break;
      }
    }
  }

  return os << ")";
}

template <typename Surface>
bool SaddleConnection<Surface>::operator==(const SaddleConnection<Surface> &rhs) const {
  bool ret = &impl->surface == &rhs.impl->surface && static_cast<typename Surface::Vector>(vector()) == static_cast<typename Surface::Vector>(rhs.vector()) && source() == rhs.source();
  assert(!ret || target() == rhs.target());
  return ret;
}

template <typename Surface>
HalfEdge SaddleConnection<Surface>::source() const { return impl->source; }

template <typename Surface>
HalfEdge SaddleConnection<Surface>::target() const { return impl->target; }

template <typename Surface>
const typename Surface::Vector &SaddleConnection<Surface>::vector() const { return impl->vector; }
}  // namespace flatsurf

// Instantiations of templates so implementations are generated for the linker
#include <exact-real/integer_ring_traits.hpp>
#include <exact-real/number_field_traits.hpp>
#include <exact-real/rational_field_traits.hpp>
#include "flatsurf/forward.hpp"
using namespace flatsurf;

template class flatsurf::SaddleConnection<flatsurf::FlatTriangulation<long long>>;
template ostream &flatsurf::operator<<(ostream &, const SaddleConnection<flatsurf::FlatTriangulation<long long>> &);
template class flatsurf::SaddleConnection<flatsurf::FlatTriangulation<eantic::renf_elem_class>>;
template ostream &flatsurf::operator<<(ostream &, const SaddleConnection<flatsurf::FlatTriangulation<eantic::renf_elem_class>> &);
template class flatsurf::SaddleConnection<flatsurf::FlatTriangulation<exactreal::Element<exactreal::IntegerRingTraits>>>;
template ostream &flatsurf::operator<<(ostream &, const SaddleConnection<flatsurf::FlatTriangulation<exactreal::Element<exactreal::IntegerRingTraits>>> &);
template class flatsurf::SaddleConnection<flatsurf::FlatTriangulation<exactreal::Element<exactreal::RationalFieldTraits>>>;
template ostream &flatsurf::operator<<(ostream &, const SaddleConnection<flatsurf::FlatTriangulation<exactreal::Element<exactreal::RationalFieldTraits>>> &);
template class flatsurf::SaddleConnection<flatsurf::FlatTriangulation<exactreal::Element<exactreal::NumberFieldTraits>>>;
template ostream &flatsurf::operator<<(ostream &, const SaddleConnection<flatsurf::FlatTriangulation<exactreal::Element<exactreal::NumberFieldTraits>>> &);

#endif
