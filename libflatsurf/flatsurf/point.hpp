/**********************************************************************
 *  This file is part of flatsurf.
 *
 *        Copyright (C) 2022 Julian Rüth
 *                      2022 Sam Freedman
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

#ifndef LIBFLATSURF_POINT_HPP
#define LIBFLATSURF_POINT_HPP

#include <boost/operators.hpp>
#include <optional>
#include <iosfwd>
#include <array>

#include "copyable.hpp"
#include "serializable.hpp"

namespace flatsurf {

// A point on a surface.
template <typename Surface>
class Point : Serializable<Point<Surface>>,
              boost::equality_comparable<Point<Surface>>,
              boost::additive<Point<Surface>, Vector<typename Surface::Coordinate>> {
  static_assert(std::is_same_v<Surface, std::decay_t<Surface>>, "type must not have modifiers such as const");

  using T = typename Surface::Coordinate;

 public:
  // The point corresponding to the vertex.
  Point(const Surface&, const Vertex&);

  // The point in the face next to ``face`` with barycentric coordinates given
  // by the triple, i.e., P=\frac{aA + bB + cC}{a + b + c} where A, B, C are
  // the vertices of the face in counterclockwise order starting from the
  // source of ``face``.
  Point(const Surface&, HalfEdge face, const T& a, const T& b, const T& c);
  Point(const Surface&, HalfEdge face, const std::array<T, 3>&);

  // The point in the face next to the half edge ``face`` with Cartesian
  // coordinates (x, y) relative to the source vertex of ``face``.
  // If the coordinates are not within the face, the resulting point is the
  // endpoint of the segment given by (x, y) starting from the source vertex of
  // ``face``.
  Point(const Surface&, HalfEdge face, const Vector<T>& xy);

  // Shift the point by ``delta``.
  // The base point must be a marked point, i.e., with total angle 2π. The
  // resulting point might be in a different face. It is obtained by following
  // the ray with slope ``delta``.
  Point& operator+=(const Vector<T>& delta);
  Point& operator-=(const Vector<T>& delta);

  // Return whether this point is the same as ``other``.
  // Points that were defined with coordinates on different faces get correctly
  // identified by this method if they are on a shared edge or vertex of the
  // faces.
  bool operator==(const Point &other) const;

  // Return the surface this point is defined on.
  const Surface &surface() const;

  // Return whether this point is a vertex of the triangulation.
  std::optional<Vertex> vertex() const;

  // Return an edge this point is on, if any.
  std::optional<Edge> edge() const;

  // Return a face this point is in. i.e., a half edge such that
  // coordinates(face) produces non-negative weights.
  HalfEdge face() const;  

  // Return whether this point is in ``face``, i.e., whether it has all
  // non-negative ``coordinates`` in this face.
  bool in(HalfEdge face) const;

  // Return whether this point is on ``edge``, i.e., whether its coordinates
  // for any face adjacent to this edge are of the form (a, b, 0).
  bool on(Edge edge) const;

  // Return whether this point is the ``vertex``.
  bool at(const Vertex& vertex) const;

  // Return the barycentric coordinates of this point with respect to the face,
  // i.e., non-negative weights for the corners of the triangle in
  // counterclockwise order, namely for the source of face, the target of face
  // and the remaining corner.
  std::array<T, 3> coordinates(HalfEdge face) const;

  // Return the vector starting at the source of origin that ends at this point.
  // The point must be in the face which origin is delimiting.
  Vector<T> vector(HalfEdge origin) const;

  template <typename S>
  friend std::ostream &operator<<(std::ostream &, const Point<S> &);

 private:
  Copyable<Point> self;

  friend ImplementationOf<Point>;
  friend Serialization<Point>;
};

template <typename Surface, typename... T>
Point(const Surface &, T &&...) -> Point<Surface>;

}  // namespace flatsurf

template <typename Surface>
struct std::hash<flatsurf::Point<Surface>> {
  size_t operator()(const flatsurf::Point<Surface> &) const;
};

#endif
