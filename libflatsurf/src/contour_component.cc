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

#include <intervalxt/interval_exchange_transformation.hpp>

#include <fmt/format.h>
#include <fmt/ostream.h>

#include "external/rx-ranges/include/rx/ranges.hpp"

#include "../flatsurf/flat_triangulation_collapsed.hpp"
#include "../flatsurf/vertical.hpp"
#include "../flatsurf/half_edge.hpp"
#include "../flatsurf/contour_connection.hpp"
#include "../flatsurf/saddle_connection.hpp"

#include "impl/contour_component.impl.hpp"
#include "impl/contour_connection.impl.hpp"
#include "impl/contour_component_state.hpp"
#include "impl/contour_decomposition_state.hpp"

#include "util/assert.ipp"

using std::ostream;
using std::vector;
using std::list;

namespace flatsurf {

template <typename Surface>
ContourComponent<Surface>::ContourComponent() {
  // we assume that the caller is aware that impl needs to be initialized.
}

template <typename Surface>
IntervalExchangeTransformation<typename Surface::Collapsed> ContourComponent<Surface>::intervalExchangeTransformation() const {
  return IntervalExchangeTransformation<typename Surface::Collapsed>(impl->state->surface, impl->state->surface->vertical().vertical(), impl->large());
}

template <typename Surface>
std::list<ContourConnection<Surface>> ContourComponent<Surface>::top() const {
  vector<HalfEdge> topEdges;
  
  ::flatsurf::Implementation<ContourComponent<typename ContourDecompositionState<Surface>::Collapsed>>::makeContour(back_inserter(topEdges), impl->large(), *impl->state->surface, impl->state->surface->vertical());

  auto top = topEdges | rx::transform([&](const HalfEdge e) {
    return -::flatsurf::Implementation<ContourConnection<Surface>>::make(impl->state, *this, e);
  }) | rx::reverse() | rx::to_list();

  ASSERT(std::all_of(top.begin(), top.end(), [&](const auto& connection) {
    return impl->state->surface->vertical().perpendicular(connection.connection()) < 0;
  }), "top connections must be negative, i.e., from right to left");

  return top;
}

template <typename Surface>
std::list<ContourConnection<Surface>> ContourComponent<Surface>::bottom() const {
  vector<HalfEdge> bottomEdges;
  
  ::flatsurf::Implementation<ContourComponent<typename ContourDecompositionState<Surface>::Collapsed>>::makeContour(back_inserter(bottomEdges), -impl->large(), *impl->state->surface, -impl->state->surface->vertical());

  auto bottom = bottomEdges | rx::reverse() | rx::transform([&](const HalfEdge e) {
    return ::flatsurf::Implementation<ContourConnection<Surface>>::make(impl->state, *this, -e);
  }) | rx::to_list();

  ASSERT(std::all_of(bottom.begin(), bottom.end(), [&](const auto& connection) {
    return impl->state->surface->vertical().perpendicular(connection.connection()) > 0;
  }), "bottom connections must be positive, i.e., from left to right");

  return bottom;
}

template <typename Surface>
std::list<ContourConnection<Surface>> ContourComponent<Surface>::perimeter() const {
  auto perimeter = bottom();
  for (auto connection : top()) perimeter.push_back(connection);
  return perimeter;
}

template <typename Surface>
HalfEdge Implementation<ContourComponent<Surface>>::large() const {
  auto vertical = state->surface->vertical();
  HalfEdge large = *std::find_if(component->component.begin(), component->component.end(), [&](HalfEdge e) {
   return vertical.large(e);
  });

  if (vertical.perpendicular(state->surface->fromEdge(large)) < 0)
    large = -large;

  return large;
}

template <typename Surface>
ContourComponent<Surface> Implementation<ContourComponent<Surface>>::make(std::shared_ptr<ContourDecompositionState<Surface>> state, ContourComponentState<Surface>* component) {
  ContourComponent<Surface> ret;
  ret.impl = spimpl::make_impl<Implementation>(state, component);
  return ret;
}

template <typename Surface>
void Implementation<ContourComponent<Surface>>::makeContour(std::back_insert_iterator<vector<HalfEdge>> target,
                 const HalfEdge source, const Surface& parent,
                 const Vertical<Surface>& vertical) {
  ASSERT_ARGUMENT(!vertical.parallel(source), "vertical edges must have been collapsed before a contour can be built");
  ASSERT_ARGUMENT(vertical.perpendicular(parent.fromEdge(source)) > 0, "contour must procede in positive direction");
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
  return impl->component->component == rhs.impl->component->component
    && impl->state->surface == rhs.impl->state->surface;
}

template <typename Surface>
ostream& operator<<(ostream& os, const ContourComponent<Surface>& self) {
  return os << "ContourComponent(" << fmt::format("{}", fmt::join(self.perimeter() | rx::transform([&](const auto& connection) { return fmt::format("{}", connection); }) | rx::to_vector(), "→")) << ")";
}

}


// Instantiations of templates so implementations are generated for the linker

#include "util/instantiate.ipp"

LIBFLATSURF_INSTANTIATE_MANY_WRAPPED((LIBFLATSURF_INSTANTIATE_WITH_IMPLEMENTATION), ContourComponent, LIBFLATSURF_SURFACE_TYPES)
