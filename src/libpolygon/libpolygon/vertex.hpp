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

#ifndef LIBPOLYGON_VERTEX_HPP
#define LIBPOLYGON_VERTEX_HPP

#include <boost/operators.hpp>

#include "libpolygon/half_edge.hpp"
#include "libpolygon/forward.hpp"

namespace flatsurf {
// A thin wrapper of a HalfEdge that gives us a notion of
// a vertex, i.e., the end point of a half edge.
// We could use HalfEdge instead of Vertex with the
// implied convention that we always mean the starting
// vertex of that half edge. However, it is semantically
// nicer to have a distinct notion. Also this gives us a
// proper operator== for vertices.
struct Vertex : boost::equality_comparable<Vertex> {
	static Vertex source(const HalfEdge&, const FlatTriangulationCombinatorial&);
	static Vertex target(const HalfEdge&, const FlatTriangulationCombinatorial&);

	bool operator==(const Vertex&) const;

	friend std::ostream& operator<<(std::ostream&, const Vertex&);

 private:
	Vertex(const HalfEdge&);

	// The actual vertex is the start of this half edge.
	// Made a unique representative by taking the half edge with this property
	// with the smallest id.
	HalfEdge representative;
};
}

#endif
