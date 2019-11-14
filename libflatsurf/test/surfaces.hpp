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

#ifndef LIBFLATSURF_TEST_SURFACES_HPP
#define LIBFLATSURF_TEST_SURFACES_HPP

#include <vector>

#include <e-antic/renfxx.h>
#include <exact-real/element.hpp>
#include <exact-real/integer_ring.hpp>
#include <exact-real/module.hpp>
#include <exact-real/number_field.hpp>
#include <exact-real/real_number.hpp>

#include <flatsurf/flat_triangulation.hpp>
#include <flatsurf/vector.hpp>

using namespace flatsurf;
using namespace exactreal;
using eantic::renf_class;
using eantic::renf_elem_class;
using std::vector;

namespace flatsurf::test {
auto K = renf_class::make("x^2 - 3", "x", "1.73 +/- 0.1");
auto L = renf_class::make("x^2 - x - 1", "x", "1.618 +/- 0.1");
auto M = renf_class::make("x^3 - x^2 - 2*x +1", "x", "1.802 +/- 0.1");

auto makeSquareCombinatorial() {
  auto vertices = vector<vector<int>>{{1, 3, 2, -1, -3, -2}};
  return FlatTriangulationCombinatorial(vertices);
}

template <typename R2>
auto makeSquare() {
  auto vectors = vector{R2(1, 0), R2(0, 1), R2(1, 1)};
  return std::make_shared<FlatTriangulation<typename R2::Coordinate>>(makeSquareCombinatorial(), vectors);
}

auto makeSquareWithBoundaryCombinatorial() {
  auto vertices = vector<vector<int>>{{-2, 1, 3, 2}, {4, -1, -3, -4}};
  return FlatTriangulationCombinatorial(vertices, {2, -4});
}

template <typename R2>
auto makeSquareWithBoundary() {
  auto vectors = vector{R2(1, 0), R2(0, 1), R2(1, 1), R2(0, 1)};
  return std::make_shared<FlatTriangulation<typename R2::Coordinate>>(makeSquareWithBoundaryCombinatorial(), vectors);
}

auto makeLCombinatorial() {
  auto vertices = vector<vector<int>>{{1, 2, 3, 4, 5, -3, 6, 7, 8, -6, -2, 9, -4, -5, -9, -1, -7, -8}};
  return FlatTriangulationCombinatorial(vertices);
}

template <typename R2>
auto makeL() {
  auto vectors = {R2(1, 0), R2(1, 1), R2(0, 1), R2(-1, 0), R2(-1, -1), R2(1, 0), R2(1, 1), R2(0, 1), R2(0, -1)};
  return std::make_shared<FlatTriangulation<typename R2::Coordinate>>(makeLCombinatorial(), vectors);
}

template <typename R2>
auto makeGoldenL() {
  vector<R2> vectors;
  auto a = L->gen();
  vectors = vector{R2(1, 0), R2(1, 1), R2(0, 1), R2(1 - a, 0), R2(1 - a, -1), R2(1, 0), R2(1, a - 1), R2(0, a - 1), R2(0, -1)};
  return std::make_shared<FlatTriangulation<typename R2::Coordinate>>(makeLCombinatorial(), vectors);
}

auto makeHexagonCombinatorial() {
  auto vertices = vector<vector<int>>({{1, 3, -4, -5, -3, -2}, {2, -1, -6, 4, 5, 6}});
  return FlatTriangulationCombinatorial(vertices);
}

template <typename R2>
auto makeHexagon() {
  vector<R2> vectors;
  auto x = K->gen();
  vectors = vector{R2(2, 0), R2(1, x), R2(3, x), R2(1, -x), R2(4, 0), R2(3, x)};
  return std::make_shared<FlatTriangulation<typename R2::Coordinate>>(makeHexagonCombinatorial(), vectors);
}

auto makeHeptagonLCombinatorial() {
  auto vertices = vector<vector<int>>{{1, 2, 3, 4, 5, 6, -4, 7, 8, -1, 9, 10, 11, 12, -10, 13, -2, -8, 14, -5, -6, -14, -7, -3, -13, -9, 15, -11, -12, -15}};
  return FlatTriangulationCombinatorial(vertices);
}

template <typename R2>
auto makeHeptagonL() {
  vector<R2> vectors;
  auto a = M->gen();
  auto b = a * a - a - 1;
  vectors = vector{R2(1, 0), R2(1, 1), R2(0, 1), R2(1 - a, 0), R2(1 - a, -b), R2(0, -b), R2(a - 1, 1), R2(0, 1), R2(-1, 1 - a), R2(0, 1 - a), R2(b, 0), R2(b, a - 1), R2(-1, 0), R2(a - 1, 0), R2(0, a - 1)};
  return std::make_shared<FlatTriangulation<typename R2::Coordinate>>(makeHeptagonLCombinatorial(), vectors);
}

auto make1221Combinatorial() {
  auto vertices = vector<vector<int>>({{-12, 4, -6, -1, -8, 6, -5, 3, -10, 5, -4, 2}, {-11, 7, 1, 8, -7, 9, -3, 10, -9, 11, -2, 12}});
  return FlatTriangulationCombinatorial(vertices);
}

template <typename R2>
auto make1221() {
  vector<R2> vectors;
  auto x = -K->gen();
  auto o = renf_elem_class(K, 1);
  using R = eantic::renf_elem_class;
  if constexpr (std::is_same_v<R2, Vector<eantic::renf_elem_class>>) {
    auto random = R(K, 200114) / 100000;
    vectors = vector{R2(1, 0), R2((R(K, 1) / 2) * 1, (R(K, 1) / 2 * x) * 1), R2((R(K, 1) / 2) * 1, (R(K, -1) / 2 * x) * 1), R2((R(K, 3) / 2) * random, (R(K, -1) / 2 * x) * random), R2((R(K, -3) / 2) * random, (R(K, -1) / 2 * x) * random), R2(0, (x)*random), R2(0, (x)*random), R2(1, (-x) * random), R2((R(K, -3) / 2) * random, (R(K, -1) / 2 * x) * random), R2((R(K, -1) / 2) * 1 + (R(K, 3) / 2) * random, (R(K, 1) / 2 * x) * 1 + (R(K, 1) / 2 * x) * random), R2((R(K, 3) / 2) * random, (R(K, -1) / 2 * x) * random), R2((R(K, -1) / 2) * 1 + (R(K, -3) / 2) * random, (R(K, -1) / 2 * x) * 1 + (R(K, 1) / 2 * x) * random)};
  } else if constexpr (std::is_same_v<R2, Vector<Element<NumberField>>>) {
    auto module = Module<NumberField>::make({RealNumber::rational(1), RealNumber::random(2.00114)}, K);
    auto g = module->gen(0);
    auto random = module->gen(1);
    vectors = vector{R2(g, 0 * g), R2((R(K, 1) / 2) * g, (R(K, 1) / 2 * x) * g), R2((R(K, 1) / 2) * g, (R(K, -1) / 2 * x) * g), R2((R(K, 3) / 2) * random, (R(K, -1) / 2 * x) * random), R2((R(K, -3) / 2) * random, (R(K, -1) / 2 * x) * random), R2(0 * g, (x)*random), R2(0 * g, (x)*random), R2(g, (-x) * random), R2((R(K, -3) / 2) * random, (R(K, -1) / 2 * x) * random), R2((R(K, -1) / 2) * g + (R(K, 3) / 2) * random, (R(K, 1) / 2 * x) * g + (R(K, 1) / 2 * x) * random), R2((R(K, 3) / 2) * random, (R(K, -1) / 2 * x) * random), R2((R(K, -1) / 2) * g + (R(K, -3) / 2) * random, (R(K, -1) / 2 * x) * g + (R(K, 1) / 2 * x) * random)};
  } else {
    throw std::logic_error("not implemented: make1221()");
  }
  return std::make_shared<FlatTriangulation<typename R2::Coordinate>>(make1221Combinatorial(), vectors);
}

}  // namespace flatsurf::test

#endif
