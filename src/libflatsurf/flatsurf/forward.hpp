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

struct Bound;

template <typename T>
class IntervalExchangeTransformation;

template <typename T>
class Component;

template <typename T>
class FlowDecomposition;
}  // namespace flatsurf

#endif
