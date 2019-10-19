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

#include <boost/range/adaptors.hpp>
#include <cstdint>
#include <ostream>
#include <unordered_map>
#include <vector>

#include "flatsurf/flat_triangulation_combinatorial.hpp"
#include "flatsurf/half_edge.hpp"
#include "flatsurf/half_edge_map.hpp"
#include "flatsurf/permutation.hpp"
#include "flatsurf/vertex.hpp"
#include "util/as_vector.ipp"
#include "util/assert.ipp"

using namespace flatsurf;
using boost::adaptors::transformed;
using std::function;
using std::ostream;
using std::pair;
using std::uintptr_t;
using std::unordered_map;
using std::vector;

namespace flatsurf {
namespace {
struct HalfEdgeMapProxy {
  function<void(HalfEdge, const FlatTriangulationCombinatorial&)> flip;
  function<void(const FlatTriangulationCombinatorial*)> relink;
};
}  // namespace

class FlatTriangulationCombinatorial::Implementation {
 public:
  Implementation(const Permutation<HalfEdge>& vertices)
      : vertices(vertices),
        // In the triangulation, the order in which half edges are attached to a
        // vertex defines the faces, so we reconstruct the faces here.
        faces(as_vector(vertices.domain() | transformed([&](const HalfEdge& e) {
                          return pair<HalfEdge, HalfEdge>(-vertices(e), e);
                        }))),
        halfEdgeMaps() {
    for (auto& c : vertices.cycles()) {
      vertexes.push_back(Vertex(c[0]));
    }
  }

  ~Implementation() {
    for (const auto& map : halfEdgeMaps) map.second.relink(nullptr);
  }

  Permutation<HalfEdge> vertices;
  Permutation<HalfEdge> faces;
  vector<Vertex> vertexes;
  mutable unordered_map<uintptr_t, HalfEdgeMapProxy> halfEdgeMaps;
};

HalfEdge FlatTriangulationCombinatorial::nextInFace(const HalfEdge e) const {
  return impl->faces(e);
}

HalfEdge FlatTriangulationCombinatorial::nextAtVertex(const HalfEdge e) const {
  return impl->vertices(e);
}

const vector<HalfEdge>& FlatTriangulationCombinatorial::halfEdges() const {
  return impl->vertices.domain();
}

const vector<Vertex>& FlatTriangulationCombinatorial::vertices() const {
  return impl->vertexes;
}

FlatTriangulationCombinatorial::FlatTriangulationCombinatorial()
    : FlatTriangulationCombinatorial(vector<vector<int>>()) {}

FlatTriangulationCombinatorial::FlatTriangulationCombinatorial(const vector<vector<int>>& vertices, std::set<int> boundaries)
    : FlatTriangulationCombinatorial(Permutation<HalfEdge>::create<int>(
          vertices, [](int e) { return HalfEdge(e); })) {
  if (boundaries.size()) {
    throw std::logic_error("not implemented: boundaries");
  }
}

FlatTriangulationCombinatorial::FlatTriangulationCombinatorial(const Permutation<HalfEdge>& vertices)
    : impl(spimpl::make_unique_impl<Implementation>(vertices)) {
  CHECK_ARGUMENT(vertices.size() % 2 == 0, "half edges must come in pairs");
  // check that faces are triangles
  for (auto edge : impl->faces.domain()) {
    CHECK_ARGUMENT(impl->faces(impl->faces(impl->faces(edge))) == edge,
                   "not fully triangulated");
  }
}

FlatTriangulationCombinatorial::FlatTriangulationCombinatorial(FlatTriangulationCombinatorial&& rhs) : FlatTriangulationCombinatorial() {
  *this = std::move(rhs);
}

FlatTriangulationCombinatorial& FlatTriangulationCombinatorial::operator=(FlatTriangulationCombinatorial&& rhs) noexcept {
  impl = std::move(rhs.impl);
  for (const auto& map : impl->halfEdgeMaps) map.second.relink(this);
  return *this;
}

std::unique_ptr<FlatTriangulationCombinatorial> FlatTriangulationCombinatorial::clone() const {
  return std::make_unique<FlatTriangulationCombinatorial>(impl->vertices);
}

vector<HalfEdge> FlatTriangulationCombinatorial::atVertex(const Vertex v) const {
  vector<HalfEdge> ret{v.representative};
  while (true) {
    HalfEdge next = nextAtVertex(ret[ret.size() - 1]);
    if (next == ret[0])
      return ret;
    ret.push_back(next);
  }
}

void FlatTriangulationCombinatorial::flip(HalfEdge e) {
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
  cycle{b, a, -e} *= impl->vertices;
  cycle{d, c, e} *= impl->vertices;

  // flip e in "faces"
  // (a b e)(c d -e) -> (a -e d)(c e b), i.e., multiply with (a d e)(c b -e)
  cycle{a, d, e} *= impl->faces;
  cycle{c, b, -e} *= impl->faces;

  // notify the half edge maps about this flip
  for (const auto& map : impl->halfEdgeMaps) map.second.flip(e, *this);
}

template <typename T>
void FlatTriangulationCombinatorial::registerMap(
    const HalfEdgeMap<T>& map) const {
  impl->halfEdgeMaps[reinterpret_cast<uintptr_t>(&map)] = HalfEdgeMapProxy{
      std::bind(
          [](const HalfEdgeMap<T>& self, HalfEdge halfEdge,
             const FlatTriangulationCombinatorial& parent) {
            self.updateAfterFlip(const_cast<HalfEdgeMap<T>&>(self), halfEdge,
                                 parent);
          },
          std::cref(map), std::placeholders::_1, std::placeholders::_2),
      std::bind(
          [](const HalfEdgeMap<T>& self, const FlatTriangulationCombinatorial* parent) {
            self.parent = parent;
          },
          std::cref(map), std::placeholders::_1)};
}

template <typename T>
void FlatTriangulationCombinatorial::deregisterMap(const HalfEdgeMap<T>& map) const {
  uintptr_t key = reinterpret_cast<uintptr_t>(&map);
  ASSERT_ARGUMENT(impl->halfEdgeMaps.find(key) != impl->halfEdgeMaps.end(), "map to deregister not found among registered maps");
  impl->halfEdgeMaps.erase(key);
}

bool FlatTriangulationCombinatorial::operator==(const FlatTriangulationCombinatorial& rhs) const noexcept {
  return this->impl->vertices == rhs.impl->vertices;
}

ostream& operator<<(ostream& os, const FlatTriangulationCombinatorial& self) {
  return os << "FlatTriangulationCombinatorial(vertices = "
            << self.impl->vertices << ", faces = " << self.impl->faces << ")";
}
}  // namespace flatsurf

