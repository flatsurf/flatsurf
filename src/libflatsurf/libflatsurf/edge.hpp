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

#ifndef LIBFLATSURF_EDGE_HPP
#define LIBFLATSURF_EDGE_HPP

#include <vector>

#include "libflatsurf/libflatsurf.hpp"

#include "libflatsurf/half_edge.hpp"

namespace flatsurf {
	// An undirected edge in a flatsurf.
	// This type-safe wrapper should be optimized away completely by the
	// compiler (at least when compiling with -flto.)
	// You might find it annoying that you cannot directly create instances of
	// this class or rather use int everywhere instead. The point is that you
	// get a lot of compile-time safety by using such a wrapper instead, e.g.,
	// you cannot add edges but you could add ints which would not make much
	// sense usually.
	struct Edge {
		Edge() = delete;

		static std::vector<Edge> makeEdges(size_t nedges);
		std::pair<HalfEdge, HalfEdge> halfEdges() const;
		size_t index() const;

	private:
		explicit Edge(size_t id);
		const size_t id;
	};
}

#endif

