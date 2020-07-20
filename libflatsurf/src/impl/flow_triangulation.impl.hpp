/**********************************************************************
 *  This file is part of flatsurf.
 *
 *        Copyright (C) 2020 Julian Rüth
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

#ifndef LIBFLATSURF_FLOW_TRIANGULATION_IMPL_HPP
#define LIBFLATSURF_FLOW_TRIANGULATION_IMPL_HPP

#include <unordered_map>
#include <memory>

#include "../../flatsurf/flow_connection.hpp"
#include "../../flatsurf/flow_triangulation.hpp"
#include "../../flatsurf/half_edge.hpp"

namespace flatsurf {

template <typename Surface>
class ImplementationOf<FlowTriangulation<Surface>> {
  using T = typename Surface::Coordinate;

 public:
  ImplementationOf(const FlowComponent<Surface>&);

  static FlowTriangulation<Surface> make(const FlowComponent<Surface>&);

  std::shared_ptr<const FlatTriangulation<T>> triangulation;

  std::unordered_map<FlowConnection<Surface>, HalfEdge> perimeter;
};

}  // namespace flatsurf

#endif
