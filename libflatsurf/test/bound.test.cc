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

#include "../flatsurf/bound.hpp"
#include "external/catch2/single_include/catch2/catch.hpp"

namespace flatsurf::test {

TEST_CASE("Bound Arithmetic", "[bound]") {
  REQUIRE(Bound() < Bound(1));

  SECTION("Multiplication works as expected") {
    REQUIRE(Bound(1) < 2 * Bound(1));
    REQUIRE(Bound(2) == 2 * Bound(1));
  }

  SECTION("Bounds in Two Variables Compare Like Vector Lengths") {
    REQUIRE(Bound(1, 0) <= Bound(1));
    REQUIRE(Bound(1, 1) <= Bound(2));
    REQUIRE(Bound(2, 2) > Bound(2));
    REQUIRE(Bound(3, 4) == Bound(5));
  }

  SECTION("Non-zero Checks") {
    REQUIRE(static_cast<bool>(Bound()) == false);
    REQUIRE(static_cast<bool>(Bound(1)) == true);
  }
}

}  // namespace flatsurf::test
