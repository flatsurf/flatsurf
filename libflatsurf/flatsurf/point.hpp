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

#include "copyable.hpp"
#include "serializable.hpp"

namespace flatsurf {

// A point on a surface.
template <typename Surface>
class Point : Serializable<Point<Surface>>,
              boost::equality_comparable<Point<Surface>> {
  static_assert(std::is_same_v<Surface, std::decay_t<Surface>>, "type must not have modifiers such as const");

  using T = typename Surface::Coordinate;

 public:
  // The point corresponding to the vertex.
  Point(const Surface&, const Vertex&);

  // The point in the face next to ``face`` with barycentric coordinates given
  // by the triple. (The coordinates do not have to sum to one.)
  Point(const Surface&, HalfEdge face, const std::tuple<T, T, T>&);

  // The point in the face next to ``face`` with barycentric coordinates given
  // by the triple. (The coordinates do not have to sum to one.)
  Point(const Surface&, HalfEdge, const T&, const T&, const T&);

  bool operator==(const Point &) const;

  const Surface &surface() const;

  // Return whether this point is a vertex of the triangulation.
  std::optional<Vertex> vertex() const;

  // Return an edge this point is on, if any.
  std::optional<Edge> edge() const;

  // Return a face this point is in. i.e., a half edge such that
  // coordinates(face) produces non-negative weights.
  HalfEdge face() const;  

  // Return whether this point is in ``face``, i.e., whether ``coordinates``
  // would be all non-negative for this face.
  bool in(HalfEdge face) const;

  bool on(Edge edge) const;

  bool at(const Vertex& vertex) const;

  // Return the barycentric coordinates of this point with respect to the face,
  // i.e., weights for the vertices in counterclockwise order, namely for the
  // source of face, the target of face and the remaining vertex.
  std::tuple<T, T, T> coordinates(HalfEdge face) const;

  // Return the vector starting at the source of origin that ends at this point.
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
