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

#ifndef LIBPOLYGON_VECTOR_LONGLONG_HPP
#define LIBPOLYGON_VECTOR_LONGLONG_HPP

#include <iosfwd>
#include <boost/operators.hpp>

#include "libpolygon/libpolygon.hpp"
#include "libpolygon/ccw.hpp"
#include "libpolygon/vector_along_triangulation.hpp"

namespace flatsurf {
	struct Bound;
	template<typename V>
	struct VectorAlongTriangulation;	

	// An exact vector in ℝ² whose coordinates are integers in the range of a long long.
	// For operations such as ccw() to work, the square of the coordinates should
	// still fit into a long long as well.
	struct VectorLongLong : boost::additive<VectorLongLong>, boost::multipliable<VectorLongLong, int>, boost::less_than_comparable<VectorLongLong, Bound> {
		using V = VectorLongLong;

		VectorLongLong();
		VectorLongLong(long long x, long long y);

		CCW ccw(const V& rhs) const;

		friend std::ostream& operator<<(std::ostream&, const V&);
		V operator-() const;
		bool operator>(const Bound) const;
		bool operator<(const Bound) const;
		explicit operator bool() const;
		V& operator+=(const V&);
		V& operator-=(const V&);
		V& operator*=(const int);

		using AlongTriangulation = VectorAlongTriangulation<VectorLongLong>;

		private:
		 long long x;
		 long long y;
	};
}

#endif
