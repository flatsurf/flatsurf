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
#include <unordered_set>

#include <fmt/format.h>

#include <boost/logic/tribool.hpp>

#include <intervalxt/component.hpp>
#include <intervalxt/decomposition_step.hpp>
#include <intervalxt/label.hpp>
#include <intervalxt/fmt.hpp>

#include "external/rx-ranges/include/rx/ranges.hpp"

#include "../flatsurf/ccw.hpp"
#include "../flatsurf/flow_component.hpp"
#include "../flatsurf/orientation.hpp"
#include "../flatsurf/vertical.hpp"
#include "../flatsurf/fmt.hpp"

#include "impl/flow_connection.impl.hpp"
#include "impl/flow_component.impl.hpp"
#include "impl/flat_triangulation_collapsed.impl.hpp"
#include "impl/contour_component.impl.hpp"
#include "impl/contour_decomposition_state.hpp"
#include "impl/collapsed_half_edge.hpp"
#include "impl/saddle_connection.impl.hpp"

#include "util/assert.ipp"

namespace flatsurf {

using std::ostream;
using std::string;

template <typename Surface>
FlowComponent<Surface>::FlowComponent() :
  // We assume that the caller takes care of setting impl afterwards.
  impl(nullptr) {
}

template <typename Surface>
bool FlowComponent<Surface>::decompose(std::function<bool(const FlowComponent<Surface>&)> target, int limit) {
  auto area = this->area();

  while(!target(*this)) {
    auto step = impl->component->dynamicalComponent.decompositionStep(limit);
    // TODO: If Cylinder, assert that the perimeter is actually a cylinder.
    if (step.result == intervalxt::DecompositionStep::Result::LIMIT_REACHED) {
      ASSERT(area == this->area(), "Area of component has changed during flow decomposition. Started as " << area << " but now is " << this->area());
      return false;
    }
    if (step.equivalent) {
      const auto surface = ::flatsurf::Implementation<FlowConnection<Surface>>::make(impl->state, *this, *begin(*step.equivalent)).saddleConnection().surface().shared_from_this();

      // Register the saddle connection we just discovered
      Chain<FlatTriangulation<typename Surface::Coordinate>> vector(surface);
      for(const auto& connection : *step.equivalent) {
        auto flowConnection = ::flatsurf::Implementation<FlowConnection<Surface>>::make(impl->state, *this, connection);
        // TODO: Unfortunately, non-verticals are not correctly oriented in intervalxt (i.e., a positive HalfEdge is a top half edge, a negative one is bottom; however, we assume things to be in a counter-clockwise context and there is currently no way to write things in a clockwise context. I guess, intervalxt should simply report the equivalent of -connection instead.)
        if (!flowConnection.vertical()) {
          vector -= flowConnection.saddleConnection();
        } else {
          vector += flowConnection.saddleConnection();
        }
        // TODO: Rename Implementation to ImplementationOf to simplify these things
      }
      ASSERT(vector, "SaddleConnection must not be the zero vector");
      ASSERT(!vertical().perpendicular(vector), "SaddleConnection must be vertical");
      ASSERT(vertical().parallel(vector) > 0, "SaddleConnection must be parallel but " << vector << " is not.");
      auto clockwiseFrom = [&]() {
        auto precedingFlowConnection = ::flatsurf::Implementation<FlowConnection<Surface>>::make(impl->state, *this, *begin(*step.equivalent));
        // TODO: Again, the weird behaviour of HalfEdge bites us here.
        return precedingFlowConnection.vertical() ? precedingFlowConnection.saddleConnection() : -precedingFlowConnection.saddleConnection();
      }();

      auto counterclockwiseTo = [&]() {
        auto finalFlowConnection = ::flatsurf::Implementation<FlowConnection<Surface>>::make(impl->state, *this, *rbegin(*step.equivalent));
        // TODO: Again, the weird behaviour of HalfEdge bites us here.
        return finalFlowConnection.vertical() ? -finalFlowConnection.saddleConnection() : finalFlowConnection.saddleConnection();
      }();

      ASSERT(clockwiseFrom.ccw(vector) != CCW::COUNTERCLOCKWISE, "Vertical must be clockwise from the half edge direction");

      auto source = [&]() {
        auto ret = clockwiseFrom.source();
        while(vertical().parallel(surface->fromEdge(ret)) < 0)
          ret = surface->previousAtVertex(ret);
        while(vertical().perpendicular(surface->fromEdge(ret)) < 0)
          ret = surface->previousAtVertex(ret);
        return ret;
      }();

      auto target = [&]() {
        auto ret = counterclockwiseTo.source();
        while(vertical().parallel(surface->fromEdge(ret)) > 0)
          ret = surface->nextAtVertex(ret);
        while(vertical().perpendicular(surface->fromEdge(ret)) <= 0)
          ret = surface->nextAtVertex(ret);
        ret = surface->previousAtVertex(ret);
        return ret;
      }();

      // auto connection = SaddleConnection<Surface>::clockwise(clockwiseFrom, vector);
      auto connection = SaddleConnection<FlatTriangulation<typename Surface::Coordinate>>(surface, source, target, vector);
      
      // TODO: This is nice but too expensive for large vectors.
      // ::flatsurf::Implementation<SaddleConnection<Surface>>::check(connection);

      impl->state->detectedConnections.emplace(*step.connection, connection);
      impl->state->detectedConnections.emplace(-*step.connection, -connection);

      ASSERT(clockwiseFrom.ccw(connection) != CCW::COUNTERCLOCKWISE, "Detected SaddleConnection must be reachable clocwise from the existing contour but " << connection << " is not clockwise from " << clockwiseFrom);
    }
    if (step.additionalComponent) {
      impl->state->components.push_back({
        impl->component->contourComponent,
        impl->component->iet,
        *step.additionalComponent,
      });
      auto additionalComponent = Implementation::make(impl->state, &*impl->state->components.rbegin());
      ASSERT(area == this->area() + additionalComponent.area(), "Area changed while creating additional component. Started as " << area << " but now is " << this->area() << " + " << additionalComponent.area() << " = " << this->area() + additionalComponent.area());
      area = this->area();
      return decompose(target, limit) && additionalComponent.decompose(target, limit);
    } else {
      ASSERT(area == this->area(), "Area changed when non-separating connection detected. Started as " << area << " but now is " << this->area());
    }
  }

  return true;
}

template <typename Surface>
boost::logic::tribool FlowComponent<Surface>::cylinder() const noexcept { return impl->component->dynamicalComponent.cylinder(); }

template <typename Surface>
boost::logic::tribool FlowComponent<Surface>::withoutPeriodicTrajectory() const noexcept { return impl->component->dynamicalComponent.withoutPeriodicTrajectory(); }

template <typename Surface>
boost::logic::tribool FlowComponent<Surface>::keane() const noexcept { return impl->component->dynamicalComponent.keane(); }

template <typename Surface>
Implementation<FlowComponent<Surface>>::Implementation(std::shared_ptr<FlowDecompositionState<Surface>> state, FlowComponentState<Surface>* component) :
  state(state),
  component(component) {
}

template <typename Surface>
std::string Implementation<FlowComponent<Surface>>::id() const {
  throw std::logic_error("not implemented: id()");
}

template <typename Surface>
Surface FlowComponent<Surface>::triangulation() const {
  throw std::logic_error("not implemented: triangulation()");
}

template <typename Surface>
Vertical<Surface> FlowComponent<Surface>::vertical() const {
  const auto& collapsedSurface = impl->component->contourComponent.impl->state->surface;
  return Vertical<Surface>(collapsedSurface->impl->original, collapsedSurface->vertical().vertical());
}

template <typename Surface>
typename Surface::Coordinate FlowComponent<Surface>::area() const {
  T height = T();
  T sum = T();
  auto vertical = this->vertical();

  for(const auto& c : perimeter()) {
    auto vector = static_cast<typename Surface::Vector>(c.saddleConnection());
    auto x = vertical.perpendicular(vector);
    auto y = vertical.parallel(vector);
    
    sum -= 2 * x * height;
    sum -= x * y;

    height += y;
  }

  return 3 * sum;
}

template <typename Surface>
typename Surface::Coordinate FlowComponent<Surface>::width() const {
  T sum = T();
  auto vertical = this->vertical();
  for(const auto& c : perimeter()) {
    auto width = vertical.perpendicular(c.saddleConnection());
    if (width > 0)
      sum += width;
  }
  return sum;
}

template <typename Surface>
typename FlowComponent<Surface>::Perimeter FlowComponent<Surface>::perimeter() const {
  Perimeter perimeter;

  for (const auto& side : impl->component->dynamicalComponent.perimeter())
    perimeter.push_back(::flatsurf::Implementation<FlowConnection<Surface>>::make(impl->state, *this, side));

  ASSERT(std::unordered_set<FlowConnection<Surface>>(begin(perimeter), end(perimeter)).size() == perimeter.size(), fmt::format("Perimeter of component can not contain duplicates. The perimeter provided by libintervalxt mapped to FlowConnections as follows: {}", fmt::join(impl->component->dynamicalComponent.perimeter() | rx::transform([&](const auto& connection) { return fmt::format("{}: {}", connection, ::flatsurf::Implementation<FlowConnection<Surface>>::make(impl->state, *this, connection)); }) | rx::to_vector(), ", ")));

  return perimeter;
}

template <typename Surface>
bool FlowComponent<Surface>::operator==(const FlowComponent<Surface>& rhs) const {
  return impl->component == rhs.impl->component;
}

template <typename Surface>
const IntervalExchangeTransformation<typename Surface::Collapsed>& FlowComponent<Surface>::intervalExchangeTransformation() const {
  return *impl->component->iet;
}

template <typename Surface>
FlowComponent<Surface> Implementation<FlowComponent<Surface>>::make(std::shared_ptr<FlowDecompositionState<Surface>> state, FlowComponentState<Surface>* component) {
  FlowComponent<Surface> ret;
  ret.impl = spimpl::make_impl<Implementation>(state, component);
  return ret;
}

template <typename Surface>
ostream& operator<<(ostream& os, const FlowComponent<Surface>& self) {
  string kind = "Component";
  if (self.cylinder()) {
    kind = "Cylinder";
  } else if (self.keane()) {
    kind = "Keane Component";
  } else if (self.withoutPeriodicTrajectory()) {
    kind = "Component Without Periodic Trajectory";
  }

  return os << fmt::format("{} with perimeter {}", kind, fmt::join(self.perimeter(), "→"));
}

}

