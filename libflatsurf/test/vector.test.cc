/**********************************************************************
 *  This file is part of flatsurf.
 *
 *        Copyright (C) 2020 Vincent Delecroix
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

#include <set>

#include <e-antic/renfxx.h>

#include <exact-real/element.hpp>
#include <exact-real/module.hpp>
#include <exact-real/number_field.hpp>
#include <exact-real/real_number.hpp>

#include "../flatsurf/vector.hpp"
#include "../flatsurf/bound.hpp"
#include "../flatsurf/saddle_connection.hpp"
#include "../flatsurf/saddle_connections.hpp"
#include "../flatsurf/saddle_connections_iterator.hpp"
#include "external/catch2/single_include/catch2/catch.hpp"
#include "surfaces.hpp"

namespace flatsurf::test {

TEMPLATE_TEST_CASE("Vector Slopes", "[vector]", (long long), (mpz_class), (mpq_class), (eantic::renf_elem_class), (exactreal::Element<exactreal::IntegerRing>), (exactreal::Element<exactreal::RationalField>), (exactreal::Element<exactreal::NumberField>)) {
  using T = TestType;
  using V = Vector<T>;

  SECTION("Slopes Compare Correctly") {
    const typename V::CompareSlope lt;

    REQUIRE(lt(V(0, 1), V(1, 0)));
    REQUIRE(!lt(V(0, 1), V(0, 2)));
  }

  SECTION("Duplicate Slopes are Detected") {
    auto square = makeSquare<V>();

    std::set<V, typename V::CompareSlope> slopes;
    for (auto connection : SaddleConnections(square, Bound(1, 0)))
      slopes.insert(connection.vector());
    
    REQUIRE(slopes.size() == 6);
  }
}

TEMPLATE_TEST_CASE("Vector Sector Containment", "[vector][inSector]", (long long), (mpz_class), (mpq_class), (eantic::renf_elem_class), (exactreal::Element<exactreal::IntegerRing>), (exactreal::Element<exactreal::RationalField>), (exactreal::Element<exactreal::NumberField>)) {
  using T = TestType;
  using V = Vector<T>;

  SECTION("A Vector is Contained in the Ray Defined By Itself") {
    V v(1, 0);
    REQUIRE(v.inSector(v, v));
  }

  SECTION("A Vector is Contained in the Sector Starting at Itself") {
    V v(1, 0);
    REQUIRE(v.inSector(v, -v));
  }

  SECTION("A Vector is Not Contained in the Sector Ending at Itself") {
    V v(1, 0);
    REQUIRE(v.inSector(-v, v));
  }
}

}
