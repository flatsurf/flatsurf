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

#include "../flatsurf/saddle_connection.hpp"

#include <boost/lexical_cast.hpp>
#include <climits>
#include <ostream>

#include "../flatsurf/bound.hpp"
#include "../flatsurf/ccw.hpp"
#include "../flatsurf/flat_triangulation.hpp"
#include "../flatsurf/half_edge_map.hpp"
#include "../flatsurf/orientation.hpp"
#include "../flatsurf/saddle_connections.hpp"
#include "../flatsurf/saddle_connections_by_length.hpp"
#include "../flatsurf/saddle_connections_by_length_iterator.hpp"
#include "../flatsurf/saddle_connections_iterator.hpp"
#include "../flatsurf/vector.hpp"
#include "../flatsurf/vertical.hpp"
#include "impl/saddle_connection.impl.hpp"
#include "util/assert.ipp"
#include "util/hash.ipp"

namespace flatsurf {

using std::begin;
using std::end;

template <typename Surface>
SaddleConnection<Surface>::SaddleConnection(const Surface& surface, HalfEdge e) :
  self(spimpl::make_impl<ImplementationOf<SaddleConnection>>(surface, e, -e, Chain(surface, e))) {
}

template <typename Surface>
SaddleConnection<Surface>::SaddleConnection(const Surface& surface, HalfEdge source, HalfEdge target, const Chain<Surface>& chain) :
  self(spimpl::make_impl<ImplementationOf<SaddleConnection>>(surface, source, target, chain)) {
  ASSERT_ARGUMENT(self->chain, "saddle connection cannot be trivial");
}

template <typename Surface>
SaddleConnection<Surface>::SaddleConnection(const Surface& surface, HalfEdge source, HalfEdge target, Chain<Surface>&& chain) :
  self(spimpl::make_impl<ImplementationOf<SaddleConnection>>(surface, source, target, std::move(chain))) {
  ASSERT_ARGUMENT(self->chain, "saddle connection cannot be trivial");
}

template <typename Surface>
bool SaddleConnection<Surface>::operator==(const SaddleConnection<Surface>& rhs) const {
  bool ret = *self->surface == *rhs.self->surface && vector() == rhs.vector() && self->source == rhs.self->source;

  ASSERT((!ret || target() == rhs.target()), "saddle connection data is inconsistent, " << *this << " == " << rhs << " but their targets do not match since " << target() << " != " << rhs.target());
  return ret;
}

template <typename Surface>
HalfEdge SaddleConnection<Surface>::source() const {
  return self->source;
}

template <typename Surface>
HalfEdge SaddleConnection<Surface>::target() const {
  return self->target;
}

template <typename Surface>
const Surface& SaddleConnection<Surface>::surface() const {
  return self->surface;
}

template <typename Surface>
SaddleConnection<Surface> SaddleConnection<Surface>::operator-() const {
  return SaddleConnection(self->surface, self->target, self->source, -self->chain);
}

template <typename Surface>
SaddleConnection<Surface> SaddleConnection<Surface>::inSector(const Surface& surface, HalfEdge source, const Vector<T>& vector) {
  CHECK_ARGUMENT(surface.inSector(source, vector), "Cannot search for " << vector << " next to " << source << " in " << surface << "; that direction is not in the search sector");

  const auto construction = SaddleConnections<Surface>(surface)
                                .bound(Bound::upper(vector))
                                .sector(source)
                                .sector(vector, vector);

  const auto ret = begin(construction);

  CHECK_ARGUMENT(ret != end(construction), "No connection with vector " << vector << " in sector starting at " << source << " in " << surface);

  return *ret;
}

template <typename Surface>
SaddleConnection<Surface> SaddleConnection<Surface>::inHalfPlane(const Surface& surface, HalfEdge side, const Vertical<Surface>& vertical, const Vector<T>& vector) {
  const CCW allowed = vertical.ccw(side);
  HalfEdge sector;
  for (sector = side; vertical.ccw(sector) == allowed; sector = surface.previousAtVertex(sector))
    ;
  do {
    if (surface.inSector(sector, vector)) {
      return SaddleConnection::inSector(surface, sector, vector);
    }
    sector = surface.nextAtVertex(sector);
  } while (vertical.ccw(sector) == allowed);

  CHECK_ARGUMENT(false, "vector " << vector << " not on the same side of " << vertical << " as HalfEdge " << side);
}

template <typename Surface>
SaddleConnection<Surface> SaddleConnection<Surface>::inPlane(const Surface& surface, HalfEdge plane, const Vector<T>& vector) {
  CHECK_ARGUMENT(vector.ccw(surface.fromHalfEdge(plane)) != CCW::COLLINEAR || vector.orientation(surface.fromHalfEdge(plane)) != ORIENTATION::OPPOSITE, "vector must not be opposite to the HalfEdge defining the plane");

  if (surface.fromHalfEdge(plane).ccw(vector) == CCW::CLOCKWISE) {
    while (surface.fromHalfEdge(plane).ccw(vector) == CCW::CLOCKWISE)
      plane = surface.previousAtVertex(plane);
  } else if (surface.fromHalfEdge(plane).ccw(vector) == CCW::COUNTERCLOCKWISE) {
    while (surface.fromHalfEdge(plane).ccw(vector) != CCW::CLOCKWISE)
      plane = surface.nextAtVertex(plane);
    plane = surface.previousAtVertex(plane);
  }

  return SaddleConnection::inSector(surface, plane, vector);
}

template <typename Surface>
SaddleConnection<Surface> SaddleConnection<Surface>::clockwise(const SaddleConnection<Surface>& clockwiseFrom, const Vector<T>& vector) {
  auto& surface = clockwiseFrom.surface();
  HalfEdge sector = clockwiseFrom.source();
  if (clockwiseFrom.vector().ccw(vector) != CCW::CLOCKWISE)
    sector = surface.previousAtVertex(sector);
  while (!surface.inSector(sector, vector)) sector = surface.previousAtVertex(sector);
  return SaddleConnection::inSector(surface, sector, vector);
}

template <typename Surface>
SaddleConnection<Surface> SaddleConnection<Surface>::alongVertical(const Surface& surface, const Vertical<Surface>& direction, HalfEdge plane) {
  CCW ccw = -direction.ccw(plane);

  if (ccw == CCW::COLLINEAR || ccw == CCW::COUNTERCLOCKWISE) {
    while (!surface.inSector(plane, direction)) {
      plane = surface.nextAtVertex(plane);
    }
    return inSector(surface, plane, direction);
  } else {
    while (true) {
      plane = surface.previousAtVertex(plane);
      ccw = -direction.ccw(plane);
      if (ccw != CCW::CLOCKWISE) {
        return inSector(surface, plane, direction);
      }
    }
  }
}

template <typename Surface>
SaddleConnection<Surface> SaddleConnection<Surface>::inSector(const Surface& surface, HalfEdge source, const Vertical<Surface>& direction) {
  CHECK_ARGUMENT(surface.inSector(source, direction), "Cannot search in direction " << direction << " next to " << source << " in " << surface << "; that direction is not in the search sector");

  const auto construction = SaddleConnections<Surface>(surface)
                                .byLength()
                                .sector(source)
                                .sector(direction, direction);

  const auto ret = begin(construction);

  return *ret;
}

template <typename Surface>
std::vector<HalfEdge> SaddleConnection<Surface>::crossings() const {
  std::vector<HalfEdge> ret;

  // We reconstruct the sequence of half edges that this saddle connection
  // crossed. This is expensive (but cheap in terms of the output size.) This
  // information seems to be essential to properly plot a saddle connection.
  // It would be good to use a finite bound instead, see https://github.com/flatsurf/flatsurf/issues/153
  auto reconstruction = SaddleConnections<Surface>(self->surface).bound(INT_MAX).sector(source());
  auto it = reconstruction.begin();
  while (*it != *this) {
    const auto ccw = it->vector().ccw(vector());
    ASSERT(ccw != CCW::COLLINEAR, "There cannot be another saddle connection in exactly the same direction as this one but in " << self->surface << " at " << source() << " we found " << it->vector() << " which has the same direction as " << vector());
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

  ASSERT(it->target() == target(), "We reconstructed the saddle connection in " << self->surface << " starting from " << source() << " with vector " << vector() << " but it does not end at " << target() << " as claimed but at " << it->target());

  return ret;
}

template <typename Surface>
SaddleConnection<Surface> SaddleConnection<Surface>::counterclockwise(const Surface& surface, HalfEdge source, HalfEdge target, const Chain<Surface>& chain) {
  const auto normalize = [&](HalfEdge& sector, const Vector<T>& vector) {
    while (surface.fromHalfEdge(sector).ccw(vector) == CCW::COUNTERCLOCKWISE)
      sector = surface.nextAtVertex(sector);
    while (surface.fromHalfEdge(sector).ccw(vector) == CCW::CLOCKWISE)
      sector = surface.previousAtVertex(sector);
  };

  const auto& vector = static_cast<const Vector<T>&>(chain);
  normalize(source, vector);
  normalize(target, -vector);

  return SaddleConnection(surface, source, target, chain);
}

template <typename Surface>
const Vector<typename Surface::Coordinate>& SaddleConnection<Surface>::vector() const {
  return static_cast<const Vector<T>&>(chain());
}

template <typename Surface>
SaddleConnection<Surface>::operator const Vector<typename Surface::Coordinate>&() const {
  return vector();
}

template <typename Surface>
const Chain<Surface>& SaddleConnection<Surface>::chain() const {
  return self->chain;
}

template <typename Surface>
SaddleConnection<Surface>::operator const Chain<Surface>&() const {
  return chain();
}

template <typename Surface>
bool SaddleConnection<Surface>::operator>(const Bound bound) const {
  return vector() > bound;
}

template <typename Surface>
bool SaddleConnection<Surface>::operator<(const Bound bound) const {
  return vector() < bound;
}

template <typename Surface>
std::ostream& operator<<(std::ostream& os, const SaddleConnection<Surface>& self) {
  return os << self.vector() << " from " << self.source() << " to " << self.target();
}

template <typename Surface>
ImplementationOf<SaddleConnection<Surface>>::ImplementationOf(const Surface& surface, HalfEdge source, HalfEdge target, const Chain<Surface>& chain) :
  surface(surface),
  source(source),
  target(target),
  chain(chain) {}

template <typename Surface>
ImplementationOf<SaddleConnection<Surface>>::ImplementationOf(const Surface& surface, HalfEdge source, HalfEdge target, Chain<Surface>&& chain) :
  surface(surface),
  source(source),
  target(target),
  chain(std::move(chain)) {}

}  // namespace flatsurf

namespace std {

using namespace flatsurf;

template <typename Surface>
size_t hash<SaddleConnection<Surface>>::operator()(const SaddleConnection<Surface>& self) const {
  return hash_combine(self.source(), self.target(), self.chain());
}

}  // namespace std

// Instantiations of templates so implementations are generated for the linker
#include "util/instantiate.ipp"

LIBFLATSURF_INSTANTIATE_MANY_WRAPPED((LIBFLATSURF_INSTANTIATE_WITH_IMPLEMENTATION)(LIBFLATSURF_INSTANTIATE_HASH), SaddleConnection, LIBFLATSURF_FLAT_TRIANGULATION_TYPES)
