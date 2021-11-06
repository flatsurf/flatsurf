/**********************************************************************
 *  This file is part of flatsurf.
 *
 *        Copyright (C) 2019 Vincent Delecroix
 *        Copyright (C) 2019-2021 Julian Rüth
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

#include <e-antic/renfxx.h>
#include <gmpxx.h>

#include <exact-real/element.hpp>
#include <exact-real/integer_ring.hpp>
#include <exact-real/number_field.hpp>

#include "../flatsurf/bound.hpp"
#include "../flatsurf/ccw.hpp"
#include "../flatsurf/flow_component.hpp"
#include "../flatsurf/flow_connection.hpp"
#include "../flatsurf/flow_decomposition.hpp"
#include "../flatsurf/flow_triangulation.hpp"
#include "../flatsurf/orientation.hpp"
#include "../flatsurf/saddle_connection.hpp"
#include "../flatsurf/saddle_connections.hpp"
#include "../flatsurf/vector.hpp"
#include "../flatsurf/vertical.hpp"
#include "../src/external/rx-ranges/include/rx/ranges.hpp"
#include "external/catch2/single_include/catch2/catch.hpp"
#include "generators/surface_generator.hpp"
#include "generators/vertical_generator.hpp"
#include "surfaces.hpp"

using eantic::renf_elem_class;

namespace flatsurf::test {

using namespace flatsurf;

TEST_CASE("Flow Decompositions Over Number Fields", "[flow_decomposition]") {
  using T = renf_elem_class;

  SECTION("A Complicated Direction For A Veech Cathedral") {
    const auto surface = makeCathedralVeech<Vector<T>>();
    CAPTURE(*surface);

    auto a = N->gen();

    const auto direction = Vector<T>(a + mpq_class(1, 2), 1);

    auto flowDecomposition = FlowDecomposition<FlatTriangulation<T>>(surface->clone(), direction);
    CAPTURE(flowDecomposition);

    REQUIRE(flowDecomposition.decompose());

    REQUIRE(flowDecomposition.components().size() == 5);
  }
}

TEMPLATE_TEST_CASE("Flow Decomposition", "[flow_decomposition]", (long long), (mpz_class), (mpq_class), (renf_elem_class), (exactreal::Element<exactreal::IntegerRing>), (exactreal::Element<exactreal::RationalField>), (exactreal::Element<exactreal::NumberField>)) {
  using T = TestType;

  const auto [name, surface_] = GENERATE(makeSurface<T>());

  const auto surface = *surface_;

  GIVEN("The surface " << *name << ", i.e., " << *surface) {
    // The limit 6 here is somewhat low. Unfortunately, these tests take some
    // time already (45 seconds in early 2021.)
    const auto vertical = GENERATE_COPY(verticals<T>(surface, 6));

    AND_GIVEN("A direction of a Saddle Connection " << vertical) {
      THEN("The flow decomposition in that direction can be computed") {
        auto flowDecomposition = FlowDecomposition<FlatTriangulation<T>>(surface->clone(), vertical);

        const auto area = [](const auto& components) {
          return components | rx::transform([](const auto& component) { return component.area(); }) | rx::sum();
        };

        CAPTURE(flowDecomposition);
        REQUIRE(area(flowDecomposition.components()) == surface->area());

        REQUIRE(flowDecomposition.decompose());

        CAPTURE(flowDecomposition);
        REQUIRE(area(flowDecomposition.components()) == surface->area());

        AND_THEN("The Area of the Cylinders is Consistent with Their Width & Height (modulo some scaling)") {
          for (const auto& component : flowDecomposition.components()) {
            if (component.cylinder()) {
              REQUIRE(component.holonomy() == std::vector{component.circumferenceHolonomy()});
              REQUIRE(2 * component.width() * component.height() == component.vertical().vertical() * component.vertical().vertical() * component.area());
            }
          }
        }

        AND_THEN("Each of its components can be triangulated") {
          const auto triangulations = flowDecomposition.components() | rx::transform([](const auto& component) { return component.triangulation(); }) | rx::to_vector();
          REQUIRE((triangulations | rx::transform([](const auto& component) { return component.triangulation().area(); }) | rx::sum()) == surface->area());
          REQUIRE(flowDecomposition.triangulation().area() == surface->area());
        }

        AND_THEN("Each component's bottom contour goes to the right") {
          for (const auto& component : flowDecomposition.components()) {
            for (const auto& bottom : component.bottom()) {
              CAPTURE(bottom);
              REQUIRE(!bottom.top());
              REQUIRE(vertical.ccw(bottom.saddleConnection()) != CCW::COUNTERCLOCKWISE);
            }
            REQUIRE(vertical.ccw(begin(component.bottom())->saddleConnection()) == CCW::CLOCKWISE);
            REQUIRE(vertical.ccw(rbegin(component.bottom())->saddleConnection()) == CCW::CLOCKWISE);
          }
        }

        AND_THEN("Each component's right contour goes to the top") {
          for (const auto& component : flowDecomposition.components()) {
            for (const auto& right : component.right()) {
              CAPTURE(right);
              REQUIRE(right.parallel());
              REQUIRE(vertical.ccw(right.saddleConnection()) == CCW::COLLINEAR);
              REQUIRE(vertical.orientation(right.saddleConnection()) == ORIENTATION::SAME);
            }
          }
        }

        AND_THEN("Each component's top contour goes to the left") {
          for (const auto& component : flowDecomposition.components()) {
            for (const auto& top : component.top()) {
              CAPTURE(top);
              REQUIRE(!top.bottom());
              REQUIRE(vertical.ccw(top.saddleConnection()) != CCW::CLOCKWISE);
            }
            REQUIRE(vertical.ccw(begin(component.top())->saddleConnection()) == CCW::COUNTERCLOCKWISE);
            REQUIRE(vertical.ccw(rbegin(component.top())->saddleConnection()) == CCW::COUNTERCLOCKWISE);
          }
        }

        AND_THEN("Each component's left contour goes to the bottom") {
          for (const auto& component : flowDecomposition.components()) {
            for (const auto& left : component.left()) {
              CAPTURE(left);
              REQUIRE(left.antiparallel());
              REQUIRE(vertical.ccw(left.saddleConnection()) == CCW::COLLINEAR);
              REQUIRE(vertical.orientation(left.saddleConnection()) == ORIENTATION::OPPOSITE);
            }
          }
        }

        AND_THEN("Each component's perimeter is made up of bottom + right + top + left") {
          for (const auto& component : flowDecomposition.components()) {
            auto perimeter = component.perimeter();
            for (const auto bottom : component.bottom()) {
              REQUIRE(*begin(perimeter) == bottom);
              perimeter.pop_front();
            }
            for (const auto right : component.right()) {
              REQUIRE(*begin(perimeter) == right);
              perimeter.pop_front();
            }
            for (const auto top : component.top()) {
              REQUIRE(*begin(perimeter) == top);
              perimeter.pop_front();
            }
            for (const auto left : component.left()) {
              REQUIRE(*begin(perimeter) == left);
              perimeter.pop_front();
            }
            REQUIRE(perimeter.size() == 0);
          }
        }

        AND_THEN("The Boundary Connections Are Consistent") {
          const bool single = flowDecomposition.components().size() == 1;

          for (const auto& component : flowDecomposition.components()) {
            auto perimeter = component.perimeter();
            for (const auto& connection : perimeter) {
              if (single)
                REQUIRE(!connection.boundary());
              REQUIRE(connection.boundary() == (-connection).boundary());
            }

            if (!single)
              REQUIRE((perimeter | rx::any_of([](const auto& connection) { return connection.boundary(); })));
          }
        }

        AND_THEN("Each Component knows about the Decomposition that Created it") {
          for (const auto& component : flowDecomposition.components())
            REQUIRE(component.decomposition() == flowDecomposition);

          for (auto& component : flowDecomposition.components())
            REQUIRE(component.decomposition() == flowDecomposition);
        }
      }
    }
  }
}

}  // namespace flatsurf::test
