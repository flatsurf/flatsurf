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

#include <memory>
#include <list>
#include <functional>
#include <iosfwd>

#include "../../flatsurf/flat_triangulation_collapsed.hpp"
#include "../../flatsurf/flat_triangulation.hpp"
#include "../../flatsurf/saddle_connection.hpp"
#include "../../flatsurf/half_edge_map.hpp"
#include "../../flatsurf/vector.hpp"
#include "../../flatsurf/vertex_map.hpp"

namespace flatsurf {

template <typename T>
class CollapsedHalfEdge;

template <typename T>
class Implementation<FlatTriangulationCollapsed<T>> {
  using Vector = typename FlatTriangulationCollapsed<T>::Vector;
  using SaddleConnection = typename FlatTriangulationCollapsed<T>::SaddleConnection;

 public:
  using CollapsedHalfEdge = ::flatsurf::CollapsedHalfEdge<T>;
  friend CollapsedHalfEdge;

  Implementation(const FlatTriangulationCombinatorial&, std::unique_ptr<FlatTriangulation<T>>, const Vector&);

  // TODO: Move to its own file.
  // TODO: This is the same as TrackingStorage::Value; merge them
  struct AsymmetricConnection {
    AsymmetricConnection(const SaddleConnection& value) : value(value) {}
    operator SaddleConnection() const { return value; }

    SaddleConnection value;

    bool operator==(const AsymmetricConnection& rhs) const {
      if constexpr (std::is_same_v<decltype(value == rhs.value), bool>)
        return value == rhs.value;
      else
        throw std::logic_error("not implemented: operator== for operands that do not implement bool operator==");
    }
    friend std::ostream& operator<<(std::ostream& os, const AsymmetricConnection& self) { return os << self.value; }
  };

  // Explicitly compute the area of this triangulation.
  static T area(const FlatTriangulationCollapsed<T>&);
  // Check that the face of this half edge is actually closed.
  static bool faceClosed(const FlatTriangulationCollapsed<T>&, HalfEdge);

  static void updateAfterFlip(HalfEdgeMap<AsymmetricConnection>&, HalfEdge);
  static void updateBeforeCollapse(HalfEdgeMap<AsymmetricConnection>&, Edge);

  template <typename M>
  static void handleCollapse(M&, Edge, const std::function<void(const FlatTriangulationCollapsed<T>&, HalfEdge)>&);
  template <typename M>
  static void handleFlip(M&, HalfEdge, const std::function<void(const FlatTriangulationCollapsed<T>&, HalfEdge, HalfEdge, HalfEdge, HalfEdge)>&);

  std::shared_ptr<const FlatTriangulation<T>> original;

  Vector vertical;

  // Tracks collapsed vertical connections.
  HalfEdgeMap<CollapsedHalfEdge> collapsedHalfEdges;
  // The vectors associated to half edges. Unlike in a FlatTriangulation,
  // vectors[-halfEdge] == -vectors[halfEdge] does not necessarily hold. These
  // vectors are only valid in the half edge's face. Use collapsedHalfEdges to
  // cross to the other face.
  HalfEdgeMap<AsymmetricConnection> vectors;
};

}

#endif
