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

#include <cstdint>
#include <ostream>
#include <set>
#include <unordered_map>
#include <vector>

#include "external/rx-ranges/include/rx/ranges.hpp"

#include "../flatsurf/edge.hpp"
#include "../flatsurf/flat_triangulation_combinatorial.hpp"
#include "../flatsurf/half_edge.hpp"
#include "../flatsurf/half_edge_map.hpp"
#include "../flatsurf/permutation.hpp"
#include "../flatsurf/vertex.hpp"

#include "impl/flat_triangulation_combinatorial.impl.hpp"
#include "impl/tracking.impl.hpp"
#include "impl/vertex.impl.hpp"

#include "util/assert.ipp"

namespace flatsurf {

using rx::filter;
using rx::to_set;
using rx::to_vector;
using rx::transform;
using std::function;
using std::ostream;
using std::pair;
using std::set;
using std::uintptr_t;
using std::unordered_map;
using std::vector;

HalfEdge FlatTriangulationCombinatorial::nextInFace(const HalfEdge e) const {
  ASSERT_ARGUMENT(!this->boundary(e), "boundary half edge has no successor since it is not on any face");
  return impl->faces(e);
}

HalfEdge FlatTriangulationCombinatorial::previousInFace(const HalfEdge e) const {
  ASSERT_ARGUMENT(!this->boundary(e), "boundary half edge has no predecessor since it is not on any face");
  return impl->faces.preimage(e);
}

bool FlatTriangulationCombinatorial::boundary(const HalfEdge e) const {
  return impl->faces(e) == e;
}

HalfEdge FlatTriangulationCombinatorial::nextAtVertex(const HalfEdge e) const {
  ASSERT_ARGUMENT(!this->boundary(e), "boundary half edge has no successor at vertex");
  return impl->vertices(e);
}

HalfEdge FlatTriangulationCombinatorial::previousAtVertex(const HalfEdge e) const {
  ASSERT_ARGUMENT(!this->boundary(-e), "complement of boundary half edge has no predecessor at vertex");
  return impl->vertices.preimage(e);
}

const vector<HalfEdge>& FlatTriangulationCombinatorial::halfEdges() const {
  return impl->vertices.domain();
}

const vector<Vertex>& FlatTriangulationCombinatorial::vertices() const {
  return impl->vertexes;
}

const vector<Edge>& FlatTriangulationCombinatorial::edges() const {
  return impl->edges;
}

FlatTriangulationCombinatorial::FlatTriangulationCombinatorial() :
  FlatTriangulationCombinatorial(vector<vector<int>>()) {}

FlatTriangulationCombinatorial::FlatTriangulationCombinatorial(const vector<vector<int>>& vertices, const set<int>& boundaries) :
  impl(spimpl::make_unique_impl<Implementation>(
      Permutation<HalfEdge>(vertices | transform([](const auto& cycle) {
        return cycle | transform([](const auto& id) { return HalfEdge(id); }) | to_vector();
      }) | to_vector()),
      boundaries | transform([](int e) { return HalfEdge(e); }) | to_set())) {
  for (auto& cycle : vertices)
    for (auto it = cycle.rbegin(); it != cycle.rend(); it++)
      CHECK_ARGUMENT(boundaries.find(*it) == boundaries.end() || it == cycle.rbegin(), "Boundary edges must be at the end of a vertex permutation");
}

FlatTriangulationCombinatorial::FlatTriangulationCombinatorial(const Permutation<HalfEdge>& vertices) :
  impl(spimpl::make_unique_impl<Implementation>(vertices, set<HalfEdge>())) {
}

FlatTriangulationCombinatorial::FlatTriangulationCombinatorial(FlatTriangulationCombinatorial&& rhs) :
  FlatTriangulationCombinatorial() {
  *this = std::move(rhs);
}

FlatTriangulationCombinatorial& FlatTriangulationCombinatorial::operator=(FlatTriangulationCombinatorial&& rhs) noexcept {
  impl = std::move(rhs.impl);
  impl->change.emit(Implementation::MessageAfterMove{this});
  return *this;
}

std::unique_ptr<FlatTriangulationCombinatorial> FlatTriangulationCombinatorial::clone() const {
  auto ret = std::make_unique<FlatTriangulationCombinatorial>();
  ret->impl = spimpl::make_unique_impl<Implementation>(
      impl->vertices,
      impl->faces.domain() | filter([&](auto& edge) { return this->boundary(edge); }) | to_set());
  return ret;
}

std::unique_ptr<FlatTriangulationCombinatorial> FlatTriangulationCombinatorial::insertAt(HalfEdge e) const {
  CHECK_ARGUMENT(!boundary(e), "cannot insert vertex beyond boundary");

  // Insert three new half edges a, b, c which go around the new vertex such that -a is next to e.
  const int nextEdge = static_cast<int>(impl->vertices.size() / 2) + 1;
  HalfEdge a = HalfEdge(nextEdge);
  HalfEdge b = HalfEdge(nextEdge + 1);
  HalfEdge c = HalfEdge(nextEdge + 2);

  auto cycles = impl->vertices.cycles();
  for (auto& cycle : cycles) {
    for (size_t i = 0; i < cycle.size(); i++) {
      if (cycle[i] == e) {
        i++;
        cycle.insert(cycle.begin() + i, -a);
      } else if (cycle[i] == nextInFace(e)) {
        i++;
        cycle.insert(cycle.begin() + i, -b);
      } else if (cycle[i] == nextInFace(nextInFace(e))) {
        i++;
        cycle.insert(cycle.begin() + i, -c);
      }
    }
  }
  cycles.push_back({a, b, c});

  auto ret = std::make_unique<FlatTriangulationCombinatorial>();
  ret->impl = spimpl::make_unique_impl<Implementation>(
      Permutation<HalfEdge>(cycles),
      impl->faces.domain() | filter([&](auto& edge) { return this->boundary(edge); }) | to_set());
  return ret;
}

std::unique_ptr<FlatTriangulationCombinatorial> FlatTriangulationCombinatorial::slot(HalfEdge e) const {
  CHECK_ARGUMENT(!boundary(e) && !boundary(-e), "cannot disconnect half edge that is already boundary");

  // We insert a new half edge ee into the vertex permutation next to e
  HalfEdge ee(static_cast<int>(halfEdges().size()) / 2 + 1);

  std::map<HalfEdge, HalfEdge> vertices;
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
      vertices[e] = impl->vertices(current);
      break;
    }
  }

  for (HalfEdge current = nextAtVertex(-e); current != -e; current = nextAtVertex(current)) {
    if (boundary(current)) {
      vertices[current] = -e;
      vertices[-ee] = impl->vertices(current);
      break;
    }
  }

  Permutation<HalfEdge> vertexPermutation(vertices);

  auto ret = std::make_unique<FlatTriangulationCombinatorial>();
  ret->impl = spimpl::make_unique_impl<Implementation>(
      Permutation<HalfEdge>(vertices),
      vertexPermutation.domain() | filter([&](auto& edge) { return edge == e || edge == -ee || (edge != ee && boundary(edge)); }) | to_set());
  return ret;
}

