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

// This header forward declares all the types that exist in this library. The
// purpose of this is mostly that C++ does not allow redeclaration of default
// arguments for template parameters so we can only declare them in one and this
// is what this file does.

#ifndef LIBFLATSURF_FORWARD_HPP
#define LIBFLATSURF_FORWARD_HPP

#include "flatsurf/flatsurf.hpp"

namespace flatsurf {
template <typename Vector, typename VectorAlongTriangulation =
                               typename Vector::AlongTriangulation>
struct SaddleConnections;

template <typename Vector, typename VectorAlongTriangulation =
                               typename Vector::AlongTriangulation>
struct SaddleConnectionsIterator;

template <typename Vector, typename VectorAlongTriangulation =
                               typename Vector::AlongTriangulation>
struct SaddleConnection;

template <typename Vector>
struct FlatTriangulation;

struct HalfEdge;

template <typename T>
struct HalfEdgeMap;

struct Edge;

template <typename T>
struct Permutation;

struct Bound;

template <typename Ring>
struct VectorExactReal;

template <typename V>
struct VectorAlongTriangulation;

struct Vertex;

struct FlatTriangulationCombinatorial;
}  // namespace flatsurf

#endif
