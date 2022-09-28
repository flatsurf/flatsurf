/**********************************************************************
 *  This file is part of flatsurf.
 *
 *        Copyright (C) 2020-2022 Julian Rüth
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

#include "../flatsurf/edge.hpp"
#include "../flatsurf/vertex.hpp"
#include "cereal.helpers.hpp"
#include "surfaces.hpp"

namespace flatsurf::test {

TEST_CASE("Vertices And Modifications of Surfaces", "[Vertex][flip][source][target]") {
  auto hexagon = makeHexagonCombinatorial();

  REQUIRE(hexagon->vertices().size() == 2);

  SECTION("vertices() updated after flip()") {
    Edge fixed = *std::find_if(begin(hexagon->halfEdges()), end(hexagon->halfEdges()), [&](const auto halfEdge) { return Vertex::source(halfEdge, *hexagon) != Vertex::target(halfEdge, *hexagon); });

    for (HalfEdge flip : hexagon->halfEdges()) {
      if (Edge(flip) == fixed) continue;

      hexagon->flip(flip);

      REQUIRE(Vertex::source(fixed.positive(), *hexagon) != Vertex::target(fixed.positive(), *hexagon));
      REQUIRE(hexagon->vertices().size() == 2);
    }
  }
}

TEST_CASE("Serialization of a Vertex", "[Vertex][save][load]") {
  auto square = makeSquare<Vector<long long>>();
  testRoundtrip(square->vertices()[0]);
}

}  // namespace flatsurf::test
