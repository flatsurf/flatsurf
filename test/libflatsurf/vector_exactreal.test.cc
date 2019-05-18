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

#include <exact-real/integer_ring_traits.hpp>
#include <exact-real/module.hpp>
#include <exact-real/real_number.hpp>
#include <flatsurf/bound.hpp>
#include <flatsurf/vector_exactreal.hpp>

using std::vector;
using testing::Test;
using testing::Types;
using namespace exactreal;
using namespace flatsurf;

TEST(VectorExactRealTest, Comparison) {
  auto m = Module<IntegerRingTraits>::make(
      {RealNumber::rational(1), RealNumber::random()});
  auto vector =
      VectorExactReal<IntegerRingTraits>(3 * m->gen(0), 4 * m->gen(0));

  EXPECT_FALSE(vector > Bound(5 * 5));
  EXPECT_FALSE(vector < Bound(5 * 5));
  EXPECT_TRUE(vector < Bound(6 * 6));
}

#include "main.hpp"
