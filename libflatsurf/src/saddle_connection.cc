/**********************************************************************
 *  This file is part of flatsurf.
 *
 *        Copyright (C) 2019 Julian Rüth
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

#ifndef LIBFLATSURF_SADDLE_CONNECTION_IPP
#define LIBFLATSURF_SADDLE_CONNECTION_IPP

#include <climits>
#include <ostream>
#include <boost/lexical_cast.hpp>
#include <intervalxt/length.hpp>

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
  Implementation(const std::shared_ptr<const Surface>& surface, HalfEdge source, HalfEdge target, const typename Surface::Vector &vector)
      : surface(surface), source(source), target(target), vector(vector) {}

  std::shared_ptr<const Surface> surface;
  HalfEdge source;
  HalfEdge target;
  typename Surface::Vector vector;
};

template <typename Surface, typename _>
ostream &operator<<(ostream &os, const SaddleConnection<Surface> &self) {
  os << "SaddleConnection(" << self.source() << " -> " << self.target()
     << " in direction " << self.vector();

  bool crossingAnyHalfEdge = false;

  for (auto & crossing : self.crossings()) {
    if (!crossingAnyHalfEdge)
      os << " crossing";
    crossingAnyHalfEdge = true;
    os << " " << crossing;
  }

  return os << ")";
}

template <typename Surface>
SaddleConnection<Surface>::SaddleConnection(const std::shared_ptr<const Surface>& surface, HalfEdge source, HalfEdge target, const typename Surface::Vector &vector) : impl(spimpl::make_impl<Implementation>(surface, source, target, vector)) {}

template <typename Surface>
bool SaddleConnection<Surface>::operator==(const SaddleConnection<Surface> &rhs) const {
  bool ret = impl->surface == rhs.impl->surface && static_cast<typename Surface::Vector>(vector()) == static_cast<typename Surface::Vector>(rhs.vector()) && source() == rhs.source();
  assert(!ret || target() == rhs.target() && "Saddle Connection data is inconsistent");
  return ret;
}

template <typename Surface>
HalfEdge SaddleConnection<Surface>::source() const { return impl->source; }

template <typename Surface>
HalfEdge SaddleConnection<Surface>::target() const { return impl->target; }

template <typename Surface>
const Surface& SaddleConnection<Surface>::surface() const { return *impl->surface; }

template <typename Surface>
const typename Surface::Vector &SaddleConnection<Surface>::vector() const { return impl->vector; }

template <typename Surface>
std::vector<HalfEdge> SaddleConnection<Surface>::crossings() const {
  std::vector<HalfEdge> ret;

  // We reconstruct the sequence of half edges that this saddle connection
  // crossed. This is expensive (but cheap in terms of the output size.) This
  // information seems to be essential to properly plot a saddle connection.
  auto reconstruction = SaddleConnections<Surface>(impl->surface, Bound(INT_MAX), source());
  auto it = reconstruction.begin();
  while (**it != *this) {
    auto ccw = (*it)->vector().ccw(vector());
    assert(ccw != CCW::COLLINEAR && "There cannot be another saddle connection in exactly the same direction.");
    it.skipSector(-ccw);
    while (true) {
      auto crossing = it.incrementWithCrossings();
      if (crossing.has_value()) {
        ret.push_back(*crossing);
      } else {
        break;
      }
    }
  }

  return ret;
}

}  // namespace flatsurf

// Instantiations of templates so implementations are generated for the linker
#include <exact-real/integer_ring.hpp>
#include <exact-real/number_field.hpp>
#include <exact-real/rational_field.hpp>
#include "flatsurf/forward.hpp"

namespace flatsurf {
template class SaddleConnection<FlatTriangulation<long long>>;
template ostream &operator<<(ostream &, const SaddleConnection<FlatTriangulation<long long>> &);
template class SaddleConnection<FlatTriangulation<eantic::renf_elem_class>>;
template ostream &operator<<(ostream &, const SaddleConnection<FlatTriangulation<eantic::renf_elem_class>> &);
template class SaddleConnection<FlatTriangulation<exactreal::Element<exactreal::IntegerRing>>>;
template ostream &operator<<(ostream &, const SaddleConnection<FlatTriangulation<exactreal::Element<exactreal::IntegerRing>>> &);
template class SaddleConnection<FlatTriangulation<exactreal::Element<exactreal::RationalField>>>;
template ostream &operator<<(ostream &, const SaddleConnection<FlatTriangulation<exactreal::Element<exactreal::RationalField>>> &);
template class SaddleConnection<FlatTriangulation<exactreal::Element<exactreal::NumberField>>>;
template ostream &operator<<(ostream &, const SaddleConnection<FlatTriangulation<exactreal::Element<exactreal::NumberField>>> &);
}

#endif
