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

#include "impl/flow_component_state.hpp"

#include <ostream>

namespace flatsurf {

template <typename Surface>
FlowComponentState<Surface>::FlowComponentState(const ContourComponent<Surface>& contour, std::shared_ptr<const IntervalExchangeTransformation<FlatTriangulationCollapsed<T>>> iet, const intervalxt::Component& component) :
  contourComponent(contour),
  iet(iet),
  dynamicalComponent(component) {
}

template <typename Surface>
std::ostream& operator<<(std::ostream&, const FlowComponentState<Surface>&) {
  throw std::logic_error("not implemented: ostream << FlowComponentState");
}

}  // namespace flatsurf

// Instantiations of templates so implementations are generated for the linker
#include "util/instantiate.ipp"

LIBFLATSURF_INSTANTIATE_MANY_WRAPPED((LIBFLATSURF_INSTANTIATE_WITHOUT_IMPLEMENTATION), FlowComponentState, LIBFLATSURF_FLAT_TRIANGULATION_TYPES)
