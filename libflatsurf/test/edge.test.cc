/**********************************************************************
 *  This file is part of flatsurf.
 *
 *        Copyright (C)      2020 Vincent Delecroix
 *        Copyright (C) 2020-2022 Julian Rüth
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
#include "../flatsurf/half_edge.hpp"
#include "cereal.helpers.hpp"

namespace flatsurf::test {

TEST_CASE("Edge Basics", "[Edge][constructor][operator==][fromIndxe]") {
  const int id = GENERATE(range(1, 1024));

  const Edge e(id);

  REQUIRE(e == Edge(id));
  REQUIRE(-e.negative() == e.positive());
  REQUIRE(e == HalfEdge(-id));
  REQUIRE(e == HalfEdge(id));
  REQUIRE(e.index() == id - 1);
  REQUIRE(Edge::fromIndex(e.index()) == e);
}

TEST_CASE("Serialization of an Edge", "[Edge][load_minimal][save_minimal]") {
  testRoundtrip(Edge(1337));
}

}  // namespace flatsurf::test
