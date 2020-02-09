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

#ifndef LIBFLATSURF_CONTOUR_DECOMPOSITION_STATE_HPP
#define LIBFLATSURF_CONTOUR_DECOMPOSITION_STATE_HPP

#include <list>
#include <memory>
#include <iosfwd>

#include "../../flatsurf/contour_component.hpp"

#include "contour_component_state.hpp"

namespace flatsurf {

template <typename Surface>
class ContourDecompositionState : public std::enable_shared_from_this<ContourDecompositionState<Surface>> {
  using T = typename Surface::Coordinate;
  using ComponentState = ContourComponentState<Surface>;

 public:
  ContourDecompositionState(std::unique_ptr<Surface> surface, const Vector<T>& vert);

  ContourComponent<Surface> make(ComponentState* component);

  std::shared_ptr<FlatTriangulationCollapsed<T>> surface;
  std::list<ComponentState> components;

  template <typename S>
  friend std::ostream& operator<<(std::ostream&, const ContourDecompositionState<S>&);
};

}  // namespace flatsurf

#endif
