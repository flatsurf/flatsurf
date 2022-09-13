/**********************************************************************
 *  This file is part of flatsurf.
 *
 *        Copyright (C) 2019-2020 Vincent Delecroix
 *        Copyright (C) 2019-2022 Julian Rüth
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

#include "../flatsurf/chain.hpp"
#include "../flatsurf/flat_triangulation.hpp"
#include "../flatsurf/half_edge.hpp"
#include "cereal.helpers.hpp"
#include "surfaces.hpp"

namespace flatsurf::test {
TEMPLATE_TEST_CASE("Chain Arithmetic", "[Chain][operator+=][operator+][operator-][operator==][operator!=]", (long long), (mpz_class), (mpq_class), (renf_elem_class), (exactreal::Element<exactreal::IntegerRing>), (exactreal::Element<exactreal::NumberField>)) {
  using R2 = Vector<TestType>;
  auto square = makeSquare<R2>();

  auto zero = Chain(*square);
  auto a = Chain(*square) += square->halfEdges()[0];
  auto b = Chain(*square) += square->halfEdges()[1];

  REQUIRE(!zero);
  REQUIRE(a);
  REQUIRE(b);

  SECTION("Addition & Subtraction") {
    REQUIRE(zero + zero == zero);
    REQUIRE(a + zero == a);
    REQUIRE(b + zero == b);

    REQUIRE(a + b == b + a);
    REQUIRE(a + b != a);
    REQUIRE(a + b - b - a == zero);
  }

  SECTION("Multiplication") {
    REQUIRE(zero * 1 == zero);
    REQUIRE(a * 1 == a);
    REQUIRE(b * 1 == b);

    REQUIRE(a * 1337 == a * 1336 + a);
    REQUIRE((a + b) * 1337 == a * 1337 + b * 1337);
  }
}

TEMPLATE_TEST_CASE("Serialization of a Chain", "[Chain][save][load]", (long long), (mpz_class), (mpq_class), (renf_elem_class), (exactreal::Element<exactreal::IntegerRing>), (exactreal::Element<exactreal::RationalField>), (exactreal::Element<exactreal::NumberField>)) {
  using R2 = Vector<TestType>;
  auto square = makeSquare<R2>();

  testRoundtrip(Chain(*square, HalfEdge(1)));
}

}  // namespace flatsurf::test
