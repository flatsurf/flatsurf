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

#include "../flatsurf/half_edge.hpp"
#include "external/catch2/single_include/catch2/catch.hpp"

namespace flatsurf::test {

TEST_CASE("HalfEdge Basics", "[half_edge]") {
  const int id = GENERATE(range(-1024, 1024));

  if (id == 0) return;

  const HalfEdge e(id);

  REQUIRE(e == HalfEdge(id));
  REQUIRE(-(-e) == e);
  REQUIRE(-e == HalfEdge(-id));
  REQUIRE(e.id() == id);
  REQUIRE((-e).id() == -e.id());
  REQUIRE(HalfEdge::fromIndex(e.index()) == e);
}

}  // namespace flatsurf::test
