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

#include <iosfwd>

#include "flatsurf.hpp"

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

// TODO: Test that this is actually a problem.
// cppyy has trouble with std::function arguments in headers
// See https://github.com/flatsurf/flatsurf/issues/149 for the upstream issue.
template <typename T>
bool decomposeFlowDecomposition(FlowDecomposition<T> &decomposition, int limit = -1) {
  return decomposition.decompose(FlowDecomposition<T>::defaultTarget, limit);
}

// TODO: Test that this is actually a problem.
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
// TODO: Check whether this is still needed with the latest cppyy.

std::ostream &operator<<(std::ostream &, const HalfEdge &);
template <typename T>
std::ostream &operator<<(std::ostream &, const Permutation<T> &);
template <typename T>
std::ostream &operator<<(std::ostream &, const FlatTriangulation<T> &);
template <typename Surface>
std::ostream &operator<<(std::ostream &, const SaddleConnections<Surface> &);
template <typename Surface>
std::ostream &operator<<(std::ostream &, const SaddleConnectionsByLength<Surface> &);
template <typename Surface>
std::ostream &operator<<(std::ostream &, const FlowComponent<Surface> &);
std::ostream &operator<<(std::ostream &, const Vertex &);

}  // namespace flatsurf

#endif
