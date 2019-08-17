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

#include <flatsurf/permutation.hpp>
#include <flatsurf/half_edge.hpp>

using std::vector;
using testing::Test;
using testing::Types;
using namespace flatsurf;

namespace flatsurf {
namespace {
TEST(Permutation, Cycles) {
  for (int n = 0; n < 10; n++) {
    auto domain = vector<HalfEdge>();
    for (int i = 1; i <= n; i++) {
      domain.push_back(HalfEdge(i));
      domain.push_back(HalfEdge(-i));
    }
    for (int run = 0; run < 128; run++) {
      auto p = Permutation<HalfEdge>::random(domain);
      EXPECT_EQ(p, Permutation<HalfEdge>(p.cycles()));
    }
  }
}
}  // namespace
}

#include "main.hpp"
