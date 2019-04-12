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

#ifndef LIBPOLYGON_HALF_EDGE_HPP
#define LIBPOLYGON_HALF_EDGE_HPP

#include <stddef.h>
#include <string>
#include <iosfwd>
#include <boost/operators.hpp>

#include "libpolygon/libpolygon.hpp"

namespace flatsurf {
	// Similar to Edge this is a wrapper to get type-safe HalfEdges without any
	// runtime overhead (at least when compiling with -flto.)
	struct HalfEdge : boost::equality_comparable<HalfEdge>, boost::less_than_comparable<HalfEdge> {
		friend struct Edge;
		template<typename T>
		friend struct HalfEdgeMap;
		friend struct FlatTriangulationCombinatorial;
		friend struct Vertex;

		HalfEdge();
		HalfEdge(const HalfEdge& edge) = default;
		explicit HalfEdge(const int id);
		struct Edge;

		HalfEdge operator-() const;
		HalfEdge& operator=(const HalfEdge& other);
		bool operator==(const HalfEdge& other) const;
		bool operator<(const HalfEdge& other) const;
		friend std::ostream& operator<<(std::ostream&, const HalfEdge&);
		Edge edge() const;
	private:
		int id;
	};
}

#endif
