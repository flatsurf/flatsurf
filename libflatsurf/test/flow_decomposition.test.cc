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

#include <e-antic/renfxx.h>
#include <gmpxx.h>

#include <exact-real/element.hpp>
#include <exact-real/integer_ring.hpp>
#include <exact-real/number_field.hpp>

#include "../flatsurf/bound.hpp"
#include "../flatsurf/flow_component.hpp"
#include "../flatsurf/flow_decomposition.hpp"
#include "../flatsurf/flow_triangulation.hpp"
#include "../flatsurf/saddle_connection.hpp"
#include "../flatsurf/saddle_connections.hpp"
#include "../flatsurf/vector.hpp"
#include "../src/external/rx-ranges/include/rx/ranges.hpp"
#include "external/catch2/single_include/catch2/catch.hpp"
#include "generators/surface_generator.hpp"
#include "generators/vertical_generator.hpp"
#include "surfaces.hpp"

using eantic::renf_class;
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
    // The limit 3 here is extremely low. Unfortunately, these tests take a
    // long time already (45 seconds in late 2020.)
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

        AND_THEN("Each of its components can be triangulated") {
          const auto triangulations = flowDecomposition.components() | rx::transform([](const auto& component) { return component.triangulation(); }) | rx::to_vector();
          REQUIRE((triangulations | rx::transform([](const auto& component) { return component.triangulation().area(); }) | rx::sum()) == surface->area());
          REQUIRE(flowDecomposition.triangulation().area() == surface->area());
        }
      }
    }
  }
}

}  // namespace flatsurf::test
