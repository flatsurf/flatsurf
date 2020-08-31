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

#ifndef LIBFLATSURF_CONTOUR_COMPONENT_IMPL_HPP
#define LIBFLATSURF_CONTOUR_COMPONENT_IMPL_HPP

#include <iterator>
#include <memory>
#include <vector>

#include "../../flatsurf/contour_component.hpp"
#include "contour_decomposition.impl.hpp"
#include "contour_decomposition_state.hpp"
#include "forward.hpp"

namespace flatsurf {
template <typename Surface>
class ImplementationOf<ContourComponent<Surface>> {
  using DecompositionState = ContourDecompositionState<Surface>;

 public:
  class ComponentState;

  static void makeContour(std::back_insert_iterator<std::vector<HalfEdge>>, const HalfEdge, const Surface&, const Vertical<Surface>&);

  static ContourConnection<Surface> nextInPerimeter(std::shared_ptr<ContourDecompositionState<Surface>>, ContourComponentState<Surface>* const, HalfEdge);
  static ContourConnection<Surface> previousInPerimeter(std::shared_ptr<ContourDecompositionState<Surface>>, ContourComponentState<Surface>* const, HalfEdge);

  HalfEdge large() const;

  ImplementationOf(std::shared_ptr<ContourDecompositionState<Surface>> state, ContourComponentState<Surface>* component) :
    state(state),
    component(component) {}

  static ContourComponent<Surface> make(std::shared_ptr<ContourDecompositionState<Surface>> state, ContourComponentState<Surface>* component);

  std::shared_ptr<ContourDecompositionState<Surface>> state;
  ContourComponentState<Surface>* const component;
};

}  // namespace flatsurf

#endif
