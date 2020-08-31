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

#include "../flatsurf/flat_triangulation_collapsed.hpp"
#include "../flatsurf/half_edge.hpp"
#include "../flatsurf/vertical.hpp"
#include "../test/surfaces.hpp"

using benchmark::DoNotOptimize;
using benchmark::State;

namespace flatsurf::benchmark {
using namespace flatsurf::test;

template <typename R2>
void FlatTriangulationCollapsedFlipUncollapsed(State& state) {
  using T = typename R2::Coordinate;

  auto collapsedL = FlatTriangulationCollapsed<T>::make(makeL<R2>()->clone(), Vector<T>(1000000007, 1));

  assert(collapsedL->size() == 9);

  auto vertical = collapsedL->vertical();

  for (auto _ : state) {
    for (auto e : collapsedL->halfEdges())
      if (vertical.large(e)) {
        collapsedL->flip(e);
        break;
      }
  }

  assert(collapsedL->size() == 9);
}
BENCHMARK_TEMPLATE(FlatTriangulationCollapsedFlipUncollapsed, Vector<long long>);
BENCHMARK_TEMPLATE(FlatTriangulationCollapsedFlipUncollapsed, Vector<mpq_class>);
BENCHMARK_TEMPLATE(FlatTriangulationCollapsedFlipUncollapsed, Vector<eantic::renf_elem_class>);
BENCHMARK_TEMPLATE(FlatTriangulationCollapsedFlipUncollapsed, Vector<exactreal::Element<exactreal::IntegerRing>>);

template <typename R2>
void FlatTriangulationCollapsedFlipCollapsed(State& state) {
  while (state.KeepRunning()) {
    state.PauseTiming();

    using T = typename R2::Coordinate;

    auto collapsedL = FlatTriangulationCollapsed<T>::make(makeL<R2>()->clone(), Vector<T>(1024, 1));

    auto vertical = collapsedL->vertical();

    assert(collapsedL->size() == 9);

    while (collapsedL->size() == 9) {
      for (auto e : {HalfEdge(7), HalfEdge(8)}) {
        if (vertical.large(e)) {
          collapsedL->flip(e);
          break;
        }
      }
    }

    assert(collapsedL->size() == 6);

    state.ResumeTiming();
    while (state.KeepRunning()) {
      if (collapsedL->size() != 6) break;
      for (auto e : collapsedL->halfEdges()) {
        if (collapsedL->nextInFace(collapsedL->nextInFace(collapsedL->nextInFace(e))) != e) continue;
        if (vertical.large(e)) {
          collapsedL->flip(e);
          break;
        }
      }
    }
  }
}
BENCHMARK_TEMPLATE(FlatTriangulationCollapsedFlipCollapsed, Vector<long long>);
BENCHMARK_TEMPLATE(FlatTriangulationCollapsedFlipCollapsed, Vector<mpq_class>);
BENCHMARK_TEMPLATE(FlatTriangulationCollapsedFlipCollapsed, Vector<eantic::renf_elem_class>);
BENCHMARK_TEMPLATE(FlatTriangulationCollapsedFlipCollapsed, Vector<exactreal::Element<exactreal::IntegerRing>>);

}  // namespace flatsurf::benchmark
