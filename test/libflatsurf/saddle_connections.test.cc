/**********************************************************************
 *  This file is part of flatsurf.
 *
 *        Copyright (C) 2019 Vincent Delecroix
 *        Copyright (C) 2019 Julian Rüth
 *
 *  Flatsurf is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 of the License, or
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

#include <exact-real/integer_ring_traits.hpp>
#include <flatsurf/bound.hpp>
#include <flatsurf/flat_triangulation.hpp>
#include <flatsurf/half_edge.hpp>
#include <flatsurf/saddle_connection.hpp>
#include <flatsurf/saddle_connections.hpp>
#include <flatsurf/vector_along_triangulation.hpp>
#include <flatsurf/vector_arb.hpp>
#include <flatsurf/vector_eantic.hpp>
#include <flatsurf/vector_exactreal.hpp>
#include <flatsurf/vector_longlong.hpp>

#include "surfaces.hpp"

using std::vector;
using testing::Test;
using testing::Types;
using namespace flatsurf;
using eantic::renf_class;

namespace {
template <class R2>
class SaddleConnectionsTest : public Test {};

using ExactVectors = Types<VectorLongLong, VectorEAntic>;
TYPED_TEST_CASE(SaddleConnectionsTest, ExactVectors);

TYPED_TEST(SaddleConnectionsTest, Trivial) {
  auto square = makeSquare<TypeParam>();
  auto connections = SaddleConnections(square, Bound(0), HalfEdge(1));
  EXPECT_EQ(connections.begin(), connections.end());
}

TYPED_TEST(SaddleConnectionsTest, Square) {
  auto square = makeSquare<TypeParam>();
  auto connections = SaddleConnections(square, Bound(100 * 100), HalfEdge(1));
  EXPECT_EQ(std::distance(connections.begin(), connections.end()), 2386);
  connections = SaddleConnections(square, Bound(100 * 100), HalfEdge(3));
  EXPECT_EQ(std::distance(connections.begin(), connections.end()), 2386);
  connections = SaddleConnections(square, Bound(100 * 100), HalfEdge(2));
  EXPECT_EQ(std::distance(connections.begin(), connections.end()), 2386 * 2);
  connections = SaddleConnections(square, Bound(100 * 100), HalfEdge(-1));
  EXPECT_EQ(std::distance(connections.begin(), connections.end()), 2386);
  connections = SaddleConnections(square, Bound(100 * 100), HalfEdge(-3));
  EXPECT_EQ(std::distance(connections.begin(), connections.end()), 2386);
  connections = SaddleConnections(square, Bound(100 * 100), HalfEdge(-2));
  EXPECT_EQ(std::distance(connections.begin(), connections.end()), 2386 * 2);
}
}  // namespace

#include "main.hpp"