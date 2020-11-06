/**********************************************************************
 *  This file is part of flatsurf.
 *
 *        Copyright (C) 2019-2020 Vincent Delecroix
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

#include <benchmark/benchmark.h>

#include <exact-real/integer_ring.hpp>

#include "../flatsurf/bound.hpp"
#include "../flatsurf/deformation.hpp"
#include "../flatsurf/flat_triangulation.hpp"
#include "../flatsurf/half_edge.hpp"
#include "../flatsurf/saddle_connection.hpp"
#include "../flatsurf/saddle_connections.hpp"
#include "../flatsurf/saddle_connections_iterator.hpp"
#include "../flatsurf/vector.hpp"
#include "../test/surfaces.hpp"

using benchmark::DoNotOptimize;
using benchmark::State;
using eantic::renf_class;

namespace flatsurf::benchmark {
using namespace flatsurf::test;
using std::begin;
using std::end;

const int LIMIT = 1 << 8;

template <typename R2>
void SaddleConnectionsSquare(State& state) {
  auto square = makeSquare<R2>();
  auto bound = Bound(state.range(0), 0);
  for (auto _ : state) {
    auto connections = SaddleConnections<FlatTriangulation<typename R2::Coordinate>>(*square).bound(bound);
    DoNotOptimize(std::distance(begin(connections), end(connections)));
  }
}
BENCHMARK_TEMPLATE(SaddleConnectionsSquare, Vector<long long>)->Range(1, LIMIT);
BENCHMARK_TEMPLATE(SaddleConnectionsSquare, Vector<mpq_class>)->Range(1, LIMIT);
BENCHMARK_TEMPLATE(SaddleConnectionsSquare, Vector<eantic::renf_elem_class>)->Range(1, LIMIT);
BENCHMARK_TEMPLATE(SaddleConnectionsSquare, Vector<exactreal::Element<exactreal::IntegerRing>>)->Range(1, LIMIT);

template <typename R2>
void SaddleConnectionsSampleSquare(State& state) {
  auto square = makeSquare<R2>();
  auto bound = Bound(state.range(0), 0);

  auto connections = SaddleConnections<FlatTriangulation<typename R2::Coordinate>>(*square).lowerBound(bound);

  for (auto _ : state) {
    DoNotOptimize(*begin(connections));
  }
}
BENCHMARK_TEMPLATE(SaddleConnectionsSampleSquare, Vector<long long>)->Arg(256)->Arg(65536);
BENCHMARK_TEMPLATE(SaddleConnectionsSampleSquare, Vector<mpq_class>)->Arg(256)->Arg(65536);
BENCHMARK_TEMPLATE(SaddleConnectionsSampleSquare, Vector<eantic::renf_elem_class>)->Arg(256)->Arg(65536);
BENCHMARK_TEMPLATE(SaddleConnectionsSampleSquare, Vector<exactreal::Element<exactreal::IntegerRing>>)->Arg(256)->Arg(65536);

template <typename R2>
void SaddleConnectionsL(State& state) {
  auto L = makeL<R2>();
  auto bound = Bound(state.range(0), 0);

  for (auto _ : state) {
    auto connections = SaddleConnections<FlatTriangulation<typename R2::Coordinate>>(*L).bound(bound);
    DoNotOptimize(std::distance(begin(connections), end(connections)));
  }
}
BENCHMARK_TEMPLATE(SaddleConnectionsL, Vector<long long>)->Range(1, LIMIT);
BENCHMARK_TEMPLATE(SaddleConnectionsL, Vector<mpq_class>)->Range(1, LIMIT);
BENCHMARK_TEMPLATE(SaddleConnectionsL, Vector<eantic::renf_elem_class>)->Range(1, LIMIT);
BENCHMARK_TEMPLATE(SaddleConnectionsL, Vector<exactreal::Element<exactreal::IntegerRing>>)->Range(1, LIMIT);

template <typename R2>
void SaddleConnectionsLWithSlot(State& state) {
  auto L = makeL<R2>();
  auto bound = Bound(state.range(0), 0);
  R2 vector({mpq_class(1009, 1361), 3});

  HalfEdge e(2);

  auto LWithSlot = L->insertAt(e, vector).surface().slit(e).surface();

  for (auto _ : state) {
    auto connections = SaddleConnections<FlatTriangulation<typename R2::Coordinate>>(LWithSlot).bound(bound).source(Vertex::source(e, LWithSlot));
    DoNotOptimize(std::distance(begin(connections), end(connections)));
  }
}
BENCHMARK_TEMPLATE(SaddleConnectionsLWithSlot, Vector<mpq_class>)->Range(1, LIMIT);
BENCHMARK_TEMPLATE(SaddleConnectionsLWithSlot, Vector<eantic::renf_elem_class>)->Range(1, LIMIT);

}  // namespace flatsurf::benchmark
