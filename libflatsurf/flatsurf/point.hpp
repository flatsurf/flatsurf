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
  Point(const Surface&, HalfEdge, const T&, const T&);
  Point(const Surface&, HalfEdge, const T&, const T&, const T&);

  bool operator==(const Point &) const;

  const Surface &surface() const;

  // Return whether this point is a vertex of the triangulation.
  std::optional<Vertex> vertex() const;

  // Return the half edge this point is on, if any.
  // If its a vertex, return the edge whose source it is.
  std::optional<Edge> edge() const;

  // Return a face this point is in,
  // i.e., weights(face) works.
  HalfEdge face() const;  

  T weight(HalfEdge source) const;

  std::tuple<T, T, T> weights(HalfEdge face) const;

  Vector<T> vector(HalfEdge origin) const;

  friend std::ostream &operator<<(std::ostream &, const Point &);

 private:
  Copyable<Point> self;

  friend ImplementationOf<Point>;
  friend Serialization<Point>;
};
}  // namespace flatsurf

template <typename Surface>
struct std::hash<flatsurf::Point<Surface>> {
  size_t operator()(const flatsurf::Point<Surface> &) const;
};

#endif
