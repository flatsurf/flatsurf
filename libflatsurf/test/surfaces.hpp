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

#include <e-antic/renfxx.h>

#include <exact-real/element.hpp>
#include <exact-real/integer_ring.hpp>
#include <exact-real/module.hpp>
#include <exact-real/number_field.hpp>
#include <exact-real/real_number.hpp>
#include <memory>
#include <vector>

#include "../flatsurf/flat_triangulation.hpp"
#include "../flatsurf/vector.hpp"
#include "generators/real_generator.hpp"

using namespace flatsurf;
using namespace exactreal;
using eantic::renf_class;
using eantic::renf_elem_class;
using std::vector;

namespace flatsurf::test {
static auto K = renf_class::make("x^2 - 3", "x", "1.73 +/- .1");
static auto L = renf_class::make("x^2 - x - 1", "x", "1.618 +/- .1");
static auto M = renf_class::make("x^3 - x^2 - 2*x +1", "x", "1.802 +/- .1");
static auto N = renf_class::make("x^2 - 2", "x", "1.414 +/- .1");
static auto O = renf_class::make("x^4 - 2", "x", "1.189 +/- .1");
static auto P = renf_class::make("x^4 - 5*x^2 + 5", "x", "1.17557050 +/- .1");
static auto Q = renf_class::make("x^4 - 5*x^2 + 5", "x", "1.902 +/- .1");

inline auto makeSquareCombinatorial() {
  auto vertices = vector<vector<int>>{{1, 3, 2, -1, -3, -2}};
  return std::make_shared<FlatTriangulationCombinatorial>(vertices);
}

template <typename R2>
auto makeSquare() {
  auto vectors = vector{R2(1, 0), R2(0, 1), R2(1, 1)};
  return std::make_shared<FlatTriangulation<typename R2::Coordinate>>(std::move(*makeSquareCombinatorial()), vectors);
}

inline auto makeSquareWithBoundaryCombinatorial() {
  auto vertices = vector<vector<int>>{{-2, 1, 3, 2}, {4, -1, -3, -4}};
  return std::make_shared<FlatTriangulationCombinatorial>(vertices, std::vector{2, -4});
}

template <typename R2>
auto makeSquareWithBoundary() {
  auto vectors = vector{R2(1, 0), R2(0, 1), R2(1, 1), R2(0, 1)};
  return std::make_shared<FlatTriangulation<typename R2::Coordinate>>(std::move(*makeSquareWithBoundaryCombinatorial()), vectors);
}

inline auto makeLCombinatorial() {
  auto vertices = vector<vector<int>>{{1, 2, 3, 4, 5, -3, 6, 7, 8, -6, -2, 9, -4, -5, -9, -1, -7, -8}};
  return std::make_shared<FlatTriangulationCombinatorial>(vertices);
}

template <typename R2>
auto makeL() {
  auto vectors = {R2(1, 0), R2(1, 1), R2(0, 1), R2(-1, 0), R2(-1, -1), R2(1, 0), R2(1, 1), R2(0, 1), R2(0, -1)};
  return std::make_shared<FlatTriangulation<typename R2::Coordinate>>(std::move(*makeLCombinatorial()), vectors);
}

template <typename R2>
auto makeGoldenL() {
  vector<R2> vectors;
  auto a = L->gen();
  vectors = vector{R2(1, 0), R2(1, 1), R2(0, 1), R2(1 - a, 0), R2(1 - a, -1), R2(1, 0), R2(1, a - 1), R2(0, a - 1), R2(0, -1)};
  return std::make_shared<FlatTriangulation<typename R2::Coordinate>>(std::move(*makeLCombinatorial()), vectors);
}

inline auto makeMcMullenLCombinatorial() {
  auto vertices = vector<vector<int>>{{1, -3, 8, -7, 3, -2, 4, -6, 2, -1, 9, -8, 7, -9, 5, -4, 6, -5}};
  return std::make_shared<FlatTriangulationCombinatorial>(vertices);
}

template <typename R2>
auto makeMcMullenL3125() {
  vector<R2> vectors;
  vectors = vector{R2(2, 1), R2(-2, 0), R2(0, -1), R2(2, 3), R2(-2, 0), R2(0, -3), R2(5, 1), R2(-5, 0), R2(0, -1)};
  return std::make_shared<FlatTriangulation<typename R2::Coordinate>>(std::move(*makeMcMullenLCombinatorial()), vectors);
}

template <typename R2>
auto makeMcMullenL1114() {
  vector<R2> vectors;
  vectors = vector{R2(1, 1), R2(-1, 0), R2(0, -1), R2(1, 1), R2(-1, 0), R2(0, -1), R2(4, 1), R2(-4, 0), R2(0, -1)};
  return std::make_shared<FlatTriangulation<typename R2::Coordinate>>(std::move(*makeMcMullenLCombinatorial()), vectors);
}

template <typename R2>
auto makeMcMullenL2111() {
  vector<R2> vectors;
  vectors = vector{R2(1, 1), R2(-1, 0), R2(0, -1), R2(1, 2), R2(-1, 0), R2(0, -2), R2(1, 1), R2(-1, 0), R2(0, -1)};
  return std::make_shared<FlatTriangulation<typename R2::Coordinate>>(std::move(*makeMcMullenLCombinatorial()), vectors);
}

template <typename R2>
auto makeLParabolicNonParabolic() {
  vector<R2> vectors;
  auto a = K->gen();
  vectors = vector{R2(1, a), R2(-1, 0), R2(0, -a), R2(1, 1), R2(-1, 0), R2(0, -1), R2(a - 1, a), R2(-a + 1, 0), R2(0, -a)};
  return std::make_shared<FlatTriangulation<typename R2::Coordinate>>(std::move(*makeMcMullenLCombinatorial()), vectors);
}

inline auto makeCathedralCombinatorial() {
  auto vertices = vector<vector<int>>{{1, -3, 25, 22, 4, -6, -21, -23, -25, -26, -27, 10, -12, 8, -7, 12, -11, 13, -15, 26, 3, -2}, {-1, 5, -4, 20, 18, 9, -8, 7, -9, 17, 14, -13, -16, -17, -18, -19, 6, -5, 2}, {-10, 24, 21, 19, -20, -22, 23, -24, 27, 15, -14, 16, 11}};
  return std::make_shared<FlatTriangulationCombinatorial>(vertices);
}

template <typename R2>
auto makeCathedral() {
  vector<R2> vectors;
  vectors = vector{R2(1, 1), R2(-1, 0), R2(0, -1), R2(mpq_class(-1, 2), mpq_class(-3, 2)), R2(0, -1), R2(mpq_class(1, 2), mpq_class(5, 2)), R2(1, 1), R2(-1, 0), R2(0, -1), R2(mpq_class(1, 2), mpq_class(3, 2)), R2(mpq_class(-1, 2), mpq_class(-1, 2)), R2(0, -1), R2(mpq_class(-1, 2), mpq_class(3, 2)), R2(0, -1), R2(mpq_class(1, 2), mpq_class(-1, 2)), R2(-1, 1), R2(-1, 0), R2(-1, -1), R2(-1, -2), R2(0, 1), R2(mpq_class(-1, 2), mpq_class(1, 2)), R2(mpq_class(-1, 2), mpq_class(-1, 2)), R2(mpq_class(-5, 2), mpq_class(1, 2)), R2(2, 0), R2(-3, 0), R2(-3, -1), R2(mpq_class(-5, 2), mpq_class(-3, 2))};
  return std::make_shared<FlatTriangulation<typename R2::Coordinate>>(std::move(*makeCathedralCombinatorial()), vectors);
}

template <typename R2>
auto makeCathedralVeech() {
  vector<R2> vectors;
  auto a = N->gen();
  const auto frac = ::flatsurf::test::frac<renf_elem_class, int>;
  vectors = vector{R2(a + frac(1, 2), 1), R2(-a - frac(1, 2), 0), R2(0, -1), R2(-frac(1, 2), -3 * a + frac(5, 2)), R2(0, -1), R2(frac(1, 2), 3 * a - frac(3, 2)), R2(a + frac(1, 2), 1), R2(-a - frac(1, 2), 0), R2(0, -1), R2(frac(1, 2), frac(3, 2)), R2(-frac(1, 2), -frac(1, 2)), R2(0, -1), R2(-frac(1, 2), 3 * a - frac(5, 2)), R2(0, -3 * a + 3), R2(frac(1, 2), -frac(1, 2)), R2(-1, 3 * a - 3), R2(-1, 0), R2(-1, -1), R2(-1, -3 * a + 2), R2(0, 3 * a - 3), R2(-frac(1, 2), frac(1, 2)), R2(-frac(1, 2), -frac(1, 2)), R2(-2 * a - frac(3, 2), frac(1, 2)), R2(2 * a + 1, 0), R2(-2 * a - 2, 0), R2(-2 * a - 2, -1), R2(-2 * a - frac(3, 2), -frac(3, 2))};
  return std::make_shared<FlatTriangulation<typename R2::Coordinate>>(std::move(*makeCathedralCombinatorial()), vectors);
}

template <typename R2>
auto makeCathedralQuartic() {
  vector<R2> vectors;
  auto a = O->gen();
  const auto frac = ::flatsurf::test::frac<renf_elem_class, int>;
  vectors = vector{R2(a, 1), R2(-a, 0), R2(0, -1), R2(-frac(1, 2), -a * a - frac(3, 2)), R2(0, -1), R2(frac(1, 2), a * a + frac(5, 2)), R2(a, 1), R2(-a, 0), R2(0, -1), R2(frac(1, 2), frac(3, 2)), R2(-frac(1, 2), -frac(1, 2)), R2(0, -1), R2(-frac(1, 2), a * a + frac(3, 2)), R2(0, (-a * a - 1)), R2(frac(1, 2), -frac(1, 2)), R2(-1, a * a + 1), R2(-1, 0), R2(-1, -1), R2(-1, -a * a - 2), R2(0, a * a + 1), R2(-frac(1, 2), frac(1, 2)), R2(-frac(1, 2), -frac(1, 2)), R2(-2 * a - frac(1, 2), frac(1, 2)), R2(2 * a, 0), R2(-2 * a - 1, 0), R2(-2 * a - 1, -1), R2(-2 * a - frac(1, 2), -frac(3, 2))};
  return std::make_shared<FlatTriangulation<typename R2::Coordinate>>(std::move(*makeCathedralCombinatorial()), vectors);
}

inline auto makeMcMullenGenus2Combinatorial() {
  auto vertices = vector<vector<int>>{{1, -3, -6, 9, 8, -7, -9, 4, 2, -1, 3, -2, -5, -8, 7, 5, -4, 6}};
  return std::make_shared<FlatTriangulationCombinatorial>(vertices);
}

template <typename R2>
auto makeMcMullenGenus2() {
  vector<R2> vectors;
  auto a = 2 * N->gen();
  vectors = vector{R2(a, a), R2(-a, 0), R2(0, -a), R2(-a + 1, 2), R2(-1, -2), R2(a, 0), R2(a - 5, -2), R2(-a + 4, 0), R2(-1, -2)};
  return std::make_shared<FlatTriangulation<typename R2::Coordinate>>(std::move(*makeMcMullenGenus2Combinatorial()), vectors);
}

inline auto makeHexagonCombinatorial() {
  auto vertices = vector<vector<int>>({{1, 3, -4, -5, -3, -2}, {2, -1, -6, 4, 5, 6}});
  return std::make_shared<FlatTriangulationCombinatorial>(vertices);
}

template <typename R2>
auto makeHexagon() {
  vector<R2> vectors;
  auto x = K->gen();
  vectors = vector{R2(2, 0), R2(1, x), R2(3, x), R2(1, -x), R2(4, 0), R2(3, x)};
  return std::make_shared<FlatTriangulation<typename R2::Coordinate>>(std::move(*makeHexagonCombinatorial()), vectors);
}

inline auto makeOctagonCombinatorial() {
  auto vertices = vector<vector<int>>({{1, -3, 7, 2, -1, 6, -5, 8, 3, -2, 4, -6, 9, 5, -4, -7, -8, -9}});
  return std::make_shared<FlatTriangulationCombinatorial>(vertices);
}

template <typename R2>
auto makeOctagon() {
  vector<R2> vectors;
  auto a = N->gen();
  vectors = vector{R2(-a / 2, a / 2 + 1), R2(0, -1), R2(a / 2, -a / 2), R2(-a / 2 - 1, -a / 2), R2(1, 0), R2(a / 2, a / 2), R2(-a / 2 - 1, -a / 2 - 1), R2(-1, -a - 1), R2(0, -a - 1)};
  return std::make_shared<FlatTriangulation<typename R2::Coordinate>>(std::move(*makeOctagonCombinatorial()), vectors);
}

inline auto makeHeptagonLCombinatorial() {
  auto vertices = vector<vector<int>>{{1, 2, 3, 4, 5, 6, -4, 7, 8, -1, 9, 10, 11, 12, -10, 13, -2, -8, 14, -5, -6, -14, -7, -3, -13, -9, 15, -11, -12, -15}};
  return std::make_shared<FlatTriangulationCombinatorial>(vertices);
}

template <typename R2>
auto makeHeptagonL() {
  vector<R2> vectors;
  auto a = M->gen();
  auto b = a * a - a - 1;
  vectors = vector{R2(1, 0), R2(1, 1), R2(0, 1), R2(1 - a, 0), R2(1 - a, -b), R2(0, -b), R2(a - 1, 1), R2(0, 1), R2(-1, 1 - a), R2(0, 1 - a), R2(b, 0), R2(b, a - 1), R2(-1, 0), R2(a - 1, 0), R2(0, a - 1)};
  return std::make_shared<FlatTriangulation<typename R2::Coordinate>>(std::move(*makeHeptagonLCombinatorial()), vectors);
}

inline auto make123Combinatorial() {
  auto vertices = vector<vector<int>>{{1, -3, -9, -18, 12, -13, 14, 6, -7, -16, 10, 4}, {-1, -5, -12, 15, 7, 2}, {-2, -6, 8, 3}, {-4, -11, 13, 5}, {-8, -14, 11, -10, 17, 9}, {-15, 18, -17, 16}};
  return std::make_shared<FlatTriangulationCombinatorial>(vertices);
}

template <typename R2>
auto make123() {
  vector<R2> vectors;
  auto a = K->gen();
  vectors = vector{R2(4, 0), R2(-1, 1 * a), R2(-3, -1 * a), R2(3, -1 * a), R2(1, 1 * a), R2(-3, -1 * a), R2(2, 2 * a), R2(-1, 1 * a), R2(-2, -2 * a), R2(2, -2 * a), R2(1, 1 * a), R2(-2, 2 * a), R2(3, -1 * a), R2(-4, 0), R2(2, 0), R2(0, 2 * a), R2(-2, 0), R2(0, -2 * a)};
  return std::make_shared<FlatTriangulation<typename R2::Coordinate>>(std::move(*make123Combinatorial()), vectors);
}

inline auto make125Combinatorial() {
  auto vertices = vector<vector<int>>{{1, -3, -9, -21, 12, -13, 18, -19, -24, 22, 14, 6, -7, -17, 10, 4}, {-1, -5, -12, -23, 24, 16, 7, 2}, {-2, -6, -15, 13, 5, -4, -11, -22, 23, 21, -20, 17, -16, 19, 8, 3}, {-8, -18, 15, -14, 11, -10, 20, 9}};
  return std::make_shared<FlatTriangulationCombinatorial>(vertices);
}

template <typename R2>
inline auto make125() {
  vector<R2> vectors;
  auto a = N->gen();
  auto b = N->one() / 2;
  vectors = vector{R2(1, 0), R2(b * a - 1, -b * a + 1), R2((-b * a), (b * a - 1)), R2((b * a), (b * a - 1)), R2((-b * a + 1), (-b * a + 1)), R2((b * a - 1), (-b * a)), R2(0, 1), R2(0, (a - 1)), R2((-b * a), (-b * a)), R2((b * a), (-b * a)), R2(0, (a - 1)), R2(0, 1), R2((-b * a + 1), (-b * a)), R2((-b * a), (-b * a)), R2((a - 1), 0), R2((-b * a + 1), (-b * a + 1)), R2((b * a - 1), (b * a)), R2((-b * a), (b * a)), R2((b * a), (b * a - 1)), R2((-a + 1), 0), R2((b * a - 1), (-b * a)), R2((-b * a), (b * a - 1)), R2((b * a - 1), (-b * a + 1)), R2(1, 0)};
  return std::make_shared<FlatTriangulation<typename R2::Coordinate>>(std::move(*make125Combinatorial()), vectors);
}

inline auto make235Combinatorial() {
  auto vertices = vector<vector<int>>{{1, -3, -9, -18, -23, -28, 25, 19, 10, 4}, {-1, -5, -12, -22, -25, 29, 24, 17, 9, -8, -14, 11, -10, -20, -26, 27, 23, 15, 7, 2}, {-2, -6, 8, 3}, {-4, -11, 13, 5}, {6, -7, -16, -24, -30, 26, 21, 12, -13, 14}, {-15, 18, -17, 16}, {-19, 22, -21, 20}, {-27, 30, -29, 28}};
  return std::make_shared<FlatTriangulationCombinatorial>(vertices);
}

template <typename R2>
auto make235() {
  vector<R2> vectors;
  auto a = Q->gen();
  const auto frac = ::flatsurf::test::frac<renf_elem_class, int>;
  vectors = vector{R2(1, 0), R2((frac(1, 4) * a * a - frac(5, 4)), (frac(1, 4) * a)), R2((-frac(1, 4) * a * a + frac(1, 4)), (-frac(1, 4) * a)), R2((frac(1, 4) * a * a - frac(1, 4)), (-frac(1, 4) * a)), R2((-frac(1, 4) * a * a + frac(5, 4)), (frac(1, 4) * a)), R2((-frac(1, 4) * a * a + frac(1, 4)), (-frac(1, 4) * a)), R2((frac(1, 2) * a * a - frac(3, 2)), (frac(1, 2) * a)), R2((frac(1, 4) * a * a - frac(5, 4)), (frac(1, 4) * a)), R2((-frac(1, 2) * a * a + frac(3, 2)), (-frac(1, 2) * a)), R2((frac(1, 2) * a * a - frac(3, 2)), (-frac(1, 2) * a)), R2((-frac(1, 4) * a * a + frac(5, 4)), (frac(1, 4) * a)), R2((-frac(1, 2) * a * a + frac(3, 2)), (frac(1, 2) * a)), R2((frac(1, 4) * a * a - frac(1, 4)), (-frac(1, 4) * a)), R2(-1, 0), R2((frac(1, 2) * a * a - frac(5, 4)), (-frac(1, 4) * a * a * a + 1 * a)), R2((-frac(1, 4)), (frac(1, 4) * a * a * a - frac(1, 2) * a)), R2((-frac(1, 2) * a * a + frac(5, 4)), (frac(1, 4) * a * a * a - 1 * a)), R2((frac(1, 4)), (-frac(1, 4) * a * a * a + frac(1, 2) * a)), R2((-frac(1, 4)), (-frac(1, 4) * a * a * a + frac(1, 2) * a)), R2((frac(1, 2) * a * a - frac(5, 4)), (frac(1, 4) * a * a * a - 1 * a)), R2((frac(1, 4)), (frac(1, 4) * a * a * a - frac(1, 2) * a)), R2((-frac(1, 2) * a * a + frac(5, 4)), (-frac(1, 4) * a * a * a + 1 * a)), R2((frac(1, 2) * a * a - 1), (-frac(1, 2) * a * a * a + frac(3, 2) * a)), R2((-frac(1, 2) * a * a + 1), (frac(1, 2) * a * a * a - frac(3, 2) * a)), R2((-frac(1, 2) * a * a + 1), (-frac(1, 2) * a * a * a + frac(3, 2) * a)), R2((frac(1, 2) * a * a - 1), (frac(1, 2) * a * a * a - frac(3, 2) * a)), R2(0, (-frac(1, 2) * a * a * a + frac(3, 2) * a)), R2((frac(1, 2) * a * a - 1), 0), R2(0, (frac(1, 2) * a * a * a - frac(3, 2) * a)), R2((-frac(1, 2) * a * a + 1), 0)};
  return std::make_shared<FlatTriangulation<typename R2::Coordinate>>(std::move(*make235Combinatorial()), vectors);
}

inline auto make1221Combinatorial() {
  auto vertices = vector<vector<int>>({{-12, 4, -6, -1, -8, 6, -5, 3, -10, 5, -4, 2}, {-11, 7, 1, 8, -7, 9, -3, 10, -9, 11, -2, 12}});
  return std::make_shared<FlatTriangulationCombinatorial>(vertices);
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
  return std::make_shared<FlatTriangulation<typename R2::Coordinate>>(std::move(*make1221Combinatorial()), vectors);
}

inline auto make1234Combinatorial() {
  auto vertices = vector<vector<int>>({{1, -3, 10, -12, 22, -24, 28, -30, 16, -18, 7, -9, 19, -21, 25, -27, 13, -15, 4, -6}, {-1, 8, -7, 20, -19, 11, -10, 23, -22, 26, -25, 14, -13, 29, -28, 17, -16, 5, -4, 2}, {-2, 15, -14, 21, -20, 18, -17, 24, -23, 3}, {-5, 30, -29, 27, -26, 12, -11, 9, -8, 6}});
  return std::make_shared<FlatTriangulationCombinatorial>(vertices);
}

template <typename R2>
auto make1234() {
  vector<R2> vectors;
  auto a = P->gen();
  const auto frac = ::flatsurf::test::frac<renf_elem_class, int>;
  vectors = vector{R2(frac(1, 2) * a * a + 4, -frac(1, 2) * a * a * a + frac(3, 2) * a), R2(-a * a - 2, 0), R2(frac(1, 2) * a * a - 2, frac(1, 2) * a * a * a - frac(3, 2) * a), R2(frac(1, 2) * a * a + 4, frac(1, 2) * a * a * a - frac(3, 2) * a), R2(-frac(1, 2) * a * a + 1, -frac(1, 2) * a * a * a + frac(3, 2) * a), R2(-5, 0), R2(3 * a * a - frac(17, 2), -frac(1, 2) * a * a * a - 1 * a), R2(-frac(1, 2) * a * a + 1, frac(1, 2) * a * a * a - frac(3, 2) * a), R2(-frac(5, 2) * a * a + frac(15, 2), frac(5, 2) * a), R2(-3 * a * a + frac(13, 2), -frac(5, 2) * a * a * a + 7 * a), R2(frac(1, 2) * a * a - frac(3, 2), frac(1, 2) * a), R2(frac(5, 2) * a * a - 5, frac(5, 2) * a * a * a - frac(15, 2) * a), R2(-3 * a * a + frac(13, 2), frac(5, 2) * a * a * a - 7 * a), R2(frac(5, 2) * a * a - frac(9, 2), -2 * a * a * a + frac(11, 2) * a), R2(frac(1, 2) * a * a - 2, -frac(1, 2) * a * a * a + frac(3, 2) * a), R2(3 * a * a - frac(17, 2), frac(1, 2) * a * a * a + 1 * a), R2(-2 * a * a + frac(11, 2), -frac(1, 2) * a * a * a - 1 * a), R2(-a * a + 3, 0), R2(2 * a * a - 6, -3 * a), R2(-2 * a * a + frac(11, 2), frac(1, 2) * a * a * a + 1 * a), R2(frac(1, 2), -frac(1, 2) * a * a * a + 2 * a), R2(-frac(5, 2) * a * a + 4, -frac(5, 2) * a * a * a + frac(15, 2) * a), R2(frac(5, 2) * a * a - frac(9, 2), 2 * a * a * a - frac(11, 2) * a), R2(frac(1, 2), frac(1, 2) * a * a * a - 2 * a), R2(-frac(5, 2) * a * a + 4, frac(5, 2) * a * a * a - frac(15, 2) * a), R2(1, 0), R2(frac(5, 2) * a * a - 5, -frac(5, 2) * a * a * a + frac(15, 2) * a), R2(2 * a * a - 6, 3 * a), R2(frac(1, 2) * a * a - frac(3, 2), -frac(1, 2) * a), R2(-frac(5, 2) * a * a + frac(15, 2), -frac(5, 2) * a)};
  return std::make_shared<FlatTriangulation<typename R2::Coordinate>>(std::move(*make1234Combinatorial()), vectors);
}

}  // namespace flatsurf::test

#endif
