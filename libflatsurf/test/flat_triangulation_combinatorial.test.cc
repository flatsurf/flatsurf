/**********************************************************************
 *  This file is part of flatsurf.
 *
 *        Copyright (C)      2019 Vincent Delecroix
 *        Copyright (C) 2019-2024 Julian Rüth
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

#include <e-antic/renfxx_fwd.hpp>
#include <exact-real/element.hpp>
#include <exact-real/number_field.hpp>
#include <unordered_set>

#include "../flatsurf/edge.hpp"
#include "../flatsurf/flat_triangulation.hpp"
#include "../flatsurf/half_edge.hpp"
#include "../flatsurf/vertex.hpp"
#include "../flatsurf/odd_half_edge_map.hpp"
#include "cereal.helpers.hpp"
#include "generators/combinatorial_surface_generator.hpp"
#include "generators/half_edge_generator.hpp"

namespace flatsurf::test {

TEST_CASE("Comparisons and Hashing", "[FlatTriangulationCombinatorial][operator==]") {
  const auto surface = GENERATE(makeSurfaceCombinatorial());
  CAPTURE(*surface);

  REQUIRE(*surface == surface->clone());
  REQUIRE(std::hash<FlatTriangulationCombinatorial>{}(*surface) == std::hash<FlatTriangulationCombinatorial>{}(surface->clone()));
  REQUIRE(*surface != *makeSquareWithBoundaryCombinatorial());
}

TEST_CASE("Edges", "[FlatTriangulationCombinatorial][edges]") {
  const auto surface = GENERATE(makeSurfaceCombinatorial());
  CAPTURE(*surface);

  auto edges = surface->edges();

  REQUIRE(edges.size() == surface->halfEdges().size() / 2);

  // This is assumed by some tracking code for efficiency.
  REQUIRE(std::is_sorted(begin(edges), end(edges), [](const auto& lhs, const auto& rhs) { return lhs.index() < rhs.index(); }));
}

TEST_CASE("Half Edges", "[FlatTriangulationCombinatorial][halfEdges]") {
  const auto surface = GENERATE(makeSurfaceCombinatorial());
  CAPTURE(*surface);

  auto halfEdges = surface->halfEdges();

  // This is assumed by some tracking code for efficiency.
  REQUIRE(std::is_sorted(begin(halfEdges), end(halfEdges), [](const auto& lhs, const auto& rhs) { return lhs.index() < rhs.index(); }));
}

TEST_CASE("Faces", "[FlatTriangulationCombinatorial][face]") {
  const auto surface = GENERATE(makeSurfaceCombinatorial());
  CAPTURE(*surface);

  const auto halfEdge = GENERATE_COPY(halfEdges(surface));
  const auto face = surface->face(halfEdge);

  REQUIRE(std::find(begin(face), end(face), halfEdge) != face.end());
  REQUIRE(std::find(begin(face), end(face), surface->nextInFace(halfEdge)) != face.end());
  REQUIRE(std::find(begin(face), end(face), surface->previousInFace(halfEdge)) != face.end());
}

TEST_CASE("Vertices", "[FlatTriangulationCombinatorial][vertices]") {
  const auto surface = GENERATE(makeSurfaceCombinatorial());
  CAPTURE(*surface);

  auto vertices = surface->vertices();

  SECTION("Each Half Edge shows up as Outgoing Exactly Once") {
    if (!surface->hasBoundary()) {
      std::vector<HalfEdge> halfEdges;
      for (auto vertex : vertices) {
        for (auto halfEdge : surface->atVertex(vertex)) {
          halfEdges.push_back(halfEdge);
        }
      }
      REQUIRE(halfEdges.size() == surface->halfEdges().size());
    }
  }
}

TEST_CASE("Insertions", "[FlatTriangulationCombinatorial][insert]") {
  const auto surface = GENERATE(makeSurfaceCombinatorial());
  CAPTURE(*surface);

  auto e = surface->halfEdges()[0];

  WHEN("We Insert a Vertex Next to " << e) {
    auto inserted = surface->insertAt(e);
    CAPTURE(inserted);

    THEN("The Combinatorics Have Changed in the Expected Way") {
      REQUIRE(*surface != inserted);
      REQUIRE(surface->vertices().size() + 1 == inserted.vertices().size());
      REQUIRE(surface->halfEdges().size() + 6 == inserted.halfEdges().size());

      auto a = -inserted.nextAtVertex(e);
      REQUIRE(a != -surface->nextAtVertex(e));
      REQUIRE(inserted.nextAtVertex(inserted.nextAtVertex(inserted.nextAtVertex(a))) == a);
    }
  }
}

TEST_CASE("Relabeling", "[FlatTriangulationCombinatorial][relabel]") {
  const auto surface = GENERATE(makeSurfaceCombinatorial());
  CAPTURE(*surface);

  SECTION("We swap half edge 1 and its Negative") {
    std::unordered_map<HalfEdge, HalfEdge> swap;
    for (const auto& halfEdge : surface->halfEdges())
      swap[halfEdge] = halfEdge;

    swap[HalfEdge(1)] = HalfEdge(-1);
    swap[HalfEdge(-1)] = HalfEdge(1);

    const Permutation<HalfEdge> permutation{swap};
    CAPTURE(permutation);

    const auto swapped = surface->relabel(permutation);

    REQUIRE(swapped != *surface);

    const auto restored = swapped.relabel(permutation);

    REQUIRE(restored == *surface);
  }
}

TEST_CASE("Serialization", "[FlatTriangulationCombinatorial][save][load]") {
  auto square = makeSquareCombinatorial();

  testRoundtrip(*square);
}

}  // namespace flatsurf::test
