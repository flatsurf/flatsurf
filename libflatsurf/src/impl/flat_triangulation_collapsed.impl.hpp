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

#ifndef LIBFLATSURF_FLAT_TRIANGULATION_COLLAPSED_IMPL_HPP
#define LIBFLATSURF_FLAT_TRIANGULATION_COLLAPSED_IMPL_HPP

#include <functional>
#include <iosfwd>
#include <list>
#include <memory>

#include "../../flatsurf/flat_triangulation.hpp"
#include "../../flatsurf/flat_triangulation_collapsed.hpp"
#include "../../flatsurf/half_edge_map.hpp"
#include "../../flatsurf/saddle_connection.hpp"
#include "../../flatsurf/vector.hpp"
#include "../../flatsurf/tracked.hpp"

namespace flatsurf {

template <typename T>
class CollapsedHalfEdge;

template <typename T>
class ImplementationOf<FlatTriangulationCollapsed<T>> {
  using SaddleConnection = flatsurf::SaddleConnection<FlatTriangulation<T>>;
  using CollapsedHalfEdge = flatsurf::CollapsedHalfEdge<T>;

  friend CollapsedHalfEdge;

 public:
  ImplementationOf(const FlatTriangulationCombinatorial&, std::unique_ptr<FlatTriangulation<T>>, const Vector<T>&);

  // Explicitly compute the area of this triangulation.
  static T area(const FlatTriangulationCollapsed<T>&);
  // Check that the face of this half edge is actually closed.
  static bool faceClosed(const FlatTriangulationCollapsed<T>&, HalfEdge);

  static void updateAfterFlip(HalfEdgeMap<SaddleConnection>&, const FlatTriangulationCombinatorial&, HalfEdge);
  static void updateBeforeCollapse(HalfEdgeMap<SaddleConnection>&, const FlatTriangulationCombinatorial&, Edge);

  template <typename M>
  static void handleCollapse(M&, const FlatTriangulationCollapsed<T>&, Edge, const std::function<void(const FlatTriangulationCollapsed<T>&, HalfEdge)>&);
  template <typename M>
  static void handleFlip(M&, const FlatTriangulationCollapsed<T>&, HalfEdge, const std::function<void(const FlatTriangulationCollapsed<T>&, HalfEdge, HalfEdge, HalfEdge, HalfEdge)>&);

  std::shared_ptr<const FlatTriangulation<T>> original;

  Vector<T> vertical;

  // Tracks collapsed vertical connections.
  Tracked<HalfEdgeMap<CollapsedHalfEdge>> collapsedHalfEdges;
  // The vectors associated to half edges. Unlike in a FlatTriangulation,
  // vectors[-halfEdge] == -vectors[halfEdge] does not necessarily hold. These
  // vectors are only valid in the half edge's face. Use collapsedHalfEdges to
  // cross to the other face.
  Tracked<HalfEdgeMap<SaddleConnection>> vectors;
};

}  // namespace flatsurf

#endif
