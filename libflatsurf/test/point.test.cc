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
#include "../flatsurf/bound.hpp"
#include "../flatsurf/saddle_connection.hpp"
#include "../flatsurf/saddle_connections.hpp"
#include "../flatsurf/saddle_connections_iterator.hpp"
#include "generators/surface_generator.hpp"
#include "generators/half_edge_generator.hpp"
#include "generators/point_generator.hpp"

namespace flatsurf::test {

TEMPLATE_TEST_CASE("Creating Points", "[Point][constructor]", (long long), (mpq_class), (renf_elem_class), (exactreal::Element<exactreal::IntegerRing>), (exactreal::Element<exactreal::RationalField>), (exactreal::Element<exactreal::NumberField>)) {
  using T = TestType;

  const auto surface = GENERATE_SURFACES(T);
  CAPTURE(surface);

  const auto face = GENERATE_COPY(halfEdges(surface));
  CAPTURE(face);

  SECTION("Points from Vertices") {
    const auto p = Point(*surface, Vertex::source(face, *surface));

    REQUIRE(p == Point(*surface, face, T(1), T(), T()));
  }

  SECTION("Points from Barycentric Coordinates") {
    const auto p = Point(*surface, face, T(1), T(2), T(3));
    const auto q = Point(*surface, face, std::array{T(1), T(2), T(3)});

    REQUIRE(p == q);
  }

  SECTION("Points from Flow") {
    if constexpr (hasFractions<T>) {
      // We flow by some arbitrary amount from a vertex.
      // There is no reason why that flow should be possible in general since
      // we might hit a non-marked vertex on the path. But this is not
      // happening in the surfaces we are testing for.
      const auto flow = 7 * surface->fromHalfEdge(face) / 3 + 19 * surface->fromHalfEdge(surface->nextAtVertex(face)) / 5;
      CAPTURE(flow);

      const auto p = Point(*surface, face, flow);
      CAPTURE(p);

      if (surface->angle(p) == 1) {
        const auto q = p - flow;

        REQUIRE(q == Point(*surface, Vertex::source(face, *surface)));
      }
    }
  }
}

TEMPLATE_TEST_CASE("Flowing Points", "[Point][operator+=][operator-=]", (long long), (mpq_class), (renf_elem_class), (exactreal::Element<exactreal::IntegerRing>), (exactreal::Element<exactreal::RationalField>), (exactreal::Element<exactreal::NumberField>)) {
  using T = TestType;

  const auto surface = GENERATE_SURFACES(T);
  CAPTURE(surface);

  const auto face = GENERATE_COPY(halfEdges(surface));

  SECTION("Flow from Marked Vertices") {
    const Point point{*surface, face, T(1), T(), T()};

    if (surface->angle(point) == 1) {
      SECTION("Flow to Another Vertex") {
        REQUIRE(point + surface->fromHalfEdge(face) == Point{*surface, face, T(), T(1), T()});
      }

      if constexpr (hasFractions<T>) {
        SECTION("Flow to an Edge") {
          const auto f = surface->fromHalfEdge(face);
          const auto g = surface->fromHalfEdge(surface->nextAtVertex(face));
          REQUIRE(point + f / 2 == Point{*surface, face, T(1), T(1), T()});
          REQUIRE(point + (f / 2 + g / 2) == Point{*surface, face, T(), T(1), T(1)});
          REQUIRE(point + f / 2 + g / 2 == Point{*surface, face, T(), T(1), T(1)});
        }
      }
    }
  }

  SECTION("Flow from an Edge") {
    const Point point{*surface, face, T(1), T(1), T()};

    if constexpr (hasFractions<T>) {
      SECTION("Flow to a Vertex") {
        auto f = surface->fromHalfEdge(face);
        auto g = surface->fromHalfEdge(surface->nextInFace(face));
        REQUIRE(point + f / 2 == Point{*surface, face, T(), T(1), T()});
        REQUIRE(point - f / 2 == Point{*surface, face, T(1), T(), T()});
        REQUIRE(point + (f / 2 + g) == Point{*surface, face, T(), T(), T(1)});
        if (surface->angle(Vertex::target(face, *surface)) == 1)
          REQUIRE(point + f / 2 + g == Point{*surface, face, T(), T(), T(1)});
      }

      SECTION("Flow to an Edge") {
        auto f = surface->fromHalfEdge(face);
        auto g = surface->fromHalfEdge(surface->nextInFace(face));
        REQUIRE(point + f / 4 == Point{*surface, face, T(1), T(3), T()});
        REQUIRE(point + (f / 2 + g / 2) == Point{*surface, face, T(), T(1), T(1)});
        if (surface->angle(Vertex::target(face, *surface)) == 1)
          REQUIRE(point + f / 2 + g / 2 == Point{*surface, face, T(), T(1), T(1)});
      }
    }
  }

  SECTION("Flow Randomly") {
    const auto point = GENERATE_COPY(points(surface, face));
    CAPTURE(point);

    if constexpr (hasFractions<T>) {
      // We flow by some arbitrary amount from a point.
      // There is no reason why that flow should be possible in general since
      // we might hit a non-marked vertex on the path. But this is not
      // happening in the surfaces we are testing for.
      if (surface->angle(point) == 1) {
        const auto flow = 7 * surface->fromHalfEdge(face) / 3 + 19 * surface->fromHalfEdge(surface->nextAtVertex(face)) / 5;
        CAPTURE(flow);

        const auto q = point + flow;
        if (surface->angle(q) == 1) {
          REQUIRE(q - flow == point);
        }
      }
    }
  }
}

TEMPLATE_TEST_CASE("Coordinates of Points", "[Point][coordinates][vertex][edge][face][vector]", (long long), (mpq_class), (renf_elem_class), (exactreal::Element<exactreal::IntegerRing>), (exactreal::Element<exactreal::RationalField>), (exactreal::Element<exactreal::NumberField>)) {
  using T = TestType;

  const auto surface = GENERATE_SURFACES(T);
  CAPTURE(surface);

  const auto face = GENERATE_COPY(halfEdges(surface));

  const auto point = GENERATE_COPY(points(surface, face));
  CAPTURE(point);

  SECTION("Barycentric Point Coordinates Corresponding to Position in Face") {
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
  }

  SECTION("Barycentric Point Coordinates Roundtrip") {
    for (const auto face_ : surface->face(face)) {
      CAPTURE(face_);
      const auto coordinates_ = point.coordinates(face_);
      CAPTURE(coordinates_);
      const auto point_ = Point(*surface, face_, coordinates_);
      CAPTURE(point_);

      REQUIRE(point == point_);
    }
  }

  SECTION("Cartesian Point Coordinates Roundtrip") {
    for (const auto face_ : surface->face(face)) {
      CAPTURE(face_);
      const auto coordinates_ = [&]() -> std::optional<Vector<T>> {
        try {
          return point.vector(face_);
        } catch (...) {
          if (std::is_same_v<T, long long> ||
              std::is_same_v<T, mpz_class> ||
              std::is_same_v<T, exactreal::Element<exactreal::IntegerRing>>)
            // Without division, cartesian coordinates might not be in the base ring.
            return std::nullopt;

          throw;
        }
      }();
      if (coordinates_) {
        CAPTURE(*coordinates_);
        const auto point_ = Point(*surface, face_, *coordinates_);
        CAPTURE(point_);

        REQUIRE(point == point_);
      }
    }
  }
}

TEMPLATE_TEST_CASE("Predicates in/at/on of Points", "[Point][in][at][on])", (long long), (mpq_class), (renf_elem_class), (exactreal::Element<exactreal::IntegerRing>), (exactreal::Element<exactreal::RationalField>), (exactreal::Element<exactreal::NumberField>)) {
  using T = TestType;

  const auto surface = GENERATE_SURFACES(T);
  CAPTURE(surface);

  const auto face = GENERATE_COPY(halfEdges(surface));

  const auto point = GENERATE_COPY(points(surface, face));
  CAPTURE(point);

  REQUIRE(point.in(point.face()));

  const auto vertex = point.vertex();
  const auto edge = point.edge();

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

TEMPLATE_TEST_CASE("Equality of Points", "[Point][operator==][hash]", (long long), (mpq_class), (renf_elem_class), (exactreal::Element<exactreal::IntegerRing>), (exactreal::Element<exactreal::RationalField>), (exactreal::Element<exactreal::NumberField>)) {
  using T = TestType;
  using Surface = FlatTriangulation<T>;

  const auto surface = GENERATE_SURFACES(T);
  CAPTURE(surface);

  const auto face = GENERATE_COPY(halfEdges(surface));

  const auto point = GENERATE_COPY(points(surface, face));
  CAPTURE(point);

  REQUIRE(point == point);

  SECTION("Compare to Vertices") {
    const auto vertex = point.vertex();

    if (vertex) {
      REQUIRE(point == Point(*surface, *vertex));
    } else {
      REQUIRE(point != Point(*surface, *surface->vertices().begin()));
    }
  }

  SECTION("Compare to Edges") {
    const auto edge = point.edge();

    if (edge) {
      const auto coordinates = point.coordinates(edge->positive());
      REQUIRE(!coordinates[2]);
      REQUIRE(point == Point(*surface, edge->negative(), coordinates[1], coordinates[0], T()));
    } else {
      REQUIRE(point != Point(*surface, HalfEdge(1), T(1), T(1), T()));
    }
  }

  SECTION("Point are Hashable") {
    REQUIRE(std::hash<Point<Surface>>{}(point) == std::hash<Point<Surface>>{}(point));

    const auto point_ = GENERATE_COPY(points(surface, face));
    CAPTURE(point_);

    if (point != point_)
      REQUIRE(std::hash<Point<Surface>>{}(point) != std::hash<Point<Surface>>{}(point_));
  }
}

TEMPLATE_TEST_CASE("Points Remember the Surface they are Defined On", "[Point][surface]", (long long), (mpq_class), (renf_elem_class), (exactreal::Element<exactreal::IntegerRing>), (exactreal::Element<exactreal::RationalField>), (exactreal::Element<exactreal::NumberField>)) {
  using T = TestType;

  const auto surface = GENERATE_SURFACES(T);

  const auto face = GENERATE_COPY(halfEdges(surface));

  const auto point = GENERATE_COPY(points(surface, face));

  REQUIRE(point.surface() == *surface);
}

}
