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
#include "../flatsurf/path.hpp"
#include "../flatsurf/path_iterator.hpp"
#include "../flatsurf/saddle_connection.hpp"
#include "../flatsurf/vertex.hpp"
#include "../flatsurf/vertical.hpp"
#include "../flatsurf/fmt.hpp"

#include "impl/contour_connection.impl.hpp"
#include "impl/contour_component.impl.hpp"
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
const SaddleConnection<FlatTriangulation<typename Surface::Coordinate>>& ContourConnection<Surface>::connection() const {
  auto& connection = impl->state->surface->fromEdge(top() ? -impl->halfEdge : impl->halfEdge);
  ASSERT(impl->state->surface->vertical().perpendicular(connection) > 0, "ContourConnection::connection() must be left-to-right with respect to the vertical but " << connection << " is not.");
  return connection;
}

template <typename Surface>
Path<FlatTriangulation<typename Surface::Coordinate>> ContourConnection<Surface>::left() const {
  auto left = top()
    ? Implementation::cross(*this, nextInPerimeter()).first
    : Implementation::cross(previousInPerimeter(), *this).second;
  
  ASSERTIONS([&]() {
    for (const auto& connection : left) {
      ASSERT(impl->state->surface->vertical().perpendicular(connection) == 0, "ContourConnection::left() must be vertical but " << connection << " is not.");
      ASSERT(impl->state->surface->vertical().parallel(connection) < 0, "ContourConnection::left() must be antiparallel but " << connection << " is not.");
    }
  });
  return left;
}

template <typename Surface>
Path<FlatTriangulation<typename Surface::Coordinate>> ContourConnection<Surface>::right() const {
  auto right = top()
    ? Implementation::cross(previousInPerimeter(), *this).second
    : Implementation::cross(*this, nextInPerimeter()).first;
  ASSERTIONS([&]() {
    for (const auto& connection : right) {
      ASSERT(impl->state->surface->vertical().perpendicular(connection) == 0, "ContourConnection::right() must be vertical but " << connection << " is not.");
      ASSERT(impl->state->surface->vertical().parallel(connection) > 0, "ContourConnection::right() must be parallel but " << connection << " is not.");
    }
  });
  return right;
}

template <typename Surface>
Path<FlatTriangulation<typename Surface::Coordinate>> ContourConnection<Surface>::perimeter() const {
  Path perimeter;
  if (top()) {
    perimeter = rx::chain(right(), std::vector{ -connection() }, left()) | rx::to_vector();
    ASSERTIONS([&]() {
        for (const  auto& connection : perimeter)
          ASSERT(impl->state->surface->vertical().perpendicular(connection) <= 0, "ContourConnection::perimeter() must be right-to-left but " << connection << " is not.");
    });
  } else {
    perimeter = rx::chain(left(), std::vector{ connection() }, right()) | rx::to_vector();
    ASSERTIONS([&]() {
        for (const  auto& connection : perimeter)
          ASSERT(impl->state->surface->vertical().perpendicular(connection) >= 0, "ContourConnection::perimeter() must be left-to-right but " << connection << " is not.");
    });
  }
  ASSERT(perimeter.simple(), "Perimeter must be simple but " << perimeter << " is not.");
  return perimeter;
}

template <typename Surface>
ContourConnection<Surface> ContourConnection<Surface>::previousInPerimeter() const {
  return ImplementationOf<ContourComponent<Surface>>::previousInPerimeter(impl->state, impl->component, impl->halfEdge);
}

template <typename Surface>
ContourConnection<Surface> ContourConnection<Surface>::nextInPerimeter() const {
  return ImplementationOf<ContourComponent<Surface>>::nextInPerimeter(impl->state, impl->component, impl->halfEdge);
}

template <typename Surface>
ContourConnection<Surface> ContourConnection<Surface>::operator-() const {
  throw std::logic_error("not implemented: ContourConnection::operator-()");
}

template <typename Surface>
bool ContourConnection<Surface>::operator==(const ContourConnection<Surface>& rhs) const {
  const bool ret = impl->state == rhs.impl->state && impl->halfEdge == rhs.impl->halfEdge;
  ASSERT(!ret || impl->component == rhs.impl->component, "One half edge cannot be in two different components.");
  ASSERT(!ret || impl->contour == rhs.impl->contour, "One half edge cannot be in two different contours.");
  return ret;
}

