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

TEMPLATE_TEST_CASE("Insert into a Flat Triangulation", "[flat_triangulation][insert]", (long long), (mpz_class), (mpq_class), (renf_elem_class), (exactreal::Element<exactreal::IntegerRing>), (exactreal::Element<exactreal::RationalField>), (exactreal::Element<exactreal::NumberField>)) {
  using T = TestType;
  using R2 = Vector<T>;

  SECTION("Insert into a Surface") {
    auto surface = GENERATE_SURFACES(T);
    CAPTURE(surface);

    auto face = GENERATE_COPY(halfEdges(surface));
    auto point = GENERATE_COPY(points(surface, face));
    CAPTURE(point);

    if (point.vertex()) {
      REQUIRE_THROWS(surface->insert(point));
    } else {
      const auto insertion = [&]() -> std::optional<Deformation<FlatTriangulation<T>>> {
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

}  // namespace flatsurf::test

