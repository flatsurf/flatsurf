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

#include "../flatsurf/flat_triangulation_combinatorial.hpp"

#include <fmt/format.h>

#include <ostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "../flatsurf/fmt.hpp"
#include "../flatsurf/half_edge_set.hpp"
#include "../flatsurf/half_edge_set_iterator.hpp"
#include "external/rx-ranges/include/rx/ranges.hpp"
#include "impl/flat_triangulation_combinatorial.impl.hpp"
#include "impl/vertex.impl.hpp"
#include "util/assert.ipp"

namespace flatsurf {

FlatTriangulationCombinatorial::FlatTriangulationCombinatorial() noexcept :
  FlatTriangulationCombinatorial({}, {}) {}

FlatTriangulationCombinatorial::FlatTriangulationCombinatorial(const std::vector<std::vector<int>>& vertices, const std::vector<int>& boundaries) :
  FlatTriangulationCombinatorial(PrivateConstructor{}, Permutation<HalfEdge>(vertices | rx::transform([](const auto& cycle) {
    return cycle | rx::transform([](const auto& id) { return HalfEdge(id); }) | rx::to_vector();
  }) | rx::to_vector()),
      boundaries | rx::transform([](int e) { return HalfEdge(e); }) | rx::to_vector()) {
  for (auto& cycle : vertices)
    for (auto it = cycle.rbegin(); it != cycle.rend(); it++)
      LIBFLATSURF_CHECK_ARGUMENT(std::find(begin(boundaries), end(boundaries), *it) == end(boundaries) || it == cycle.rbegin(), "Boundary edges must be at the end of a vertex permutation");
}

FlatTriangulationCombinatorial::FlatTriangulationCombinatorial(const Permutation<HalfEdge>& vertices) :
  FlatTriangulationCombinatorial(PrivateConstructor{}, vertices, std::vector<HalfEdge>{}) {}

FlatTriangulationCombinatorial::FlatTriangulationCombinatorial(const std::vector<std::tuple<HalfEdge, HalfEdge, HalfEdge>>& faces) :
  FlatTriangulationCombinatorial(PrivateConstructor{}, [&]() {
    std::unordered_map<HalfEdge, HalfEdge> initial, final;
    std::unordered_map<HalfEdge, HalfEdge> vertexPermutation;
    HalfEdgeSet nonboundary;

    const auto record = [&](HalfEdge a, HalfEdge b) {
      nonboundary.insert(a);
      LIBFLATSURF_ASSERT(vertexPermutation.find(a) == vertexPermutation.end(), "half edge must not be in two faces but " << a << " shows up more than once");
      vertexPermutation[a] = b;

      if (initial.find(a) == initial.end())
        initial[a] = a;
      if (final.find(b) == final.end())
        final[b] = b;

      final[initial[a]] = final[b];
      initial[final[b]] = initial[a];
    };

    for (const auto& [a, b, c] : faces) {
      record(a, -c);
      record(b, -a);
      record(c, -b);
    }

    const auto boundary = nonboundary | rx::filter([&](const auto halfEdge) {
      return !nonboundary.contains(-halfEdge);
    }) | rx::transform([](const auto halfEdge) {
      return -halfEdge;
    }) | rx::to_vector();

    for (const auto halfEdge : boundary) {
      LIBFLATSURF_ASSERT(vertexPermutation.find(halfEdge) == vertexPermutation.end(), "boundary edge must not be in any face");
      LIBFLATSURF_ASSERT(final.at(halfEdge) == halfEdge, "boundary half edge must be final when walking around ccw a vertex");
      LIBFLATSURF_ASSERT(initial.at(halfEdge) != halfEdge, "boundary half edge must not be initial when walking ccw around a vertex");
      vertexPermutation[halfEdge] = initial.at(halfEdge);
      LIBFLATSURF_ASSERT(final.at(vertexPermutation[halfEdge]) == halfEdge, "half edges around a vertex must form a loop but if we go from " << halfEdge << " to " << vertexPermutation[halfEdge] << " we eventually only get to " << final.at(vertexPermutation[halfEdge]) << " from there");
    }

    return std::make_shared<ImplementationOf<FlatTriangulationCombinatorial>>(Permutation<HalfEdge>(vertexPermutation), boundary);
  }()) {
  LIBFLATSURF_ASSERT(
      faces | rx::all_of([&](const auto face) {
        return self->faces(std::get<0>(face)) == std::get<1>(face) && self->faces(std::get<1>(face)) == std::get<2>(face) && self->faces(std::get<2>(face)) == std::get<0>(face);
      }),
      "triangulation changed faces, expected " << fmt::format("({})", fmt::join(faces | rx::transform([](const auto face) { return fmt::format("({}, {}, {})", std::get<0>(face), std::get<1>(face), std::get<2>(face)); }) | rx::to_vector(), ", ")) << " but got " << self->faces);
}

ImplementationOf<FlatTriangulationCombinatorial>::ImplementationOf(const Permutation<HalfEdge>& vertices, const std::vector<HalfEdge>& boundaries) :
  vertices(vertices),
  faces(
      // In the triangulation, the order in which half edges are attached to a
      // vertex defines the faces, so we reconstruct the faces here.
      vertices.domain() | rx::transform([&](const HalfEdge& e) {
        return find(begin(boundaries), end(boundaries), e) == end(boundaries)
                   ? std::pair<HalfEdge, HalfEdge>(-vertices(e), e)
                   // We mark boundary half edges by putting them into a trivial face
                   // which only contains that half edge.
                   : std::pair<HalfEdge, HalfEdge>(e, e);
      }) |
      rx::to_vector()),
  vertexes() {
  LIBFLATSURF_CHECK_ARGUMENT(vertices.size() % 2 == 0, "half edges must come in pairs");

  resetVertexes();
  resetEdges();

  // check that faces are triangles
  for (auto edge : faces.domain()) {
    if (faces(edge) != edge) {
      LIBFLATSURF_CHECK_ARGUMENT(faces(faces(faces(edge))) == edge, "not fully triangulated");
    } else {
      LIBFLATSURF_ASSERT_ARGUMENT(std::find(begin(boundaries), end(boundaries), edge) != end(boundaries), "faces must not be trivial");
    }
  }

  check();
}

sigslot::connection ImplementationOf<FlatTriangulationCombinatorial>::connect(const ImplementationOf* surface, std::function<void(Message)> handler) {
  return surface->change.connect(handler);
}

void ImplementationOf<FlatTriangulationCombinatorial>::check() const {
  assert(faces.domain().size() == vertices.domain().size() && "faces and vertices must have the same half edges as domain");
  assert([&]() {
    for (auto edge : faces.domain()) {
      if (faces(edge) == edge)
        // nothing to check for boundaries yet
        continue;
      if (faces(faces(edge)) == edge)
        // nothing to check for collapsed faces yet
        continue;
      if (-faces.preimage(edge) == edge)
        // surface is only connected in a point here, the faces do not encode
        // enough information to reconstruct vertices.
        continue;
      if (-faces.preimage(vertices(edge)) == vertices(edge))
        // surface is only connected in a point here, the faces do not encode
        // enough information to reconstruct vertices.
        continue;
      if (vertices(edge) != -faces.preimage(edge))
        return false;
    }
    return true;
  }() && "vertices must be consistent with faces");
}

void ImplementationOf<FlatTriangulationCombinatorial>::resetVertexes() {
  this->vertexes.clear();
  for (const auto& cycle : vertices.cycles())
    vertexes.push_back(::flatsurf::ImplementationOf<Vertex>::make(cycle));
}

void ImplementationOf<FlatTriangulationCombinatorial>::resetVertices() {
  std::vector<std::pair<HalfEdge, HalfEdge>> vertices;
  for (auto e : faces.domain()) {
    if (faces(e) == e)
      throw std::logic_error("not implemented: resetVertices() with boundaries");
    if (faces(e) == -e && faces(faces(e)) == e) {
      vertices.push_back({e, e});
    } else {
      vertices.push_back({e, -faces.preimage(e)});
    }
  }
  this->vertices = Permutation<HalfEdge>(vertices);
}

void ImplementationOf<FlatTriangulationCombinatorial>::resetEdges() {
  std::unordered_set<Edge> edges;
  for (const auto& e : faces.domain())
    edges.insert(e);
  this->edges = std::vector<Edge>(begin(edges), end(edges));
  // This can probably go away, see https://github.com/flatsurf/flatsurf/issues/147
  std::sort(begin(this->edges), end(this->edges), [&](const auto& lhs, const auto& rhs) { return lhs.index() < rhs.index(); });

  halfEdges = vertices.domain();
  // This can probably go away, see https://github.com/flatsurf/flatsurf/issues/147
  std::sort(begin(halfEdges), end(halfEdges), [&](const auto& lhs, const auto& rhs) { return lhs.index() < rhs.index(); });
}

void ImplementationOf<FlatTriangulationCombinatorial>::swap(HalfEdge a, HalfEdge b) {
  if (a == b) return;

  change(ImplementationOf::MessageBeforeSwap{a, b});

  vertices *= {a, b};
  std::vector{a, b} *= vertices;

  if (a != -b) {
    vertices *= {-a, -b};
    std::vector{-a, -b} *= vertices;
  }

  faces *= {a, b};
  std::vector{a, b} *= faces;

  if (a != -b) {
    faces *= {-a, -b};
    std::vector{-a, -b} *= faces;
  }

  resetVertexes();
}

void ImplementationOf<FlatTriangulationCombinatorial>::flip(HalfEdge e) {
  auto self = from_this();

  LIBFLATSURF_ASSERT_ARGUMENT(!self.boundary(e) && !self.boundary(-e), "cannot flip a boundary edge");

  // Let (e a b) and (-e c d) be the faces containing e and -e before the flip.
  const HalfEdge a = self.nextInFace(e);
  const HalfEdge b = self.nextInFace(a);
  const HalfEdge c = self.nextInFace(-e);
  const HalfEdge d = self.nextInFace(c);

  using cycle = std::vector<HalfEdge>;
  // flip e in "vertices"
  // (... b -a ...)(... a -e -d ...) -> (... b -e -a ...)(... a -d ...) so we
  // multiply vertices with (b a -e)
  // (... d -c ...)(... c e -b ...) -> (... d e -c ...)(... c -b ...) so we
  // multiply vertices with (d c e)
  vertices *= cycle{b, a, -e};
  vertices *= cycle{d, c, e};

  // flip e in "faces"
  // (a b e)(c d -e) -> (a -e d)(c e b), i.e., multiply with (a d e)(c b -e)
  faces *= cycle{a, d, e};
  faces *= cycle{c, b, -e};

  for (auto& vertex : vertexes)
    ImplementationOf<Vertex>::afterFlip(vertex, self, e);

  // notify attached structures about this flip
  change(ImplementationOf<FlatTriangulationCombinatorial>::MessageAfterFlip{e});

  check();
}

std::pair<HalfEdge, HalfEdge> ImplementationOf<FlatTriangulationCombinatorial>::collapse(HalfEdge collapse) {
  auto self = from_this();

  if (self.boundary(collapse) || self.boundary(-collapse))
    throw std::logic_error("not implemented: cannot collapse boundary edge yet");

  if (self.nextInFace(self.nextInFace(self.nextInFace(collapse))) != collapse || self.nextInFace(self.nextInFace(self.nextInFace(-collapse))) != -collapse)
    throw std::logic_error("not implemented: cannot collapse collapsed edge yet");

  // notify attached structures about this collapse
  change(ImplementationOf<FlatTriangulationCombinatorial>::MessageBeforeCollapse{collapse});

  // In principle, we will drop three pairs of half edges, namely e,
  // previousAtVertex(-e), nextAtVertex(-e).
  // Things get a bit more complicated when the faces at e and -e have
  // identified edges. In this case, we have to make sure we do not drop the
  // entire gadget surrounding e out of existence.
  std::unordered_set<Edge> dropEdges{collapse};

  {
    HalfEdge x = self.nextInFace(collapse);
    if (x == -self.previousInFace(collapse)) {
      // x must survive so we do not drop one entire side
    } else {
      dropEdges.insert(x);
    }
  }

  {
    HalfEdge y = -self.previousInFace(-collapse);
    if (self.nextInFace(collapse) == -self.nextInFace(-collapse) && -self.previousInFace(-collapse) == self.previousInFace(collapse)) {
      // y must survive so we do not drop the entire gadget out of existence
    } else if (y == self.nextInFace(-collapse)) {
      // y must survive so we do not drop one entire side
    } else {
      dropEdges.insert(y);
    }
  }

  // So we need to make sure that these have the highest numbers since many
  // bits of the code assume that there is no gap in the half edge numbering.
  {
    std::vector<Edge> originalDropEdges(begin(dropEdges), end(dropEdges));
    // This is probably necessary to correctly identify the maxEdge, see https://github.com/flatsurf/flatsurf/issues/147
    std::sort(begin(originalDropEdges), end(originalDropEdges), [&](const auto& lhs, const auto& rhs) { return lhs.index() < rhs.index(); });

    int maxEdge = static_cast<int>(self.edges().size());
    for (auto d : originalDropEdges) {
      while (maxEdge && dropEdges.find(Edge(maxEdge)) != end(dropEdges))
        maxEdge--;
      if (d.index() < self.edges().size() - dropEdges.size()) {
        HalfEdge swap = Edge(maxEdge).positive();
        this->swap(d.positive(), swap);
        if (d.positive() == collapse)
          collapse = swap;
        else if (d.negative() == collapse)
          collapse = -swap;
        dropEdges.erase(d);
        dropEdges.insert(maxEdge);
      }
    }
  }

  // The isolated half edges to remove
  std::unordered_set<HalfEdge> dropHalfEdges;
  for (auto d : dropEdges) {
    dropHalfEdges.insert(d.positive());
    dropHalfEdges.insert(d.negative());
  }

  change(ImplementationOf<FlatTriangulationCombinatorial>::MessageBeforeErase{dropEdges | rx::to_vector()});

  // Consider the faces (collapse, x, -a) and (-collapse, c, -y).
  const HalfEdge a = -self.previousInFace(collapse);
  const HalfEdge c = self.nextInFace(-collapse);

  // Remove any mention of these edges from `faces`:
  // Consider again the faces (collapse, x, -a) and (-collapse, c, -y).
  // To collapse these faces, -a needs to take the place of -x …
  {
    const HalfEdge _a = self.previousInFace(collapse);
    assert(_a == -a && "permutations not consistent in face to collapse");
    const HalfEdge _x = -self.nextInFace(collapse);
    const HalfEdge p_x = self.previousInFace(_x);
    const HalfEdge x = self.nextInFace(collapse);
    faces *= {_a, _x};
    faces *= {p_x, x};
    faces *= {self.previousInFace(collapse), collapse};
  }
  // … and c needs to take the place of y.
  {
    // Note that this might not be the original c anymore.
    const HalfEdge cc = self.nextInFace(-collapse);
    const HalfEdge y = -self.previousInFace(-collapse);
    const HalfEdge py = self.previousInFace(y);
    const HalfEdge _e = -collapse;
    faces *= {cc, y};
    faces *= {py, _e};
    faces *= {self.previousInFace(-collapse), -collapse};
  }

  faces.drop(dropHalfEdges | rx::to_vector());

  // Calculate vertex permutation from half edges, note that this might
  // separate vertices when a connection from a vertex to itself has been
  // collapsed.
  resetVertices();

  // The vertices at the end of e need to merge (or separate.) We could do this
  // more efficiently, but it's probably not worth it.
  resetVertexes();

  // This could also be done more efficiently.
  resetEdges();

  check();

  if (dropEdges.find(a) != end(dropEdges)) {
    assert(dropEdges.find(c) == end(dropEdges) && "an entire horizontal part of the surface has been dropped out of existence");
    return std::make_pair(-c, c);
  } else if (dropEdges.find(c) != end(dropEdges)) {
    assert(dropEdges.find(a) == end(dropEdges) && "an entire horizontal part of the surface has been dropped out of existence");
    return std::make_pair(a, -a);
  } else {
    return std::make_pair(a, c);
  }
}

ImplementationOf<FlatTriangulationCombinatorial>::~ImplementationOf() {
  change(MessageAfterMove{nullptr});
}

std::ostream& operator<<(std::ostream& os, const FlatTriangulationCombinatorial& self) {
  return os << static_cast<const FlatTriangulationCombinatorics<FlatTriangulationCombinatorial>&>(self);
}

}  // namespace flatsurf
