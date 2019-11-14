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

#include <e-antic/renfxx_fwd.h>
#include <exact-real/element.hpp>
#include <exact-real/number_field.hpp>

#include <flatsurf/flat_triangulation.hpp>
#include <flatsurf/half_edge.hpp>
#include <flatsurf/saddle_connection.hpp>
#include <flatsurf/saddle_connections.hpp>
#include <flatsurf/vector.hpp>
#include <flatsurf/vector_along_triangulation.hpp>

#include "surfaces.hpp"

namespace flatsurf::test {

TEST_CASE("Flat Triangulation Comparisons", "[flat_triangulation][operator==]") {
  auto square = makeSquareCombinatorial();
  GIVEN("The Square " << square) {
    REQUIRE(square == *square.clone());
    REQUIRE(square != makeSquareWithBoundaryCombinatorial());
  }
}

TEST_CASE("Flat Triangulation Insertions", "[flat_triangulation][insert]") {
  auto square = makeSquareCombinatorial();

  GIVEN("The Square " << square) {
    auto e = square.halfEdges()[0];

    WHEN("We Insert a Vertex Next to " << e) {
      auto square_ = square.insertAt(e);
      CAPTURE(*square_);

      THEN("The Combinatorics Have Changed in the Expected Way") {
        REQUIRE(square != *square_);
        REQUIRE(square.vertices().size() + 1 == square_->vertices().size());
        REQUIRE(square.halfEdges().size() + 6 == square_->halfEdges().size());

        auto a = -square_->nextAtVertex(e);
        REQUIRE(a != -square.nextAtVertex(e));
        REQUIRE(square_->nextAtVertex(square_->nextAtVertex(square_->nextAtVertex(a))) == a);
      }
    }
  }
}

}  // namespace flatsurf::test
