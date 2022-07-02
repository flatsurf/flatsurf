/**********************************************************************
 *  This file is part of flatsurf.
 *
 *        Copyright (C) 2021 Julian Rüth
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
#include "external/catch2/single_include/catch2/catch.hpp"
#include "generators/saddle_connection_generator.hpp"
#include "generators/surface_generator.hpp"

namespace flatsurf::test {

TEMPLATE_TEST_CASE("Angle between Saddle Connections", "[saddle_connection][angle]", (long long), (mpq_class), (renf_elem_class), (exactreal::Element<exactreal::IntegerRing>), (exactreal::Element<exactreal::NumberField>)) {
  using T = TestType;
  using Surface = FlatTriangulation<T>;

  const auto [name, surface_] = GENERATE(makeSurface<T>());
  const auto surface = *surface_;
  CAPTURE(*name, *surface);

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
    const auto other = GENERATE_REF(saddleConnections(surface, Vertex::source(connection.source(), *surface), 8));
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

}  // namespace flatsurf::test
