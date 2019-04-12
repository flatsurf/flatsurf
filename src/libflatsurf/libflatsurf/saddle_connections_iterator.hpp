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

#ifndef LIBFLATSURF_SADDLE_CONNECTIONS_ITERATOR_HPP
#define LIBFLATSURF_SADDLE_CONNECTIONS_ITERATOR_HPP

#include <boost/iterator/iterator_facade.hpp>
#include "external/spimpl/spimpl.h"

#include "libflatsurf/libflatsurf.hpp"
#include "libflatsurf/ccw.hpp"
#include "libflatsurf/forward.hpp"
#include "libflatsurf/saddle_connection.hpp"

namespace flatsurf {
	template<typename Vector, typename VectorAlongTriangulation>
	struct SaddleConnectionsIterator : boost::iterator_facade<SaddleConnectionsIterator<Vector, VectorAlongTriangulation>, SaddleConnection<Vector> const, boost::incrementable_traversal_tag> {
		friend class boost::iterator_core_access;
		void increment();
		bool equal(const SaddleConnectionsIterator<Vector, VectorAlongTriangulation>& other) const;
		SaddleConnection<Vector, VectorAlongTriangulation>& dereference() const;

		void skipSector(CCW sector);
	 private:
		friend struct SaddleConnections<Vector, VectorAlongTriangulation>;
		explicit SaddleConnectionsIterator();
		explicit SaddleConnectionsIterator(const SaddleConnections<Vector, VectorAlongTriangulation>& parent);

		struct Implementation;
		spimpl::unique_impl_ptr<Implementation> impl;
	};
}

#endif
