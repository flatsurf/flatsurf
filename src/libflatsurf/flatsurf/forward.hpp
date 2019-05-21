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

#include "flatsurf/ccw.hpp"
#include "flatsurf/orientation.hpp"

namespace flatsurf {
template <typename Vector, typename VectorAlongTriangulation = typename Vector::AlongTriangulation>
class SaddleConnections;

template <typename Vector, typename VectorAlongTriangulation = typename Vector::AlongTriangulation>
class SaddleConnectionsIterator;

template <typename Vector, typename VectorAlongTriangulation = typename Vector::AlongTriangulation>
class SaddleConnection;

template <typename Vector>
class FlatTriangulation;

class HalfEdge;

template <typename T>
class HalfEdgeMap;

class Edge;

template <typename T>
class Permutation;

struct Bound;

template <typename Ring>
class VectorExactReal;

template <typename V>
class VectorAlongTriangulation;

class VectorArb;

class Vertex;

class FlatTriangulationCombinatorial;
}  // namespace flatsurf

#endif
