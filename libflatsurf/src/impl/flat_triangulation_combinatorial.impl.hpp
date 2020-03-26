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

#ifndef LIBFLATSURF_FLAT_TRIANGULATION_COMBINATORIAL_IMPL_HPP
#define LIBFLATSURF_FLAT_TRIANGULATION_COMBINATORIAL_IMPL_HPP

#include <functional>
#include <variant>
#include <vector>

#include "../external/slimsig/include/slimsig/slimsig.h"

#include "../../flatsurf/edge.hpp"
#include "../../flatsurf/flat_triangulation_combinatorial.hpp"
#include "../../flatsurf/half_edge.hpp"
#include "../../flatsurf/permutation.hpp"

namespace flatsurf {

template <>
class ImplementationOf<FlatTriangulationCombinatorial> {
 public:
  ImplementationOf(const Permutation<HalfEdge>&, const std::vector<HalfEdge>&);
  ~ImplementationOf();

  struct MessageAfterFlip {
    HalfEdge e;
  };
  struct MessageBeforeCollapse {
    Edge e;
  };
  struct MessageBeforeSwap {
    HalfEdge a, b;
  };
  struct MessageBeforeErase {
    std::vector<Edge> erase;
  };
  struct MessageAfterMove {
    FlatTriangulationCombinatorial* target;
  };

  using Message = std::variant<MessageAfterFlip, MessageBeforeCollapse, MessageBeforeSwap, MessageBeforeErase, MessageAfterMove>;

  void resetVertexes();
  void resetVertices();
  void resetEdges();
  void swap(HalfEdge a, HalfEdge b);

  // Sanity check this triangulation
  void check();

  // Connect to change event.
  static slimsig::signal<void(Message)>::connection connect(const FlatTriangulationCombinatorial&, std::function<void(Message)>);

  std::vector<Edge> edges;
  Permutation<HalfEdge> vertices;
  Permutation<HalfEdge> faces;
  std::vector<Vertex> vertexes;
  std::vector<HalfEdge> halfEdges;

  slimsig::signal<void(Message)> change;
};

}  // namespace flatsurf

#endif
