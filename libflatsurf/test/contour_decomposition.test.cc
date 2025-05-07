/**********************************************************************
 *  This file is part of flatsurf.
 *
 *        Copyright (C) 2019-2020 Vincent Delecroix
 *        Copyright (C) 2019-2025 Julian Rüth
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
#include <intervalxt/interval_exchange_transformation.hpp>

#include "../flatsurf/contour_decomposition.hpp"
#include "../flatsurf/vector.hpp"
#include "external/catch2/single_include/catch2/catch.hpp"
#include "generators/surface_generator.hpp"

namespace flatsurf::test {

using namespace flatsurf;

TEST_CASE("Perimeter of Contour Decomposition", "[ContourDecomposition][ostream]") {
  SECTION("A Trivial Case") {
    using T = long long;
    using R2 = Vector<T>;
    auto surface = makeSquare<R2>();
    CAPTURE(*surface);

    auto decomposition = ContourDecomposition<FlatTriangulation<T>>(surface->clone(), {1, 1});

    CAPTURE(decomposition);
    REQUIRE(boost::lexical_cast<std::string>(decomposition) == "[[-2 → 3 → 2 → -3]]");
  }

  SECTION("A Less Trivial Case") {
    using T = long long;
    using R2 = Vector<T>;
    auto surface = makeL<R2>();
    CAPTURE(*surface);

    auto decomposition = ContourDecomposition<FlatTriangulation<T>>(surface->clone(), {2, 1});

    CAPTURE(decomposition);
    REQUIRE(boost::lexical_cast<std::string>(decomposition) == "[[-2 → (2, 1) from 1 to 4 → 2 → (-2, -1) from -6 to -4], [5 → (2, 1) from -4 to -6 → (2, 1) from 6 to -1 → -5 → (-2, -1) from 4 to 1 → (-2, -1) from -1 to 6]]");
  }

  SECTION("A Complicated Surface With Some Collapsed Edges") {
    using T = eantic::renf_elem_class;
    using R2 = Vector<T>;
    auto surface = make125<R2>();
    CAPTURE(*surface);

    auto decomposition = ContourDecomposition<FlatTriangulation<T>>(surface->clone(), {static_cast<R2>(-surface->fromHalfEdge(5)).x() + 3, static_cast<R2>(surface->fromHalfEdge(5)).x()});

    CAPTURE(decomposition);
    REQUIRE(boost::lexical_cast<std::string>(decomposition) == "[[((-1/2*x-1 ~ -1.7071068), (1/2*x-1 ~ -0.29289322)) from -1 to -11 → ((-3*x+1 ~ -3.2426407), (-x+1 ~ -0.41421356)) from -4 to -20 → -20 → 15 → ((2*x+1 ~ 3.8284271), (x-1 ~ 0.41421356)) from -2 to -4 → ((-x+1 ~ -0.41421356), (-x+1 ~ -0.41421356)) from -15 to 19 → ((1/2*x-2 ~ -1.2928932), (1/2*x-1 ~ -0.29289322)) from 17 to 1 → 1 → 24 → ((-1/2*x+2 ~ 1.2928932), (-1/2*x+1 ~ 0.29289322)) from 1 to 17 → ((3*x-1 ~ 3.2426407), (x-1 ~ 0.41421356)) from -20 to -4 → ((-2*x-1 ~ -3.8284271), (-x+1 ~ -0.41421356)) from -4 to -2 → ((x-1 ~ 0.41421356), (x-1 ~ 0.41421356)) from 19 to -15 → -15 → 20 → ((1/2*x+1 ~ 1.7071068), (-1/2*x+1 ~ 0.29289322)) from -11 to -1 → -1 → -24]]");
  }

  SECTION("A Complicated Case With Many Collapsed Edges") {
    using T = eantic::renf_elem_class;
    using R2 = Vector<T>;
    auto surface = make125<R2>();
    CAPTURE(*surface);

    auto decomposition = ContourDecomposition<FlatTriangulation<T>>(surface->clone(), {static_cast<R2>(surface->fromHalfEdge(5)).x() + 1, static_cast<R2>(surface->fromHalfEdge(5)).x()});

    CAPTURE(decomposition);
    REQUIRE(boost::lexical_cast<std::string>(decomposition) == "[[((-1/2*x-2 ~ -2.7071068), (-1/2*x ~ -0.70710678)) from -1 to -2 → ((x+1 ~ 2.4142136), (x-1 ~ 0.41421356)) from -2 to -4 → ((-x+1 ~ -0.41421356), (-x+1 ~ -0.41421356)) from -15 to 19 → ((1/2*x-2 ~ -1.2928932), (1/2*x-1 ~ -0.29289322)) from 17 to 1 → 22 → ((1/2*x+1 ~ 1.7071068), (-1/2*x+1 ~ 0.29289322)) from -11 to -1 → 24 → ((-1/2*x+2 ~ 1.2928932), (-1/2*x+1 ~ 0.29289322)) from 1 to 17 → ((2*x-1 ~ 1.8284271), (x-1 ~ 0.41421356)) from -20 to -4 → ((-x-1 ~ -2.4142136), (-x+1 ~ -0.41421356)) from -4 to -2 → ((x-1 ~ 0.41421356), (x-1 ~ 0.41421356)) from 19 to -15 → ((1/2*x+2 ~ 2.7071068), (1/2*x ~ 0.70710678)) from -2 to -1 → ((-3/2*x-1 ~ -3.1213203), (-1/2*x ~ -0.70710678)) from -1 to 15 → ((3/2*x+1 ~ 3.1213203), (1/2*x ~ 0.70710678)) from 15 to -1 → ((-1/2*x-1 ~ -1.7071068), (1/2*x-1 ~ -0.29289322)) from -1 to -11 → ((-2*x+1 ~ -1.8284271), (-x+1 ~ -0.41421356)) from -4 to -20 → -22 → -24]]");
  }
}

}  // namespace flatsurf::test
