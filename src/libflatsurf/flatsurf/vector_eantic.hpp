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

#ifndef LIBFLATSURF_VECTOR_EANTIC
#define LIBFLATSURF_VECTOR_EANTIC

#include <iosfwd>
#include <boost/operators.hpp>
#include <e-antic/renfxx.h>

#include "flatsurf/flatsurf.hpp"
#include "flatsurf/ccw.hpp"
#include "flatsurf/orientation.hpp"
#include "flatsurf/vector_along_triangulation_with_approximation.hpp"

namespace flatsurf {
	struct Bound;
	template<typename V>
	struct VectorArbAlongTriangulation;
	struct VectorArb;

	struct VectorEAntic : boost::additive<VectorEAntic>, boost::multipliable<VectorEAntic, int>, boost::equality_comparable<VectorEAntic> {
		using V = VectorEAntic;
		friend struct VectorArb;

		VectorEAntic();
		VectorEAntic(const eantic::renf_elem_class& x, const eantic::renf_elem_class& y);

		CCW ccw(const V&) const;
		ORIENTATION orientation(const V&) const;

		friend std::ostream& operator<<(std::ostream& os, const VectorEAntic&);
		V operator-() const;
		bool operator<(const Bound bound) const;
		bool operator>(const Bound bound) const;
		bool operator==(const VectorEAntic&) const;
		explicit operator bool() const;
		V& operator+=(const VectorEAntic& rhs);
		V& operator-=(const VectorEAntic& rhs);
		V& operator*=(const int rhs);
		// Note that the returned VectorArb is only valid as long as this element
		// is unchanged.
		operator const VectorArb&() const;

		using AlongTriangulation = VectorAlongTriangulationWithApproximation<VectorEAntic, VectorArb>;

		private:
		 struct Implementation;
		 spimpl::impl_ptr<Implementation> impl;
	};
}

#endif
