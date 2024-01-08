/**********************************************************************
 *  This file is part of flatsurf.
 *
 *        Copyright (C) 2021-2022 Julian Rüth
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

#include <exact-real/element.hpp>

#include "../flatsurf/ccw.hpp"
#include "../flatsurf/orientation.hpp"
#include "../flatsurf/saddle_connection.hpp"
#include "../flatsurf/half_edge_set.hpp"
#include "../flatsurf/half_edge_set_iterator.hpp"
#include "../flatsurf/vertical.hpp"
#include "cereal.helpers.hpp"
#include "generators/saddle_connection_generator.hpp"
#include "generators/surface_generator.hpp"

namespace flatsurf::test {

TEMPLATE_TEST_CASE("Angle between Saddle Connections", "[SaddleConnection][angle]", (long long), (mpq_class), (renf_elem_class), (exactreal::Element<exactreal::IntegerRing>), (exactreal::Element<exactreal::NumberField>)) {
  using T = TestType;
  using Surface = FlatTriangulation<T>;

  const auto surface = GENERATE_SURFACES(T);
  CAPTURE(surface);

  const auto connection = GENERATE_REF(saddleConnections(surface));
  CAPTURE(connection);

  const auto totalAngle = surface->angle(Vertex::source(connection.source(), *surface));
  CAPTURE(totalAngle);

  SECTION("Saddle Connections have Angle Zero with Itself") {
    REQUIRE(connection.angle(connection) == 0);
  }

  SECTION("Saddle Connections have Angle Zero at Marked Points") {
    const auto other = SaddleConnection<Surface>(*surface, surface->previousAtVertex(connection.source()));

    if (totalAngle == 1) {
      REQUIRE(connection.angle(other) == 0);
    } else {
      REQUIRE(connection.angle(other) > 0);
    }
  }

  SECTION("angle() and ccw() are Consistent") {
    // We check consistency for a sample of saddle connections starting at the
    // same vertex as connection. However, since enumerating saddle connections
    // takes some time, we only do this for some connections, namely the first
    // one we find at each vertex.
    const auto sourceVertex = Vertex::source(connection.source(), *surface);
    if (connection.source() == *sourceVertex.outgoing().begin()) {

      const auto other = GENERATE_REF(saddleConnections(surface, Vertex::source(connection.source(), *surface)));
      CAPTURE(other);

      const auto ccw = connection.ccw(other);
      CAPTURE(ccw);

      const auto angle = connection.angle(other);
      CAPTURE(angle);

      if (ccw == CCW::COLLINEAR) {
        REQUIRE(connection.vector().ccw(other.vector()) == CCW::COLLINEAR);
        REQUIRE((angle == 0 || angle == totalAngle / 2));
        if (angle != 0) {
          if (totalAngle % 2 == 0)
            REQUIRE(connection.vector().orientation(other.vector()) == ORIENTATION::SAME);
          else
            REQUIRE(connection.vector().orientation(other.vector()) == ORIENTATION::OPPOSITE);
        }
      } else if (ccw == CCW::CLOCKWISE) {
        REQUIRE(angle >= totalAngle / 2);
      } else {
        REQUIRE(angle <= totalAngle / 2);
      }
    }
  }
}

TEMPLATE_TEST_CASE("Saddle Connection Constructors", "[SaddleConnection][constructor]", (long long), (mpq_class), (renf_elem_class), (exactreal::Element<exactreal::IntegerRing>), (exactreal::Element<exactreal::NumberField>)) {
  using T = TestType;
  using Surface = FlatTriangulation<T>;

  const auto surface = GENERATE_SURFACES(T);
  CAPTURE(surface);

  const auto connection = GENERATE_REF(saddleConnections(surface));
  CAPTURE(connection);

  SECTION("Saddle Connection from Explicit Chain") {
    REQUIRE(connection == SaddleConnection(*surface, connection.source(), connection.target(), connection.chain()));
  }

  SECTION("Search for Saddle Connection in Sector") {
    SECTION("Saddle Connection with Known Vector") {
      REQUIRE(connection == SaddleConnection<Surface>::inSector(*surface, connection.source(), connection.vector()));
    }

    SECTION("Saddle Connection in Known Direction") {
      REQUIRE(connection == SaddleConnection<Surface>::inSector(*surface, connection.source(), Vertical<Surface>(*surface, connection.vector())));
    }

    SECTION("Deprecated Saddle Connection Constructions") {
      const auto halfPlane = surface->fromHalfEdge(connection.source()).orientation(connection) == ORIENTATION::SAME ? connection.source() : surface->nextAtVertex(connection.source());
      REQUIRE(connection == SaddleConnection<Surface>::inHalfPlane(*surface, halfPlane, Vertical<Surface>(*surface, connection), connection));
      REQUIRE(connection == SaddleConnection<Surface>::inPlane(*surface, connection.source(), connection));
      REQUIRE(connection == SaddleConnection<Surface>::inPlane(*surface, connection.source(), Vertical<Surface>(*surface, connection)));
      REQUIRE(connection == SaddleConnection<Surface>::inPlane(*surface, connection.source(), connection.target(), connection));
      REQUIRE(connection == SaddleConnection<Surface>::alongVertical(*surface, Vertical<Surface>(*surface, connection), halfPlane));
      REQUIRE(connection == SaddleConnection<Surface>::clockwise(SaddleConnection(*surface, surface->nextAtVertex(connection.source())), connection));
      REQUIRE(connection == SaddleConnection<Surface>::counterclockwise(*surface, connection.source(), connection.target(), connection));
      REQUIRE(connection == SaddleConnection<Surface>::counterclockwise(*surface, connection, Vertical<Surface>(*surface, connection)));
    }
  }

}

TEMPLATE_TEST_CASE("Serialization of a SaddleConnection", "[SaddleConnection][save][load]", (long long), (mpz_class), (mpq_class), (renf_elem_class), (exactreal::Element<exactreal::IntegerRing>), (exactreal::Element<exactreal::RationalField>), (exactreal::Element<exactreal::NumberField>)) {
  using T = TestType;
  using R2 = Vector<T>;
  auto square = makeSquare<R2>();

  const auto saddleConnection = GENERATE_COPY(saddleConnections<T>(square));
  testRoundtrip(saddleConnection);
}

}  // namespace flatsurf::test
