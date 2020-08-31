/**********************************************************************
 *  This file is part of flatsurf.
 *
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

#include "../flatsurf/flow_decomposition.hpp"

#include <gmpxx.h>

#include <boost/algorithm/string/join.hpp>
#include <boost/lexical_cast.hpp>
#include <intervalxt/dynamical_decomposition.hpp>
#include <intervalxt/sample/arithmetic.hpp>
#include <intervalxt/sample/e-antic-arithmetic.hpp>
#include <intervalxt/sample/exact-real-arithmetic.hpp>
#include <intervalxt/sample/long-long-int-arithmetic.hpp>
#include <intervalxt/sample/rational-arithmetic.hpp>
#include <memory>
#include <ostream>
#include <string>
#include <unordered_map>
#include <vector>

#include "../flatsurf/contour_component.hpp"
#include "../flatsurf/contour_decomposition.hpp"
#include "../flatsurf/flat_triangulation.hpp"
#include "../flatsurf/flat_triangulation_collapsed.hpp"
#include "../flatsurf/flow_connection.hpp"
#include "../flatsurf/flow_triangulation.hpp"
#include "../flatsurf/half_edge.hpp"
#include "../flatsurf/half_edge_map.hpp"
#include "../flatsurf/path.hpp"
#include "../flatsurf/vector.hpp"
#include "../flatsurf/vertical.hpp"
#include "external/rx-ranges/include/rx/ranges.hpp"
#include "impl/contour_decomposition.impl.hpp"
#include "impl/flow_component.impl.hpp"
#include "impl/flow_component_state.hpp"
#include "impl/flow_decomposition.impl.hpp"
#include "impl/flow_decomposition_state.hpp"
#include "impl/interval_exchange_transformation.impl.hpp"
#include "util/assert.ipp"

using std::ostream;

using intervalxt::DynamicalDecomposition;

namespace flatsurf {

template <typename Surface>
template <typename... Args>
FlowDecomposition<Surface>::FlowDecomposition(PrivateConstructor, Args&&... args) :
  impl(spimpl::make_impl<Implementation>(std::forward<Args>(args)...)) {}

template <typename Surface>
FlowDecomposition<Surface>::FlowDecomposition(std::unique_ptr<Surface> surface, const Vector<T>& vertical) :
  impl(spimpl::make_impl<Implementation>(std::move(surface), vertical)) {
  ASSERTIONS(([&]() {
    auto paths = components() | rx::transform([](const auto& component) { return Path(component.perimeter() | rx::transform([](const auto& connection) { return connection.saddleConnection(); }) | rx::to_vector()); }) | rx::to_vector();
    ImplementationOf<ContourDecomposition<Surface>>::check(paths, Vertical(this->surface(), vertical));
  }));
}

template <typename Surface>
std::shared_ptr<const Surface> FlowDecomposition<Surface>::surface() const {
  return impl->state->contourDecomposition.collapsed()->uncollapsed();
}

template <typename Surface>
bool FlowDecomposition<Surface>::decompose(std::function<bool(const FlowComponent<Surface>&)> target, int limit) {
  bool targetReached = true;
  for (auto& component : components())
    targetReached = targetReached && component.decompose(target, limit);
  return targetReached;
}

template <typename Surface>
std::vector<FlowComponent<Surface>> FlowDecomposition<Surface>::components() const {
  std::vector<FlowComponent<Surface>> components;
  for (auto& component : impl->state->components) {
    components.push_back(ImplementationOf<FlowComponent<Surface>>::make(impl->state, &component));
  }
  return components;
}

template <typename Surface>
std::shared_ptr<const FlatTriangulation<typename Surface::Coordinate>> FlowDecomposition<Surface>::triangulation() const {
  std::unordered_map<HalfEdge, Vector<T>> vectors;
  const auto triangulations = components() | rx::transform([](const auto& component) { return component.triangulation(); }) | rx::to_vector();
  const auto faces = triangulations | rx::transform([&](const auto& triangulation) {
    const auto embedding = triangulation.embedding();
    for (auto localHalfEdge : triangulation.triangulation()->halfEdges())
      vectors[embedding[localHalfEdge]] = triangulation.triangulation()->fromEdge(localHalfEdge);
    return triangulation.triangulation()->faces() | rx::transform([&](const auto& face) {
      return std::tuple{embedding[std::get<0>(face)], embedding[std::get<1>(face)], embedding[std::get<2>(face)]};
    }) | rx::to_vector();
  }) | rx::to_vector() |
                     rx::flatten<1>() | rx::to_vector();

  return std::make_shared<FlatTriangulation<T>>(FlatTriangulationCombinatorial(faces), [&](const HalfEdge he) {
    return vectors.at(he);
  });
}

template <typename Surface>
boost::logic::tribool FlowDecomposition<Surface>::hasCylinder() const {
  boost::logic::tribool state = false;
  for (auto& component : components()) {
    state = state || component.cylinder();
    if (state) return true;
  }
  return state;
}

template <typename Surface>
boost::logic::tribool FlowDecomposition<Surface>::completelyPeriodic() const {
  for (auto& component : components()) {
    boost::logic::tribool state = component.cylinder();
    if (state != true) return state;
  }
  return true;
}

template <typename Surface>
boost::logic::tribool FlowDecomposition<Surface>::parabolic() const {
  T a0(0), hnorm20(0);
  boost::logic::tribool ans = true;
  for (auto& component : components()) {
    boost::logic::tribool state = component.cylinder();
    if (state == false) return false;
    ans = ans && state;
    Vector<T> h = component.circumferenceHolonomy();
    T hnorm2 = h.x() * h.x() + h.y() * h.y();
    T a = component.area();
    if (hnorm20 == 0) {
      hnorm20 = hnorm2;
      a0 = a;
    } else {
      std::vector<mpq_class> u = intervalxt::sample::Arithmetic<T>::coefficients(a0 * hnorm2);
      std::vector<mpq_class> v = intervalxt::sample::Arithmetic<T>::coefficients(a * hnorm20);
      for (size_t i = 1; i < u.size(); i++) {
        if (u[0] * v[i] != u[i] * v[0]) {
          return false;
        }
      }
    }
  }
  return ans;
}

template <typename Surface>
ImplementationOf<FlowDecomposition<Surface>>::ImplementationOf(std::unique_ptr<Surface> surface, const Vector<T>& vertical) :
  state(std::make_shared<FlowDecompositionState<Surface>>(std::move(surface), vertical)) {
  for (auto& component : state->components)
    ImplementationOf<IntervalExchangeTransformation<FlatTriangulationCollapsed<T>>>::registerDecomposition(*component.iet, state);
}

template <typename Surface>
ImplementationOf<FlowDecomposition<Surface>>::ImplementationOf(std::shared_ptr<FlowDecompositionState<Surface>> state) :
  state(std::move(state)) {}

template <typename Surface>
Edge ImplementationOf<FlowDecomposition<Surface>>::firstInnerEdge(const FlowComponent<Surface>& component) {
  size_t perimeterHalfEdges = 0;
  for (const auto& other : component.decomposition().components())
    perimeterHalfEdges += other.perimeter().size();
  ASSERT(perimeterHalfEdges % 2 == 0, "edges on the perimeter must come in pairs");

  size_t innerHalfEdges = 0;
  for (const auto& other : component.decomposition().components()) {
    if (other == component) break;
    ASSERT(other.perimeter().size() >= 4, "component has no area");
    innerHalfEdges += 2 * (other.perimeter().size() - 3);
  }

  return Edge(static_cast<int>((perimeterHalfEdges + innerHalfEdges) / 2 + 1));
}

template <typename Surface>
HalfEdge ImplementationOf<FlowDecomposition<Surface>>::halfEdge(const FlowConnection<Surface>& connection) {
  std::unordered_map<FlowConnection<Surface>, HalfEdge> toHalfEdge;
  for (const auto component : connection.component().decomposition().components()) {
    for (const auto other : component.perimeter()) {
      if (toHalfEdge.find(other) == toHalfEdge.end()) {
        toHalfEdge[other] = HalfEdge(static_cast<int>(toHalfEdge.size() / 2 + 1));
        toHalfEdge[-other] = -toHalfEdge.at(other);
      }
      if (other == connection || other == -connection)
        return toHalfEdge.at(connection);
    }
  }
  UNREACHABLE("connection does not show up in this decomposition");
}

template <typename Surface>
FlowDecomposition<Surface> ImplementationOf<FlowDecomposition<Surface>>::make(std::shared_ptr<FlowDecompositionState<Surface>> state) {
  return FlowDecomposition<Surface>(PrivateConstructor{}, std::move(state));
}

template <typename Surface>
FlowDecomposition<Surface> FlowComponent<Surface>::decomposition() { return ImplementationOf<FlowDecomposition<Surface>>::make(impl->state); }

template <typename Surface>
const FlowDecomposition<Surface> FlowComponent<Surface>::decomposition() const { return ImplementationOf<FlowDecomposition<Surface>>::make(impl->state); }

template <typename Surface>
ostream& operator<<(ostream& os, const FlowDecomposition<Surface>& self) {
  std::vector<std::string> components;
  for (auto& component : self.components())
    components.push_back(boost::lexical_cast<std::string>(component));

  return os << "FlowDecompsition(components=[" << boost::algorithm::join(components, ", ") << "])";
}

}  // namespace flatsurf

// Instantiations of templates so implementations are generated for the linker
#include "util/instantiate.ipp"

LIBFLATSURF_INSTANTIATE_MANY_WRAPPED((LIBFLATSURF_INSTANTIATE_WITH_IMPLEMENTATION), FlowDecomposition, LIBFLATSURF_FLAT_TRIANGULATION_TYPES)
