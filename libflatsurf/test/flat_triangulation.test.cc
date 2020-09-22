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

#include <e-antic/renfxx_fwd.h>
#include <fmt/format.h>
#include <fmt/ostream.h>

#include <exact-real/element.hpp>
#include <exact-real/number_field.hpp>
#include <memory>
#include <numeric>

#include "../flatsurf/flat_triangulation.hpp"
#include "../flatsurf/half_edge.hpp"
#include "../flatsurf/odd_half_edge_map.hpp"
#include "../flatsurf/saddle_connection.hpp"
#include "../flatsurf/saddle_connections.hpp"
#include "../flatsurf/vector.hpp"
#include "../src/external/rx-ranges/include/rx/ranges.hpp"
#include "external/catch2/single_include/catch2/catch.hpp"
#include "generators/surface_generator.hpp"
#include "surfaces.hpp"

namespace flatsurf::test {
TEMPLATE_TEST_CASE("Flip in a Flat Triangulation", "[flat_triangulation][flip]", (long long), (mpz_class), (mpq_class), (renf_elem_class), (exactreal::Element<exactreal::IntegerRing>), (exactreal::Element<exactreal::RationalField>), (exactreal::Element<exactreal::NumberField>)) {
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

TEMPLATE_TEST_CASE("Insert into a Flat Triangulation", "[flat_triangulation][insert][slit]", (long long), (mpz_class), (mpq_class), (renf_elem_class), (exactreal::Element<exactreal::IntegerRing>), (exactreal::Element<exactreal::RationalField>), (exactreal::Element<exactreal::NumberField>)) {
  using R2 = Vector<TestType>;

  SECTION("Insert into an L") {
    auto surface = makeL<R2>()->scale(3);

    SECTION("Insert without Flip") {
      auto sector = HalfEdge(1);
      REQUIRE(fmt::format("{}", *surface->insertAt(sector, R2(2, 1))) == "FlatTriangulationCombinatorial(vertices = (1, -10, 2, 3, 4, 5, -3, 6, 7, 8, -6, -2, -12, 9, -4, -5, -9, -11, -1, -7, -8)(10, 11, 12), faces = (1, -11, 10)(-1, -8, 7)(2, -6, -3)(-2, -10, 12)(3, 5, -4)(4, 9, -5)(6, 8, -7)(-9, -12, 11)) with vectors 1: (3, 0), 2: (3, 3), 3: (0, 3), 4: (-3, 0), 5: (-3, -3), 6: (3, 0), 7: (3, 3), 8: (0, 3), 9: (0, -3), 10: (-2, -1), 11: (1, -1), 12: (1, 2)");
    }

    SECTION("Insert without Flip onto Edge") {
      auto sector = HalfEdge(1);
      REQUIRE(fmt::format("{}", *surface->insertAt(sector, R2(1, 0))) == "FlatTriangulationCombinatorial(vertices = (1, 8, -6, -2, -12, 9, -4, -5, -9, -11, -7, -8, -10, 2, 3, 4, 5, -3, 6, 7)(-1, 11, 12, 10), faces = (1, 10, -8)(-1, 7, -11)(2, -6, -3)(-2, -10, 12)(3, 5, -4)(4, 9, -5)(6, 8, -7)(-9, -12, 11)) with vectors 1: (1, 3), 2: (3, 3), 3: (0, 3), 4: (-3, 0), 5: (-3, -3), 6: (3, 0), 7: (3, 3), 8: (0, 3), 9: (0, -3), 10: (-1, 0), 11: (2, 0), 12: (2, 3)");
    }

    SECTION("Insert with Single Flip onto Edge") {
      auto sector = HalfEdge(1);
      // Actually, we perform more than one flip. One would be enough but we
      // cannot handle inserts onto a half edge other than the sector boundary.
      REQUIRE(fmt::format("{}", *surface->insertAt(sector, R2(4, 1))) == "FlatTriangulationCombinatorial(vertices = (1, -10, 5, 9, 2, 4, -9, 6, 7, 8, -6, -5, -12, 3, -2, -4, -3, -11, -1, -7, -8)(10, 11, 12), faces = (1, -11, 10)(-1, -8, 7)(2, 3, -4)(-2, 9, 4)(-3, -12, 11)(5, -6, -9)(-5, -10, 12)(6, 8, -7)) with vectors 1: (3, 0), 2: (3, 3), 3: (-6, -3), 4: (-3, 0), 5: (9, 3), 6: (3, 0), 7: (3, 3), 8: (0, 3), 9: (6, 3), 10: (-4, -1), 11: (-1, -1), 12: (5, 2)");
    }

    SECTION("Insert with Several Flips") {
      auto sector = HalfEdge(1);
      REQUIRE(fmt::format("{}", *surface->insertAt(sector, R2(5, 1))) == "FlatTriangulationCombinatorial(vertices = (1, -10, 3, 5, 9, 4, -3, -12, 2, -9, 6, 7, 8, -6, -5, -4, -2, -11, -1, -7, -8)(10, 11, 12), faces = (1, -11, 10)(-1, -8, 7)(2, -4, 9)(-2, -12, 11)(3, 4, -5)(-3, -10, 12)(5, -6, -9)(6, 8, -7)) with vectors 1: (3, 0), 2: (-9, -3), 3: (12, 3), 4: (-3, 0), 5: (9, 3), 6: (3, 0), 7: (3, 3), 8: (0, 3), 9: (6, 3), 10: (-5, -1), 11: (-2, -1), 12: (7, 2)");
    }
  }

  SECTION("Slit at Many Places in the First Sector") {
    auto surface = GENERATE(values({std::shared_ptr(makeSquare<R2>()->scale(3)), std::shared_ptr(makeL<R2>()->scale(3))}));

    GIVEN("The surface " << *surface) {
      auto x = GENERATE(range(1, 32));
      auto y = GENERATE(range(1, 32));

      if (x > y) {
        bool crossesSingularity = false;
        int xx = x / std::gcd(x, y);
        int yy = y / std::gcd(x, y);
        for (int n = 1; xx * n <= x; n++) {
          if (xx * n % 3 == 0 && yy * n % 3 == 0)
            crossesSingularity = true;
        }

        if (!crossesSingularity) {
          R2 v = R2(x, y);
          HalfEdge e(1);
          WHEN("We Insert a Vertex at " << v << " next to " << e) {
            auto surf = surface->insertAt(e, v);

            CAPTURE(*surf);

            THEN("The Surface has Changed in the Right Way") {
              REQUIRE(*surface != *surf);
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
}

TEMPLATE_TEST_CASE("Deform a Flat Triangulation", "[flat_triangulation][deformation]", (long long), (mpz_class), (mpq_class), (renf_elem_class), (exactreal::Element<exactreal::IntegerRing>), (exactreal::Element<exactreal::RationalField>), (exactreal::Element<exactreal::NumberField>)) {
  using R2 = Vector<TestType>;

  const auto surface = makeL<R2>();

  SECTION("Trivially deform an L") {
    auto shift = OddHalfEdgeMap<R2>(*surface);
    REQUIRE(*surface->operator+(shift) == *surface);
  }

  SECTION("Stretch an L") {
    auto shift = OddHalfEdgeMap<R2>(*surface);

    shift.set(HalfEdge(8), R2(0, 1));
    shift.set(HalfEdge(7), R2(0, 1));

    REQUIRE(*surface->operator+(shift) != *surface);
  }
}

TEMPLATE_TEST_CASE("Eliminate Marked Points", "[flat_triangulation][eliminate_marked_points]", (long long), (mpz_class), (mpq_class), (renf_elem_class), (exactreal::Element<exactreal::IntegerRing>), (exactreal::Element<exactreal::RationalField>), (exactreal::Element<exactreal::NumberField>)) {
  using T = TestType;

  const auto [name, surface] = GENERATE(makeSurface<T>());
  GIVEN("The Surface " << *name) {
    const auto simplified = (*surface)->eliminateMarkedPoints();

    CAPTURE(*simplified);

    const auto unmarkedPoints = [](const auto& surface) {
      return surface.vertices() | rx::filter([&](const auto& vertex) { return surface.angle(vertex) != 1; }) | rx::count();
    };

    const auto markedPoints = [](const auto& surface) {
      return surface.vertices() | rx::filter([&](const auto& vertex) { return surface.angle(vertex) == 1; }) | rx::count();
    };

    if (unmarkedPoints(**surface)) {
      THEN("All Marked Points Can Be Removed") {
        REQUIRE(markedPoints(*simplified) == 0);
      }
    } else {
      THEN("All But A Single Marked Point Can Be Removed") {
        REQUIRE(markedPoints(*simplified) == 1);
        REQUIRE(unmarkedPoints(*simplified) == 0);
      }
    }

    REQUIRE((*surface)->area() == simplified->area());
  }
}

}  // namespace flatsurf::test
