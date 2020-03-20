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

#include "bound.hpp"
#include "edge.hpp"
#include "chain.hpp"
#include "chain_iterator.hpp"
#include "flat_triangulation.hpp"
#include "flat_triangulation_combinatorial.hpp"
#include "flow_component.hpp"
#include "flow_decomposition.hpp"
#include "flow_connection.hpp"
#include "forward.hpp"
#include "half_edge.hpp"
#include "interval_exchange_transformation.hpp"
#include "permutation.hpp"
#include "saddle_connection.hpp"
#include "saddle_connections.hpp"
#include "vector.hpp"
#include "vertex.hpp"
#include "vertical.hpp"

namespace flatsurf {
// cppyy sometimes has trouble with rvalues, let's help it to create a FlatTriangulation
template <typename T>
std::shared_ptr<FlatTriangulation<T>> makeFlatTriangulation(const std::vector<std::vector<int>> &vertices, const std::vector<Vector<T>> &vectors) {
  return std::make_shared<FlatTriangulation<T>>(FlatTriangulationCombinatorial(vertices), vectors);
}

// cppyy gets the lifetime of the surfaces wrong when methods return a unique_ptr<Surface>
// See https://github.com/flatsurf/flatsurf/issues/148 for the upstream issue.
template <typename T>
std::shared_ptr<FlatTriangulation<T>> insertAt(const FlatTriangulation<T> &surface, HalfEdge &e, const Vector<T> &v) {
  return surface.insertAt(e, v);
}

template <typename T>
std::shared_ptr<FlatTriangulation<T>> slot(const FlatTriangulation<T> &surface, HalfEdge e) {
  return surface.slot(e);
}

template <typename T>
FlowDecomposition<FlatTriangulation<T>> makeFlowDecomposition(const FlatTriangulation<T> &surface, const Vector<T> &v) {
  return FlowDecomposition<FlatTriangulation<T>>(surface.clone(), v);
}

// cppyy has trouble with std::function arguments in headers
// See https://github.com/flatsurf/flatsurf/issues/149 for the upstream issue.
template <typename T>
void decomposeFlowDecomposition(FlowDecomposition<T> &decomposition, int limit = -1) {
  decomposition.decompose(FlowDecomposition<T>::defaultTarget, limit);
}

// The following block of forward declarations is a bit odd. It only exists to
// work around bugs in cppyy.

// See https://bitbucket.org/wlav/cppyy/issues/95/lookup-of-friend-operator

std::ostream &operator<<(std::ostream &, const HalfEdge &);
template <typename T>
std::ostream &operator<<(std::ostream &, const Permutation<T> &);
template <typename K, typename V>
std::ostream &operator<<(std::ostream &, const TrackingMap<K, V> &);
template <typename T>
std::ostream &operator<<(std::ostream &, const FlatTriangulation<T> &);
// See saddle_connection.hpp for the _ parameter.
template <typename Surface, typename _>
std::ostream &operator<<(std::ostream &, const SaddleConnection<Surface> &);
template <typename Surface>
std::ostream &operator<<(std::ostream &, const SaddleConnections<Surface> &);
template <typename Surface>
std::ostream &operator<<(std::ostream &, const FlowComponent<Surface> &);

// Work around https://bitbucket.org/wlav/cppyy/issues/96/cannot-make-wrapper-for-a-function
extern template std::ostream &operator<<(std::ostream &, const Permutation<HalfEdge> &);
extern template std::ostream &operator<<(std::ostream &, const FlatTriangulation<long long> &);
extern template std::ostream &operator<<(std::ostream &, const FlatTriangulation<eantic::renf_elem_class> &);
extern template std::ostream &operator<<(std::ostream &, const SaddleConnection<FlatTriangulation<long long>> &);
extern template std::ostream &operator<<(std::ostream &, const SaddleConnection<FlatTriangulation<eantic::renf_elem_class>> &);
extern template std::ostream &operator<<(std::ostream &, const SaddleConnections<FlatTriangulation<long long>> &);
extern template std::ostream &operator<<(std::ostream &, const SaddleConnections<FlatTriangulation<eantic::renf_elem_class>> &);
extern template std::ostream &operator<<(std::ostream &, const FlowComponent<FlatTriangulation<eantic::renf_elem_class>> &);
std::ostream &operator<<(std::ostream &, const Vertex &);

}  // namespace flatsurf

#endif
