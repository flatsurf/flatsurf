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

#include <gtest/gtest.h>
#include <boost/lexical_cast.hpp>

#include <exact-real/element.hpp>
#include <exact-real/number_field.hpp>
#include <intervalxt/length.hpp>

#include <flatsurf/flat_triangulation.hpp>
#include <flatsurf/half_edge.hpp>
#include <flatsurf/saddle_connection.hpp>
#include <flatsurf/saddle_connections.hpp>
#include <flatsurf/vector.hpp>
#include <flatsurf/vector_along_triangulation.hpp>

#include "surfaces.hpp"

using std::vector;
using testing::Test;
using testing::Types;
using namespace flatsurf;
using eantic::renf_class;
using eantic::renf_elem_class;

namespace {
template <class R2>
class SaddleConnectionsTest : public Test {};

using ExactVectors = Types<Vector<long long>, Vector<renf_elem_class>, Vector<exactreal::Element<exactreal::NumberField>>>;
TYPED_TEST_CASE(SaddleConnectionsTest, ExactVectors);

TYPED_TEST(SaddleConnectionsTest, Trivial) {
  auto square = makeSquare<TypeParam>();
  auto connections = SaddleConnections(square, 0, HalfEdge(1));
  EXPECT_EQ(connections.begin(), connections.end());
}

TYPED_TEST(SaddleConnectionsTest, Square) {
  auto square = makeSquare<TypeParam>();
  auto bound = 16;
  // saddle connections correspond to coprime (x, y) coordinates in the search sector
  int expected = 0;
  for (int x = 1; x < bound + 1; x++)
    for (int y = 1; y <= x; y++)
      if (x*x + y*y < bound.squared() && std::gcd(x, y) == 1)
        expected++;

  auto connections = SaddleConnections(square, bound, HalfEdge(1));
  EXPECT_EQ(std::distance(connections.begin(), connections.end()), expected);
  connections = SaddleConnections(square, bound, HalfEdge(3));
  EXPECT_EQ(std::distance(connections.begin(), connections.end()), expected);
  connections = SaddleConnections(square, bound, HalfEdge(2));
  EXPECT_EQ(std::distance(connections.begin(), connections.end()), expected * 2);
  connections = SaddleConnections(square, bound, HalfEdge(-1));
  EXPECT_EQ(std::distance(connections.begin(), connections.end()), expected);
  connections = SaddleConnections(square, bound, HalfEdge(-3));
  EXPECT_EQ(std::distance(connections.begin(), connections.end()), expected);
  connections = SaddleConnections(square, bound, HalfEdge(-2));
  EXPECT_EQ(std::distance(connections.begin(), connections.end()), expected * 2);

  connections = SaddleConnections(square, bound);
  EXPECT_EQ(std::distance(connections.begin(), connections.end()), 480);

  EXPECT_EQ(boost::lexical_cast<std::string>(**connections.begin()), "SaddleConnection(3 -> -1 in direction (0, 1))");

  EXPECT_EQ(**connections.begin(), **connections.begin());
}

TYPED_TEST(SaddleConnectionsTest, SquareWithBoundary) {
  auto square = makeSquareWithBoundary<TypeParam>();
  auto bound = 16;
  auto connections = SaddleConnections(square, bound, HalfEdge(1));
  EXPECT_EQ(std::distance(connections.begin(), connections.end()), 1);
  connections = SaddleConnections(square, bound, HalfEdge(2));
  EXPECT_EQ(std::distance(connections.begin(), connections.end()), 0);
  connections = SaddleConnections(square, bound, HalfEdge(3));
  EXPECT_EQ(std::distance(connections.begin(), connections.end()), bound - 1);
  connections = SaddleConnections(square, bound, HalfEdge(4));
  EXPECT_EQ(std::distance(connections.begin(), connections.end()), bound);
  connections = SaddleConnections(square, bound, HalfEdge(-1));
  EXPECT_EQ(std::distance(connections.begin(), connections.end()), 1);
  connections = SaddleConnections(square, bound, HalfEdge(-2));
  EXPECT_EQ(std::distance(connections.begin(), connections.end()), bound);
  connections = SaddleConnections(square, bound, HalfEdge(-3));
  EXPECT_EQ(std::distance(connections.begin(), connections.end()), bound - 1);
  connections = SaddleConnections(square, bound, HalfEdge(-4));
  EXPECT_EQ(std::distance(connections.begin(), connections.end()), 0);

  connections = SaddleConnections(square, bound);
  EXPECT_EQ(std::distance(connections.begin(), connections.end()), 64);

  EXPECT_EQ(boost::lexical_cast<std::string>(**connections.begin()), "SaddleConnection(3 -> -1 in direction (0, 1))");

  EXPECT_EQ(**connections.begin(), **connections.begin());
}

TYPED_TEST(SaddleConnectionsTest, Hexagon) {
  if constexpr (std::is_same_v<TypeParam, Vector<long long>>) {
    // An regular hexagon can not be constructed with integer coordinates.
    return;
  } else {
    auto hexagon = makeHexagon<TypeParam>();
    auto bound = Bound(16);
    auto connections = SaddleConnections(hexagon, bound, HalfEdge(1));
    EXPECT_EQ(std::distance(connections.begin(), connections.end()), 10);
    connections = SaddleConnections(hexagon, bound, HalfEdge(2));
    EXPECT_EQ(std::distance(connections.begin(), connections.end()), 36);
    connections = SaddleConnections(hexagon, bound, HalfEdge(3));
    EXPECT_EQ(std::distance(connections.begin(), connections.end()), 26);
    connections = SaddleConnections(hexagon, bound, HalfEdge(4));
    EXPECT_EQ(std::distance(connections.begin(), connections.end()), 18);
    connections = SaddleConnections(hexagon, bound, HalfEdge(5));
    EXPECT_EQ(std::distance(connections.begin(), connections.end()), 8);
    connections = SaddleConnections(hexagon, bound, HalfEdge(6));
    EXPECT_EQ(std::distance(connections.begin(), connections.end()), 10);

    connections = SaddleConnections(hexagon, bound);
    EXPECT_EQ(std::distance(connections.begin(), connections.end()), 216);
  }
}
}  // namespace

#include "main.hpp"
