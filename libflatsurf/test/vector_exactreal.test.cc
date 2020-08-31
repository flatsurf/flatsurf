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

#include <e-antic/renfxx.h>

#include <exact-real/element.hpp>
#include <exact-real/module.hpp>
#include <exact-real/number_field.hpp>
#include <exact-real/real_number.hpp>

#include "../flatsurf/bound.hpp"
#include "../flatsurf/vector.hpp"
#include "external/catch2/single_include/catch2/catch.hpp"

namespace flatsurf::test {

TEMPLATE_TEST_CASE("Vector<exactreal::Element<...>>", "[vector_exact_real]", (exactreal::IntegerRing), (exactreal::RationalField), (exactreal::NumberField)) {
  using exactreal::Element;
  using exactreal::Module;
  using exactreal::RealNumber;

  using V = Vector<Element<TestType>>;

  auto m = Module<TestType>::make({RealNumber::rational(1), RealNumber::random()});

  auto v = V(3, 4);

  REQUIRE(!(v < Bound(5, 0)));
  REQUIRE(!(v > Bound(5, 0)));
  REQUIRE(v < Bound(6, 0));
}

}  // namespace flatsurf::test
