/**********************************************************************
 *  This file is part of flatsurf.
 *
 *        Copyright (C) 2020-2021 Julian Rüth
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

#include <fmt/format.h>

#include <boost/logic/tribool.hpp>
#include <exact-real/arb.hpp>
#include <optional>
#include <type_traits>
#include <variant>
#include <vector>

#include "../flatsurf/chain.hpp"
#include "../flatsurf/fmt.hpp"
#include "../flatsurf/half_edge_intersection.hpp"
#include "../flatsurf/saddle_connection.hpp"
#include "../flatsurf/vector.hpp"
#include "external/gmpxxll/gmpxxll/mpz_class.hpp"
#include "external/rx-ranges/include/rx/ranges.hpp"
#include "impl/half_edge_intersection.impl.hpp"
#include "impl/quadratic_polynomial.hpp"
#include "impl/saddle_connections.impl.hpp"
#include "impl/saddle_connections_iterator.impl.hpp"
#include "util/assert.ipp"

namespace flatsurf {

using std::vector;

template <typename Surface>
ImplementationOf<SaddleConnectionsIterator<Surface>>::ImplementationOf(const ImplementationOf<SaddleConnections<Surface>>& connections, const typename vector<Sector>::const_iterator begin, const typename vector<Sector>::const_iterator end) :
  connections(connections),
  sector(begin),
  end(end),
  boundary{Vector<T>(), Vector<T>()},
  nextEdgeEnd(connections.surface),
  connection(SaddleConnection(*connections.surface, connections.surface->halfEdges()[0])) {
  prepareSearch();
}

template <typename Surface>
void ImplementationOf<SaddleConnectionsIterator<Surface>>::prepareSearch() {
  assert(state.size() == 0);
  assert(tmp.size() == 0);
  assert(moves.size() == 0);

  if (sector == end)
    return;

  const HalfEdge e = sector->source;

  if (connections.surface->boundary(e)) {
    sector++;
    prepareSearch();
    return;
  }

  boundary[0] = Chain(*connections.surface) + e;
  if (sector->sector)
    boundary[0] = sector->sector->first;

  nextEdge = connections.surface->nextInFace(e);
  boundary[1] = Chain(*connections.surface) + e + nextEdge;
  if (sector->sector)
    boundary[1] = sector->sector->second;

  nextEdgeEnd = (Chain<Surface>(connections.surface) += e) += nextEdge;
  state.push_back(State::END);
  state.push_back(State::START_AT_EDGE);

  // Report the half edge "e" as a saddle connection unless it is outside the
  // search scope.
  const auto initial = SaddleConnection(*connections.surface, e);
  if (std::holds_alternative<Vector<T>>(boundary[0]) && sector->contains(initial))
    boundary[0] = Chain(*connections.surface) + e;
  if ((connections.searchRadius && initial > *connections.searchRadius) || !sector->contains(initial) || initial <= connections.lowerBound) {
    while (!increment())
      ;
  }
}

template <typename Surface>
CCW ImplementationOf<SaddleConnectionsIterator<Surface>>::ccw(const Boundary& lhs, const Chain<Surface>& rhs) {
  return std::visit([&](const auto& l) {
    using B = std::decay_t<decltype(l)>;
    if constexpr (std::is_same_v<B, Chain<Surface>>) {
      auto ccw = static_cast<const Vector<exactreal::Arb>&>(l).ccw(static_cast<const Vector<exactreal::Arb>&>(rhs));
      if (ccw) return *ccw;
    }
    return ccw(lhs, static_cast<const Vector<T>&>(rhs));
  },
      lhs);
}

template <typename Surface>
CCW ImplementationOf<SaddleConnectionsIterator<Surface>>::ccw(const Boundary& lhs, const Vector<T>& rhs) {
  return std::visit([&](const auto& b) { return static_cast<const Vector<T>&>(b).ccw(rhs); }, lhs);
}

template <typename Surface>
CCW ImplementationOf<SaddleConnectionsIterator<Surface>>::ccw(const Boundary& lhs, const Boundary& rhs) {
  return std::visit([&](const auto& b) {
    return ccw(lhs, b);
  },
      rhs);
}

template <typename Surface>
typename ImplementationOf<SaddleConnectionsIterator<Surface>>::State ImplementationOf<SaddleConnectionsIterator<Surface>>::pop() {
  auto s = state.back();
  state.pop_back();

  // If we don't know anything about the next edge, we need to find out whether
  // it has one endpoint inside/outside the search radius. Otherwise there are
  // cases where this algorithm never stops.
  if (s == State::START_AT_EDGE) {
    if (connections.searchRadius) {
      applyMoves();

      if (static_cast<const Vector<T>&>(nextEdgeEnd) < *connections.searchRadius)
        s = State::START_AT_EDGE_ENDS_INSIDE_RADIUS;
      else
        s = State::START_AT_EDGE_ENDS_OUTSIDE_RADIUS;
    }
  }

  return s;
}

template <typename Surface>
bool ImplementationOf<SaddleConnectionsIterator<Surface>>::increment() {
  assert(state.size());
  assert(sector != end);
  assert(ccw(boundary[0], boundary[1]) != CCW::CLOCKWISE);

  const auto s = pop();

  switch (s) {
    case State::END:
      applyMoves();
      sector++;
      if (sector != end) {
        prepareSearch();
      }
      return true;

    // Some endpoints of nextEdge are outside the search radius. If all of
    // the edge corresponding to nextEdgeEnd is outside the search radius, we
    // can abort the search.
    case State::START_AT_EDGE_ENDS_OUTSIDE_RADIUS:
    case State::START_AT_EDGE_STARTS_OUTSIDE_RADIUS:
    case State::START_AT_EDGE_STARTS_OUTSIDE_RADIUS_ENDS_OUTSIDE_RADIUS:
      LIBFLATSURF_ASSERT(connections.searchRadius, "If no search radius has been specified, the endpoints cannot be beyond the search radius.");

      // We write this edge as p + λe. Then it is completely outside the search
      // radius if |p + λe|² - R² > 0 for all λ in [0, 1].
      // Note that there was no relevant performance gain in treating the cases
      // where only one endpoint is known to be outside the search radius
      // differently.
      {
        applyMoves();
        const Vector<T>& edge = connections.surface->fromHalfEdge(-nextEdge);
        const Vector<T>& edgeEnd = nextEdgeEnd;

        T R2;
        if constexpr (std::is_same_v<T, long long>) {
          R2 = gmpxxll::mpz_class((*connections.searchRadius).squared()).get_sll();
        } else {
          R2 = T((*connections.searchRadius).squared());
        }

        if (QuadraticPolynomial<T>(
                edge.x() * edge.x() + edge.y() * edge.y(),
                2 * edgeEnd.x() * edge.x() + 2 * edgeEnd.y() * edge.y(),
                edgeEnd.x() * edgeEnd.x() + edgeEnd.y() * edgeEnd.y() - R2)
                .positive())
          return false;
      }
      [[fallthrough]];

    // Some endpoint of nextEdge is inside the search radius. We cross over
    // this edge and search for saddle connections.
    case State::START_AT_EDGE:
    case State::START_AT_EDGE_STARTS_INSIDE_RADIUS_ENDS_INSIDE_RADIUS:
    case State::START_AT_EDGE_STARTS_INSIDE_RADIUS:
    case State::START_AT_EDGE_STARTS_INSIDE_RADIUS_ENDS_OUTSIDE_RADIUS:
    case State::START_AT_EDGE_ENDS_INSIDE_RADIUS:
    case State::START_AT_EDGE_STARTS_OUTSIDE_RADIUS_ENDS_INSIDE_RADIUS:
      moves.push_back(Move::GOTO_OTHER_FACE);

      if (onBoundary()) {
        moves.push_back(Move::GOTO_OTHER_FACE);
        return false;
      }

      moves.push_back(Move::GOTO_NEXT_EDGE);

      switch (classifyHalfEdgeEnd()) {
        case Classification::OUTSIDE_SEARCH_SECTOR_CLOCKWISE: {
          // This vertex is outside of the search sector on the
          // clockwise side, we skip the clockwise sector in the recursive
          // search and recurse into the counterclockwise sector.
          moves.push_back(Move::GOTO_NEXT_EDGE);

          state.push_back(State::OUTSIDE_SEARCH_SECTOR_CLOCKWISE);

          // When crossing over this edge, we (often) already know whether one
          // of its endpoints is inside or outside the search radius; that's
          // encoded in s. Now that we just performed a classifyHalfEdgeEnd()
          // it is cheap to also ask whether we are below the search radius
          // with the other end point.
          pushStart(connections.searchRadius && !(nextEdgeEnd < *connections.searchRadius), s);

          return false;
        }
        case Classification::OUTSIDE_SEARCH_SECTOR_COUNTERCLOCKWISE: {
          // Similarly, we skip the counterclockwise sector (but only after
          // we visited the clockwise one.)
          state.push_back(State::OUTSIDE_SEARCH_SECTOR_COUNTERCLOCKWISE);

          // In parallel to the previous case, we could ask cheaply whether one
          // end point is still within the search radius. I.e., perform this call:
          // pushStart(s, connections.searchRadius && !(nextEdgeEnd < *connections.searchRadius));
          // However, it is beneficial to only perform this call in one of
          // these branches as the check does not come entirely for free.
          pushStart(s, {boost::logic::indeterminate});

          return false;
        }
        case Classification::SADDLE_CONNECTION: {
          const bool beyondRadius = connections.searchRadius && nextEdgeEnd > *connections.searchRadius;

          // Prepare the recursive descent into the two half edges attached
          // to this vertex.
          state.push_back(State::SADDLE_CONNECTION_FOUND_SEARCHING_SECOND);
          pushStart(beyondRadius, s);
          state.push_back(State::SADDLE_CONNECTION_FOUND_SEARCHING_FIRST);
          pushStart(s, beyondRadius);

          // Shrink the search sector for the clockwise descent.
          if (std::holds_alternative<Chain<Surface>>(boundary[1]) || std::get<Vector<T>>(boundary[1]).ccw(nextEdgeEnd) != CCW::COUNTERCLOCKWISE) {
            tmp.push(std::move(boundary[1]));
            boundary[1] = nextEdgeEnd;
          } else {
            tmp.push(boundary[1]);
          }
          // Exclude sectorBegin from future search if this saddle connections
          // hits it exactly so we hide all future vertices that lie on this
          // line.
          if (std::holds_alternative<Vector<T>>(boundary[0]) && std::get<Vector<T>>(boundary[0]).ccw(nextEdgeEnd) == CCW::COLLINEAR)
            boundary[0] = nextEdgeEnd;

          if (beyondRadius) {
            return false;
          } else {
            state.push_back(State::SADDLE_CONNECTION_FOUND);
            return nextEdgeEnd > connections.lowerBound;
          }
        }
        default:
          throw std::logic_error("unknown classification result");
      }
    case State::SADDLE_CONNECTION_FOUND:
      return false;
    case State::SADDLE_CONNECTION_FOUND_SEARCHING_FIRST:
      // We have just come back from the search in the clockwise sector; now
      // we prepare the recursive descend into the counterclockwise sector.
      boundary[1] = std::move(tmp.top());
      tmp.pop();
      applyMoves();
      // Shrink the search sector for the counter-clockwise descent
      if (std::holds_alternative<Chain<Surface>>(boundary[0]) || std::get<Vector<T>>(boundary[0]).ccw(nextEdgeEnd) != CCW::CLOCKWISE) {
        tmp.push(std::move(boundary[0]));
        boundary[0] = nextEdgeEnd;
      } else {
        tmp.push(boundary[0]);
      }
      moves.push_back(Move::GOTO_NEXT_EDGE);
      return false;
    case State::SADDLE_CONNECTION_FOUND_SEARCHING_SECOND:
      // We have just come back from the search in the counterclockwise
      // sector; we are done here and return in the recursion.
      boundary[0] = std::move(tmp.top());
      tmp.pop();
      moves.push_back(Move::GOTO_NEXT_EDGE);
      moves.push_back(Move::GOTO_OTHER_FACE);
      return false;
    case State::OUTSIDE_SEARCH_SECTOR_CLOCKWISE:
      moves.push_back(Move::GOTO_NEXT_EDGE);
      moves.push_back(Move::GOTO_OTHER_FACE);
      return false;
    case State::OUTSIDE_SEARCH_SECTOR_COUNTERCLOCKWISE:
      moves.push_back(Move::GOTO_NEXT_EDGE);
      moves.push_back(Move::GOTO_NEXT_EDGE);
      moves.push_back(Move::GOTO_OTHER_FACE);
      return false;
    default:
      throw std::logic_error("unknown State in increment()");
  }
}

template <typename Surface>
bool ImplementationOf<SaddleConnectionsIterator<Surface>>::onBoundary() {
  // This code path could be optimized away by adding a bool template
  // parameter hasBoundary to increment().
  applyMoves();
  return connections.surface->boundary(nextEdge);
}

template <typename Surface>
void ImplementationOf<SaddleConnectionsIterator<Surface>>::skipSector(CCW sector) {
  LIBFLATSURF_ASSERT_ARGUMENT(sector != CCW::COLLINEAR, "Cannot skip this sector. There is no such thing as a collinear sector.");
  assert(state.size() && "cannot skip a sector in a completed search");

  switch (state.back()) {
    case State::SADDLE_CONNECTION_FOUND:
      state.pop_back();

      if (sector == CCW::CLOCKWISE) {
        // Go directly to the second sector by skipping the recursive call,
        // i.e., the START.
        switch (state.back()) {
          case State::START_AT_EDGE_STARTS_INSIDE_RADIUS_ENDS_INSIDE_RADIUS:
          case State::START_AT_EDGE_STARTS_INSIDE_RADIUS:
          case State::START_AT_EDGE_STARTS_INSIDE_RADIUS_ENDS_OUTSIDE_RADIUS:
          case State::START_AT_EDGE_ENDS_INSIDE_RADIUS:
          case State::START_AT_EDGE:
          case State::START_AT_EDGE_ENDS_OUTSIDE_RADIUS:
          case State::START_AT_EDGE_STARTS_OUTSIDE_RADIUS_ENDS_INSIDE_RADIUS:
          case State::START_AT_EDGE_STARTS_OUTSIDE_RADIUS:
          case State::START_AT_EDGE_STARTS_OUTSIDE_RADIUS_ENDS_OUTSIDE_RADIUS:
            state.pop_back();
            break;
          default:
            break;
        }
        LIBFLATSURF_ASSERT(state.back() == State::SADDLE_CONNECTION_FOUND_SEARCHING_FIRST, "State machine of SaddleConnections is inconsistent when trying to skip clockwise sector.");
      } else if (sector == CCW::COUNTERCLOCKWISE) {
        // Skip the second recursive call by dropping its START.
        std::stack<State> unchanged;
        while (state.back() != State::SADDLE_CONNECTION_FOUND_SEARCHING_SECOND) {
          unchanged.push(state.back());
          state.pop_back();
        }

        switch (unchanged.top()) {
          case State::START_AT_EDGE_STARTS_INSIDE_RADIUS_ENDS_INSIDE_RADIUS:
          case State::START_AT_EDGE_STARTS_INSIDE_RADIUS:
          case State::START_AT_EDGE_STARTS_INSIDE_RADIUS_ENDS_OUTSIDE_RADIUS:
          case State::START_AT_EDGE_ENDS_INSIDE_RADIUS:
          case State::START_AT_EDGE:
          case State::START_AT_EDGE_ENDS_OUTSIDE_RADIUS:
          case State::START_AT_EDGE_STARTS_OUTSIDE_RADIUS_ENDS_INSIDE_RADIUS:
          case State::START_AT_EDGE_STARTS_OUTSIDE_RADIUS:
          case State::START_AT_EDGE_STARTS_OUTSIDE_RADIUS_ENDS_OUTSIDE_RADIUS:
            unchanged.pop();
          default:
            break;
        }

        while (!unchanged.empty()) {
          state.push_back(unchanged.top());
          unchanged.pop();
        }
      }
      break;
    case State::START_AT_EDGE_STARTS_INSIDE_RADIUS_ENDS_INSIDE_RADIUS:
    case State::START_AT_EDGE_STARTS_INSIDE_RADIUS:
    case State::START_AT_EDGE_STARTS_INSIDE_RADIUS_ENDS_OUTSIDE_RADIUS:
    case State::START_AT_EDGE_ENDS_INSIDE_RADIUS:
    case State::START_AT_EDGE:
    case State::START_AT_EDGE_ENDS_OUTSIDE_RADIUS:
    case State::START_AT_EDGE_STARTS_OUTSIDE_RADIUS_ENDS_INSIDE_RADIUS:
    case State::START_AT_EDGE_STARTS_OUTSIDE_RADIUS:
    case State::START_AT_EDGE_STARTS_OUTSIDE_RADIUS_ENDS_OUTSIDE_RADIUS:
      if (state.size() == 2) {
        if (sector == CCW::COUNTERCLOCKWISE) {
          // We are in the initial state, the reported saddle connection is on
          // the clockwise end of the search sector. If we skip the
          // counterclockwise sector, then we skip everything.
          state.pop_back();
          assert(state.back() == State::END);
        } else {
          // We are skipping the clockwise sector anyway.
          ;
        }
        break;
      }
      [[fallthrough]];
    default:
      throw std::logic_error("sectors can only be skipped when a saddle connection has been reported");
  }
}

template <typename Surface>
void ImplementationOf<SaddleConnectionsIterator<Surface>>::apply(const Move m) {
  switch (m) {
    case Move::GOTO_NEXT_EDGE:
      nextEdge = connections.surface->nextInFace(nextEdge);
      nextEdgeEnd += nextEdge;
      break;
    case Move::GOTO_OTHER_FACE:
      nextEdge = -nextEdge;
      nextEdgeEnd += nextEdge;
      break;
    case Move::GOTO_PREVIOUS_EDGE:
      nextEdgeEnd -= nextEdge;
      nextEdge = connections.surface->nextAtVertex(nextEdge);
      nextEdge = -nextEdge;
      break;
  }
}

template <typename Surface>
void ImplementationOf<SaddleConnectionsIterator<Surface>>::applyMoves() {
  if (moves.size() == 0) {
    return;
  }
  while (moves.size()) {
    const auto m = moves.front();
    moves.pop_front();
    if (moves.size() == 0) {
      apply(m);
      return;
    }

    const auto n = moves.front();
    moves.pop_front();

    switch (m) {
      case Move::GOTO_NEXT_EDGE:
        switch (n) {
          case Move::GOTO_NEXT_EDGE:
            moves.push_front(Move::GOTO_PREVIOUS_EDGE);
            continue;
          case Move::GOTO_PREVIOUS_EDGE:
            continue;
          case Move::GOTO_OTHER_FACE:
            nextEdge = connections.surface->nextInFace(nextEdge);
            nextEdge = -nextEdge;
            continue;
        }
        assert(false && "impossible enum value");
        break;
      case Move::GOTO_PREVIOUS_EDGE:
        switch (n) {
          case Move::GOTO_NEXT_EDGE:
            continue;
          case Move::GOTO_PREVIOUS_EDGE:
            moves.push_front(Move::GOTO_NEXT_EDGE);
            continue;
          case Move::GOTO_OTHER_FACE:
            apply(Move::GOTO_NEXT_EDGE);
            moves.push_front(Move::GOTO_OTHER_FACE);
            moves.push_front(Move::GOTO_NEXT_EDGE);
            continue;
        }
        assert(false && "impossible enum value");
        break;
      case Move::GOTO_OTHER_FACE:
        switch (n) {
          case Move::GOTO_NEXT_EDGE:
            moves.push_front(Move::GOTO_PREVIOUS_EDGE);
            [[fallthrough]];
          case Move::GOTO_PREVIOUS_EDGE:
            nextEdge = -nextEdge;
            nextEdge = connections.surface->nextAtVertex(nextEdge);
            nextEdge = -nextEdge;
            continue;
          case Move::GOTO_OTHER_FACE:
            continue;
        }
        assert(false && "impossible enum value");
        break;
    }
  }
}

template <typename Surface>
typename ImplementationOf<SaddleConnectionsIterator<Surface>>::Classification ImplementationOf<SaddleConnectionsIterator<Surface>>::classifyHalfEdgeEnd() {
  applyMoves();
  switch (ccw(boundary[0], nextEdgeEnd)) {
    case CCW::CLOCKWISE:
      return Classification::OUTSIDE_SEARCH_SECTOR_CLOCKWISE;
    case CCW::COLLINEAR:
      if (std::holds_alternative<Chain<Surface>>(boundary[0]))
        return Classification::OUTSIDE_SEARCH_SECTOR_CLOCKWISE;
      else
        return Classification::SADDLE_CONNECTION;
    case CCW::COUNTERCLOCKWISE:
      switch (ccw(boundary[1], nextEdgeEnd)) {
        case CCW::CLOCKWISE:
          return Classification::SADDLE_CONNECTION;
        case CCW::COUNTERCLOCKWISE:
        case CCW::COLLINEAR:
          return Classification::OUTSIDE_SEARCH_SECTOR_COUNTERCLOCKWISE;
      }
  }
  throw std::logic_error("impossible to happen");
}

template <typename Surface>
void ImplementationOf<SaddleConnectionsIterator<Surface>>::pushStart(boost::logic::tribool fromOutside, State current) {
  switch (current) {
    case State::START_AT_EDGE_STARTS_INSIDE_RADIUS_ENDS_INSIDE_RADIUS:
    case State::START_AT_EDGE_ENDS_INSIDE_RADIUS:
    case State::START_AT_EDGE_STARTS_OUTSIDE_RADIUS_ENDS_INSIDE_RADIUS:
      pushStart(fromOutside, false);
      return;
    case State::START_AT_EDGE_STARTS_INSIDE_RADIUS_ENDS_OUTSIDE_RADIUS:
    case State::START_AT_EDGE_ENDS_OUTSIDE_RADIUS:
    case State::START_AT_EDGE_STARTS_OUTSIDE_RADIUS_ENDS_OUTSIDE_RADIUS:
      pushStart(fromOutside, true);
      return;
    default:
      pushStart(fromOutside, {boost::logic::indeterminate});
      return;
  }
}

template <typename Surface>
void ImplementationOf<SaddleConnectionsIterator<Surface>>::pushStart(State current, boost::logic::tribool toOutside) {
  switch (current) {
    case State::START_AT_EDGE_STARTS_INSIDE_RADIUS_ENDS_INSIDE_RADIUS:
    case State::START_AT_EDGE_STARTS_INSIDE_RADIUS:
    case State::START_AT_EDGE_STARTS_INSIDE_RADIUS_ENDS_OUTSIDE_RADIUS:
      pushStart(false, toOutside);
      return;
    case State::START_AT_EDGE_STARTS_OUTSIDE_RADIUS_ENDS_INSIDE_RADIUS:
    case State::START_AT_EDGE_STARTS_OUTSIDE_RADIUS:
    case State::START_AT_EDGE_STARTS_OUTSIDE_RADIUS_ENDS_OUTSIDE_RADIUS:
      pushStart(true, toOutside);
      return;
    default:
      pushStart({boost::logic::indeterminate}, toOutside);
      return;
  }
}

template <typename Surface>
void ImplementationOf<SaddleConnectionsIterator<Surface>>::pushStart(boost::logic::tribool fromOutside, boost::logic::tribool toOutside) {
  if (fromOutside) {
    if (toOutside) {
      state.push_back(State::START_AT_EDGE_STARTS_OUTSIDE_RADIUS_ENDS_OUTSIDE_RADIUS);
    } else if (!toOutside) {
      state.push_back(State::START_AT_EDGE_STARTS_OUTSIDE_RADIUS_ENDS_INSIDE_RADIUS);
    } else {
      state.push_back(State::START_AT_EDGE_STARTS_OUTSIDE_RADIUS);
    }
  } else if (!fromOutside) {
    if (toOutside) {
      state.push_back(State::START_AT_EDGE_STARTS_INSIDE_RADIUS_ENDS_OUTSIDE_RADIUS);
    } else if (!toOutside) {
      state.push_back(State::START_AT_EDGE_STARTS_INSIDE_RADIUS_ENDS_INSIDE_RADIUS);
    } else {
      state.push_back(State::START_AT_EDGE_STARTS_INSIDE_RADIUS);
    }
  } else {
    if (toOutside) {
      state.push_back(State::START_AT_EDGE_ENDS_OUTSIDE_RADIUS);
    } else if (!toOutside) {
      state.push_back(State::START_AT_EDGE_ENDS_INSIDE_RADIUS);
    } else {
      state.push_back(State::START_AT_EDGE);
    }
  }
}

template <typename Surface>
bool SaddleConnectionsIterator<Surface>::equal(const SaddleConnectionsIterator<Surface>& other) const {
  if (*self->connections.surface == *other.self->connections.surface && self->connections.lowerBound == other.self->connections.lowerBound && self->connections.searchRadius == other.self->connections.searchRadius && self->sector == other.self->sector && self->end == other.self->end) {
    if (self->sector == self->end)
      return true;

    return self->boundary[0] == other.self->boundary[0] && self->boundary[1] == other.self->boundary[1] && self->nextEdgeEnd == other.self->nextEdgeEnd && self->nextEdge == other.self->nextEdge;
  }

  return false;
}

template <typename Surface>
void SaddleConnectionsIterator<Surface>::increment() {
  // Run the search for saddle connections until we find a connection that is within the search bounds.
  while (!self->increment())
    ;
}

template <typename Surface>
void SaddleConnectionsIterator<Surface>::skipSector(CCW ccw) {
  self->skipSector(ccw);
}

template <typename Surface>
std::optional<HalfEdge> SaddleConnectionsIterator<Surface>::incrementWithCrossings() {
  auto intersection = incrementWithIntersections();

  if (intersection)
    return intersection->second;

  return std::nullopt;
}

template <typename Surface>
std::optional<std::pair<Chain<Surface>, HalfEdge>> SaddleConnectionsIterator<Surface>::incrementWithIntersections() {
  using Implementation = ImplementationOf<SaddleConnectionsIterator<Surface>>;
  LIBFLATSURF_ASSERT(self->sector != self->end, "iterator is at end()");

  while (true) {
    if (self->sector == self->end) {
      return std::nullopt;
    } else
      switch (self->state.back()) {
        case Implementation::State::START_AT_EDGE:
        case Implementation::State::START_AT_EDGE_STARTS_INSIDE_RADIUS_ENDS_INSIDE_RADIUS:
        case Implementation::State::START_AT_EDGE_STARTS_INSIDE_RADIUS:
        case Implementation::State::START_AT_EDGE_STARTS_INSIDE_RADIUS_ENDS_OUTSIDE_RADIUS:
        case Implementation::State::START_AT_EDGE_ENDS_INSIDE_RADIUS:
        case Implementation::State::START_AT_EDGE_ENDS_OUTSIDE_RADIUS:
        case Implementation::State::START_AT_EDGE_STARTS_OUTSIDE_RADIUS_ENDS_INSIDE_RADIUS:
        case Implementation::State::START_AT_EDGE_STARTS_OUTSIDE_RADIUS:
        case Implementation::State::START_AT_EDGE_STARTS_OUTSIDE_RADIUS_ENDS_OUTSIDE_RADIUS: {
          self->applyMoves();
          const auto crossing = self->nextEdge;
          const auto base = self->nextEdgeEnd - crossing;

          self->increment();
          return std::pair{base, crossing};
        }
        case ImplementationOf<SaddleConnectionsIterator>::State::SADDLE_CONNECTION_FOUND:
          return std::nullopt;
        default:
          self->increment();
      }
  }
}

template <typename Surface>
const SaddleConnection<Surface>& SaddleConnectionsIterator<Surface>::dereference() const {
  LIBFLATSURF_ASSERT(self->sector != self->end, "iterator is at end()");

  switch (self->state.back()) {
    case ImplementationOf<SaddleConnectionsIterator>::State::START_AT_EDGE:
      // This makes the first reported connection work: It is not nextEdgeEnd but the sector boundary.
      self->connection = SaddleConnection(*self->connections.surface, self->sector->source);
      break;
    case ImplementationOf<SaddleConnectionsIterator>::State::SADDLE_CONNECTION_FOUND:
      self->connection = SaddleConnection<Surface>(self->connections.surface, self->sector->source, self->connections.surface->previousAtVertex(-self->nextEdge), self->nextEdgeEnd);
      break;
    default:
      LIBFLATSURF_ASSERT(false, "iterator cannot be dereferenced in this state");
      break;
  }

  LIBFLATSURF_ASSERT(!self->connections.searchRadius || self->connection <= *self->connections.searchRadius, "Iterator stopped at connection " << self->connection << " which is beyond the search radius " << *self->connections.searchRadius);
  LIBFLATSURF_ASSERT(self->connection > self->connections.lowerBound, "Iterator stopped at connection " << self->connection << " which is within the excluded lower bound " << self->connections.lowerBound);

  return self->connection;
}

template <typename Surface>
std::ostream& operator<<(std::ostream& os, const SaddleConnectionsIterator<Surface>& self) {
  using T = typename Surface::Coordinate;
  using Implementation = ImplementationOf<SaddleConnectionsIterator<Surface>>;

  if (self.self->sector == self.self->end) {
    return os << "Iterator(END)";
  } else {
    return os << fmt::format("Iterator(sector={}, nextEdge={}, nextEdgeEnd={}, stack=[{}])", self.self->sector->source, self.self->nextEdge, static_cast<Vector<T>>(self.self->nextEdgeEnd), fmt::join(self.self->state | rx::transform([](const auto& state) {
      switch (state) {
        case Implementation::State::START_AT_EDGE_STARTS_INSIDE_RADIUS_ENDS_INSIDE_RADIUS:
          return "START_AT_EDGE_STARTS_INSIDE_RADIUS_ENDS_INSIDE_RADIUS";
        case Implementation::State::START_AT_EDGE_STARTS_INSIDE_RADIUS:
          return "START_AT_EDGE_STARTS_INSIDE_RADIUS";
        case Implementation::State::START_AT_EDGE_STARTS_INSIDE_RADIUS_ENDS_OUTSIDE_RADIUS:
          return "START_AT_EDGE_STARTS_INSIDE_RADIUS_ENDS_OUTSIDE_RADIUS";
        case Implementation::State::START_AT_EDGE_ENDS_INSIDE_RADIUS:
          return "START_AT_EDGE_ENDS_INSIDE_RADIUS";
        case Implementation::State::START_AT_EDGE:
          return "START_AT_EDGE";
        case Implementation::State::START_AT_EDGE_ENDS_OUTSIDE_RADIUS:
          return "START_AT_EDGE_ENDS_OUTSIDE_RADIUS";
        case Implementation::State::START_AT_EDGE_STARTS_OUTSIDE_RADIUS_ENDS_INSIDE_RADIUS:
          return "START_AT_EDGE_STARTS_OUTSIDE_RADIUS_ENDS_INSIDE_RADIUS";
        case Implementation::State::START_AT_EDGE_STARTS_OUTSIDE_RADIUS:
          return "START_AT_EDGE_STARTS_OUTSIDE_RADIUS";
        case Implementation::State::START_AT_EDGE_STARTS_OUTSIDE_RADIUS_ENDS_OUTSIDE_RADIUS:
          return "START_AT_EDGE_STARTS_OUTSIDE_RADIUS_ENDS_OUTSIDE_RADIUS";
        case Implementation::State::OUTSIDE_SEARCH_SECTOR_COUNTERCLOCKWISE:
          return "OUTSIDE_SEARCH_SECTOR_COUNTERCLOCKWISE";
        case Implementation::State::OUTSIDE_SEARCH_SECTOR_CLOCKWISE:
          return "OUTSIDE_SEARCH_SECTOR_COUNTERCLOCKWISE";
        case Implementation::State::SADDLE_CONNECTION_FOUND:
          return "SADDLE_CONNECTION_FOUND";
        case Implementation::State::SADDLE_CONNECTION_FOUND_SEARCHING_FIRST:
          return "SADDLE_CONNECTION_FOUND_SEARCHING_FIRST";
        case Implementation::State::SADDLE_CONNECTION_FOUND_SEARCHING_SECOND:
          return "SADDLE_CONNECTION_FOUND_SEARCHING_SECOND";
        case Implementation::State::END:
          return "END";
        default:
          throw std::logic_error("unknown State in operator<<");
      }
    }) | rx::to_vector(),
                                                                                                                                                                                                 ", "));
  }
}

}  // namespace flatsurf

// Instantiations of templates so implementations are generated for the linker
#include "util/instantiate.ipp"

LIBFLATSURF_INSTANTIATE_MANY_WRAPPED((LIBFLATSURF_INSTANTIATE_WITH_IMPLEMENTATION)(LIBFLATSURF_INSTANTIATE_HASH), SaddleConnectionsIterator, LIBFLATSURF_FLAT_TRIANGULATION_TYPES)
