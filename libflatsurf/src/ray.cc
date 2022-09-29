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

#include "../flatsurf/ray.hpp"

#include "impl/ray.impl.hpp"

#include "../flatsurf/ccw.hpp"
#include "../flatsurf/deformation.hpp"
#include "../flatsurf/edge.hpp"
#include "../flatsurf/half_edge.hpp"
#include "../flatsurf/half_edge_set.hpp"
#include "../flatsurf/half_edge_set_iterator.hpp"
#include "../flatsurf/point.hpp"
#include "../flatsurf/saddle_connection.hpp"
#include "../flatsurf/path.hpp"
#include "../flatsurf/segment.hpp"
#include "../flatsurf/vector.hpp"
#include "../flatsurf/vertex.hpp"
#include "../flatsurf/vertical.hpp"

#include "util/assert.ipp"

namespace flatsurf {

namespace {

// Return a source half edge which can be used to describe vector emenating from p.
template <typename Surface>
HalfEdge sector(const Point<Surface>& p, const Vector<typename Surface::Coordinate>& vector) {
  if (p.vertex()) {
    const auto& surface = p.surface();

    for (const auto outgoing : p.vertex()->outgoing())
      if (surface.inSector(outgoing, vector))
        return outgoing;

    LIBFLATSURF_ASSERT(false, "no sector surrounding " << p << " contains " << vector);
  }
  return p.face();
}

// Return the angle turning from sourceVector next to source to targetVector next target.
// Namely, if the angle is α to turn in counterclockwise direction, return α/π⌋.
template <typename Surface>
int angle(const Surface& surface, HalfEdge source, const Vector<typename Surface::Coordinate>& sourceVector, HalfEdge target, const Vector<typename Surface::Coordinate>& targetVector) {
  LIBFLATSURF_CHECK_ARGUMENT(Vertex::source(source, surface) == Vertex::source(target, surface), "Cannot determine angle between vector starting at " << source << " and a vector starting at " << target << " since they do not start at the same vertex.");

  using T = typename Surface::Coordinate;

  HalfEdge sector = source;
  Vector<T> turned = sourceVector;

  int angle = 0;

  CCW sgn = CCW::COUNTERCLOCKWISE;

  while (sector != target || turned.ccw(targetVector) != CCW::COLLINEAR) {
    if (sector == target && turned.ccw(targetVector) != CCW::CLOCKWISE) {
      turned = targetVector;
    } else {
      sector = surface.nextAtVertex(sector);
      turned = surface.fromHalfEdge(sector);
    }

    const auto ccw = sourceVector.ccw(turned);

    if (ccw == CCW::COLLINEAR || ccw != sgn) {
      angle++;
      sgn = -sgn;
    }
  }

  LIBFLATSURF_ASSERT(angle < 2 * surface.angle(Vertex::source(source, surface)), "Turn angle must be smaller than the full vertex angle but the turn from " << source << ", " << sourceVector << " to " << target << ", " << targetVector << " in " << surface << " is " << angle << "/2");

  return angle;
}

}  // namespace

template <typename Surface>
Ray<Surface>::Ray(const Point<Surface>& start, const Vector<T>& vector) {
  LIBFLATSURF_CHECK_ARGUMENT(start.surface().angle(start) == 1, "cannot create Ray from starting point and vector since starting point is a singularity");

  *this = Ray{start, sector(start, vector), vector};
}

template <typename Surface>
Ray<Surface>::Ray(const Point<Surface>& start, HalfEdge source, const Vector<T>& vector) :
  self(spimpl::make_impl<ImplementationOf<Ray>>(start, source, vector)) {
  LIBFLATSURF_CHECK_ARGUMENT(start.in(source), "start point of ray must be in source face");

  self->normalize();
}

template <typename Surface>
Ray<Surface>::Ray(const Surface& surface, HalfEdge source, const Vector<T>& vector) :
  Ray(Point<Surface>{surface, Vertex::source(source, surface)}, source, vector) {}

template <typename Surface>
Ray<Surface>::Ray(const Surface& surface, HalfEdge vector) :
  Ray(surface, vector, surface.fromHalfEdge(vector)) {}

template <typename Surface>
const Point<Surface>& Ray<Surface>::start() const {
  return self->start;
}

template <typename Surface>
HalfEdge Ray<Surface>::source() const {
  return self->source;
}

template <typename Surface>
Ray<Surface>::operator const Vector<T> &() const {
  return self->vector;
}

template <typename Surface>
const Vector<typename Surface::Coordinate>& Ray<Surface>::vector() const {
  return self->vector;
}

template <typename Surface>
Ray<Surface>::operator Vertical<Surface>() const {
  return Vertical{this->surface(), this->vector()};
}

template <typename Surface>
SaddleConnection<Surface> Ray<Surface>::saddleConnection() const {
  return SaddleConnection<Surface>::inSector(this->surface(), self->source, *this);
}

template <typename Surface>
int Ray<Surface>::angle(const Ray& other) const {
  return flatsurf::angle(this->surface(), this->source(), this->vector(), other.source(), other.vector()) / 2;
}

template <typename Surface>
CCW Ray<Surface>::ccw(const Ray& other) const {
  LIBFLATSURF_CHECK_ARGUMENT(start() == other.start(), "can only compute ccw() for rays starting from the same point");

  const int angle = flatsurf::angle(this->surface(), this->source(), this->vector(), other.source(), other.vector());

  if (angle == 0) {
    // The angle between the rays is in [0, π).
    const auto ccw = this->vector().ccw(other.vector());
    LIBFLATSURF_ASSERT(ccw == CCW::COLLINEAR || ccw == CCW::COUNTERCLOCKWISE, "rays that enclose an angle <π must be collinear or counterclockwise");
    return ccw;
  }

  const int totalAngle = this->surface().angle(Vertex::source(this->source(), this->surface()));

  // Let k·2π be the total angle at the vertex.

  if (angle < totalAngle)
    // The angle between the rays is less than k·π.
    return CCW::COUNTERCLOCKWISE;
  
  if (angle > totalAngle)
    // The angle between the rays is more than k·π.
    return CCW::CLOCKWISE;

  // The angle between the rays is in [k·π, (k + 1)·π). If it's exactly k·π, then the rays are collinear.
  if (this->vector().ccw(other.vector()) == CCW::COLLINEAR)
    return CCW::COLLINEAR;

  return CCW::CLOCKWISE;
}

template <typename Surface>
bool Ray<Surface>::operator==(const Ray&) const {
  throw std::logic_error("not implemented: operator==()");
}

template <typename Surface>
const Surface& Ray<Surface>::surface() const {
  return self->start.surface();
}

template <typename Surface>
Segment<Surface> Ray<Surface>::segment(const Point<Surface>& end) const {
  const auto segmentAfterInsertion = [&](const auto& insertion) {
    const auto segment = insertion(*this).segment(insertion(end));

    const auto preimage = insertion.section()(Path{segment});

    LIBFLATSURF_ASSERT(preimage, "preimage of a segment after insertion of a point must exist");

    const auto simplified = preimage->segment();

    LIBFLATSURF_ASSERT(simplified, "preimage of segment after insertion must be a segment");

    return *simplified;
  };

  if (!start().vertex())
    return segmentAfterInsertion(surface().insert(start()));

  if (!end.vertex())
    return segmentAfterInsertion(surface().insert(end));

  Path<Surface> segment;

  do {
    const auto partial = SaddleConnection<Surface>::inSector(surface(), source(), *this).segment();
    segment.push_back(partial);
    if (partial.end() == end)
      return *segment.segment();

    LIBFLATSURF_CHECK_ARGUMENT(surface().angle(partial.end()) == 1, "endpoint not reachable along this ray");
  }while(true);
}

template <typename Surface>
ImplementationOf<Ray<Surface>>::ImplementationOf(const Point<Surface>& start, HalfEdge source, const Vector<T>& vector) :
  start(start),
  source(source),
  vector(vector) {}

template <typename Surface>
void ImplementationOf<Ray<Surface>>::normalize() {
  source = normalizeSource(start, source, vector);
}

template <typename Surface>
HalfEdge ImplementationOf<Ray<Surface>>::normalizeSource(const Point<Surface>& start, HalfEdge source, const Vector<T>& vector) {
  const auto normalizeSourceAtVertex = [](const auto& surface, auto& source, const auto& vector) {
    while(true) {
      if (surface.inSector(source, vector))
        break;
      if (surface.fromHalfEdge(surface.nextAtVertex(source)).parallel(vector)) {
        source = surface.nextAtVertex(source);
        break;
      }

      source = surface.nextInFace(source);
    }
  };

  const auto normalizeSourceAtEdge = [](const auto& surface, auto& source, const auto& vector) {
    if (surface.fromHalfEdge(source).ccw(vector) == CCW::COUNTERCLOCKWISE || surface.fromHalfEdge(source).parallel(vector))
      return;

    source = -source;
  };

  if (start.vertex())
    normalizeSourceAtVertex(start.surface(), source, vector);
  else if (start.edge())
    normalizeSourceAtEdge(start.surface(), source, vector);
  else
    source = start.face();

  return source;
}

template <typename Surface>
std::ostream &operator<<(std::ostream &, const Ray<Surface> &) {
  throw std::logic_error("not implemented: printing of rays");
}

}

namespace std {

using namespace flatsurf;

template <typename Surface>
size_t hash<Ray<Surface>>::operator()(const Ray<Surface>&) const {
  throw std::logic_error("not implemented: hashing of rays");
}

}

// Instantiations of templates so implementations are generated for the linker
#include "util/instantiate.ipp"

LIBFLATSURF_INSTANTIATE_MANY_WRAPPED((LIBFLATSURF_INSTANTIATE_WITH_IMPLEMENTATION)(LIBFLATSURF_INSTANTIATE_HASH), Ray, LIBFLATSURF_FLAT_TRIANGULATION_TYPES)
