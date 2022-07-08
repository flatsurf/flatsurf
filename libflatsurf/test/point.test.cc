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

#include "../flatsurf/point.hpp"
#include "../flatsurf/edge.hpp"
#include "../flatsurf/vertex.hpp"
#include "generators/surface_generator.hpp"
#include "generators/half_edge_generator.hpp"
#include "generators/point_generator.hpp"

namespace flatsurf::test {

TEMPLATE_TEST_CASE("Coordinates of Points", "[point]", (long long), (mpq_class), (renf_elem_class), (exactreal::Element<exactreal::IntegerRing>), (exactreal::Element<exactreal::NumberField>)) {
  using T = TestType;

  const auto surface = GENERATE_SURFACES(T);

  const auto face = GENERATE_COPY(halfEdges(surface));
  const auto point = GENERATE_COPY(points(surface, face));
  CAPTURE(point);

  SECTION("Point Coordinates Roundtrip") {
    for (const auto face_ : surface->face(face)) {
      CAPTURE(face_);
      const auto coordinates_ = point.coordinates(face_);
      CAPTURE(coordinates_);
      const auto point_ = Point(*surface, face_, coordinates_);
      CAPTURE(point_);

      REQUIRE(point == point_);
    }
  }
}

TEMPLATE_TEST_CASE("Predicates of Points", "[point]", (long long), (mpq_class), (renf_elem_class), (exactreal::Element<exactreal::IntegerRing>), (exactreal::Element<exactreal::NumberField>)) {
  using T = TestType;

  const auto surface = GENERATE_SURFACES(T);

  const auto face = GENERATE_COPY(halfEdges(surface));

  const auto point = GENERATE_COPY(points(surface, face));
  CAPTURE(point);

  REQUIRE(point.in(point.face()));
  
  const auto [a, b, c] = point.coordinates(point.face());
  REQUIRE(a >= 0);
  REQUIRE(b >= 0);
  REQUIRE(c >= 0);

  const int positives = (a > 0) + (b > 0) + (c > 0);
  CAPTURE(positives);

  const auto vertex = point.vertex();
  const auto edge = point.edge();

  REQUIRE(positives != 0);
  REQUIRE((positives == 1) == vertex.has_value());
  REQUIRE((positives <= 2) == edge.has_value());

  if (vertex) {
    REQUIRE(point.at(*vertex));
    REQUIRE(edge);
  }

  if (edge) {
    REQUIRE(point.on(*edge));
    if (!vertex) {
      REQUIRE(!point.on(surface->nextInFace(edge->positive())));
      REQUIRE(!point.on(surface->previousInFace(edge->positive())));
    }
  } else {
    REQUIRE(!point.in(-point.face()));
  }
}

}
