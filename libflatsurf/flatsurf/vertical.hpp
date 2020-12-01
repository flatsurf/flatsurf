/**********************************************************************
 *  This file is part of flatsurf.
 *
 *        Copyright (C) 2019-2020 Julian Rüth
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

#ifndef LIBFLATSURF_VERTICAL_HPP
#define LIBFLATSURF_VERTICAL_HPP

#include <boost/operators.hpp>
#include <iosfwd>
#include <type_traits>
#include <unordered_set>
#include <vector>

#include "copyable.hpp"
#include "serializable.hpp"

namespace flatsurf {

// A vertical direction on a translation surface.
template <typename Surface>
class Vertical : Serializable<Vertical<Surface>>,
                 boost::equality_comparable<Vertical<Surface>> {
  static_assert(std::is_same_v<Surface, std::decay_t<Surface>>, "type must not have modifiers such as const");

  using T = typename Surface::Coordinate;

 public:
  Vertical(const Surface &, const Vector<T> &vertical);

  enum class TRIANGLE {
    BACKWARD = 1,
    FORWARD = -1,
    COLLAPSED_TO_TWO_FACES = 0,
    LEFT_VERTICAL = 2,
    RIGHT_VERTICAL = -2,
  };

  TRIANGLE classifyFace(HalfEdge) const;

  // Return whether this is a large edge, i.e., whether it is a wide edge of
  // both of its adjacent triangles.
  bool large(HalfEdge) const;

  // Return the defining vector of this vertical direction.
  const Vector<T> &vertical() const;

  // Return whether the vertical direction and  this edge are parallel.
  bool parallel(Edge) const;

  // Return whether the vertical direction and this edge are orthogonal.
  bool perpendicular(Edge) const;

  CCW ccw(HalfEdge) const;
  ORIENTATION orientation(HalfEdge) const;

  CCW ccw(const Vector<T> &) const;
  ORIENTATION orientation(const Vector<T> &) const;

  // Return a scalar projection of this half edge onto the vertical. Note that
  // this projection might not be normalized, i.e., it is typically just the
  // scalar product with the vertical defining vector.
  T project(HalfEdge) const;

  // Return a scalar projection of this vector onto the vertical. Note that
  // this projection might not be normalized, i.e., it is typically just the
  // scalar product with the vertical defining vector.
  T project(const Vector<T> &) const;

  // Return a scalar projection of this half edge onto the direction
  // perpendicular to this vertical. Note that this projection might not be
  // normalized, i.e., it is typically just the scalar product with a fixed
  // vector that is orthogonal to the defining vector.
  T projectPerpendicular(HalfEdge) const;

  // Return a scalar projection of this vector onto the direction perpendicular
  // to this vertical. Note that this projection might not be normalized, i.e.,
  // it is typically just the scalar product with a fixed vector that is
  // orthogonal to the defining vector.
  T projectPerpendicular(const Vector<T> &) const;

  const Surface &surface() const;

  std::vector<std::unordered_set<HalfEdge>> components() const;

  bool operator==(const Vertical &) const;

  Vertical<Surface> operator-() const;

  operator const Vector<T> &() const;

  template <typename S>
  friend std::ostream &operator<<(std::ostream &, const Vertical<S> &);

 private:
  ManagedMovable<Vertical> self;

  friend ImplementationOf<Vertical>;
  friend ImplementationOf<ManagedMovable<Vertical>>;
};

template <typename Surface, typename... Args>
Vertical(const Surface &, Args &&...) -> Vertical<Surface>;

}  // namespace flatsurf

#endif
