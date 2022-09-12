/**********************************************************************
 *  This file is part of flatsurf.
 *
 *        Copyright (C)      2019 Vincent Delecroix
 *        Copyright (C) 2019-2022 Julian Rüth
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
#include <numeric>

#include "../flatsurf/bound.hpp"
#include "../flatsurf/edge.hpp"
#include "../flatsurf/flat_triangulation_collapsed.hpp"
#include "../flatsurf/path.hpp"
#include "../flatsurf/path_iterator.hpp"
#include "../flatsurf/saddle_connection.hpp"
#include "../flatsurf/saddle_connections.hpp"
#include "external/catch2/single_include/catch2/catch.hpp"
#include "generators/saddle_connection_generator.hpp"
#include "surfaces.hpp"

namespace flatsurf::test {

TEMPLATE_TEST_CASE("Collapse in a Flat Triangulation", "[FlatTriangulationCollapsed]", (long long), (mpq_class), (renf_elem_class), (exactreal::Element<exactreal::IntegerRing>), (exactreal::Element<exactreal::NumberField>)) {
  using T = TestType;
  using R2 = Vector<TestType>;

  auto contains = [](auto& surf, auto& vector) {
    return std::any_of(surf->halfEdges().begin(), surf->halfEdges().end(), [&](HalfEdge e) {
      return surf->fromHalfEdge(e) == vector;
    });
  };

  SECTION("Collapse on a Square") {
    auto surface = GENERATE(value(makeSquare<R2>()));
    auto sc = GENERATE_COPY(saddleConnections<T>(surface));
    CAPTURE(surface);

    WHEN("We collapse in the direction of " << sc) {
      auto collapsed = FlatTriangulationCollapsed<TestType>(surface->clone(), sc);

      if (contains(surface, sc)) {
        THEN("We end up with " << collapsed << " where everything is collapsed into a single edge") {
          REQUIRE(collapsed.edges().size() == 1);
          REQUIRE(collapsed.fromHalfEdge(HalfEdge(1)) == -collapsed.fromHalfEdge(HalfEdge(-1)));

          auto cross = collapsed.cross(HalfEdge(1));
          REQUIRE(cross.size() == 1);
          CAPTURE(*cross.begin());
          REQUIRE((*cross.begin() == sc || *cross.begin() == -sc));

          cross = collapsed.cross(HalfEdge(-1));
          REQUIRE(cross.size() == 1);
          CAPTURE(*cross.begin());
          REQUIRE((*cross.begin() == sc || *cross.begin() == -sc));
        }
      } else {
        THEN("Nothing happens, we are left with the same " << collapsed) {
          REQUIRE(collapsed.edges().size() == surface->edges().size());
        }
      }
    }
  }

  SECTION("Collapse in an L") {
    auto surface = GENERATE(value(makeL<R2>()));
    auto sc = GENERATE_COPY(saddleConnections<T>(surface));
    CAPTURE(surface);

    WHEN("We collapse in the direction of " << sc) {
      auto collapsed = FlatTriangulationCollapsed<TestType>(surface->clone(), sc);

      if (contains(surface, sc)) {
        if (sc <= Bound(1, 0)) {
          THEN("We end up with " << collapsed << " where everything is collapsed into two edges") {
            REQUIRE(collapsed.edges().size() == 2);
          }
        } else {
          THEN("We end up with " << collapsed << " where everything is collapsed into a single edge") {
            REQUIRE(collapsed.edges().size() == 1);
          }
        }
      } else {
        THEN("Nothing happens, we are left with the same " << collapsed) {
          REQUIRE(collapsed.edges().size() == surface->edges().size());
        }
      }
    }
  }
}

}  // namespace flatsurf::test
