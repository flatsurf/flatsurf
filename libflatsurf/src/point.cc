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
#include <stdexcept>

#include "../flatsurf/point.hpp"
#include "../flatsurf/edge.hpp"
#include "../flatsurf/vertex.hpp"
#include "../flatsurf/half_edge.hpp"
#include "../flatsurf/half_edge_set.hpp"
#include "../flatsurf/half_edge_set_iterator.hpp"
#include "../flatsurf/vector.hpp"
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
  HalfEdge boundary[] = { face, self->surface->nextInFace(face), self->surface->previousInFace(face) };

  for (const auto& b : boundary)
    if (self->face == b)
      return true;

  for (const auto& b : boundary)
    if (this->on(b))
      return true;

  return false;
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

  if (a + b + c < 0) {
    throw std::invalid_argument("sum of barycentric coordinates must not be negative");
  }

  if (a < 0 || b < 0 || c < 0) {
    if ((a < 0) + (b < 0) + (c < 0) >= 2)
      throw std::invalid_argument("at most one barycentric coordinate can be negative");

    // Normalize coordinates such that c is negative.
    if (a < 0) {
      // Rotate the coordinates such that they are (b c a)
      std::tie(a, b, c) = {b, c, a};
      face = surface->nextInFace(face);
    } else if (b < 0) {
      // Rotate the coordinates such that they are (c a b)
      std::tie(a, b, c) = {c, a, b};
      face = surface->previousInFace(face);
    }

    LIBFLATSURF_ASSERT(a >= 0 && b >= 0 && c < 0, "Barycentric coordinates are not normalized correctly");

    // Let D be the vertex opposite to -face.
    // We have to write the point with coordinates in -face, i.e., (A, D, B).
    // Let us first write C with coordinates in (A, D, B).
    // If we pretend that D is the origin, we can write C = a_c A + b_c B + d_c D with an arbitrary d_c.
    // For any λ if we set d_c such that λ a_c + λ b_c + d_c = λ, this gives
    // barycentric coordinates for C. (Classically, one would pick λ=1 but
    // since we want to avoid divisions, we are going to collect the divisors
    // into the λ.)
    // To determine a_c and b_c we solve the system:
    // / B_x A_x \  / b_c \   / C_x \
    // |         |  |     | = |     |
    // \ B_y A_y /  \ a_c /   \ C_y /
    const auto A = -surface->fromHalfEdge(surface->nextInFace(-face));
    const auto B = surface->fromHalfEdge(surface->previousInFace(-face));
    const auto C = B + surface->fromHalfEdge(surface->nextInFace(face));
    const T det = B.x() * A.y() - B.y() * A.x();

    const T λb_c =  A.y() * C.x() - A.x() * C.y();
    const T λa_c = -B.y() * C.x() + B.x() * C.y();
    const T d_c = det - λa_c - λb_c;

    // Now we can rewrite this point in terms of the other face:
    // P = (aA + bB + cC) / (a + b + c)
    //   = (aA + bB + c(λ a_c A + λ b_c B + d_c D) / λ) / (a + b + c)
    // So in (denormalized) barycentric coordinates with respect to (A, D, B) we get:
    // P = (λ a + c λ a_c, c d_c , λ b + c λ b_c)
    std::tie(a, b, c) = std::tuple<T, T, T>{
      det * a + c * λa_c,
      c * d_c,
      det * b + c * λb_c,
    };

    // Recursively normalize in the new face. This could imply a lengthy walk
    // on the surface if the point was way outside of the original face.
    normalize();
  }
}

template <typename Surface>
std::ostream& operator<<(std::ostream& os, const Point<Surface>& point) {
  return os << "(" << point.self->a << ", " << point.self->b << ", " << point.self->c << ") in " << "(" << point.self->face << ", " << point.self->surface->nextInFace(point.self->face) << ", " << point.self->surface->previousInFace(point.self->face) << ")";
}

}


// Instantiations of templates so implementations are generated for the linker
#include "util/instantiate.ipp"

LIBFLATSURF_INSTANTIATE_MANY_WRAPPED((LIBFLATSURF_INSTANTIATE_WITH_IMPLEMENTATION)(LIBFLATSURF_INSTANTIATE_HASH), Point, LIBFLATSURF_FLAT_TRIANGULATION_TYPES)
