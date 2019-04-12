/**********************************************************************
 *  This file is part of Flatsurf.
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
 *  along with Flatsurf. If not, see <https://www.gnu.org/licenses/>.
 *********************************************************************/

#include <ostream>
#include <vector>
#include "external/boolinq/include/boolinq/boolinq.h"

#include "libflatsurf/assert.hpp"
#include "libflatsurf/flat_triangulation_combinatorial.hpp"
#include "libflatsurf/half_edge.hpp"
#include "libflatsurf/permutation.hpp"

using boolinq::from;
using std::ostream;
using std::pair;
using std::vector;

namespace flatsurf {
struct FlatTriangulationCombinatorial::Implementation {
  Implementation(const Permutation<HalfEdge>& vertices)
      : vertices(vertices),
        // In the triangulation, the order in which half edges are attached to a
        // vertex defines the faces, so we reconstruct the faces here.
        faces(from(vertices.domain())
                  .select([&](const HalfEdge& e) {
                    return pair<HalfEdge, HalfEdge>(-vertices(e), e);
                  })
                  .toVector()) {}

  const Permutation<HalfEdge> vertices;
  const Permutation<HalfEdge> faces;
};

HalfEdge FlatTriangulationCombinatorial::nextInFace(const HalfEdge e) const {
  return impl->faces(e);
}

HalfEdge FlatTriangulationCombinatorial::nextAtVertex(const HalfEdge e) const {
  return impl->vertices(e);
}

const vector<HalfEdge>& FlatTriangulationCombinatorial::edges() const {
  return impl->vertices.domain();
}

FlatTriangulationCombinatorial::FlatTriangulationCombinatorial(
    const vector<vector<int>>& vertices)
    : FlatTriangulationCombinatorial(Permutation<HalfEdge>::create<int>(
          vertices, [](int e) { return HalfEdge(e); })) {}

FlatTriangulationCombinatorial::FlatTriangulationCombinatorial(
    const Permutation<HalfEdge>& vertices)
    : nedges(vertices.size() / 2),
      impl(spimpl::make_unique_impl<Implementation>(vertices)) {
  CHECK_ARGUMENT(vertices.size() % 2 == 0, "half edges must come in pairs");
  // check that faces are triangles
  for (auto edge : impl->faces.domain()) {
    CHECK_ARGUMENT(impl->faces(impl->faces(impl->faces(edge))) == edge,
                   "not fully triangulated");
  }
}

FlatTriangulationCombinatorial::FlatTriangulationCombinatorial(
    FlatTriangulationCombinatorial&& rhs)
    : nedges(rhs.impl->vertices.size() / 2), impl(std::move(rhs.impl)) {}

ostream& operator<<(ostream& os, const FlatTriangulationCombinatorial& self) {
  return os << "FlatTriangulationCombinatorial(vertices = "
            << self.impl->vertices << ", faces = " << self.impl->faces << ")";
}
}  // namespace flatsurf
