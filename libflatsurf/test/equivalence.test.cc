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

#include "../flatsurf/delaunay.hpp"
#include "../flatsurf/edge.hpp"
#include "../flatsurf/equivalence.hpp"
#include "../flatsurf/equivalence_class.hpp"
#include "../flatsurf/deformation.hpp"
#include "../flatsurf/point.hpp"
#include "../flatsurf/isomorphism.hpp"
#include "generators/surface_generator.hpp"
#include "generators/half_edge_generator.hpp"

namespace flatsurf::test {

TEMPLATE_TEST_CASE("Combinatorial Equivalence of Surfaces", "[Equivalence][combinatorial]", (long long), (mpz_class), (mpq_class), (renf_elem_class), (exactreal::Element<exactreal::IntegerRing>), (exactreal::Element<exactreal::RationalField>), (exactreal::Element<exactreal::NumberField>)) {
  using T = TestType;
  using Surface = FlatTriangulation<T>;

  const auto surface = GENERATE_SURFACES(T);
  CAPTURE(surface);
  
  SECTION("Combinatorial Equivalence is Equal to Itself") {
    REQUIRE(Equivalence<Surface>::combinatorial() == Equivalence<Surface>::combinatorial());
  }

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

        const auto equivalence = Equivalence<Surface>::combinatorial(true, [&](const Surface&, const Edge edge) { return edge != halfEdge; });

        // If we ignore the flipped edge, the surfaces must be indistinguishable.
        REQUIRE(EquivalenceClass(*surface, equivalence) == EquivalenceClass(flipped, equivalence));
      }
    }
  }

  SECTION("Equivalence modulo Linear Deformation of Positive Determinant") {
    const auto equivalence = Equivalence<Surface>::combinatorial();

    // Linear Deformation do not change the combinatorial structure (if their
    // determinant is positive.)
    REQUIRE(EquivalenceClass(*surface, equivalence) == EquivalenceClass(surface->applyMatrix(T(2), T(1), T(3), T(4)).codomain(), equivalence));
  }

  SECTION("Equivalence modulo Linear Deformation of Negative Determinant") {
    const auto equivalence = Equivalence<Surface>::combinatorial(false);

    // If the determinant is negative, we can ask for equivalence modulo orientation.
    REQUIRE(EquivalenceClass(*surface, equivalence) == EquivalenceClass(surface->applyMatrix(T(1), T(3), T(3), T(7)).codomain(), equivalence));
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
        const auto equivalence = Equivalence<Surface>::combinatorial(true, [&](const Surface&, const Edge edge) {
          return edge.index() < surface->size();
        });

        REQUIRE(EquivalenceClass(*surface, equivalence) == EquivalenceClass(inserted.codomain(), equivalence));
      }
    }
  }
}

TEMPLATE_TEST_CASE("Equivalence of Surfaces Modulo Labels", "[Equivalence][unlabeled]", (long long), (mpz_class), (mpq_class), (renf_elem_class), (exactreal::Element<exactreal::IntegerRing>), (exactreal::Element<exactreal::RationalField>), (exactreal::Element<exactreal::NumberField>)) {
  using T = TestType;
  using Surface = FlatTriangulation<T>;

  const auto surface = GENERATE_SURFACES(T);
  CAPTURE(surface);

  SECTION("A Surface is Equivalent to Itself") {
    const auto equivalence = Equivalence<Surface>::unlabeled();

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

        const auto equivalence = Equivalence<Surface>::unlabeled([&](const Surface&, const Edge edge) { return edge != halfEdge; });

        // If we ignore the flipped edge, the surfaces must be indistinguishable.
        REQUIRE(EquivalenceClass(*surface, equivalence) == EquivalenceClass(flipped, equivalence));
      }
    }
  }

  SECTION("Equivalence modulo Linear Deformation") {
    const auto equivalence = Equivalence<Surface>::unlabeled();

    // A linear deformation with non-unit determinant must yield a different equivalence class.
    REQUIRE(EquivalenceClass(*surface, equivalence) != EquivalenceClass(surface->applyMatrix(T(2), T(1), T(3), T(4)).codomain(), equivalence));

    // Rotating by π might leave us with the same surface modulo labeling.
    const auto rotated = surface->applyMatrix(T(-1), T(), T(), T(-1)).codomain().clone();

    REQUIRE(rotated.isomorphism(*surface, ISOMORPHISM::FACES).has_value() == (EquivalenceClass(*surface, equivalence) == EquivalenceClass(rotated, equivalence)));

    // Rotating by π might leave us with the same surface modulo labeling; in particular, if we ignore edges in the interior of Delaunay cells.
    auto rotatedDelaunay = rotated.clone();
    rotatedDelaunay.delaunay();

    auto surfaceDelaunay = surface->clone();
    surfaceDelaunay.delaunay();

    const auto delaunayEquivalence = Equivalence<Surface>::unlabeled([](const Surface& surface, Edge e) -> bool {
      return surface.delaunay(e) != DELAUNAY::AMBIGUOUS;
    });
    REQUIRE(rotatedDelaunay.isomorphism(surfaceDelaunay, ISOMORPHISM::DELAUNAY_CELLS).has_value() == (EquivalenceClass(surfaceDelaunay, delaunayEquivalence) == EquivalenceClass(rotatedDelaunay, delaunayEquivalence)));
  }
}

TEMPLATE_TEST_CASE("Equivalence of Surfaces Modulo GL2", "[Equivalence][linear]", (mpq_class), (renf_elem_class)) {
  using T = TestType;
  using Surface = FlatTriangulation<T>;

  const auto surface = GENERATE_SURFACES(T);
  CAPTURE(surface);

  SECTION("A Surface is Equivalent to Itself") {
    const auto equivalence = Equivalence<Surface>::linear();

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

        const auto equivalence = Equivalence<Surface>::linear(true, [](const Surface&, HalfEdge, HalfEdge) { return std::tuple{T(1), T(), T(), T(1)}; }, [&](const Surface&, const Edge edge) { return edge != halfEdge; });

        // If we ignore the flipped edge, the surfaces must be indistinguishable.
        REQUIRE(EquivalenceClass(*surface, equivalence) == EquivalenceClass(flipped, equivalence));
      }
    }
  }

  SECTION("Equivalence modulo Linear Deformation With Positive Determinant") {
    const auto equivalence = Equivalence<Surface>::linear();

    const auto deformation = surface->applyMatrix(T(2), T(1), T(7), T(6));

    REQUIRE(EquivalenceClass(*surface, equivalence) == EquivalenceClass(deformation.codomain(), equivalence));
  }

  SECTION("Equivalence modulo Linear Deformation With Negative Determinant") {
    const auto equivalence = Equivalence<Surface>::linear(false);

    const auto deformation = surface->applyMatrix(T(1), T(3), T(3), T(7));

    REQUIRE(EquivalenceClass(*surface, equivalence) == EquivalenceClass(deformation.codomain(), equivalence));
  }
}

}
