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

#include "../../flatsurf/flow_connection.hpp"
#include "../../flatsurf/flow_triangulation.hpp"
#include "../../flatsurf/half_edge.hpp"

#include "read_only.hpp"

namespace flatsurf {

template <typename Surface>
class ImplementationOf<FlowTriangulation<Surface>> {
  using T = typename Surface::Coordinate;

 public:
  ImplementationOf(const FlowComponent<Surface>&);

  static FlowTriangulation<Surface> make(const FlowComponent<Surface>&);

  ReadOnly<FlatTriangulation<T>> triangulation;

  FlowComponent<Surface> component;

  std::unordered_map<FlowConnection<Surface>, HalfEdge> toHalfEdge;
  std::unordered_map<HalfEdge, FlowConnection<Surface>> toConnection;
};

template <typename Surface>
template <typename... Args>
FlowTriangulation<Surface>::FlowTriangulation(PrivateConstructor, Args&&... args) :
  self(spimpl::make_impl<ImplementationOf<FlowTriangulation>>(std::forward<Args>(args)...)) {}

}  // namespace flatsurf

#endif
