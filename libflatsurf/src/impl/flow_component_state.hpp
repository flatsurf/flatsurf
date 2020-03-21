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

#ifndef LIBFLATSURF_FLOW_COMPONENT_STATE_HPP
#define LIBFLATSURF_FLOW_COMPONENT_STATE_HPP

#include <iosfwd>

#include <list>
#include <memory>

#include <intervalxt/component.hpp>

#include "../../flatsurf/contour_component.hpp"
#include "../../flatsurf/interval_exchange_transformation.hpp"

namespace flatsurf {
template <typename Surface>
class FlowComponentState {
  using T = typename Surface::Coordinate;

 public:
  FlowComponentState(const ContourComponent<Surface>& contour, std::shared_ptr<const IntervalExchangeTransformation<FlatTriangulationCollapsed<T>>> iet, const intervalxt::Component& component);

  ContourComponent<Surface> contourComponent;
  std::shared_ptr<const IntervalExchangeTransformation<FlatTriangulationCollapsed<T>>> iet;
  intervalxt::Component dynamicalComponent;

  template <typename S>
  friend std::ostream& operator<<(std::ostream&, const FlowComponentState<S>&);
};
}  // namespace flatsurf

#endif
