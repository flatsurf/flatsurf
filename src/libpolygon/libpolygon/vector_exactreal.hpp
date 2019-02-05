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

#ifndef LIBPOLYGON_VECTOR_EXACTREAL
#define LIBPOLYGON_VECTOR_EXACTREAL

#include <exact-real/element.hpp>

#include "libpolygon/libpolygon.hpp"
#include "libpolygon/ccw.hpp"
#include "libpolygon/orientation.hpp"
#include "libpolygon/vector_along_triangulation_with_approximation.hpp"

namespace flatsurf {
	struct Bound;
	template<typename V>
	struct VectorArbAlongTriangulation;
	struct VectorArb;

	template <typename Ring>
	struct VectorExactReal : boost::additive<VectorExactReal<Ring>>, boost::multipliable<VectorExactReal<Ring>, int> {
		using V = VectorExactReal;
		friend struct VectorArb;

    VectorExactReal();
		VectorExactReal(const exactreal::Element<Ring>& x, const exactreal::Element<Ring>& y);

		CCW ccw(const V&) const;
		ORIENTATION orientation(const V&) const;

		template<typename R>
		friend std::ostream& operator<<(std::ostream& os, const VectorExactReal<R>&);
		V operator-() const;
		bool operator<(const Bound bound) const;
		bool operator>(const Bound bound) const;
		explicit operator bool() const;
		V& operator+=(const VectorExactReal<Ring>& rhs);
		V& operator-=(const VectorExactReal<Ring>& rhs);
		V& operator*=(const int rhs);
		// Note that the returned VectorArb is only valid as long as this element
		// is unchanged.
		operator const VectorArb&() const;

		using AlongTriangulation = VectorAlongTriangulationWithApproximation<VectorExactReal<Ring>, VectorArb>;

		private:
		 struct Implementation;
		 spimpl::impl_ptr<Implementation> impl;
	};
}

#endif
