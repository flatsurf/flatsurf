/**********************************************************************
 *  This file is part of flatsurf.
 *
 *        Copyright (C)      2019 Vincent Delecroix
 *        Copyright (C) 2019-2025 Julian Rüth
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
#include "../flatsurf/equivalence.hpp"
#include "../flatsurf/edge.hpp"
#include "../flatsurf/flat_triangulation.hpp"
#include "../flatsurf/fmt.hpp"
#include "../flatsurf/half_edge.hpp"
#include "../flatsurf/half_edge_set.hpp"
#include "../flatsurf/interval_exchange_transformation.hpp"
#include "../flatsurf/isomorphism.hpp"
#include "../flatsurf/odd_half_edge_map.hpp"
#include "../flatsurf/path.hpp"
#include "../flatsurf/path_iterator.hpp"
#include "../flatsurf/saddle_connection.hpp"
#include "../flatsurf/saddle_connections_iterator.hpp"
#include "../flatsurf/saddle_connections.hpp"
#include "../flatsurf/vector.hpp"
#include "../src/external/rx-ranges/include/rx/ranges.hpp"
#include "cereal.helpers.hpp"
#include "generators/half_edge_generator.hpp"
#include "generators/real_generator.hpp"
#include "generators/surface_generator.hpp"
#include "generators/point_generator.hpp"
#include "surfaces.hpp"

namespace flatsurf::test {

TEMPLATE_TEST_CASE("Comparisons and Hashing", "[FlatTriangulation][operator==]", (long long), (mpz_class), (mpq_class), (renf_elem_class), (exactreal::Element<exactreal::IntegerRing>), (exactreal::Element<exactreal::RationalField>), (exactreal::Element<exactreal::NumberField>)) {
  using T = TestType;

  const auto surface = GENERATE_SURFACES(T);
  CAPTURE(surface);

  const auto other = GENERATE_SURFACES(T);
  CAPTURE(other);

  REQUIRE(*surface == *surface);
  REQUIRE(*surface == surface->clone());

  if (*surface == *other) {
    REQUIRE(std::hash<FlatTriangulation<T>>{}(*surface) == std::hash<FlatTriangulation<T>>{}(*other));
  }
}

TEMPLATE_TEST_CASE("Compute Total Angle at a Point", "[FlatTriangulation][angle]", (long long), (mpz_class), (mpq_class), (renf_elem_class), (exactreal::Element<exactreal::IntegerRing>), (exactreal::Element<exactreal::RationalField>), (exactreal::Element<exactreal::NumberField>)) {
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

TEMPLATE_TEST_CASE("Delaunay Triangulation of a Square", "[FlatTriangulation][delaunay]", (long long), (mpz_class), (mpq_class), (eantic::renf_elem_class), (Element<exactreal::IntegerRing>), (Element<exactreal::RationalField>), (Element<exactreal::NumberField>)) {
  using T = TestType;
  using Vector = Vector<T>;

  auto square = makeSquare<Vector>();

  auto bound = Bound(2, 0);

  auto flip = GENERATE_COPY(halfEdges(square));
  CAPTURE(flip);

  square->flip(flip);

  SECTION("The Delaunay Condition holds after performing Delaunay Triangulation") {
    square->delaunay();
    CAPTURE(*square);
    for (auto halfEdge : square->halfEdges()) {
      REQUIRE(square->delaunay(halfEdge.edge()) != DELAUNAY::NON_DELAUNAY);
      REQUIRE(square->fromHalfEdge(halfEdge) < bound);
    }
  }
}

TEMPLATE_TEST_CASE("Delaunay Triangulation of an Octagon", "[FlatTriangulation][delaunay]", (eantic::renf_elem_class), (Element<exactreal::NumberField>)) {
  using T = TestType;
  using Vector = Vector<T>;

  SECTION("A Regular Octagon") {
    auto octagon = makeOctagon<Vector>();

    auto a = octagon->fromHalfEdge(HalfEdge(1)).x();

    auto flip = GENERATE_COPY(halfEdges(octagon));
    CAPTURE(flip);

    if (octagon->convex(flip)) {
      octagon->flip(flip);

      SECTION("The Delaunay Cells do not Change") {
        octagon->delaunay();
        CAPTURE(*octagon);
        for (auto halfEdge : octagon->halfEdges()) {
          CAPTURE(halfEdge);
          auto v = octagon->fromHalfEdge(halfEdge);
          CAPTURE(v);

          const auto isBoundary = (v.x() == 0 && v.y() == 1) || (v.x() == 1 && v.y() == 0) || (v.x() == 0 && v.y() == -1) || (v.x() == -1 && v.y() == 0) || (v.x() == a && v.y() == a) || (v.x() == -a && v.y() == -a) || (v.x() == a && v.y() == -a) || (v.x() == -a && v.y() == a);
          CAPTURE(isBoundary);
          REQUIRE(octagon->delaunay(halfEdge.edge()) == (isBoundary ? DELAUNAY::DELAUNAY : DELAUNAY::AMBIGUOUS));
        }
      }
    }
  }
}

TEMPLATE_TEST_CASE("Delaunay Triangulation", "[FlatTriangulation][delaunay]", (long long), (mpz_class), (mpq_class), (renf_elem_class), (exactreal::Element<exactreal::IntegerRing>), (exactreal::Element<exactreal::RationalField>), (exactreal::Element<exactreal::NumberField>)) {
  using T = TestType;

  const auto surface_ = GENERATE_SURFACES(T);
  CAPTURE(surface_);

  auto surface = surface_->clone();

  surface.delaunay();

  SECTION("Delaunay Cells are Convex and their Boundaries Connected") {
    HalfEdgeSet boundary;
    for (auto halfEdge : surface.halfEdges())
      if (surface.delaunay(halfEdge.edge()) == DELAUNAY::DELAUNAY)
        boundary.insert(halfEdge);

    HalfEdge anyBoundaryEdge;
    for (auto he : surface.halfEdges())
      if (boundary.contains(he))
        anyBoundaryEdge = he;

    HalfEdgeSet traced;
    const std::function<void(HalfEdge)> walk = [&](const HalfEdge he) {
      if (traced.contains(he))
        return;

      traced.insert(he);

      walk(-he);

      HalfEdge next = he;
      do {
        next = surface.nextAtVertex(next);
      } while (!boundary.contains(next));

      REQUIRE(surface.fromHalfEdge(he).ccw(surface.fromHalfEdge(next)) == CCW::COUNTERCLOCKWISE);

      walk(next);
    };

    walk(anyBoundaryEdge);

    for (auto he : surface.halfEdges())
      REQUIRE(boundary.contains(he) == traced.contains(he));
  }
}

TEMPLATE_TEST_CASE("Eliminate Marked Points", "[FlatTriangulation][eliminateMarkedPoints]", (long long), (mpz_class), (mpq_class), (renf_elem_class), (exactreal::Element<exactreal::IntegerRing>), (exactreal::Element<exactreal::RationalField>), (exactreal::Element<exactreal::NumberField>)) {
  using T = TestType;
  using Surface = FlatTriangulation<T>;

  const auto surface = GENERATE_SURFACES(T);
  CAPTURE(surface);

  const auto simplified = surface->eliminateMarkedPoints();

  CAPTURE(simplified);

  const auto unmarkedPoints = [](const auto& surface) {
    return surface.vertices() | rx::filter([&](const auto& vertex) { return surface.angle(vertex) != 1; }) | rx::to_vector();
  };

  const auto markedPoints = [](const auto& surface) {
    return surface.codomain().vertices() | rx::filter([&](const auto& vertex) { return surface.codomain().angle(vertex) == 1; }) | rx::to_vector();
  };

  if (unmarkedPoints(*surface).size()) {
    REQUIRE(markedPoints(simplified).size() == 0);
  } else {
    REQUIRE(markedPoints(simplified).size() == 1);
    REQUIRE(unmarkedPoints(simplified.codomain()).size() == 0);
  }

  REQUIRE(surface->area2() == simplified.codomain().area2());

  for (const auto& preimage : surface->halfEdges()) {
    if (surface->angle(Vertex::source(*surface, preimage)) != 1 && surface->angle(Vertex::target(*surface, preimage)) != 1) {
      REQUIRE(simplified(SaddleConnection<Surface>(*surface, preimage)).has_value());
      REQUIRE(simplified(SaddleConnection<Surface>(*surface, preimage))->begin()->vector() == surface->fromHalfEdge(preimage));
    }
  }

  const auto section = simplified.section();
  CAPTURE(section);

  for (const auto& image : section.domain().halfEdges()) {
    CAPTURE(image);

    const auto preimage = section(SaddleConnection<Surface>(section.domain(), image));

    REQUIRE(preimage);

    CAPTURE(*preimage);

    Vector<T> vector;
    for (const auto& connection : *preimage)
      vector += connection;

    REQUIRE(vector == section.domain().fromHalfEdge(image));
  }
}

TEMPLATE_TEST_CASE("Flip in a Flat Triangulation", "[FlatTriangulation][flip]", (long long), (mpz_class), (mpq_class), (renf_elem_class), (exactreal::Element<exactreal::IntegerRing>), (exactreal::Element<exactreal::RationalField>), (exactreal::Element<exactreal::NumberField>)) {
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

TEMPLATE_TEST_CASE("Insert into a Flat Triangulation", "[FlatTriangulation][insert]", (long long), (mpz_class), (mpq_class), (renf_elem_class), (exactreal::Element<exactreal::IntegerRing>), (exactreal::Element<exactreal::RationalField>), (exactreal::Element<exactreal::NumberField>)) {
  using T = TestType;
  using R2 = Vector<T>;
  using Surface = FlatTriangulation<T>;

  SECTION("Insert into a Surface") {
    auto surface = GENERATE_SURFACES(T);
    CAPTURE(surface);

    auto face = GENERATE_COPY(halfEdges(surface));
    auto point = GENERATE_COPY(points(surface, face));
    CAPTURE(point);

    if (point.vertex()) {
      REQUIRE_THROWS(surface->insert(point));
    } else {
      const auto insertion = [&]() -> std::optional<Deformation<Surface>> {
        try {
          return surface->insert(point);
        } catch (...) {
          if (std::is_same_v<T, long long> ||
              std::is_same_v<T, mpz_class> ||
              std::is_same_v<T, exactreal::Element<exactreal::IntegerRing>> ||
              std::is_same_v<T, exactreal::Element<exactreal::RationalField>> ||
              std::is_same_v<T, exactreal::Element<exactreal::NumberField>>)
            // When not in a field, insertion might not be possible if the
            // point has no coordinates in the base ring.
            return std::nullopt;

          throw;
        }
      }();
      if (insertion) {
        REQUIRE(insertion->domain() == *surface);
        CAPTURE(insertion->codomain());

        const auto image = insertion->operator()(point);
        CAPTURE(image);
        REQUIRE(image.vertex());

        const auto preimage = insertion->section()(image);
        REQUIRE(preimage == point);
      }
    }
  }

  SECTION("Insert into an L") {
    auto surface = makeL<R2>()->scale(3);

    SECTION("Insert without Flip") {
      auto sector = HalfEdge(1);
      REQUIRE(fmt::format("{}", surface.insertAt(sector, R2(2, 1))) == "FlatTriangulationCombinatorial(vertices = (1, 2, 3, 4, 5, -3, 6, 7, 8, -6, -2, 9, -4, -5, -9, -1, -7, -8), faces = (1, -9, -2)(-1, -8, 7)(2, -6, -3)(3, 5, -4)(4, 9, -5)(6, 8, -7)) with vectors {1: (3, 0), 2: (3, 3), 3: (0, 3), 4: (-3, 0), 5: (-3, -3), 6: (3, 0), 7: (3, 3), 8: (0, 3), 9: (0, -3)} → FlatTriangulationCombinatorial(vertices = (1, -10, 2, 3, 4, 5, -3, 6, 7, 8, -6, -2, -12, 9, -4, -5, -9, -11, -1, -7, -8)(10, 11, 12), faces = (1, -11, 10)(-1, -8, 7)(2, -6, -3)(-2, -10, 12)(3, 5, -4)(4, 9, -5)(6, 8, -7)(-9, -12, 11)) with vectors {1: (3, 0), 2: (3, 3), 3: (0, 3), 4: (-3, 0), 5: (-3, -3), 6: (3, 0), 7: (3, 3), 8: (0, 3), 9: (0, -3), 10: (-2, -1), 11: (1, -1), 12: (1, 2)} inserting {10, 11, 12}");
    }

    SECTION("Insert without Flip onto Edge") {
      auto sector = HalfEdge(1);
      REQUIRE(fmt::format("{}", surface.insertAt(sector, R2(1, 0))) == "FlatTriangulationCombinatorial(vertices = (1, 2, 3, 4, 5, -3, 6, 7, 8, -6, -2, 9, -4, -5, -9, -1, -7, -8), faces = (1, -9, -2)(-1, -8, 7)(2, -6, -3)(3, 5, -4)(4, 9, -5)(6, 8, -7)) with vectors {1: (3, 0), 2: (3, 3), 3: (0, 3), 4: (-3, 0), 5: (-3, -3), 6: (3, 0), 7: (3, 3), 8: (0, 3), 9: (0, -3)} → FlatTriangulationCombinatorial(vertices = (1, 8, -6, -2, -12, 9, -4, -5, -9, -11, -7, -8, -10, 2, 3, 4, 5, -3, 6, 7)(-1, 11, 12, 10), faces = (1, 10, -8)(-1, 7, -11)(2, -6, -3)(-2, -10, 12)(3, 5, -4)(4, 9, -5)(6, 8, -7)(-9, -12, 11)) with vectors {1: (1, 3), 2: (3, 3), 3: (0, 3), 4: (-3, 0), 5: (-3, -3), 6: (3, 0), 7: (3, 3), 8: (0, 3), 9: (0, -3), 10: (-1, 0), 11: (2, 0), 12: (2, 3)} inserting {-1, 10, 11, 12}");
    }

    SECTION("Insert with Single Flip onto Edge") {
      auto sector = HalfEdge(1);
      // Actually, we perform more than one flip. One would be enough but we
      // cannot handle inserts onto a half edge other than the sector boundary.
      REQUIRE(fmt::format("{}", surface.insertAt(sector, R2(4, 1))) == "(((FlatTriangulationCombinatorial(vertices = (1, 2, 3, 4, 5, -3, 6, 7, 8, -6, -2, 9, -4, -5, -9, -1, -7, -8), faces = (1, -9, -2)(-1, -8, 7)(2, -6, -3)(3, 5, -4)(4, 9, -5)(6, 8, -7)) with vectors {1: (3, 0), 2: (3, 3), 3: (0, 3), 4: (-3, 0), 5: (-3, -3), 6: (3, 0), 7: (3, 3), 8: (0, 3), 9: (0, -3)} → FlatTriangulationCombinatorial(vertices = (1, 9, 2, 3, 4, -9, 5, -3, 6, 7, 8, -6, -2, -4, -5, -1, -7, -8), faces = (1, -5, -9)(-1, -8, 7)(2, -6, -3)(-2, 9, 4)(3, 5, -4)(6, 8, -7)) with vectors {1: (3, 0), 2: (3, 3), 3: (0, 3), 4: (-3, 0), 5: (-3, -3), 6: (3, 0), 7: (3, 3), 8: (0, 3), 9: (6, 3)} flipping -9) then (FlatTriangulationCombinatorial(vertices = (1, 9, 2, 3, 4, -9, 5, -3, 6, 7, 8, -6, -2, -4, -5, -1, -7, -8), faces = (1, -5, -9)(-1, -8, 7)(2, -6, -3)(-2, 9, 4)(3, 5, -4)(6, 8, -7)) with vectors {1: (3, 0), 2: (3, 3), 3: (0, 3), 4: (-3, 0), 5: (-3, -3), 6: (3, 0), 7: (3, 3), 8: (0, 3), 9: (6, 3)} → FlatTriangulationCombinatorial(vertices = (1, 9, 2, 4, -9, 5, 6, 7, 8, -6, 3, -2, -4, -3, -5, -1, -7, -8), faces = (1, -5, -9)(-1, -8, 7)(2, 3, -4)(-2, 9, 4)(-3, -6, 5)(6, 8, -7)) with vectors {1: (3, 0), 2: (3, 3), 3: (-6, -3), 4: (-3, 0), 5: (-3, -3), 6: (3, 0), 7: (3, 3), 8: (0, 3), 9: (6, 3)} flipping 3)) then (FlatTriangulationCombinatorial(vertices = (1, 9, 2, 4, -9, 5, 6, 7, 8, -6, 3, -2, -4, -3, -5, -1, -7, -8), faces = (1, -5, -9)(-1, -8, 7)(2, 3, -4)(-2, 9, 4)(-3, -6, 5)(6, 8, -7)) with vectors {1: (3, 0), 2: (3, 3), 3: (-6, -3), 4: (-3, 0), 5: (-3, -3), 6: (3, 0), 7: (3, 3), 8: (0, 3), 9: (6, 3)} → FlatTriangulationCombinatorial(vertices = (1, 5, 9, 2, 4, -9, 6, 7, 8, -6, -5, 3, -2, -4, -3, -1, -7, -8), faces = (1, -3, -5)(-1, -8, 7)(2, 3, -4)(-2, 9, 4)(5, -6, -9)(6, 8, -7)) with vectors {1: (3, 0), 2: (3, 3), 3: (-6, -3), 4: (-3, 0), 5: (9, 3), 6: (3, 0), 7: (3, 3), 8: (0, 3), 9: (6, 3)} flipping -5)) then (FlatTriangulationCombinatorial(vertices = (1, 5, 9, 2, 4, -9, 6, 7, 8, -6, -5, 3, -2, -4, -3, -1, -7, -8), faces = (1, -3, -5)(-1, -8, 7)(2, 3, -4)(-2, 9, 4)(5, -6, -9)(6, 8, -7)) with vectors {1: (3, 0), 2: (3, 3), 3: (-6, -3), 4: (-3, 0), 5: (9, 3), 6: (3, 0), 7: (3, 3), 8: (0, 3), 9: (6, 3)} → FlatTriangulationCombinatorial(vertices = (1, -10, 5, 9, 2, 4, -9, 6, 7, 8, -6, -5, -12, 3, -2, -4, -3, -11, -1, -7, -8)(10, 11, 12), faces = (1, -11, 10)(-1, -8, 7)(2, 3, -4)(-2, 9, 4)(-3, -12, 11)(5, -6, -9)(-5, -10, 12)(6, 8, -7)) with vectors {1: (3, 0), 2: (3, 3), 3: (-6, -3), 4: (-3, 0), 5: (9, 3), 6: (3, 0), 7: (3, 3), 8: (0, 3), 9: (6, 3), 10: (-4, -1), 11: (-1, -1), 12: (5, 2)} inserting {10, 11, 12})");
    }

    SECTION("Insert with Several Flips") {
      auto sector = HalfEdge(1);
      REQUIRE(fmt::format("{}", surface.insertAt(sector, R2(5, 1))) == "(((((FlatTriangulationCombinatorial(vertices = (1, 2, 3, 4, 5, -3, 6, 7, 8, -6, -2, 9, -4, -5, -9, -1, -7, -8), faces = (1, -9, -2)(-1, -8, 7)(2, -6, -3)(3, 5, -4)(4, 9, -5)(6, 8, -7)) with vectors {1: (3, 0), 2: (3, 3), 3: (0, 3), 4: (-3, 0), 5: (-3, -3), 6: (3, 0), 7: (3, 3), 8: (0, 3), 9: (0, -3)} → FlatTriangulationCombinatorial(vertices = (1, 9, 2, 3, 4, -9, 5, -3, 6, 7, 8, -6, -2, -4, -5, -1, -7, -8), faces = (1, -5, -9)(-1, -8, 7)(2, -6, -3)(-2, 9, 4)(3, 5, -4)(6, 8, -7)) with vectors {1: (3, 0), 2: (3, 3), 3: (0, 3), 4: (-3, 0), 5: (-3, -3), 6: (3, 0), 7: (3, 3), 8: (0, 3), 9: (6, 3)} flipping -9) then (FlatTriangulationCombinatorial(vertices = (1, 9, 2, 3, 4, -9, 5, -3, 6, 7, 8, -6, -2, -4, -5, -1, -7, -8), faces = (1, -5, -9)(-1, -8, 7)(2, -6, -3)(-2, 9, 4)(3, 5, -4)(6, 8, -7)) with vectors {1: (3, 0), 2: (3, 3), 3: (0, 3), 4: (-3, 0), 5: (-3, -3), 6: (3, 0), 7: (3, 3), 8: (0, 3), 9: (6, 3)} → FlatTriangulationCombinatorial(vertices = (1, 9, 2, 4, -9, 5, 6, 7, 8, -6, 3, -2, -4, -3, -5, -1, -7, -8), faces = (1, -5, -9)(-1, -8, 7)(2, 3, -4)(-2, 9, 4)(-3, -6, 5)(6, 8, -7)) with vectors {1: (3, 0), 2: (3, 3), 3: (-6, -3), 4: (-3, 0), 5: (-3, -3), 6: (3, 0), 7: (3, 3), 8: (0, 3), 9: (6, 3)} flipping 3)) then (FlatTriangulationCombinatorial(vertices = (1, 9, 2, 4, -9, 5, 6, 7, 8, -6, 3, -2, -4, -3, -5, -1, -7, -8), faces = (1, -5, -9)(-1, -8, 7)(2, 3, -4)(-2, 9, 4)(-3, -6, 5)(6, 8, -7)) with vectors {1: (3, 0), 2: (3, 3), 3: (-6, -3), 4: (-3, 0), 5: (-3, -3), 6: (3, 0), 7: (3, 3), 8: (0, 3), 9: (6, 3)} → FlatTriangulationCombinatorial(vertices = (1, 5, 9, 2, 4, -9, 6, 7, 8, -6, -5, 3, -2, -4, -3, -1, -7, -8), faces = (1, -3, -5)(-1, -8, 7)(2, 3, -4)(-2, 9, 4)(5, -6, -9)(6, 8, -7)) with vectors {1: (3, 0), 2: (3, 3), 3: (-6, -3), 4: (-3, 0), 5: (9, 3), 6: (3, 0), 7: (3, 3), 8: (0, 3), 9: (6, 3)} flipping -5)) then (FlatTriangulationCombinatorial(vertices = (1, 5, 9, 2, 4, -9, 6, 7, 8, -6, -5, 3, -2, -4, -3, -1, -7, -8), faces = (1, -3, -5)(-1, -8, 7)(2, 3, -4)(-2, 9, 4)(5, -6, -9)(6, 8, -7)) with vectors {1: (3, 0), 2: (3, 3), 3: (-6, -3), 4: (-3, 0), 5: (9, 3), 6: (3, 0), 7: (3, 3), 8: (0, 3), 9: (6, 3)} → FlatTriangulationCombinatorial(vertices = (1, 5, 9, 4, 2, -9, 6, 7, 8, -6, -5, 3, -4, -2, -3, -1, -7, -8), faces = (1, -3, -5)(-1, -8, 7)(2, -4, 9)(-2, 4, 3)(5, -6, -9)(6, 8, -7)) with vectors {1: (3, 0), 2: (-9, -3), 3: (-6, -3), 4: (-3, 0), 5: (9, 3), 6: (3, 0), 7: (3, 3), 8: (0, 3), 9: (6, 3)} flipping 2)) then (FlatTriangulationCombinatorial(vertices = (1, 5, 9, 4, 2, -9, 6, 7, 8, -6, -5, 3, -4, -2, -3, -1, -7, -8), faces = (1, -3, -5)(-1, -8, 7)(2, -4, 9)(-2, 4, 3)(5, -6, -9)(6, 8, -7)) with vectors {1: (3, 0), 2: (-9, -3), 3: (-6, -3), 4: (-3, 0), 5: (9, 3), 6: (3, 0), 7: (3, 3), 8: (0, 3), 9: (6, 3)} → FlatTriangulationCombinatorial(vertices = (1, 3, 5, 9, 4, -3, 2, -9, 6, 7, 8, -6, -5, -4, -2, -1, -7, -8), faces = (1, -2, -3)(-1, -8, 7)(2, -4, 9)(3, 4, -5)(5, -6, -9)(6, 8, -7)) with vectors {1: (3, 0), 2: (-9, -3), 3: (12, 3), 4: (-3, 0), 5: (9, 3), 6: (3, 0), 7: (3, 3), 8: (0, 3), 9: (6, 3)} flipping -3)) then (FlatTriangulationCombinatorial(vertices = (1, 3, 5, 9, 4, -3, 2, -9, 6, 7, 8, -6, -5, -4, -2, -1, -7, -8), faces = (1, -2, -3)(-1, -8, 7)(2, -4, 9)(3, 4, -5)(5, -6, -9)(6, 8, -7)) with vectors {1: (3, 0), 2: (-9, -3), 3: (12, 3), 4: (-3, 0), 5: (9, 3), 6: (3, 0), 7: (3, 3), 8: (0, 3), 9: (6, 3)} → FlatTriangulationCombinatorial(vertices = (1, -10, 3, 5, 9, 4, -3, -12, 2, -9, 6, 7, 8, -6, -5, -4, -2, -11, -1, -7, -8)(10, 11, 12), faces = (1, -11, 10)(-1, -8, 7)(2, -4, 9)(-2, -12, 11)(3, 4, -5)(-3, -10, 12)(5, -6, -9)(6, 8, -7)) with vectors {1: (3, 0), 2: (-9, -3), 3: (12, 3), 4: (-3, 0), 5: (9, 3), 6: (3, 0), 7: (3, 3), 8: (0, 3), 9: (6, 3), 10: (-5, -1), 11: (-2, -1), 12: (7, 2)} inserting {10, 11, 12})");
    }
  }
}

TEMPLATE_TEST_CASE("Detect Isomorphic Surfaces", "[FlatTriangulation][isomorphism]", (long long), (mpz_class), (mpq_class), (renf_elem_class), (exactreal::Element<exactreal::IntegerRing>), (exactreal::Element<exactreal::RationalField>), (exactreal::Element<exactreal::NumberField>)) {
  using T = TestType;
  using Transformation = std::tuple<T, T, T, T>;
  using Surface = FlatTriangulation<T>;

  SECTION("Generic Tests") {
    const auto surface_ = GENERATE_SURFACES(T);
    CAPTURE(surface_);

    auto surface = surface_->clone();

    const int delaunay = GENERATE(values({0, 1}));
    const auto isomorphism = delaunay ? ISOMORPHISM::DELAUNAY_CELLS : ISOMORPHISM::FACES;

    CAPTURE(delaunay);
    if (delaunay)
      surface.delaunay();

    REQUIRE(surface.isomorphism(surface, isomorphism));

    std::vector<Transformation> transformations = {Transformation{1, 0, 0, 1}};

    Transformation candidate;
    auto filter = [&](const T& a, const T& b, const T& c, const T& d) {
      candidate = Transformation{a, b, c, d};
      return std::find(begin(transformations), end(transformations), candidate) == end(transformations);
    };

    while (true) {
      const auto deformation = surface.isomorphism(surface, isomorphism, filter);

      if (!deformation)
        break;

      const auto [a, b, c, d] = candidate;
      CAPTURE(a, b, c, d);

      REQUIRE((!deformation->trivial() || (a == 1 && b == 0 && c == 0 && d == 1)));

      std::unordered_set<HalfEdge> image;
      for (const auto& halfEdge : surface.halfEdges()) {
        CAPTURE(halfEdge);

        if (delaunay && surface.delaunay(halfEdge.edge()) == DELAUNAY::AMBIGUOUS)
          continue;

        auto he = (*deformation)(halfEdge);

        REQUIRE(he.has_value());

        image.insert(*he);

        const auto v = surface.fromHalfEdge(halfEdge);
        const auto v_ = deformation->surface().fromHalfEdge(*he);
        REQUIRE(Vector<T>(v.x() * a + v.y() * b, v.x() * c + v.y() * d) == v_);
      }
      if (!delaunay)
        REQUIRE(image.size() == surface.halfEdges().size());

      transformations.push_back(candidate);

      REQUIRE((*deformation * deformation->section()).trivial());
      REQUIRE((deformation->section() * *deformation).trivial());
    }

    SECTION("Check Consistency with Equivalence::isomorphisms") {
      // Equivalences do not support working on subsets of edges (we need to
      // implement non-triangulated surfaces for this to work first.)
      if (isomorphism == ISOMORPHISM::FACES) {
        // GL equivalence is only supported when division is possible.
        if constexpr (isField<T>) {
          REQUIRE(Equivalence<Surface>::linear(false).isomorphisms(surface, surface).size() == Equivalence<Surface>::unlabeled().isomorphisms(surface, surface).size() * transformations.size());
        }
      }
    }

    auto scaled = surface.scale(2);
    CAPTURE(scaled);

    REQUIRE(!surface.isomorphism(scaled, isomorphism));
    REQUIRE(surface.isomorphism(scaled, isomorphism, [](const auto&, const auto&, const auto&, const auto&) { return true; }));
  }

  SECTION("Particular Cases") {
    using R2 = flatsurf::Vector<T>;
    SECTION("An Isomorphism Coming from a Veech Group Element") {
      const auto domain = FlatTriangulation<T>(
        FlatTriangulationCombinatorial(std::vector<std::vector<int>>{{1, -3, -8, 9, 3, -2, -7, 4, -5, 7, -6, 8, -9, 6, 2, -1, -4, 5}}),
        vector{R2(0, -1), R2(1, 1), R2(-1, 0), R2(1, 0), R2(-1, -1), R2(1, 0), R2(0, 1), R2(-1, -1), R2(0, 1)});
      const auto codomain = FlatTriangulation<T>(
        FlatTriangulationCombinatorial(std::vector<std::vector<int>>{{1, -3, 7, -6, 3, -2, -5, -9, 8, 5, -4, -7, 6, 4, 2, -1, 9, -8}}),
        vector{R2(0, -1), R2(1, 1), R2(-1, 0), R2(1, 0), R2(0, 1), R2(0, -1), R2(1, 1), R2(1, 1), R2(-1, 0)});

      CAPTURE(domain);
      CAPTURE(codomain);

      REQUIRE(domain.isomorphism(codomain, ISOMORPHISM::FACES).has_value());
      REQUIRE(domain.isomorphism(codomain, ISOMORPHISM::DELAUNAY_CELLS).has_value());
    }
  }
}

TEMPLATE_TEST_CASE("Insert a slit into a Flat Triangulation", "[FlatTriangulation][slit]", (long long), (mpz_class), (mpq_class), (renf_elem_class), (exactreal::Element<exactreal::IntegerRing>), (exactreal::Element<exactreal::RationalField>), (exactreal::Element<exactreal::NumberField>)) {
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

TEMPLATE_TEST_CASE("Relabel half edges of a Flat Triangulation", "[FlatTriangulation][relabel]", (long long), (mpz_class), (mpq_class), (renf_elem_class), (exactreal::Element<exactreal::IntegerRing>), (exactreal::Element<exactreal::RationalField>), (exactreal::Element<exactreal::NumberField>)) {
  using T = TestType;

  const auto surface = GENERATE_SURFACES(T);

  SECTION("Swap half edge 1 and its Negative") {
    std::unordered_map<HalfEdge, HalfEdge> swap;
    for (const auto& halfEdge : surface->halfEdges())
      swap[halfEdge] = halfEdge;

    swap[HalfEdge(1)] = HalfEdge(-1);
    swap[HalfEdge(-1)] = HalfEdge(1);

    const Permutation<HalfEdge> permutation{swap};
    CAPTURE(permutation);

    const auto swapped = surface->relabel(permutation);

    REQUIRE(*surface != swapped.codomain());

    const auto restored = swapped.codomain().relabel(permutation);

    REQUIRE(*surface == restored.codomain());
  }
}

TEMPLATE_TEST_CASE("Serialization of a FlatTriangulation", "[FlatTriangulation][save][load]", (long long), (mpz_class), (mpq_class), (renf_elem_class), (exactreal::Element<exactreal::IntegerRing>), (exactreal::Element<exactreal::RationalField>), (exactreal::Element<exactreal::NumberField>)) {
  using R2 = Vector<TestType>;
  auto square = makeSquare<R2>();

  testRoundtrip(*square);
}

TEST_CASE("Example from the README.md", "[FlatTriangulation]") {
  // Make sure to update the README.md if you make any changes to this test case.
  using flatsurf::FlatTriangulation;
  using R = eantic::renf_elem_class;
  using R2 = flatsurf::Vector<R>;

  // We create a number field to represent the edges of the hexagon.
  auto K = eantic::renf_class::make("x^2 - 3", "x", "1.73 +/- 0.1");
  auto x = K->gen();

  // The edge vectors of a regular hexagon.
  auto vectors = std::vector{R2(R(*K, 2), R(*K, 0)), R2(R(*K, 1), x), R2(R(*K, 3), x), R2(R(*K, 1), -x), R2(R(*K, 4), R(*K, 0)), R2(R(*K, 3), x)};

  // The edges are going to be labeled 1,…,6.
  // The labels -1,…,-6 refer to the negative of these edges.
  // We give the permutation obtained by walking around the two marked vertices
  // of the hexagon to specify the edge gluing in our translation surface.
  auto vertices = std::vector<std::vector<int>>({{1, 3, -4, -5, -3, -2}, {2, -1, -6, 4, 5, 6}});

  auto hexagon = FlatTriangulation(vertices, vectors);

  REQUIRE(fmt::format("{}", hexagon) == "FlatTriangulationCombinatorial(vertices = (1, 3, -4, -5, -3, -2)(-1, -6, 4, 5, 6, 2), faces = (1, 2, -3)(-1, -2, 6)(3, -5, 4)(-4, -6, 5)) with vectors {1: (2, 0), 2: (1, (x ~ 1.7320508)), 3: (3, (x ~ 1.7320508)), 4: (1, (-x ~ -1.7320508)), 5: (4, 0), 6: (3, (x ~ 1.7320508))}");

  auto connections = flatsurf::SaddleConnections(hexagon).bound(4);

  std::string out;
  for (auto c : connections)
    out += fmt::format("{}\n", c);

  REQUIRE(out == "1\n-1\n2\n(-3, (x ~ 1.7320508)) from 2 to 4\n(0, (2*x ~ 3.4641016)) from 2 to -6\n(-2, (2*x ~ 3.4641016)) from 2 to -2\n-2\n(3, (-x ~ -1.7320508)) from -2 to -4\n(0, (-2*x ~ -3.4641016)) from -2 to 3\n(2, (-2*x ~ -3.4641016)) from -2 to 2\n3\n(2, (2*x ~ 3.4641016)) from 3 to -6\n(0, (2*x ~ 3.4641016)) from 3 to -2\n-3\n4\n(3, (-x ~ -1.7320508)) from 4 to 2\n-4\n(-3, (x ~ 1.7320508)) from -4 to -2\n5\n-5\n6\n-6\n(-2, (-2*x ~ -3.4641016)) from -6 to 3\n(0, (-2*x ~ -3.4641016)) from -6 to 2\n");
}

}  // namespace flatsurf::test
