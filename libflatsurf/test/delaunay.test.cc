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

#include <exact-real/element.hpp>

#include "../flatsurf/bound.hpp"
#include "../flatsurf/half_edge.hpp"
#include "external/catch2/single_include/catch2/catch.hpp"
#include "generators/half_edge_generator.hpp"
#include "surfaces.hpp"

namespace flatsurf::test {

TEMPLATE_TEST_CASE("Delaunay Triangulation", "[delaunay]", (long long), (mpq_class), (eantic::renf_elem_class), (Element<exactreal::IntegerRing>)) {
  using T = TestType;
  using Vector = Vector<T>;

  GIVEN("A Flat Triangulation of a Square") {
    auto square = makeSquare<Vector>();

    auto bound = Bound(2, 0);

    auto halfEdge = GENERATE_COPY(halfEdges(square));
    WHEN("We Flip Edge " << halfEdge) {
      square->flip(halfEdge);
      THEN("The Delaunay Condition holds after performing Delaunay Triangulation") {
        square->delaunay();
        CAPTURE(*square);
        for (auto edge : square->halfEdges()) {
          REQUIRE(square->delaunay(edge));
          REQUIRE(square->fromHalfEdge(edge) < bound);
        }
      }
    }
  }
}

}  // namespace flatsurf::test
