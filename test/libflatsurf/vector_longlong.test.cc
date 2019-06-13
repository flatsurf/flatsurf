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

#include <flatsurf/vector.hpp>

using namespace flatsurf;
using testing::Test;
using testing::Types;

TEST(VectorLongLongTest, CCW) {
  using V = Vector<long long>;
  V vertical(2, 3);

  EXPECT_EQ(vertical.ccw(V(1, 2)), CCW::COUNTERCLOCKWISE);
  EXPECT_EQ(vertical.ccw(V(-1, -1)), CCW::COUNTERCLOCKWISE);
  EXPECT_EQ(vertical.ccw(V(0, -1)), CCW::CLOCKWISE);
}

#include "main.hpp"

