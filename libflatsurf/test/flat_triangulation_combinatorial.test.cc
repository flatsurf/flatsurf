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

#include <e-antic/renfxx_fwd.h>
#include <exact-real/element.hpp>
#include <exact-real/number_field.hpp>

#include <flatsurf/flat_triangulation.hpp>
#include <flatsurf/half_edge.hpp>
#include <flatsurf/saddle_connection.hpp>
#include <flatsurf/saddle_connections.hpp>
#include <flatsurf/vector.hpp>
#include <flatsurf/vector_along_triangulation.hpp>

#include "surfaces.hpp"

using namespace flatsurf;
using eantic::renf_class;
using eantic::renf_elem_class;
using std::vector;
using testing::Test;
using testing::Types;

namespace {
class FlatTriangulationCombinatorialTest : public Test {};

TEST(FlatTriangulationCombinatorialTest, Comparison) {
  auto square = makeSquareCombinatorial();
  EXPECT_EQ(square, *square.clone());

  square = makeSquareWithBoundaryCombinatorial();
  EXPECT_EQ(square, *square.clone());
  EXPECT_NE(square, makeSquareCombinatorial());
}

TEST(FlatTriangulationCombinatorialTest, Insert) {
  auto square = makeSquareCombinatorial();
  auto e = square.halfEdges()[0];
  auto square_ = square.insertAt(e);

  ASSERT_NE(square, *square_);
  ASSERT_EQ(square.vertices().size() + 1, square_->vertices().size());
  ASSERT_EQ(square.halfEdges().size() + 6, square_->halfEdges().size());

  auto a = -square_->nextAtVertex(e);
  ASSERT_NE(a, -square.nextAtVertex(e));

  EXPECT_EQ(square_->nextAtVertex(square_->nextAtVertex(square_->nextAtVertex(a))), a);
}

}  // namespace

#include "main.hpp"
