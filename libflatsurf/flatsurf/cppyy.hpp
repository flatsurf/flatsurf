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
// cppyy sometimes has trouble with rvalues, let's help it to create a FlatTriangulation
// See https://bitbucket.org/wlav/cppyy/issues/275/result-of-cppyygblstdmove-is-not-an-rvalue.
template <typename T>
FlatTriangulation<T> makeFlatTriangulation(const std::vector<std::vector<int>> &vertices, const std::vector<Vector<T>> &vectors) {
  return FlatTriangulation<T>(FlatTriangulationCombinatorial(vertices), vectors);
}

// cppyy sometimes has trouble with rvalues, let's help it to create a FlowDecomposition
// See https://bitbucket.org/wlav/cppyy/issues/275/result-of-cppyygblstdmove-is-not-an-rvalue.
template <typename T>
FlowDecomposition<FlatTriangulation<T>> makeFlowDecomposition(const FlatTriangulation<T> &surface, const Vector<T> &v) {
  return FlowDecomposition<FlatTriangulation<T>>(surface.clone(), v);
}

// Work around https://bitbucket.org/wlav/cppyy/issues/273/segfault-in-cpycppyy-anonymous-namespace
template <typename T>
auto makeOddHalfEdgeMap(const FlatTriangulationCombinatorial &surface, const std::vector<T> &values) {
  return OddHalfEdgeMap<T>(surface, [&](const HalfEdge &e) {
    return e == Edge(e).positive() ? values.at(Edge(e).index()) : -values.at(Edge(e).index());
  });
}

// Work around https://bitbucket.org/wlav/cppyy/issues/273/segfault-in-cpycppyy-anonymous-namespace
template <typename T>
bool decomposeFlowDecomposition(FlowDecomposition<T> &decomposition, int limit = -1) {
  return decomposition.decompose(FlowDecomposition<T>::defaultTarget, limit);
}

template <typename T>
struct IsomorphismFilterMatrix {
  IsomorphismFilterMatrix(T a, T b, T c, T d) :
    a(std::move(a)),
    b(std::move(b)),
    c(std::move(c)),
    d(std::move(d)) {}
  T a, b, c, d;
};

// cppyy has trouble with complex std::function parameters so we simplify the
// interface of FlatTriangulation::isomorphism to take away all qualifiers and
// most commas.
template <typename T>
std::optional<Deformation<FlatTriangulation<T>>> isomorphism(const FlatTriangulation<T> &preimage, const FlatTriangulation<T> &image, std::function<bool(IsomorphismFilterMatrix<T>)> filter_matrix, std::function<bool(HalfEdge, HalfEdge)> filter_map) {
  return preimage.isomorphism(
      image, [&](const T &a, const T &b, const T &c, const T &d) {
        return filter_matrix(IsomorphismFilterMatrix<T>(a, b, c, d));
      },
      filter_map);
}

}  // namespace flatsurf

#endif