vector<HalfEdge> FlatTriangulationCombinatorial::atVertex(const Vertex v) const {
  assert(std::any_of(impl->vertexes.begin(), impl->vertexes.end(), [&](Vertex vv) { return v == vv; }) && "vertex is not on this surface");
  return vector<HalfEdge>(v.impl->sources.begin(), v.impl->sources.end());
}

void FlatTriangulationCombinatorial::flip(HalfEdge e) {
  ASSERT_ARGUMENT(!this->boundary(e) && !this->boundary(-e), "cannot flip a boundary edge");

  // Let (e a b) and (-e c d) be the faces containing e and -e before the flip.
  const HalfEdge a = nextInFace(e);
  const HalfEdge b = nextInFace(a);
  const HalfEdge c = nextInFace(-e);
  const HalfEdge d = nextInFace(c);

  using cycle = vector<HalfEdge>;
  // flip e in "vertices"
  // (... b -a ...)(... a -e -d ...) -> (... b -e -a ...)(... a -d ...) so we
  // multiply vertices with (b a -e)
  // (... d -c ...)(... c e -b ...) -> (... d e -c ...)(... c -b ...) so we
  // multiply vertices with (d c e)
  impl->vertices *= cycle{b, a, -e};
  impl->vertices *= cycle{d, c, e};

  // flip e in "faces"
  // (a b e)(c d -e) -> (a -e d)(c e b), i.e., multiply with (a d e)(c b -e)
  impl->faces *= cycle{a, d, e};
  impl->faces *= cycle{c, b, -e};

  // notify attached structures about this flip
  impl->change.emit(Implementation::MessageAfterFlip{e});

  impl->resetVertexes();

  impl->check();
}

