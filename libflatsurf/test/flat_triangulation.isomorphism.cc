/**********************************************************************
 *  This file is part of flatsurf.
 *
 *        Copyright (C)      2019 Vincent Delecroix
 *        Copyright (C) 2019-2022 Julian Rüth
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

#include <fmt/format.h>

#include <e-antic/renfxx_fwd.hpp>
#include <exact-real/element.hpp>
#include <exact-real/number_field.hpp>
#include <numeric>
#include <unordered_set>

#include "../flatsurf/ccw.hpp"
#include "../flatsurf/deformation.hpp"
#include "../flatsurf/delaunay.hpp"
#include "../flatsurf/flat_triangulation.hpp"
#include "../flatsurf/fmt.hpp"
#include "../flatsurf/half_edge.hpp"
#include "../flatsurf/edge.hpp"
#include "../flatsurf/half_edge_set.hpp"
#include "../flatsurf/path.hpp"
#include "../flatsurf/path_iterator.hpp"
#include "../flatsurf/interval_exchange_transformation.hpp"
#include "../flatsurf/isomorphism.hpp"
#include "../flatsurf/saddle_connection.hpp"
#include "../flatsurf/saddle_connections.hpp"
#include "../flatsurf/vector.hpp"
#include "../src/external/rx-ranges/include/rx/ranges.hpp"
#include "external/catch2/single_include/catch2/catch.hpp"
#include "generators/half_edge_generator.hpp"
#include "generators/real_generator.hpp"
#include "generators/surface_generator.hpp"
#include "generators/point_generator.hpp"
#include "surfaces.hpp"

namespace flatsurf::test {

TEMPLATE_TEST_CASE("Detect Isomorphic Surfaces", "[flat_triangulation][isomorphism]", (long long), (mpz_class), (mpq_class), (renf_elem_class), (exactreal::Element<exactreal::IntegerRing>), (exactreal::Element<exactreal::RationalField>), (exactreal::Element<exactreal::NumberField>)) {
  using T = TestType;
  using Transformation = std::tuple<T, T, T, T>;

  const auto surface_ = GENERATE_SURFACES(T);
  CAPTURE(surface_);

  auto surface = surface_->clone();

  const int delaunay = GENERATE(values({0, 1}));
  const auto isomorphism = delaunay ? ISOMORPHISM::DELAUNAY_CELLS : ISOMORPHISM::FACES;

  CAPTURE(delaunay);
  if (delaunay)
    surface.delaunay();

  REQUIRE(surface.isomorphism(surface, isomorphism));

  std::vector<Transformation> transformations = {Transformation{1, 0, 0, 1}};

  Transformation candidate;
  auto filter = [&](const T& a, const T& b, const T& c, const T& d) {
    candidate = Transformation{a, b, c, d};
    return std::find(begin(transformations), end(transformations), candidate) == end(transformations);
  };

  while (true) {
    const auto deformation = surface.isomorphism(surface, isomorphism, filter);

    if (!deformation)
      break;

    const auto [a, b, c, d] = candidate;
    CAPTURE(a, b, c, d);

    REQUIRE((!deformation->trivial() || (a == 1 && b == 0 && c == 0 && d == 1)));

    std::unordered_set<HalfEdge> image;
    for (const auto& halfEdge : surface.halfEdges()) {
      CAPTURE(halfEdge);

      if (delaunay && surface.delaunay(halfEdge.edge()) == DELAUNAY::AMBIGUOUS)
        continue;

      auto he = (*deformation)(halfEdge);

      REQUIRE(he.has_value());

      image.insert(*he);

      const auto v = surface.fromHalfEdge(halfEdge);
      const auto v_ = deformation->surface().fromHalfEdge(*he);
      REQUIRE(Vector<T>(v.x() * a + v.y() * b, v.x() * c + v.y() * d) == v_);
    }
    if (!delaunay)
      REQUIRE(image.size() == surface.halfEdges().size());

    transformations.push_back(candidate);

    REQUIRE((*deformation * deformation->section()).trivial());
    REQUIRE((deformation->section() * *deformation).trivial());
  }

  auto scaled = surface.scale(2);
  CAPTURE(scaled);

  REQUIRE(!surface.isomorphism(scaled, isomorphism));
  REQUIRE(surface.isomorphism(scaled, isomorphism, [](const auto&, const auto&, const auto&, const auto&) { return true; }));
}

}  // namespace flatsurf::test

