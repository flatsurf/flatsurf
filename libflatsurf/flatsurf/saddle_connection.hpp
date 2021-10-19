/**********************************************************************
 *  This file is part of flatsurf.
 *
 *        Copyright (C) 2019-2021 Julian Rüth
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

#ifndef LIBFLATSURF_SADDLE_CONNECTION_HPP
#define LIBFLATSURF_SADDLE_CONNECTION_HPP

#include <gmpxx.h>

#include <boost/operators.hpp>
#include <iosfwd>
#include <optional>
#include <vector>

#include "bound.hpp"
#include "copyable.hpp"
#include "half_edge.hpp"
#include "serializable.hpp"

namespace flatsurf {
template <typename Surface>
class SaddleConnection : public Serializable<SaddleConnection<Surface>>,
                         boost::equality_comparable<SaddleConnection<Surface>>,
                         boost::less_than_comparable<SaddleConnection<Surface>, Bound> {
  static_assert(std::is_same_v<Surface, std::decay_t<Surface>>, "type must not have modifiers such as const");

  using T = typename Surface::Coordinate;

 public:
  SaddleConnection(const Surface &, HalfEdge e);
  // The saddle connection described by the given chain that starts in the
  // sector counterclockwise next to source and ends in the sector
  // counterclockwise next to target.
  SaddleConnection(const Surface &, HalfEdge source, HalfEdge target, const Chain<Surface> &);
  SaddleConnection(const Surface &, HalfEdge source, HalfEdge target, Chain<Surface> &&);

  static SaddleConnection<Surface> inSector(const Surface &, HalfEdge source, const Vector<T> &);
  static SaddleConnection<Surface> inSector(const Surface &, HalfEdge source, const Vertical<Surface> &direction);
  static SaddleConnection<Surface> inHalfPlane(const Surface &, HalfEdge side, const Vertical<Surface> &, const Vector<T> &);
  static SaddleConnection<Surface> inPlane(const Surface &, HalfEdge plane, const Vector<T> &);
  static SaddleConnection<Surface> inPlane(const Surface &, HalfEdge plane, const Vertical<Surface> &direction);
  static SaddleConnection<Surface> inPlane(const Surface &, HalfEdge sourcePlane, HalfEdge targetPlane, const Chain<Surface> &);
  static SaddleConnection<Surface> alongVertical(const Surface &, const Vertical<Surface> &direction, HalfEdge plane);
  static SaddleConnection<Surface> clockwise(const SaddleConnection &from, const Vector<T> &);
  // Return the saddle connection that starts counterclockwise from source
  // (but not necessarily in the sector next to source) and ends
  // counterclockwise from target (but not necessarily in the sector next to
  // target.)
  static SaddleConnection<Surface> counterclockwise(const Surface &, HalfEdge source, HalfEdge target, const Chain<Surface> &);

  // Return the saddle connection that starts counterclockwise (or collinear)
  // from source and goes in `direction`.
  static SaddleConnection<Surface> counterclockwise(const Surface &, const SaddleConnection<Surface> &source, const Vertical<Surface> &direction);

  const Vector<T> &vector() const;
  const Chain<Surface> &chain() const;

  operator const Vector<T> &() const;
  operator const Chain<Surface> &() const;

  // The saddle connection is leaving from the vertex at the source of source.
  // It is leaving in a direction that is contained in the sector next to
  // source (counterclockwise) inclusive.
  HalfEdge source() const;

  // Symmetrical to source(), the connection is such that -vector() is in the
  // sector counterclockwise next to target() inclusive.
  HalfEdge target() const;

  const Surface &surface() const;

  // Return the sequence of half edges this saddle connection crosses.
  [[deprecated("Use path() instead")]] std::vector<HalfEdge> crossings() const;

  // The sequence of vertices and half edges this saddle connection crosses.
  std::vector<HalfEdgeIntersection<Surface>> path() const;

  // Return the turns between saddle connections.
  // If the angle between this saddle connection and the argument is α going
  // in counter-clockwise direction, returns ⌊α/2π⌋.
  int angle(const SaddleConnection<Surface> &) const;

  int angle(HalfEdge source, const Vector<T> &vector) const;

  // Return the turn direction between saddle connections.
  // Returns whether it is shorter to turn in counterclockwise or
  // clockwise direction. Returns COLLINEAR if both are the same.
  CCW ccw(const SaddleConnection<Surface>&) const;

  // Return the turn direction to the vector next to source.
  // Returns whether it is shorter to turn in counterclockwise or
  // clockwise direction. Returns COLLINEAR if both are the same.
  CCW ccw(HalfEdge source, const Vector<T> &vector) const;

  SaddleConnection<Surface> operator-() const;

  bool operator==(const SaddleConnection<Surface> &) const;

  bool operator>(const Bound) const;
  bool operator<(const Bound) const;

  template <typename S>
  friend std::ostream &operator<<(std::ostream &, const SaddleConnection<S> &);

 private:
  Copyable<SaddleConnection> self;

  friend ImplementationOf<SaddleConnection>;
};

template <typename Surface, typename... T>
SaddleConnection(const Surface &, T &&...) -> SaddleConnection<Surface>;

}  // namespace flatsurf

namespace std {

template <typename Surface>
struct hash<::flatsurf::SaddleConnection<Surface>> { size_t operator()(const ::flatsurf::SaddleConnection<Surface> &) const; };

}  // namespace std

#endif
