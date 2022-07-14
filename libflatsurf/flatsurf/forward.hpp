/**********************************************************************
 *  This file is part of flatsurf.
 *
 *        Copyright (C) 2019-2022 Julian Rüth
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
// arguments for template parameters so we can only declare them in one place
// and this is what this file does.

#ifndef LIBFLATSURF_FORWARD_HPP
#define LIBFLATSURF_FORWARD_HPP

#include <iosfwd>

#include "local.hpp"

namespace flatsurf {

class Bound;

enum class CCW;

template <typename Surface>
class Chain;

template <typename Surface>
class ContourComponent;

template <typename Surface>
class ContourConnection;

template <typename Surface>
class ContourDecomposition;

template <typename Surface>
class HalfEdgeIntersection;

template <typename Surface>
class DecompositionStep;

template <typename Surface>
class Deformation;

enum class DELAUNAY;

class Edge;

template <typename T>
class EdgeMap;

class EdgeSet;

class EdgeSetIterator;

template <typename T>
class FlatTriangulation;

class FlatTriangulationCombinatorial;

template <typename Surface>
class FlatTriangulationCombinatorics;

template <typename T>
class FlatTriangulationCollapsed;

template <typename Surface>
class FlowComponent;

template <typename Surface>
class FlowConnection;

template <typename Surface>
class FlowDecomposition;

template <typename Surface>
class FlowTriangulation;

class HalfEdge;

template <typename T>
class HalfEdgeMap;

class HalfEdgeSet;

class HalfEdgeSetIterator;

template <typename T>
class ImplementationOf;

template <typename Surface>
class IntervalExchangeTransformation;

enum class ISOMORPHISM;

template <typename T>
class ManagedMovable;

template <typename Surface>
class MaybeVerticalFlowConnection;

template <typename T>
class OddHalfEdgeMap;

enum class ORIENTATION;

template <typename Surface>
class Path;

template <typename Surface>
class PathIterator;

template <typename T>
class Permutation;

template <typename Surface>
class Point;

struct PrivateConstructor {};

struct ProtectedConstructor {};

template <typename Surface>
class SaddleConnection;

template <typename Surface>
class SaddleConnections;

template <typename Surface>
class SaddleConnectionsByLength;

template <typename Surface>
class SaddleConnectionsByLengthIterator;

template <typename Surface>
class SaddleConnectionsIterator;

template <typename Surface>
class SaddleConnectionsSample;

template <typename Surface>
class SaddleConnectionsSampleIterator;

template <typename T>
class Serializable;

template <typename T>
struct Serialization;

template <typename T>
class Vector;

class Vertex;

template <typename Surface>
class Vertical;

}  // namespace flatsurf

// Forward declare cereal::access so we can befriend it in our classes.
// cereal is a serialization library that you can use by including the cereal.hpp header.
namespace cereal {
struct access;
}

#endif
