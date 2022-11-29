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

#include "../flatsurf/edge.hpp"
#include "../flatsurf/equivalence.hpp"
#include "../flatsurf/equivalence_class.hpp"
#include "../flatsurf/deformation.hpp"
#include "../flatsurf/point.hpp"
#include "generators/surface_generator.hpp"
#include "generators/half_edge_generator.hpp"

namespace flatsurf::test {

TEMPLATE_TEST_CASE("Combinatorial Equivalence of Surfaces", "[Equivalence][combinatorial]", (long long), (mpz_class), (mpq_class), (renf_elem_class), (exactreal::Element<exactreal::IntegerRing>), (exactreal::Element<exactreal::RationalField>), (exactreal::Element<exactreal::NumberField>)) {
  using T = TestType;
  using Surface = FlatTriangulation<T>;

  const auto surface = GENERATE_SURFACES(T);
  CAPTURE(surface);

  SECTION("A Surface is Combinatorially Equivalent to Itself") {
    const auto equivalence = Equivalence<Surface>::combinatorial();

    REQUIRE(EquivalenceClass(*surface, equivalence) == EquivalenceClass(*surface, equivalence));
  }

  SECTION("Equivalence modulo Flipping an Edge") {
    const auto halfEdge = GENERATE_COPY(halfEdges(surface));

    // Don't test for each edge twice.
    if (halfEdge == halfEdge.edge().positive()) {
      if (surface->convex(halfEdge, true)) {
        auto flipped = surface->clone();
        flipped.flip(halfEdge);
        CAPTURE(flipped);

        const auto equivalence = Equivalence<Surface>::combinatorial([&](const Surface&, const Edge edge) { return edge != halfEdge; });

        // If we ignore the flipped edge, the surfaces must be indistinguishable.
        REQUIRE(EquivalenceClass(*surface, equivalence) == EquivalenceClass(flipped, equivalence));
      }
    }
  }

  SECTION("Equivalence modulo Linear Deformation") {
    const auto equivalence = Equivalence<Surface>::combinatorial();

    // Linear Deformation do not change the combinatorial structure (if their
    // determinant is positive.)
    REQUIRE(EquivalenceClass(*surface, equivalence) == EquivalenceClass(surface->applyMatrix(T(2), T(1), T(3), T(4)).codomain(), equivalence));
  }

  if constexpr (hasFractions<T>) {
    SECTION("Equivalence modulo Insertions") {
      const Point<Surface> point{*surface, HalfEdge(1), T(1), T(1), T(1)};

      const auto inserted = surface->insert(point);

      SECTION("Inserting a Point Changes the Combinatorial Structure of a Surface") {
        const auto equivalence = Equivalence<Surface>::combinatorial();

        REQUIRE(EquivalenceClass(*surface, equivalence) != EquivalenceClass(inserted.codomain(), equivalence));
      }

      SECTION("Inserting a Point does not Change the Combinatorial Structure away from that Point") {
        const auto equivalence = Equivalence<Surface>::combinatorial([&](const Surface&, const Edge edge) {
          return edge.index() < surface->size();
        });

        REQUIRE(EquivalenceClass(*surface, equivalence) == EquivalenceClass(inserted.codomain(), equivalence));
      }
    }
  }
}

}
