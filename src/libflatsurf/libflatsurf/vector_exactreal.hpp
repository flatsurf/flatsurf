/**********************************************************************
 *  This file is part of Flatsurf.
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
 *  along with Flatsurf. If not, see <https://www.gnu.org/licenses/>.
 *********************************************************************/

#ifndef LIBFLATSURF_VECTOR_EXACTREAL
#define LIBFLATSURF_VECTOR_EXACTREAL

#include <exact-real/element.hpp>

#include "libflatsurf/libflatsurf.hpp"
#include "libflatsurf/ccw.hpp"
#include "libflatsurf/orientation.hpp"
#include "libflatsurf/vector_along_triangulation_with_approximation.hpp"

namespace flatsurf {
	struct Bound;
	template<typename V>
	struct VectorArbAlongTriangulation;
	struct VectorArb;

	template <typename Ring>
	struct VectorExactReal : boost::additive<VectorExactReal<Ring>>, boost::multipliable<VectorExactReal<Ring>, int>, boost::equality_comparable<VectorExactReal<Ring>> {
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
		bool operator==(const VectorExactReal<Ring>&) const;
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
