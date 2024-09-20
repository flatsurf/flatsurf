/**********************************************************************
 *  This file is part of flatsurf.
 *
 *        Copyright (C) 2019-2024 Julian Rüth
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

// Work around https://bitbucket.org/wlav/cppyy/issues/310/templatized-reference-in-callback by not using references in the callback.
template <typename Surface>
bool decomposeFlowDecomposition(
    FlowDecomposition<Surface> &decomposition, const std::function<bool(FlowComponent<Surface>)> &target, int limit = -1) {
  return decomposition.decompose(target, limit);
}

// Work around spurious issues on macOS: Failed to materialize symbols: { (main, { __ZN16__cppyy_internal10fptr_wrap3EN8flatsurf13FlowComponentINS0_17FlatTriangulationIN9exactreal7ElementINS3_13RationalFieldEEEEEEE }) }
template <typename Surface>
bool decomposeFlowDecomposition(
    FlowDecomposition<Surface> &decomposition, int limit = -1) {
  return decomposition.decompose([](auto component) { return FlowDecomposition<Surface>::defaultTarget(component); }, limit);
}

// Work around https://bitbucket.org/wlav/cppyy/issues/310/templatized-reference-in-callback by not using references in the callback.
template <typename Surface>
bool decomposeFlowComponent(
    FlowComponent<Surface> &component, const std::function<bool(FlowComponent<Surface>)> &target = [](auto component) { return FlowComponent<Surface>::defaultTarget(component); }, int limit = -1) {
  return component.decompose(target, limit);
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
std::optional<Deformation<FlatTriangulation<T>>> isomorphism(const FlatTriangulation<T> &preimage, const FlatTriangulation<T> &image, ISOMORPHISM kind, std::function<bool(IsomorphismFilterMatrix<T>)> filter_matrix, std::function<bool(HalfEdge, HalfEdge)> filter_map) {
  return preimage.isomorphism(
      image, kind, [&](const T &a, const T &b, const T &c, const T &d) {
        return filter_matrix(IsomorphismFilterMatrix<T>(a, b, c, d));
      },
      filter_map);
}

// cppyy has trouble with complex std::function parameters so we simplify the
// interface of Tracked::Tracked to take away all qualifiers and most commas.
template <typename T>
Tracked<T> track(const FlatTriangulationCombinatorial &surface, T value) {
  return Tracked<T>(surface, std::move(value));
}

// Extract the value of a Tracked<T>.
// There seems to be no immediate way to call the operator* in cppyy.
template <typename T>
const T &unwrapTracked(const Tracked<T> &tracked) {
  return tracked;
}

// Work around https://github.com/wlav/cppyy/issues/19 by removing std::forward
// from the converting constructor of Vector.
namespace cppyy {
template <typename T>
class Vector : public ::flatsurf::Vector<T> {
 public:
  using Coordinate = T;

  Vector() :
    ::flatsurf::Vector<T>() {}
  Vector(const Coordinate &x, const Coordinate &y) :
    ::flatsurf::Vector<T>(x, y) {}

  template <typename X, typename Y, std::enable_if_t<!std::is_convertible_v<X, Coordinate> || !std::is_convertible_v<Y, Coordinate>, int> = 0>
  Vector(const X &x, const Y &y) :
    ::flatsurf::Vector<T>(static_cast<T>(x), static_cast<T>(y)) {}
};

// Work around https://github.com/wlav/cppyy/issues/245
template <typename T>
std::vector<T> vectorFromList(const std::list<T>& x) {
  return std::vector(x.begin(), x.end());
}

}  // namespace cppyy

}  // namespace flatsurf

#endif
