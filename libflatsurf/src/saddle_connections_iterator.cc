/**********************************************************************
 *  This file is part of flatsurf.
 *
 *        Copyright (C) 2020 Julian Rüth
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

#include <exact-real/arb.hpp>
#include <vector>

#include "../flatsurf/chain.hpp"
#include "../flatsurf/fmt.hpp"
#include "../flatsurf/saddle_connection.hpp"
#include "../flatsurf/vector.hpp"
#include "external/rx-ranges/include/rx/ranges.hpp"
#include "impl/saddle_connections_iterator.impl.hpp"
#include "util/assert.ipp"

namespace flatsurf {

using std::vector;

template <typename Surface>
ImplementationOf<SaddleConnectionsIterator<Surface>>::ImplementationOf(const std::shared_ptr<const Surface>& surface, const Bound searchRadius, const vector<HalfEdge>::const_iterator begin, const vector<HalfEdge>::const_iterator end) :
  surface(std::move(surface)),
  searchRadius(searchRadius),
  sector(begin),
  end(end),
  boundary{Chain(this->surface), Chain(this->surface)},
  nextEdgeEnd(this->surface),
  connection(SaddleConnection(this->surface, surface->halfEdges()[0])) {
  prepareSearch();
}

template <typename Surface>
void ImplementationOf<SaddleConnectionsIterator<Surface>>::prepareSearch() {
  assert(state.size() == 0);
  assert(tmp.size() == 0);
  assert(moves.size() == 0);

  if (sector == end) {
    return;
  }

  const HalfEdge e = *sector;

  if (surface->boundary(e)) {
    sector++;
    prepareSearch();
    return;
  }

  boundary[0] = (Chain(surface) + e);
  nextEdge = surface->nextInFace(e);
  boundary[1] = boundary[0] + nextEdge;
  nextEdgeEnd = (Chain<Surface>(surface) += e) += nextEdge;
  state.push_back(State::END);
  state.push_back(State::START_FROM_INSIDE_TO_INSIDE);

  // Report sector as a saddle connection unless it is already outside of
  // the search radius.
  if (boundary[0] > searchRadius) {
    while (!increment())
      ;
  }
}

template <typename Surface>
CCW ImplementationOf<SaddleConnectionsIterator<Surface>>::ccw(const Chain<Surface>& lhs, const Chain<Surface>& rhs) {
  auto ccw = static_cast<const Vector<exactreal::Arb>&>(lhs).ccw(static_cast<const Vector<exactreal::Arb>&>(rhs));
  if (ccw) return *ccw;
  return static_cast<const Vector<T>&>(lhs).ccw(static_cast<const Vector<T>&>(rhs));
}

template <typename Surface>
bool ImplementationOf<SaddleConnectionsIterator<Surface>>::increment() {
  assert(state.size());
  assert(sector != end);
  assert(ccw(boundary[0], boundary[1]) == CCW::COUNTERCLOCKWISE);

  applyMoves();

  const auto s = state.back();
  state.pop_back();
  switch (s) {
    case State::END:
      applyMoves();
      sector++;
      if (sector != end) {
        prepareSearch();
      }
      return true;
    case State::START_FROM_INSIDE_TO_INSIDE:
    case State::START_FROM_INSIDE_TO_OUTSIDE:
    case State::START_FROM_OUTSIDE_TO_INSIDE:
      moves.push_back(Move::GOTO_OTHER_FACE);

      if (onBoundary()) {
        moves.push_back(Move::GOTO_OTHER_FACE);
        return false;
      }

      moves.push_back(Move::GOTO_NEXT_EDGE);

      switch (classifyHalfEdgeEnd()) {
        case Classification::OUTSIDE_SEARCH_SECTOR_CLOCKWISE: {
          const bool nothingBeyondThisVertex = !(nextEdgeEnd < searchRadius);

          // This vertex is outside of the search sector on the
          // clockwise side, we skip the clockwise sector in the recursive
          // search and recurse into the counterclockwise sector.
          moves.push_back(Move::GOTO_NEXT_EDGE);

          state.push_back(State::OUTSIDE_SEARCH_SECTOR_CLOCKWISE);

          pushStart(nothingBeyondThisVertex, s == State::START_FROM_INSIDE_TO_OUTSIDE);

          return false;
        }
        case Classification::OUTSIDE_SEARCH_SECTOR_COUNTERCLOCKWISE: {
          const bool nothingBeyondThisVertex = !(nextEdgeEnd < searchRadius);

          // Similarly, we skip the counterclockwise sector (but only after
          // we visited the clockwise one.)
          state.push_back(State::OUTSIDE_SEARCH_SECTOR_COUNTERCLOCKWISE);

          pushStart(s == State::START_FROM_OUTSIDE_TO_INSIDE, nothingBeyondThisVertex);

          return false;
        }
        case Classification::SADDLE_CONNECTION: {
          const bool beyondRadius = nextEdgeEnd > searchRadius;

          // Prepare the recursive descent into the two half edges attached
          // to this vertex.
          state.push_back(State::SADDLE_CONNECTION_FOUND_SEARCHING_SECOND);
          pushStart(beyondRadius, s == State::START_FROM_INSIDE_TO_OUTSIDE);
          state.push_back(State::SADDLE_CONNECTION_FOUND_SEARCHING_FIRST);
          pushStart(s == State::START_FROM_OUTSIDE_TO_INSIDE, beyondRadius);

          // Shrink the search sector for the clockwise descent.
          tmp.push(std::move(boundary[1]));
          boundary[1] = nextEdgeEnd;

          if (beyondRadius) {
            return false;
          } else {
            state.push_back(State::SADDLE_CONNECTION_FOUND);
            return true;
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
      tmp.push(std::move(boundary[0]));
      applyMoves();
      boundary[0] = nextEdgeEnd;
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
      throw std::logic_error("unknown State");
  }
}

template <typename Surface>
bool ImplementationOf<SaddleConnectionsIterator<Surface>>::onBoundary() {
  // This code path could be optimized away by adding a bool template
  // parameter hasBoundary to increment().
  applyMoves();
  return surface->boundary(nextEdge);
}

template <typename Surface>
void ImplementationOf<SaddleConnectionsIterator<Surface>>::skipSector(CCW sector) {
  ASSERT_ARGUMENT(sector != CCW::COLLINEAR, "Cannot skip this sector. There is no such thing as a collinear sector.");
  assert(state.size() && "cannot skip a sector in a completed search");

  switch (state.back()) {
    case State::SADDLE_CONNECTION_FOUND:
      state.pop_back();

      if (sector == CCW::CLOCKWISE) {
        // Go directly to the second sector by skipping the recursive call,
        // i.e., the START.
        switch (state.back()) {
          case State::START_FROM_INSIDE_TO_INSIDE:
          case State::START_FROM_INSIDE_TO_OUTSIDE:
          case State::START_FROM_OUTSIDE_TO_INSIDE:
            state.pop_back();
            break;
          default:
            break;
        }
        ASSERT(state.back() == State::SADDLE_CONNECTION_FOUND_SEARCHING_FIRST, "State machine of SaddleConnections is inconsistent when trying to skip clockwise sector.");
      } else if (sector == CCW::COUNTERCLOCKWISE) {
        // Skip the second recursive call by dropping its START.
        std::stack<State> unchanged;
        while (state.back() != State::SADDLE_CONNECTION_FOUND_SEARCHING_SECOND) {
          unchanged.push(state.back());
          state.pop_back();
        }

        switch (unchanged.top()) {
          case State::START_FROM_INSIDE_TO_INSIDE:
          case State::START_FROM_INSIDE_TO_OUTSIDE:
          case State::START_FROM_OUTSIDE_TO_INSIDE:
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
    case State::START_FROM_INSIDE_TO_INSIDE:
    case State::START_FROM_OUTSIDE_TO_INSIDE:
    case State::START_FROM_INSIDE_TO_OUTSIDE:
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
      nextEdge = surface->nextInFace(nextEdge);
      nextEdgeEnd += nextEdge;
      break;
    case Move::GOTO_OTHER_FACE:
      nextEdge = -nextEdge;
      nextEdgeEnd += nextEdge;
      break;
    case Move::GOTO_PREVIOUS_EDGE:
      nextEdgeEnd -= nextEdge;
      nextEdge = surface->nextAtVertex(nextEdge);
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
            nextEdge = surface->nextInFace(nextEdge);
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
            nextEdge = surface->nextAtVertex(nextEdge);
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
    case CCW::COLLINEAR:
      return Classification::OUTSIDE_SEARCH_SECTOR_CLOCKWISE;
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
void ImplementationOf<SaddleConnectionsIterator<Surface>>::pushStart(bool fromOutside, bool toOutside) {
  if (fromOutside) {
    if (toOutside) {
      ;
    } else {
      state.push_back(State::START_FROM_OUTSIDE_TO_INSIDE);
    }
  } else {
    if (toOutside) {
      state.push_back(State::START_FROM_INSIDE_TO_OUTSIDE);
    } else {
      state.push_back(State::START_FROM_INSIDE_TO_INSIDE);
    }
  }
}

template <typename Surface>
bool SaddleConnectionsIterator<Surface>::equal(const SaddleConnectionsIterator<Surface>& other) const {
  if (impl->surface == other.impl->surface && impl->searchRadius == other.impl->searchRadius && impl->sector == other.impl->sector && impl->end == other.impl->end) {
    if (impl->sector == impl->end)
      return true;

    return impl->boundary[0] == other.impl->boundary[0] && impl->boundary[1] == other.impl->boundary[1] && impl->nextEdgeEnd == other.impl->nextEdgeEnd && impl->nextEdge == other.impl->nextEdge;
  }

  return false;
}

template <typename Surface>
void SaddleConnectionsIterator<Surface>::increment() {
  while (!impl->increment())
    ;
}

template <typename Surface>
void SaddleConnectionsIterator<Surface>::skipSector(CCW ccw) {
  impl->skipSector(ccw);
}

template <typename Surface>
std::optional<HalfEdge> SaddleConnectionsIterator<Surface>::incrementWithCrossings() {
  ASSERT(impl->sector != impl->end, "iterator is at end()");

  while (true) {
    if (impl->sector == impl->end) {
      return std::nullopt;
    } else
      switch (impl->state.back()) {
        case Implementation::State::START_FROM_INSIDE_TO_INSIDE:
        case Implementation::State::START_FROM_INSIDE_TO_OUTSIDE:
        case Implementation::State::START_FROM_OUTSIDE_TO_INSIDE: {
          impl->applyMoves();
          const auto ret = impl->nextEdge;
          impl->increment();
          return ret;
        }
        case Implementation::State::SADDLE_CONNECTION_FOUND:
          return std::nullopt;
        default:
          impl->increment();
      }
  }
}

template <typename Surface>
const SaddleConnection<Surface>& SaddleConnectionsIterator<Surface>::dereference() const {
  ASSERT(impl->sector != impl->end, "iterator is at end()");

  switch (impl->state.back()) {
    case Implementation::State::START_FROM_INSIDE_TO_INSIDE:
      // This makes the first reported connection work: It is not nextEdgeEnd but the sector boundary.
      impl->connection = SaddleConnection(impl->surface, *impl->sector);
      break;
    case Implementation::State::SADDLE_CONNECTION_FOUND:
      impl->connection = SaddleConnection<Surface>(impl->surface, *impl->sector, impl->surface->previousAtVertex(-impl->nextEdge), impl->nextEdgeEnd);
      break;
    default:
      ASSERT(false, "iterator cannot hold in this state");
  }

  ASSERT(impl->connection <= impl->searchRadius, "Iterator stopped at connection " << impl->connection << " which is beyond the search radius " << impl->searchRadius);

  return impl->connection;
}

template <typename Surface>
std::ostream& operator<<(std::ostream& os, const SaddleConnectionsIterator<Surface>& self) {
  using T = typename Surface::Coordinate;
  using Implementation = typename SaddleConnectionsIterator<Surface>::Implementation;

  if (self.impl->sector == self.impl->end) {
    return os << "Iterator(END)";
  } else {
    return os << fmt::format("Iterator(sector={}, boundary=({}, {}), nextEdge={}, nextEdgeEnd={}, stack=[{}])", *self.impl->sector, static_cast<Vector<T>>(self.impl->boundary[0]), static_cast<Vector<T>>(self.impl->boundary[1]), self.impl->nextEdge, static_cast<Vector<T>>(self.impl->nextEdgeEnd), fmt::join(self.impl->state | rx::transform([](const auto& state) {
      switch (state) {
        case Implementation::State::START_FROM_INSIDE_TO_INSIDE:
          return "START_FROM_INSIDE_TO_INSIDE";
        case Implementation::State::START_FROM_INSIDE_TO_OUTSIDE:
          return "START_FROM_INSIDE_TO_OUTSIDE";
        case Implementation::State::START_FROM_OUTSIDE_TO_INSIDE:
          return "START_FROM_OUTSIDE_TO_INSIDE";
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
          throw std::logic_error("unknown State");
      }
    }) | rx::to_vector(),
                                                                                                                                                                                                                                                                                                             ", "));
  }
}

}  // namespace flatsurf

// Instantiations of templates so implementations are generated for the linker
#include "util/instantiate.ipp"

LIBFLATSURF_INSTANTIATE_MANY_WRAPPED((LIBFLATSURF_INSTANTIATE_WITH_IMPLEMENTATION)(LIBFLATSURF_INSTANTIATE_HASH), SaddleConnectionsIterator, LIBFLATSURF_FLAT_TRIANGULATION_TYPES)
