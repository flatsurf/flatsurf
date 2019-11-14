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
#include <flatsurf/permutation.hpp>

namespace flatsurf::test {
TEST_CASE("Permutation", "[permutation]") {
  auto size = GENERATE(range(0, 10));
  GIVEN("A Random Permutation on the Half Edges indexed from " << -size << " to " << size) {
    auto domain = std::vector<HalfEdge>();
    for (int i = 1; i <= size; i++) {
      domain.push_back(HalfEdge(i));
      domain.push_back(HalfEdge(-i));
    }
    auto p = Permutation<HalfEdge>::random(domain);

    THEN("It can be Reconstructed From its Cycles") {
      REQUIRE(p == Permutation<HalfEdge>(p.cycles()));
    }
  }
}  // namespace flatsurf
}  // namespace flatsurf::test
