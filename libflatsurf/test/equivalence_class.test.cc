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
  CAPTURE(surface);

  const auto equivalence = GENERATE_COPY(equivalences(surface));
  CAPTURE(equivalence);

  const auto equivalenceClass = EquivalenceClass(*surface, equivalence);
  CAPTURE(equivalenceClass);

  SECTION("Equality is Reflexive") {
    REQUIRE(equivalenceClass == equivalenceClass);
  }

  // We cannot compare equivalence classes of surfaces defined over different
  // (exact-real) number fields. This is a limitation in exact-real and e-antic, see
  // https://github.com/flatsurf/exact-real/issues/166 and
  // https://github.com/flatsurf/e-antic/issues/254
  if constexpr (!hasNumberFieldElements<T>) {
    SECTION("Equality is Symmetric") {
      const auto other = GENERATE_SURFACES(T);
      const auto otherClass = EquivalenceClass(*other, equivalence);
      CAPTURE(other);
      CAPTURE(otherClass);

      REQUIRE((equivalenceClass == otherClass) == (otherClass == equivalenceClass));

      if (equivalenceClass == otherClass) {
        SECTION("Equality is Transitive") {
          const auto third = GENERATE_SURFACES(T);
          const auto thirdClass = EquivalenceClass(*third, equivalence);
          CAPTURE(third);
          CAPTURE(thirdClass);

          if (otherClass == thirdClass)
            REQUIRE(equivalenceClass == thirdClass);
        }
      }
    }
  }
}

TEMPLATE_TEST_CASE("Representative of Equivalence Classes", "[EquivalenceClass][representative]", (long long), (mpz_class), (mpq_class), (renf_elem_class), (exactreal::Element<exactreal::IntegerRing>), (exactreal::Element<exactreal::RationalField>), (exactreal::Element<exactreal::NumberField>)) {
  using T = TestType;

  const auto surface = GENERATE_SURFACES(T);
  CAPTURE(surface);

  const auto equivalence = GENERATE_COPY(equivalences(surface));
  CAPTURE(equivalence);

  const auto equivalenceClass = EquivalenceClass(*surface, equivalence);
  CAPTURE(equivalenceClass);

  const auto& representative = equivalenceClass.representative();
  CAPTURE(representative);
  
  REQUIRE(EquivalenceClass(representative, equivalence) == equivalenceClass);
}


TEMPLATE_TEST_CASE("Automorphisms of Surfaces", "[EquivalenceClass][automorphisms]", (long long), (mpz_class), (mpq_class), (renf_elem_class), (exactreal::Element<exactreal::IntegerRing>), (exactreal::Element<exactreal::RationalField>), (exactreal::Element<exactreal::NumberField>)) {
  using T = TestType;

  const auto surface = GENERATE_SURFACES(T);
  CAPTURE(surface);

  const auto equivalence = GENERATE_COPY(equivalences(surface));
  CAPTURE(equivalence);

  const auto equivalenceClass = EquivalenceClass(*surface, equivalence);
  CAPTURE(equivalenceClass);

  REQUIRE(equivalenceClass.automorphisms() >= 1);
}
}
