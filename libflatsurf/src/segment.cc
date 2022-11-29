/**********************************************************************
 *  This file is part of flatsurf.
 *
 *        Copyright (C) 2022 Julian Rüth
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

#include "../flatsurf/segment.hpp"

#include "../flatsurf/saddle_connection.hpp"
#include "../flatsurf/vertex.hpp"
#include "../flatsurf/edge.hpp"
#include "../flatsurf/ccw.hpp"
#include "../flatsurf/orientation.hpp"
#include "../flatsurf/ray.hpp"
#include "../flatsurf/vertical.hpp"

#include "impl/segment.impl.hpp"
#include "impl/point.impl.hpp"
#include "impl/ray.impl.hpp"
#include "impl/flat_triangulation.impl.hpp"

#include "util/instantiate.ipp"
#include "util/assert.ipp"
#include "util/hash.ipp"

namespace flatsurf {

template <typename Surface>
Segment<Surface>::Segment(const Point<Surface>& start, const Vector<T>& vector) {
  LIBFLATSURF_CHECK_ARGUMENT(start.surface().angle(start) == 1, "cannot create Segment from starting point and vector since starting point is a singularity");

  HalfEdge source = start.face();

  Point<Surface> end = start;
  const HalfEdge target = start.vertex() ? ImplementationOf<Point<Surface>>::translate(end, source, vector) : ImplementationOf<Point<Surface>>::translate(end, vector);
  
  *this = Segment{start, source, end, target, vector};
}

template <typename Surface>
Segment<Surface>::Segment(const Point<Surface>& start, const Point<Surface>& end, const Vector<T>& vector) {
  LIBFLATSURF_CHECK_ARGUMENT(ImplementationOf<Surface>::identical(start.surface(), end.surface()), "start and end must be defined on the same surface");

  if (start.surface().angle(start) > 1) {
    LIBFLATSURF_CHECK_ARGUMENT(!end.vertex() || end.surface().angle(end) == 1, "cannot create Segment from points and vector if both points are singularities");

    // When the starting point of the segment is a singularity we find out what
    // the source half edge is by retracing -segment (which might be slow.)
    Segment s{end, start, -vector};
    s = -s;

    *this = std::move(s);
    return;
  }

  if (end.surface().angle(end) > 1) {
    // Since the endpoint of the segment is a singularity, we need to
    // retrace the segment in the surface (which might be very slow.)
    *this = Segment{start, Ray{start, vector}.source(), vector};
    return;
  }

  *this = Segment{start, Ray{start, vector}.source(), end, Ray{end, -vector}.source(), vector};
}

template <typename Surface>
Segment<Surface>::Segment(const Point<Surface>& start, HalfEdge source, const Vector<T>& vector) {
  LIBFLATSURF_CHECK_ARGUMENT(start.in(source), "start point of segment must be in source face");

  if (!start.vertex()) {
    // Source can be ignored if the starting point is not a vertex.
    *this = Segment{start, vector};
  } else {
    LIBFLATSURF_CHECK_ARGUMENT(start.surface().inSector(source, vector), "vector defining translation of segment must be in the source sector but " << vector << " is not in " << source << " of " << start.surface() << " for translation of vertex " << start);

    Point<Surface> end = start;
    const HalfEdge target = ImplementationOf<Point<Surface>>::translate(end, source, vector);
    
    *this = Segment{start, source, end, target, vector};
  }

}

template <typename Surface>
Segment<Surface>::Segment(const Point<Surface>& start, HalfEdge source, const Point<Surface>& end, HalfEdge target, const Vector<T>& vector) :
  self(spimpl::make_impl<ImplementationOf<Segment>>(source, start, target, end, vector)) {
  LIBFLATSURF_CHECK_ARGUMENT(ImplementationOf<Surface>::identical(start.surface(), end.surface()), "start and end must be defined on the same surface");
  LIBFLATSURF_CHECK_ARGUMENT(start.in(source), "start point of segment must be in source face");
  LIBFLATSURF_CHECK_ARGUMENT(end.in(target), "end point of segment must be in target face");
  LIBFLATSURF_CHECK_ARGUMENT(vector, "vector defining segment must not be trivial");

  self->normalize();
}

template <typename Surface>
const Point<Surface>& Segment<Surface>::start() const {
  return self->start;
}

template <typename Surface>
const Point<Surface>& Segment<Surface>::end() const {
  return self->end;
}

template <typename Surface>
HalfEdge Segment<Surface>::source() const {
  return self->source;
}

template <typename Surface>
HalfEdge Segment<Surface>::target() const {
  return self->target;
}

template <typename Surface>
const FlatTriangulation<typename Surface::Coordinate>& Segment<Surface>::surface() const {
  return *self->surface;
}

template <typename Surface>
const Vector<typename Surface::Coordinate>& Segment<Surface>::vector() const {
  return self->vector;
}

template <typename Surface>
bool Segment<Surface>::overlapping() const {
  return static_cast<Ray<Surface>>(*this).segment(end()) != *this;
}

template <typename Surface>
std::optional<SaddleConnection<Surface>> Segment<Surface>::saddleConnection() const {
  if (!start().vertex())
    return std::nullopt;

  if (!end().vertex())
    return std::nullopt;

  const auto saddleConnection = SaddleConnection<Surface>::inSector(*self->surface, self->source, Vertical(*self->surface, self->vector));

  if (saddleConnection.vector() != self->vector) {
    // There is a shorter saddle connection in this direction. The segment crosses a marked point.
    LIBFLATSURF_ASSERT(self->vector.orientation(self->vector - saddleConnection.vector()) == ORIENTATION::SAME, "segment from " << self->start << " ends after " << self->vector << " at vertex " << self->end << " but we only found a longer saddle connection " << saddleConnection << " in the same direction");
    LIBFLATSURF_ASSERT(self->surface->angle(saddleConnection.end()) == 1, "segment cannot pass through non-marked point but " << saddleConnection.end() << " which is on the segment is a singularity");

    return std::nullopt;
  }

  return saddleConnection;
}

template <typename Surface>
Segment<Surface>::operator Ray<Surface>() const {
  return ray();
}

template <typename Surface>
Ray<Surface> Segment<Surface>::ray() const {
  return Ray{self->start, self->source, self->vector};
}

template <typename Surface>
Segment<Surface> Segment<Surface>::operator-() const {
  return Segment{self->end, self->target, self->start, self->source, -self->vector};
}

template <typename Surface>
bool Segment<Surface>::operator==(const Segment<Surface>& other) const {
  if (surface() != other.surface() || start() != other.start() || end() != other.end() || vector() != other.vector())
    return false;

  if (start().vertex() && source() != other.source())
    return false;

  if (end().vertex() && target() != other.target())
    return false;

  return true;
}

template <typename Surface>
std::ostream &operator<<(std::ostream& os, const Segment<Surface>& self) {
  const auto saddleConnection = self.saddleConnection();
  if (saddleConnection)
    return os << *saddleConnection;

  os << self.vector() << " from ";

  if (self.start().vertex())
    os << self.source();
  else
    os << self.start();

  os << " to ";

  if (self.end().vertex())
    os << self.target();
  else
    os << self.end();

  return os;
}

template <typename Surface>
ImplementationOf<Segment<Surface>>::ImplementationOf(HalfEdge source, const Point<Surface>& start, HalfEdge target, const Point<Surface>& end, const Vector<T>& vector) : surface(start.surface()), source(source), start(start), target(target), end(end), vector(vector) {}

template <typename Surface>
void ImplementationOf<Segment<Surface>>::normalize() {
  source = ImplementationOf<Ray<Surface>>::normalizeSource(start, source, vector);
  target = ImplementationOf<Ray<Surface>>::normalizeSource(end, target, -vector);
}

}

namespace std {

using namespace flatsurf;

template <typename Surface>
size_t hash<Segment<Surface>>::operator()(const Segment<Surface>& self) const {
  // We do not hash source & target because they are not unique for segments starting in the interior of a face.
  return ::flatsurf::hash_combine(self.start(), self.end(), self.vector());
}

}  // namespace std

// Instantiations of templates so implementations are generated for the linker
#include "util/instantiate.ipp"

LIBFLATSURF_INSTANTIATE_MANY_WRAPPED((LIBFLATSURF_INSTANTIATE_WITH_IMPLEMENTATION)(LIBFLATSURF_INSTANTIATE_HASH), Segment, LIBFLATSURF_FLAT_TRIANGULATION_TYPES)
