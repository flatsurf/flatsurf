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

#include <flatsurf/half_edge.hpp>
#include <flatsurf/length_along_triangulation.hpp>
#include <flatsurf/vector.hpp>

#include "surfaces.hpp"

using namespace flatsurf;
using std::vector;
using testing::Test;
using testing::Types;

namespace {
TEST(LengthAlongTriangulationTest, Square) {
  using Vector = Vector<long long>;
  using Length = LengthAlongTriangulation<long long>;
  auto square = makeSquare<Vector>();
  auto horizontal = Vector(7, -3);

  auto v10 = Length(&square, &horizontal, HalfEdge(1));
  auto v01 = Length(&square, &horizontal, HalfEdge(-2));
  auto v11 = Length(&square, &horizontal, HalfEdge(3));

  EXPECT_EQ(v10 - v01, v11);
  EXPECT_EQ(3 * v10, 7 * v01);
  EXPECT_EQ(3 * v10 - 7 * v01, Length());

  EXPECT_EQ(v10 / v01, 7 / 3);
  EXPECT_EQ(2 * v10 / v01, 2 * 7 / 3);
  EXPECT_EQ(3 * v10 / v01, 7);
}
}  // namespace

#include "main.hpp"
