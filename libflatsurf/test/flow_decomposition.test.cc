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

#include <e-antic/renfxx.h>
#include <gmpxx.h>
#include <exact-real/element.hpp>
#include <exact-real/integer_ring.hpp>
#include <exact-real/number_field.hpp>

#include "../flatsurf/bound.hpp"
#include "../flatsurf/flow_decomposition.hpp"
#include "../flatsurf/saddle_connection.hpp"
#include "../flatsurf/saddle_connections.hpp"
#include "../flatsurf/vector.hpp"

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

TEMPLATE_TEST_CASE("Flow Decomposition", "[flow_decomposition]", (renf_elem_class)) {
  using T = TestType;

  const auto surface = GENERATE(makeSurface<T>());

  GIVEN("The surface " << *surface) {
    const auto vertical = GENERATE_COPY(verticals<T>(surface));

    AND_GIVEN("A direction of a Saddle Connection " << vertical) {
      THEN("The flow decomposition in that direction can be computed") {
        auto flowDecomposition = FlowDecomposition<FlatTriangulation<T>>(surface->clone(), vertical);

        const auto area = [](const auto& decomposition) {
          T sum = T();
          for (const auto& component : decomposition.components()) sum += component.area();
          return sum;
        };

        CAPTURE(flowDecomposition);
        REQUIRE(area(flowDecomposition) == surface->area());

        REQUIRE(flowDecomposition.decompose());

        CAPTURE(flowDecomposition);
        REQUIRE(area(flowDecomposition) == surface->area());
      }
    }
  }
}

}  // namespace flatsurf::test
