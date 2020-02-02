/**********************************************************************
 *  This file is part of flatsurf.
 *
 *        Copyright (C) 2019 Vincent Delecroix
 *        Copyright (C) 2019 Julian Rüth
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

#include <memory>
#include <boost/lexical_cast.hpp>

#include "external/catch2/single_include/catch2/catch.hpp"

// TODO: This is not a good idea as it reports warnings from there as ours as
// it ignores isystem.
#include "external/cereal/include/cereal/archives/json.hpp"

// TODO: Change all includes in the tests to this format so editors do not need
// compile commands to figure out include paths.
#include "../flatsurf/cereal.hpp"

#include "cereal.helpers.hpp"
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

TEST_CASE("Serialization of a Vertex", "[cereal]") {
  // TODO: Enable again
  /*
  auto square = makeSquare<Vector<long long>>();
  testRoundtrip(square->vertices()[0]);
  */
}

TEST_CASE("Serialization of a FlatTriangulationCombinatorial", "[cereal]") {
  auto vertices = std::vector<std::vector<int>>{{1, 3, 2, -1, -3, -2}};
  auto square = std::make_shared<FlatTriangulationCombinatorial>(vertices);
  testRoundtrip(square);
}

TEST_CASE("Serialization of a FlatTriangulation", "[cereal]") {
  testRoundtrip(*makeSquare<Vector<long long>>());
}

TEST_CASE("Serialization of a SaddleConnection", "[cereal]") {
  auto square = makeSquare<Vector<long long>>();
  auto connections = SaddleConnections(square, Bound(3, 0));
  for (auto c : connections)
    testRoundtrip(c);
}

}  // namespace flatsurf::test