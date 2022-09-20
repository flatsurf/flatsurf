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

#ifndef LIBFLATSURF_RAY_HPP
#define LIBFLATSURF_RAY_HPP

#include <boost/operators.hpp>
#include <iosfwd>
#include <type_traits>

#include "copyable.hpp"
#include "serializable.hpp"

namespace flatsurf {

// A ray starting at a point of a translation surface.
template <typename Surface>
class Ray : Serializable<Ray<Surface>>,
            boost::equality_comparable<Ray<Surface>> {
  static_assert(std::is_same_v<Surface, std::decay_t<Surface>>, "type must not have modifiers such as const");

  using T = typename Surface::Coordinate;

 public:
  Ray(const Point<Surface>&, const Vector<T>&);
  Ray(const Point<Surface>&, HalfEdge source, const Vector<T>&);
  Ray(const Surface&, HalfEdge source, const Vector<T>&);
  Ray(const Surface&, HalfEdge);

  const Point<Surface>& start() const;

  HalfEdge source() const;

  // Return a vector pointing in the direction of this ray.
  explicit operator const Vector<T> &() const;

  // Return a vector pointing in the direction of this ray.
  const Vector<T>& vector() const;

  operator Vertical<Surface>() const;

  // Return the saddle connection obtained by extending this ray.
  SaddleConnection<Surface> saddleConnection() const;

  // Return the turns between two rays starting at the same point.
  // If the angle between this ray and the argument is α going
  // in counter-clockwise direction, returns ⌊α/2π⌋.
  int angle(const Ray&) const;

  // Return the turn direction between rays.
  // Returns whether it is shorter to turn in counterclockwise or
  // clockwise direction. Returns COLLINEAR if both are the same.
  CCW ccw(const Ray&) const;

  // Return whether two rays are the same, i.e., they are pointing in the same
  // direction from the same starting point.
  bool operator==(const Ray&) const;

  const Surface &surface() const;

  // Return the segment obtained by following this ray until ``end``.
  Segment<Surface> segment(const Point<Surface>& end) const;

  template <typename S>
  friend std::ostream &operator<<(std::ostream &, const Ray<S> &);

 private:
  Copyable<Ray> self;

  friend ImplementationOf<Ray>;
};

template <typename Surface, typename... Args>
Ray(const Surface &, Args &&...) -> Ray<Surface>;

template <typename Surface, typename... Args>
Ray(const Point<Surface>&, Args &&...) -> Ray<Surface>;

}  // namespace flatsurf

namespace std {

template <typename Surface>
struct hash<::flatsurf::Ray<Surface>> { size_t operator()(const ::flatsurf::Ray<Surface> &) const; };

}  // namespace std

#endif

