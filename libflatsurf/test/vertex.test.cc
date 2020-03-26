/**********************************************************************
 *  This file is part of flatsurf.
 *
 *        Copyright (C) 2020 Julian Rüth
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

#include "external/catch2/single_include/catch2/catch.hpp"

#include "../flatsurf/edge.hpp"
#include "../flatsurf/vertex.hpp"

#include "surfaces.hpp"

namespace flatsurf::test {

TEST_CASE("Total Angle Around Vertices", "[flat_triangulation][angle]") {
  using R2 = Vector<eantic::renf_elem_class>;

  SECTION("A Square Has No Singularities") {
    const auto square = makeSquare<R2>();
    for (auto vertex : square->vertices()) {
      REQUIRE(square->angle(vertex) == 1);
    }
  }

  SECTION("The Unfolding of the (1, 2, 3) Triangle Has No Singularities") {
    const auto _123 = make123<R2>();
    for (auto vertex : _123->vertices()) {
      REQUIRE(_123->angle(vertex) == 1);
    }
  }

  SECTION("The L Has A Single Singularity") {
    const auto L = makeL<R2>();
    REQUIRE(L->vertices().size() == 1);
    for (auto vertex : L->vertices()) {
      REQUIRE(L->angle(vertex) == 3);
    }
  }
}

TEST_CASE("Vertices And Modifications of Surfaces", "[vertex][flip]") {
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

}  // namespace flatsurf::test
