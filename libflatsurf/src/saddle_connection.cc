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

#include <climits>
#include <ostream>

#include <boost/lexical_cast.hpp>

#include "../flatsurf/bound.hpp"
#include "../flatsurf/ccw.hpp"
#include "../flatsurf/flat_triangulation.hpp"
#include "../flatsurf/half_edge_map.hpp"
#include "../flatsurf/orientation.hpp"
#include "../flatsurf/saddle_connection.hpp"
#include "../flatsurf/saddle_connections.hpp"
#include "../flatsurf/saddle_connections_iterator.hpp"
#include "../flatsurf/vector.hpp"
#include "../flatsurf/vertical.hpp"

#include "impl/saddle_connection.impl.hpp"

#include "util/assert.ipp"
#include "util/hash.ipp"

using std::ostream;

namespace flatsurf {

template <typename Surface>
SaddleConnection<Surface>::SaddleConnection(std::shared_ptr<const Surface> surface, HalfEdge e) :
  impl(spimpl::make_impl<Implementation>(surface, e)) {
}

template <typename Surface>
SaddleConnection<Surface>::SaddleConnection(std::shared_ptr<const Surface> surface, HalfEdge source, HalfEdge target, const Chain<Surface>& chain) :
  impl(spimpl::make_impl<Implementation>(surface, source, target, chain)) {
}

template <typename Surface>
bool SaddleConnection<Surface>::operator==(const SaddleConnection<Surface>& rhs) const {
  bool ret = *impl->surface == *rhs.impl->surface && vector() == rhs.vector() && impl->source == rhs.impl->source;

  ASSERT((!ret || target() == rhs.target()), "saddle connection data is inconsistent, " << *this << " == " << rhs << " but their targets do not match since " << target() << " != " << rhs.target());
  return ret;
}

template <typename Surface>
HalfEdge SaddleConnection<Surface>::source() const {
  return impl->source;
}

template <typename Surface>
HalfEdge SaddleConnection<Surface>::target() const {
  return impl->target;
}

template <typename Surface>
const Surface& SaddleConnection<Surface>::surface() const {
  return *impl->surface;
}

template <typename Surface>
SaddleConnection<Surface> SaddleConnection<Surface>::operator-() const noexcept {
  return SaddleConnection(impl->surface, impl->target, impl->source, -impl->chain);
}

template <typename Surface>
SaddleConnection<Surface> SaddleConnection<Surface>::inSector(std::shared_ptr<const Surface> surface, HalfEdge source, const Vector<T>& vector) {
  CHECK_ARGUMENT(surface->inSector(source, vector), "Cannot search for " << vector << " next to " << source << " in " << *surface << "; that direction is not in the search sector");

  // It would be good to use a finite bound instead, see https://github.com/flatsurf/flatsurf/issues/153
  return reconstruct(
      surface, source, [&](const auto& it) { return it->vector() == vector; }, [&](const auto& it) { return -it->vector().ccw(vector); });
}

template <typename Surface>
SaddleConnection<Surface> SaddleConnection<Surface>::reconstruct(std::shared_ptr<const Surface> surface, HalfEdge source, std::function<bool(const SaddleConnectionsIterator<Surface>&)> until, std::function<CCW(const SaddleConnectionsIterator<Surface>&)> skip, Bound bound) {
  auto reconstruction = SaddleConnections<Surface>(surface, bound, source);
  auto it = reconstruction.begin();
  for (; !until(it); ++it) it.skipSector(skip(it));
  return *it;
}

template <typename Surface>
SaddleConnection<Surface> SaddleConnection<Surface>::inHalfPlane(std::shared_ptr<const Surface> surface, HalfEdge side, const Vertical<Surface>& vertical, const Vector<T>& vector) {
  CCW allowed = vertical.vertical().ccw(surface->fromEdge(side));
  HalfEdge sector;
  for (sector = side; vertical.vertical().ccw(surface->fromEdge(sector)) == allowed; sector = surface->previousAtVertex(sector))
    ;
  do {
    if (surface->inSector(sector, vector)) {
      return SaddleConnection::inSector(surface, sector, vector);
    }
    sector = surface->nextAtVertex(sector);
  } while (vertical.vertical().ccw(surface->fromEdge(sector)) == allowed);

  CHECK_ARGUMENT(false, "vector " << vector << " not on the same side of " << vertical << " as HalfEdge " << side);
}

template <typename Surface>
SaddleConnection<Surface> SaddleConnection<Surface>::inPlane(std::shared_ptr<const Surface> surface, HalfEdge plane, const Vector<T>& vector) {
  CHECK_ARGUMENT(vector.ccw(surface->fromEdge(plane)) != CCW::COLLINEAR || vector.orientation(surface->fromEdge(plane)) != ORIENTATION::OPPOSITE, "vector must not be opposite to the HalfEdge defining the plane");

  if (surface->fromEdge(plane).ccw(vector) == CCW::CLOCKWISE) {
    while (surface->fromEdge(plane).ccw(vector) == CCW::CLOCKWISE)
      plane = surface->previousAtVertex(plane);
  } else if (surface->fromEdge(plane).ccw(vector) == CCW::COUNTERCLOCKWISE) {
    while (surface->fromEdge(plane).ccw(vector) != CCW::CLOCKWISE)
      plane = surface->nextAtVertex(plane);
    plane = surface->previousAtVertex(plane);
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
  return SaddleConnection::inSector(surface.shared_from_this(), sector, vector);
}

template <typename Surface>
SaddleConnection<Surface> SaddleConnection<Surface>::alongVertical(std::shared_ptr<const Surface> surface, const Vertical<Surface>& direction, HalfEdge plane) {
  CCW ccw = surface->fromEdge(plane).ccw(direction.vertical());

  if (ccw == CCW::COLLINEAR || ccw == CCW::COUNTERCLOCKWISE) {
    while (!surface->inSector(plane, direction)) {
      plane = surface->nextAtVertex(plane);
    }
    return inSector(surface, plane, direction);
  } else {
    while (true) {
      plane = surface->previousAtVertex(plane);
      ccw = surface->fromEdge(plane).ccw(direction.vertical());
      if (ccw != CCW::CLOCKWISE) {
        return inSector(surface, plane, direction);
      }
    }
  }
}

template <typename Surface>
SaddleConnection<Surface> SaddleConnection<Surface>::inSector(std::shared_ptr<const Surface> surface, HalfEdge source, const Vertical<Surface>& direction) {
  CHECK_ARGUMENT(surface->inSector(source, direction.vertical()), "Cannot search in direction " << direction << " next to " << source << " in " << *surface << "; that direction is not in the search sector");

  return reconstruct(
      surface, source, [&](const auto& it) { return !direction.perpendicular(it->vector()); }, [&](const auto& it) { return -it->vector().ccw(direction.vertical()); });
}

template <typename Surface>
std::vector<HalfEdge> SaddleConnection<Surface>::crossings() const {
  std::vector<HalfEdge> ret;

  // We reconstruct the sequence of half edges that this saddle connection
  // crossed. This is expensive (but cheap in terms of the output size.) This
  // information seems to be essential to properly plot a saddle connection.
  // It would be good to use a finite bound instead, see https://github.com/flatsurf/flatsurf/issues/153
  auto reconstruction = SaddleConnections<Surface>(impl->surface, Bound(INT_MAX, 0), source());
  auto it = reconstruction.begin();
  while (*it != *this) {
    const auto ccw = it->vector().ccw(vector());
    ASSERT(ccw != CCW::COLLINEAR, "There cannot be another saddle connection in exactly the same direction as this one but in " << impl->surface << " at " << source() << " we found " << it->vector() << " which has the same direction as " << vector());
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

  ASSERT(it->target() == target(), "We reconstructed the saddle connection in " << impl->surface << " starting from " << source() << " with vector " << vector() << " but it does not end at " << target() << " as claimed but at " << it->target());

  return ret;
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
  return impl->chain;
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

template <typename Surface, typename _>
ostream& operator<<(ostream& os, const SaddleConnection<Surface>& self) {
  return os << self.vector() << " from " << self.source() << " to " << self.target();
}

template <typename Surface>
ImplementationOf<SaddleConnection<Surface>>::ImplementationOf(std::shared_ptr<const Surface>& surface, HalfEdge e) :
  surface(surface),
  source(e),
  target(-e),
  chain(surface, e) {
}

template <typename Surface>
ImplementationOf<SaddleConnection<Surface>>::ImplementationOf(std::shared_ptr<const Surface>& surface, HalfEdge source, HalfEdge target, const Chain<Surface>& chain) :
  surface(surface),
  source(source),
  target(target),
  chain(chain) {
  ASSERT_ARGUMENT(chain, "saddle connection cannot be trivial");

  normalize();
}

template <typename Surface>
void ImplementationOf<SaddleConnection<Surface>>::normalize() {
  const auto normalize = [&](HalfEdge& sector, const Vector<T>& vector) {
    while (surface->fromEdge(sector).ccw(vector) == CCW::COUNTERCLOCKWISE)
      sector = surface->nextAtVertex(sector);
    while (surface->fromEdge(sector).ccw(vector) == CCW::CLOCKWISE)
      sector = surface->previousAtVertex(sector);
  };

  const auto& vector = static_cast<const Vector<T>&>(chain);
  normalize(this->source, vector);
  normalize(this->target, -vector);
}

}  // namespace flatsurf

namespace std {

using namespace flatsurf;

template <typename Surface>
size_t hash<SaddleConnection<Surface>>::operator()(const SaddleConnection<Surface>& self) const noexcept {
  return hash_combine(self.source(), self.target(), self.chain());
}

}  // namespace std

// Instantiations of templates so implementations are generated for the linker
#include "util/instantiate.ipp"

LIBFLATSURF_INSTANTIATE_MANY_WRAPPED((LIBFLATSURF_INSTANTIATE_WITH_IMPLEMENTATION)(LIBFLATSURF_INSTANTIATE_HASH), SaddleConnection, LIBFLATSURF_FLAT_TRIANGULATION_TYPES)
