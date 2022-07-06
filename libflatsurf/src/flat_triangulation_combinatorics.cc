/**********************************************************************
 *  This file is part of flatsurf.
 *
 *        Copyright (C) 2019-2020 Julian Rüth
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

#include "../flatsurf/flat_triangulation_combinatorics.hpp"

#include <fmt/format.h>

#include <cstdint>
#include <ostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "../flatsurf/edge.hpp"
#include "../flatsurf/fmt.hpp"
#include "../flatsurf/half_edge.hpp"
#include "../flatsurf/half_edge_map.hpp"
#include "../flatsurf/half_edge_set_iterator.hpp"
#include "../flatsurf/permutation.hpp"
#include "../flatsurf/vertex.hpp"
#include "external/rx-ranges/include/rx/ranges.hpp"
#include "impl/flat_triangulation_combinatorial.impl.hpp"
#include "impl/vertex.impl.hpp"
#include "util/assert.ipp"

namespace flatsurf {

using std::begin;

template <typename Surface>
HalfEdge FlatTriangulationCombinatorics<Surface>::nextInFace(const HalfEdge e) const {
  LIBFLATSURF_ASSERT_ARGUMENT(!boundary(e), "boundary half edge has no successor since it is not on any face");
  return self->faces(e);
}

template <typename Surface>
HalfEdge FlatTriangulationCombinatorics<Surface>::previousInFace(const HalfEdge e) const {
  LIBFLATSURF_ASSERT_ARGUMENT(!boundary(e), "boundary half edge has no predecessor since it is not on any face");
  return self->faces.preimage(e);
}

template <typename Surface>
bool FlatTriangulationCombinatorics<Surface>::boundary(const HalfEdge e) const {
  return self->faces(e) == e;
}

template <typename Surface>
HalfEdge FlatTriangulationCombinatorics<Surface>::nextAtVertex(const HalfEdge e) const {
  LIBFLATSURF_ASSERT_ARGUMENT(!boundary(e), "boundary half edge has no successor at vertex");
  return self->vertices(e);
}

template <typename Surface>
HalfEdge FlatTriangulationCombinatorics<Surface>::previousAtVertex(const HalfEdge e) const {
  LIBFLATSURF_ASSERT_ARGUMENT(!boundary(-e), "complement of boundary half edge has no predecessor at vertex");
  return self->vertices.preimage(e);
}

template <typename Surface>
const std::vector<HalfEdge>& FlatTriangulationCombinatorics<Surface>::halfEdges() const {
  return self->halfEdges;
}

template <typename Surface>
const std::vector<Vertex>& FlatTriangulationCombinatorics<Surface>::vertices() const {
  return self->vertexes;
}

template <typename Surface>
const std::vector<Edge>& FlatTriangulationCombinatorics<Surface>::edges() const {
  return self->edges;
}

template <typename Surface>
FlatTriangulationCombinatorial FlatTriangulationCombinatorics<Surface>::clone() const {
  return ImplementationOf<FlatTriangulationCombinatorial>::make(
      self->vertices,
      self->faces.domain() | rx::filter([&](auto& edge) { return this->boundary(edge); }) | rx::to_vector());
}

template <typename Surface>
FlatTriangulationCombinatorial FlatTriangulationCombinatorics<Surface>::insertAt(HalfEdge e) const {
  LIBFLATSURF_CHECK_ARGUMENT(!boundary(e), "cannot insert vertex beyond boundary");

  // Insert three new half edges a, b, c which go around the new vertex such that -a is next to e.
  const int nextEdge = static_cast<int>(self->vertices.size() / 2) + 1;
  HalfEdge a = HalfEdge(nextEdge);
  HalfEdge b = HalfEdge(nextEdge + 1);
  HalfEdge c = HalfEdge(nextEdge + 2);

  auto cycles = self->vertices.cycles();
  for (auto& cycle : cycles) {
    for (size_t i = 0; i < cycle.size(); i++) {
      if (cycle[i] == e) {
        i++;
        cycle.insert(begin(cycle) + i, -a);
      } else if (cycle[i] == nextInFace(e)) {
        i++;
        cycle.insert(begin(cycle) + i, -b);
      } else if (cycle[i] == nextInFace(nextInFace(e))) {
        i++;
        cycle.insert(begin(cycle) + i, -c);
      }
    }
  }
  cycles.push_back({a, b, c});

  return ImplementationOf<FlatTriangulationCombinatorial>::make(
      Permutation<HalfEdge>(cycles),
      self->faces.domain() | rx::filter([&](auto& edge) { return this->boundary(edge); }) | rx::to_vector());
}

template <typename Surface>
std::array<HalfEdge, 3> FlatTriangulationCombinatorics<Surface>::face(HalfEdge face) const {
  return {face, nextInFace(face), previousInFace(face)};
}

template <typename Surface>
std::vector<std::tuple<HalfEdge, HalfEdge, HalfEdge>> FlatTriangulationCombinatorics<Surface>::faces() const {
  return self->faces.cycles() | rx::filter([](const auto& face) {
    // Boundary half edges have trivial faces.
    return face.size() == 3;
  }) | rx::transform([](const auto& face) {
    return std::tuple{face[0], face[1], face[2]};
  }) | rx::to_vector();
}

template <typename Surface>
FlatTriangulationCombinatorial FlatTriangulationCombinatorics<Surface>::slit(HalfEdge e) const {
  LIBFLATSURF_CHECK_ARGUMENT(!boundary(e) && !boundary(-e), "cannot disconnect half edge that is already boundary");

  // We insert a new half edge ee into the vertex permutation next to e
  auto ee = HalfEdge(static_cast<int>(halfEdges().size()) / 2 + 1);

  std::unordered_map<HalfEdge, HalfEdge> vertices;
  for (auto h : halfEdges()) {
    if (h == e) {
      vertices[e] = ee;
      vertices[ee] = nextAtVertex(e);
    } else if (nextAtVertex(h) == -e) {
      vertices[h] = -ee;
      vertices[-ee] = -e;
    } else {
      vertices[h] = nextAtVertex(h);
    }
  }

  // If there is already a boundary at the vertex of e, we need to split this
  // vertex as we do not allow connections only in a point.
  for (HalfEdge current = nextAtVertex(e); current != e; current = nextAtVertex(current)) {
    if (boundary(current)) {
      vertices[current] = ee;
      vertices[e] = self->vertices(current);
      break;
    }
  }

  for (HalfEdge current = nextAtVertex(-e); current != -e; current = nextAtVertex(current)) {
    if (boundary(current)) {
      vertices[current] = -e;
      vertices[-ee] = self->vertices(current);
      break;
    }
  }

  Permutation<HalfEdge> vertexPermutation(vertices);

  return ImplementationOf<FlatTriangulationCombinatorial>::make(
      Permutation<HalfEdge>(vertices),
      vertexPermutation.domain() | rx::filter([&](auto& edge) { return edge == e || edge == -ee || (edge != ee && boundary(edge)); }) | rx::to_vector());
}

template <typename Surface>
std::vector<HalfEdge> FlatTriangulationCombinatorics<Surface>::atVertex(const Vertex& v) const {
  return self->vertices.cycle(*begin(ImplementationOf<Vertex>::outgoing(v)));
}

template <typename Surface>
bool FlatTriangulationCombinatorics<Surface>::hasBoundary() const {
  return halfEdges() | rx::any_of([&](const auto& edge) { return boundary(edge); });
}

template <typename Surface>
size_t FlatTriangulationCombinatorics<Surface>::size() const {
  return self->edges.size();
}

template <typename Surface>
void FlatTriangulationCombinatorics<Surface>::flip(HalfEdge e) {
  self->flip(e);
}

template <typename Surface>
std::pair<HalfEdge, HalfEdge> FlatTriangulationCombinatorics<Surface>::collapse(HalfEdge e) {
  return self->collapse(e);
}

template <typename Surface>
FlatTriangulationCombinatorial FlatTriangulationCombinatorics<Surface>::operator~() const {
  return ImplementationOf<FlatTriangulationCombinatorial>::make(~self->vertices, halfEdges() | rx::filter([&](HalfEdge he) { return boundary(he); }) | rx::transform([](HalfEdge he) { return -he; }) | rx::to_vector());
}

template <typename Surface>
bool FlatTriangulationCombinatorics<Surface>::operator==(const FlatTriangulationCombinatorial& rhs) const {
  if (self.state == ImplementationOf<FlatTriangulationCombinatorial>::self(rhs).state) return true;
  return this->self->vertices == ImplementationOf<FlatTriangulationCombinatorial>::self(rhs)->vertices;
}

template <typename Surface>
FlatTriangulationCombinatorial& FlatTriangulationCombinatorics<Surface>::combinatorial() {
  return *this;
}

template <typename Surface>
const FlatTriangulationCombinatorial& FlatTriangulationCombinatorics<Surface>::combinatorial() const {
  return *this;
}

template <typename Surface>
FlatTriangulationCombinatorics<Surface>::operator const FlatTriangulationCombinatorial&() const {
  return *reinterpret_cast<const FlatTriangulationCombinatorial*>(this);
}

template <typename Surface>
FlatTriangulationCombinatorics<Surface>::operator FlatTriangulationCombinatorial&() {
  return *reinterpret_cast<FlatTriangulationCombinatorial*>(this);
}

template <typename Surface>
std::ostream& operator<<(std::ostream& os, const FlatTriangulationCombinatorics<Surface>& self) {
  return os << "FlatTriangulationCombinatorial(vertices = "
            << self.self->vertices << ", faces = " << self.self->faces << ")";
}
}  // namespace flatsurf

// Instantiations of templates so implementations are generated for the linker
#include "impl/flat_triangulation.impl.hpp"
#include "impl/flat_triangulation_collapsed.impl.hpp"
#include "util/instantiate.ipp"

LIBFLATSURF_INSTANTIATE_MANY_WRAPPED((LIBFLATSURF_INSTANTIATE_WITHOUT_IMPLEMENTATION), FlatTriangulationCombinatorics, LIBFLATSURF_FLAT_TRIANGULATION_TYPES(FlatTriangulationCombinatorial) LIBFLATSURF_FLAT_TRIANGULATION_COLLAPSED_TYPES)
