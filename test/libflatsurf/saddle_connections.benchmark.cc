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

#include <benchmark/benchmark.h>
#include <gtest/gtest.h>
#include <boost/lexical_cast.hpp>

#include <exact-real/integer_ring_traits.hpp>
#include <flatsurf/flat_triangulation.hpp>
#include <flatsurf/half_edge.hpp>
#include <flatsurf/saddle_connection.hpp>
#include <flatsurf/saddle_connections.hpp>
#include <flatsurf/vector.hpp>
#include <flatsurf/vector_along_triangulation.hpp>
#include <intervalxt/length.hpp>

#include "surfaces.hpp"

using std::vector;
using testing::Test;
using testing::Types;
using namespace flatsurf;
using eantic::renf_class;

namespace {
template <class R2>
void SaddleConnectionsSquare(benchmark::State& state) {
  auto square = makeSquare<R2>();
  auto bound = Bound(state.range(0));
  auto expected = state.range(1);
  for (auto _ : state) {
    auto connections = SaddleConnections(&square, bound, HalfEdge(1));
    EXPECT_EQ(std::distance(connections.begin(), connections.end()), expected);
    connections = SaddleConnections(&square, bound, HalfEdge(3));
    EXPECT_EQ(std::distance(connections.begin(), connections.end()), expected);
    connections = SaddleConnections(&square, bound, HalfEdge(2));
    EXPECT_EQ(std::distance(connections.begin(), connections.end()), expected * 2);
    connections = SaddleConnections(&square, bound, HalfEdge(-1));
    EXPECT_EQ(std::distance(connections.begin(), connections.end()), expected);
    connections = SaddleConnections(&square, bound, HalfEdge(-3));
    EXPECT_EQ(std::distance(connections.begin(), connections.end()), expected);
    connections = SaddleConnections(&square, bound, HalfEdge(-2));
    EXPECT_EQ(std::distance(connections.begin(), connections.end()), expected * 2);
  }
}
BENCHMARK_TEMPLATE(SaddleConnectionsSquare, Vector<long long>)->Args({64, 980});
BENCHMARK_TEMPLATE(SaddleConnectionsSquare, Vector<eantic::renf_elem_class>)->Args({64, 980});

}  // namespace

#include "main.hpp"
