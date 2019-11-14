/**********************************************************************
 *  This file is part of flatsurf.
 *
 *        Copyright (C) 2019 Vincent Delecroix
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

#include "catch.hpp"

#include <exact-real/element.hpp>
#include <exact-real/number_field.hpp>

#include <flatsurf/bound.hpp>
#include <flatsurf/flat_triangulation.hpp>
#include <flatsurf/half_edge.hpp>
#include <flatsurf/saddle_connection.hpp>
#include <flatsurf/saddle_connections.hpp>
#include <flatsurf/vector.hpp>
#include <flatsurf/vector_along_triangulation.hpp>

#include "surfaces.hpp"

namespace flatsurf::test {

TEMPLATE_TEST_CASE("Saddle Connections on a Torus", "[saddle_connections]", (long long), (mpq_class), (renf_elem_class), (exactreal::Element<exactreal::IntegerRing>), (exactreal::Element<exactreal::NumberField>)) {
  using R2 = Vector<TestType>;
  auto square = makeSquare<R2>();

  GIVEN("The Square " << *square) {
    auto bound = GENERATE(0, 2, 16);
    THEN("Saddle Connections Within a Bound of " << bound << " Correspond to Coprime Coordinates") {
      int expected = 0;
      for (int x = 1; x < bound + 1; x++)
        for (int y = 1; y <= x; y++)
          if (x * x + y * y < bound * bound && std::gcd(x, y) == 1)
            expected++;

      auto connections = SaddleConnections(square, bound);
      auto count = std::distance(connections.begin(), connections.end());

      REQUIRE(count == expected * 8);

      AND_THEN("Saddle Connections are Equally Distributed Next to the Half Edges") {
        auto [edge, required] = GENERATE_REF(table<HalfEdge, int>({{HalfEdge(1), expected}, {HalfEdge(2), 2 * expected}, {HalfEdge(3), expected}, {HalfEdge(-1), expected}, {HalfEdge(-2), 2 * expected}, {HalfEdge(-3), expected}}));

        CAPTURE(edge);
        connections = SaddleConnections(square, bound, edge);
        count = std::distance(connections.begin(), connections.end());
        REQUIRE(count == required);
      }
    }
  }
}

TEMPLATE_TEST_CASE("Saddle Connections on a Square With Boundary", "[saddle_connections]", (long long), (mpq_class), (renf_elem_class), (exactreal::Element<exactreal::IntegerRing>), (exactreal::Element<exactreal::NumberField>)) {
  using R2 = Vector<TestType>;
  auto square = makeSquareWithBoundary<R2>();

  GIVEN("The Square With Boundaries " << *square) {
    auto bound = GENERATE(2, 16);
    THEN("Saddle Connections Within a Bound of " << bound << " Are Essentially Trivial") {
      auto connections = SaddleConnections(square, bound);
      auto count = std::distance(connections.begin(), connections.end());

      REQUIRE(count == bound * 4);

      auto [edge, required] = GENERATE_REF(table<HalfEdge, int>({{HalfEdge(2), 0}, {HalfEdge(-4), 0}, {HalfEdge(1), 1}, {HalfEdge(-1), 1}, {HalfEdge(3), bound - 1}, {HalfEdge(-3), bound - 1}, {HalfEdge(-2), bound}, {HalfEdge(4), bound}}));

      CAPTURE(edge);
      connections = SaddleConnections(square, bound, edge);
      count = std::distance(connections.begin(), connections.end());
      REQUIRE(count == required);
    }
  }
}

TEMPLATE_TEST_CASE("Saddle Connections on a Hexagon", "[saddle_connections]", (renf_elem_class), (exactreal::Element<exactreal::NumberField>)) {
  using R2 = Vector<TestType>;
  auto hexagon = makeHexagon<R2>();

  GIVEN("The Hexagon " << *hexagon) {
    auto bound = Bound(16);
    auto [edge, required] = GENERATE(table<HalfEdge, int>({{HalfEdge(1), 10}, {HalfEdge(2), 36}, {HalfEdge(3), 26}, {HalfEdge(4), 18}, {HalfEdge(5), 8}, {HalfEdge(6), 10}}));

    CAPTURE(edge);
    auto connections = SaddleConnections(hexagon, bound, edge);
    auto count = std::distance(connections.begin(), connections.end());
    REQUIRE(count == required);
  }
}

}  // namespace flatsurf::test
