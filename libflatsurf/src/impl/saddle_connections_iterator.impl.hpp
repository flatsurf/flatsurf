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

#ifndef LIBFLATSURF_SADDLE_CONNECTIONS_ITERATOR_IMPL_HPP
#define LIBFLATSURF_SADDLE_CONNECTIONS_ITERATOR_IMPL_HPP

#include <boost/logic/tribool_fwd.hpp>
#include <deque>
#include <stack>
#include <variant>
#include <vector>

#include "../../flatsurf/bound.hpp"
#include "../../flatsurf/ccw.hpp"
#include "../../flatsurf/half_edge.hpp"
#include "../../flatsurf/saddle_connections_iterator.hpp"

namespace flatsurf {

template <typename Surface>
class ImplementationOf<SaddleConnectionsIterator<Surface>> {
  using T = typename Surface::Coordinate;
  using Sector = typename ImplementationOf<SaddleConnections<Surface>>::Sector;

 public:
  enum class Classification {
    OUTSIDE_SEARCH_SECTOR_CLOCKWISE,
    SADDLE_CONNECTION,
    OUTSIDE_SEARCH_SECTOR_COUNTERCLOCKWISE,
  };

  enum class State {
    // The search will now cross nextEdge which starts and ends inside the
    // search radius (or at least that's what we are assuming for simplicity.)
    START_AT_EDGE_STARTS_INSIDE_RADIUS_ENDS_INSIDE_RADIUS,
    // The search will now cross nextEdge which starts inside the search radius
    // (and probably also ends there but we do not know.)
    START_AT_EDGE_STARTS_INSIDE_RADIUS,
    // The search will now cross nextEdge which starts inside the search radius
    // but ends outside of the search radius.
    START_AT_EDGE_STARTS_INSIDE_RADIUS_ENDS_OUTSIDE_RADIUS,
    // The search will now cross nextEdge which ends inside the search radius
    // but we do not know whether it also starts there.
    START_AT_EDGE_ENDS_INSIDE_RADIUS,
    // The search will now cross nextEdge. We do not know whether it starts or
    // ends inside the search radius.
    START_AT_EDGE,
    // The search will now cross nextEdge which ends outside (or at) the search
    // radius. We do not know whether it starts inside or outside.
    START_AT_EDGE_ENDS_OUTSIDE_RADIUS,
    // The search will now cross nextEdge which starts outside (or at) the
    // search radius and ends inside the search radius.
    START_AT_EDGE_STARTS_OUTSIDE_RADIUS_ENDS_INSIDE_RADIUS,
    // The search will now cross nextEdge which starts outside (or at) the
    // search radius. We do not know whether it ends inside or outside the
    // search radius.
    START_AT_EDGE_STARTS_OUTSIDE_RADIUS,
    // The search will now cross nexetEdge with starts and ends outside (or at)
    // the search radius.
    START_AT_EDGE_STARTS_OUTSIDE_RADIUS_ENDS_OUTSIDE_RADIUS,
    OUTSIDE_SEARCH_SECTOR_COUNTERCLOCKWISE,
    OUTSIDE_SEARCH_SECTOR_CLOCKWISE,
    // The iterator has stopped at a Saddle Connection inside or at the search radius
    SADDLE_CONNECTION_FOUND,
    SADDLE_CONNECTION_FOUND_SEARCHING_FIRST,
    SADDLE_CONNECTION_FOUND_SEARCHING_SECOND,
    END,
  };

  enum class Move {
    GOTO_OTHER_FACE,
    GOTO_NEXT_EDGE,
    GOTO_PREVIOUS_EDGE,
  };

  using Boundary = std::variant<Chain<Surface>, Vector<T>>;

  static CCW ccw(const Boundary& lhs, const Chain<Surface>& rhs);
  static CCW ccw(const Boundary& lhs, const Boundary& rhs);
  static CCW ccw(const Boundary& lhs, const Vector<T>& rhs);

  ImplementationOf(const ImplementationOf<SaddleConnections<Surface>>&, const typename std::vector<Sector>::const_iterator begin, const typename std::vector<Sector>::const_iterator end);

  void prepareSearch();

  const ImplementationOf<SaddleConnections<Surface>>& connections;

  // The half edge nextEdge, to which we are currently changing, points into
  // sectors. Advanced when we are done searching an entire such sector for all
  // saddle connections.
  typename std::vector<Sector>::const_iterator sector;

  typename std::vector<Sector>::const_iterator end;

  // The rays that enclose the search sector, in counterclockwise order. When
  // chains, they are both exclusive. When vectors, the first one is inclusive,
  // the second one exclusive.
  Boundary boundary[2];
  // The half-edge that we are about to cross, seen from the search origin,
  // i.e., oriented so that it starts on the side of boundary[0]
  HalfEdge nextEdge;
  // The vector to the target of nextEdge
  Chain<Surface> nextEdgeEnd;

  // The call stack for increment().
  // As of early 2019, C++ lacks stackless coroutines. This code would be much
  // more readable with async/await idioms. And likely faster.
  // Unfortunately, boost's coroutines are not stackless and therefore too
  // slow, and co2 exceeds the stack size quickly, so we have to roll our own
  // stack for the time being.
  // At the same time, we cannot make recursive calls inside this function,
  // i.e., "return increment()" since that also exceeds the stack size for
  // (much larger) radii. (Strangely, GCC, as of early 2019, does not
  // optimize such tail recursion.)
  // (This should really be a stack. But we want to print its content easily
  // for debugging which a stack does not support.)
  std::deque<State> state;

  // We collect pending moves across the surface here (adding half edges to
  // nextEdgeEnd mostly.) When the exact value of nextEdgeEnd is required, we
  // can often combine several move more efficiently, see applyMoves().
  std::deque<Move> moves;

  // Storage space for temporary values of boundary, when we descend
  // recursively into a subsector.
  std::stack<Boundary> tmp;

  // The current connection so we can return it in dereference by reference.
  mutable SaddleConnection<Surface> connection;

  bool increment();

  bool onBoundary();

  void skipSector(CCW sector);

  void skip();

  void apply(const Move m);

  void applyMoves();

  Classification classifyHalfEdgeEnd();

  // Return the topmost State from the recursion stack.
  State pop();

  // Push a START_* state onto the stack.
  void pushStart(boost::logic::tribool fromOutside, boost::logic::tribool toOutside);

  // Push a START_* state onto the stack.
  void pushStart(State current, boost::logic::tribool toOutside);

  // Push a START_* state onto the stack.
  void pushStart(boost::logic::tribool fromOutside, State current);
};

template <typename Surface>
template <typename... Args>
SaddleConnectionsIterator<Surface>::SaddleConnectionsIterator(PrivateConstructor, Args&&... args) :
  self(spimpl::make_impl<ImplementationOf<SaddleConnectionsIterator>>(std::forward<Args>(args)...)) {}

}  // namespace flatsurf

#endif
