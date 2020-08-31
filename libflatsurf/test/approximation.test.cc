/**********************************************************************
 *  This file is part of flatsurf.
 *
 *        Copyright (C) 2020 Julian Rüth
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

#include "../src/impl/approximation.hpp"
#include "external/catch2/single_include/catch2/catch.hpp"
#include "generators/real_generator.hpp"

namespace flatsurf::test {

TEMPLATE_TEST_CASE("Approximation", "[arb]", LIBFLATSURF_TEST_REAL_TYPES) {
  using T = TestType;
  using Arb = exactreal::Arb;
  using Arf = exactreal::Arf;

  const auto arb = Approximation<T>::arb;

  SECTION("Integers Convert As Expected") {
    auto n = GENERATE(-3, 0, 3);

    const auto approx = arb(T(n), 64);
    CAPTURE(approx);
    if (approx.is_exact()) {
      REQUIRE(approx.equal(Arb(n)));
    } else {
      const auto bounds = static_cast<std::pair<Arf, Arf>>(approx);
      REQUIRE(bounds.first <= n);
      REQUIRE(bounds.second >= n);
    }
  }

  SECTION("Precision Matters More for Non-Dyadic Rationals") {
    if constexpr (hasFractions<T>) {
      const auto frac = ::flatsurf::test::frac<T, int>;

      {
        const auto approx = arb(frac(1, 2), 64);
        CAPTURE(approx);
        if (approx.is_exact()) {
          REQUIRE(approx.equal(arb(frac(1, 2), 1024)));
        } else {
          REQUIRE(!approx.equal(arb(frac(1, 2), 1024)));
        }
      }

      {
        const auto approx = arb(frac(1, 3), 64);
        CAPTURE(approx);
        REQUIRE(!approx.is_exact());
        REQUIRE(!approx.equal(arb(frac(1, 3), 1024)));
      }
    }
  }
}

}  // namespace flatsurf::test
