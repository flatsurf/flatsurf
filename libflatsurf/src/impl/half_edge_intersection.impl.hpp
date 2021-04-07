/**********************************************************************
 *  This file is part of flatsurf.
 *
 *        Copyright (C) 2021 Julian Rüth
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

#ifndef LIBFLATSURF_HALF_EDGE_INTERSECTION_IMPL_HPP
#define LIBFLATSURF_HALF_EDGE_INTERSECTION_IMPL_HPP

#include "../../flatsurf/half_edge.hpp"
#include "../../flatsurf/half_edge_intersection.hpp"
#include "../../flatsurf/vector.hpp"
#include "read_only.hpp"

namespace flatsurf {

template <typename Surface>
class ImplementationOf<HalfEdgeIntersection<Surface>> {
  using T = typename Surface::Coordinate;

 public:
  ImplementationOf(const Surface&, const Vector<T>& base, HalfEdge halfEdge, const Vector<T>& direction);

  // Return the intersection corresponding to the saddle connection with
  // `direction` as it is crossing over `cross`. The half edge is assumed to be
  // anchored in the coordinate origin and the saddle connection to start at
  // `base`.
  static HalfEdgeIntersection<Surface> make(const Surface&, const Vector<T>& base, HalfEdge cross, const Vector<T>& direction);

  // Return the projective coordinates of the intersection point.
  std::tuple<T, T, T> projective() const;

  // Return the coordinate of the intersection point on the half edge as a fraction in [0, 1].
  std::tuple<T, T> relative() const;

  ReadOnly<Surface> surface;
  Vector<T> base;
  Vector<T> direction;
  HalfEdge halfEdge;
};

template <typename Surface>
template <typename... Args>
HalfEdgeIntersection<Surface>::HalfEdgeIntersection(PrivateConstructor, Args&&... args) :
  self(spimpl::make_impl<ImplementationOf<HalfEdgeIntersection>>(std::forward<Args>(args)...)) {}

}  // namespace flatsurf

#endif
