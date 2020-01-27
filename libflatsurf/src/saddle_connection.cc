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

#include <boost/lexical_cast.hpp>
#include <climits>
#include <ostream>

#include "../flatsurf/bound.hpp"
#include "../flatsurf/flat_triangulation.hpp"
#include "../flatsurf/saddle_connection.hpp"
#include "../flatsurf/saddle_connections.hpp"
#include "../flatsurf/vector.hpp"
#include "../flatsurf/vertical.hpp"
#include "../flatsurf/half_edge_map.hpp"

#include "impl/saddle_connection.impl.hpp"

#include "util/assert.ipp"
#include "util/hash.ipp"

using std::ostream;

namespace flatsurf {

template <typename Surface>
SaddleConnection<Surface>::SaddleConnection() :
  // TODO: Would be better if this were a saddle connection on a trivial surface whatever that might mean.
  impl(nullptr) {}

template <typename Surface>
SaddleConnection<Surface>::SaddleConnection(std::shared_ptr<const Surface> surface, HalfEdge source, HalfEdge target, const Chain<Surface>& chain) :
  impl(spimpl::make_impl<Implementation>(surface, source, target, chain)) {
}

template <typename Surface>
bool SaddleConnection<Surface>::operator==(const SaddleConnection<Surface> &rhs) const {
  if (!*this) return !rhs;
  if (!rhs) return false;

  bool ret = *impl->surface == *rhs.impl->surface
    && static_cast<typename Surface::Vector>(*this) == static_cast<typename Surface::Vector>(rhs)
    && impl->source == rhs.impl->source;

  ASSERT((!ret || target() == rhs.target()), "saddle connection data is inconsistent, " << *this << " == " << rhs << " but their targets do not match since " << target() << " != " << rhs.target());
  return ret;
}

template <typename Surface>
HalfEdge SaddleConnection<Surface>::source() const {
  CHECK_ARGUMENT(*this, "trivial saddle connection has no source");
  return impl->source;
}

template <typename Surface>
HalfEdge SaddleConnection<Surface>::target() const {
  CHECK_ARGUMENT(*this, "trivial saddle connection has no target");
  return impl->target;
}

template <typename Surface>
const Surface &SaddleConnection<Surface>::surface() const {
  ASSERT_ARGUMENT(*this, "trivial saddle connection not associated to a specific surface");
  return *impl->surface;
}

template <typename Surface>
SaddleConnection<Surface> SaddleConnection<Surface>::operator-() const noexcept {
  if (!*this) return *this;
  return SaddleConnection(impl->surface, impl->target, impl->source, -impl->chain);
}

template <typename Surface>
SaddleConnection<Surface>::operator bool() const {
  if (impl == nullptr) return false;
  ASSERT(static_cast<typename Surface::Vector>(impl->chain), "saddle connections must be nonzero");
  return true;
}

template <typename Surface>
SaddleConnection<Surface> SaddleConnection<Surface>::inSector(std::shared_ptr<const Surface> surface, HalfEdge source, const Vector& vector) {
  CHECK_ARGUMENT(surface->inSector(source, vector), "Cannot search for " << vector << " next to " << source << " in " << *surface << "; that direction is not in the search sector");

  // TODO: Bound should be length of vector
  auto reconstruction = SaddleConnections<Surface>(surface, Bound(INT_MAX, 0), source);
  auto it = reconstruction.begin();
  for (; *it != vector; it++) {
    auto ccw = static_cast<Vector>(*it).ccw(vector);
    it.skipSector(-ccw);
  }
  return *it;
}

template <typename Surface>
SaddleConnection<Surface> SaddleConnection<Surface>::fromEdge(std::shared_ptr<const Surface> surface, HalfEdge edge) {
  return SaddleConnection(surface, edge, -edge, Chain<Surface>(surface) += edge);
}

template <typename Surface>
SaddleConnection<Surface> SaddleConnection<Surface>::inHalfPlane(std::shared_ptr<const Surface> surface, HalfEdge side, const Vertical<Surface>& vertical, const Vector& vector) {
  CCW allowed = vertical.vertical().ccw(surface->fromEdge(side));
  HalfEdge sector;
  for(sector = side; vertical.vertical().ccw(surface->fromEdge(sector)) == allowed; sector = surface->previousAtVertex(sector));
  do {
    if (surface->inSector(sector, vector)) {
      return SaddleConnection::inSector(surface, sector, vector);
    }
    sector = surface->nextAtVertex(sector);
  }while(vertical.vertical().ccw(surface->fromEdge(sector)) == allowed);

  CHECK_ARGUMENT(false, "vector " << vector << " not on the same side of " << vertical << " as HalfEdge " << side);
}

template <typename Surface>
SaddleConnection<Surface> SaddleConnection<Surface>::inPlane(std::shared_ptr<const Surface> surface, HalfEdge plane, const Vector& vector) {
  CHECK_ARGUMENT(vector.ccw(surface->fromEdge(plane)) != CCW::COLLINEAR || vector.orientation(surface->fromEdge(plane)) != ORIENTATION::OPPOSITE, "vector must not be opposite to the HalfEdge defining the plane");

  if (surface->fromEdge(plane).ccw(vector) == CCW::CLOCKWISE) {
    while(surface->fromEdge(plane).ccw(vector) == CCW::CLOCKWISE)
      plane = surface->previousAtVertex(plane);
  } else if (surface->fromEdge(plane).ccw(vector) == CCW::COUNTERCLOCKWISE) {
    while(surface->fromEdge(plane).ccw(vector) != CCW::CLOCKWISE)
      plane = surface->nextAtVertex(plane);
    plane = surface->previousAtVertex(plane);
  }

  return SaddleConnection::inSector(surface, plane, vector);
}

template <typename Surface>
SaddleConnection<Surface> SaddleConnection<Surface>::clockwise(const SaddleConnection<Surface>& clockwiseFrom, const Vector& vector) {
  auto& surface = clockwiseFrom.surface();
  HalfEdge sector = clockwiseFrom.source();
  if (clockwiseFrom.ccw(vector) != CCW::CLOCKWISE)
    sector = surface.previousAtVertex(sector);
  while(!surface.inSector(sector, vector)) sector = surface.previousAtVertex(sector);
  return SaddleConnection::inSector(surface.shared_from_this(), sector, vector);
}

template <typename Surface>
SaddleConnection<Surface> SaddleConnection<Surface>::alongVertical(std::shared_ptr<const Surface> surface, const Vertical<Surface>& direction, HalfEdge plane) {
  CCW ccw = surface->fromEdge(plane).ccw(direction.vertical());

  if (ccw == CCW::COLLINEAR || ccw == CCW::COUNTERCLOCKWISE) {
    while(!surface->inSector(plane, direction)) {
      plane = surface->nextAtVertex(plane);
    }
    return inSector(surface, plane, direction);
  } else {
    while(true) {
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

  // TODO: Pull this reconstruction process out of the two inSector methods.
  auto reconstruction = SaddleConnections<Surface>(surface, Bound(INT_MAX, 0), source);
  auto it = reconstruction.begin();
  for(; direction.perpendicular(*it); it++) {
    auto ccw = static_cast<Vector>(*it).ccw(direction.vertical());
    it.skipSector(-ccw);
  }
  return *it;
}

template <typename Surface>
std::vector<HalfEdge> SaddleConnection<Surface>::crossings() const {
  std::vector<HalfEdge> ret;

  // We reconstruct the sequence of half edges that this saddle connection
  // crossed. This is expensive (but cheap in terms of the output size.) This
  // information seems to be essential to properly plot a saddle connection.
  // TODO: Bound should be length of vector
  auto reconstruction = SaddleConnections<Surface>(impl->surface, Bound(INT_MAX, 0), source());
  auto it = reconstruction.begin();
  while (*it != *this) {
    auto ccw = it->ccw(*this);
    ASSERT(ccw != CCW::COLLINEAR, "There cannot be another saddle connection in exactly the same direction as this one but in " << impl->surface << " at " << source() << " we found " << static_cast<Vector>(*it) << " which has the same direction as " << static_cast<Vector>(*this));
    it.skipSector(-ccw);
    while (true) {
      auto crossing = it.incrementWithCrossings();
      if (crossing.has_value()) {
        // TODO: Remove
        // ASSERT(ret.size() < 10000, "Failed to find " << *this << " in its source sector");
        ret.push_back(*crossing);
      } else {
        break;
      }
    }
  }

  ASSERT(it->target() == target(), "We reconstructed the saddle connection in " << impl->surface << " starting from " << source() << " with vector " << static_cast<Vector>(*this) << " but it does not end at " << target() << " as claimed but at " << it->target());

  return ret;
}

template <typename Surface>
ORIENTATION SaddleConnection<Surface>::orientation(const typename SaddleConnection::Vector& rhs) const {
  return static_cast<typename Surface::Vector>(*this).orientation(rhs);
}

template <typename Surface>
CCW SaddleConnection<Surface>::ccw(const typename SaddleConnection::Vector& rhs) const {
  return static_cast<typename Surface::Vector>(*this).ccw(rhs);
}

template <typename Surface>
SaddleConnection<Surface>::operator typename Surface::Vector() const {
  return static_cast<Chain<Surface>>(*this);
}

template <typename Surface>
SaddleConnection<Surface>::operator Chain<Surface>() const {
  if (!*this) return Chain<Surface>();
  return impl->chain;
}

template <typename Surface>
bool SaddleConnection<Surface>::operator>(const Bound bound) const {
  return static_cast<Vector>(*this) > bound;
}

template <typename Surface>
bool SaddleConnection<Surface>::operator<(const Bound bound) const {
  return static_cast<Vector>(*this) < bound;
}

template <typename Surface, typename _>
ostream &operator<<(ostream &os, const SaddleConnection<Surface> &self) {
  if (!self) return os << "0";
  return os << "SaddleConnection(" << static_cast<typename Surface::Vector>(self) << " from " << self.source() << ")";
}

template <typename Surface>
Implementation<SaddleConnection<Surface>>::Implementation(std::shared_ptr<const Surface> &surface, HalfEdge source, HalfEdge target, const Chain<Surface> &chain) :
  surface(surface),
  source(source),
  target(target),
  chain(chain) {
  ASSERT_ARGUMENT(chain, "saddle connection cannot be trivial");

  normalize();
}

template <typename Surface>
void Implementation<SaddleConnection<Surface>>::normalize() {
  const auto normalize = [&](HalfEdge& sector, const typename Surface::Vector& vector) {
    while (surface->fromEdge(sector).ccw(vector) == CCW::COUNTERCLOCKWISE)
      sector = surface->nextAtVertex(sector);
    while (surface->fromEdge(sector).ccw(vector) == CCW::CLOCKWISE)
      sector = surface->previousAtVertex(sector);
  };

  const auto vector = static_cast<typename Surface::Vector>(chain);
  normalize(this->source, vector);
  normalize(this->target, -vector);
}


template <typename Surface>
void Implementation<SaddleConnection<Surface>>::check(const SaddleConnection& connection) {
  // Run checks in constructor
  assert([&]() {
    SaddleConnection copy = connection;
    return true;
  }());

  // Run expensive checks in crossings()
  assert([&]() {
    connection.crossings();
    return true;
  }());
}

}  // namespace flatsurf

namespace std {

using namespace flatsurf;

template <typename Surface>
size_t hash<SaddleConnection<Surface>>::operator()(const SaddleConnection<Surface>& self) const noexcept {
  return hash_combine(self.source(), self.target(), static_cast<Chain<Surface>>(self));
}

}

// Instantiations of templates so implementations are generated for the linker
#include "util/instantiate.ipp"

LIBFLATSURF_INSTANTIATE_MANY_WRAPPED((LIBFLATSURF_INSTANTIATE_WITH_IMPLEMENTATION)(LIBFLATSURF_INSTANTIATE_HASH), SaddleConnection, LIBFLATSURF_FLAT_TRIANGULATION_TYPES)
