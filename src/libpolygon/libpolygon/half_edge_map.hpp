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

#ifndef LIBPOLYGON_HALF_EDGE_MAP_HPP
#define LIBPOLYGON_HALF_EDGE_MAP_HPP

#include <iosfwd>
#include <vector>
#include <functional>
#include <boost/operators.hpp>

#include "libpolygon/half_edge.hpp"

namespace flatsurf {
	struct HalfEdge;

	template<typename T>
	struct HalfEdgeMap {
		explicit HalfEdgeMap(const std::vector<T>& data);

		const T& get(const HalfEdge key) const;
		void set(const HalfEdge key, const T& value);
		void apply(std::function<void(HalfEdge, const T&)>) const;

		template<typename S>
		friend std::ostream& operator<<(std::ostream&, const HalfEdgeMap<S>&);

		static size_t index(const HalfEdge);

		private:
		mutable std::vector<T> data;
	};
}

#endif
