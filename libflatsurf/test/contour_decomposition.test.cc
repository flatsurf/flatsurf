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
#include "../flatsurf/flat_triangulation_collapsed.hpp"
#include "../flatsurf/contour_decomposition.hpp"
#include "../flatsurf/contour_component.hpp"
#include "../flatsurf/contour_connection.hpp"
#include "../flatsurf/saddle_connection.hpp"
#include "../flatsurf/saddle_connections.hpp"
#include "../flatsurf/vector.hpp"

#include "surfaces.hpp"

using eantic::renf_class;
using eantic::renf_elem_class;
using boost::lexical_cast;

namespace flatsurf::test {

using namespace flatsurf;

TEST_CASE("Perimeter of Contour Decomposition", "[contour_decomposition][perimeter]") {
  using T = long long;
  using R2 = Vector<T>;
  auto surface = makeSquare<R2>();

  auto decomposition = ContourDecomposition<FlatTriangulation<T>>(surface->clone(), {1, 1});

  CAPTURE(decomposition);
  REQUIRE(lexical_cast<std::string>(decomposition) == "[ContourComponent(ContourConnection(←SaddleConnection((0, -1) from -2)→SaddleConnection((1, 1) from 3))→ContourConnection(SaddleConnection((-1, -1) from -3)←SaddleConnection((0, 1) from 2)→))]");
}

TEMPLATE_TEST_CASE("Connections and IET from Contour Decomposition", "[contour_decomposition][iet]", (long long), (mpq_class), (renf_elem_class), (exactreal::Element<exactreal::IntegerRing>), (exactreal::Element<exactreal::NumberField>)) {
  using T = TestType;
  using R2 = Vector<T>;

  int bound;
  std::shared_ptr<FlatTriangulation<T>> surface;
  if constexpr (std::is_same_v<T, long long> || std::is_same_v<T, mpq_class> || std::is_same_v<T, exactreal::Element<exactreal::IntegerRing>>) {
    surface = GENERATE(makeSquare<R2>(), makeL<R2>());
    bound = 5;
  } else {
    surface = GENERATE(makeSquare<R2>(), makeL<R2>(), make1221<R2>(), makeHexagon<R2>(), makeHeptagonL<R2>(), makeGoldenL<R2>());
    bound = 2;
  }

  GIVEN("The surface " << *surface) {
    using  SaddleConnection = SaddleConnection<FlatTriangulation<T>>;

    for (auto sc : SaddleConnections<FlatTriangulation<T>>(surface, Bound(bound, 0))) {
      // TODO: Raises assertion in Catch
      // THEN("We can compute a ContourDecomposition in direction " << sc) {
        auto decomposition = ContourDecomposition<FlatTriangulation<T>>(surface->clone(), sc);
        AND_THEN("All connections show up once with both signs in this decomposition") {
          std::unordered_map<SaddleConnection, int> connections;

          auto track = [&](const SaddleConnection& connection) {
            CAPTURE(connection);

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
            CAPTURE(component);
            for (auto contourConnection : component.perimeter()) {
              track(contourConnection.connection());
              for (auto connection : contourConnection.left())
                track(connection);
              for (auto connection : contourConnection.right())
                track(connection);
            }
          }
        }

        AND_THEN("We can compute IETs in this direction") {
          for (auto component : decomposition.components()) {
            auto iet = component.intervalExchangeTransformation();
          }
        }
      //}
    }
  }
}

}  // namespace flatsurf::test
