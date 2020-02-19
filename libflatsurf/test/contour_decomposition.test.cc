/**********************************************************************
 *  This file is part of flatsurf.
 *
 *        Copyright (C) 2019 - 2020 Vincent Delecroix
 *        Copyright (C) 2019 - 2020 Julian Rüth
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

#include <unordered_set>

#include <boost/lexical_cast.hpp>

#include "external/catch2/single_include/catch2/catch.hpp"

#include <exact-real/element.hpp>
#include <exact-real/integer_ring.hpp>
#include <exact-real/number_field.hpp>

#include <intervalxt/interval_exchange_transformation.hpp>

#include "../flatsurf/bound.hpp"
#include "../flatsurf/contour_component.hpp"
#include "../flatsurf/contour_connection.hpp"
#include "../flatsurf/contour_decomposition.hpp"
#include "../flatsurf/flat_triangulation_collapsed.hpp"
#include "../flatsurf/saddle_connection.hpp"
#include "../flatsurf/saddle_connections.hpp"
#include "../flatsurf/vector.hpp"
#include "../flatsurf/vertical.hpp"

#include "generators/surface_generator.hpp"
#include "generators/saddle_connections_generator.hpp"

using boost::lexical_cast;
using eantic::renf_class;
using eantic::renf_elem_class;

namespace flatsurf::test {

using namespace flatsurf;

TEST_CASE("Perimeter of Contour Decomposition", "[contour_decomposition][perimeter]") {
  SECTION("A Trivial Case") {
    using T = long long;
    using R2 = Vector<T>;
    auto surface = makeSquare<R2>();

    auto decomposition = ContourDecomposition<FlatTriangulation<T>>(surface->clone(), {1, 1});

    CAPTURE(decomposition);
    REQUIRE(lexical_cast<std::string>(decomposition) == "[ContourComponent(ContourConnection(←SaddleConnection((0, -1) from -2)→SaddleConnection((1, 1) from 3))→ContourConnection(SaddleConnection((-1, -1) from -3)←SaddleConnection((0, 1) from 2)→))]");
  }

  SECTION("A Complicated Surface With Some Collapsed Edges") {
    using T = renf_elem_class;
    using R2 = Vector<T>;
    auto surface = make125<R2>();

    auto decomposition = ContourDecomposition<FlatTriangulation<T>>(surface->clone(), { static_cast<R2>(-surface->fromEdge(5)).x() + 3, static_cast<R2>(surface->fromEdge(5)).x() });

    CAPTURE(decomposition);
		REQUIRE(lexical_cast<std::string>(decomposition) == "[ContourComponent(ContourConnection(←SaddleConnection(((1/2*x-2 ~ -1.2928932), (1/2*x-1 ~ -0.29289322)) from -24)→)→ContourConnection(←SaddleConnection(((-x+2 ~ 0.58578644), 0) from 13)→)→ContourConnection(←SaddleConnection(((-x+1 ~ -0.41421356), (-x+1 ~ -0.41421356)) from -4)→)→ContourConnection(←SaddleConnection(((x-1 ~ 0.41421356), 0) from -20)→)→ContourConnection(←SaddleConnection(((x-1 ~ 0.41421356), 0) from 15)→)→ContourConnection(←SaddleConnection(((-1/2*x-1 ~ -1.7071068), (1/2*x-1 ~ -0.29289322)) from 8)→)→ContourConnection(←SaddleConnection((1, 0) from 24)→)→ContourConnection(←SaddleConnection((1, 0) from 1)→)→ContourConnection(←SaddleConnection(((1/2*x+1 ~ 1.7071068), (-1/2*x+1 ~ 0.29289322)) from 24)→)→ContourConnection(←SaddleConnection(((x-2 ~ -0.58578644), 0) from 23)→SaddleConnection(((-2*x-1 ~ -3.8284271), (-x+1 ~ -0.41421356)) from 23))→ContourConnection(←SaddleConnection(((-x+1 ~ -0.41421356), 0) from -15)→)→ContourConnection(SaddleConnection(((-2*x-1 ~ -3.8284271), (-x+1 ~ -0.41421356)) from 23)←SaddleConnection(((-x+1 ~ -0.41421356), 0) from 20)→)→ContourConnection(←SaddleConnection(((x-1 ~ 0.41421356), (x-1 ~ 0.41421356)) from -20)→)→ContourConnection(←SaddleConnection(((-1/2*x+2 ~ 1.2928932), (-1/2*x+1 ~ 0.29289322)) from 13)→)→ContourConnection(←SaddleConnection((-1, 0) from -24)→)→ContourConnection(←SaddleConnection((-1, 0) from -1)→))]");
  }

  SECTION("A Complicated Case With Many Collapsed Edges") {
    using T = renf_elem_class;
    using R2 = Vector<T>;
    auto surface = make125<R2>();

    auto decomposition = ContourDecomposition<FlatTriangulation<T>>(surface->clone(), { static_cast<R2>(surface->fromEdge(5)).x() + 1, static_cast<R2>(surface->fromEdge(5)).x() });

    CAPTURE(decomposition);
    REQUIRE(lexical_cast<std::string>(decomposition) == "[ContourComponent(ContourConnection(←SaddleConnection(((-1/2*x ~ -0.70710678), (1/2*x-1 ~ -0.29289322)) from 22)→)→ContourConnection(←SaddleConnection(((-x+1 ~ -0.41421356), (-x+1 ~ -0.41421356)) from -4)→)→ContourConnection(←SaddleConnection(((x-1 ~ 0.41421356), 0) from -20)→)→ContourConnection(←SaddleConnection(((x-1 ~ 0.41421356), 0) from 15)→)→ContourConnection(←SaddleConnection(((-1/2*x ~ -0.70710678), (1/2*x-1 ~ -0.29289322)) from 3)→)→ContourConnection(←SaddleConnection((1, 0) from 1)→SaddleConnection(((3/2*x+1 ~ 3.1213203), (1/2*x ~ 0.70710678)) from 24))→ContourConnection(←SaddleConnection(((-x+1 ~ -0.41421356), 0) from 20)→)→ContourConnection(SaddleConnection(((1/2*x-2 ~ -1.2928932), (1/2*x-1 ~ -0.29289322)) from -24)←SaddleConnection(((-2*x+1 ~ -1.8284271), (-x+1 ~ -0.41421356)) from -15)←SaddleConnection(((1/2*x ~ 0.70710678), (-1/2*x+1 ~ 0.29289322)) from -22)→)→ContourConnection(←SaddleConnection(((x-1 ~ 0.41421356), (x-1 ~ 0.41421356)) from -20)→)→ContourConnection(←SaddleConnection(((1/2*x ~ 0.70710678), (-1/2*x+1 ~ 0.29289322)) from -3)→SaddleConnection(((1/2*x-2 ~ -1.2928932), (1/2*x-1 ~ -0.29289322)) from -24))→ContourConnection(SaddleConnection(((-3/2*x-1 ~ -3.1213203), (-1/2*x ~ -0.70710678)) from 20)←SaddleConnection(((-x+1 ~ -0.41421356), 0) from -15)→SaddleConnection(((-2*x+1 ~ -1.8284271), (-x+1 ~ -0.41421356)) from -15))→ContourConnection(←SaddleConnection((-1, 0) from -1)→))]");
  }
}

TEMPLATE_TEST_CASE("Connections and IET from Contour Decomposition", "[contour_decomposition][iet]", (long long), (mpz_class), (mpq_class), (renf_elem_class), (exactreal::Element<exactreal::IntegerRing>), (exactreal::Element<exactreal::RationalField>), (exactreal::Element<exactreal::NumberField>)) {
  using T = TestType;

  // TODO: Use this everywhere to fix broken SECTION inside loop.
  const auto surface = GENERATE(makeSurface<T>());

  GIVEN("The surface " << *surface) {
    const auto saddleConnection = GENERATE_COPY(saddleConnections<T>(surface));

    AND_GIVEN("A direction of a Saddle Connection " << saddleConnection) {
      THEN("The Contour Decomposition in that Direction can be Computed") {
        auto decomposition = ContourDecomposition<FlatTriangulation<T>>(surface->clone(), saddleConnection);

        // All connections show up once with both signs in the decomposition.
        std::unordered_map<SaddleConnection<FlatTriangulation<T>>, int> connections;

        auto track = [&](const auto& connection) {
          if (connections.find(connection) == connections.end()) {
            connections[connection] = 1;
            connections[-connection] = 0;
          } else {
            connections[connection]++;
            REQUIRE(connections[connection] == 1);
            REQUIRE(connections[-connection] == 1);
          }
        };

        for (auto component : decomposition.components()) {
          const auto vertical = decomposition.collapsed()->vertical();
          CAPTURE(component);
          for (auto contourConnection : component.perimeterContour()) {
            const bool top = contourConnection.top();
            REQUIRE(vertical.perpendicular(contourConnection.connection()) != 0);
            track(top ? -contourConnection.connection() : contourConnection.connection());
            for (auto connection : contourConnection.left()) {
              REQUIRE(vertical.perpendicular(connection) == 0);
              REQUIRE(vertical.parallel(top ? -connection : connection) > 0);
              track(top ? connection : -connection);
            }
            for (auto connection : contourConnection.right()) {
              REQUIRE(vertical.perpendicular(connection) == 0);
              REQUIRE(vertical.parallel(top ? -connection : connection) > 0);
              track(top ? -connection : connection);
            }
          }
        }
        
        AND_THEN("We can construct the IETs from the components") {
          for (auto component : decomposition.components()) {
            auto iet = component.intervalExchangeTransformation();
          }
        }
      }
    }
  }
}

}  // namespace flatsurf::test
