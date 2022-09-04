/**********************************************************************
 *  This file is part of flatsurf.
 *
 *        Copyright (C)      2019 Vincent Delecroix
 *        Copyright (C) 2019-2022 Julian Rüth
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

#include <fmt/format.h>

#include <e-antic/renfxx_fwd.hpp>
#include <exact-real/element.hpp>
#include <exact-real/number_field.hpp>
#include <numeric>
#include <unordered_set>

#include "../flatsurf/ccw.hpp"
#include "../flatsurf/deformation.hpp"
#include "../flatsurf/delaunay.hpp"
#include "../flatsurf/flat_triangulation.hpp"
#include "../flatsurf/fmt.hpp"
#include "../flatsurf/half_edge.hpp"
#include "../flatsurf/edge.hpp"
#include "../flatsurf/half_edge_set.hpp"
#include "../flatsurf/path.hpp"
#include "../flatsurf/path_iterator.hpp"
#include "../flatsurf/interval_exchange_transformation.hpp"
#include "../flatsurf/isomorphism.hpp"
#include "../flatsurf/saddle_connection.hpp"
#include "../flatsurf/saddle_connections.hpp"
#include "../flatsurf/vector.hpp"
#include "../src/external/rx-ranges/include/rx/ranges.hpp"
#include "external/catch2/single_include/catch2/catch.hpp"
#include "generators/half_edge_generator.hpp"
#include "generators/real_generator.hpp"
#include "generators/surface_generator.hpp"
#include "generators/point_generator.hpp"
#include "surfaces.hpp"

namespace flatsurf::test {

TEMPLATE_TEST_CASE("Flip in a Flat Triangulation", "[flat_triangulation][flip]", (long long), (mpz_class), (mpq_class), (renf_elem_class), (exactreal::Element<exactreal::IntegerRing>), (exactreal::Element<exactreal::RationalField>), (exactreal::Element<exactreal::NumberField>)) {
  using R2 = Vector<TestType>;
  auto square = makeSquare<R2>();
  auto vertices = square->vertices();
  CAPTURE(square);

  auto halfEdge = GENERATE(as<HalfEdge>{}, 1, 2, 3, -1, -2, -3);
  CAPTURE(halfEdge);

  SECTION("Four Flips of a Half Edge Restore the Initial Surface") {
    const auto vector = square->fromHalfEdge(halfEdge);
    square->flip(halfEdge);
    REQUIRE(vector != square->fromHalfEdge(halfEdge));
    square->flip(halfEdge);
    REQUIRE(vector == -square->fromHalfEdge(halfEdge));
    square->flip(halfEdge);
    square->flip(halfEdge);
    REQUIRE(vector == square->fromHalfEdge(halfEdge));

    // a square (torus) has only a single vertex so it won't change; in general
    // it should not change, however, the representatives attached to a vertex
    // are currently not properly updated: https://github.com/flatsurf/flatsurf/issues/100
    REQUIRE(vertices == square->vertices());
  }
}

}  // namespace flatsurf::test

