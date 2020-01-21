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

#ifndef LIBFLATSURF_FLOW_DECOMPOSITION_IMPL_HPP
#define LIBFLATSURF_FLOW_DECOMPOSITION_IMPL_HPP

#include <memory>

#include "../../flatsurf/flow_decomposition.hpp"

#include "flow_decomposition_state.hpp"

namespace flatsurf {
template <typename Surface>
class Implementation<FlowDecomposition<Surface>> {
 public:
  Implementation(std::unique_ptr<Surface> surface, const Vector<typename Surface::Coordinate>& vertical);

  std::shared_ptr<FlowDecompositionState<Surface>> state;
};

}

#endif
