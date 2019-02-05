/**********************************************************************
 *  This file is part of Polygon.
 *
 *        Copyright (C) 2019 Julian Rüth
 *
 *  Polygon is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  Polygon is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Polygon. If not, see <https://www.gnu.org/licenses/>.
 *********************************************************************/

#ifndef LIBPOLYGON_SADDLE_CONNECTIONS_HPP
#define LIBPOLYGON_SADDLE_CONNECTIONS_HPP

#include "external/spimpl/spimpl.h"

#include "libpolygon/libpolygon.hpp"
#include "libpolygon/forward.hpp"
#include "libpolygon/saddle_connections_iterator.hpp"
#include "libpolygon/half_edge.hpp"
#include "libpolygon/bound.hpp"
#include "libpolygon/vertex.hpp"

// TODO: make this a pimpl
namespace flatsurf {
	template<typename Vector, typename VectorAlongTriangulation>
	struct SaddleConnections {
		using Surface = FlatTriangulation<Vector>;

		// The saddle connections that are starting at the source of sectorBegin
		// and lie in the sector between sectorBegin and the follow half edge in
		// counter-clockwise order.
		SaddleConnections(const Surface&, const Bound searchRadius, const HalfEdge sectorBegin);
		// The saddle connections that are starting at source and are in the
		// sector between sectorBegin and sectorEnd; these two must be connected
		// by a half edge, sectorBoundary.
		SaddleConnections(const Surface& surface, const Bound searchRadius, const Vertex& source, const HalfEdge sectorBoundary, const VectorAlongTriangulation& sectorBegin, const VectorAlongTriangulation& sectorEnd);

		SaddleConnectionsIterator<Vector, VectorAlongTriangulation> begin() const;
		SaddleConnectionsIterator<Vector, VectorAlongTriangulation> end() const;

	private:
		friend struct SaddleConnectionsIterator<Vector, VectorAlongTriangulation>;
	  const FlatTriangulation<Vector>& surface;
		const Bound searchRadius;
		const Vertex source;
	  const HalfEdge sectorBoundary;
	  const VectorAlongTriangulation sector[2];
	};
}

#endif
