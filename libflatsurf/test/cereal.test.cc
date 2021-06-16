/**********************************************************************
 *  This file is part of flatsurf.
 *
 *        Copyright (C) 2019 Vincent Delecroix
 *        Copyright (C) 2019-2020 Julian Rüth
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

#include "../flatsurf/cereal.hpp"
#include "../flatsurf/vertical.hpp"
#include "cereal.helpers.hpp"
#include "external/catch2/single_include/catch2/catch.hpp"
#include "external/cereal/include/cereal/archives/json.hpp"
#include "generators/saddle_connections_generator.hpp"
#include "surfaces.hpp"

namespace flatsurf::test {

template <typename T>
static void testRoundtrip(const T& x) {
  using cereal::JSONInputArchive;
  using cereal::JSONOutputArchive;

  CAPTURE(printer<T>::toString(x));

  std::stringstream s;

  {
    JSONOutputArchive archive(s);
    archive(cereal::make_nvp("test", x));
  }

  INFO("Serialized to " << s.str());

  auto y = factory<T>::make();

  {
    JSONInputArchive archive(s);
    archive(cereal::make_nvp("test", *y));
  }

  CAPTURE(printer<T>::toString(*y));

  REQUIRE(comparer<T>::eq(x, *y));
}

TEST_CASE("Serialization of a HalfEdge", "[cereal]") {
  testRoundtrip(HalfEdge(1337));
}

TEST_CASE("Serialization of an Edge", "[cereal]") {
  testRoundtrip(Edge(1337));
}

TEST_CASE("Serialization of a Vertex", "[cereal]") {
  auto square = makeSquare<Vector<long long>>();
  testRoundtrip(square->vertices()[0]);
}

TEMPLATE_TEST_CASE("Serialization of a Vector", "[cereal]", (long long), (mpz_class), (mpq_class), (renf_elem_class), (exactreal::Element<exactreal::IntegerRing>), (exactreal::Element<exactreal::RationalField>), (exactreal::Element<exactreal::NumberField>)) {
  using R2 = Vector<TestType>;
  auto square = makeSquare<R2>();

  for (auto halfEdge : square->halfEdges()) {
    testRoundtrip(square->fromHalfEdge(halfEdge));
    testRoundtrip(square->fromHalfEdgeApproximate(HalfEdge(1)));
  }
}

TEST_CASE("Serialization of a FlatTriangulationCombinatorial", "[cereal]") {
  auto square = makeSquareCombinatorial();

  testRoundtrip(*square);
}

TEMPLATE_TEST_CASE("Serialization of a FlatTriangulation", "[cereal]", (long long), (mpz_class), (mpq_class), (renf_elem_class), (exactreal::Element<exactreal::IntegerRing>), (exactreal::Element<exactreal::RationalField>), (exactreal::Element<exactreal::NumberField>)) {
  using R2 = Vector<TestType>;
  auto square = makeSquare<R2>();

  testRoundtrip(*square);
}

TEMPLATE_TEST_CASE("Serialization of a Vertical", "[cereal]", (long long), (mpz_class), (mpq_class), (renf_elem_class), (exactreal::Element<exactreal::IntegerRing>), (exactreal::Element<exactreal::RationalField>), (exactreal::Element<exactreal::NumberField>)) {
  using R2 = Vector<TestType>;
  auto square = makeSquare<R2>();

  testRoundtrip(Vertical(*square, square->fromHalfEdge(HalfEdge(1))));
}

TEMPLATE_TEST_CASE("Serialization of a Chain", "[cereal]", (long long), (mpz_class), (mpq_class), (renf_elem_class), (exactreal::Element<exactreal::IntegerRing>), (exactreal::Element<exactreal::RationalField>), (exactreal::Element<exactreal::NumberField>)) {
  using R2 = Vector<TestType>;
  auto square = makeSquare<R2>();

  testRoundtrip(Chain(*square, HalfEdge(1)));
}

TEMPLATE_TEST_CASE("Serialization of a SaddleConnection", "[cereal]", (long long), (mpz_class), (mpq_class), (renf_elem_class), (exactreal::Element<exactreal::IntegerRing>), (exactreal::Element<exactreal::RationalField>), (exactreal::Element<exactreal::NumberField>)) {
  using R2 = Vector<TestType>;
  auto square = makeSquare<R2>();

  const auto saddleConnection = GENERATE_COPY(saddleConnections<TestType>(square));
  testRoundtrip(saddleConnection);
}

TEST_CASE("Serialization of a Bound", "[cereal]") {
  testRoundtrip(Bound(13, 37));
}

TEST_CASE("Serialization of a Permutation", "[cereal]") {
  Permutation<HalfEdge> p({{HalfEdge(1), HalfEdge(2), HalfEdge(-1)}, {HalfEdge(-2)}});

  testRoundtrip(p);
}

}  // namespace flatsurf::test
