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

TEMPLATE_TEST_CASE("Compute Total Angle at a Point", "[flat_triangulation][angle]", (long long), (mpz_class), (mpq_class), (renf_elem_class), (exactreal::Element<exactreal::IntegerRing>), (exactreal::Element<exactreal::RationalField>), (exactreal::Element<exactreal::NumberField>)) {
  using T = TestType;
  using R2 = Vector<eantic::renf_elem_class>;

  SECTION("A Square Has No Singularities") {
    const auto square = makeSquare<R2>();
    for (auto vertex : square->vertices()) {
      REQUIRE(square->angle(vertex) == 1);
    }
  }

  if constexpr (hasNumberFieldElements<T>) {
    SECTION("The Unfolding of the (1, 2, 3) Triangle Has No Singularities") {
      const auto _123 = make123<R2>();
      for (auto vertex : _123->vertices()) {
        REQUIRE(_123->angle(vertex) == 1);
      }
    }
  }

  SECTION("The L Has A Single Singularity") {
    const auto L = makeL<R2>();
    REQUIRE(L->vertices().size() == 1);
    for (auto vertex : L->vertices()) {
      REQUIRE(L->angle(vertex) == 3);
    }
  }

  SECTION("Total Angle of Other Surfaces") {
    const auto surface = GENERATE_SURFACES(T);
    CAPTURE(surface);

    SECTION("Total Angle at Vertices") {
      for (auto vertex : surface->vertices())
        REQUIRE(surface->angle(vertex) >= 1);
    }

    SECTION("Total Angle at General Points") {
      const auto face = GENERATE_COPY(halfEdges(surface));
      const auto point = GENERATE_COPY(points(surface, face));
      CAPTURE(point);

      int angle = surface->angle(point);
      REQUIRE(angle >= 1);
      REQUIRE(((point.vertex()) || angle == 1));
    }
  }
}

}  // namespace flatsurf::test

