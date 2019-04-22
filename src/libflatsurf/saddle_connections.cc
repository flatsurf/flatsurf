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

#include "flatsurf/flat_triangulation.hpp"
#include "flatsurf/half_edge.hpp"
#include "flatsurf/saddle_connections.hpp"

namespace flatsurf {
template <typename Vector, typename VectorAlongTriangulation>
struct SaddleConnections<Vector, VectorAlongTriangulation>::Implementation {
  Implementation(const Surface &surface, const Bound searchRadius,
                 const HalfEdge sectorBoundary,
                 const VectorAlongTriangulation &sectorBegin,
                 const VectorAlongTriangulation &sectorEnd)
      : surface(&surface),
        searchRadius(searchRadius),
        sectorBoundary(sectorBoundary),
        sector{sectorBegin, sectorEnd} {
    assert(static_cast<Vector>(sectorBegin) +
                   surface.fromEdge(sectorBoundary) ==
               static_cast<Vector>(sectorEnd) &&
           "sectorBoundary does not connect sectorBegin and sectorEnd");
  }

  FlatTriangulation<Vector> const *surface;
  Bound searchRadius;
  HalfEdge sectorBoundary;
  VectorAlongTriangulation sector[2];
};

template <typename Vector, typename VectorAlongTriangulation>
SaddleConnections<Vector, VectorAlongTriangulation>::SaddleConnections(
    const Surface &surface, const Bound searchRadius,
    const HalfEdge sectorBoundary, const VectorAlongTriangulation &sectorBegin,
    const VectorAlongTriangulation &sectorEnd)
    : impl(spimpl::make_impl<Implementation>(
          surface, searchRadius, sectorBoundary, sectorBegin, sectorEnd)) {}

template <typename Vector, typename VectorAlongTriangulation>
SaddleConnections<Vector, VectorAlongTriangulation>::SaddleConnections(
    const Surface &surface, const Bound searchRadius,
    const HalfEdge sectorBegin)
    : SaddleConnections(
          surface, searchRadius, surface.nextInFace(sectorBegin),
          VectorAlongTriangulation(surface) + sectorBegin,
          VectorAlongTriangulation(surface) -
              surface.nextInFace(surface.nextInFace(sectorBegin))) {}

template <typename Vector, typename VectorAlongTriangulation>
SaddleConnectionsIterator<Vector, VectorAlongTriangulation>
SaddleConnections<Vector, VectorAlongTriangulation>::begin() const {
  return SaddleConnectionsIterator<Vector, VectorAlongTriangulation>(
      *impl->surface, impl->searchRadius, impl->sectorBoundary, impl->sector[0],
      impl->sector[1]);
}

template <typename Vector, typename VectorAlongTriangulation>
SaddleConnectionsIterator<Vector, VectorAlongTriangulation>
SaddleConnections<Vector, VectorAlongTriangulation>::end() const {
  return SaddleConnectionsIterator<Vector, VectorAlongTriangulation>();
}
}  // namespace flatsurf

// Instantiations of templates so implementations are generated for the linker
#include <exact-real/number_field_traits.hpp>

#include "flatsurf/vector_eantic.hpp"
#include "flatsurf/vector_exactreal.hpp"
#include "flatsurf/vector_longlong.hpp"
using namespace flatsurf;

template struct flatsurf::SaddleConnections<VectorLongLong>;
template struct flatsurf::SaddleConnections<VectorEAntic>;
template struct flatsurf::SaddleConnections<
    VectorEAntic, VectorAlongTriangulation<VectorEAntic>>;
template struct flatsurf::SaddleConnections<
    VectorExactReal<exactreal::NumberFieldTraits>>;
