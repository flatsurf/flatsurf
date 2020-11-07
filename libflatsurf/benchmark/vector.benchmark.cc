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
#include <e-antic/renfxx.h>

#include "../flatsurf/bound.hpp"
#include "../flatsurf/vector.hpp"
#include "../test/generators/real_generator.hpp"
#include "../test/surfaces.hpp"

using benchmark::DoNotOptimize;
using benchmark::State;

namespace flatsurf::benchmark {

using namespace flatsurf::test;

template <typename T>
Vector<T> makeVector() {
  if constexpr (hasNumberFieldElements<T>) {
    return make1221<Vector<T>>()->fromHalfEdge(HalfEdge(4));
  } else {
    return Vector<T>(13, 37);
  }
}

template <typename T>
void VectorUnaryMinus(State& state) {
  auto vector = makeVector<T>();

  for (auto _ : state)
    DoNotOptimize(-vector);
}
BENCHMARK_TEMPLATE(VectorUnaryMinus, long long);
BENCHMARK_TEMPLATE(VectorUnaryMinus, mpz_class);
BENCHMARK_TEMPLATE(VectorUnaryMinus, mpq_class);
BENCHMARK_TEMPLATE(VectorUnaryMinus, eantic::renf_elem_class);
BENCHMARK_TEMPLATE(VectorUnaryMinus, exactreal::Element<exactreal::IntegerRing>);
BENCHMARK_TEMPLATE(VectorUnaryMinus, exactreal::Element<exactreal::RationalField>);
BENCHMARK_TEMPLATE(VectorUnaryMinus, exactreal::Element<exactreal::NumberField>);

template <typename T>
void VectorPlus(State& state) {
  auto vector = makeVector<T>();

  for (auto _ : state)
    DoNotOptimize(vector + vector);
}
BENCHMARK_TEMPLATE(VectorPlus, long long);
BENCHMARK_TEMPLATE(VectorPlus, mpz_class);
BENCHMARK_TEMPLATE(VectorPlus, mpq_class);
BENCHMARK_TEMPLATE(VectorPlus, eantic::renf_elem_class);
BENCHMARK_TEMPLATE(VectorPlus, exactreal::Element<exactreal::IntegerRing>);
BENCHMARK_TEMPLATE(VectorPlus, exactreal::Element<exactreal::RationalField>);
BENCHMARK_TEMPLATE(VectorPlus, exactreal::Element<exactreal::NumberField>);

template <typename T>
void VectorMinus(State& state) {
  auto vector = makeVector<T>();

  for (auto _ : state)
    DoNotOptimize(vector - vector);
}
BENCHMARK_TEMPLATE(VectorMinus, long long);
BENCHMARK_TEMPLATE(VectorMinus, mpz_class);
BENCHMARK_TEMPLATE(VectorMinus, mpq_class);
BENCHMARK_TEMPLATE(VectorMinus, eantic::renf_elem_class);
BENCHMARK_TEMPLATE(VectorMinus, exactreal::Element<exactreal::IntegerRing>);
BENCHMARK_TEMPLATE(VectorMinus, exactreal::Element<exactreal::RationalField>);
BENCHMARK_TEMPLATE(VectorMinus, exactreal::Element<exactreal::NumberField>);

template <typename T>
void VectorTimesInt(State& state) {
  auto vector = makeVector<T>();

  for (auto _ : state)
    DoNotOptimize(vector * 3);
}
BENCHMARK_TEMPLATE(VectorTimesInt, long long);
BENCHMARK_TEMPLATE(VectorTimesInt, mpz_class);
BENCHMARK_TEMPLATE(VectorTimesInt, mpq_class);
BENCHMARK_TEMPLATE(VectorTimesInt, eantic::renf_elem_class);
BENCHMARK_TEMPLATE(VectorTimesInt, exactreal::Element<exactreal::IntegerRing>);
BENCHMARK_TEMPLATE(VectorTimesInt, exactreal::Element<exactreal::RationalField>);
BENCHMARK_TEMPLATE(VectorTimesInt, exactreal::Element<exactreal::NumberField>);

template <typename T>
void VectorTimesMpz(State& state) {
  auto vector = makeVector<T>();
  auto c = mpz_class(1337);

  for (auto _ : state)
    DoNotOptimize(vector * c);
}
BENCHMARK_TEMPLATE(VectorTimesMpz, long long);
BENCHMARK_TEMPLATE(VectorTimesMpz, mpz_class);
BENCHMARK_TEMPLATE(VectorTimesMpz, mpq_class);
BENCHMARK_TEMPLATE(VectorTimesMpz, eantic::renf_elem_class);
BENCHMARK_TEMPLATE(VectorTimesMpz, exactreal::Element<exactreal::IntegerRing>);
BENCHMARK_TEMPLATE(VectorTimesMpz, exactreal::Element<exactreal::RationalField>);
BENCHMARK_TEMPLATE(VectorTimesMpz, exactreal::Element<exactreal::NumberField>);

template <typename T>
void VectorDivInt(State& state) {
  auto vector = makeVector<T>();

  for (auto _ : state)
    DoNotOptimize(vector / 3);
}
BENCHMARK_TEMPLATE(VectorDivInt, long long);
BENCHMARK_TEMPLATE(VectorDivInt, mpz_class);
BENCHMARK_TEMPLATE(VectorDivInt, mpq_class);
BENCHMARK_TEMPLATE(VectorDivInt, eantic::renf_elem_class);
BENCHMARK_TEMPLATE(VectorDivInt, exactreal::Element<exactreal::RationalField>);
BENCHMARK_TEMPLATE(VectorDivInt, exactreal::Element<exactreal::NumberField>);

template <typename T>
void VectorDivMpz(State& state) {
  auto vector = makeVector<T>();
  auto c = mpz_class(1337);

  for (auto _ : state)
    DoNotOptimize(vector / c);
}
BENCHMARK_TEMPLATE(VectorDivMpz, long long);
BENCHMARK_TEMPLATE(VectorDivMpz, mpz_class);
BENCHMARK_TEMPLATE(VectorDivMpz, mpq_class);
BENCHMARK_TEMPLATE(VectorDivMpz, eantic::renf_elem_class);
BENCHMARK_TEMPLATE(VectorDivMpz, exactreal::Element<exactreal::RationalField>);
BENCHMARK_TEMPLATE(VectorDivMpz, exactreal::Element<exactreal::NumberField>);

template <typename T>
void VectorPerpendicular(State& state) {
  auto vector = makeVector<T>();

  for (auto _ : state)
    DoNotOptimize(vector.perpendicular());
}
BENCHMARK_TEMPLATE(VectorPerpendicular, long long);
BENCHMARK_TEMPLATE(VectorPerpendicular, mpz_class);
BENCHMARK_TEMPLATE(VectorPerpendicular, mpq_class);
BENCHMARK_TEMPLATE(VectorPerpendicular, eantic::renf_elem_class);
BENCHMARK_TEMPLATE(VectorPerpendicular, exactreal::Element<exactreal::IntegerRing>);
BENCHMARK_TEMPLATE(VectorPerpendicular, exactreal::Element<exactreal::RationalField>);
BENCHMARK_TEMPLATE(VectorPerpendicular, exactreal::Element<exactreal::NumberField>);

template <typename T>
void VectorArb(State& state) {
  auto vector = makeVector<T>();

  for (auto _ : state)
    DoNotOptimize(static_cast<Vector<exactreal::Arb>>(vector));
}
BENCHMARK_TEMPLATE(VectorArb, long long);
BENCHMARK_TEMPLATE(VectorArb, mpz_class);
BENCHMARK_TEMPLATE(VectorArb, mpq_class);
BENCHMARK_TEMPLATE(VectorArb, eantic::renf_elem_class);
BENCHMARK_TEMPLATE(VectorArb, exactreal::Element<exactreal::IntegerRing>);
BENCHMARK_TEMPLATE(VectorArb, exactreal::Element<exactreal::RationalField>);
BENCHMARK_TEMPLATE(VectorArb, exactreal::Element<exactreal::NumberField>);

template <typename T>
void VectorCcw(State& state) {
  auto vector = makeVector<T>();
  auto perp = vector.perpendicular();

  for (auto _ : state)
    DoNotOptimize(vector.ccw(perp));
}
BENCHMARK_TEMPLATE(VectorCcw, long long);
BENCHMARK_TEMPLATE(VectorCcw, mpz_class);
BENCHMARK_TEMPLATE(VectorCcw, mpq_class);
BENCHMARK_TEMPLATE(VectorCcw, eantic::renf_elem_class);
BENCHMARK_TEMPLATE(VectorCcw, exactreal::Element<exactreal::IntegerRing>);
BENCHMARK_TEMPLATE(VectorCcw, exactreal::Element<exactreal::RationalField>);
BENCHMARK_TEMPLATE(VectorCcw, exactreal::Element<exactreal::NumberField>);

template <typename T>
void VectorOrientation(State& state) {
  auto vector = makeVector<T>();

  for (auto _ : state)
    DoNotOptimize(vector.orientation(vector));
}
BENCHMARK_TEMPLATE(VectorOrientation, long long);
BENCHMARK_TEMPLATE(VectorOrientation, mpz_class);
BENCHMARK_TEMPLATE(VectorOrientation, mpq_class);
BENCHMARK_TEMPLATE(VectorOrientation, eantic::renf_elem_class);
BENCHMARK_TEMPLATE(VectorOrientation, exactreal::Element<exactreal::IntegerRing>);
BENCHMARK_TEMPLATE(VectorOrientation, exactreal::Element<exactreal::RationalField>);
BENCHMARK_TEMPLATE(VectorOrientation, exactreal::Element<exactreal::NumberField>);

template <typename T>
void VectorTimesVector(State& state) {
  auto vector = makeVector<T>();

  for (auto _ : state)
    DoNotOptimize(vector * vector);
}
BENCHMARK_TEMPLATE(VectorTimesVector, long long);
BENCHMARK_TEMPLATE(VectorTimesVector, mpz_class);
BENCHMARK_TEMPLATE(VectorTimesVector, mpq_class);
BENCHMARK_TEMPLATE(VectorTimesVector, eantic::renf_elem_class);
BENCHMARK_TEMPLATE(VectorTimesVector, exactreal::Element<exactreal::IntegerRing>);
BENCHMARK_TEMPLATE(VectorTimesVector, exactreal::Element<exactreal::RationalField>);
BENCHMARK_TEMPLATE(VectorTimesVector, exactreal::Element<exactreal::NumberField>);

template <typename T>
void VectorLtBound(State& state) {
  auto vector = makeVector<T>();

  for (auto _ : state)
    DoNotOptimize(vector < Bound(1));
}
BENCHMARK_TEMPLATE(VectorLtBound, long long);
BENCHMARK_TEMPLATE(VectorLtBound, mpz_class);
BENCHMARK_TEMPLATE(VectorLtBound, mpq_class);
BENCHMARK_TEMPLATE(VectorLtBound, eantic::renf_elem_class);
BENCHMARK_TEMPLATE(VectorLtBound, exactreal::Element<exactreal::IntegerRing>);
BENCHMARK_TEMPLATE(VectorLtBound, exactreal::Element<exactreal::RationalField>);
BENCHMARK_TEMPLATE(VectorLtBound, exactreal::Element<exactreal::NumberField>);

template <typename T>
void VectorEqVector(State& state) {
  auto vector = makeVector<T>();

  for (auto _ : state)
    DoNotOptimize(vector == vector);
}
BENCHMARK_TEMPLATE(VectorEqVector, long long);
BENCHMARK_TEMPLATE(VectorEqVector, mpz_class);
BENCHMARK_TEMPLATE(VectorEqVector, mpq_class);
BENCHMARK_TEMPLATE(VectorEqVector, eantic::renf_elem_class);
BENCHMARK_TEMPLATE(VectorEqVector, exactreal::Element<exactreal::IntegerRing>);
BENCHMARK_TEMPLATE(VectorEqVector, exactreal::Element<exactreal::RationalField>);
BENCHMARK_TEMPLATE(VectorEqVector, exactreal::Element<exactreal::NumberField>);

}  // namespace flatsurf::benchmark
