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

#include <gtest/gtest.h>
#include <boost/lexical_cast.hpp>

#include <exact-real/element.hpp>
#include <exact-real/number_field_traits.hpp>

#include <flatsurf/vector.hpp>
#include <flatsurf/bound.hpp>
#include <flatsurf/interval_exchange_transformation.hpp>
#include <flatsurf/saddle_connections.hpp>
#include <flatsurf/saddle_connection.hpp>

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

using ExactVectors = Types<Vector<long long>, Vector<renf_elem_class>, Vector<exactreal::Element<exactreal::NumberFieldTraits>>>;
TYPED_TEST_CASE(IntervalExchangeTransformationTest, ExactVectors);

TYPED_TEST(IntervalExchangeTransformationTest, Square) {
  auto square = makeSquare<TypeParam>();

  for (auto sc : SaddleConnections<FlatTriangulation<typename TypeParam::Coordinate>>(square, Bound(4*4))) {
    auto iet = IntervalExchangeTransformation<typename TypeParam::Coordinate>(square, sc->vector());
  }
}

}  // namespace

#include "main.hpp"

