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

#ifndef LIBFLATSURF_CONTOUR_COMPONENT_IMPL_COMPONENT_STATE_HPP
#define LIBFLATSURF_CONTOUR_COMPONENT_IMPL_COMPONENT_STATE_HPP

#include <list>
#include <memory>
#include <unordered_set>

#include "../../flatsurf/half_edge.hpp"

#include "forward.hpp"

namespace flatsurf {
template <typename Surface>
class ContourComponentState {
  using T = typename Surface::Coordinate;

 public:
  ContourComponentState(const ContourDecompositionState<Surface>&, const std::unordered_set<HalfEdge>&);

  std::unordered_set<HalfEdge> halfEdges;

  HalfEdge large;
  // The edges on the top of this component, from right to left, oriented from right to left.
  std::vector<HalfEdge> topEdges;
  // The edges on the bottom of this component, from left to right, oriented from left to right.
  std::vector<HalfEdge> bottomEdges;

  template <typename S>
  friend std::ostream& operator<<(std::ostream&, const ContourComponentState<S>&);
};
}  // namespace flatsurf

#endif
