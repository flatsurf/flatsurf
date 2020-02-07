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

#include "external/catch2/single_include/catch2/catch.hpp"

#include <numeric>

#include <e-antic/renfxx_fwd.h>
#include <exact-real/element.hpp>
#include <exact-real/number_field.hpp>

#include "../flatsurf/bound.hpp"
#include "../flatsurf/edge.hpp"
#include "../flatsurf/flat_triangulation_collapsed.hpp"
#include "../flatsurf/saddle_connection.hpp"
#include "../flatsurf/saddle_connections.hpp"

#include "surfaces.hpp"

namespace flatsurf::test {
TEMPLATE_TEST_CASE("Collapse in a Flat Triangulation", "[flat_triangulation_collapsed][collapse]", (long long), (mpq_class), (renf_elem_class), (exactreal::Element<exactreal::IntegerRing>), (exactreal::Element<exactreal::NumberField>)) {
  using R2 = Vector<TestType>;

  auto contains = [](auto& surf, auto& vector) {
    return std::any_of(surf->halfEdges().begin(), surf->halfEdges().end(), [&](HalfEdge e) {
      return surf->fromEdge(e) == vector;
    });
  };

  {
    auto surface = makeSquare<R2>();

    GIVEN("The Square " << *surface) {
      for (auto sc : SaddleConnections(surface, Bound(3, 0))) {
        WHEN("We collapse in the direction of " << sc) {
          auto collapsed = FlatTriangulationCollapsed<TestType>::make(surface->clone(), sc);

          if (contains(surface, sc)) {
            THEN("We end up with " << *collapsed << " where everything is collapsed into a single edge") {
              REQUIRE(collapsed->edges().size() == 1);
              REQUIRE(collapsed->fromEdge(HalfEdge(1)) == -collapsed->fromEdge(HalfEdge(-1)));

              auto cross = collapsed->cross(HalfEdge(1));
              REQUIRE(cross.size() == 1);
              CAPTURE(*cross.begin());
              REQUIRE((*cross.begin() == sc || *cross.begin() == -sc));

              cross = collapsed->cross(HalfEdge(-1));
              REQUIRE(cross.size() == 1);
              CAPTURE(*cross.begin());
              REQUIRE((*cross.begin() == sc || *cross.begin() == -sc));
            }
          } else {
            THEN("Nothing happens, we are left with the same " << *collapsed) {
              REQUIRE(collapsed->edges().size() == surface->edges().size());
            }
          }
        }
      }
    }
  }

  {
    auto surface = makeL<R2>();

    GIVEN("The L " << *surface) {
      for (auto sc : SaddleConnections(surface, Bound(3, 0))) {
        WHEN("We collapse in the direction of " << sc) {
          auto collapsed = FlatTriangulationCollapsed<TestType>::make(surface->clone(), sc);

          if (contains(surface, sc)) {
            if (sc <= Bound(1, 0)) {
              THEN("We end up with " << *collapsed << " where everything is collapsed into two edges") {
                REQUIRE(collapsed->edges().size() == 2);
              }
            } else {
              THEN("We end up with " << *collapsed << " where everything is collapsed into a single edge") {
                REQUIRE(collapsed->edges().size() == 1);
              }
            }
          } else {
            THEN("Nothing happens, we are left with the same " << *collapsed) {
              REQUIRE(collapsed->edges().size() == surface->edges().size());
            }
          }
        }
      }
    }
  }
}

}  // namespace flatsurf::test