size_t FlatTriangulationCombinatorial::size() const noexcept {
  return impl->edges.size();
}

std::pair<HalfEdge, HalfEdge> FlatTriangulationCombinatorial::collapse(HalfEdge collapse) {
  if (this->boundary(collapse) || this->boundary(-collapse))
    throw std::logic_error("not implemented: cannot collapse boundary edge yet");

  if (nextInFace(nextInFace(nextInFace(collapse))) != collapse || nextInFace(nextInFace(nextInFace(-collapse))) != -collapse)
    throw std::logic_error("not implemented: cannot collapse collapsed edge yet");

  // notify attached structures about this collapse
  impl->change.emit(Implementation::MessageBeforeCollapse{collapse});

  // In principle, we will drop three pairs of half edges, namely e,
  // previousAtVertex(-e), nextAtVertex(-e).
  // Things get a bit more complicated when the faces at e and -e have
  // identified edges. In this case, we have to make sure we do not drop the
  // entire gadget surrounding e out of existence.
  std::set<Edge> dropEdges{collapse};

  {
    HalfEdge x = nextInFace(collapse);
    if (x == -previousInFace(collapse)) {
      // x must survive so we do not drop one entire side
    } else {
      dropEdges.insert(x);
    }
  }

  {
    HalfEdge y = -previousInFace(-collapse);
    if (nextInFace(collapse) == -nextInFace(-collapse) && -previousInFace(-collapse) == previousInFace(collapse)) {
      // y must survive so we do not drop the entire gadget out of existence
    } else if (y == nextInFace(-collapse)) {
      // y must survive so we do not drop one entire side
    } else {
      dropEdges.insert(y);
    }
  }

  // So we need to make sure that these have the highest numbers since many
  // bits of the code assume that there is no gap in the half edge numbering.
  {
    std::vector<Edge> originalDropEdges(dropEdges.rbegin(), dropEdges.rend());

    int maxEdge = static_cast<int>(edges().size());
    for (auto d : originalDropEdges) {
      while (maxEdge && dropEdges.find(Edge(maxEdge)) != dropEdges.end())
        maxEdge--;
      if (d.index() < edges().size() - dropEdges.size()) {
        HalfEdge swap = Edge(maxEdge).positive();
        impl->swap(d.positive(), swap);
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
  std::set<HalfEdge> dropHalfEdges;
  for (auto d : dropEdges) {
    dropHalfEdges.insert(d.positive());
    dropHalfEdges.insert(d.negative());
  }

  impl->change.emit(Implementation::MessageBeforeErase{dropEdges});

  // Consider the faces (collapse, x, -a) and (-collapse, c, -y).
  const HalfEdge a = -previousInFace(collapse);
  const HalfEdge c = nextInFace(-collapse);

  // Remove any mention of these edges from `faces`:
  // Consider again the faces (collapse, x, -a) and (-collapse, c, -y).
  // To collapse these faces, -a needs to take the place of -x …
  {
    const HalfEdge _a = previousInFace(collapse);
    assert(_a == -a && "permutations not consistent in face to collapse");
    const HalfEdge _x = -nextInFace(collapse);
    const HalfEdge p_x = previousInFace(_x);
    const HalfEdge x = nextInFace(collapse);
    impl->faces *= {_a, _x};
    impl->faces *= {p_x, x};
    impl->faces *= {previousInFace(collapse), collapse};
  }
  // … and c needs to take the place of y.
  {
    // Note that this might not be the original c anymore.
    const HalfEdge cc = nextInFace(-collapse);
    const HalfEdge y = -previousInFace(-collapse);
    const HalfEdge py = previousInFace(y);
    const HalfEdge _e = -collapse;
    impl->faces *= {cc, y};
    impl->faces *= {py, _e};
    impl->faces *= {previousInFace(-collapse), -collapse};
  }

  impl->faces.drop(dropHalfEdges);

  // Calculate vertex permutation from half edges, note that this might
  // separate vertices when a connection from a vertex to itself has been
  // collapsed.
  impl->resetVertices();

  // The vertices at the end of e need to merge (or separate.) We could do this
  // more efficiently, but it's probably not worth it.
  impl->resetVertexes();

  // This could also be done more efficiently.
  impl->resetEdges();

  impl->check();

  if (dropEdges.find(a) != dropEdges.end()) {
    assert(dropEdges.find(c) == dropEdges.end() && "an entire horizontal part of the surface has been dropped out of existence");
    return std::make_pair(-c, c);
  } else if (dropEdges.find(c) != dropEdges.end()) {
    assert(dropEdges.find(a) == dropEdges.end() && "an entire horizontal part of the surface has been dropped out of existence");
    return std::make_pair(a, -a);
  } else {
    return std::make_pair(a, c);
  }
}

Implementation<FlatTriangulationCombinatorial>::Implementation(const Permutation<HalfEdge>& vertices, const set<HalfEdge>& boundaries) :
  vertices(vertices),
  faces(
      // In the triangulation, the order in which half edges are attached to a
      // vertex defines the faces, so we reconstruct the faces here.
      vertices.domain() | transform([&](const HalfEdge& e) {
        return boundaries.find(e) == boundaries.end()
                   ? pair<HalfEdge, HalfEdge>(-vertices(e), e)
                   // We mark boundary half edges by putting them into a trivial face
                   // which only contains that half edge.
                   : pair<HalfEdge, HalfEdge>(e, e);
      }) |
      to_vector()),
  vertexes() {
  CHECK_ARGUMENT(vertices.size() % 2 == 0, "half edges must come in pairs");

  resetVertexes();
  resetEdges();

  // check that faces are triangles
  for (auto edge : faces.domain()) {
    if (faces(edge) != edge) {
      CHECK_ARGUMENT(faces(faces(faces(edge))) == edge, "not fully triangulated");
    } else {
      ASSERT_ARGUMENT(boundaries.find(edge) != boundaries.end(), "faces must not be trivial");
    }
  }

  check();
}

void Implementation<FlatTriangulationCombinatorial>::check() {
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

void Implementation<FlatTriangulationCombinatorial>::resetVertexes() {
  using HalfEdges = std::set<HalfEdge>;
  this->vertexes.clear();
  for (const auto& cycle : vertices.cycles()) {
    Vertex v;
    v.impl = spimpl::make_impl<Vertex::Implementation>(HalfEdges(cycle.begin(), cycle.end()));
    this->vertexes.push_back(v);
  }
}

void Implementation<FlatTriangulationCombinatorial>::resetVertices() {
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

void Implementation<FlatTriangulationCombinatorial>::resetEdges() {
  std::set<Edge> edges;
  for (const auto& e : faces.domain())
    edges.insert(e);
  this->edges = std::vector<Edge>(edges.begin(), edges.end());
}

void Implementation<FlatTriangulationCombinatorial>::swap(HalfEdge a, HalfEdge b) {
  if (a == b) return;

  change.emit(Implementation::MessageBeforeSwap{a, b});

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
}

Implementation<FlatTriangulationCombinatorial>::~Implementation() {
  change.emit(MessageAfterMove{nullptr});
}

bool FlatTriangulationCombinatorial::operator==(const FlatTriangulationCombinatorial& rhs) const noexcept {
  if (this == &rhs) return true;
  return this->impl->vertices == rhs.impl->vertices;
}

ostream& operator<<(ostream& os, const FlatTriangulationCombinatorial& self) {
  return os << "FlatTriangulationCombinatorial(vertices = "
            << self.impl->vertices << ", faces = " << self.impl->faces << ")";
}
}  // namespace flatsurf
