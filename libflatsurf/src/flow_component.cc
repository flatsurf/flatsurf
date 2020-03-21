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
#include <intervalxt/fmt.hpp>
#include <intervalxt/label.hpp>

#include "external/rx-ranges/include/rx/ranges.hpp"

#include "../flatsurf/ccw.hpp"
#include "../flatsurf/flow_component.hpp"
#include "../flatsurf/fmt.hpp"
#include "../flatsurf/orientation.hpp"
#include "../flatsurf/vertical.hpp"

#include "impl/collapsed_half_edge.hpp"
#include "impl/contour_component.impl.hpp"
#include "impl/contour_decomposition_state.hpp"
#include "impl/flat_triangulation_collapsed.impl.hpp"
#include "impl/flow_component.impl.hpp"
#include "impl/flow_connection.impl.hpp"
#include "impl/saddle_connection.impl.hpp"

#include "util/assert.ipp"

namespace flatsurf {

using std::ostream;
using std::string;

template <typename Surface>
template <typename ...Args>
FlowComponent<Surface>::FlowComponent(PrivateConstructor, Args&&...args) :
  impl(spimpl::make_impl<Implementation>(std::forward<Args>(args)...)) {}

template <typename Surface>
bool FlowComponent<Surface>::decompose(std::function<bool(const FlowComponent<Surface>&)> target, int limit) {
  const auto check = [&]() {
    ASSERTIONS(([&]() {
      auto paths = impl->state->components | rx::transform([&](const auto& component) { return Path(ImplementationOf<FlowComponent<Surface>>::make(impl->state, &const_cast<FlowComponentState<Surface>&>(component)).perimeter() | rx::transform([](const auto& connection) { return connection.saddleConnection(); }) | rx::to_vector()); }) | rx::to_vector();
    ImplementationOf<ContourDecomposition<Surface>>::check(paths, vertical());
    }));
  };

  while (!target(*this)) {
    check();

    auto step = impl->component->dynamicalComponent.decompositionStep(limit);

    if (step.result == intervalxt::DecompositionStep::Result::LIMIT_REACHED)
      return false;

    if (step.equivalent) {
      // We found a SaddleConnection in intervalxt. step.equivalent contains a
      // sequence of known FlowConnections that sum up to that new
      // SaddleConnection. We now construct that new SaddleConnection by
      // constructing the vector that describes it and finding where it starts
      // and ends in the original surface.

      const auto surface = vertical().surface();

      // Reconstruct the vector of our new SaddleConnection.
      Chain<FlatTriangulation<T>> vector(surface);
      for (const auto& connection : *step.equivalent) {
        auto flowConnection = ImplementationOf<FlowConnection<Surface>>::make(impl->state, *this, connection);
        if (!flowConnection.vertical()) {
          // Since the default for ::make() is to assume that things were made
          // for walking the contour counterclockwise, a HalfEdge on the top is
          // left-to-right, and a HalfEdge on the bottom is right-to-left.
          // However, here we need the opposite since we are walking
          // step.equivalent clockwise. intervalxt should probably report
          // things more consistently, i.e., non-verticals with an explicit
          // orientation so we do not need this switch anymore.
          vector -= flowConnection.saddleConnection();
        } else {
          vector += flowConnection.saddleConnection();
        }
      }

      ASSERT(vector, "SaddleConnection must not be the zero vector");
      ASSERT(!vertical().perpendicular(vector), "SaddleConnection must be vertical");
      ASSERT(vertical().parallel(vector) > 0, "SaddleConnection must be parallel but " << vector << " is antiparallel.");

      // The first SaddleConnection of step.equivalent. The new
      // SaddleConnection must start clockwise from that one.
      auto clockwiseFrom = [&]() {
        const auto precedingFlowConnection = ImplementationOf<FlowConnection<Surface>>::make(impl->state, *this, *begin(*step.equivalent));
        // Similarly, to the above, we need to turn a connection coming from a HalfEdge around.
        return precedingFlowConnection.vertical() ? precedingFlowConnection.saddleConnection() : -precedingFlowConnection.saddleConnection();
      }();

      // The negative of the last SaddleConnection of step.equivalent.
      // The negative of the new SaddleConnection must start counterclockwise
      // from that one.
      auto counterclockwiseTo = [&]() {
        const auto finalFlowConnection = ImplementationOf<FlowConnection<Surface>>::make(impl->state, *this, *rbegin(*step.equivalent));
        // Similarly, to the above, we need to turn a connection coming from a HalfEdge around.
        return finalFlowConnection.vertical() ? -finalFlowConnection.saddleConnection() : finalFlowConnection.saddleConnection();
      }();

      ASSERT(clockwiseFrom.vector().ccw(vector) != CCW::COUNTERCLOCKWISE, "Vertical must be clockwise from the half edge direction");

      enum SECTOR {
        NORTH,
        NORTH_WEST,
        WEST,
        SOUTH_WEST,
        SOUTH,
        SOUTH_EAST,
        EAST,
        NORTH_EAST,
      };

      const auto classify = [](const auto& vertical, const auto& vector) {
        if (vertical.perpendicular(vector) < 0) {
          if (vertical.parallel(vector) < 0) return SOUTH_WEST;
          if (vertical.parallel(vector) > 0) return NORTH_WEST;
          return WEST;
        }
        if (vertical.perpendicular(vector) > 0) {
          if (vertical.parallel(vector) < 0) return SOUTH_EAST;
          if (vertical.parallel(vector) > 0) return NORTH_EAST;
          return EAST;
        }
          if (vertical.parallel(vector) < 0) return SOUTH;
          if (vertical.parallel(vector) > 0) return NORTH;
          throw std::logic_error("cannot classify zero vector");
      };

      // The following logic should probably be abstracted away into SaddleConnection somehow.

      // The source of the new SaddleConnection, i.e.,
      // counterclockwise to which HalfEdge of the original surface
      // SaddleConnection starts (inclusive.)
      const auto source = [&]() {
        auto ret = clockwiseFrom.source();

        while(true) {
          switch(classify(vertical(), surface->fromEdge(ret))) {
            case NORTH:
            case NORTH_EAST:
            case EAST:
            case SOUTH_EAST:
              break;
            default:
              ret = surface->previousAtVertex(ret);
              continue;
          }
          break;
        }

        ASSERT(surface->inSector(ret, vector), "We determined that the new SaddleConnection " << vector << " must start in the sector counterclockwise from " << ret << " but that vector is not in the sector.");

        return ret;
      }();

      // The target of the new SaddleConnection, i.e.,
      // counterclockwise to which HalfEdge of the original surface
      // -SaddleConnection starts (inclusive.)
      auto target = [&]() {
        auto ret = counterclockwiseTo.source();

        while(true) {
          switch(classify(vertical(), surface->fromEdge(ret))) {
            case NORTH_WEST:
            case WEST:
            case SOUTH_WEST:
              break;
            default:
              ret = surface->nextAtVertex(ret);
              continue;
          }
          break;
        }

        while(true) {
          switch(classify(vertical(), surface->fromEdge(ret))) {
            case SOUTH_EAST:
            case EAST:
            case NORTH_EAST:
              break;
            default:
              ret = surface->nextAtVertex(ret);
              continue;
          }
          break;
        }

        ret = surface->previousAtVertex(ret);

        ASSERT(surface->inSector(ret, -vector), "We determined that the new SaddleConnection " << vector << " must end in the sector counterclockwise from " << ret << " but the negative of that vector is not in the sector.");

        return ret;
      }();

      const auto connection = SaddleConnection<FlatTriangulation<T>>(surface, source, target, vector);

      ASSERT(vertical().perpendicular(connection) == 0, "Detected connection must be vertical but " << connection << " is not.");
      ASSERT(vertical().parallel(connection) > 0, " Detected connection is parallel but " << connection << " is antiparallel.");

      ASSERT(connection.source() == source && connection.target() == target, "SaddleConnection normalization was unhappy with our source()/target() but we had picked them so they would be correct.");

      ASSERT(clockwiseFrom.vector().ccw(connection) == CCW::CLOCKWISE || (clockwiseFrom.vector().ccw(connection) == CCW::COLLINEAR && clockwiseFrom.vector().orientation(connection) == ORIENTATION::OPPOSITE), "Detected SaddleConnection must be reachable clockwise from the existing contour but " << connection << " is not clockwise from " << clockwiseFrom);

      impl->state->detectedConnections.emplace(*step.connection, connection);
      impl->state->detectedConnections.emplace(-*step.connection, -connection);
    }
    if (step.additionalComponent) {
      impl->state->components.push_back({
          impl->component->contourComponent,
          impl->component->iet,
          *step.additionalComponent,
      });

      auto additionalComponent = Implementation::make(impl->state, &*impl->state->components.rbegin());

      check();

      return decompose(target, limit) && additionalComponent.decompose(target, limit);
    } else {
      check();
    }
  }

  return true;
}

template <typename Surface>
boost::logic::tribool FlowComponent<Surface>::cylinder() const { return impl->component->dynamicalComponent.cylinder(); }

template <typename Surface>
boost::logic::tribool FlowComponent<Surface>::withoutPeriodicTrajectory() const { return impl->component->dynamicalComponent.withoutPeriodicTrajectory(); }

template <typename Surface>
boost::logic::tribool FlowComponent<Surface>::keane() const { return impl->component->dynamicalComponent.keane(); }

template <typename Surface>
Surface FlowComponent<Surface>::triangulation() const {
  throw std::logic_error("not implemented: triangulation()");
}

template <typename Surface>
Vertical<Surface> FlowComponent<Surface>::vertical() const {
  const auto& collapsedSurface = impl->state->contourDecomposition.collapsed();
  return Vertical<Surface>(collapsedSurface->uncollapsed(), collapsedSurface->vertical().vertical());
}

template <typename Surface>
typename Surface::Coordinate FlowComponent<Surface>::area() const {
  return Vector<T>::area(perimeter() | rx::transform([&](const auto & connection) { return static_cast<const Vector<T>&>(connection.saddleConnection()); }) | rx::to_vector());
}

template <typename Surface>
typename Surface::Coordinate FlowComponent<Surface>::width() const {
  T sum = T();
  auto vertical = this->vertical();
  for (const auto& c : perimeter()) {
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
    perimeter.push_back(ImplementationOf<FlowConnection<Surface>>::make(impl->state, *this, side));

  ASSERTIONS([&]() {
    Path<FlatTriangulation<T>> path = perimeter | rx::transform([&](const auto connection) { return connection.saddleConnection();}) | rx::to_vector();
    ASSERT(path.simple(), "Perimeter of FlowComponent must not contain duplicates but " << path << " does.");
    ASSERT(path.closed(), "Perimeter of FlowComponent must be closed but " << path << " is not.");
  });

  return perimeter;
}

template <typename Surface>
bool FlowComponent<Surface>::operator==(const FlowComponent<Surface>& rhs) const {
  return impl->component == rhs.impl->component;
}

template <typename Surface>
const IntervalExchangeTransformation<FlatTriangulationCollapsed<typename Surface::Coordinate>>& FlowComponent<Surface>::intervalExchangeTransformation() const {
  return *impl->component->iet;
}

template <typename Surface>
FlowComponent<Surface> ImplementationOf<FlowComponent<Surface>>::make(std::shared_ptr<FlowDecompositionState<Surface>> state, FlowComponentState<Surface>* component) {
  return FlowComponent<Surface>(PrivateConstructor{}, state, component);
}

template <typename Surface>
ImplementationOf<FlowComponent<Surface>>::ImplementationOf(std::shared_ptr<FlowDecompositionState<Surface>> state, FlowComponentState<Surface>* component) :
  state(state),
  component(component) {
}

template <typename Surface>
std::string ImplementationOf<FlowComponent<Surface>>::id() const {
  throw std::logic_error("not implemented: id()");
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

  return os << fmt::format("{} with perimeter [{}]", kind, fmt::join(self.perimeter(), " → "));
}

}  // namespace flatsurf

// Instantiations of templates so implementations are generated for the linker
#include "util/instantiate.ipp"

LIBFLATSURF_INSTANTIATE_MANY_WRAPPED((LIBFLATSURF_INSTANTIATE_WITH_IMPLEMENTATION), FlowComponent, LIBFLATSURF_FLAT_TRIANGULATION_TYPES)
