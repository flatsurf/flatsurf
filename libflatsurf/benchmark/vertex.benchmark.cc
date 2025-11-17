/**********************************************************************
 *  This file is part of flatsurf.
 *
 *        Copyright (C) 2019-2020 Vincent Delecroix
 *        Copyright (C) 2019-2022 Julian Rüth
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

#include "../flatsurf/half_edge.hpp"
#include "../flatsurf/vertex.hpp"
#include "../test/surfaces.hpp"

using benchmark::DoNotOptimize;
using benchmark::State;

namespace flatsurf::benchmark {
using namespace flatsurf::test;

void VertexAtTarget(State& state) {
  const auto surface = makeL<Vector<long long>>();
  HalfEdge e(1);

  for (auto _ : state) {
    auto target = Vertex::target(*surface, e)
    DoNotOptimize(target);
  }
}
BENCHMARK(VertexAtTarget);

void VertexAtSource(State& state) {
  const auto surface = makeL<Vector<long long>>();
  HalfEdge e(1);

  for (auto _ : state) {
    auto source = Vertex::source(*surface, e)
    DoNotOptimize(source);
  }
}
BENCHMARK(VertexAtSource);

}  // namespace flatsurf::benchmark
