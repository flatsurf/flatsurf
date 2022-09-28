/**********************************************************************
 *  This file is part of flatsurf.
 *
 *        Copyright (C) 2019-2022 Julian Rüth
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

#include <climits>
#include <ostream>

#include "../flatsurf/bound.hpp"
#include "../flatsurf/ccw.hpp"
#include "../flatsurf/flat_triangulation.hpp"
#include "../flatsurf/half_edge_intersection.hpp"
#include "../flatsurf/half_edge_map.hpp"
#include "../flatsurf/orientation.hpp"
#include "../flatsurf/point.hpp"
#include "../flatsurf/ray.hpp"
#include "../flatsurf/saddle_connections.hpp"
#include "../flatsurf/saddle_connections_by_length.hpp"
#include "../flatsurf/saddle_connections_by_length_iterator.hpp"
#include "../flatsurf/saddle_connections_iterator.hpp"
#include "../flatsurf/segment.hpp"
#include "../flatsurf/vector.hpp"
#include "../flatsurf/vertex.hpp"
#include "../flatsurf/vertical.hpp"
#include "impl/half_edge_intersection.impl.hpp"
#include "impl/saddle_connection.impl.hpp"
#include "util/assert.ipp"
#include "util/hash.ipp"

namespace flatsurf {

template <typename Surface>
SaddleConnection<Surface>::SaddleConnection(const Surface& surface, HalfEdge e) :
  self(spimpl::make_impl<ImplementationOf<SaddleConnection>>(surface, e, -e, Chain(surface, e))) {
}

template <typename Surface>
SaddleConnection<Surface>::SaddleConnection(const Surface& surface, HalfEdge source, HalfEdge target, const Chain<Surface>& chain) :
  self(spimpl::make_impl<ImplementationOf<SaddleConnection>>(surface, source, target, chain)) {
  LIBFLATSURF_ASSERT_ARGUMENT(self->chain, "saddle connection cannot be trivial");
}

template <typename Surface>
SaddleConnection<Surface>::SaddleConnection(const Surface& surface, HalfEdge source, HalfEdge target, Chain<Surface>&& chain) :
  self(spimpl::make_impl<ImplementationOf<SaddleConnection>>(surface, source, target, std::move(chain))) {
  LIBFLATSURF_ASSERT_ARGUMENT(self->chain, "saddle connection cannot be trivial");
}

template <typename Surface>
bool SaddleConnection<Surface>::operator==(const SaddleConnection<Surface>& rhs) const {
  bool ret = *self->surface == *rhs.self->surface && vector() == rhs.vector() && self->source == rhs.self->source;

  LIBFLATSURF_ASSERT((!ret || target() == rhs.target()), "saddle connection data is inconsistent, " << *this << " == " << rhs << " but their targets do not match since " << target() << " != " << rhs.target() << " in " << *self->surface);
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
Vertex SaddleConnection<Surface>::start() const {
  return Vertex::source(self->source, *self->surface);
}

template <typename Surface>
Vertex SaddleConnection<Surface>::end() const {
  return Vertex::source(self->target, *self->surface);
}

template <typename Surface>
const Surface& SaddleConnection<Surface>::surface() const {
  return self->surface;
}

template <typename Surface>
Ray<Surface> SaddleConnection<Surface>::ray() const {
  return Ray<Surface>(self->surface, self->source, *this);
}

template <typename Surface>
SaddleConnection<Surface> SaddleConnection<Surface>::operator-() const {
  return SaddleConnection(self->surface, self->target, self->source, -self->chain);
}

template <typename Surface>
SaddleConnection<Surface> SaddleConnection<Surface>::inSector(const Surface& surface, HalfEdge source, const Vector<T>& vector) {
  LIBFLATSURF_CHECK_ARGUMENT(surface.inSector(source, vector), "Cannot search for " << vector << " next to " << source << " in " << surface << "; that direction is not in the search sector");

  const auto construction = SaddleConnections<Surface>(surface)
                                .bound(Bound::upper(vector))
                                .sector(source)
                                .sector(vector, vector);

  const auto ret = std::begin(construction);

  LIBFLATSURF_CHECK_ARGUMENT(ret != std::end(construction), "No connection with vector " << vector << " in sector starting at " << source << " in " << surface);

  return *ret;
}

template <typename Surface>
SaddleConnection<Surface> SaddleConnection<Surface>::inSector(const Surface& surface, HalfEdge source, const Vertical<Surface>& direction) {
  LIBFLATSURF_CHECK_ARGUMENT(surface.inSector(source, direction), "Cannot search in direction " << direction << " next to " << source << " in " << surface << "; that direction is not in the search sector");

  const auto construction = SaddleConnections<Surface>(surface)
                                .byLength()
                                .sector(source)
                                .sector(direction, direction);

  const auto ret = std::begin(construction);

  return *ret;
}

template <typename Surface>
SaddleConnection<Surface> SaddleConnection<Surface>::inHalfPlane(const Surface& surface, HalfEdge side, const Vertical<Surface>& vertical, const Vector<T>& vector) {
  return inSector(surface, surface.sector(side, vertical, vector), vector);
}

template <typename Surface>
SaddleConnection<Surface> SaddleConnection<Surface>::inPlane(const Surface& surface, HalfEdge plane, const Vertical<Surface>& direction) {
  return SaddleConnection::inSector(surface, surface.sector(plane, direction), direction);
}

template <typename Surface>
SaddleConnection<Surface> SaddleConnection<Surface>::inPlane(const Surface& surface, HalfEdge plane, const Vector<T>& vector) {
  return SaddleConnection::inSector(surface, surface.sector(plane, vector), vector);
}

template <typename Surface>
SaddleConnection<Surface> SaddleConnection<Surface>::inPlane(const Surface& surface, HalfEdge sourcePlane, HalfEdge targetPlane, const Chain<Surface>& chain) {
  const auto& vector = static_cast<const Vector<T>&>(chain);

  return SaddleConnection(surface, surface.sector(sourcePlane, vector), surface.sector(targetPlane, -vector), chain);
}

template <typename Surface>
SaddleConnection<Surface> SaddleConnection<Surface>::clockwise(const SaddleConnection<Surface>& clockwiseFrom, const Vector<T>& vector) {
  return inSector(clockwiseFrom.surface(), clockwiseFrom.surface().sector(clockwiseFrom.source(), clockwiseFrom, CCW::CLOCKWISE, vector, true), vector);
}

template <typename Surface>
SaddleConnection<Surface> SaddleConnection<Surface>::counterclockwise(const Surface& surface, HalfEdge source, HalfEdge target, const Chain<Surface>& chain) {
  const Vector<T> vector = chain;

  return SaddleConnection(surface, surface.sector(source, CCW::COUNTERCLOCKWISE, vector), surface.sector(target, CCW::COUNTERCLOCKWISE, -vector), chain);
}

template <typename Surface>
SaddleConnection<Surface> SaddleConnection<Surface>::counterclockwise(const Surface& surface, const SaddleConnection<Surface>& source, const Vertical<Surface>& direction) {
  return inSector(surface, surface.sector(source.source(), source.vector(), CCW::COUNTERCLOCKWISE, direction), direction);
}

template <typename Surface>
SaddleConnection<Surface> SaddleConnection<Surface>::alongVertical(const Surface& surface, const Vertical<Surface>& direction, HalfEdge plane) {
  LIBFLATSURF_CHECK_ARGUMENT(surface.inHalfPlane(plane, direction), "direction must be in the half plane singled out by half edge");

  return inSector(surface, surface.sector(plane, direction), direction);
}

template <typename Surface>
int SaddleConnection<Surface>::angle(HalfEdge source, const Vector<T>& vector) const {
  return ray().angle(Ray{surface(), source, vector});
}

template <typename Surface>
int SaddleConnection<Surface>::angle(const SaddleConnection<Surface>& rhs) const {
  LIBFLATSURF_CHECK_ARGUMENT(surface() == rhs.surface(), "Cannot determine angle between " << *this << " and " << rhs << " since they live in different surfaces, i.e., " << surface() << " and " << rhs.surface() << " respectively.");

  return angle(rhs.source(), rhs.vector());
}

template <typename Surface>
CCW SaddleConnection<Surface>::ccw(HalfEdge source, const Vector<T>& vector) const {
  return ray().ccw(Ray{*self->surface, source, vector});
}

template <typename Surface>
CCW SaddleConnection<Surface>::ccw(const SaddleConnection<Surface>& rhs) const {
  LIBFLATSURF_CHECK_ARGUMENT(surface() == rhs.surface(), "Cannot determine turning direction between " << *this << " and " << rhs << " since they live in different surfaces, i.e., " << surface() << " and " << rhs.surface() << " respectively.");

  return ccw(rhs.source(), rhs.vector());
}

template <typename Surface>
std::vector<HalfEdge> SaddleConnection<Surface>::crossings() const {
  std::vector<HalfEdge> ret;

  for (auto& intersection : this->path())
    ret.emplace_back(intersection.halfEdge());

  return ret;
}

template <typename Surface>
std::vector<HalfEdgeIntersection<Surface>> SaddleConnection<Surface>::path() const {
  std::vector<HalfEdgeIntersection<Surface>> path;

  // We reconstruct the sequence of half edges that this saddle connection
  // crossed. This is expensive (but cheap in terms of the output size.)
  auto reconstruction = SaddleConnections<Surface>(self->surface).sector(source());
  auto it = reconstruction.begin();

  while (*it != *this) {
    const auto ccw = it->vector().ccw(vector());
    LIBFLATSURF_ASSERT(ccw != CCW::COLLINEAR, "There cannot be another saddle connection in exactly the same direction as this one but in " << self->surface << " at " << source() << " we found " << it->vector() << " which has the same direction as " << vector());
    it.skipSector(-ccw);
    while (true) {
      auto intersection = it.incrementWithIntersections();
      if (!intersection.has_value())
        break;

      path.emplace_back(ImplementationOf<HalfEdgeIntersection<Surface>>::make(*self->surface, -intersection->first, intersection->second, *this));
    }
  }

  LIBFLATSURF_ASSERT(it->target() == target(), "We reconstructed the saddle connection in " << self->surface << " starting from " << source() << " with vector " << vector() << " but it does not end at " << target() << " as claimed but at " << it->target());

  return path;
}

template <typename Surface>
const Vector<typename Surface::Coordinate>& SaddleConnection<Surface>::vector() const {
  return static_cast<const Vector<T>&>(chain());
}

template <typename Surface>
SaddleConnection<Surface>::operator const Vector<typename Surface::Coordinate> &() const {
  return vector();
}

template <typename Surface>
Segment<Surface> SaddleConnection<Surface>::segment() const {
  const auto segment = Segment(
      Point<Surface>{*self->surface, Vertex::source(self->source, *self->surface)},
      self->source,
      Point<Surface>{*self->surface, Vertex::source(self->target, *self->surface)},
      self->target,
      *this);

  LIBFLATSURF_ASSERT(segment.source() == self->source && segment.target() == self->target, "conversion from SaddleConnection to Segment produced incompatible source & target half edges");

  return segment;
}

template <typename Surface>
SaddleConnection<Surface>::operator Segment<Surface>() const {
  return segment();
}

template <typename Surface>
const Chain<Surface>& SaddleConnection<Surface>::chain() const {
  return self->chain;
}

template <typename Surface>
SaddleConnection<Surface>::operator const Chain<Surface> &() const {
  return chain();
}

template <typename Surface>
SaddleConnection<Surface>::operator Ray<Surface>() const {
  throw std::logic_error("not implemented: SaddleConnection::operator Ray");
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
  if (self.source() == -self.target() && self.vector() == self.surface().fromHalfEdge(self.source()))
    return os << self.source();
  else
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
  return hash_combine(self.source(), self.target(), self.chain().vector());
}

}  // namespace std

// Instantiations of templates so implementations are generated for the linker
#include "util/instantiate.ipp"

LIBFLATSURF_INSTANTIATE_MANY_WRAPPED((LIBFLATSURF_INSTANTIATE_WITH_IMPLEMENTATION)(LIBFLATSURF_INSTANTIATE_HASH), SaddleConnection, LIBFLATSURF_FLAT_TRIANGULATION_TYPES)
