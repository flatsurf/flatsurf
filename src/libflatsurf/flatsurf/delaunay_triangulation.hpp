/**********************************************************************
 *  This file is part of flatsurf.
 *
 *        Copyright (C) 2019 Julian Rüth
 *
 *  Flatsurf is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 of the License, or
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

#ifndef LIBFLATSURF_DELAUNAY_TRIANGULATION_HPP
#define LIBFLATSURF_DELAUNAY_TRIANGULATION_HPP

#include "flatsurf/forward.hpp"

namespace flatsurf {
template <typename Vector>
class DelaunayTriangulation {
 public:
  // Flip edges in this triangulation so that all faces satisfy the l²-Delaunay condition.
  static void transform(FlatTriangulation<Vector>&);

  // Return whether this half edge satisfies the l²-Delaunay condition, i.e.,
  // whether the circumcircles of the face attached to this edge does not
  // contain the face attached to the reverse half edge.
  static bool test(const FlatTriangulation<Vector>&, HalfEdge);
};
}  // namespace flatsurf

#endif
