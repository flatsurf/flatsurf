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

#include "../flatsurf/contour_connection.hpp"

#include <fmt/format.h>
#include <fmt/ostream.h>

#include <unordered_set>

#include "../flatsurf/fmt.hpp"
#include "../flatsurf/half_edge.hpp"
#include "../flatsurf/path.hpp"
#include "../flatsurf/path_iterator.hpp"
#include "../flatsurf/saddle_connection.hpp"
#include "../flatsurf/vertex.hpp"
#include "../flatsurf/vertical.hpp"
#include "external/rx-ranges/include/rx/ranges.hpp"
#include "impl/contour_component.impl.hpp"
#include "impl/contour_connection.impl.hpp"
#include "impl/contour_decomposition_state.hpp"
#include "util/assert.ipp"

namespace flatsurf {

using std::begin;
using std::end;

template <typename Surface>
bool ContourConnection<Surface>::top() const {
  return !bottom();
}

template <typename Surface>
bool ContourConnection<Surface>::bottom() const {
  return self->contour == ImplementationOf<ContourConnection>::Contour::BOTTOM;
}

template <typename Surface>
const SaddleConnection<FlatTriangulation<typename Surface::Coordinate>>& ContourConnection<Surface>::horizontal() const {
  auto& connection = self->state->surface.fromHalfEdge(top() ? -self->halfEdge : self->halfEdge);
  ASSERT(self->state->surface.vertical().perpendicular(connection) > 0, "ContourConnection::horizontal() must be left-to-right with respect to the vertical but " << connection << " is not.");
  return connection;
}

template <typename Surface>
Path<FlatTriangulation<typename Surface::Coordinate>> ContourConnection<Surface>::left() const {
  auto left = top()
                  ? ImplementationOf<ContourConnection>::cross(*this, nextInPerimeter()).first
                  : ImplementationOf<ContourConnection>::cross(previousInPerimeter(), *this).second;

  ASSERTIONS([&]() {
    for (const auto& connection : left) {
      ASSERT(self->state->surface.vertical().perpendicular(connection) == 0, "ContourConnection::left() must be vertical but " << connection << " is not.");
      ASSERT(self->state->surface.vertical().parallel(connection) < 0, "ContourConnection::left() must be antiparallel but " << connection << " is not.");
    }
  });
  return left;
}

template <typename Surface>
Path<FlatTriangulation<typename Surface::Coordinate>> ContourConnection<Surface>::right() const {
  auto right = top()
                   ? ImplementationOf<ContourConnection>::cross(previousInPerimeter(), *this).second
                   : ImplementationOf<ContourConnection>::cross(*this, nextInPerimeter()).first;
  ASSERTIONS([&]() {
    for (const auto& connection : right) {
      ASSERT(self->state->surface.vertical().perpendicular(connection) == 0, "ContourConnection::right() must be vertical but " << connection << " is not.");
      ASSERT(self->state->surface.vertical().parallel(connection) > 0, "ContourConnection::right() must be parallel but " << connection << " is not.");
    }
  });
  return right;
}

template <typename Surface>
Path<FlatTriangulation<typename Surface::Coordinate>> ContourConnection<Surface>::perimeter() const {
  Path perimeter;
  if (top()) {
    perimeter = rx::chain(right(), std::vector{-horizontal()}, left()) | rx::to_vector();
    ASSERTIONS([&]() {
      for (const auto& connection : perimeter)
        ASSERT(self->state->surface.vertical().perpendicular(connection) <= 0, "ContourConnection::perimeter() must be right-to-left but " << connection << " is not.");
    });
  } else {
    perimeter = rx::chain(left(), std::vector{horizontal()}, right()) | rx::to_vector();
    ASSERTIONS([&]() {
      for (const auto& connection : perimeter)
        ASSERT(self->state->surface.vertical().perpendicular(connection) >= 0, "ContourConnection::perimeter() must be left-to-right but " << connection << " is not.");
    });
  }
  ASSERT(perimeter.simple(), "Perimeter must be simple but " << perimeter << " is not.");
  return perimeter;
}

template <typename Surface>
ContourConnection<Surface> ContourConnection<Surface>::previousInPerimeter() const {
  return ImplementationOf<ContourComponent<Surface>>::previousInPerimeter(self->state, self->component, self->halfEdge);
}

template <typename Surface>
ContourConnection<Surface> ContourConnection<Surface>::nextInPerimeter() const {
  return ImplementationOf<ContourComponent<Surface>>::nextInPerimeter(self->state, self->component, self->halfEdge);
}

template <typename Surface>
ContourConnection<Surface> ContourConnection<Surface>::operator-() const {
  throw std::logic_error("not implemented: ContourConnection::operator-()");
}

template <typename Surface>
bool ContourConnection<Surface>::operator==(const ContourConnection<Surface>& rhs) const {
  const bool ret = self->state == rhs.self->state && self->halfEdge == rhs.self->halfEdge;
  ASSERT(!ret || self->component == rhs.self->component, "One half edge cannot be in two different components.");
  ASSERT(!ret || self->contour == rhs.self->contour, "One half edge cannot be in two different contours.");
  return ret;
}

template <typename Surface>
ContourConnection<Surface> ImplementationOf<ContourConnection<Surface>>::makeTop(std::shared_ptr<ContourDecompositionState<Surface>> state, ContourComponentState<Surface>* const component, HalfEdge e) {
  ContourConnection<Surface> ret(PrivateConstructor{}, state, component, e, true);
  ASSERT(state->surface.vertical().perpendicular(state->surface.fromHalfEdge(e)) < 0, "HalfEdge must be from right to left but " << e << " is not in " << state->surface);
  return ret;
}

template <typename Surface>
ContourConnection<Surface> ImplementationOf<ContourConnection<Surface>>::makeBottom(std::shared_ptr<ContourDecompositionState<Surface>> state, ContourComponentState<Surface>* const component, HalfEdge e) {
  ContourConnection<Surface> ret(PrivateConstructor{}, state, component, e, false);
  ASSERT(state->surface.vertical().perpendicular(state->surface.fromHalfEdge(e)) > 0, "HalfEdge must be from left to right but " << e << " is not in " << state->surface);
  return ret;
}

template <typename Surface>
ImplementationOf<ContourConnection<Surface>>::ImplementationOf(std::shared_ptr<ContourDecompositionState<Surface>> state, ContourComponentState<Surface>* const component, HalfEdge halfEdge, bool top) :
  state(state),
  component(component),
  halfEdge(halfEdge),
  contour(top ? Contour::TOP : Contour::BOTTOM) {}

template <typename Surface>
Path<FlatTriangulation<typename Surface::Coordinate>> ImplementationOf<ContourConnection<Surface>>::turn(const ContourConnection<Surface>& from, const ContourConnection<Surface>& to) {
  ASSERT(to == from.nextInPerimeter(), "can only cross between adjacent connections but " << to.self->halfEdge << " does not follow " << from.self->halfEdge);

  auto& surface = from.self->state->surface;

  Path<FlatTriangulation<T>> turn;

  if (from.bottom() && to.bottom()) {
    // A typical pair of connections on the bottom of the contour.
    turn = surface.turn(surface.nextInFace(from.self->halfEdge), to.self->halfEdge);
  } else if (from.bottom() && to.top()) {
    // The last connection on the bottom of the contour and the first on the top of the contour.
    ASSERT(Vertex::target(from.self->halfEdge, surface) == Vertex::source(to.self->halfEdge, surface), "final connections must point to the same vertex but " << from.self->halfEdge << " and " << to.self->halfEdge << " do not");
    turn = surface.cross(surface.nextInFace(from.self->halfEdge));
    turn.splice(end(turn), surface.turn(surface.previousAtVertex(surface.nextInFace(from.self->halfEdge)), surface.nextInFace(-to.self->halfEdge)));
  } else if (from.top() && to.top()) {
    // A typical pair of connections on the top of the contour; note that even
    // here it is not enough to turn from -from.self->halfEdge to
    // nextInFace(-to.self->halfEdge) as these two half edges can be the same.
    turn = surface.cross(-from.self->halfEdge);
    turn.splice(end(turn), surface.turn(surface.previousAtVertex(-from.self->halfEdge), surface.nextInFace(-to.self->halfEdge)));
  } else {
    // The last connection on the top of the contour and the first on the bottom of the contour.
    ASSERT(from.top() && to.bottom(), "inconsistent top() / bottom()");
    ASSERT(Vertex::target(from.self->halfEdge, surface) == Vertex::source(to.self->halfEdge, surface), "initial connections must start at the same vertex but " << from << " and " << to << " do not");
    turn = surface.cross(-from.self->halfEdge);
    turn.splice(end(turn), surface.turn(surface.previousAtVertex(-from.self->halfEdge), to.self->halfEdge));
  }

  ASSERT(turn.simple(), "Connections cannot appear more than once when moving from one contour connection to the next");

  return turn;
}

template <typename Surface>
std::pair<Path<FlatTriangulation<typename Surface::Coordinate>>, Path<FlatTriangulation<typename Surface::Coordinate>>> ImplementationOf<ContourConnection<Surface>>::cross(const ContourConnection<Surface>& from, const ContourConnection<Surface>& to) {
  auto connections = turn(from, to) | rx::to_list();

  const auto& surface = from.self->state->surface;
  const auto vertical = surface.vertical();

  std::list<SaddleConnection<FlatTriangulation<T>>> atFrom, atTo;

  if (from.bottom() && to.bottom()) {
    // A typical pair of connections on the bottom of the contour.
    atTo.splice(begin(atTo), connections, std::find_if(begin(connections), end(connections), [&](const auto& connection) { return vertical.parallel(connection) < 0; }), end(connections));
    atFrom = connections;
  } else if (from.bottom() && to.top()) {
    // The last connection on the bottom of the contour and the first on the top of the contour.
    atFrom = connections;
  } else if (from.top() && to.top()) {
    // A typical pair of connections on the top of the contour.
    atTo.splice(begin(atTo), connections, std::find_if(begin(connections), end(connections), [&](const auto& connection) { return vertical.parallel(connection) > 0; }), end(connections));
    atFrom = connections;
  } else {
    // The last connection on the top of the contour and the first on the bottom of the contour.
    ASSERT(from.top() && to.bottom(), "inconsistent top() / bottom()");
    atFrom = connections;
  }

  ASSERTIONS([&]() {
    for (const auto& connection : atFrom) {
      ASSERT(vertical.perpendicular(connection) == 0, "connection must be vertical");
      if (from.top()) {
        ASSERT(vertical.parallel(connection) < 0, "connection must be antiparallel");
      } else {
        ASSERT(vertical.parallel(connection) > 0, "connection must be parallel");
      }
    }
    for (const auto& connection : atTo) {
      ASSERT(vertical.perpendicular(connection) == 0, "connection must be vertical");
      if (to.top()) {
        ASSERT(vertical.parallel(connection) > 0, "connection must be parallel");
      } else {
        ASSERT(vertical.parallel(connection) < 0, "connection must be antiparallel");
      }
    }
  });

  return {atFrom | rx::to_vector(), atTo | rx::to_vector()};
}

template <typename Surface>
std::ostream& operator<<(std::ostream& os, const ContourConnection<Surface>& self) {
  return os << self.perimeter();
}

}  // namespace flatsurf

// Instantiations of templates so implementations are generated for the linker
#include "util/instantiate.ipp"

LIBFLATSURF_INSTANTIATE_MANY_WRAPPED((LIBFLATSURF_INSTANTIATE_WITH_IMPLEMENTATION), ContourConnection, LIBFLATSURF_SURFACE_TYPES)