// Instantiations of templates so implementations are generated for the linker
#include <e-antic/renfxx.h>
#include <exact-real/element.hpp>
#include <exact-real/integer_ring.hpp>
#include <exact-real/number_field.hpp>
#include <exact-real/rational_field.hpp>

namespace flatsurf {

template class FlowComponent<FlatTriangulation<long long>>;
template ostream& operator<<(ostream&, const FlowComponent<FlatTriangulation<long long>>&);
template class FlowComponent<FlatTriangulation<mpz_class>>;
template ostream& operator<<(ostream&, const FlowComponent<FlatTriangulation<mpz_class>>&);
template class FlowComponent<FlatTriangulation<mpq_class>>;
template ostream& operator<<(ostream&, const FlowComponent<FlatTriangulation<mpq_class>>&);
template class FlowComponent<FlatTriangulation<eantic::renf_elem_class>>;
template ostream& operator<<(ostream&, const FlowComponent<FlatTriangulation<eantic::renf_elem_class>>&);
template class FlowComponent<FlatTriangulation<exactreal::Element<exactreal::IntegerRing>>>;
template ostream& operator<<(ostream&, const FlowComponent<FlatTriangulation<exactreal::Element<exactreal::IntegerRing>>>&);
template class FlowComponent<FlatTriangulation<exactreal::Element<exactreal::RationalField>>>;
template ostream& operator<<(ostream&, const FlowComponent<FlatTriangulation<exactreal::Element<exactreal::RationalField>>>&);
template class FlowComponent<FlatTriangulation<exactreal::Element<exactreal::NumberField>>>;
template ostream& operator<<(ostream&, const FlowComponent<FlatTriangulation<exactreal::Element<exactreal::NumberField>>>&);

}

