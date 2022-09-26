/**********************************************************************
 *  This file is part of flatsurf.
 *
 *        Copyright (C) 2022 Julian Rüth
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

#include "external/catch2/single_include/catch2/catch.hpp"

#include "../flatsurf/ray.hpp"
#include "generators/surface_generator.hpp"
#include "generators/half_edge_generator.hpp"
#include "generators/point_generator.hpp"

namespace flatsurf::test {

TEMPLATE_TEST_CASE("Creating Rays", "[Ray][constructor]", (long long), (mpq_class), (renf_elem_class), (exactreal::Element<exactreal::IntegerRing>), (exactreal::Element<exactreal::RationalField>), (exactreal::Element<exactreal::NumberField>)) {
  using T = TestType;
  using Surface = FlatTriangulation<T>;

  const auto surface = GENERATE_SURFACES(T);
  CAPTURE(surface);

  const auto face = GENERATE_COPY(halfEdges(surface));

  SECTION("Ray Starting at a Point") {
    const auto start = GENERATE_COPY(points(surface, face));
    CAPTURE(start);

    const auto ray = Ray{start, face, surface->fromHalfEdge(face)};

    if (surface->angle(start) == 1) {
      REQUIRE(ray == Ray{start, surface->fromHalfEdge(face)});
    } else {
      REQUIRE_THROWS(Ray{start, surface->fromHalfEdge(face)});
    }

    REQUIRE(ray == Ray{*surface, face, surface->fromHalfEdge(face)});
    REQUIRE(ray == Ray{*surface, face});
  }
}

}
