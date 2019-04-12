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

#ifndef LIBPOLYGON_FLAT_TRIANGULATION_HPP
#define LIBPOLYGON_FLAT_TRIANGULATION_HPP

#include <iosfwd>
#include <vector>
#include <memory>

#include "libpolygon/libpolygon.hpp"
#include "libpolygon/forward.hpp"
#include "libpolygon/flat_triangulation_combinatorial.hpp"
#include "external/spimpl/spimpl.h"

namespace flatsurf {
	template<typename T>
	struct HalfEdgeMap;

	template<class Vector>
	struct FlatTriangulation : FlatTriangulationCombinatorial {
		FlatTriangulation(FlatTriangulationCombinatorial&&, const std::vector<Vector>& vectors);
		FlatTriangulation(FlatTriangulationCombinatorial&&, const HalfEdgeMap<Vector>& vectors);
		~FlatTriangulation() = default;

		const Vector& fromEdge(const HalfEdge e) const;

		template<typename W>
		friend std::ostream& operator<<(std::ostream&, const FlatTriangulation<W>&);
	 private:
		struct Implementation;
		spimpl::unique_impl_ptr<Implementation> impl;
	};
}

#endif
