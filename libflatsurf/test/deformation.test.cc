/**********************************************************************
 *  This file is part of flatsurf.
 *
 *        Copyright (C) 2021-2022 Julian Rüth
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

#include "../flatsurf/deformation.hpp"
#include "../flatsurf/edge.hpp"
#include "../flatsurf/tracked.hpp"
#include "../flatsurf/odd_half_edge_map.hpp"
#include "../flatsurf/path.hpp"
#include "../flatsurf/path_iterator.hpp"
#include "external/catch2/single_include/catch2/catch.hpp"
#include "generators/surface_generator.hpp"
#include "generators/saddle_connection_generator.hpp"
#include "generators/half_edge_generator.hpp"

namespace flatsurf::test {

TEMPLATE_TEST_CASE("Deformations", "[deformation]", (long long), (mpq_class), (renf_elem_class), (exactreal::Element<exactreal::IntegerRing>), (exactreal::Element<exactreal::NumberField>)) {
  using T = TestType;

  const auto [name, surface_] = GENERATE(makeSurface<T>());
  const auto surface = *surface_;
  CAPTURE(*name, *surface);

  SECTION("Deformations can be Copied and Moved") {
    auto trivial = Deformation(*surface);
    auto copy = trivial;
    trivial = copy;

    auto move = std::move(trivial);
    trivial = std::move(move);
  }

  SECTION("Trivial Deformations Follow Flips") {
    const auto domain = surface->clone();
    auto track = Tracked(*surface, Deformation(domain));

    for (auto edge : surface->edges()) {
      if (surface->convex(edge.positive(), true))
        surface->flip(edge.positive());
    }

    REQUIRE(track->domain() == domain);
    REQUIRE(track->codomain() == *surface);

    surface->delaunay();

    REQUIRE(track->domain() == domain);
    REQUIRE(track->codomain() == *surface);
  }
}

TEMPLATE_TEST_CASE("Deform a Flat Triangulation", "[flat_triangulation][deformation]", (long long), (mpz_class), (mpq_class), (renf_elem_class), (exactreal::Element<exactreal::IntegerRing>), (exactreal::Element<exactreal::RationalField>), (exactreal::Element<exactreal::NumberField>)) {
  using R2 = Vector<TestType>;

  const auto surface = makeL<R2>();

  SECTION("Trivially deform an L") {
    const auto shift = OddHalfEdgeMap<R2>(*surface);
    REQUIRE(surface->operator+(shift).codomain() == *surface);
  }

  SECTION("Stretch an L") {
    auto shift = OddHalfEdgeMap<R2>(*surface);

    shift.set(HalfEdge(8), R2(0, 1));
    shift.set(HalfEdge(7), R2(0, 1));

    const auto shifted = surface->operator+(shift);

    GIVEN("The Shifted Surface " << shifted) {
      REQUIRE(shifted.codomain() != *surface);

      THEN("Half Edges can be Mapped") {
        for (auto he : surface->halfEdges())
          REQUIRE(shifted(Path(SaddleConnection(*surface, he))).has_value());

        REQUIRE(shifted(SaddleConnection(*surface, HalfEdge(8)))->size() == 1);
        REQUIRE(shifted(SaddleConnection(*surface, HalfEdge(8)))->begin()->vector() == surface->fromHalfEdge(HalfEdge(8)) + R2(0, 1));

        REQUIRE(shifted(SaddleConnection(*surface, HalfEdge(7)))->size() == 1);
        REQUIRE(shifted(SaddleConnection(*surface, HalfEdge(7)))->begin()->vector() == surface->fromHalfEdge(HalfEdge(7)) + R2(0, 1));
      }

      AND_THEN("Saddle Connections can be Mapped") {
        const auto connection = GENERATE_REF(saddleConnections(surface));

        const auto image = shifted(connection);
        REQUIRE(image.has_value());
      }
    }
  }
}

}  // namespace flatsurf::test
