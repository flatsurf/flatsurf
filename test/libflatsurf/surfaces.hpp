/**********************************************************************
 *  This file is part of flatsurf.
 *
 *        Copyright (C) 2019 Vincent Delecroix
 *        Copyright (C) 2019 Julian Rüth
 *
 *  Flatsurf is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 of the License, or
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

#include <vector>

#include <e-antic/renfxx.h>
#include <exact-real/element.hpp>
#include <exact-real/integer_ring_traits.hpp>
#include <exact-real/module.hpp>
#include <exact-real/number_field_traits.hpp>
#include <exact-real/real_number.hpp>

#include <flatsurf/flat_triangulation.hpp>
#include <flatsurf/vector.hpp>

using namespace flatsurf;
using namespace exactreal;
using eantic::renf_class;
using eantic::renf_elem_class;
using std::vector;

namespace {
auto K = renf_class::make("x^2 - 3", "x", "1.73 +/- 0.1");

template <typename R2>
auto makeSquare() {
  vector<R2> vectors;
  if constexpr (std::is_same_v<R2, Vector<Element<IntegerRingTraits>>>) {
    auto module = Module<IntegerRingTraits>::make({RealNumber::rational(1)});
    vectors = {R2(module->gen(0), Element(module)), R2(Element(module), module->gen(0)), R2(module->gen(0), module->gen(0))};
  } else if constexpr (std::is_same_v<R2, Vector<Element<NumberFieldTraits>>>) {
    auto module = Module<NumberFieldTraits>::make({RealNumber::rational(1)}, K);
    vectors = {R2(module->gen(0), Element(module)), R2(Element(module), module->gen(0)), R2(module->gen(0), module->gen(0))};
  } else {
    vectors = vector{R2(1, 0), R2(0, 1), R2(1, 1)};
  }
  auto vertices = vector<vector<int>>{{1, 3, 2, -1, -3, -2}};
  return FlatTriangulation(vertices, vectors);
}

template <typename R2>
auto makeHexagon() {
  vector<R2> vectors;
  auto x = K->gen();
  if constexpr (std::is_same_v<R2, Vector<eantic::renf_elem_class>>) {
    using R = eantic::renf_elem_class;
    vectors = vector{R2(R(K, 2), R(K, 0)), R2(R(K, 1), x), R2(R(K, 3), x), R2(R(K, 1), -x), R2(R(K, 4), R(K, 0)), R2(R(K, 3), x)};
  } else if constexpr (std::is_same_v<R2, Vector<Element<NumberFieldTraits>>>) {
    auto module = Module<NumberFieldTraits>::make({RealNumber::rational(1)}, K);
    vectors = vector{R2(module->gen(0) * 2, Element(module)), R2(module->gen(0), module->gen(0) * x), R2(module->gen(0) * 3, module->gen(0) * x), R2(module->gen(0), -module->gen(0) * x), R2(module->gen(0) * 4, Element(module)), R2(module->gen(0) * 3, module->gen(0) * x)};
  } else {
    throw std::logic_error("not implemented: makeHexagon()");
  }
  auto vertices = vector<vector<int>>({{1, 3, -4, -5, -3, -2}, {2, -1, -6, 4, 5, 6}});
  return FlatTriangulation(vertices, vectors);
}

template <typename R2>
auto make1221() {
  vector<R2> vectors;
  auto x = -K->gen();
  auto o = renf_elem_class(K, 1);
  using R = eantic::renf_elem_class;
  if constexpr (std::is_same_v<R2, Vector<eantic::renf_elem_class>>) {
    auto random = R(K, 200114) / 100000;
    vectors = vector{R2(1, 0), R2( (R(K, 1)/2)*1, (R(K, 1)/2*x)*1), R2( (R(K, 1)/2)*1, (R(K, -1)/2*x)*1), R2( (R(K, 3)/2)*random, (R(K, -1)/2*x)*random), R2( (R(K, -3)/2)*random, (R(K, -1)/2*x)*random), R2( 0, (x)*random), R2( 0, (x)*random), R2( 1, (-x)*random), R2( (R(K, -3)/2)*random, (R(K, -1)/2*x)*random), R2( (R(K, -1)/2)*1 + (R(K, 3)/2)*random, (R(K, 1)/2*x)*1 + (R(K, 1)/2*x)*random), R2( (R(K, 3)/2)*random, (R(K, -1)/2*x)*random), R2( (R(K, -1)/2)*1 + (R(K, -3)/2)*random, (R(K, -1)/2*x)*1 + (R(K, 1)/2*x)*random)};
  } else if constexpr (std::is_same_v<R2, Vector<Element<NumberFieldTraits>>>) {
    auto module = Module<NumberFieldTraits>::make({RealNumber::rational(1), RealNumber::random(2.00114)}, K);
    auto g = module->gen(0);
    auto random = module->gen(1);
    vectors = vector{R2(g, 0*g), R2( (R(K, 1)/2)*g, (R(K, 1)/2*x)*g), R2( (R(K, 1)/2)*g, (R(K, -1)/2*x)*g), R2( (R(K, 3)/2)*random, (R(K, -1)/2*x)*random), R2( (R(K, -3)/2)*random, (R(K, -1)/2*x)*random), R2( 0*g, (x)*random), R2( 0*g, (x)*random), R2( g, (-x)*random), R2( (R(K, -3)/2)*random, (R(K, -1)/2*x)*random), R2( (R(K, -1)/2)*g + (R(K, 3)/2)*random, (R(K, 1)/2*x)*g + (R(K, 1)/2*x)*random), R2( (R(K, 3)/2)*random, (R(K, -1)/2*x)*random), R2( (R(K, -1)/2)*g + (R(K, -3)/2)*random, (R(K, -1)/2*x)*g + (R(K, 1)/2*x)*random)};
  } else {
    throw std::logic_error("not implemented: make1221()");
  }

  auto vertices = vector<vector<int>>({{-12, 4, -6, -1, -8, 6, -5, 3, -10, 5, -4, 2}, {-11, 7, 1, 8, -7, 9, -3, 10, -9, 11, -2, 12}});
  return FlatTriangulation(vertices, vectors);
}

}  // namespace
