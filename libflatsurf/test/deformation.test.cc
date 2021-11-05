/**********************************************************************
 *  This file is part of flatsurf.
 *
 *        Copyright (C) 2021 Julian Rüth
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

#include <exact-real/element.hpp>

#include "../flatsurf/deformation.hpp"

#include "external/catch2/single_include/catch2/catch.hpp"
#include "generators/surface_generator.hpp"

namespace flatsurf::test {

TEMPLATE_TEST_CASE("Deformations", "[deformation]", (long long), (mpq_class), (renf_elem_class), (exactreal::Element<exactreal::IntegerRing>), (exactreal::Element<exactreal::NumberField>)) {
  using T = TestType;

  const auto [name, surface_] = GENERATE(makeSurface<T>());
  const auto surface = *surface_;
  CAPTURE(*name, *surface);

  SECTION("Deformations can be Copied and Moved") {
    auto trivial = Deformation(*surface);
    auto copy = trivial;
    trivial = copy;

    auto move = std::move(trivial);
    trivial = std::move(move);
  }
}

}
