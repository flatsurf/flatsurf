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

#include <boost/lexical_cast.hpp>

#include "catch.hpp"

#include <flatsurf/vector.hpp>

namespace flatsurf::test {
using namespace flatsurf;

TEST_CASE("Vector<long long>", "[vector_long_long]") {
  using V = Vector<long long>;
  V vertical(2, 3);

  GIVEN("The Vertical Direction " << vertical) {
    THEN("CCWs are Computed") {
      REQUIRE(vertical.ccw(V(1, 2)) == CCW::COUNTERCLOCKWISE);
      REQUIRE(vertical.ccw(V(-1, -1)) == CCW::COUNTERCLOCKWISE);
      REQUIRE(vertical.ccw(V(0, -1)) == CCW::CLOCKWISE);
    }

    THEN("It Prints") {
      REQUIRE(boost::lexical_cast<std::string>(vertical) == "(2, 3)");
    }
  }
}
}  // namespace flatsurf::test
