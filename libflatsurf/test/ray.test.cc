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

#include "../flatsurf/ccw.hpp"
#include "../flatsurf/segment.hpp"

#include "generators/surface_generator.hpp"
#include "generators/half_edge_generator.hpp"
#include "generators/point_generator.hpp"
#include "generators/ray_generator.hpp"
#include "generators/saddle_connection_generator.hpp"

namespace flatsurf::test {

TEMPLATE_TEST_CASE("Creating Rays", "[Ray][constructor]", (long long), (mpq_class), (renf_elem_class), (exactreal::Element<exactreal::IntegerRing>), (exactreal::Element<exactreal::RationalField>), (exactreal::Element<exactreal::NumberField>)) {
  using T = TestType;

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

TEMPLATE_TEST_CASE("Turning Sense of Rays", "[Ray][ccw]", (long long), (mpq_class), (renf_elem_class), (exactreal::Element<exactreal::IntegerRing>), (exactreal::Element<exactreal::RationalField>), (exactreal::Element<exactreal::NumberField>)) {
  using T = TestType;

  const auto surface = GENERATE_SURFACES(T);
  CAPTURE(surface);

  const auto face = GENERATE_COPY(halfEdges(surface));

  const auto ray = GENERATE_COPY(rays(surface, face));

  SECTION("A Ray is Collinear to Itself and its Negative") {
    REQUIRE(ray.ccw(ray) == CCW::COLLINEAR);

    if (surface->angle(ray.start()) == 1)
      REQUIRE(ray.ccw(Ray{ray.start(), -ray.vector()}) == CCW::COLLINEAR);
  }

  SECTION("Turning is Consistent") {
    const auto s = GENERATE_COPY(rays(surface, face));

    if (ray.start() == s.start())
      REQUIRE(ray.ccw(s) == -s.ccw(ray));
  }
}

TEMPLATE_TEST_CASE("Rays are Compatible with Saddle Connections", "[Ray][saddleConnection][source][vector][start]", (long long), (mpq_class), (renf_elem_class), (exactreal::Element<exactreal::IntegerRing>), (exactreal::Element<exactreal::RationalField>), (exactreal::Element<exactreal::NumberField>)) {
  using T = TestType;

  const auto surface = GENERATE_SURFACES(T);
  CAPTURE(surface);

  const auto saddleConnection = GENERATE_COPY(saddleConnections(surface));
  CAPTURE(saddleConnection);

  const auto ray = saddleConnection.ray(); 
  CAPTURE(ray);

  REQUIRE(ray.saddleConnection() == saddleConnection);
  REQUIRE(ray.source() == saddleConnection.source());
  REQUIRE(ray.vector().ccw(saddleConnection.vector()) == CCW::COLLINEAR);
  REQUIRE(ray.start() == Point{*surface, saddleConnection.start()});
}

TEMPLATE_TEST_CASE("A Ray Remembers its Surface", "[Ray][surface]", (long long), (mpq_class), (renf_elem_class), (exactreal::Element<exactreal::IntegerRing>), (exactreal::Element<exactreal::RationalField>), (exactreal::Element<exactreal::NumberField>)) {
  using T = TestType;

  const auto surface = GENERATE_SURFACES(T);
  CAPTURE(surface);

  const auto face = GENERATE_COPY(halfEdges(surface));

  const auto ray = GENERATE_COPY(rays(surface, face));
  CAPTURE(ray);

  REQUIRE(ray.surface() == *surface);
}

TEMPLATE_TEST_CASE("A Ray can be used to Define a Segment", "[Ray][segment]", (long long), (mpq_class), (renf_elem_class), (exactreal::Element<exactreal::IntegerRing>), (exactreal::Element<exactreal::RationalField>), (exactreal::Element<exactreal::NumberField>)) {
  using T = TestType;

  const auto surface = GENERATE_SURFACES(T);
  CAPTURE(surface);

  const auto face = GENERATE_COPY(halfEdges(surface));

  const auto ray = Ray{*surface, face};

  const auto segment = ray.segment(Point{*surface, -face, T(1), T(), T()});

  REQUIRE(segment == SaddleConnection{*surface, face}.segment());
}


TEMPLATE_TEST_CASE("Rays can be Compared", "[Ray][operator==][hash]", (long long), (mpq_class), (renf_elem_class), (exactreal::Element<exactreal::IntegerRing>), (exactreal::Element<exactreal::RationalField>), (exactreal::Element<exactreal::NumberField>)) {
  using T = TestType;
  using Surface = FlatTriangulation<T>;

  const auto surface = GENERATE_SURFACES(T);
  CAPTURE(surface);

  const auto face = GENERATE_COPY(halfEdges(surface));

  const auto ray = GENERATE_COPY(rays(surface, face));
  CAPTURE(ray);

  const auto ray2 = Ray{ray.start(), ray.source(), ray.vector() * 2};

  const auto perpendicular = Ray{*surface, ray.source(), Vector<T>{ray.vector().y(), -ray.vector().x()}};

  REQUIRE(ray == ray);
  REQUIRE(ray == ray2);
  REQUIRE(ray != perpendicular);

  const auto hash = std::hash<Ray<Surface>>{};
  REQUIRE(hash(ray) == hash(ray2));

  // Check that the hash function can distinguish such evidently different rays.
  REQUIRE(hash(ray) != hash(perpendicular));
}

TEMPLATE_TEST_CASE("Serialization of Rays", "[Ray][save][load]", (long long), (mpq_class), (renf_elem_class), (exactreal::Element<exactreal::IntegerRing>), (exactreal::Element<exactreal::RationalField>), (exactreal::Element<exactreal::NumberField>)) {
  // TODO
}

}