template <typename Surface>
ContourConnection<Surface> ImplementationOf<ContourConnection<Surface>>::makeTop(std::shared_ptr<ContourDecompositionState<Surface>> state, ContourComponentState<Surface>* const component, HalfEdge e) {
  ContourConnection<Surface> ret;
  ret.impl = spimpl::make_impl<ImplementationOf>(state, component, e, true);
  ASSERT(state->surface->vertical().perpendicular(state->surface->fromEdge(e)) < 0, "HalfEdge must be from right to left but " << e << " is not in " << *state->surface);
  return ret;
}

template <typename Surface>
ContourConnection<Surface> ImplementationOf<ContourConnection<Surface>>::makeBottom(std::shared_ptr<ContourDecompositionState<Surface>> state, ContourComponentState<Surface>* const component, HalfEdge e) {
  ContourConnection<Surface> ret;
  ret.impl = spimpl::make_impl<ImplementationOf>(state, component, e, false);
  ASSERT(state->surface->vertical().perpendicular(state->surface->fromEdge(e)) > 0, "HalfEdge must be from left to right but " << e << " is not in " << *state->surface);
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
  ASSERT(to == from.nextInPerimeter(), "can only cross between adjacent connections but " << to.impl->halfEdge << " does not follow " << from.impl->halfEdge);

  auto& surface = *from.impl->state->surface;

  Path<FlatTriangulation<T>> turn;

  if (from.bottom() && to.bottom()) {
    // A typical pair of connections on the bottom of the contour.
    turn = surface.turn(surface.nextInFace(from.impl->halfEdge), to.impl->halfEdge);
  } else if (from.bottom() && to.top()) {
    // The last connection on the bottom of the contour and the first on the top of the contour.
    ASSERT(Vertex::target(from.impl->halfEdge, surface) == Vertex::source(to.impl->halfEdge, surface), "final connections must point to the same vertex but " << from.impl->halfEdge << " and " << to.impl->halfEdge << " do not");
    turn = surface.cross(surface.nextInFace(from.impl->halfEdge));
    turn.splice(end(turn), surface.turn(surface.previousAtVertex(surface.nextInFace(from.impl->halfEdge)), surface.nextInFace(-to.impl->halfEdge)));
  } else if (from.top() && to.top()) {
    // A typical pair of connections on the top of the contour; note that even
    // here it is not enough to turn from -from.impl->halfEdge to
    // nextInFace(-to.impl->halfEdge) as these two half edges can be the same.
    turn = surface.cross(-from.impl->halfEdge);
    turn.splice(end(turn), surface.turn(surface.previousAtVertex(-from.impl->halfEdge), surface.nextInFace(-to.impl->halfEdge)));
  } else {
    // The last connection on the top of the contour and the first on the bottom of the contour.
    ASSERT(from.top() && to.bottom(), "inconsistent top() / bottom()");
    ASSERT(Vertex::target(from.impl->halfEdge, surface) == Vertex::source(to.impl->halfEdge, surface), "initial connections must start at the same vertex but " << from << " and " << to << " do not");
    turn = surface.cross(-from.impl->halfEdge);
    turn.splice(end(turn), surface.turn(surface.previousAtVertex(-from.impl->halfEdge), to.impl->halfEdge));
  }

  ASSERT(turn.simple(), "Connections cannot appear more than once when moving from one contour connection to the next");

  return turn;
}

template <typename Surface>
std::pair<Path<FlatTriangulation<typename Surface::Coordinate>>, Path<FlatTriangulation<typename Surface::Coordinate>>> ImplementationOf<ContourConnection<Surface>>::cross(const ContourConnection<Surface>& from, const ContourConnection<Surface>& to) {
  auto connections = turn(from, to) | rx::to_list();

  const auto& surface = *from.impl->state->surface;
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

  return {atFrom | rx::to_vector(), atTo | rx::to_vector() };
}

template <typename Surface>
std::ostream& operator<<(std::ostream& os, const ContourConnection<Surface>& self) {
  return os << self.perimeter();
}

}  // namespace flatsurf

// Instantiations of templates so implementations are generated for the linker
#include "util/instantiate.ipp"

LIBFLATSURF_INSTANTIATE_MANY_WRAPPED((LIBFLATSURF_INSTANTIATE_WITH_IMPLEMENTATION), ContourConnection, LIBFLATSURF_SURFACE_TYPES)
