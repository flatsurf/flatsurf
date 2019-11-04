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

#include "catch.hpp"

#include <exact-real/element.hpp>
#include <exact-real/integer_ring.hpp>
#include <exact-real/number_field.hpp>

#include <flatsurf/interval_exchange_transformation.hpp>
#include <flatsurf/saddle_connection.hpp>
#include <flatsurf/saddle_connections.hpp>
#include <flatsurf/vector.hpp>
#include <flatsurf/bound.hpp>

#include "surfaces.hpp"

using eantic::renf_class;
using eantic::renf_elem_class;

namespace flatsurf::test {

using namespace flatsurf;

TEMPLATE_TEST_CASE("Interval Exchange Transformation", "[interval_exchange_transformation]", (long long), (mpq_class), (renf_elem_class), (exactreal::Element<exactreal::IntegerRing>), (exactreal::Element<exactreal::NumberField>)) {
  using T = TestType;
  using R2 = Vector<T>;

  int bound;
  std::shared_ptr<FlatTriangulation<T>> surface;
  if constexpr (std::is_same_v<T, long long> || std::is_same_v<T, mpq_class> || std::is_same_v<T, exactreal::Element<exactreal::IntegerRing>>) {
    surface = GENERATE(makeSquare<R2>(), makeL<R2>());
    bound = 8;
  } else {
    surface = GENERATE(makeSquare<R2>(), makeL<R2>(), make1221<R2>(), makeHexagon<R2>(), makeHeptagonL<R2>(), makeGoldenL<R2>());
    bound = 3;
  }

  GIVEN("The " << *surface << " we can Compute Corresponding IETs") {
    for (auto sc : SaddleConnections<FlatTriangulation<T>>(surface, Bound(bound))) {
      auto iet = IntervalExchangeTransformation<T>(*surface, sc->vector());
    }
  }
}

}
