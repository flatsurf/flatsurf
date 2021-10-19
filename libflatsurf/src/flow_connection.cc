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

#include "../flatsurf/flow_connection.hpp"

#include <intervalxt/label.hpp>
#include <ostream>

#include "../flatsurf/ccw.hpp"
#include "../flatsurf/flat_triangulation.hpp"
#include "../flatsurf/vertical.hpp"
#include "impl/flow_component.impl.hpp"
#include "impl/flow_connection.impl.hpp"
#include "impl/flow_decomposition_state.hpp"
#include "impl/saddle_connection.impl.hpp"
#include "util/assert.ipp"

namespace flatsurf {

template <typename Surface>
SaddleConnection<FlatTriangulation<typename Surface::Coordinate>> FlowConnection<Surface>::saddleConnection() const {
  return self->saddleConnection;
}

template <typename Surface>
bool FlowConnection<Surface>::operator==(const FlowConnection<Surface>& rhs) const {
  return self->saddleConnection == rhs.self->saddleConnection;
}

template <typename Surface>
bool FlowConnection<Surface>::vertical() const {
  return parallel() || antiparallel();
}

template <typename Surface>
bool FlowConnection<Surface>::parallel() const {
  return self->kind == ImplementationOf<FlowConnection>::Kind::PARALLEL;
}

template <typename Surface>
bool FlowConnection<Surface>::antiparallel() const {
  return self->kind == ImplementationOf<FlowConnection>::Kind::ANTIPARALLEL;
}

template <typename Surface>
bool FlowConnection<Surface>::bottom() const {
  return self->kind == ImplementationOf<FlowConnection>::Kind::BOTTOM;
}

template <typename Surface>
bool FlowConnection<Surface>::top() const {
  return self->kind == ImplementationOf<FlowConnection>::Kind::TOP;
}

template <typename Surface>
bool FlowConnection<Surface>::boundary() const {
  return vertical() && (-*this).component() != component();
}

template <typename Surface>
FlowConnection<Surface> FlowConnection<Surface>::operator-() const {
  const auto search = -saddleConnection();

  const auto find = [&](const auto& component) {
    for (const auto& connection : component.perimeter())
      if (connection.saddleConnection() == search) {
        LIBFLATSURF_ASSERT(vertical() || this->component() == component, "Non-vertical connections can not be attached to distinct components.");
        return std::optional<FlowConnection>(connection);
      }
    return std::optional<FlowConnection>();
  };

  if (!self->negative) {
    if (!vertical())
      self->negative = find(self->component);
    else
      for (auto& component_ : self->state->components) {
        if (self->negative) break;
        const auto component = ImplementationOf<FlowComponent<Surface>>::make(self->state, &component_);
        self->negative = find(component);
      }
  }

  LIBFLATSURF_ASSERT(self->negative->self->kind == static_cast<typename ImplementationOf<FlowConnection>::Kind>(-static_cast<int>(self->kind)), "Negative of connection is of unexpected type.");

  return self->negative.value();
}

template <typename Surface>
FlowConnection<Surface> FlowConnection<Surface>::previousInPerimeter() const {
  const auto perimeter = self->component.perimeter();
  for (auto it = begin(perimeter); it != end(perimeter); it++) {
    if (*it == *this) {
      if (it == begin(perimeter))
        it = end(perimeter);
      return *--it;
    }
  }
  LIBFLATSURF_UNREACHABLE("connection must appear in its own perimeter")
}

template <typename Surface>
FlowConnection<Surface> FlowConnection<Surface>::nextInPerimeter() const {
  const auto perimeter = self->component.perimeter();
  for (auto it = begin(perimeter); it != end(perimeter); it++) {
    if (*it == *this) {
      it++;
      if (it == end(perimeter))
        it = begin(perimeter);
      return *it;
    }
  }
  LIBFLATSURF_UNREACHABLE("connection must appear in its own perimeter")
}

template <typename Surface>
FlowComponent<Surface> FlowConnection<Surface>::component() const {
  return self->component;
}

template <typename Surface>
ImplementationOf<FlowConnection<Surface>>::ImplementationOf(std::shared_ptr<FlowDecompositionState<Surface>> state, const FlowComponent<Surface>& component, const SaddleConnection<FlatTriangulation<T>>& saddleConnection, Kind kind) :
  state(state),
  component(component),
  saddleConnection(saddleConnection),
  kind(kind) {}

template <typename Surface>
FlowConnection<Surface> ImplementationOf<FlowConnection<Surface>>::make(std::shared_ptr<FlowDecompositionState<Surface>> state, const FlowComponent<Surface>& component, const intervalxt::Connection& connection) {
  LIBFLATSURF_ASSERT(state->injectedConnections.find(connection) != end(state->injectedConnections) || state->detectedConnections.find(connection) != end(state->detectedConnections), "Connection " << connection << " not known to " << *state);

  const Kind kind = connection.parallel() ? Kind::PARALLEL : Kind::ANTIPARALLEL;

  FlowConnection<Surface> ret = (state->injectedConnections.find(connection) != state->injectedConnections.end())
                                    ? FlowConnection<Surface>(PrivateConstructor{}, state, component, state->injectedConnections.at(connection), kind)
                                    : FlowConnection<Surface>(PrivateConstructor{}, state, component, state->detectedConnections.at(connection), kind);

  LIBFLATSURF_ASSERT(ret.vertical(), "FlowConnection created from vertical Connection must be vertical but " << ret << " created from " << connection << " is not.");
  LIBFLATSURF_ASSERT(connection.parallel() == ret.parallel(), "FlowConnection must have same parallelity as Connection but " << ret << " and " << connection << " do not coincide");

  return ret;
}

template <typename Surface>
FlowConnection<Surface> ImplementationOf<FlowConnection<Surface>>::make(std::shared_ptr<FlowDecompositionState<Surface>> state, const FlowComponent<Surface>& component, const intervalxt::HalfEdge& edge) {
  auto connection = component.intervalExchangeTransformation()[static_cast<intervalxt::Label>(edge)];
  if (edge.top()) connection = -connection;

  FlowConnection<Surface> ret(PrivateConstructor{}, state, component, connection, edge.top() ? Kind::TOP : Kind::BOTTOM);

  LIBFLATSURF_ASSERT(!ret.vertical(), "FlowConnection created from HalfEdge must not be vertical but " << ret << " created from " << edge << " is vertical.");

  return ret;
}

template <typename Surface>
FlowConnection<Surface> ImplementationOf<FlowConnection<Surface>>::make(std::shared_ptr<FlowDecompositionState<Surface>> state, const FlowComponent<Surface>& component, const std::variant<intervalxt::Connection, intervalxt::HalfEdge>& side) {
  if (auto connection = std::get_if<intervalxt::Connection>(&side)) {
    return ImplementationOf<FlowConnection<Surface>>::make(state, component, *connection);
  } else if (auto e = std::get_if<intervalxt::HalfEdge>(&side)) {
    return ImplementationOf<FlowConnection<Surface>>::make(state, component, *e);
  } else {
    throw std::logic_error("not implemented: unknown intervalxt perimeter type");
  }
}

template <typename Surface>
std::ostream& operator<<(std::ostream& os, const FlowConnection<Surface>& self) {
  return os << self.saddleConnection();
}

}  // namespace flatsurf

namespace std {

using namespace flatsurf;

template <typename Surface>
size_t hash<FlowConnection<Surface>>::operator()(const FlowConnection<Surface>& self) const {
  return std::hash<SaddleConnection<FlatTriangulation<typename Surface::Coordinate>>>()(self.saddleConnection());
}

}  // namespace std

// Instantiations of templates so implementations are generated for the linker
#include "util/instantiate.ipp"

LIBFLATSURF_INSTANTIATE_MANY_WRAPPED((LIBFLATSURF_INSTANTIATE_WITH_IMPLEMENTATION), FlowConnection, LIBFLATSURF_FLAT_TRIANGULATION_TYPES)
LIBFLATSURF_INSTANTIATE_MANY_WRAPPED((LIBFLATSURF_INSTANTIATE_HASH), FlowConnection, LIBFLATSURF_FLAT_TRIANGULATION_TYPES)
