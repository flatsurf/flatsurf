/**********************************************************************
 *  This file is part of flatsurf.
 *
 *        Copyright (C) 2019-2020 Julian Rüth
 *
 *  Flatsurf is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
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

#ifndef LIBFLATSURF_FLATSURF_HPP
#define LIBFLATSURF_FLATSURF_HPP

// Include the headers that define the full API of
// flatsurf (expect for special purpose headers such as
// cppyy.hpp for the Python interface and cereal.hpp for
// serialization with cereal.)

#include "bound.hpp"
#include "ccw.hpp"
#include "chain.hpp"
#include "chain_iterator.hpp"
#include "contour_component.hpp"
#include "contour_connection.hpp"
#include "contour_decomposition.hpp"
#include "copyable.hpp"
#include "decomposition_step.hpp"
#include "delaunay_triangulation.hpp"
#include "edge.hpp"
#include "edge_map.hpp"
#include "edge_set.hpp"
#include "edge_set_iterator.hpp"
#include "flat_triangulation_collapsed.hpp"
#include "flat_triangulation_combinatorial.hpp"
#include "flat_triangulation_combinatorics.hpp"
#include "flat_triangulation.hpp"
#include "flow_component.hpp"
#include "flow_connection.hpp"
#include "flow_decomposition.hpp"
#include "flow_triangulation.hpp"
#include "fmt.hpp"
#include "forward.hpp"
#include "half_edge.hpp"
#include "half_edge_map.hpp"
#include "half_edge_set.hpp"
#include "half_edge_set_iterator.hpp"
#include "interval_exchange_transformation.hpp"
#include "local.hpp"
#include "managed_movable.hpp"
#include "movable.hpp"
#include "odd_half_edge_map.hpp"
#include "orientation.hpp"
#include "path.hpp"
#include "path_iterator.hpp"
#include "permutation.hpp"
#include "saddle_connection.hpp"
#include "saddle_connections_by_length.hpp"
#include "saddle_connections_by_length_iterator.hpp"
#include "saddle_connections.hpp"
#include "saddle_connections_iterator.hpp"
#include "serializable.hpp"
#include "tracked.hpp"
#include "vector.hpp"
#include "vertex.hpp"
#include "vertical.hpp"

#endif
