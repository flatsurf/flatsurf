/**********************************************************************
 *  This file is part of flatsurf.
 *
 *        Copyright (C) 2019-2022 Julian Rüth
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
  // sector counterclockwise next to `source` (including source but excluding
  // the direction given by the half edge that is next counterclockwise) and
  // ends in the sector counterclockwise next to `target` (again, including
  // target but excluding the following half edge.)
  SaddleConnection(const Surface &, HalfEdge source, HalfEdge target, const Chain<Surface> &);
  SaddleConnection(const Surface &, HalfEdge source, HalfEdge target, Chain<Surface> &&);

  // Return the saddle connection in the sector counterclockwise (or collinear)
  // next to `source` (excluding the counterclockwise following half edge) that
  // is described by the vector.
  static SaddleConnection<Surface> inSector(const Surface &, HalfEdge source, const Vector<T> &);
  // Return the saddle connection in the sector counterclockwise (or collinear)
  // next to `source` (excluding the counterclockwise following half edge) in
  // the given direction.
  static SaddleConnection<Surface> inSector(const Surface &, HalfEdge source, const Vertical<Surface> &direction);

  // Return the saddle connection given by the vector in the half plane defined
  // by `vertical` where vertical is taken to have a value strictly less than π
  // with `side`.
  [[deprecated("use inSector() instead and determine the sector with surface.sector()")]]
  static SaddleConnection<Surface> inHalfPlane(const Surface &, HalfEdge side, const Vertical<Surface> &, const Vector<T> &);
  [[deprecated("use inSector() instead and determine the sector with surface.sector()")]]
  static SaddleConnection<Surface> inPlane(const Surface &, HalfEdge plane, const Vector<T> &);
  [[deprecated("use inSector() instead and determine the sector with surface.sector()")]]
  static SaddleConnection<Surface> inPlane(const Surface &, HalfEdge plane, const Vertical<Surface> &direction);
  [[deprecated("use SaddleConnection() instead and determine source & target with surface.sector()")]]
  static SaddleConnection<Surface> inPlane(const Surface &, HalfEdge sourcePlane, HalfEdge targetPlane, const Chain<Surface> &);

  [[deprecated("use inSector() instead and determine the sector with surface.sector()")]]
  static SaddleConnection<Surface> alongVertical(const Surface &, const Vertical<Surface> &direction, HalfEdge plane);

  // Return the saddle connection strictly clockwise from the given saddle
  // connection with the given direction.
  [[deprecated("use inSector() instead and determine the sector with surface.sector()")]]
  static SaddleConnection<Surface> clockwise(const SaddleConnection &from, const Vector<T> &);

  // Return the saddle connection that starts counterclockwise (or collinear)
  // from source (but unlike the `SaddleConnection` constructor not necessarily
  // in the sector formed with the counterclockwise next half edge after
  // source) and ends counterclockwise (or collinear) from target (but, again,
  // not necessarily in the sector next to target.)
  [[deprecated("use SaddleConnection() instead and determine the source & target with surface.sector()")]]
  static SaddleConnection<Surface> counterclockwise(const Surface &, HalfEdge source, HalfEdge target, const Chain<Surface> &);

  // Return the saddle connection that starts counterclockwise (or collinear)
  // from source and goes in `direction`.
  [[deprecated("use inSector() instead and determine the sector with surface.sector()")]]
  static SaddleConnection<Surface> counterclockwise(const Surface &, const SaddleConnection<Surface> &source, const Vertical<Surface> &direction);

  const Vector<T> &vector() const;
  const Chain<Surface> &chain() const;
  Segment<Surface> segment() const;

  operator const Vector<T> &() const;
  operator const Chain<Surface> &() const;
  operator Segment<Surface>() const;

  // The saddle connection is leaving from the vertex at the source of source.
  // It is leaving in a direction that is contained in the sector next to
  // source (counterclockwise) inclusive.
  HalfEdge source() const;

  // Symmetrical to source(), the connection is such that -vector() is in the
  // sector counterclockwise next to target() inclusive.
  HalfEdge target() const;

  // Return the vertex at which this saddle connection starts.
  Vertex start() const;

  // Return the vertex at which this saddle connection ends.
  Vertex end() const;

  const Surface &surface() const;

  // Return the sequence of half edges this saddle connection crosses.
  [[deprecated("Use path() instead")]] std::vector<HalfEdge> crossings() const;

  // The sequence of half edges this saddle connection crosses.
  // This does not include the source and target half edge since the connection
  // there only touches.
  std::vector<HalfEdgeIntersection<Surface>> path() const;

  // Return the turns between saddle connections.
  // If the angle between this saddle connection and the argument is α going
  // in counter-clockwise direction, returns ⌊α/2π⌋.
  [[deprecated("use ray().angle() instead")]]
  int angle(const SaddleConnection<Surface> &) const;

  [[deprecated("use ray().angle() instead")]]
  int angle(HalfEdge source, const Vector<T> &vector) const;

  // Return the turn direction between saddle connections.
  // Returns whether it is shorter to turn in counterclockwise or
  // clockwise direction. Returns COLLINEAR if both are the same.
  [[deprecated("use ray().ccw() instead")]]
  CCW ccw(const SaddleConnection<Surface> &) const;

  // Return the turn direction to the vector next to source.
  // Returns whether it is shorter to turn in counterclockwise or
  // clockwise direction. Returns COLLINEAR if both are the same.
  [[deprecated("use ray().ccw() instead")]]
  CCW ccw(HalfEdge source, const Vector<T> &vector) const;

  explicit operator Ray<Surface>() const;

  Ray<Surface> ray() const;

  SaddleConnection<Surface> operator-() const;

  bool operator==(const SaddleConnection&) const;

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
