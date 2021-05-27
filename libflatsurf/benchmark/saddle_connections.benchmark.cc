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
#include "../flatsurf/saddle_connections_by_length.hpp"
#include "../flatsurf/saddle_connections_by_length_iterator.hpp"
#include "../flatsurf/saddle_connections_iterator.hpp"
#include "../flatsurf/vector.hpp"
#include "../flatsurf/vertex.hpp"
#include "../test/surfaces.hpp"

using benchmark::DoNotOptimize;
using benchmark::State;

namespace flatsurf::benchmark {
using namespace flatsurf::test;
using std::begin;
using std::end;

// Benchmark how long it takes to enumerate all saddle connections up to length "bound" in a torus.
template <typename R2>
void SaddleConnectionsSquare(State& state) {
  const auto square = makeSquare<R2>();
  const auto bound = Bound(state.range(0), 0);

  for (auto _ : state) {
    const auto connections = SaddleConnections<FlatTriangulation<typename R2::Coordinate>>(*square).bound(bound);
    DoNotOptimize(std::distance(begin(connections), end(connections)));
  }
}
BENCHMARK_TEMPLATE(SaddleConnectionsSquare, Vector<long long>)->Range(1, 64);
BENCHMARK_TEMPLATE(SaddleConnectionsSquare, Vector<mpq_class>)->Range(1, 64);
BENCHMARK_TEMPLATE(SaddleConnectionsSquare, Vector<eantic::renf_elem_class>)->Range(1, 64);
BENCHMARK_TEMPLATE(SaddleConnectionsSquare, Vector<exactreal::Element<exactreal::IntegerRing>>)->Range(1, 64);

// Benchmark how long it takes to enumaret all saddle connections up to length "bound" in the L surface.
template <typename R2>
void SaddleConnectionsL(State& state) {
  const auto L = makeL<R2>();
  const auto bound = Bound(state.range(0), 0);

  for (auto _ : state) {
    auto connections = SaddleConnections<FlatTriangulation<typename R2::Coordinate>>(*L).bound(bound);
    DoNotOptimize(std::distance(begin(connections), end(connections)));
  }
}
BENCHMARK_TEMPLATE(SaddleConnectionsL, Vector<long long>)->Range(1, 64);
BENCHMARK_TEMPLATE(SaddleConnectionsL, Vector<mpq_class>)->Range(1, 64);
BENCHMARK_TEMPLATE(SaddleConnectionsL, Vector<eantic::renf_elem_class>)->Range(1, 64);
BENCHMARK_TEMPLATE(SaddleConnectionsL, Vector<exactreal::Element<exactreal::IntegerRing>>)->Range(1, 64);

// Benchmark how long it takes to get the first few saddle connections in a
// surface ordered by length. (This is crucial in sage-flatsurf in the initial
// process when trying to compute orbit closures.)
template <typename R2>
void SaddleConnectionsByLength(State& state) {
  auto surface = make1234<R2>();
  const auto scale = state.range(0);

  const auto scaled = FlatTriangulation<typename R2::Coordinate>(
      static_cast<const FlatTriangulationCombinatorial&>(*surface).clone(),
      [&](const HalfEdge he) { return surface->fromHalfEdge(he) / static_cast<int>(scale); });

  for (auto _ : state) {
    const auto connections = SaddleConnections<FlatTriangulation<typename R2::Coordinate>>(scaled).byLength();
    auto it = begin(connections);

    for (int i = 0; i < scale; i++)
      DoNotOptimize(++it);
  }
}
BENCHMARK_TEMPLATE(SaddleConnectionsByLength, Vector<eantic::renf_elem_class>)->Arg(1)->Arg(64);
BENCHMARK_TEMPLATE(SaddleConnectionsByLength, Vector<exactreal::Element<exactreal::NumberField>>)->Arg(1)->Arg(64);

// Benchmark how long it takes to get a single random saddle connection in the
// torus that longer that "bound". (This is crucial in sage-flatsurf in the
// later process when trying to compute orbit closures.)
template <typename R2>
void SaddleConnectionsSampleSquare(State& state) {
  const auto square = makeSquare<R2>();
  const auto bound = Bound(state.range(0), 0);

  const auto connections = SaddleConnections<FlatTriangulation<typename R2::Coordinate>>(*square).lowerBound(bound);

  for (auto _ : state) {
    DoNotOptimize(*begin(connections));
  }
}
BENCHMARK_TEMPLATE(SaddleConnectionsSampleSquare, Vector<long long>)->Arg(256)->Arg(65536);
BENCHMARK_TEMPLATE(SaddleConnectionsSampleSquare, Vector<mpq_class>)->Arg(256)->Arg(65536);
BENCHMARK_TEMPLATE(SaddleConnectionsSampleSquare, Vector<eantic::renf_elem_class>)->Arg(256)->Arg(65536);
BENCHMARK_TEMPLATE(SaddleConnectionsSampleSquare, Vector<exactreal::Element<exactreal::IntegerRing>>)->Arg(256)->Arg(65536);

// Benchmark how long it takes to enumerate all saddle connections up to length
// "bound" in an L with an added slit.
template <typename R2>
void SaddleConnectionsLWithSlit(State& state) {
  const auto L = makeL<R2>();
  const auto bound = Bound(state.range(0), 0);
  const R2 vector({mpq_class(1009, 1361), 3});

  HalfEdge e(2);

  const auto LWithSlit = L->insertAt(e, vector).surface().slit(e).surface();

  for (auto _ : state) {
    auto connections = SaddleConnections<FlatTriangulation<typename R2::Coordinate>>(LWithSlit).bound(bound).source(Vertex::source(e, LWithSlit));
    DoNotOptimize(std::distance(begin(connections), end(connections)));
  }
}
BENCHMARK_TEMPLATE(SaddleConnectionsLWithSlit, Vector<mpq_class>)->Range(1, 64);
BENCHMARK_TEMPLATE(SaddleConnectionsLWithSlit, Vector<eantic::renf_elem_class>)->Range(1, 64);

}  // namespace flatsurf::benchmark
