/**********************************************************************
 *  This file is part of flatsurf.
 *
 *        Copyright (C) 2019 Julian Rüth
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
#include "flat_triangulation.hpp"
#include "flat_triangulation_combinatorial.hpp"
#include "flow_component.hpp"
#include "flow_decomposition.hpp"
#include "half_edge.hpp"
#include "interval_exchange_transformation.hpp"
#include "permutation.hpp"
#include "saddle_connection.hpp"
#include "saddle_connections.hpp"
#include "vector.hpp"
#include "vertex.hpp"

namespace flatsurf {
// cppyy sometimes has trouble with rvalues, let's help it to create a FlatTriangulation
template <typename T, typename V>
auto makeFlatTriangulation(const T &vertices, const std::vector<V> &vectors) {
  return std::make_shared<FlatTriangulation<T>>(FlatTriangulationCombinatorial(vertices), vectors);
}

// cppyy gets the lifetime of the surfaces wrong when methods return a unique_ptr<Surface>
// TODO: Report this upstream
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
// TODO: Report this upstream
template <typename T>
void decomposeFlowDecomposition(FlowDecomposition<T> &decomposition, int limit = -1) {
  decomposition.decompose();
}

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
