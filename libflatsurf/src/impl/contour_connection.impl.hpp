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

#ifndef LIBFLATSURF_CONTOUR_CONNECTION_IMPL_HPP
#define LIBFLATSURF_CONTOUR_CONNECTION_IMPL_HPP

#include "../../flatsurf/contour_connection.hpp"
#include "../../flatsurf/half_edge.hpp"
#include "forward.hpp"

namespace flatsurf {

template <typename Surface>
class ImplementationOf<ContourConnection<Surface>> {
  using T = typename Surface::Coordinate;

 public:
  ImplementationOf(std::shared_ptr<ContourDecompositionState<Surface>>, ContourComponentState<Surface>* const component, HalfEdge, bool top);

  static ContourConnection<Surface> makeTop(std::shared_ptr<ContourDecompositionState<Surface>>, ContourComponentState<Surface>* const component, HalfEdge);
  static ContourConnection<Surface> makeBottom(std::shared_ptr<ContourDecompositionState<Surface>>, ContourComponentState<Surface>* const component, HalfEdge);

  // Return the collapsed vertical connections that need to be crossed when
  // turning clockwise from from (i.e., nextInFace(from)) to to.
  static Path<FlatTriangulation<T>> turn(const ContourConnection<Surface>& from, const ContourConnection<Surface>& to);

  // Return the collapsed vertical connections that need to be crossed when going from from to to.
  // The connections are split in two buckets of continuous connections, the
  // ones attached to from and the ones attached to to.
  // The connections are oriented as they show up in the turn, i.e., you can
  // walk the connections on the interior of the contour turning clockwise
  // around the vertices.
  static std::pair<Path<FlatTriangulation<T>>, Path<FlatTriangulation<T>>> cross(const ContourConnection<Surface>& from, const ContourConnection<Surface>& to);

  std::shared_ptr<ContourDecompositionState<Surface>> state;
  ContourComponentState<Surface>* const component;
  // The half edge describing this connection going from left to right if on
  // the bottom, otherwise going from right to left.
  HalfEdge halfEdge;
  // Whether this is part of the top or of the bottom contour.
  enum class Contour {
    TOP,
    BOTTOM,
  } contour;
};

}  // namespace flatsurf

#endif
