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
#ifndef LIBFLATSURF_VECTOR_ALONG_TRIANGULATION
#define LIBFLATSURF_VECTOR_ALONG_TRIANGULATION

#include <iosfwd>
#include <boost/operators.hpp>
#include "external/spimpl/spimpl.h"

#include "libflatsurf/ccw.hpp"

namespace flatsurf {
	template<typename Vector>
	struct FlatTriangulation;
	struct HalfEdge;
	struct Bound;
	template<typename T>
	struct HalfEdgeMap;

	// A generic implementation of a vector in ℝ² that is a sum of half edges of a flat triangulation.
	template<typename V>
	struct VectorAlongTriangulation : boost::additive<VectorAlongTriangulation<V>>, boost::additive<VectorAlongTriangulation<V>, HalfEdge>, boost::less_than_comparable<VectorAlongTriangulation<V>, Bound>, boost::equality_comparable<VectorAlongTriangulation<V>> {
			using Surface = FlatTriangulation<V>;

			explicit VectorAlongTriangulation(const Surface& surface);
			VectorAlongTriangulation(const Surface& surface, const HalfEdgeMap<int>& coefficients);

			CCW ccw(const VectorAlongTriangulation<V>& rhs) const;

		  template<typename W>
			friend std::ostream& operator<<(std::ostream&, const VectorAlongTriangulation<W>&);
			bool operator>(const Bound bound) const;
			bool operator<(const Bound bound) const;
			VectorAlongTriangulation<V>& operator+=(const HalfEdge);
			VectorAlongTriangulation<V>& operator-=(const HalfEdge);
			VectorAlongTriangulation<V>& iadd(const HalfEdge, const int);
			bool operator==(const VectorAlongTriangulation<V>&);
			operator V() const;
		 private:
		  struct Implementation;
		  spimpl::impl_ptr<Implementation> impl;
		};
}

#endif
