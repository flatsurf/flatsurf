/**********************************************************************
 *  This file is part of flatsurf.
 *
 *        Copyright (C) 2019-2021 Julian Rüth
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

#include "../flatsurf/flow_component.hpp"

#include <fmt/format.h>

#include <boost/logic/tribool.hpp>
#include <intervalxt/component.hpp>
#include <intervalxt/decomposition_step.hpp>
#include <intervalxt/fmt.hpp>
#include <intervalxt/label.hpp>
#include <memory>
#include <ostream>
#include <unordered_set>

#include "../flatsurf/ccw.hpp"
#include "../flatsurf/fmt.hpp"
#include "../flatsurf/orientation.hpp"
#include "../flatsurf/path_iterator.hpp"
#include "../flatsurf/vertical.hpp"
#include "external/rx-ranges/include/rx/ranges.hpp"
#include "impl/collapsed_half_edge.hpp"
#include "impl/contour_component.impl.hpp"
#include "impl/contour_decomposition_state.hpp"
#include "impl/flat_triangulation_collapsed.impl.hpp"
#include "impl/flow_decomposition.impl.hpp"
#include "impl/flow_component.impl.hpp"
#include "impl/flow_connection.impl.hpp"
#include "impl/flow_triangulation.impl.hpp"
#include "impl/saddle_connection.impl.hpp"
#include "util/assert.ipp"

namespace flatsurf {

template <typename Surface>
bool FlowComponent<Surface>::decompose(std::function<bool(const FlowComponent<Surface>&)> target, int limit) {
  bool limitSufficed = true;

  const auto check = [&]() {
    auto paths = self->state->components | rx::transform([&](const auto& component) { return Path(ImplementationOf<FlowComponent<Surface>>::make(self->state, &const_cast<FlowComponentState<Surface>&>(component)).perimeter() | rx::transform([](const auto& connection) { return connection.saddleConnection(); }) | rx::to_vector()); }) | rx::to_vector();
    ImplementationOf<ContourDecomposition<Surface>>::check(paths, vertical());
  };

  while (!target(*this)) {
    auto step = self->component->dynamicalComponent.decompositionStep(limit);

    if (step.result == intervalxt::DecompositionStep::Result::LIMIT_REACHED) {
      LIBFLATSURF_ASSERTIONS(check);
      limitSufficed = false;
      break;
    }

    if (step.equivalent) {
      // We found a SaddleConnection in intervalxt. step.equivalent contains a
      // sequence of known FlowConnections that sum up to that new
      // SaddleConnection. We now construct that new SaddleConnection by
      // constructing the vector that describes it and finding where it starts
      // and ends in the original surface.

      const ReadOnly<Surface> surface = vertical().surface();

      // Reconstruct the vector of our new SaddleConnection.
      Chain<FlatTriangulation<T>> vector(surface);
      for (const auto& connection : *step.equivalent) {
        auto flowConnection = ImplementationOf<FlowConnection<Surface>>::make(self->state, *this, connection);
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

      LIBFLATSURF_ASSERT(vector, "SaddleConnection must not be the zero vector");
      LIBFLATSURF_ASSERT(vertical().ccw(vector) == CCW::COLLINEAR, "SaddleConnection must be vertical");
      LIBFLATSURF_ASSERT(vertical().orientation(vector) == ORIENTATION::SAME, "SaddleConnection must be parallel but " << vector << " is antiparallel.");

      // The first SaddleConnection of step.equivalent. The new
      // SaddleConnection must start clockwise from that one.
      auto clockwiseFrom = [&]() {
        const auto precedingFlowConnection = ImplementationOf<FlowConnection<Surface>>::make(self->state, *this, *begin(*step.equivalent));
        // Similarly, to the above, we need to turn a connection coming from a HalfEdge around.
        return precedingFlowConnection.vertical() ? precedingFlowConnection.saddleConnection() : -precedingFlowConnection.saddleConnection();
      }();

      // The negative of the last SaddleConnection of step.equivalent.
      // The negative of the new SaddleConnection must start counterclockwise
      // from that one.
      auto counterclockwiseTo = [&]() {
        const auto finalFlowConnection = ImplementationOf<FlowConnection<Surface>>::make(self->state, *this, *rbegin(*step.equivalent));
        // Similarly, to the above, we need to turn a connection coming from a HalfEdge around.
        return finalFlowConnection.vertical() ? -finalFlowConnection.saddleConnection() : finalFlowConnection.saddleConnection();
      }();

      LIBFLATSURF_ASSERT(clockwiseFrom.vector().ccw(vector) != CCW::COUNTERCLOCKWISE, "Vertical must be clockwise from the half edge direction");

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
        switch (vertical.ccw(vector)) {
          case CCW::COUNTERCLOCKWISE:
            switch (vertical.orientation(vector)) {
              case ORIENTATION::OPPOSITE:
                return SOUTH_WEST;
              case ORIENTATION::SAME:
                return NORTH_WEST;
              default:
                return WEST;
            }
          case CCW::CLOCKWISE:
            switch (vertical.orientation(vector)) {
              case ORIENTATION::OPPOSITE:
                return SOUTH_EAST;
              case ORIENTATION::SAME:
                return NORTH_EAST;
              default:
                return EAST;
            }
          default:
            switch (vertical.orientation(vector)) {
              case ORIENTATION::OPPOSITE:
                return SOUTH;
              case ORIENTATION::SAME:
                return NORTH;
              default:
                LIBFLATSURF_UNREACHABLE("cannot classify zero vector");
            }
        }
      };

      // The following logic should probably be abstracted away into SaddleConnection somehow.

      // The source of the new SaddleConnection, i.e.,
      // counterclockwise to which HalfEdge of the original surface
      // SaddleConnection starts (inclusive.)
      const auto source = [&]() {
        auto ret = clockwiseFrom.source();

        while (true) {
          switch (classify(vertical(), surface->fromHalfEdge(ret))) {
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

        LIBFLATSURF_ASSERT(surface->inSector(ret, vector), "We determined that the new SaddleConnection " << vector << " must start in the sector counterclockwise from " << ret << " but that vector is not in the sector.");

        return ret;
      }();

      // The target of the new SaddleConnection, i.e.,
      // counterclockwise to which HalfEdge of the original surface
      // -SaddleConnection starts (inclusive.)
      auto target = [&]() {
        auto ret = counterclockwiseTo.source();

        while (true) {
          switch (classify(vertical(), surface->fromHalfEdge(ret))) {
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

        while (true) {
          switch (classify(vertical(), surface->fromHalfEdge(ret))) {
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

        LIBFLATSURF_ASSERT(surface->inSector(ret, -vector), "We determined that the new SaddleConnection " << vector << " must end in the sector counterclockwise from " << ret << " but the negative of that vector is not in the sector.");

        return ret;
      }();

      const auto connection = SaddleConnection<FlatTriangulation<T>>(surface, source, target, vector);

      LIBFLATSURF_ASSERT(vertical().ccw(connection) == CCW::COLLINEAR, "Detected connection must be vertical but " << connection << " is not.");
      LIBFLATSURF_ASSERT(vertical().orientation(connection) == ORIENTATION::SAME, " Detected connection is parallel but " << connection << " is antiparallel.");

      LIBFLATSURF_ASSERT(connection.source() == source && connection.target() == target, "SaddleConnection normalization was unhappy with our source()/target() but we had picked them so they would be correct.");

      LIBFLATSURF_ASSERT(clockwiseFrom.vector().ccw(connection) == CCW::CLOCKWISE || (clockwiseFrom.vector().ccw(connection) == CCW::COLLINEAR && clockwiseFrom.vector().orientation(connection) == ORIENTATION::OPPOSITE), "Detected SaddleConnection must be reachable clockwise from the existing contour but " << connection << " is not clockwise from " << clockwiseFrom);

      self->state->detectedConnections.emplace(*step.connection, connection);
      self->state->detectedConnections.emplace(-*step.connection, -connection);
    }

    if (step.additionalComponent) {
      self->state->components.push_back({
          self->component->contourComponent,
          self->component->iet,
          *step.additionalComponent,
      });

      auto component = ImplementationOf<FlowComponent>::make(self->state, &*self->state->components.rbegin());

      limitSufficed = component.decompose(target, limit) && limitSufficed;
    }
  }

  LIBFLATSURF_ASSERTIONS(check);

  return limitSufficed;
}

template <typename Surface>
boost::logic::tribool FlowComponent<Surface>::cylinder() const { return self->component->dynamicalComponent.cylinder(); }

template <typename Surface>
boost::logic::tribool FlowComponent<Surface>::withoutPeriodicTrajectory() const { return self->component->dynamicalComponent.withoutPeriodicTrajectory(); }

template <typename Surface>
boost::logic::tribool FlowComponent<Surface>::keane() const { return self->component->dynamicalComponent.keane(); }

template <typename Surface>
Vertical<Surface> FlowComponent<Surface>::vertical() const {
  const auto& collapsedSurface = self->state->contourDecomposition.collapsed();
  return Vertical<Surface>(collapsedSurface.uncollapsed(), collapsedSurface.vertical());
}

template <typename Surface>
typename Surface::Coordinate FlowComponent<Surface>::area() const {
  return Vector<T>::area(perimeter() | rx::transform([&](const auto& connection) { return static_cast<const Vector<T>&>(connection.saddleConnection()); }) | rx::to_vector());
}

template <typename Surface>
typename Surface::Coordinate FlowComponent<Surface>::width() const {
  T sum = T();
  auto vertical = this->vertical();
  for (const auto& c : perimeter()) {
    auto width = vertical.projectPerpendicular(c.saddleConnection());
    if (width > 0)
      sum += width;
  }
  return sum;
}

template <typename Surface>
std::vector<Vector<typename Surface::Coordinate>> FlowComponent<Surface>::holonomy() const {
  std::unordered_map<SaddleConnection<Surface>, Vector<T>> toSaddleConnection;

  Vector<T> pos;
  for (const auto& connection : perimeter()) {
    if (connection.bottom())
      toSaddleConnection[connection.saddleConnection()] = pos;
    pos += connection.saddleConnection();
    if (connection.top())
      toSaddleConnection[connection.saddleConnection()] = pos;
  }

  LIBFLATSURF_ASSERT(!pos, "perimeter is not closed");

  std::vector<Vector<T>> holonomy;
  for (const auto& connection : bottom()) {
    if (connection.vertical()) continue;

    const auto saddleConnection = connection.saddleConnection();
    const auto zeta = toSaddleConnection.at(-saddleConnection) - toSaddleConnection.at(saddleConnection);
    LIBFLATSURF_ASSERT(zeta, "holonomy cannot be zero");
    holonomy.emplace_back(zeta);
  }

  return holonomy;
}

template <typename Surface>
Vector<typename Surface::Coordinate> FlowComponent<Surface>::circumferenceHolonomy() const {
  if (not this->cylinder())
    throw std::logic_error("circumferenceHolonomy can only be called on cylinders");
  return *begin(holonomy());
}

template <typename Surface>
typename Surface::Coordinate FlowComponent<Surface>::height() const {
  // The vertical components of the holonomies are a lower bound for the
  // circumference of a cylinder. Note that these can be negative.
  // We have to use rx::max() because of a bug in rx::min(), see https://github.com/simonask/rx-ranges/pull/47
  return -*(holonomy() | rx::transform([&](const auto& v) { return vertical().project(-v); }) | rx::max());
}

template <typename Surface>
typename FlowComponent<Surface>::Perimeter FlowComponent<Surface>::bottom() const {
  return self->makePerimeter(self->component->dynamicalComponent.bottom());
}

template <typename Surface>
typename FlowComponent<Surface>::Perimeter FlowComponent<Surface>::right() const {
  return self->makePerimeter(self->component->dynamicalComponent.right());
}

template <typename Surface>
typename FlowComponent<Surface>::Perimeter FlowComponent<Surface>::top() const {
  return self->makePerimeter(self->component->dynamicalComponent.top());
}

template <typename Surface>
typename FlowComponent<Surface>::Perimeter FlowComponent<Surface>::left() const {
  return self->makePerimeter(self->component->dynamicalComponent.left());
}

template <typename Surface>
typename FlowComponent<Surface>::Perimeter FlowComponent<Surface>::perimeter() const {
  auto perimeter = self->makePerimeter(self->component->dynamicalComponent.perimeter());

  LIBFLATSURF_ASSERTIONS([&]() {
    Path<FlatTriangulation<typename Surface::Coordinate>> path = perimeter | rx::transform([&](const auto connection) { return connection.saddleConnection(); }) | rx::to_vector();
    LIBFLATSURF_ASSERT(path.closed(), "Perimeter of FlowComponent must be closed but " << path << " is not.");

    auto a = path.begin();

    auto b = a;
    ++b;

    for (; b != path.end(); a++, b++) {
      const int angle = b->angle(-*a);
      LIBFLATSURF_ASSERT(angle == 0 || (angle == 1 && (-*b).vector().ccw(*a) == CCW::COLLINEAR && (-*b).vector().orientation(*a) == ORIENTATION::SAME), "Connections in perimeter must be turning clockwise by an angle in (0, 2π] but " << *b << " follows " << *a << " in perimeter.");
    }
  });

  return perimeter;
}

template <typename Surface>
typename FlowComponent<Surface>::Perimeter ImplementationOf<FlowComponent<Surface>>::makePerimeter(const std::vector<intervalxt::Side>& sides) const {
  Perimeter perimeter;

  for (const auto& side : sides)
    perimeter.push_back(ImplementationOf<FlowConnection<Surface>>::make(state, ImplementationOf<FlowComponent<Surface>>::make(state, component), side));

  LIBFLATSURF_ASSERTIONS([&]() {
    Path<FlatTriangulation<typename Surface::Coordinate>> path = perimeter | rx::transform([&](const auto connection) { return connection.saddleConnection(); }) | rx::to_vector();
    LIBFLATSURF_ASSERT(path.simple(), "Perimeter of FlowComponent must not contain duplicates but " << path << " does.");
  });

  return perimeter;
}

template <typename Surface>
bool FlowComponent<Surface>::operator==(const FlowComponent<Surface>& rhs) const {
  return self->component == rhs.self->component;
}

template <typename Surface>
const IntervalExchangeTransformation<FlatTriangulationCollapsed<typename Surface::Coordinate>>& FlowComponent<Surface>::intervalExchangeTransformation() const {
  return *self->component->iet;
}

template <typename Surface>
FlowTriangulation<Surface> FlowComponent<Surface>::triangulation() const {
  return ImplementationOf<FlowTriangulation<Surface>>::make(*this);
}

template <typename Surface>
const ::intervalxt::Component& FlowComponent<Surface>::dynamicalComponent() const {
  return self->component->dynamicalComponent;
}

template <typename Surface>
FlowDecomposition<Surface> FlowComponent<Surface>::decomposition() { return ImplementationOf<FlowDecomposition<Surface>>::make(self->state); }

template <typename Surface>
const FlowDecomposition<Surface> FlowComponent<Surface>::decomposition() const { return ImplementationOf<FlowDecomposition<Surface>>::make(self->state); }

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
std::ostream& operator<<(std::ostream& os, const FlowComponent<Surface>& self) {
  std::string kind = "Component";
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
