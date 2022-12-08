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
  using Surface = FlatTriangulation<T>;

  const auto surface = GENERATE_SURFACES(T);
  const auto equivalence = GENERATE_COPY(equivalences(surface));

  const auto equivalenceClass = EquivalenceClass(*surface, equivalence);

  SECTION("Equality is Reflexive") {
    REQUIRE(equivalenceClass == equivalenceClass);
  }

  SECTION("Equality is Symmetric") {
    const auto other = GENERATE_SURFACES(T);
    const auto otherClass = EquivalenceClass(*other, equivalence);

    REQUIRE((equivalenceClass == otherClass) == (otherClass == equivalenceClass));

    if (equivalenceClass == otherClass) {
      SECTION("Equality is Transitive") {
        const auto third = GENERATE_SURFACES(T);
        const auto thirdClass = EquivalenceClass(*third, equivalence);

        if (otherClass == thirdClass)
          REQUIRE(equivalenceClass == thirdClass);
      }
    }
  }

  SECTION("Equality Depends on the Notion of Equivalence") {
    // This custom equivalence, though it has the same implementation as
    // Equivalence::combinatorial(), is different from any other equivalence so
    // it's classes will be distinct from previously created classes.
    const auto anotherEquivalence = Equivalence<Surface>::combinatorial(true, [](const Surface&, Edge) { return true; });

    REQUIRE(EquivalenceClass(*surface, anotherEquivalence) != equivalenceClass);
    REQUIRE(EquivalenceClass(*surface, anotherEquivalence) == EquivalenceClass(*surface, anotherEquivalence));
  }
}

}
