/**********************************************************************
 *  This file is part of flatsurf.
 *
 *        Copyright (C) 2020 Julian Rüth
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
#include "../flatsurf/path.hpp"
#include "../flatsurf/saddle_connection.hpp"

#include "../test/surfaces.hpp"

using benchmark::DoNotOptimize;
using benchmark::State;

namespace flatsurf::benchmark {
using namespace flatsurf::test;

template <typename R2>
void PathConstructor(State& state) {
  using Surface = FlatTriangulation<typename R2::Coordinate>;
  auto L = makeL<R2>();

  using Segment = SaddleConnection<Surface>;
  const auto segments = std::vector{Segment(L, HalfEdge(1)), Segment(L, HalfEdge(-9)), Segment(L, HalfEdge(-2))};

  for (auto _ : state) {
    DoNotOptimize(Path(segments));
  }
}
BENCHMARK_TEMPLATE(PathConstructor, Vector<long long>);
BENCHMARK_TEMPLATE(PathConstructor, Vector<eantic::renf_elem_class>);

}  // namespace flatsurf::benchmark
