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

#include "catch.hpp"

#include <flatsurf/half_edge.hpp>
#include <flatsurf/length_along_triangulation.hpp>
#include <flatsurf/vector.hpp>

#include "surfaces.hpp"

namespace flatsurf::test {
TEMPLATE_TEST_CASE("Length along Triangulation", "[length_along_triangulation]", (long long), (mpq_class), (eantic::renf_elem_class), (Element<exactreal::IntegerRing>)) {
  using T = TestType;
  using Vector = Vector<T>;
  using Length = LengthAlongTriangulation<T>;

  auto square = makeSquare<Vector>();
  GIVEN("The Square " << *square) {

    auto horizontal = Vector(7, -3);
    WHEN("We Fix the Vertical Direction Given by the Horizontal " << horizontal) {

      THEN("Lengths Relative to this Direction are as Expected") {
        auto v10 = Length(square, &horizontal, HalfEdge(1));
        auto v01 = Length(square, &horizontal, HalfEdge(-2));
        auto v11 = Length(square, &horizontal, HalfEdge(3));

        REQUIRE(v10 - v01 == v11);
        REQUIRE(3 * v10 == 7 * v01);
        REQUIRE(3 * v10 - 7 * v01 == Length());
        REQUIRE(v10 / v01 == 7 / 3);
        REQUIRE(2 * v10 / v01 == 2 * 7 / 3);
        REQUIRE(3 * v10 / v01 == 7);
      }
    }
  }
}  // namespace
}
