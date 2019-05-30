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

#include <flatsurf/bound.hpp>
#include <flatsurf/delaunay_triangulation.hpp>
#include <flatsurf/half_edge.hpp>

#include "surfaces.hpp"

using namespace flatsurf;
using eantic::renf_class;
using std::vector;
using testing::Test;
using testing::Types;

namespace {
TEST(DelaunayTest, Square) {
  using T = Element<exactreal::IntegerRingTraits>;
  using Vector = Vector<T>;
  auto square = makeSquare<Vector>();

  for (auto halfEdge : square.halfEdges()) {
    square.flip(halfEdge);
    DelaunayTriangulation<T>::transform(square);
    for (auto edge : square.halfEdges()) {
      EXPECT_TRUE(DelaunayTriangulation<T>::test(square, edge));
      EXPECT_LT(square.fromEdge(edge), Bound(2 * 2));
    }
  }
}
}  // namespace

#include "main.hpp"
