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

// This header forward declares all the types that exist in this library. The
// purpose of this is mostly that C++ does not allow redeclaration of default
// arguments for template parameters so we can only declare them in one place
// and this is what this file does.

#ifndef LIBFLATSURF_FORWARD_HPP
#define LIBFLATSURF_FORWARD_HPP

#include <iosfwd>

#include "local.hpp"

namespace flatsurf {
enum class CCW;

enum class ORIENTATION;

struct PrivateConstructor {};

template <typename T>
class Vector;

class FlatTriangulationCombinatorial;

template <typename T>
class FlatTriangulation;

template <typename T>
class FlatTriangulationCollapsed;

template <typename Surface>
class Vertical;

template <typename Surface>
class Chain;

template <typename Surface>
class SaddleConnections;

template <typename Surface>
class SaddleConnectionsIterator;

template <typename Surface>
class ContourConnection;

template <typename Surface>
class SaddleConnection;

class HalfEdge;

class Edge;

class Vertex;

template <typename T>
class Tracking;

template <typename K, typename V>
class TrackingMap;

template <typename T>
class EdgeMap;

template <typename T>
class HalfEdgeMap;

template <typename T>
class OddHalfEdgeMap;

class EdgeSet;

class EdgeSetIterator;

class HalfEdgeSet;

class HalfEdgeSetIterator;

template <typename T>
class Permutation;

class Bound;

template <typename Surface>
class IntervalExchangeTransformation;

template <typename Surface>
class ContourDecomposition;

template <typename Surface>
class ContourComponent;

template <typename Surface>
class FlowDecomposition;

template <typename Surface>
class FlowComponent;

template <typename Surface>
class DecompositionStep;

template <typename Surface>
class FlowConnection;

template <typename Surface>
class MaybeVerticalFlowConnection;

template <typename Surface>
class Path;

template <typename Surface>
class PathIterator;

template <typename T>
class Serializable;

template <typename T>
struct Serialization;

template <typename T>
class ImplementationOf;

}  // namespace flatsurf

// Forward declare cereal::access so we can befriend it in our classes.
// cereal is a serialization library that you can use by including the cereal.hpp header.
namespace cereal {
struct access;
}

#endif
