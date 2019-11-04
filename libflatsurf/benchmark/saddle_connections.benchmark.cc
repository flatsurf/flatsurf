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

#include <benchmark/benchmark.h>
#include <memory>

#include <exact-real/integer_ring.hpp>
#include <flatsurf/flat_triangulation.hpp>
#include <flatsurf/half_edge.hpp>
#include <flatsurf/saddle_connection.hpp>
#include <flatsurf/saddle_connections.hpp>
#include <flatsurf/vector.hpp>
#include <flatsurf/vector_along_triangulation.hpp>
#include <intervalxt/length.hpp>

#include "surfaces.hpp"

using eantic::renf_class;
using benchmark::State;
using benchmark::DoNotOptimize;

namespace flatsurf::benchmark {
using namespace flatsurf::test;

template <class R2>
void SaddleConnectionsSquare(State& state) {
  auto square = makeSquare<R2>();
  auto bound = Bound(state.range(0));
  for (auto _ : state) {
    auto connections = SaddleConnections(square, bound);
    DoNotOptimize(std::distance(connections.begin(), connections.end()));
  }
}
BENCHMARK_TEMPLATE(SaddleConnectionsSquare, Vector<long long>)->Range(1, 1<<8);
BENCHMARK_TEMPLATE(SaddleConnectionsSquare, Vector<mpq_class>)->Range(1, 1<<8);
BENCHMARK_TEMPLATE(SaddleConnectionsSquare, Vector<eantic::renf_elem_class>)->Range(1, 1<<8);
BENCHMARK_TEMPLATE(SaddleConnectionsSquare, Vector<exactreal::Element<exactreal::IntegerRing>>)->Range(1, 1<<8);

template <class R2>
void SaddleConnectionsL(State& state) {
  auto L = makeL<R2>();
  auto bound = Bound(state.range(0));

  HalfEdge e(1);

  for (auto _ : state) {
    auto connections = SaddleConnections(L, bound, e);
    DoNotOptimize(std::distance(connections.begin(), connections.end()));
  }
}
BENCHMARK_TEMPLATE(SaddleConnectionsL, Vector<long long>)->Range(1, 1<<8);
BENCHMARK_TEMPLATE(SaddleConnectionsL, Vector<mpq_class>)->Range(1, 1<<8);
BENCHMARK_TEMPLATE(SaddleConnectionsL, Vector<eantic::renf_elem_class>)->Range(1, 1<<8);
BENCHMARK_TEMPLATE(SaddleConnectionsL, Vector<exactreal::Element<exactreal::IntegerRing>>)->Range(1, 1<<8);

template <class R2>
void SaddleConnectionsLWithSlot(State& state) {
  auto L = makeL<R2>();

  auto bound = Bound(state.range(0));
  R2 vector({mpq_class(1009, 1361), state.range(1)});

  HalfEdge e(2);

  auto LWithSlot = std::shared_ptr<const FlatTriangulation<typename R2::Coordinate>>(L->insertAt(e, vector)->slot(e));

  for (auto _ : state) {
    auto connections = SaddleConnections(LWithSlot, bound, Vertex::source(e, *LWithSlot));
    DoNotOptimize(std::distance(connections.begin(), connections.end()));
  }
}
BENCHMARK_TEMPLATE(SaddleConnectionsLWithSlot, Vector<mpq_class>)->Range(1, 1<<8);
BENCHMARK_TEMPLATE(SaddleConnectionsLWithSlot, Vector<eantic::renf_elem_class>)->Range(1, 1<<8);

}  // namespace
