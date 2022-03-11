/**********************************************************************
 *  This file is part of flatsurf.
 *
 *        Copyright (C) 2020      Vincent Delecroix
 *        Copyright (C) 2020-2022 Julian Rüth
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

#include <boost/type_erasure/any_cast.hpp>
#include <intervalxt/interval_exchange_transformation.hpp>
#include <intervalxt/sample/lengths.hpp>

#include "../flatsurf/contour_decomposition.hpp"
#include "../flatsurf/contour_component.hpp"
#include "../flatsurf/flat_triangulation.hpp"
#include "../flatsurf/interval_exchange_transformation.hpp"
#include "../flatsurf/flat_triangulation_collapsed.hpp"
#include "../flatsurf/vector.hpp"
#include "external/catch2/single_include/catch2/catch.hpp"

#include "generators/surface_generator.hpp"
#include "surfaces.hpp"

namespace flatsurf::test {

TEMPLATE_TEST_CASE("Access intervalxt object underlying an Interval Exchange Transformation", "[interval_exchange_transformation]", (long long), (mpz_class), (mpq_class), (renf_elem_class), (exactreal::Element<exactreal::IntegerRing>), (exactreal::Element<exactreal::RationalField>), (exactreal::Element<exactreal::NumberField>)) {
  using T = TestType;

  const auto [name, surface_] = GENERATE(makeSurface<T>());

  const auto surface = *surface_;

  GIVEN("The surface " << *name << ", i.e., " << *surface << " we consider the vertical direction (0, 1)") {
    const auto decomposition = ContourDecomposition<FlatTriangulation<T>>(surface->clone(), Vector<T>{0, 1});

    for (const auto& component : decomposition.components()) {
      const auto iet = component.intervalExchangeTransformation();
      CAPTURE(iet);

      SECTION("We can Forget the Structure Coming from the Surface") {
        const auto jet = iet.forget();
        CAPTURE(jet);

        for (const auto& label : jet.top()) {
          const auto length = jet.lengths()->get(label);
          CAPTURE(boost::type_erasure::typeid_of(length).name());
          REQUIRE_NOTHROW(boost::type_erasure::any_cast<T>(length));
        }
      }
    }
  }
}

}  // namespace flatsurf::test
