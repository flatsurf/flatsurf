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

// This header forward declares all the types that exist in this library. The
// purpose of this is mostly that C++ does not allow redeclaration of default
// arguments for template parameters so we can only declare them in one and this
// is what this file does.

#ifndef LIBFLATSURF_FORWARD_HPP
#define LIBFLATSURF_FORWARD_HPP

#include <intervalxt/forward.hpp>

#include "flatsurf/flatsurf.hpp"

#include "flatsurf/ccw.hpp"
#include "flatsurf/orientation.hpp"

namespace flatsurf {
template <typename T>
class Vector;

class FlatTriangulationCombinatorial;

template <typename T>
class FlatTriangulation;

template <typename T, typename Approximation = void, typename Surface = FlatTriangulation<T>>
class VectorAlongTriangulation;

template <typename T>
class LengthAlongTriangulation;

template <typename Surface>
class SaddleConnections;

template <typename Surface>
class SaddleConnection;

class HalfEdge;

class Edge;

class Vertex;

template <typename T>
class HalfEdgeMap;

template <typename T>
class Permutation;

using Bound = intervalxt::Length<long long>;

template <typename T>
class IntervalExchangeTransformation;

template <typename T>
class Component;

template <typename T>
class FlowDecomposition;
}  // namespace flatsurf

// The following block of forward declarations is a bit odd. It only exists to
// work around bugs in cppyy.

// See https://bitbucket.org/wlav/cppyy/issues/95/lookup-of-friend-operator
namespace flatsurf {
std::ostream &operator<<(std::ostream &, const HalfEdge &);
template <typename T>
std::ostream &operator<<(std::ostream &, const Permutation<T> &);
template <typename T>
std::ostream &operator<<(std::ostream &, const HalfEdgeMap<T> &);
template <typename T>
std::ostream &operator<<(std::ostream &, const FlatTriangulation<T> &);
// Strangely, when we do not put the _ here and try to print a
// FlatTriangulation<eantic::renf_elem_class> through cppyy, it would compile
// code that looks sane but fail because the overload resolution picked this
// overload (which seems to be completely unrelated.) This fails because
// renf_elem_class, does not have a ::Vector which SaddleConnection class
// requires. This is clearly a bug in cppyy, but we have not been able to
// create a minimal reproducer yet.
template <typename Surface, typename _ = typename Surface::Vector>
std::ostream &operator<<(std::ostream &, const SaddleConnection<Surface> &);
template <typename Surface>
std::ostream &operator<<(std::ostream &, const SaddleConnections<Surface> &);

// This does not work due to https://bitbucket.org/wlav/cppyy/issues/112/operator-for-a-base-class-is-not-found
// namespace detail {
// template <typename V>
// std::ostream &operator<<(std::ostream &, const detail::VectorBase<V> &);
// }  // namespace detail

}  // namespace flatsurf

// Forward declare cereal::access so we can befriend it in our classes.
// cereal is a serialization library that you can use by including the cereal.hpp header.
namespace cereal {
struct access;
}

#endif
