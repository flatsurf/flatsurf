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

TEMPLATE_TEST_CASE("Delaunay Triangulation of a Square", "[flat_triangulation][delaunay]", (long long), (mpz_class), (mpq_class), (eantic::renf_elem_class), (Element<exactreal::IntegerRing>), (Element<exactreal::RationalField>), (Element<exactreal::NumberField>)) {
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

TEMPLATE_TEST_CASE("Delaunay Triangulation of an Octagon", "[flat_triangulation][delaunay]", (eantic::renf_elem_class), (Element<exactreal::NumberField>)) {
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

TEMPLATE_TEST_CASE("Delaunay Triangulation", "[flat_triangulation][delaunay]", (long long), (mpz_class), (mpq_class), (renf_elem_class), (exactreal::Element<exactreal::IntegerRing>), (exactreal::Element<exactreal::RationalField>), (exactreal::Element<exactreal::NumberField>)) {
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

}  // namespace flatsurf::test

