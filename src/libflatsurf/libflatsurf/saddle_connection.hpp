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

#ifndef LIBFLATSURF_SADDLE_CONNECTION_HPP
#define LIBFLATSURF_SADDLE_CONNECTION_HPP

#include <iosfwd>

#include "libflatsurf/libflatsurf.hpp"
#include "libflatsurf/vertex.hpp"

namespace flatsurf {
	template<typename Vector>
	struct FlatTriangulation;

	struct Vertex;

	// TODO: Maybe this should be a pimpl? But this only makes sense if it's
	// constructor is accepting the underlying pimpl. But then the calling code
	// would need to know the pimpl…, i.e., we would have to move the
	// implementation inside saddle_connections_iterator.cc. That makes sense but
	// is breaking a bit with our conventions here. So we might want to include
	// saddle_conection.tcc from saddle_connection_iterator.cc.
	template<typename Vector, typename AlongTriangulation>
	struct SaddleConnection {
		SaddleConnection(const FlatTriangulation<Vector>&, const AlongTriangulation&, const Vertex& source, const Vertex& target);

		template <typename V, typename A>
		friend std::ostream& operator<<(std::ostream&, const SaddleConnection<V, A>&);

		const FlatTriangulation<Vector>& surface;
		const AlongTriangulation vector;
		const Vertex source;
		const Vertex target;
	};
}

#endif

