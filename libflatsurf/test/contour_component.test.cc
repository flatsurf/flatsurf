/**********************************************************************
 *  This file is part of flatsurf.
 *
 *        Copyright (C) 2019-2020 Vincent Delecroix
 *        Copyright (C) 2019-2025 Julian Rüth
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

#include <boost/lexical_cast.hpp>
#include <exact-real/element.hpp>
#include <exact-real/integer_ring.hpp>
#include <exact-real/number_field.hpp>
#include <intervalxt/interval_exchange_transformation.hpp>

#include "../flatsurf/contour_component.hpp"
#include "../flatsurf/contour_connection.hpp"
#include "../flatsurf/contour_decomposition.hpp"
#include "../flatsurf/flat_triangulation_collapsed.hpp"
#include "../flatsurf/interval_exchange_transformation.hpp"
#include "../flatsurf/path.hpp"
#include "../flatsurf/path_iterator.hpp"
#include "../flatsurf/saddle_connection.hpp"
#include "../flatsurf/saddle_connections.hpp"
#include "../flatsurf/vector.hpp"
#include "../flatsurf/vertical.hpp"
#include "external/catch2/single_include/catch2/catch.hpp"
#include "../src/external/rx-ranges/include/rx/ranges.hpp"
#include "generators/surface_generator.hpp"
#include "generators/vertical_generator.hpp"

namespace flatsurf::test {

using namespace flatsurf;

TEST_CASE("Area of a Contour Decomposition", "[ContourComponent][area]") {
  SECTION("A Surface with Multiple Components") {
    using T = long long;
    using R2 = Vector<T>;
    auto surface = makeL<R2>();
    CAPTURE(*surface);

    auto decomposition = ContourDecomposition<FlatTriangulation<T>>(surface->clone(), {2, 1});

    auto area = decomposition.components() | rx::transform([&](const auto& component) { return component.area2(); }) | rx::sum();
    REQUIRE(area == surface->area2());
  }
}

TEMPLATE_TEST_CASE("Connections and IET from Contour Decomposition", "[ContourDecomposition][intervalExchangeTransformation]", (long long), (mpz_class), (mpq_class), (eantic::renf_elem_class), (exactreal::Element<exactreal::IntegerRing>), (exactreal::Element<exactreal::RationalField>), (exactreal::Element<exactreal::NumberField>)) {
  using T = TestType;

  const auto surface = GENERATE_SURFACES(T);
  CAPTURE(surface);

  const auto saddleConnection = GENERATE_COPY(verticals<T>(surface));
  CAPTURE(saddleConnection);

  SECTION("The Contour Decomposition in that Direction can be Computed") {
    auto decomposition = ContourDecomposition<FlatTriangulation<T>>(surface->clone(), saddleConnection);

    CAPTURE(decomposition);
    CAPTURE(decomposition.collapsed());

    SECTION("We can construct the IETs from the components") {
      for (auto component : decomposition.components()) {
        REQUIRE(component.perimeter().tighten() == Path<FlatTriangulation<T>>{});
        auto iet = component.intervalExchangeTransformation();
        REQUIRE(iet.intervalExchangeTransformation().size() == component.topContour().size());
      }
    }
  }
}

}  // namespace flatsurf::test
