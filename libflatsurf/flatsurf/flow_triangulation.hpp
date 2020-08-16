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

#ifndef LIBFLATSURF_FLOW_TRIANGULATION_HPP
#define LIBFLATSURF_FLOW_TRIANGULATION_HPP

#include "copyable.hpp"
#include "forward.hpp"

namespace flatsurf {

// An immutable triangulation of a component of a flow decomposition.
template <typename Surface>
class FlowTriangulation {
  static_assert(std::is_same_v<Surface, std::decay_t<Surface>>, "type must not have modifiers such as const");

  using T = typename Surface::Coordinate;

  // Flow triangulations cannot be created directly (other than copying & moving them.)
  // They are created with FlowComponent::triangulation()
  template <typename... Args>
  FlowTriangulation(PrivateConstructor, Args&&... args);

 public:
  std::shared_ptr<const FlatTriangulation<T>> triangulation() const;

  FlowComponent<Surface> component() const;

  HalfEdge halfEdge(const FlowConnection<Surface>&) const;

  // Return a map from the half edges in triangulation() to half edges in the
  // flow decomposition's full triangulation.
  HalfEdgeMap<HalfEdge> embedding() const;

  // Return whether two trangulations are identical, i.e., triangulating the
  // same component in the same way.
  bool operator==(const FlowTriangulation&) const;

  template <typename S>
  friend std::ostream& operator<<(std::ostream&, const FlowTriangulation<S>&);

 private:
  using Implementation = ImplementationOf<FlowTriangulation>;
  Copyable<Implementation> impl;

  friend Implementation;
};

}  // namespace flatsurf

#endif
