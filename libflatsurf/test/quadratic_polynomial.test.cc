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

#include <exact-real/arb.hpp>
#include "external/catch2/single_include/catch2/catch.hpp"

#include "../src/impl/quadratic_polynomial.hpp"

#include "generators/real_generator.hpp"

namespace flatsurf::test {

TEMPLATE_TEST_CASE("Quadratic Polynomial", "[quadratic_polynomial]", LIBFLATSURF_TEST_REAL_TYPES) {
  using T = TestType;
  using Arb = exactreal::Arb;

  const auto REQUIRE_eq = [](const std::optional<exactreal::Arb>& lhs, const exactreal::Arb& rhs) {
    REQUIRE(lhs.has_value());
    REQUIRE(*lhs == rhs);
    CAPTURE(*lhs);
    CAPTURE(rhs);
    REQUIRE(*(*lhs == rhs));
  };

  const auto Q = [](const auto& a, const auto& b, const auto& c) {
    return QuadraticPolynomial<T>(T(a), T(b), T(c));
  };

  SECTION("Constant Polynomials") {
    auto zero = Q(0, 0, 0);
    REQUIRE(!zero.positive());
    REQUIRE(zero(T()) == T());
    REQUIRE(zero(T(1)) == T());
    REQUIRE(zero.root().has_value());

    auto one = Q(0, 0, 1);
    REQUIRE(one.positive());
    REQUIRE(one(T()) == T(1));
    REQUIRE(one(T(1)) == T(1));
    REQUIRE(!one.root().has_value());

    auto minus_one = Q(0, 0, -1);
    REQUIRE(!minus_one.positive());
    REQUIRE(minus_one(T()) == T(-1));
    REQUIRE(minus_one(T(1)) == T(-1));
    REQUIRE(!minus_one.root().has_value());
  }

  SECTION("Linear Polynomials") {
    auto t = Q(0, 1, 0);
    REQUIRE(!t.positive());
    REQUIRE_eq(t.root(), Arb());

    auto f = Q(0, 1, 1);
    REQUIRE(f.positive());
    REQUIRE(!f.root().has_value());

    f = Q(0, 1, -1);
    REQUIRE(!f.positive());
    REQUIRE_eq(f.root(), Arb(1));

    f = Q(0, T(-2), 1);
    REQUIRE(!f.positive());
    REQUIRE(f.root());
  }

  SECTION("Quadratic Polynomials") {
    auto f = Q(1, 0, 0);
    REQUIRE(!f.positive());
    REQUIRE_eq(f.root(), Arb(0));

    f = Q(1, 0, 1);
    REQUIRE(f.positive());

    f = Q(1, 0, -1);
    REQUIRE(!f.positive());
    REQUIRE_eq(f.root(), Arb(1));

    f = Q(-1, 0, 1);
    REQUIRE(!f.positive());
    REQUIRE_eq(f.root(), Arb(1));

    f = Q(-2, 0, 1);
    REQUIRE(!f.positive());
    REQUIRE(f.root());

    f = Q(-2, 0, 3);
    REQUIRE(f.positive());
    REQUIRE(!f.root().has_value());

    REQUIRE(!(Q(-2, 0, 1) < Q(-2, 0, 1)));
    REQUIRE(Q(-2, 0, 1) < Q(-2, 0, 2));
  }
}

}  // namespace flatsurf::test
