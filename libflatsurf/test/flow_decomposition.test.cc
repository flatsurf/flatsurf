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

#include "surfaces.hpp"

using eantic::renf_class;
using eantic::renf_elem_class;

namespace flatsurf::test {

using namespace flatsurf;

TEMPLATE_TEST_CASE("Flow Decomposition", "[flow_decomposition]", (long long), (mpq_class), (renf_elem_class), (exactreal::Element<exactreal::IntegerRing>), (exactreal::Element<exactreal::NumberField>)) {
  using T = TestType;
  using R2 = Vector<T>;

  int bound;
  std::shared_ptr<FlatTriangulation<T>> surface;
  if constexpr (std::is_same_v<T, long long> || std::is_same_v<T, mpq_class> || std::is_same_v<T, exactreal::Element<exactreal::IntegerRing>>) {
    surface = GENERATE(makeSquare<R2>(), makeL<R2>());
    bound = 10;
  } else {
    surface = GENERATE(makeSquare<R2>(), makeL<R2>(), makeHexagon<R2>(), makeHeptagonL<R2>(), makeGoldenL<R2>(), make1221<R2>());
    bound = 5;
  }

  GIVEN("The surface " << *surface) {
    for (auto sc : SaddleConnections<FlatTriangulation<T>>(surface, Bound(bound, 0))) {
      THEN("The flow decomposition in direction " << static_cast<R2>(sc) << " can be computed") {
        auto flowDecomposition = FlowDecomposition<FlatTriangulation<T>>(surface->clone(), sc);

        const auto area = [](const auto& decomposition) {
          T sum = T();
          for (const auto& component : decomposition.components()) sum += component.area();
          return sum;
        };

        // TODO: Norm areas!
        T scale = (static_cast<R2>(sc).x() * static_cast<R2>(sc).x() + static_cast<R2>(sc).y() * static_cast<R2>(sc).y());

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
