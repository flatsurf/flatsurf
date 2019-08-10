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

#include <gtest/gtest.h>
#include <boost/lexical_cast.hpp>

#include <exact-real/element.hpp>
#include <exact-real/number_field.hpp>
#include <intervalxt/length.hpp>

#include <flatsurf/interval_exchange_transformation.hpp>
#include <flatsurf/saddle_connection.hpp>
#include <flatsurf/saddle_connections.hpp>
#include <flatsurf/vector.hpp>

#include "surfaces.hpp"

using std::vector;
using testing::Test;
using testing::Types;
using namespace flatsurf;
using eantic::renf_class;
using eantic::renf_elem_class;

namespace {
template <class R2>
class IntervalExchangeTransformationTest : public Test {};

using ExactVectors = Types<Vector<long long>, Vector<renf_elem_class>, Vector<exactreal::Element<exactreal::NumberField>>>;
TYPED_TEST_CASE(IntervalExchangeTransformationTest, ExactVectors);

TYPED_TEST(IntervalExchangeTransformationTest, Square) {
  auto square = makeSquare<TypeParam>();

  for (auto sc : SaddleConnections<FlatTriangulation<typename TypeParam::Coordinate>>(&square, Bound(10))) {
    auto iet = IntervalExchangeTransformation<typename TypeParam::Coordinate>(square, sc->vector());
  }
}

TYPED_TEST(IntervalExchangeTransformationTest, Hexagon) {
  if constexpr (std::is_same_v<TypeParam, Vector<long long>>) {
    ;
  } else {
    auto hexagon = makeHexagon<TypeParam>();

    for (auto sc : SaddleConnections<FlatTriangulation<typename TypeParam::Coordinate>>(&hexagon, Bound(10))) {
      auto iet = IntervalExchangeTransformation<typename TypeParam::Coordinate>(hexagon, sc->vector());
    }
  }
}

TYPED_TEST(IntervalExchangeTransformationTest, HeptagonL) {
  if constexpr (std::is_same_v<TypeParam, Vector<long long>>) {
    ;
  } else if constexpr (std::is_same_v<TypeParam, Vector<Element<NumberField>>>) {
    ;
  } else {
    auto heptagonL = makeHeptagonL<TypeParam>();

    for (auto sc : SaddleConnections<FlatTriangulation<typename TypeParam::Coordinate>>(&heptagonL, Bound(5))) {
      auto iet = IntervalExchangeTransformation<typename TypeParam::Coordinate>(heptagonL, sc->vector());
    }
  }
}

TYPED_TEST(IntervalExchangeTransformationTest, GoldenL) {
  if constexpr (std::is_same_v<TypeParam, Vector<long long>>) {
    ;
  } else if constexpr (std::is_same_v<TypeParam, Vector<Element<NumberField>>>) {
    ;
  } else {
    auto goldenL = makeGoldenL<TypeParam>();

    for (auto sc : SaddleConnections<FlatTriangulation<typename TypeParam::Coordinate>>(&goldenL, Bound(10))) {
      auto iet = IntervalExchangeTransformation<typename TypeParam::Coordinate>(goldenL, sc->vector());
    }
  }
}

TYPED_TEST(IntervalExchangeTransformationTest, _1221) {
  if constexpr (std::is_same_v<TypeParam, Vector<long long>>) {
    ;
  } else {
    auto _1221 = make1221<TypeParam>();

    for (auto sc : SaddleConnections<FlatTriangulation<typename TypeParam::Coordinate>>(&_1221, Bound(5))) {
      auto iet = IntervalExchangeTransformation<typename TypeParam::Coordinate>(_1221, sc->vector());
    }
  }
}

}  // namespace

#include "main.hpp"
