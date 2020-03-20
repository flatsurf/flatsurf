/**********************************************************************
 *  This file is part of flatsurf.
 *
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

#include <memory>
#include <ostream>
#include <unordered_map>

#include <fmt/format.h>
#include <fmt/ostream.h>

#include <intervalxt/dynamical_decomposition.hpp>
#include <intervalxt/interval_exchange_transformation.hpp>
#include <intervalxt/label.hpp>

#include "external/rx-ranges/include/rx/ranges.hpp"

#include "../flatsurf/ccw.hpp"
#include "../flatsurf/contour_component.hpp"
#include "../flatsurf/contour_connection.hpp"
#include "../flatsurf/contour_decomposition.hpp"
#include "../flatsurf/flow_component.hpp"
#include "../flatsurf/flow_connection.hpp"
#include "../flatsurf/interval_exchange_transformation.hpp"
#include "../flatsurf/orientation.hpp"
#include "../flatsurf/path.hpp"
#include "../flatsurf/path_iterator.hpp"
#include "../flatsurf/saddle_connection.hpp"

#include "impl/flow_component_state.hpp"
#include "impl/flow_decomposition_state.hpp"
#include "impl/interval_exchange_transformation.impl.hpp"

#include "util/assert.ipp"
#include "util/instantiate.ipp"

namespace flatsurf {

template <typename Surface>
FlowDecompositionState<Surface>::FlowDecompositionState(std::unique_ptr<Surface> surface, const Vector<T>& direction) :
  contourDecomposition(std::move(surface), direction) {
  for (auto& contour : contourDecomposition.components()) {
    auto iet = std::make_shared<IntervalExchangeTransformation<FlatTriangulationCollapsed<T>>>(contour.intervalExchangeTransformation());
    auto decomposition = intervalxt::DynamicalDecomposition(iet->intervalExchangeTransformation());
    ASSERT(decomposition.components().size() == 1, "contour component must yield exactly one flow component initially");
    for (auto& component : decomposition.components()) {
      components.push_back(FlowComponentState<Surface>{contour, iet, component});
    }
  }

  // Inject collapsed verticals into intervalxt components.
  // TODO: This is all quite messy...

  std::unordered_map<SaddleConnection<Surface>, std::pair<intervalxt::Label, intervalxt::Label>> injectedConnections;

  std::unordered_map<SaddleConnection<Surface>, bool> isTop;

  using Injection = std::pair<::intervalxt::Label, ::intervalxt::Label>;

  for (auto right : {true, false}) {
    for (auto& component : components) {
      auto& dynamicalComponent = component.dynamicalComponent;
      auto& contourComponent = component.contourComponent;

      const auto inject = [&](const ::intervalxt::HalfEdge& label, const std::vector<Injection>& injectLeft, const std::vector<SaddleConnection<Surface>>& leftVerticals, const std::vector<Injection>& injectRight, const std::vector<SaddleConnection<Surface>>& rightVerticals) {
        assert(injectLeft.size() == leftVerticals.size());
        assert(injectRight.size() == rightVerticals.size());
        const auto [leftInjected, rightInjected] = dynamicalComponent.inject(label, injectLeft, injectRight);
        assert(leftInjected.size() == injectLeft.size());
        assert(rightInjected.size() == injectRight.size());

        for (const auto& [vertical, injected] : rx::zip(leftVerticals, leftInjected)) {
          this->injectedConnections.emplace(injected, vertical);
          ASSERT(vertical.vector().ccw(direction) == CCW::COLLINEAR, "Injected verticals must be collinear with flow direction but " << vertical << " is not.");
          ;
          ASSERT(direction.orientation(vertical) == ORIENTATION::OPPOSITE, "Injected left verticals must be antiparallel with flow direction but " << vertical << " is not.");
        }
        for (const auto& [vertical, injected] : rx::zip(rightVerticals, rightInjected)) {
          this->injectedConnections.emplace(injected, vertical);
          ASSERT(vertical.vector().ccw(direction) == CCW::COLLINEAR, "Injected verticals must be collinear with flow direction but " << vertical << " is not.");
          ;
          ASSERT(direction.orientation(vertical) == ORIENTATION::SAME, "Injected right verticals must be parallel with flow direction but " << vertical << " is not.");
        }
      };

      for (auto top : {true, false}) {
        const auto& labels = top ? dynamicalComponent.topContour() : dynamicalComponent.bottomContour();
        auto connections = top ? contourComponent.topContour() : contourComponent.bottomContour();
        if (top)
          std::reverse(begin(connections), end(connections));
        for (const auto& [label, connection] : rx::zip(labels, connections)) {
          std::vector<Injection> injectHere;
          std::vector<SaddleConnection<Surface>> verticals;

          if (right) {
            // First we inject connections on the right which define how
            // connections are named by attaching them to existing separatrices
            // (this is not true for the final vertical connections but we use
            // the same approach to name them consistently.)
            ::intervalxt::Label source = label;
            auto rights = static_cast<std::vector<SaddleConnection<Surface>>>(connection.right());
            if (top)
              std::reverse(begin(rights), end(rights));
            for (auto vertical : rights) {
              // TODO: This is again a case where our normalization (or lack thereof) bites us.
              ASSERT(injectedConnections.find(vertical) == end(injectedConnections), "an injected connection and its inverse must appear on different sides (left/right) of the contour");
              const auto target = ::intervalxt::Label(-(injectedConnections.size() + 1));
              injectHere.push_back(injectedConnections[vertical] = (top ? std::pair{target, source} : std::pair{source, target}));
              source = target;
              verticals.push_back(vertical);
              isTop[vertical] = top;
            }
            inject(label, {}, {}, injectHere, verticals);
          } else {
            // Now we inject the inverse connections on the left using the existing
            // naming scheme.
            auto lefts = static_cast<std::vector<SaddleConnection<Surface>>>(connection.left());
            if (!top)
              std::reverse(begin(lefts), end(lefts));
            for (auto vertical : lefts) {
              ASSERT(injectedConnections.find(-vertical) != end(injectedConnections), "a left connection must have a corresponding right connection");
              const auto corresponding = injectedConnections[-vertical];
              injectHere.push_back({corresponding.second, corresponding.first});
              verticals.push_back(vertical);
            }
            inject(label, injectHere, verticals, {}, {});
          }
        }
      }
    }
  }
}

template <typename Surface>
std::ostream& operator<<(std::ostream& os, const FlowDecompositionState<Surface>& self) {
  return os << fmt::format("FlowDecompositionState(injected={}, detected={})",
             fmt::join(self.injectedConnections | rx::transform([&](const auto& connection) { return fmt::format("{}: {}", connection.first, connection.second); }) | rx::to_vector(), ", "),
             fmt::join(self.detectedConnections | rx::transform([&](const auto& connection) { return fmt::format("{}: {}", connection.first, connection.second); }) | rx::to_vector(), ", "));
}

}  // namespace flatsurf

// Instantiations of templates so implementations are generated for the linker
LIBFLATSURF_INSTANTIATE_MANY_WRAPPED((LIBFLATSURF_INSTANTIATE_WITHOUT_IMPLEMENTATION), FlowDecompositionState, LIBFLATSURF_FLAT_TRIANGULATION_TYPES)
