/**********************************************************************
 *  This file is part of flatsurf.
 *
 *        Copyright (C) 2019-2020 Vincent Delecroix
 *        Copyright (C) 2019-2020 Julian Rüth
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

#include <boost/lexical_cast.hpp>
#include <exact-real/element.hpp>
#include <exact-real/integer_ring.hpp>
#include <exact-real/number_field.hpp>
#include <unordered_set>

#include "../flatsurf/flow_decomposition.hpp"
#include "external/catch2/single_include/catch2/catch.hpp"
#include "generators/surface_generator.hpp"

using boost::lexical_cast;
using eantic::renf_class;
using eantic::renf_elem_class;

namespace flatsurf::test {

using namespace flatsurf;

TEST_CASE("Parabolic", "[surface]") {
  SECTION("L with sqrt(3) horizontal") {
    using T = renf_elem_class;
    using R2 = Vector<T>;
    auto surface = makeLParabolicNonParabolic<R2>();
    CAPTURE(*surface);

    const auto direction = Vector<T>(1, 0);
    auto flowDecomposition = FlowDecomposition<FlatTriangulation<T>>(surface->clone(), direction);
    CAPTURE(flowDecomposition);

    REQUIRE(flowDecomposition.decompose());

    REQUIRE(flowDecomposition.parabolic() == boost::logic::tribool(true));
  }

  SECTION("L with sqrt(3) vertical") {
    using T = renf_elem_class;
    using R2 = Vector<T>;
    auto surface = makeLParabolicNonParabolic<R2>();
    CAPTURE(*surface);

    const auto direction = Vector<T>(0, 1);
    auto flowDecomposition = FlowDecomposition<FlatTriangulation<T>>(surface->clone(), direction);
    CAPTURE(flowDecomposition);

    REQUIRE(flowDecomposition.decompose());

    REQUIRE(flowDecomposition.parabolic() == boost::logic::tribool(false));
  }

  SECTION("L with sqrt(3) diagonal") {
    using T = renf_elem_class;
    using R2 = Vector<T>;
    auto surface = makeLParabolicNonParabolic<R2>();
    CAPTURE(*surface);

    const auto direction = Vector<T>(1, 1);
    auto flowDecomposition = FlowDecomposition<FlatTriangulation<T>>(surface->clone(), direction);
    CAPTURE(flowDecomposition);

    REQUIRE(flowDecomposition.decompose());

    REQUIRE(flowDecomposition.parabolic() == boost::logic::tribool(false));
  }

  SECTION("(2,3,5) triangle in (0, 1)") {
    using T = renf_elem_class;
    using R2 = Vector<T>;
    auto surface = make235<R2>();
    CAPTURE(*surface);

    const auto direction = Vector<T>(0, 1);
    auto flowDecomposition = FlowDecomposition<FlatTriangulation<T>>(surface->clone(), direction);
    CAPTURE(flowDecomposition);

    REQUIRE(flowDecomposition.decompose());

    REQUIRE(flowDecomposition.parabolic() == boost::logic::tribool(false));
  }

  SECTION("(2,3,5) triangle in (c^2 -5,c)") {
    using T = renf_elem_class;
    using R2 = Vector<T>;
    auto surface = make235<R2>();
    CAPTURE(*surface);

    auto c = P->gen();
    const auto direction = Vector<T>(c*c - 5, c);
    auto flowDecomposition = FlowDecomposition<FlatTriangulation<T>>(surface->clone(), direction);
    CAPTURE(flowDecomposition);

    REQUIRE(flowDecomposition.decompose());

    REQUIRE(flowDecomposition.parabolic() == boost::logic::tribool(false));
  }

  SECTION("(2,3,5) triangle in (-5, -c^3 + 2c") {
    using T = renf_elem_class;
    using R2 = Vector<T>;
    auto surface = make235<R2>();
    CAPTURE(*surface);

    auto c = P->gen();
    const auto direction = Vector<T>(-5, -c*c*c + 2*c);
    auto flowDecomposition = FlowDecomposition<FlatTriangulation<T>>(surface->clone(), direction);
    CAPTURE(flowDecomposition);

    REQUIRE(flowDecomposition.decompose());

    REQUIRE(flowDecomposition.parabolic() == boost::logic::tribool(false));
  }
}

}  // namespace flatsurf::test