using namespace flatsurf;

// Instantiations of templates so implementations are generated for the linker
#include <e-antic/renfxx.h>
#include <exact-real/element.hpp>
#include <exact-real/integer_ring.hpp>
#include <exact-real/number_field.hpp>
#include <exact-real/rational_field.hpp>

template void flatsurf::FlatTriangulationCombinatorial::registerMap(const HalfEdgeMap<int>&) const;
template void flatsurf::FlatTriangulationCombinatorial::registerMap(const HalfEdgeMap<long long>&) const;
template void flatsurf::FlatTriangulationCombinatorial::registerMap(const HalfEdgeMap<mpz_class>&) const;
template void flatsurf::FlatTriangulationCombinatorial::registerMap(const HalfEdgeMap<Vector<long long>>&) const;
template void flatsurf::FlatTriangulationCombinatorial::registerMap(const HalfEdgeMap<Vector<eantic::renf_elem_class>>&) const;
template void flatsurf::FlatTriangulationCombinatorial::registerMap(const HalfEdgeMap<Vector<exactreal::Element<exactreal::IntegerRing>>>&) const;
template void flatsurf::FlatTriangulationCombinatorial::registerMap(const HalfEdgeMap<Vector<exactreal::Element<exactreal::RationalField>>>&) const;
template void flatsurf::FlatTriangulationCombinatorial::registerMap(const HalfEdgeMap<Vector<exactreal::Element<exactreal::NumberField>>>&) const;
template void flatsurf::FlatTriangulationCombinatorial::deregisterMap(const HalfEdgeMap<int>&) const;
template void flatsurf::FlatTriangulationCombinatorial::deregisterMap(const HalfEdgeMap<long long>&) const;
template void flatsurf::FlatTriangulationCombinatorial::deregisterMap(const HalfEdgeMap<mpz_class>&) const;
template void flatsurf::FlatTriangulationCombinatorial::deregisterMap(const HalfEdgeMap<Vector<long long>>&) const;
template void flatsurf::FlatTriangulationCombinatorial::deregisterMap(const HalfEdgeMap<Vector<eantic::renf_elem_class>>&) const;
template void flatsurf::FlatTriangulationCombinatorial::deregisterMap(const HalfEdgeMap<Vector<exactreal::Element<exactreal::IntegerRing>>>&) const;
template void flatsurf::FlatTriangulationCombinatorial::deregisterMap(const HalfEdgeMap<Vector<exactreal::Element<exactreal::RationalField>>>&) const;
template void flatsurf::FlatTriangulationCombinatorial::deregisterMap(const HalfEdgeMap<Vector<exactreal::Element<exactreal::NumberField>>>&) const;
