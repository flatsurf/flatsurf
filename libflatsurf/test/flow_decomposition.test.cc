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

#include <exact-real/element.hpp>
#include <exact-real/integer_ring.hpp>
#include <exact-real/number_field.hpp>

#include "../flatsurf/bound.hpp"
#include "../flatsurf/flow_decomposition.hpp"
#include "../flatsurf/saddle_connection.hpp"
#include "../flatsurf/saddle_connections.hpp"
#include "../flatsurf/vector.hpp"

#include "generators/surface_generator.hpp"
#include "generators/saddle_connections_generator.hpp"

using eantic::renf_class;
using eantic::renf_elem_class;

namespace flatsurf::test {

using namespace flatsurf;

TEMPLATE_TEST_CASE("Flow Decomposition", "[flow_decomposition]", (long long), (mpz_class), (mpq_class), (renf_elem_class), (exactreal::Element<exactreal::IntegerRing>), (exactreal::Element<exactreal::RationalField>), (exactreal::Element<exactreal::NumberField>)) {
  using T = TestType;
  using R2 = Vector<T>;

  const auto surface = GENERATE(makeSurface<T>());

  GIVEN("The surface " << *surface) {
    const auto saddleConnection = GENERATE_COPY(saddleConnections<T>(surface));

    AND_GIVEN("A direction of a Saddle Connection " << saddleConnection) {
      THEN("The flow decomposition in that direction can be computed") {
        auto flowDecomposition = FlowDecomposition<FlatTriangulation<T>>(surface->clone(), saddleConnection);

        const auto area = [](const auto& decomposition) {
          T sum = T();
          for (const auto& component : decomposition.components()) sum += component.area();
          return sum;
        };

        // TODO: Norm areas!
        T scale = (static_cast<R2>(saddleConnection).x() * static_cast<R2>(saddleConnection).x() + static_cast<R2>(saddleConnection).y() * static_cast<R2>(saddleConnection).y());

        CAPTURE(flowDecomposition);
        REQUIRE(area(flowDecomposition) == scale * surface->area());

        REQUIRE(flowDecomposition.decompose());

        CAPTURE(flowDecomposition);
        REQUIRE(area(flowDecomposition) == scale * surface->area());
      }
    }
  }
}

}  // namespace flatsurf::test
