/**********************************************************************
 *  This file is part of flatsurf.
 *
 *        Copyright (C) 2019 Vincent Delecroix
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

#include <fmt/format.h>
#include <fmt/ostream.h>

#include <exact-real/element.hpp>
#include <exact-real/number_field.hpp>

#include "../flatsurf/bound.hpp"
#include "../flatsurf/ccw.hpp"
#include "../flatsurf/flat_triangulation.hpp"
#include "../flatsurf/half_edge.hpp"
#include "../flatsurf/orientation.hpp"
#include "../flatsurf/saddle_connection.hpp"
#include "../flatsurf/saddle_connections.hpp"
#include "../flatsurf/saddle_connections_by_length.hpp"
#include "../flatsurf/saddle_connections_by_length_iterator.hpp"
#include "../flatsurf/saddle_connections_iterator.hpp"
#include "../flatsurf/vector.hpp"
#include "external/catch2/single_include/catch2/catch.hpp"
#include "generators/saddle_connections_generator.hpp"
#include "generators/surface_generator.hpp"
#include "surfaces.hpp"

namespace flatsurf::test {

TEMPLATE_TEST_CASE("Saddle Connections on a Torus", "[saddle_connections]", (long long), (mpq_class), (renf_elem_class), (exactreal::Element<exactreal::IntegerRing>), (exactreal::Element<exactreal::NumberField>)) {
  using R2 = Vector<TestType>;
  auto square = makeSquare<R2>();

  GIVEN("The Square " << *square) {
    THEN("Saddle Connections in a Sector are between the Starting Half Edge (inclusive) and the Ending Half Edge (exclusive)") {
      const auto connections = square->connections().bound(2).sector(HalfEdge(1));
      auto search = begin(connections);

      REQUIRE(fmt::format("{}", *search) == "(1, 0) from 1 to -1");
      REQUIRE(++search == end(connections));
    }

    THEN("The Same With SaddleConnections as Sector Boundaries") {
      const auto connections = square->connections().bound(2).sector(SaddleConnection(square, HalfEdge(1)), SaddleConnection(square, square->nextAtVertex(HalfEdge(1))));
      auto search = begin(connections);

      REQUIRE(fmt::format("{}", *search) == "(1, 0) from 1 to -1");
      REQUIRE(++search == end(connections));
    }

    THEN("The Same With Vectors as Sector Boundaries") {
      const auto connections = square->connections().bound(2).sector(square->fromEdge(HalfEdge(1)), square->fromEdge(square->nextAtVertex(HalfEdge(1))));
      auto search = begin(connections);

      REQUIRE(fmt::format("{}", *search) == "(1, 0) from 1 to -1");
      REQUIRE(++search == end(connections));
    }

    THEN("Saddle Connections Within a Fixed Bound Correspond to Coprime Coordinates") {
      auto bound = GENERATE(0, 2, 16);

      int expected = 0;
      for (int x = 1; x < bound + 1; x++)
        for (int y = 1; y <= x; y++)
          if (x * x + y * y < bound * bound && std::gcd(x, y) == 1)
            expected++;

      {
        const auto connections = square->connections().bound(bound);
        const auto count = std::distance(begin(connections), end(connections));

        REQUIRE(count == expected * 8);
      }

      AND_THEN("Saddle Connections are Equally Distributed Next to the Half Edges") {
        const auto [edge, required] = GENERATE_REF(table<HalfEdge, int>({{HalfEdge(1), expected}, {HalfEdge(2), 2 * expected}, {HalfEdge(3), expected}, {HalfEdge(-1), expected}, {HalfEdge(-2), 2 * expected}, {HalfEdge(-3), expected}}));

        CAPTURE(edge);
        const auto connections = square->connections().bound(bound).sector(edge);
        const auto count = std::distance(begin(connections), end(connections));
        REQUIRE(count == required);
      }

      AND_THEN("We Find the Same Connections if we Iterate By Length") {
        auto count = 0;
        for (auto connection : square->connections().byLength()) {
          if (connection > Bound(bound, 0))
            break;
          count++;
        }
        REQUIRE(count == expected * 8);
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
      auto connections = square->connections().bound(bound);
      auto count = std::distance(begin(connections), end(connections));

      REQUIRE(count == bound * 4);

      auto [edge, required] = GENERATE_REF(table<HalfEdge, int>({{HalfEdge(2), 0}, {HalfEdge(-4), 0}, {HalfEdge(1), 1}, {HalfEdge(-1), 1}, {HalfEdge(3), bound - 1}, {HalfEdge(-3), bound - 1}, {HalfEdge(-2), bound}, {HalfEdge(4), bound}}));

      CAPTURE(edge);
      connections = square->connections().bound(bound).sector(edge);
      count = std::distance(begin(connections), end(connections));
      REQUIRE(count == required);
    }
  }
}

TEMPLATE_TEST_CASE("Saddle Connections on an L with an Added Slit", "[saddle_connections][slit]", (mpq_class), (renf_elem_class), (exactreal::Element<exactreal::RationalField>), (exactreal::Element<exactreal::NumberField>)) {
  using R2 = Vector<TestType>;
  auto L = makeL<R2>();

  auto slit = R2(TestType(3) / 13371337, TestType(2) / 13371337);
  auto source = HalfEdge(1);

  L = L->insertAt(source, slit);
  L = L->slot(L->nextAtVertex(source));

  GIVEN("The L with Slit " << *L) {
    auto bound = GENERATE(2, 16);
    THEN("We can count Saddle Connections Within the Bound of " << bound) {
      auto connections = L->connections().bound(bound);
      (void)std::distance(begin(connections), end(connections));
    }
  }
}

TEMPLATE_TEST_CASE("Saddle Connections on a Hexagon", "[saddle_connections]", (renf_elem_class), (exactreal::Element<exactreal::NumberField>)) {
  using R2 = Vector<TestType>;
  auto hexagon = makeHexagon<R2>();

  GIVEN("The Hexagon " << *hexagon) {
    auto bound = Bound(16, 0);
    auto [edge, required] = GENERATE(table<HalfEdge, int>({{HalfEdge(1), 10}, {HalfEdge(2), 36}, {HalfEdge(3), 26}, {HalfEdge(4), 18}, {HalfEdge(5), 8}, {HalfEdge(6), 10}}));

    CAPTURE(edge);
    auto connections = hexagon->connections().bound(bound).sector(edge);
    auto count = std::distance(begin(connections), end(connections));
    REQUIRE(count == required);
  }
}

TEMPLATE_TEST_CASE("Saddle Connections on a Surface", "[saddle_connections]", (long long), (mpz_class), (mpq_class), (renf_elem_class), (exactreal::Element<exactreal::IntegerRing>), (exactreal::Element<exactreal::RationalField>), (exactreal::Element<exactreal::NumberField>)) {
  using T = TestType;

  const auto [name, surface_] = GENERATE(makeSurface<T>());

  const auto surface = *surface_;

  GIVEN("The surface " << *name << ", i.e., " << *surface) {
    // Unfortunately, these tests take a long time (one minute in late 2020) so
    // we can only run this for a small number of saddle connections.
    const auto connection = GENERATE_REF(saddleConnections(surface, 3));
    const auto bound = Bound::upper(connection.vector());

    AND_GIVEN("The saddle connection " << connection) {
      THEN("There is exactly that saddle connection in the sector bounded by this connection") {
        const auto connections_ = surface->connections().bound(2 * bound).sector(connection, connection);
        REQUIRE(std::distance(begin(connections_), end(connections_)) == 1);
      }

      THEN("There is at least that saddle connection in the sector bounded by this connection's direction") {
        const auto connections_ = surface->connections().bound(2 * bound).sector(connection.vector(), connection.vector());
        REQUIRE(std::distance(begin(connections_), end(connections_)) >= 1);
      }

      THEN("There is at exactly that saddle connection in the sector bounded by this connection's direction and starting from the same half edge") {
        const auto connections_ = surface->connections().bound(2 * bound).sector(connection.vector(), connection.vector()).sector(connection.source());
        REQUIRE(std::distance(begin(connections_), end(connections_)) == 1);
      }
    }

    AND_GIVEN("Two connections that define sectors that cover the entire plane") {
      const auto sectorBegin = connection;
      const auto sectorEnd = GENERATE_REF(saddleConnections(surface, 3));

      if (sectorBegin != sectorEnd && Vertex::source(sectorBegin.source(), *surface) == Vertex::source(sectorEnd.source(), *surface)) {
        const auto connectionsAtSource = surface->connections().bound(bound).source(Vertex::source(sectorBegin.source(), *surface));
        const auto countAtSource = std::distance(begin(connectionsAtSource), end(connectionsAtSource));

        THEN("All connections can be found in either sector") {
          CAPTURE(sectorBegin);
          CAPTURE(sectorEnd);

          const auto first = surface->connections().bound(bound).sector(sectorBegin, sectorEnd);
          CAPTURE(std::distance(begin(first), end(first)));

          const auto second = surface->connections().bound(bound).sector(sectorEnd, sectorBegin);
          CAPTURE(std::distance(begin(second), end(second)));

          REQUIRE(std::distance(begin(first), end(first)) + std::distance(begin(second), end(second)) == countAtSource);
        }
      }

      if (sectorBegin.vector().ccw(sectorEnd.vector()) != CCW::COLLINEAR || sectorBegin.vector().orientation(sectorEnd.vector()) != ORIENTATION::SAME) {
        const auto connections = surface->connections().bound(bound);
        const auto count = std::distance(begin(connections), end(connections));

        THEN("All connections can be found in either sector given by the vectors of the connections") {
          CAPTURE(sectorBegin.vector());
          CAPTURE(sectorEnd.vector());

          const auto first = surface->connections().bound(bound).sector(sectorBegin.vector(), sectorEnd.vector());
          CAPTURE(std::distance(begin(first), end(first)));

          const auto second = surface->connections().bound(bound).sector(sectorEnd.vector(), sectorBegin.vector());
          CAPTURE(std::distance(begin(second), end(second)));

          REQUIRE(std::distance(begin(first), end(first)) + std::distance(begin(second), end(second)) == count);
        }
      }
    }
  }
}

}  // namespace flatsurf::test
