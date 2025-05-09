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
#include "../../flatsurf/tracked.hpp"
#include "../../flatsurf/vector.hpp"
#include "flat_triangulation_combinatorial.impl.hpp"
#include "read_only.hpp"

namespace flatsurf {

template <typename T>
class CollapsedHalfEdge;

template <typename T>
class ImplementationOf<FlatTriangulationCollapsed<T>> : protected ImplementationOf<ManagedMovable<FlatTriangulationCollapsed<T>>>,
                                                        public ImplementationOf<FlatTriangulationCombinatorial> {
  using SaddleConnection = flatsurf::SaddleConnection<FlatTriangulation<T>>;
  using CollapsedHalfEdge = flatsurf::CollapsedHalfEdge<T>;

  friend CollapsedHalfEdge;

 public:
  ImplementationOf(const FlatTriangulation<T>&, const Vector<T>&);

  virtual void flip(HalfEdge) override;
  virtual std::pair<HalfEdge, HalfEdge> collapse(HalfEdge) override;

  ReadOnly<FlatTriangulation<T>> original;

  Vector<T> vertical;

  // Tracks collapsed vertical connections.
  Tracked<HalfEdgeMap<CollapsedHalfEdge>> collapsedHalfEdges;

  // The vectors associated to half edges. Unlike in a FlatTriangulation,
  // vectors[-halfEdge] == -vectors[halfEdge] does not necessarily hold. These
  // vectors are only valid in the half edge's face. Use collapsedHalfEdges to
  // cross to the other face.
  Tracked<HalfEdgeMap<SaddleConnection>> vectors;

  // Return twice the area of this triangulation.
  // It is better to just call .area2() on the collapsed triangulation directly
  // which returns the area of the uncollapsed triangulation.
  // This method exists for the sake of asserting that no area was lost in the
  // process of collapsing triangles.
  static T area2(const FlatTriangulationCollapsed<T>&);

  // Check that the face of this half edge is actually closed.
  bool faceClosed(HalfEdge);

  static void updateAfterFlip(HalfEdgeMap<SaddleConnection>&, const FlatTriangulationCombinatorial&, HalfEdge);
  static void updateBeforeCollapse(HalfEdgeMap<SaddleConnection>&, const FlatTriangulationCombinatorial&, Edge);

 protected:
  using ImplementationOf<ManagedMovable<FlatTriangulationCollapsed<T>>>::from_this;
  using ImplementationOf<ManagedMovable<FlatTriangulationCollapsed<T>>>::self;

  friend ReadOnly<FlatTriangulationCollapsed<T>>;
  friend ImplementationOf<ManagedMovable<FlatTriangulationCollapsed<T>>>;
};

}  // namespace flatsurf

#endif
