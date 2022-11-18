/**********************************************************************
 *  This file is part of flatsurf.
 *
 *        Copyright (C) 2022 Julian Rüth
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

#include "../flatsurf/edge.hpp"
#include "../flatsurf/equivalence.hpp"
#include "../flatsurf/equivalence_class.hpp"
#include "generators/surface_generator.hpp"
#include "generators/equivalence_generator.hpp"

namespace flatsurf::test {

TEMPLATE_TEST_CASE("Equality of Equivalence Classes is Consistent", "[EquivalenceClass][operator==]", (long long), (mpz_class), (mpq_class), (renf_elem_class), (exactreal::Element<exactreal::IntegerRing>), (exactreal::Element<exactreal::RationalField>), (exactreal::Element<exactreal::NumberField>)) {
  using T = TestType;

  const auto surface = GENERATE_SURFACES(T);
  const auto equivalence = GENERATE_COPY(points(surface));

  const auto equivalence_class = EquivalenceClass(*surface, equivalence);

  SECTION("Equality is Reflexive") {
    REQUIRE(equivalence_class == equivalence_class);
  }

  SECTION("Equality is Symmetric") {
    const auto other = GENERATE_SURFACES(T);
    const auto other_class = EquivalenceClass(*other, equivalence);

    REQUIRE((equivalence_class == other_class) == (other_class == equivalence_class));

    if (equivalence_class == other_class) {
      SECTION("Equality is Transitive") {
        const auto third = GENERATE_SURFACES(T);
        const auto third_class = EquivalenceClass(*third, equivalence);

        if (other_class == third_class)
          REQUIRE(equivalence_class == third_class);
      }
    }
  }
}

}
