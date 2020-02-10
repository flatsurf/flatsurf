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

#include <exact-real/integer_ring.hpp>
#include "../flatsurf/chain.hpp"

#include "surfaces.hpp"

using benchmark::DoNotOptimize;
using benchmark::State;
using eantic::renf_class;

namespace flatsurf::benchmark {
using namespace flatsurf::test;

template <class T>
void CreateChain(State& state) {
  using R2 = Vector<T>;
  const auto L = makeL<R2>();

  for (auto _ : state) {
    DoNotOptimize(Chain(L));
  }
}
BENCHMARK_TEMPLATE(CreateChain, long long);

template <class T>
void CopyConstructChain(State& state) {
  using R2 = Vector<T>;
  const auto L = makeL<R2>();
  const auto chain = Chain(L);

  for (auto _ : state) {
    DoNotOptimize(Chain(chain));
  }
}
BENCHMARK_TEMPLATE(CopyConstructChain, long long);

template <class T>
void CopyAssignChain(State& state) {
  using R2 = Vector<T>;
  const auto L = makeL<R2>();
  const auto chain = Chain(L);
  auto target = chain;

  for (auto _ : state) {
    target = chain;
  }
}
BENCHMARK_TEMPLATE(CopyAssignChain, long long);

template <class T>
void MoveChain(State& state) {
  using R2 = Vector<T>;
  const auto L = makeL<R2>();
  auto chain = Chain(L);

  for (auto _ : state) {
    Chain tmp = std::move(chain);
    chain = std::move(tmp);
  }
}
BENCHMARK_TEMPLATE(MoveChain, long long);

}  // namespace flatsurf::benchmark

