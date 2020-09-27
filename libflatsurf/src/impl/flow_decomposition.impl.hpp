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

#include "../../flatsurf/flow_decomposition.hpp"
#include "flow_decomposition_state.hpp"

namespace flatsurf {
template <typename Surface>
class ImplementationOf<FlowDecomposition<Surface>> {
  using T = typename Surface::Coordinate;

 public:
  ImplementationOf(Surface&& surface, const Vector<T>& vertical);
  ImplementationOf(std::shared_ptr<FlowDecompositionState<Surface>>);

  static FlowDecomposition<Surface> make(std::shared_ptr<FlowDecompositionState<Surface>>);

  static Edge firstInnerEdge(const FlowComponent<Surface>&);
  static HalfEdge halfEdge(const FlowConnection<Surface>&);

  std::shared_ptr<FlowDecompositionState<Surface>> state;
};

template <typename Surface>
template <typename... Args>
FlowDecomposition<Surface>::FlowDecomposition(PrivateConstructor, Args&&... args) :
  self(spimpl::make_unique_impl<ImplementationOf<FlowDecomposition>>(std::forward<Args>(args)...)) {}

}  // namespace flatsurf

#endif
