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

  void normalize();

  // Return barycentric coordinates of this point with respect to the ``face``.
  std::array<T, 3> rotated(HalfEdge face) const;

  // Describe this point in barycentric coordinates based at ``face`` (port of
  // normalize.)
  void rotate(HalfEdge face);

  // Return barycentric coordinates with respect to the opposite face.
  std::array<T, 3> crossed() const;

  ReadOnly<Surface> surface;
  HalfEdge face;
  T a, b, c;
};

}  // namespace flatsurf
#endif

