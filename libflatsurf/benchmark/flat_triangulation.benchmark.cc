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

#include "../flatsurf/half_edge.hpp"
#include "../flatsurf/vertical.hpp"
#include "../test/surfaces.hpp"

using benchmark::DoNotOptimize;
using benchmark::State;

namespace flatsurf::benchmark {
using namespace flatsurf::test;

template <typename R2>
void FlatTriangulationFlip(State& state) {
  auto L = makeL<R2>();

  auto vertical = Vertical(*L, R2(1000000007, 1));

  for (auto _ : state) {
    for (auto e : L->halfEdges())
      if (vertical.large(e)) {
        L->flip(e);
        break;
      }
  }
}
BENCHMARK_TEMPLATE(FlatTriangulationFlip, Vector<long long>);
BENCHMARK_TEMPLATE(FlatTriangulationFlip, Vector<mpq_class>);
BENCHMARK_TEMPLATE(FlatTriangulationFlip, Vector<eantic::renf_elem_class>);
BENCHMARK_TEMPLATE(FlatTriangulationFlip, Vector<exactreal::Element<exactreal::IntegerRing>>);

}  // namespace flatsurf::benchmark
