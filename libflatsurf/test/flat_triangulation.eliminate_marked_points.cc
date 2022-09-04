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

TEMPLATE_TEST_CASE("Eliminate Marked Points", "[flat_triangulation][eliminate_marked_points]", (long long), (mpz_class), (mpq_class), (renf_elem_class), (exactreal::Element<exactreal::IntegerRing>), (exactreal::Element<exactreal::RationalField>), (exactreal::Element<exactreal::NumberField>)) {
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

  REQUIRE(surface->area() == simplified.codomain().area());

  for (const auto preimage : surface->halfEdges()) {
    if (surface->angle(Vertex::source(preimage, *surface)) != 1 && surface->angle(Vertex::target(preimage, *surface)) != 1) {
      REQUIRE(simplified(SaddleConnection<Surface>(*surface, preimage)).has_value());
      REQUIRE(simplified(SaddleConnection<Surface>(*surface, preimage))->begin()->vector() == surface->fromHalfEdge(preimage));
    }
  }

  const auto section = simplified.section();
  CAPTURE(section);

  for (const auto image : section.domain().halfEdges()) {
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

}  // namespace flatsurf::test

