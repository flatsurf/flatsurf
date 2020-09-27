/**********************************************************************
 *  This file is part of flatsurf.
 *
 *        Copyright (C) 2019 Julian Rüth
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

#ifndef LIBFLATSURF_FLAT_TRIANGULATION_COLLAPSED_IMPL_COLLAPSED_HALF_EDGE_HPP
#define LIBFLATSURF_FLAT_TRIANGULATION_COLLAPSED_IMPL_COLLAPSED_HALF_EDGE_HPP

#include <iosfwd>
#include <list>

#include "../../flatsurf/flat_triangulation.hpp"
#include "../../flatsurf/path.hpp"
#include "../../flatsurf/saddle_connection.hpp"
#include "./flat_triangulation_collapsed.impl.hpp"

namespace flatsurf {

template <typename T>
class CollapsedHalfEdge {
  using Surface = FlatTriangulationCollapsed<T>;

 public:
  static void updateAfterFlip(HalfEdgeMap<CollapsedHalfEdge>&, const FlatTriangulationCombinatorial&, HalfEdge);
  static void updateBeforeCollapse(HalfEdgeMap<CollapsedHalfEdge>&, const FlatTriangulationCombinatorial&, Edge);

  bool operator==(const CollapsedHalfEdge&) const;

  template <typename S>
  friend std::ostream& operator<<(std::ostream& os, const CollapsedHalfEdge<S>&);

  // Tracks the collapsed vertical saddle connections, namely the connections
  // that we need to cross to pass from this half edge's face to the one of
  // its corresponding negative half edge.
  Path<FlatTriangulation<T>> connections = Path<FlatTriangulation<T>>();
};

}  // namespace flatsurf

namespace std {

template <typename T>
struct hash<::flatsurf::CollapsedHalfEdge<T>> { std::size_t operator()(const ::flatsurf::CollapsedHalfEdge<T>&) const; };

}  // namespace std

#endif
