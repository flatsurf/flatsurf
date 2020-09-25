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

#ifndef LIBFLATSURF_CPPYY_HPP
#define LIBFLATSURF_CPPYY_HPP

#include <e-antic/renfxx_fwd.h>

#include <iosfwd>
#include <memory>
#include <stdexcept>

#include "bound.hpp"
#include "chain.hpp"
#include "chain_iterator.hpp"
#include "edge.hpp"
#include "flat_triangulation.hpp"
#include "flat_triangulation_combinatorial.hpp"
#include "flow_component.hpp"
#include "flow_connection.hpp"
#include "flow_decomposition.hpp"
#include "forward.hpp"
#include "half_edge.hpp"
#include "interval_exchange_transformation.hpp"
#include "odd_half_edge_map.hpp"
#include "permutation.hpp"
#include "saddle_connection.hpp"
#include "saddle_connections.hpp"
#include "saddle_connections_by_length.hpp"
#include "saddle_connections_by_length_iterator.hpp"
#include "saddle_connections_iterator.hpp"
#include "vector.hpp"
#include "vertex.hpp"
#include "vertical.hpp"

namespace flatsurf {
// TODO: Test that it actually has trouble.
// cppyy sometimes has trouble with rvalues, let's help it to create a FlatTriangulation
template <typename T>
FlatTriangulation<T> makeFlatTriangulation(const std::vector<std::vector<int>> &vertices, const std::vector<Vector<T>> &vectors) {
  return FlatTriangulation<T>(FlatTriangulationCombinatorial(vertices), vectors);
}

// TODO: Test that it actually has trouble.
// cppyy sometimes has trouble with rvalues, let's help it to create a FlowDecomposition
template <typename T>
FlowDecomposition<FlatTriangulation<T>> makeFlowDecomposition(const FlatTriangulation<T> &surface, const Vector<T> &v) {
  return FlowDecomposition<FlatTriangulation<T>>(surface.clone(), v);
}

// cppyy has trouble with std::function arguments in headers
// See https://github.com/flatsurf/flatsurf/issues/149 for the upstream issue.
template <typename T>
bool decomposeFlowDecomposition(FlowDecomposition<T> &decomposition, int limit = -1) {
  return decomposition.decompose(FlowDecomposition<T>::defaultTarget, limit);
}

// Work around https://bitbucket.org/wlav/cppyy/issues/273/segfault-in-cpycppyy-anonymous-namespace
template <typename T>
auto makeOddHalfEdgeMap(const FlatTriangulationCombinatorial& surface, const std::vector<T>& values) {
  return OddHalfEdgeMap<T>(surface, [&](const HalfEdge &e) {
    return e == Edge(e).positive() ? values.at(Edge(e).index()) : -values.at(Edge(e).index());
  });
}

// The following block of forward declarations is a bit odd. It only exists to
// work around bugs in cppyy.
// See https://bitbucket.org/wlav/cppyy/issues/95/lookup-of-friend-operator

std::ostream &operator<<(std::ostream &, const HalfEdge &);
template <typename T>
std::ostream &operator<<(std::ostream &, const Permutation<T> &);
template <typename T>
std::ostream &operator<<(std::ostream &, const FlatTriangulation<T> &);
// See saddle_connection.hpp for the _ parameter.
template <typename Surface, typename _>
std::ostream &operator<<(std::ostream &, const SaddleConnection<Surface> &);
template <typename Surface>
std::ostream &operator<<(std::ostream &, const SaddleConnections<Surface> &);
template <typename Surface>
std::ostream &operator<<(std::ostream &, const SaddleConnectionsByLength<Surface> &);
template <typename Surface>
std::ostream &operator<<(std::ostream &, const FlowComponent<Surface> &);
std::ostream &operator<<(std::ostream &, const Vertex &);

}  // namespace flatsurf

#endif
