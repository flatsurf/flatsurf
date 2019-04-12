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

#ifndef LIBPOLYGON_FLAT_TRIANGULATION_COMBINATORIAL_HPP
#define LIBPOLYGON_FLAT_TRIANGULATION_COMBINATORIAL_HPP

#include <iosfwd>
#include <vector>
#include <memory>
#include "external/spimpl/spimpl.h"

#include "libpolygon/libpolygon.hpp"
#include "libpolygon/forward.hpp"

namespace flatsurf {
	template<typename T>
	struct HalfEdgeMap;

	struct FlatTriangulationCombinatorial {
		FlatTriangulationCombinatorial(const std::vector<std::vector<int>>& vertices);
		FlatTriangulationCombinatorial(const Permutation<HalfEdge>& vertices);
		FlatTriangulationCombinatorial(FlatTriangulationCombinatorial&&);
		~FlatTriangulationCombinatorial() = default;

		HalfEdge nextAtVertex(const HalfEdge e) const;
		HalfEdge nextInFace(const HalfEdge e) const;

		const std::vector<HalfEdge>& edges() const;
		const std::vector<Vertex>& vertices() const;

		friend std::ostream& operator<<(std::ostream&, const FlatTriangulationCombinatorial&);
		const size_t nedges;
	 private:
		struct Implementation;
		spimpl::unique_impl_ptr<Implementation> impl;
	};
}

#endif

