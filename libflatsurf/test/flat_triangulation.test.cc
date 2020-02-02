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
#include <memory>

#include <fmt/format.h>
#include <fmt/ostream.h>
#include <e-antic/renfxx_fwd.h>
#include <exact-real/element.hpp>
#include <exact-real/number_field.hpp>

#include "../flatsurf/flat_triangulation.hpp"
#include "../flatsurf/half_edge.hpp"
#include "../flatsurf/saddle_connection.hpp"
#include "../flatsurf/saddle_connections.hpp"
#include "../flatsurf/vector.hpp"

#include "surfaces.hpp"

namespace flatsurf::test {
TEMPLATE_TEST_CASE("Flip in a Flat Triangulation", "[flat_triangulation][flip]", (long long), (mpq_class), (renf_elem_class), (exactreal::Element<exactreal::IntegerRing>), (exactreal::Element<exactreal::NumberField>)) {
  using R2 = Vector<TestType>;
  auto square = makeSquare<R2>();
  auto vertices = square->vertices();

  GIVEN("The Square " << *square) {
    auto halfEdge = GENERATE(as<HalfEdge>{}, 1, 2, 3, -1, -2, -3);
    THEN("Four Flips of a Half Edge Restore the Initial Surface") {
      const auto vector = square->fromEdge(halfEdge);
      square->flip(halfEdge);
      REQUIRE(vector != square->fromEdge(halfEdge));
      square->flip(halfEdge);
      REQUIRE(vector == -square->fromEdge(halfEdge));
      square->flip(halfEdge);
      square->flip(halfEdge);
      REQUIRE(vector == square->fromEdge(halfEdge));

      // a square (torus) has only a single vertex so it won't change; in general
      // it should not change, however, the representatives attached to a vertex
      // are currently not properly updated: https://github.com/flatsurf/flatsurf/issues/100
      REQUIRE(vertices == square->vertices());
    }
  }
}

TEMPLATE_TEST_CASE("Insert into a Flat Triangulation", "[flat_triangulation][insert][slot]", (long long), (mpq_class), (renf_elem_class), (exactreal::Element<exactreal::IntegerRing>), (exactreal::Element<exactreal::NumberField>)) {
  using R2 = Vector<TestType>;
  auto square = makeSquare<R2>()->scale(3);

  GIVEN("The Square " << *square) {
    auto x = GENERATE(range(1, 32));
    auto y = GENERATE(range(1, 32));

    if (x < y) {
      bool crossesSingularity = false;
      int xx = x / std::gcd(x, y);
      int yy = y / std::gcd(x, y);
      for (int n = 1; xx * n <= x; n++) {
        if (xx * n % 3 == 0 && yy * n % 3 == 0)
          crossesSingularity = true;
      }

      if (!crossesSingularity) {
        R2 v = R2(x, y);
        HalfEdge e(3);
        WHEN("We Insert a Vertex at " << v << " next to " << e) {
          auto surf = square->insertAt(e, v);

          CAPTURE(*surf);

          THEN("The Surface has Changed in the Right Way") {
            REQUIRE(*square != *surf);
            REQUIRE(surf->fromEdge(surf->nextAtVertex(e)) == v);
          }

          AND_WHEN("We Make a Slot There") {
            surf = surf->slot(surf->nextAtVertex(e));

            THEN("There is a Boundary at " << e) {
              REQUIRE(surf->boundary(surf->nextAtVertex(e)));
            }
          }
        }
      }
    }
  }
}

}  // namespace flatsurf::test
