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

#include <fmt/format.h>
#include <fmt/ostream.h>

#include <intervalxt/interval_exchange_transformation.hpp>
#include <ostream>
#include <vector>

#include "../flatsurf/ccw.hpp"
#include "../flatsurf/contour_connection.hpp"
#include "../flatsurf/flat_triangulation_collapsed.hpp"
#include "../flatsurf/half_edge.hpp"
#include "../flatsurf/interval_exchange_transformation.hpp"
#include "../flatsurf/orientation.hpp"
#include "../flatsurf/path.hpp"
#include "../flatsurf/path_iterator.hpp"
#include "../flatsurf/saddle_connection.hpp"
#include "../flatsurf/vertical.hpp"
#include "external/rx-ranges/include/rx/ranges.hpp"
#include "impl/contour_component.impl.hpp"
#include "impl/contour_component_state.hpp"
#include "impl/contour_connection.impl.hpp"
#include "impl/contour_decomposition_state.hpp"
#include "util/assert.ipp"

namespace flatsurf {

using std::begin;
using std::end;

template <typename Surface>
IntervalExchangeTransformation<FlatTriangulationCollapsed<typename Surface::Coordinate>> ContourComponent<Surface>::intervalExchangeTransformation() const {
  return IntervalExchangeTransformation(self->state->surface, self->state->surface.vertical(), self->large());
}

template <typename Surface>
std::vector<ContourConnection<Surface>> ContourComponent<Surface>::topContour() const {
  return self->component->topEdges | rx::transform([&](const HalfEdge e) {
    return ImplementationOf<ContourConnection<Surface>>::makeTop(self->state, self->component, e);
  }) | rx::to_vector();
}

template <typename Surface>
Path<FlatTriangulation<typename Surface::Coordinate>> ContourComponent<Surface>::top() const {
  return topContour() | rx::transform([&](const auto& contourConnection) { return contourConnection.perimeter() | rx::to_vector(); }) | rx::to_vector() | rx::flatten<1>() | rx::to_vector();
}

template <typename Surface>
std::vector<ContourConnection<Surface>> ContourComponent<Surface>::bottomContour() const {
  return self->component->bottomEdges | rx::transform([&](const HalfEdge e) {
    return ImplementationOf<ContourConnection<Surface>>::makeBottom(self->state, self->component, e);
  }) | rx::to_vector();
}

template <typename Surface>
Path<FlatTriangulation<typename Surface::Coordinate>> ContourComponent<Surface>::bottom() const {
  return bottomContour() | rx::transform([&](const auto& contourConnection) { return contourConnection.perimeter() | rx::to_vector(); }) | rx::to_vector() | rx::flatten<1>() | rx::to_vector();
}

template <typename Surface>
std::vector<ContourConnection<Surface>> ContourComponent<Surface>::perimeterContour() const {
  auto perimeter = bottomContour();
  for (auto connection : topContour()) perimeter.push_back(connection);

  return perimeter;
}

template <typename Surface>
Path<FlatTriangulation<typename Surface::Coordinate>> ContourComponent<Surface>::perimeter() const {
  Path<FlatTriangulation<T>> perimeter = rx::chain(bottom(), top()) | rx::to_vector();
  LIBFLATSURF_ASSERT(perimeter.closed(), "Perimeter of a component must be closed but " << perimeter << " is not.");

  LIBFLATSURF_ASSERTIONS([&]() {
    auto a = perimeter.begin();

    auto b = a;
    ++b;

    for (; b != perimeter.end(); a++, b++) {
      const int angle = b->angle(-*a);
      LIBFLATSURF_ASSERT(angle == 0 || (angle == 1 && (-*b).vector().ccw(*a) == CCW::COLLINEAR && (-*b).vector().orientation(*a) == ORIENTATION::SAME), "Connections in perimeter must be turning clockwise by an angle in (0, 2π] but " << *b << " follows " << *a << " in perimeter.");
    }

    LIBFLATSURF_ASSERT(perimeter.simple(), "Perimeter of a component must be simple but " << perimeter << " is not.");
  });
  return perimeter;
}

template <typename Surface>
HalfEdge ImplementationOf<ContourComponent<Surface>>::large() const {
  return component->large;
}

template <typename Surface>
typename Surface::Coordinate ContourComponent<Surface>::area() const {
  // For some strange reason we return 6 times the area here. Probably nobody
  // is using this but since it was undocumented what this method does in the
  // first place, let's keep this deprecated method's behavior.
  return 3 * area2();
}

template <typename Surface>
typename Surface::Coordinate ContourComponent<Surface>::area2() const {
  return Vector<T>::area2(perimeter() | rx::transform([&](const auto& connection) { return connection.vector(); }) | rx::to_vector());
}

template <typename Surface>
ContourComponent<Surface> ImplementationOf<ContourComponent<Surface>>::make(std::shared_ptr<ContourDecompositionState<Surface>> state, ContourComponentState<Surface>* component) {
  return ContourComponent<Surface>(PrivateConstructor{}, state, component);
}

template <typename Surface>
ContourConnection<Surface> ImplementationOf<ContourComponent<Surface>>::nextInPerimeter(std::shared_ptr<ContourDecompositionState<Surface>> state, ContourComponentState<Surface>* const component, HalfEdge e) {
  if (e == *rbegin(component->bottomEdges)) {
    return ::flatsurf::ImplementationOf<ContourConnection<Surface>>::makeTop(state, component, *begin(component->topEdges));
  }

  if (e == *rbegin(component->topEdges)) {
    return ::flatsurf::ImplementationOf<ContourConnection<Surface>>::makeBottom(state, component, *begin(component->bottomEdges));
  }

  auto bottom = std::find(begin(component->bottomEdges), end(component->bottomEdges), e);
  if (bottom != end(component->bottomEdges)) {
    return ::flatsurf::ImplementationOf<ContourConnection<Surface>>::makeBottom(state, component, *++bottom);
  }

  auto top = std::find(begin(component->topEdges), end(component->topEdges), e);
  if (top != end(component->topEdges)) {
    return ::flatsurf::ImplementationOf<ContourConnection<Surface>>::makeTop(state, component, *++top);
  }

  throw std::logic_error("Half Edge not in this component");
}

template <typename Surface>
ContourConnection<Surface> ImplementationOf<ContourComponent<Surface>>::previousInPerimeter(std::shared_ptr<ContourDecompositionState<Surface>> state, ContourComponentState<Surface>* const component, HalfEdge e) {
  if (e == *begin(component->bottomEdges)) {
    return ::flatsurf::ImplementationOf<ContourConnection<Surface>>::makeTop(state, component, *rbegin(component->topEdges));
  }

  if (e == *begin(component->topEdges)) {
    return ::flatsurf::ImplementationOf<ContourConnection<Surface>>::makeBottom(state, component, *rbegin(component->bottomEdges));
  }

  auto bottom = std::find(begin(component->bottomEdges), end(component->bottomEdges), e);
  if (bottom != end(component->bottomEdges)) {
    return ::flatsurf::ImplementationOf<ContourConnection<Surface>>::makeBottom(state, component, *--bottom);
  }

  auto top = std::find(begin(component->topEdges), end(component->topEdges), e);
  if (top != end(component->topEdges)) {
    return ::flatsurf::ImplementationOf<ContourConnection<Surface>>::makeTop(state, component, *--top);
  }

  throw std::logic_error("Half Edge not in this component");
}

template <typename Surface>
void ImplementationOf<ContourComponent<Surface>>::makeContour(std::back_insert_iterator<std::vector<HalfEdge>> target,
    const HalfEdge source, const Surface& parent,
    const Vertical<Surface>& vertical) {
  LIBFLATSURF_ASSERT_ARGUMENT(vertical.ccw(source) != CCW::COLLINEAR, "vertical edges must have been collapsed before a contour can be built");
  LIBFLATSURF_ASSERT_ARGUMENT(vertical.ccw(source) == CCW::CLOCKWISE, "contour must proceed in positive direction");
  switch (vertical.classifyFace(source)) {
    case Vertical<Surface>::TRIANGLE::BACKWARD:
      // In a backward triangle, we recurse into both edges on the top.
      makeContour(target, -parent.previousInFace(source), parent, vertical);
      makeContour(target, -parent.nextInFace(source), parent, vertical);
      break;
    case Vertical<Surface>::TRIANGLE::FORWARD:
    case Vertical<Surface>::TRIANGLE::COLLAPSED_TO_TWO_FACES:
      // In a forward triangle, we backtrack but log the edge we just crossed.
      target = source;
      break;
    default:
      // Having vertical edges around complicates things a lot. We had this in
      // the code at some point. Basically, you need to select a representative
      // of any partial cylinder to get a consistent top & bottom half edge.
      throw std::logic_error("not implemented: Contour cannot be created when there are vertical edges. Use a FlatTriangulationCollapsed() to eliminate vertical edges.");
  }
}

template <typename Surface>
bool ContourComponent<Surface>::operator==(const ContourComponent<Surface>& rhs) const {
  if (self->component == rhs.self->component)
    return true;

  if (self->state == rhs.self->state)
    return false;

  return self->state->surface == rhs.self->state->surface && self->component->halfEdges == rhs.self->component->halfEdges;
}

template <typename Surface>
std::ostream& operator<<(std::ostream& os, const ContourComponent<Surface>& self) {
  return os << self.perimeter();
}

}  // namespace flatsurf

// Instantiations of templates so implementations are generated for the linker

#include "util/instantiate.ipp"

LIBFLATSURF_INSTANTIATE_MANY_WRAPPED((LIBFLATSURF_INSTANTIATE_WITH_IMPLEMENTATION), ContourComponent, LIBFLATSURF_SURFACE_TYPES)
