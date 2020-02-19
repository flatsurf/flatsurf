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
#include <memory>

#include "../flatsurf/half_edge.hpp"

#include "surfaces.hpp"

using benchmark::DoNotOptimize;
using benchmark::State;

namespace flatsurf::benchmark {
using namespace flatsurf::test;

void FlatTriangulationCombinatorialFlip(State& state) {
  auto L = makeLCombinatorial();

  const int N = static_cast<int>(L->size());

  int idx = 0;

  for (auto _ : state) {
    HalfEdge e = HalfEdge::fromIndex(idx++ % N);
    L->flip(e);
  }
}
BENCHMARK(FlatTriangulationCombinatorialFlip);

}  // namespace flatsurf::benchmark
