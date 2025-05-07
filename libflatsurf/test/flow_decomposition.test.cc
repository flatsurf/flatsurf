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

#include <e-antic/renfxx.h>
#include <gmpxx.h>

#include <exact-real/element.hpp>
#include <exact-real/integer_ring.hpp>
#include <exact-real/number_field.hpp>

#include "../flatsurf/bound.hpp"
#include "../flatsurf/ccw.hpp"
#include "../flatsurf/flow_component.hpp"
#include "../flatsurf/flow_connection.hpp"
#include "../flatsurf/flow_decomposition.hpp"
#include "../flatsurf/flow_triangulation.hpp"
#include "../flatsurf/orientation.hpp"
#include "../flatsurf/saddle_connection.hpp"
#include "../flatsurf/saddle_connections.hpp"
#include "../flatsurf/vector.hpp"
#include "../flatsurf/vertical.hpp"
#include "../src/external/rx-ranges/include/rx/ranges.hpp"
#include "external/catch2/single_include/catch2/catch.hpp"
#include "generators/surface_generator.hpp"
#include "generators/vertical_generator.hpp"
#include "surfaces.hpp"

namespace flatsurf::test {

TEST_CASE("Flow Decompositions Over Number Fields", "[FlowDecomposition]") {
  using T = renf_elem_class;

  SECTION("A Complicated Direction For A Veech Cathedral") {
    const auto surface = makeCathedralVeech<Vector<T>>();
    CAPTURE(*surface);

    auto a = N->gen();

    const auto direction = Vector<T>(a + mpq_class(1, 2), 1);

    auto flowDecomposition = FlowDecomposition<FlatTriangulation<T>>(surface->clone(), direction);
    CAPTURE(flowDecomposition);

    REQUIRE(flowDecomposition.decompose());

    REQUIRE(flowDecomposition.components().size() == 5);
  }
}

TEMPLATE_TEST_CASE("Flow Decomposition", "[FlowDecomposition]", (long long), (mpz_class), (mpq_class), (renf_elem_class), (exactreal::Element<exactreal::IntegerRing>), (exactreal::Element<exactreal::RationalField>), (exactreal::Element<exactreal::NumberField>)) {
  using T = TestType;

  const auto surface = GENERATE_SURFACES(T);
  CAPTURE(surface);

  // The limit 6 here is somewhat low. Unfortunately, these tests take some
  // time already (45 seconds in early 2021.)
  const auto vertical = GENERATE_COPY(verticals<T>(surface, 6));
  CAPTURE(vertical);

  SECTION("The flow decomposition in that direction can be computed") {
    auto flowDecomposition = FlowDecomposition<FlatTriangulation<T>>(surface->clone(), vertical);

    const auto area2 = [](const auto& components) {
      return components | rx::transform([](const auto& component) { return component.area2(); }) | rx::sum();
    };

    CAPTURE(flowDecomposition);
    REQUIRE(area2(flowDecomposition.components()) == surface->area2());

    // Return the sum of the SAF invariants of the components that make up
    // a flow decomposition.
    // Note that the length of the SAF invariants can differ because some
    // components can be rational and therefore their SAF invariant is
    // trivial.
    const auto safSum = [](const auto& components) {
      std::vector<mpq_class> sum;
      for (const auto& component : components) {
        const auto saf = component.safInvariant();
        if (saf.size() == 0)
          continue;

        if (sum.size() == 0) {
          sum = saf;
          continue;
        }

        REQUIRE(sum.size() == saf.size());

        for (int i = 0; i < sum.size(); i++)
          sum[i] += saf[i];
      }

      return sum;
    };

    auto saf = safSum(flowDecomposition.components());

    REQUIRE(flowDecomposition.decompose());

    CAPTURE(flowDecomposition);
    REQUIRE(area2(flowDecomposition.components()) == surface->area2());

    SECTION("The SAF Invariants are Consistent") {
      auto saf2 = safSum(flowDecomposition.components());

      if (saf.size() == 0)
        saf = std::vector<mpq_class>(saf2.size());
      if (saf2.size() == 0)
        saf2 = std::vector<mpq_class>(saf.size());

      REQUIRE_THAT(saf2, Catch::Matchers::Equals(saf));
    }

    SECTION("The Area of the Cylinders is Consistent with Their Width & Height (modulo some scaling)") {
      for (const auto& component : flowDecomposition.components()) {
        if (component.cylinder()) {
          REQUIRE(component.holonomy() == std::vector{component.circumferenceHolonomy()});
          REQUIRE(2 * component.width() * component.height() == component.vertical().vertical() * component.vertical().vertical() * component.area2());
        }
      }
    }

    SECTION("Each of its components can be triangulated") {
      const auto triangulations = flowDecomposition.components() | rx::transform([](const auto& component) { return component.triangulation(); }) | rx::to_vector();
      REQUIRE((triangulations | rx::transform([](const auto& component) { return component.triangulation().area2(); }) | rx::sum()) == surface->area2());
      REQUIRE(flowDecomposition.triangulation().area2() == surface->area2());
    }

    SECTION("Each component's bottom contour goes to the right") {
      for (const auto& component : flowDecomposition.components()) {
        for (const auto& bottom : component.bottom()) {
          CAPTURE(bottom);
          REQUIRE(!bottom.top());
          REQUIRE(vertical.ccw(bottom.saddleConnection()) != CCW::COUNTERCLOCKWISE);
        }
        REQUIRE(vertical.ccw(begin(component.bottom())->saddleConnection()) == CCW::CLOCKWISE);
        REQUIRE(vertical.ccw(rbegin(component.bottom())->saddleConnection()) == CCW::CLOCKWISE);
      }
    }

    SECTION("Each component's right contour goes to the top") {
      for (const auto& component : flowDecomposition.components()) {
        for (const auto& right : component.right()) {
          CAPTURE(right);
          REQUIRE(right.parallel());
          REQUIRE(vertical.ccw(right.saddleConnection()) == CCW::COLLINEAR);
          REQUIRE(vertical.orientation(right.saddleConnection()) == ORIENTATION::SAME);
        }
      }
    }

    SECTION("Each component's top contour goes to the left") {
      for (const auto& component : flowDecomposition.components()) {
        for (const auto& top : component.top()) {
          CAPTURE(top);
          REQUIRE(!top.bottom());
          REQUIRE(vertical.ccw(top.saddleConnection()) != CCW::CLOCKWISE);
        }
        REQUIRE(vertical.ccw(begin(component.top())->saddleConnection()) == CCW::COUNTERCLOCKWISE);
        REQUIRE(vertical.ccw(rbegin(component.top())->saddleConnection()) == CCW::COUNTERCLOCKWISE);
      }
    }

    SECTION("Each component's left contour goes to the bottom") {
      for (const auto& component : flowDecomposition.components()) {
        for (const auto& left : component.left()) {
          CAPTURE(left);
          REQUIRE(left.antiparallel());
          REQUIRE(vertical.ccw(left.saddleConnection()) == CCW::COLLINEAR);
          REQUIRE(vertical.orientation(left.saddleConnection()) == ORIENTATION::OPPOSITE);
        }
      }
    }

    SECTION("Each component's perimeter is made up of bottom + right + top + left") {
      for (const auto& component : flowDecomposition.components()) {
        auto perimeter = component.perimeter();
        for (const auto& bottom : component.bottom()) {
          REQUIRE(*begin(perimeter) == bottom);
          perimeter.pop_front();
        }
        for (const auto& right : component.right()) {
          REQUIRE(*begin(perimeter) == right);
          perimeter.pop_front();
        }
        for (const auto& top : component.top()) {
          REQUIRE(*begin(perimeter) == top);
          perimeter.pop_front();
        }
        for (const auto& left : component.left()) {
          REQUIRE(*begin(perimeter) == left);
          perimeter.pop_front();
        }
        REQUIRE(perimeter.size() == 0);
      }
    }

    SECTION("The Boundary Connections Are Consistent") {
      const bool single = flowDecomposition.components().size() == 1;

      for (const auto& component : flowDecomposition.components()) {
        auto perimeter = component.perimeter();
        for (const auto& connection : perimeter) {
          if (single)
            REQUIRE(!connection.boundary());
          REQUIRE(connection.boundary() == (-connection).boundary());
        }

        if (!single)
          REQUIRE((perimeter | rx::any_of([](const auto& connection) { return connection.boundary(); })));
      }
    }

    SECTION("Each Component knows about the Decomposition that Created it") {
      for (const auto& component : flowDecomposition.components())
        REQUIRE(component.decomposition() == flowDecomposition);

      for (auto& component : flowDecomposition.components())
        REQUIRE(component.decomposition() == flowDecomposition);
    }
  }
}

TEST_CASE("Parabolic", "[FlowDecomposition][parabolic]") {
  SECTION("L with sqrt(3) horizontal") {
    using T = renf_elem_class;
    using R2 = Vector<T>;
    auto surface = makeLParabolicNonParabolic<R2>();
    CAPTURE(*surface);

    const auto direction = Vector<T>(1, 0);
    auto flowDecomposition = FlowDecomposition<FlatTriangulation<T>>(surface->clone(), direction);
    CAPTURE(flowDecomposition);

    REQUIRE(flowDecomposition.decompose());

    REQUIRE(flowDecomposition.completelyPeriodic() == boost::logic::tribool(true));
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

    REQUIRE(flowDecomposition.completelyPeriodic() == boost::logic::tribool(true));
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

    REQUIRE(flowDecomposition.completelyPeriodic() == boost::logic::tribool(true));
    REQUIRE(flowDecomposition.parabolic() == boost::logic::tribool(false));
  }

  SECTION("(2,3,5) triangle in (1, 0)") {
    using T = renf_elem_class;
    using R2 = Vector<T>;
    auto surface = make235<R2>();
    CAPTURE(*surface);
    CAPTURE(surface->area2());

    const auto direction = Vector<T>(1, 0);
    auto flowDecomposition = FlowDecomposition<FlatTriangulation<T>>(surface->clone(), direction);
    CAPTURE(flowDecomposition);

    REQUIRE(flowDecomposition.decompose());

    REQUIRE(flowDecomposition.completelyPeriodic() == boost::logic::tribool(true));
    REQUIRE(flowDecomposition.components().size() == 4);
    REQUIRE(flowDecomposition.parabolic() == boost::logic::tribool(true));
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

    REQUIRE(flowDecomposition.completelyPeriodic() == boost::logic::tribool(true));
    REQUIRE(flowDecomposition.components().size() == 6);
    REQUIRE(flowDecomposition.parabolic() == boost::logic::tribool(false));
  }

  SECTION("(2,3,5) triangle in (2c^2 - 1, -c^3 + 4c)") {
    using T = renf_elem_class;
    using R2 = Vector<T>;
    auto surface = make235<R2>();
    CAPTURE(*surface);

    auto c = Q->gen();
    const auto direction = Vector(2 * c * c - 1, -c * c * c + 4 * c);
    auto flowDecomposition = FlowDecomposition<FlatTriangulation<T>>(surface->clone(), direction);
    CAPTURE(flowDecomposition);

    REQUIRE(flowDecomposition.decompose());

    REQUIRE(flowDecomposition.completelyPeriodic() == boost::logic::tribool(true));
    REQUIRE(flowDecomposition.components().size() == 4);
    REQUIRE(flowDecomposition.parabolic() == boost::logic::tribool(true));
  }

  SECTION("(2,3,5) triangle in (c^2 - 5, c)") {
    using T = renf_elem_class;
    using R2 = Vector<T>;
    auto surface = make235<R2>();
    CAPTURE(*surface);

    auto c = Q->gen();
    const auto direction = Vector<T>(c * c - 5, c);
    auto flowDecomposition = FlowDecomposition<FlatTriangulation<T>>(surface->clone(), direction);
    CAPTURE(flowDecomposition);

    REQUIRE(flowDecomposition.decompose());

    REQUIRE(flowDecomposition.completelyPeriodic() == boost::logic::tribool(true));
    REQUIRE(flowDecomposition.components().size() == 6);
    REQUIRE(flowDecomposition.parabolic() == boost::logic::tribool(false));
  }

  SECTION("(2,3,5) triangle in (-5, -c^3 + 2c)") {
    using T = renf_elem_class;
    using R2 = Vector<T>;
    auto surface = make235<R2>();
    CAPTURE(*surface);

    auto c = Q->gen();
    const auto direction = Vector<T>(-5, -c * c * c + 2 * c);
    auto flowDecomposition = FlowDecomposition<FlatTriangulation<T>>(surface->clone(), direction);
    CAPTURE(flowDecomposition);

    REQUIRE(flowDecomposition.decompose());

    REQUIRE(flowDecomposition.completelyPeriodic() == boost::logic::tribool(true));
    REQUIRE(flowDecomposition.components().size() == 6);
    REQUIRE(flowDecomposition.parabolic() == boost::logic::tribool(false));
  }
}

}  // namespace flatsurf::test
