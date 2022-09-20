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

#ifndef LIBFLATSURF_POINT_IMPL_HPP
#define LIBFLATSURF_POINT_IMPL_HPP

#include "../../flatsurf/point.hpp"
#include "../../flatsurf/half_edge.hpp"
#include "read_only.hpp"

namespace flatsurf {

template <typename Surface>
class ImplementationOf<Point<Surface>> {
  using Point = flatsurf::Point<Surface>;
  using T = typename Surface::Coordinate;

 public:
  ImplementationOf(const Surface&, HalfEdge face, T a, T b, T c);
  ImplementationOf(const Surface&, HalfEdge face, const Vector<T>& xy);

  void normalize();

  // Return barycentric coordinates of this point with respect to the ``face``.
  std::array<T, 3> rotated(HalfEdge face) const;

  // Describe this point in barycentric coordinates based at ``face`` (part of
  // normalize.)
  void rotate(HalfEdge face);

  // Return barycentric coordinates with respect to the opposite face.
  std::array<T, 3> crossed() const;

  // Return Cartesian coordinates for this point with respect to the source of its ``face``.
  Vector<T> cartesian() const;

  // Move this point by xy in Cartesian coordinates.
  // The point might not be a marked point. The movement is performed with
  // respect to the face which is selected by its current ``face``.
  // Returns a half edge such that shifting the resulting point back by -xy
  // with respect to that half edge produces the original point.
  // Or equivalently, returns a face which contains the initial part of -xy
  // pointing to the original point.
  HalfEdge translate(const Vector<T>& xy);

  // Move this point by xy in Cartesian coordinates.
  // Unlike the above, this normalizes the face first, i.e., the point must be
  // marked.
  // Note: This method is not exposed to the user because of the somewhat
  // confusing interface. The functionality is available though, it's
  // essentially equivalent to Segment(p, xy).target().
  static HalfEdge translate(Point& p, const Vector<T>& xy);

  // Return barycentric coordinates of the point xy in face.
  // The returned coordinates are not normalized, in particular, they might not
  // be all non-negative if the point is not in the face.
  static std::array<T, 3> barycentric(const Surface&, HalfEdge face, const Vector<T>& xy);

  ReadOnly<Surface> surface;
  HalfEdge face;
  T a, b, c;
};

}  // namespace flatsurf
#endif

