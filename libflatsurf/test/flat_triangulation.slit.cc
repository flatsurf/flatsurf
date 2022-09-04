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

TEMPLATE_TEST_CASE("Insert into a Flat Triangulation", "[flat_triangulation][insert][slit]", (long long), (mpz_class), (mpq_class), (renf_elem_class), (exactreal::Element<exactreal::IntegerRing>), (exactreal::Element<exactreal::RationalField>), (exactreal::Element<exactreal::NumberField>)) {
  using T = TestType;
  using R2 = Vector<T>;

  SECTION("Slit at Many Places in the First Sector") {
    auto unscaled = GENERATE(values({makeSquare<R2>(), makeL<R2>()}));
    auto surface = unscaled->scale(3);
    CAPTURE(surface);

    auto x = GENERATE(range(1, 32));
    auto y = GENERATE(range(1, 32));

    if (x > y) {
      bool crossesSingularity = false;
      int xx = x / std::gcd(x, y);
      int yy = y / std::gcd(x, y);
      for (int n = 1; xx * n <= x; n++) {
        if (xx * n % 3 == 0 && yy * n % 3 == 0)
          crossesSingularity = true;
      }

      if (!crossesSingularity) {
        R2 v = R2(x, y);
        HalfEdge e(1);

        DYNAMIC_SECTION("Insert a Vertex at " << v << " next to " << e) {
          auto surf = surface.insertAt(e, v).codomain().clone();

          CAPTURE(e);
          CAPTURE(surf);

          SECTION("The Surface has Changed in the Right Way") {
            REQUIRE(surface != surf);
            REQUIRE(surf.fromHalfEdge(surf.nextAtVertex(e)) == v);
          }

          SECTION("Slot Introduces a Boundary") {
            surf = surf.slit(surf.nextAtVertex(e)).codomain().clone();

            REQUIRE(surf.boundary(surf.nextAtVertex(e)));
          }
        }
      }
    }
  }
}

}  // namespace flatsurf::test

