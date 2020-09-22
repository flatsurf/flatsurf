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

#ifndef LIBFLATSURF_FLOW_DECOMPOSITION_STATE_HPP
#define LIBFLATSURF_FLOW_DECOMPOSITION_STATE_HPP

#include <intervalxt/connection.hpp>
#include <iosfwd>
#include <list>

#include "../../flatsurf/contour_decomposition.hpp"
#include "../../flatsurf/saddle_connection.hpp"
#include "../../flatsurf/vector.hpp"
#include "flow_component_state.hpp"

namespace flatsurf {
template <typename Surface>
class FlowDecompositionState : public std::enable_shared_from_this<FlowDecompositionState<Surface>> {
  using T = typename Surface::Coordinate;

 public:
  FlowDecompositionState(Surface&& surface, const Vector<T>& vert);

  ContourDecomposition<Surface> contourDecomposition;

  std::list<FlowComponentState<Surface>> components;
  std::unordered_map<::intervalxt::Connection, SaddleConnection<FlatTriangulation<T>>> injectedConnections;
  std::unordered_map<::intervalxt::Connection, SaddleConnection<FlatTriangulation<T>>> detectedConnections;

  template <typename S>
  friend std::ostream& operator<<(std::ostream&, const FlowDecompositionState<S>&);
};
}  // namespace flatsurf

#endif
