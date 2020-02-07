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

#include <unordered_set>

#include <fmt/format.h>
#include <fmt/ostream.h>

#include "external/rx-ranges/include/rx/ranges.hpp"

#include "../flatsurf/contour_connection.hpp"
#include "../flatsurf/half_edge.hpp"
#include "../flatsurf/saddle_connection.hpp"
#include "../flatsurf/vertex.hpp"
#include "../flatsurf/vertical.hpp"

#include "impl/contour_connection.impl.hpp"
#include "impl/contour_decomposition_state.hpp"

#include "util/assert.ipp"

namespace flatsurf {

template <typename Surface>
ContourConnection<Surface>::ContourConnection() :
  // We assume that the caller initializes impl afterwards.
  impl(nullptr) {}

template <typename Surface>
bool ContourConnection<Surface>::top() const {
  return !bottom();
}

template <typename Surface>
bool ContourConnection<Surface>::bottom() const {
  return impl->contour == Implementation::Contour::BOTTOM;
}

template <typename Surface>
SaddleConnection<FlatTriangulation<typename Surface::Coordinate>> ContourConnection<Surface>::connection() const {
  if (bottom())
    return impl->state->surface->fromEdge(impl->e);
  return -(-*this).connection();
}

template <typename Surface>
std::list<SaddleConnection<FlatTriangulation<typename Surface::Coordinate>>> ContourConnection<Surface>::left() const {
  if (top())
    return Implementation::cross(*this, nextInPerimeter()).first;
  else
    return Implementation::cross(previousInPerimeter(), *this).second;
}

template <typename Surface>
std::list<SaddleConnection<FlatTriangulation<typename Surface::Coordinate>>> ContourConnection<Surface>::right() const {
  if (top())
    return Implementation::cross(previousInPerimeter(), *this).second;
  else
    return Implementation::cross(*this, nextInPerimeter()).first;
}

template <typename Surface>
ContourConnection<Surface> ContourConnection<Surface>::previousInPerimeter() const {
  auto perimeter = impl->component.perimeter();
  for (auto connection = ++begin(perimeter); connection != end(perimeter); connection++)
    if (*this == *connection)
      return *--connection;
  if (*this == *begin(perimeter))
    return *--end(perimeter);
  ASSERT(false, "Connection " << *this << " not present in perimeter of its own component " << impl->component);
}

template <typename Surface>
ContourConnection<Surface> ContourConnection<Surface>::nextInPerimeter() const {
  auto perimeter = impl->component.perimeter();
  for (auto connection = begin(perimeter); connection != --end(perimeter); connection++)
    if (*this == *connection)
      return *++connection;
  if (*this == *(--end(perimeter)))
    return *begin(perimeter);
  ASSERT(false, "Connection " << *this << " not present in perimeter of its own component " << impl->component);
}

template <typename Surface>
ContourConnection<Surface> ContourConnection<Surface>::operator-() const {
  using Contour = typename Implementation::Contour;
  auto neg = *this;
  neg.impl->contour = impl->contour == Contour::TOP ? Contour::BOTTOM : Contour::TOP;
  return neg;
}

template <typename Surface>
bool ContourConnection<Surface>::operator==(const ContourConnection<Surface>& rhs) const {
  return impl->component == rhs.impl->component && impl->e == rhs.impl->e && impl->contour == rhs.impl->contour;
}

template <typename Surface>
ContourConnection<Surface> Implementation<ContourConnection<Surface>>::make(std::shared_ptr<ContourDecompositionState<Surface>> state, const ContourComponent<Surface>& component, HalfEdge e) {
  ContourConnection<Surface> ret;
  ret.impl = spimpl::make_impl<Implementation>(state, component, e, Contour::BOTTOM);
  ASSERT(state->surface->vertical().perpendicular(state->surface->fromEdge(e)) > 0, "HalfEdge must be from left to right but " << e << " is not in " << *state->surface);
  return ret;
}

template <typename Surface>
Implementation<ContourConnection<Surface>>::Implementation(std::shared_ptr<ContourDecompositionState<Surface>> state, const ContourComponent<Surface>& component, HalfEdge e, Contour contour) :
  state(state),
  component(component),
  e(e),
  contour(contour) {}

template <typename Surface>
std::list<SaddleConnection<FlatTriangulation<typename Surface::Coordinate>>> Implementation<ContourConnection<Surface>>::turn(const ContourConnection<Surface>& from, const ContourConnection<Surface>& to) {
  using SaddleConnection = flatsurf::SaddleConnection<FlatTriangulation<T>>;

  ASSERT(to == from.nextInPerimeter(), "can only cross between adjacent connections but " << to << " does not follow " << from);

  auto& surface = *from.impl->state->surface;

  std::list<SaddleConnection> turn;

  if (from.bottom() && to.bottom()) {
    // A typical pair of connections on the bottom of the contour.
    turn = surface.turn(surface.nextInFace(from.impl->e), to.impl->e) | rx::to_list();
  } else if (from.bottom() && to.top()) {
    // The last connection on the bottom of the contour and the first on the top of the contour.
    ASSERT(Vertex::target(from.impl->e, surface) == Vertex::target(to.impl->e, surface), "final connections must point to the same vertex but " << from << " and " << to << " do not");
    turn = surface.cross(surface.nextInFace(from.impl->e)) | rx::to_list();
    turn.splice(end(turn), surface.turn(surface.previousAtVertex(surface.nextInFace(from.impl->e)), surface.nextInFace(to.impl->e)) | rx::to_list());
  } else if (from.top() && to.top()) {
    // A typical pair of connections on the top of the contour.
    turn = surface.turn(from.impl->e, surface.nextInFace(to.impl->e)) | rx::to_list();
  } else {
    // The last connection on the top of the contour and the first on the bottom of the contour.
    ASSERT(from.top() && to.bottom(), "inconsistent top() / bottom()");
    ASSERT(Vertex::source(from.impl->e, surface) == Vertex::source(to.impl->e, surface), "initial connections must start at the same vertex but " << from << " and " << to << " do not");
    turn = surface.cross(from.impl->e) | rx::to_list();
    turn.splice(end(turn), surface.turn(surface.previousAtVertex(from.impl->e), to.impl->e) | rx::to_list());
  }

  std::list<SaddleConnection> simplified;

  for (auto& connection : turn) {
    if (!simplified.empty() && connection == -*rbegin(simplified))
      simplified.pop_back();
    else
      simplified.push_back(connection);
  }

  ASSERT(std::unordered_set<SaddleConnection>(begin(simplified), end(simplified)).size() == simplified.size(), "connections cannot appear more than once when moving from one contour connection to the next");

  return simplified;
}

template <typename Surface>
std::pair<std::list<SaddleConnection<FlatTriangulation<typename Surface::Coordinate>>>, std::list<SaddleConnection<FlatTriangulation<typename Surface::Coordinate>>>> Implementation<ContourConnection<Surface>>::cross(const ContourConnection<Surface>& from, const ContourConnection<Surface>& to) {
  auto connections = turn(from, to);

  const auto& surface = *from.impl->state->surface;
  const auto vertical = surface.vertical();

  std::list<SaddleConnection<FlatTriangulation<T>>> atFrom, atTo;

  if (from.bottom() && to.bottom()) {
    // A typical pair of connections on the bottom of the contour.
    atTo.splice(begin(atTo), connections, std::find_if(begin(connections), end(connections), [&](const auto& connection) { return vertical.parallel(connection) < 0; }), end(connections));
    std::reverse(begin(atTo), end(atTo));
    for (auto& connection : atTo) connection = -connection;
    atFrom = connections;
  } else if (from.bottom() && to.top()) {
    // The last connection on the bottom of the contour and the first on the top of the contour.
    atFrom = connections;
  } else if (from.top() && to.top()) {
    // A typical pair of connections on the top of the contour.
    atTo.splice(begin(atTo), connections, std::find_if(begin(connections), end(connections), [&](const auto& connection) { return vertical.parallel(connection) > 0; }), end(connections));
    std::reverse(begin(atTo), end(atTo));
    for (auto& connection : atTo) connection = -connection;
    atFrom = connections;
  } else {
    // The last connection on the top of the contour and the first on the bottom of the contour.
    ASSERT(from.top() && to.bottom(), "inconsistent top() / bottom()");
    atFrom = connections;
  }

  ASSERT(std::all_of(begin(atFrom), end(atFrom), [&](const auto& connection) {
    auto parallel = vertical.parallel(connection);
    return from.bottom() ? parallel > 0 : parallel < 0;
  }),
      "Connections must be parallel");
  ASSERT(std::all_of(begin(atTo), end(atTo), [&](const auto& connection) {
    auto parallel = vertical.parallel(connection);
    return to.bottom() ? parallel > 0 : parallel < 0;
  }),
      "Connections must be parallel");

  return {atFrom, atTo};
}

template <typename Surface>
std::ostream& operator<<(std::ostream& os, const ContourConnection<Surface>& self) {
  return os << fmt::format("ContourConnection({}←{}→{})", fmt::format("{}", fmt::join(self.left() | rx::transform([&](const auto& connection) { return fmt::format("{}", connection); }) | rx::to_vector(), "←")), self.connection(), fmt::join(self.right() | rx::transform([&](const auto& connection) { return fmt::format("{}", connection); }) | rx::to_vector(), "→"));
}

}  // namespace flatsurf

// Instantiations of templates so implementations are generated for the linker
#include "util/instantiate.ipp"

LIBFLATSURF_INSTANTIATE_MANY_WRAPPED((LIBFLATSURF_INSTANTIATE_WITH_IMPLEMENTATION), ContourConnection, LIBFLATSURF_SURFACE_TYPES)
