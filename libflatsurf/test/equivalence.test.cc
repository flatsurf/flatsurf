/**********************************************************************
 *  This file is part of flatsurf.
 *
 *        Copyright (C) 2022-2024 Julian Rüth
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
#include "generators/equivalence_generator.hpp"

namespace flatsurf::test {

TEMPLATE_TEST_CASE("Combinatorial Equivalence of Surfaces", "[Equivalence][combinatorial]", (long long), (mpz_class), (mpq_class), (renf_elem_class), (exactreal::Element<exactreal::IntegerRing>), (exactreal::Element<exactreal::RationalField>), (exactreal::Element<exactreal::NumberField>)) {
  using T = TestType;
  using Surface = FlatTriangulation<T>;

  const auto surface = GENERATE_SURFACES(T);
  CAPTURE(surface);

  SECTION("Equality of Combinatorial Equivalence") {
    REQUIRE(Equivalence<Surface>::combinatorial() == Equivalence<Surface>::combinatorial());
    REQUIRE(Equivalence<Surface>::combinatorial(false) == Equivalence<Surface>::combinatorial(false));
    REQUIRE(Equivalence<Surface>::combinatorial() != Equivalence<Surface>::combinatorial(false));
  }

  SECTION("A Surface is Combinatorially Equivalent to Itself") {
    const auto equivalence = Equivalence<Surface>::combinatorial();

    REQUIRE(EquivalenceClass(*surface, equivalence) == EquivalenceClass(*surface, equivalence));
  }

  SECTION("Equivalence modulo Flipping an Edge") {
    const auto halfEdge = GENERATE_COPY(halfEdges(surface));

    const auto equivalence = Equivalence<Surface>::combinatorial(true);

    // Don't test for each edge twice.
    if (halfEdge == halfEdge.edge().positive()) {
      if (surface->convex(halfEdge, true)) {
        auto flipped = surface->clone();
        flipped.flip(halfEdge);
        CAPTURE(flipped);

        // The surfaces might still be isomorphic combinatorially. In any case,
        // comparing them should be possible.
        REQUIRE_NOTHROW(EquivalenceClass(*surface, equivalence) == EquivalenceClass(flipped, equivalence));
      }
    }
  }

  SECTION("Equivalence modulo Linear Deformation of Positive Determinant") {
    const auto equivalence = Equivalence<Surface>::combinatorial();

    // Linear Deformation do not change the combinatorial structure (if their
    // determinant is positive.)
    const auto deformed = surface->applyMatrix(T(2), T(1), T(3), T(4));
    REQUIRE(EquivalenceClass(*surface, equivalence) == EquivalenceClass(deformed.codomain(), equivalence));
  }

  SECTION("Equivalence modulo Linear Deformation of Negative Determinant") {
    const auto equivalence = Equivalence<Surface>::combinatorial(false);

    // If the determinant is negative, we can ask for equivalence modulo orientation.
    const auto deformed = surface->applyMatrix(T(1), T(3), T(3), T(7));
    REQUIRE(EquivalenceClass(*surface, equivalence) == EquivalenceClass(deformed.codomain(), equivalence));
  }

  if constexpr (hasFractions<T>) {
    SECTION("Equivalence modulo Insertions") {
      const Point<Surface> point{*surface, HalfEdge(1), T(1), T(1), T(1)};

      const auto inserted = surface->insert(point);

      SECTION("Inserting a Point Changes the Combinatorial Structure of a Surface") {
        const auto equivalence = Equivalence<Surface>::combinatorial();

        REQUIRE(EquivalenceClass(*surface, equivalence) != EquivalenceClass(inserted.codomain(), equivalence));
      }
    }
  }
}

TEMPLATE_TEST_CASE("Equivalence of Surfaces Modulo Labels", "[Equivalence][unlabeled]", (long long), (mpz_class), (mpq_class), (renf_elem_class), (exactreal::Element<exactreal::IntegerRing>), (exactreal::Element<exactreal::RationalField>), (exactreal::Element<exactreal::NumberField>)) {
  using T = TestType;
  using Surface = FlatTriangulation<T>;

  const auto surface = GENERATE_SURFACES(T);
  CAPTURE(surface);

  const auto equivalence = Equivalence<Surface>::unlabeled();

  SECTION("Unlabeled Equivalence is Equal to Itself") {
    REQUIRE(Equivalence<Surface>::unlabeled() == Equivalence<Surface>::unlabeled());
  }

  SECTION("A Surface is Equivalent to Itself") {
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

        // The surfaces are likely different now. In any case, comparing them should not throw.
        REQUIRE_NOTHROW(EquivalenceClass(*surface, equivalence) == EquivalenceClass(flipped, equivalence));
      }
    }
  }

  SECTION("Equivalence modulo Linear Deformation") {
    // A linear deformation with non-unit determinant must yield a different equivalence class.
    REQUIRE(EquivalenceClass(*surface, equivalence) != EquivalenceClass(surface->applyMatrix(T(2), T(1), T(3), T(4)).codomain(), equivalence));

    // Rotating by π might leave us with the same surface modulo labeling.
    const auto rotated = surface->applyMatrix(T(-1), T(), T(), T(-1)).codomain().clone();

    REQUIRE(rotated.isomorphism(*surface, ISOMORPHISM::FACES).has_value() == (EquivalenceClass(*surface, equivalence) == EquivalenceClass(rotated, equivalence)));

    // Rotating by π might leave us with the same surface modulo labeling; in
    // particular, if we ignore edges in the interior of Delaunay cells.
    auto rotatedDelaunay = rotated.clone();
    rotatedDelaunay.delaunay();

    auto surfaceDelaunay = surface->clone();
    surfaceDelaunay.delaunay();

    REQUIRE_NOTHROW(rotatedDelaunay.isomorphism(surfaceDelaunay, ISOMORPHISM::DELAUNAY_CELLS).has_value() == (EquivalenceClass(surfaceDelaunay, equivalence) == EquivalenceClass(rotatedDelaunay, equivalence)));
  }
}

TEMPLATE_TEST_CASE("Equivalence of Surfaces Modulo GL2", "[Equivalence][linear]", (mpq_class), (renf_elem_class)) {
  using T = TestType;
  using Surface = FlatTriangulation<T>;

  const auto surface = GENERATE_SURFACES(T);
  CAPTURE(surface);

  const auto equivalence = Equivalence<Surface>::linear();

  SECTION("Equality of Linear Equivalences") {
    REQUIRE(Equivalence<Surface>::linear() == Equivalence<Surface>::linear());
    REQUIRE(Equivalence<Surface>::linear(false) == Equivalence<Surface>::linear(false));
    REQUIRE(Equivalence<Surface>::linear() != Equivalence<Surface>::linear(false));
  }

  SECTION("A Surface is Equivalent to Itself") {
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

       // If we ignored the flipped edge, the surfaces would be
       // indistinguishable. In any case, comparing them should not throw.
        REQUIRE_NOTHROW(EquivalenceClass(*surface, equivalence) == EquivalenceClass(flipped, equivalence));
      }
    }
  }

  SECTION("Equivalence modulo Linear Deformation With Positive Determinant") {
    const auto deformation = surface->applyMatrix(T(2), T(1), T(7), T(6));

    REQUIRE(EquivalenceClass(*surface, equivalence) == EquivalenceClass(deformation.codomain(), equivalence));
  }

  SECTION("Equivalence modulo Linear Deformation With Negative Determinant") {
    const auto unsignedEquivalence = Equivalence<Surface>::linear(false);

    const auto deformation = surface->applyMatrix(T(1), T(3), T(3), T(7));

    REQUIRE(EquivalenceClass(*surface, unsignedEquivalence) == EquivalenceClass(deformation.codomain(), unsignedEquivalence));
  }
}

TEMPLATE_TEST_CASE("Equivalence of Surfaces Modulo SL2", "[Equivalence][areaPreserving]", (mpq_class), (renf_elem_class)) {
  using T = TestType;
  using Surface = FlatTriangulation<T>;

  const auto surface = GENERATE_SURFACES(T);
  CAPTURE(surface);

  const auto equivalence = Equivalence<Surface>::areaPreserving();

  SECTION("Equality of Area-Preserving Equivalences") {
    REQUIRE(Equivalence<Surface>::areaPreserving() == Equivalence<Surface>::areaPreserving());
    REQUIRE(Equivalence<Surface>::areaPreserving(false) == Equivalence<Surface>::areaPreserving(false));
    REQUIRE(Equivalence<Surface>::areaPreserving() != Equivalence<Surface>::areaPreserving(false));
  }

  SECTION("A Surface is Equivalent to Itself") {
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

        // If we ignored the flipped edge, the surfaces would be
        // indistinguishable. In any case, comparing them should be supported.
        REQUIRE_NOTHROW(EquivalenceClass(*surface, equivalence) == EquivalenceClass(flipped, equivalence));
      }
    }
  }

  SECTION("Equivalence modulo a Skew") {
    const auto deformation = surface->applyMatrix(T(2), T(0), T(0), T(1)/2);

    REQUIRE(EquivalenceClass(*surface, equivalence) == EquivalenceClass(deformation.codomain(), equivalence));
  }

  SECTION("Equivalence modulo a Flip-Skew") {
    const auto unsignedEquivalence = Equivalence<Surface>::areaPreserving(false);

    const auto deformation = surface->applyMatrix(T(-2), T(0), T(0), T(1)/2);

    REQUIRE(EquivalenceClass(*surface, unsignedEquivalence) == EquivalenceClass(deformation.codomain(), unsignedEquivalence));
  }

  SECTION("Non-Equivalence modulo Scaling") {
    const auto unsignedEquivalence = Equivalence<Surface>::areaPreserving(false);

    const auto deformation = surface->applyMatrix(T(2), T(0), T(0), T(1));

    REQUIRE(EquivalenceClass(*surface, unsignedEquivalence) != EquivalenceClass(deformation.codomain(), unsignedEquivalence));
  }
}

TEMPLATE_TEST_CASE("Isomorphy of Surfaces", "[Equivalence][isomorphic]", (mpq_class), (renf_elem_class)) {
  using T = TestType;

  const auto surface = GENERATE_SURFACES(T);
  CAPTURE(surface);

  const auto& equivalence = GENERATE_REF(equivalences(surface));
  CAPTURE(equivalence);

  SECTION("Surfaces are Isomorphic to Themselves") {
    REQUIRE(equivalence.isomorphic(*surface, *surface));
  }
}

TEMPLATE_TEST_CASE("Isomorphisms of Surfaces", "[Equivalence][isomorphisms]", (mpq_class), (renf_elem_class)) {
  using T = TestType;

  const auto surface = GENERATE_SURFACES(T);
  CAPTURE(surface);

  const auto& equivalence = GENERATE_REF(equivalences(surface));
  CAPTURE(equivalence);

  SECTION("Surfaces have Isomorphisms to Themselves") {
    const auto isomorphisms = equivalence.isomorphisms(*surface, *surface);
    CAPTURE(isomorphisms);

    REQUIRE(!isomorphisms.empty());
    for (const auto& isomorphism : isomorphisms) {
      REQUIRE(isomorphism.domain() == *surface);
      REQUIRE(isomorphism.codomain() == *surface);
    }
  }
}

}
