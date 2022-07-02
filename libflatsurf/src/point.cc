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

#include <ostream>

#include "../flatsurf/point.hpp"
#include "../flatsurf/edge.hpp"
#include "../flatsurf/vertex.hpp"
#include "../flatsurf/half_edge.hpp"
#include "../flatsurf/half_edge_set.hpp"
#include "../flatsurf/half_edge_set_iterator.hpp"
#include "util/assert.ipp"
#include "impl/point.impl.hpp"

namespace flatsurf {

template <typename Surface>
Point<Surface>::Point(const Surface& surface, const Vertex& vertex) : Point(surface, *vertex.outgoing().begin(), T(1), T(), T()) {}

template <typename Surface>
Point<Surface>::Point(const Surface& surface, HalfEdge face, const T& a, const T& b, const T& c) : self(spimpl::make_impl<ImplementationOf<Point>>(surface, face, a, b, c)) {
}

template <typename Surface>
Point<Surface>::Point(const Surface& surface, HalfEdge face, const std::tuple<T, T, T>& coordinates) : Point(surface, face, std::get<0>(coordinates), std::get<1>(coordinates), std::get<2>(coordinates)) {
}

template <typename Surface>
std::tuple<typename Surface::Coordinate, typename Surface::Coordinate, typename Surface::Coordinate> Point<Surface>::coordinates(HalfEdge face) const {
  if (face == self->face)
    return std::tuple{self->a, self->b, self->c};

  throw std::logic_error("not implemented: coordinates()");
}

template <typename Surface>
HalfEdge Point<Surface>::face() const {
  return self->face;
}

template <typename Surface>
std::optional<Vertex> Point<Surface>::vertex() const {
  if (self->a) {
    if (self->b || self->c)
      return std::nullopt;

    return Vertex::source(self->face, *self->surface);
  } else if (self->b) {
    if (self->c)
      return std::nullopt;

    return Vertex::target(self->face, *self->surface);
  } else if (self->c) {
    return Vertex::source(self->surface->previousInFace(self->face), *self->surface);
  }

  LIBFLATSURF_UNREACHABLE("point with barycentric coordinates (0, 0, 0)");
}

template <typename Surface>
std::optional<Edge> Point<Surface>::edge() const {
  if (!self->a)
    return self->surface->nextInFace(self->face);
  else if (!self->b)
    return self->surface->previousInFace(self->face);
  else if (!self->c)
    return self->face;

  return std::nullopt;
}

template <typename Surface>
bool Point<Surface>::in(HalfEdge face) const {
  if (face == self->face || face == self->surface->nextInFace(face) || face == self->surface->previousInFace(face))
    return true;

  throw std::logic_error("not implemented: in()");
}

template <typename Surface>
bool Point<Surface>::on(Edge edge) const {
  const auto vertex = this->vertex();
  if (vertex)
    return *vertex == Vertex::source(edge.positive(), *self->surface) || *vertex == Vertex::source(edge.negative(), *self->surface);

  return this->edge() == edge;
}

template <typename Surface>
bool Point<Surface>::at(const Vertex& vertex) const {
  return this->vertex() == vertex;
}

template <typename Surface>
ImplementationOf<Point<Surface>>::ImplementationOf(const Surface& surface, HalfEdge face, T a, T b, T c) : surface(surface), face(face), a(std::move(a)), b(std::move(b)), c(std::move(c)) {
  normalize();
}

template <typename Surface>
void ImplementationOf<Point<Surface>>::normalize() {
  if (!a && !b && !c)
    throw std::invalid_argument("cannot create point from coordinates (0, 0, 0)");

  if (a < 0 || b < 0 || c < 0)
    throw std::logic_error("not implemented: normalize()");
}

template <typename Surface>
std::ostream& operator<<(std::ostream& os, const Point<Surface>& point) {
  return os << "(" << point.self->a << ", " << point.self->b << ", " << point.self->c << ") in " << "(" << point.self->face << ", " << point.self->surface->nextInFace(point.self->face) << ", " << point.self->surface->previousInFace(point.self->face) << ")";
}

}


// Instantiations of templates so implementations are generated for the linker
#include "util/instantiate.ipp"

LIBFLATSURF_INSTANTIATE_MANY_WRAPPED((LIBFLATSURF_INSTANTIATE_WITH_IMPLEMENTATION)(LIBFLATSURF_INSTANTIATE_HASH), Point, LIBFLATSURF_FLAT_TRIANGULATION_TYPES)
