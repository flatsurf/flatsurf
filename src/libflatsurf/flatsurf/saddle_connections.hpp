/**********************************************************************
 *  This file is part of flatsurf.
 *
 *        Copyright (C) 2019 Julian Rüth
 *
 *  Flatsurf is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 of the License, or
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

#ifndef LIBFLATSURF_SADDLE_CONNECTIONS_HPP
#define LIBFLATSURF_SADDLE_CONNECTIONS_HPP

#include "external/spimpl/spimpl.h"

#include "flatsurf/bound.hpp"
#include "flatsurf/forward.hpp"
#include "flatsurf/half_edge.hpp"
#include "flatsurf/saddle_connections_iterator.hpp"
#include "flatsurf/vertex.hpp"

namespace flatsurf {
template <typename Vector, typename VectorAlongTriangulation>
class SaddleConnections {
 public:
  using Surface = FlatTriangulation<Vector>;

  // The saddle connections that are starting at the source of sectorBegin
  // and lie in the sector between sectorBegin and the follow half edge in
  // counter-clockwise order.
  SaddleConnections(const Surface &, Bound searchRadius, HalfEdge sectorBegin);
  // The saddle connections that are crossing sectorBoundary and are in the
  // sector between sectorBegin and sectorEnd; these two must be connected
  // by a half edge, sectorBoundary.
  SaddleConnections(const Surface &surface, const Bound searchRadius,
                    const HalfEdge sectorBoundary,
                    const VectorAlongTriangulation &sectorBegin,
                    const VectorAlongTriangulation &sectorEnd);

  SaddleConnectionsIterator<Vector, VectorAlongTriangulation> begin() const;
  SaddleConnectionsIterator<Vector, VectorAlongTriangulation> end() const;

 private:
  friend SaddleConnectionsIterator<Vector, VectorAlongTriangulation>;
  class Implementation;
  spimpl::impl_ptr<Implementation> impl;
};

template <typename Vector>
SaddleConnections(const FlatTriangulation<Vector> &, Bound, HalfEdge)
    ->SaddleConnections<Vector, typename Vector::AlongTriangulation>;
}  // namespace flatsurf

#endif
