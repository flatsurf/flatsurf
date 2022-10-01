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

#include "../flatsurf/path.hpp"
#include "../flatsurf/saddle_connection.hpp"
#include "../flatsurf/saddle_connections.hpp"
#include "../flatsurf/saddle_connections_iterator.hpp"
#include "../flatsurf/saddle_connections_sample.hpp"
#include "../flatsurf/saddle_connections_sample_iterator.hpp"
#include "external/catch2/single_include/catch2/catch.hpp"
#include "generators/half_edge_generator.hpp"
#include "generators/saddle_connection_generator.hpp"
#include "generators/surface_generator.hpp"
#include "generators/segment_generator.hpp"

namespace flatsurf::test {

using namespace flatsurf;

TEMPLATE_TEST_CASE("Nullhomotopic Paths", "[Path]", (long long), (mpz_class), (mpq_class), (renf_elem_class), (exactreal::Element<exactreal::IntegerRing>), (exactreal::Element<exactreal::RationalField>), (exactreal::Element<exactreal::NumberField>)) {
  SECTION("Trivial Paths can be Tigthened") {
    using T = TestType;
    using Surface = FlatTriangulation<T>;

    REQUIRE(Path<Surface>().tighten() == Path<Surface>());
  }

  using T = TestType;
  using Surface = FlatTriangulation<T>;

  const auto surface = GENERATE_SURFACES(T);
  CAPTURE(surface);

  SECTION("Joining a Saddle Connection and its Negative, is Trivial") {
    const auto connection = GENERATE_REF(saddleConnections(surface));
    CAPTURE(connection);

    const auto path = Path(std::vector{connection, -connection});
    CAPTURE(path);

    REQUIRE(path.tighten() == Path<Surface>());
  }

  SECTION("Joining a Segment and its Negative is Trivial") {
    const auto face = GENERATE_COPY(halfEdges(surface));
    CAPTURE(face);

    const auto segment = GENERATE_COPY(segments(surface, face));

    const auto path = Path(std::vector{segment, -segment});
    CAPTURE(path);

    REQUIRE(path.tighten() == Path<Surface>());
  }

  SECTION("Tigthening a Path Walking around a Face") {
    const auto face = GENERATE_COPY(halfEdges(surface));
    CAPTURE(face);

    const auto path = Path(std::vector{
        SaddleConnection{*surface, face},
        SaddleConnection{*surface, surface->nextInFace(face)},
        SaddleConnection{*surface, surface->previousInFace(face)}});
    CAPTURE(path);

    REQUIRE(path.tighten() == Path<Surface>());
  }
}

TEMPLATE_TEST_CASE("Short Paths", "[Path]", (long long), (mpz_class), (mpq_class), (renf_elem_class), (exactreal::Element<exactreal::IntegerRing>), (exactreal::Element<exactreal::RationalField>), (exactreal::Element<exactreal::NumberField>)) {
  using T = TestType;
  using Surface = FlatTriangulation<T>;

  const auto surface = GENERATE_SURFACES(T);
  CAPTURE(surface);

  SECTION("Tightening a Path Along two Edges of a Face") {
    const auto face = GENERATE_COPY(halfEdges(surface));
    CAPTURE(face);

    const auto path = Path(std::vector{
        SaddleConnection{*surface, face},
        SaddleConnection{*surface, surface->nextInFace(face)},
    });
    CAPTURE(path);

    REQUIRE(path.tighten() == Path<Surface>(-SaddleConnection<Surface>(*surface, surface->previousInFace(face))));
  }

  SECTION("A Single Segment cannot Tighten Further") {
    const auto face = GENERATE_COPY(halfEdges(surface));
    CAPTURE(face);

    const auto segment = GENERATE_COPY(segments(surface, face));
    CAPTURE(segment);

    const auto path = Path(segment);
    CAPTURE(path);

    REQUIRE(path.tighten() == path);
  }
}

TEMPLATE_TEST_CASE("Random Paths", "[Path]", (long long), (mpz_class), (mpq_class), (renf_elem_class), (exactreal::Element<exactreal::IntegerRing>), (exactreal::Element<exactreal::RationalField>), (exactreal::Element<exactreal::NumberField>)) {
  using T = TestType;
  using Surface = FlatTriangulation<T>;

  const auto surface = GENERATE_SURFACES(T);
  CAPTURE(surface);

  SECTION("Tightening a Random Path of Saddle Connections") {
    const auto initial = GENERATE_COPY(take(8, halfEdges(surface)));
    CAPTURE(initial);

    const int length = 8;

    auto last = SaddleConnection{*surface, initial};
    auto path = Path{last};
    for (int i = 1; i < length; i++) {
      const auto connections = SaddleConnections<Surface>(*surface).source(Vertex::source(last.target(), *surface)).sample();
      for (const auto& next : connections) {
        CAPTURE(last, next);
        if ((-last).angle(next) != 0)
          continue;
        last = next;
        path += last;
        break;
      }
    }
    CAPTURE(path);

    const auto tightened = path.tighten();
    REQUIRE(tightened.tighten() == tightened);
    REQUIRE((path + (-path)).tighten() == Path<Surface>());
  }

  SECTION("Tightening a Random Path of Segments") {
    // TODO
  }
}

}  // namespace flatsurf::test
