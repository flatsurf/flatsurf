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

#include <memory>
#include <list>

#include "contour_decomposition.impl.hpp"
#include "forward.hpp"

namespace flatsurf {

template <typename Surface>
class ContourDecompositionState : public std::enable_shared_from_this<ContourDecompositionState<Surface>> {
  using ComponentState = ContourComponentState<Surface>;
 public:
  using Collapsed = std::conditional_t<std::is_same_v<Surface, FlatTriangulationCollapsed<typename Surface::Coordinate>>, Surface, typename Surface::Collapsed>;

  ContourDecompositionState(std::unique_ptr<Surface> surface, const Vector<typename Surface::Coordinate>& vert);

  ContourComponent<Surface> make(ComponentState* component);

  std::shared_ptr<typename Surface::Collapsed> surface;
  std::list<ComponentState> components;

  template <typename S>
  friend std::ostream& operator<<(std::ostream&, const ContourDecompositionState<S>&);
};

}

#endif
