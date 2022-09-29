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

#include "../flatsurf/segment.hpp"
#include "../flatsurf/saddle_connection.hpp"
#include "../flatsurf/vertex.hpp"
#include "../flatsurf/ray.hpp"
#include "generators/surface_generator.hpp"
#include "generators/half_edge_generator.hpp"
#include "generators/segment_generator.hpp"

namespace flatsurf::test {

TEMPLATE_TEST_CASE("Creating Segments", "[Segment][constructor]", (long long), (mpq_class), (renf_elem_class), (exactreal::Element<exactreal::IntegerRing>), (exactreal::Element<exactreal::RationalField>), (exactreal::Element<exactreal::NumberField>)) {
  using T = TestType;
  using Surface = FlatTriangulation<T>;

  const auto surface = GENERATE_SURFACES(T);
  CAPTURE(surface);

  const auto face = GENERATE_COPY(halfEdges(surface));

  SECTION("Segment Starting at a Vertex") {
    const Point start{*surface, face, T(1), T(), T()};

    SECTION("Segment Ending at Another Vertex") {
      const Point end{*surface, face, T(), T(1), T()};
      const auto vector = surface->fromHalfEdge(face);

      const Segment<Surface> s{start, face, vector};
      const Segment<Surface> t{start, face, end, -face, vector};

      REQUIRE(s == t);
    }

    if constexpr (hasFractions<T>) {
      SECTION("Segment Ending on an Edge") {
        const Point end{*surface, face, T(1), T(1), T()};
        const auto vector = surface->fromHalfEdge(face) / 2;

        const Segment<Surface> s{start, face, vector};
        const Segment<Surface> t{start, face, end, -face, vector};

        REQUIRE(s == t);
      }

      SECTION("Segment Ending inside a Face") {
        const Point end{*surface, face, T(1), T(1), T(1)};
        const auto vector = surface->fromHalfEdge(face) / 4 + surface->fromHalfEdge(surface->previousAtVertex(face)) / 4;

        const Segment<Surface> s{start, face, vector};
        const Segment<Surface> t{start, face, end, face, vector};

        REQUIRE(s == t);
      }
    }
  }

  SECTION("Segment Starting inside a Face") {
    const Point start{*surface, face, T(1), T(1), T(1)};

    if constexpr (hasFractions<T>) {
      SECTION("Segment ending inside another Face") {
        const Point end{*surface, -face, T(1), T(1), T(1)};
        const auto vector = - surface->fromHalfEdge(surface->previousAtVertex(face)) / 4 + surface->fromHalfEdge(surface->nextAtVertex(face)) / 4;

        const Segment<Surface> s{start, end, vector};
        const Segment<Surface> t{start, vector};

        REQUIRE(s == t);
      }
    }
  }

  SECTION("Segments must not be trivial") {
    const Point start{*surface, face, T(1), T(1), T(1)};

    REQUIRE_THROWS(Segment<Surface>(start, {T(), T()}));
  }
}

TEMPLATE_TEST_CASE("Defining Data of a Segment", "[Segment][start][end][source][target][vector]", (long long), (mpq_class), (renf_elem_class), (exactreal::Element<exactreal::IntegerRing>), (exactreal::Element<exactreal::RationalField>), (exactreal::Element<exactreal::NumberField>)) {
  using T = TestType;

  const auto surface = GENERATE_SURFACES(T);
  CAPTURE(surface);

  const auto face = GENERATE_COPY(halfEdges(surface));

  const auto segment = GENERATE_COPY(segments(surface, face));

  CAPTURE(segment);

  REQUIRE_NOTHROW(segment.start().coordinates(segment.source()));
  REQUIRE_NOTHROW(segment.end().coordinates(segment.target()));

  REQUIRE(segment.end() == Point(*surface, segment.source(), segment.vector()));
}

TEMPLATE_TEST_CASE("Segments know whether they are a Saddle Connection", "[Segment][saddleConnection]", (long long), (mpq_class), (renf_elem_class), (exactreal::Element<exactreal::IntegerRing>), (exactreal::Element<exactreal::RationalField>), (exactreal::Element<exactreal::NumberField>)) {
  using T = TestType;
  using Surface = FlatTriangulation<T>;

  const auto surface = GENERATE_SURFACES(T);
  CAPTURE(surface);

  const auto face = GENERATE_COPY(halfEdges(surface));

  const auto segment = GENERATE_COPY(segments(surface, face));

  CAPTURE(segment);

  const auto saddleConnection = segment.saddleConnection();

  REQUIRE(saddleConnection.has_value() == (segment.start().vertex() && segment.end().vertex()));

  if (saddleConnection)
    REQUIRE(static_cast<const Segment<Surface>&>(*saddleConnection) == segment);
}

TEMPLATE_TEST_CASE("Segments can be Reversed", "[Segment][operator-]", (long long), (mpq_class), (renf_elem_class), (exactreal::Element<exactreal::IntegerRing>), (exactreal::Element<exactreal::RationalField>), (exactreal::Element<exactreal::NumberField>)) {
  using T = TestType;

  const auto surface = GENERATE_SURFACES(T);
  CAPTURE(surface);

  const auto face = GENERATE_COPY(halfEdges(surface));

  const auto segment = GENERATE_COPY(segments(surface, face));

  CAPTURE(segment);

  REQUIRE(-(-segment) == segment);
  REQUIRE(-(-segment.vector()) == -segment.vector());
}

TEMPLATE_TEST_CASE("Segments can be Compared", "[Segment][operator==][hash]", (long long), (mpq_class), (renf_elem_class), (exactreal::Element<exactreal::IntegerRing>), (exactreal::Element<exactreal::RationalField>), (exactreal::Element<exactreal::NumberField>)) {
  using T = TestType;
  using Surface = FlatTriangulation<T>;

  const auto surface = GENERATE_SURFACES(T);
  CAPTURE(surface);

  const auto face = GENERATE_COPY(halfEdges(surface));

  const auto segment = GENERATE_COPY(segments(surface, face));

  CAPTURE(segment);
  
  REQUIRE(segment == segment);
  REQUIRE(segment != -segment);
  REQUIRE(std::hash<Segment<Surface>>{}(segment) == std::hash<Segment<Surface>>{}(segment));

  // Verify that the hash function is good enough to distinguish such trivial cases.
  REQUIRE(std::hash<Segment<Surface>>{}(segment) != std::hash<Segment<Surface>>{}(-segment));
}

TEMPLATE_TEST_CASE("Segments Remember the Surface they are Defined On", "[Segment][surface]", (long long), (mpq_class), (renf_elem_class), (exactreal::Element<exactreal::IntegerRing>), (exactreal::Element<exactreal::RationalField>), (exactreal::Element<exactreal::NumberField>)) {
  using T = TestType;

  const auto surface = GENERATE_SURFACES(T);
  CAPTURE(surface);

  const auto face = GENERATE_COPY(halfEdges(surface));

  const auto segment = GENERATE_COPY(segments(surface, face));

  CAPTURE(segment);

  REQUIRE(segment.surface() == *surface);
}

TEMPLATE_TEST_CASE("Segments Define a Direction", "[Segment][ray]", (long long), (mpq_class), (renf_elem_class), (exactreal::Element<exactreal::IntegerRing>), (exactreal::Element<exactreal::RationalField>), (exactreal::Element<exactreal::NumberField>)) {
  using T = TestType;

  const auto surface = GENERATE_SURFACES(T);
  CAPTURE(surface);

  const auto face = GENERATE_COPY(halfEdges(surface));

  const auto segment = GENERATE_COPY(segments(surface, face));
  CAPTURE(segment);

  const auto ray = segment.ray();

  REQUIRE(ray.start() == segment.start());
  REQUIRE(ray.vector().parallel(segment.vector()));
}

TEMPLATE_TEST_CASE("Segments can Self-Overlap", "[Segment][overlapping]", (long long), (mpq_class), (renf_elem_class), (exactreal::Element<exactreal::IntegerRing>), (exactreal::Element<exactreal::RationalField>), (exactreal::Element<exactreal::NumberField>)) {
  using T = TestType;
  using Surface = FlatTriangulation<T>;

  const auto surface = GENERATE_SURFACES(T);
  CAPTURE(surface);

  const auto face = GENERATE_COPY(halfEdges(surface));

  if constexpr (!hasFractions<T>) {
    const auto segment = GENERATE_COPY(segments(surface, face));
    CAPTURE(segment);

    const auto shorter = Segment<Surface>{segment.start(), segment.source(), segment.vector() / 2};

    if (!segment.overlapping())
      REQUIRE(!shorter.overlapping());

    if (segment.start().vertex() && segment.end().vertex() && surface->angle(segment.start()) == 1 && surface->angle(segment.end()) == 1) {
      REQUIRE(Segment{segment.start(), segment.source(), segment.end(), segment.target(), segment.vector() * 2}.overlapping());
    }
  }
}

TEMPLATE_TEST_CASE("Serialization of Segments", "[Segment][save][load]", (long long), (mpq_class), (renf_elem_class), (exactreal::Element<exactreal::IntegerRing>), (exactreal::Element<exactreal::RationalField>), (exactreal::Element<exactreal::NumberField>)) {
  // TODO
}

}
