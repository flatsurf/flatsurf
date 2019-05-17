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

#include <stack>

#include "flatsurf/assert.hpp"
#include "flatsurf/ccw.hpp"
#include "flatsurf/flat_triangulation.hpp"
#include "flatsurf/half_edge.hpp"
#include "flatsurf/saddle_connections.hpp"
#include "flatsurf/saddle_connections_iterator.hpp"

#include "saddle_connection.ipp"

namespace flatsurf {
template <typename Vector, typename VectorAlongTriangulation>
struct SaddleConnectionsIterator<Vector,
                                 VectorAlongTriangulation>::Implementation {
  enum class Classification {
    OUTSIDE_SEARCH_SECTOR_CLOCKWISE,
    SADDLE_CONNECTION,
    OUTSIDE_SEARCH_SECTOR_COUNTERCLOCKWISE,
  };

  enum class State {
    START,
    OUTSIDE_SEARCH_SECTOR_CLOCKWISE_SEARCHING,
    OUTSIDE_SEARCH_SECTOR_COUNTERCLOCKWISE_SEARCHING,
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

  explicit Implementation(const FlatTriangulation<Vector> &surface,
                          const Bound searchRadius,
                          const HalfEdge sectorBoundary,
                          const VectorAlongTriangulation &sectorBegin,
                          const VectorAlongTriangulation &sectorEnd)
      : surface(&surface),
        searchRadius(searchRadius),
        source(-surface.nextAtVertex(sectorBoundary)),
        sector{sectorBegin, sectorEnd},
        nextEdge(sectorBoundary),
        nextEdgeEnd(sectorEnd),
        state({State::END, State::START}) {
    // Since we don't call increment(), we report sector[1] as a saddle
    // connection (which is intentional.)
    if (nextEdgeEnd > searchRadius) {
      increment();
    }
  }

  FlatTriangulation<Vector> const *surface;
  Bound searchRadius;
  HalfEdge source;
  // The rays that enclose the search sector, in counterclockwise order.
  // They themselves come from saddle connections, starting at base and
  // pointing to a vertex of the flatsurf.
  VectorAlongTriangulation sector[2];
  // The half-edge that we are about to cross, seen from the origin, i.e.,
  // oriented so that it starts on the side of sector[0]
  HalfEdge nextEdge;
  // The vector to the target of nextEdge
  VectorAlongTriangulation nextEdgeEnd;

  std::stack<State> state;
  std::deque<Move> moves;
  std::stack<VectorAlongTriangulation> tmp;

  bool atEnd() const { return state.top() == State::END; }

  bool increment() {
    assert(sector[0].ccw(sector[1]) == CCW::COUNTERCLOCKWISE);

    const auto s = state.top();
    state.pop();
    // As of early 2019, C++ lacks stackless coroutines. This code would be much
    // more readable with async/await idioms. And likely faster.
    // Unfortunately, boost's coroutines are not stackless and therefore too
    // slow, and co2 exceeds the stack size quickly, so we have to roll our own
    // stack for the time being.
    // At the same time, we cannot make recursive calls inside this function,
    // i.e., "return increment()" since that also exceeds the stack size for
    // (much larger) radii. (Strangely, GCC, as of early 2019, does not
    // optimize such tail recursion.)
    switch (s) {
      case State::END:
        assert(state.size() == 0);
        assert(tmp.size() == 0);
        state.push(State::END);
        return true;
      case State::START:
        moves.push_back(Move::GOTO_OTHER_FACE);
        moves.push_back(Move::GOTO_NEXT_EDGE);

        applyMoves();
        switch (classifyHalfEdgeEnd()) {
          case Classification::OUTSIDE_SEARCH_SECTOR_CLOCKWISE:
            // Since this vertex is outside of the search sector on the
            // clockwise side, we skip the clockwise sector in the recursive
            // search and recurse into the counterclockwise sector.
            moves.push_back(Move::GOTO_NEXT_EDGE);
            // Note that the following is a nop that just exists for symmetry.
            state.push(State::OUTSIDE_SEARCH_SECTOR_CLOCKWISE_SEARCHING);
            state.push(State::START);
            return false;
          case Classification::OUTSIDE_SEARCH_SECTOR_COUNTERCLOCKWISE:
            // Similarly, we skip the counterclockwise sector.
            state.push(State::OUTSIDE_SEARCH_SECTOR_COUNTERCLOCKWISE_SEARCHING);
            state.push(State::START);
            return false;
          case Classification::SADDLE_CONNECTION:
            state.push(State::SADDLE_CONNECTION_FOUND);
            if (!(nextEdgeEnd > searchRadius)) {
              // Report this saddle connection.
              return true;
            } else {
              // If the vertex is beyond the search radius, we do not report
              // this new saddle connection. If additionaly, the other vertices
              // of this triangle had already been outside of the search radius,
              // we abort the search here.
              bool baseIsAlreadyExceedingSearchRadius = true;
              moves.push_back(Move::GOTO_NEXT_EDGE);
              applyMoves();
              baseIsAlreadyExceedingSearchRadius &=
                  (nextEdgeEnd > searchRadius);
              moves.push_back(Move::GOTO_NEXT_EDGE);
              applyMoves();
              baseIsAlreadyExceedingSearchRadius &=
                  (nextEdgeEnd > searchRadius);
              if (baseIsAlreadyExceedingSearchRadius) {
                // The other vertices of the triangle are outside of the search
                // radius; abort the search here, i.e., backtrack.
                moves.push_back(Move::GOTO_OTHER_FACE);
                state.pop();
              } else {
                // One of the vertices is inside the search radius; continue the
                // search.
                moves.push_back(Move::GOTO_NEXT_EDGE);
              }
              return false;
            }
        }
        throw std::logic_error("impossible to happen");
      case State::SADDLE_CONNECTION_FOUND:
        // We have just reported a saddle connection; now we prepare
        // the recursive descend into the clockwise sector.
        tmp.push(sector[1]);
        applyMoves();
        sector[1] = nextEdgeEnd;
        state.push(State::SADDLE_CONNECTION_FOUND_SEARCHING_SECOND);
        state.push(State::START);
        state.push(State::SADDLE_CONNECTION_FOUND_SEARCHING_FIRST);
        state.push(State::START);
        return false;
      case State::SADDLE_CONNECTION_FOUND_SEARCHING_FIRST:
        // We have just come back from the search in the clockwise sector; now
        // we prepare the recursive descend into the counterclockwise sector.
        sector[1] = tmp.top();
        tmp.pop();
        tmp.push(sector[0]);
        applyMoves();
        sector[0] = nextEdgeEnd;
        moves.push_back(Move::GOTO_NEXT_EDGE);
        return false;
      case State::SADDLE_CONNECTION_FOUND_SEARCHING_SECOND:
        // We have just come back from the search in the counterclockwise
        // sector; we are done here and return in the recursion.
        sector[0] = tmp.top();
        tmp.pop();
        break;
      case State::OUTSIDE_SEARCH_SECTOR_COUNTERCLOCKWISE_SEARCHING:
        moves.push_back(Move::GOTO_NEXT_EDGE);
        break;
      case State::OUTSIDE_SEARCH_SECTOR_CLOCKWISE_SEARCHING:
        break;
    }

    moves.push_back(Move::GOTO_NEXT_EDGE);
    moves.push_back(Move::GOTO_OTHER_FACE);
    return false;
  }

  void skipSector(CCW sector) {
    ASSERT_ARGUMENT(sector != CCW::COLLINEAR,
                    "There is no such thing like a collinear sector.");

    if (state.top() == State::SADDLE_CONNECTION_FOUND) {
      increment();

      if (sector == CCW::CLOCKWISE) {
        // Go directly to the second sector by skipping the recursive call,
        // i.e., the START.
        assert(state.top() == State::START);
        state.pop();

        assert(state.top() == State::SADDLE_CONNECTION_FOUND_SEARCHING_FIRST);
      } else if (sector == CCW::COUNTERCLOCKWISE) {
        assert(state.top() == State::START);
        state.pop();

        assert(state.top() == State::SADDLE_CONNECTION_FOUND_SEARCHING_FIRST);
        state.pop();

        assert(state.top() == State::START);
        // Skip the second recursive call by dropping its START.
        state.pop();

        // And push the rest back on the stack unchanged.
        state.push(State::SADDLE_CONNECTION_FOUND_SEARCHING_FIRST);
        state.push(State::START);
      }
    } else if (state.top() == State::START && state.size() == 2) {
      if (sector == CCW::CLOCKWISE) {
        // We are in the initial state, the reported saddle connection is on
        // the counterclockwise end of the search vector. If we skip the
        // clockwise sector, then we skip everything.
        state.pop();
        assert(state.top() == State::END);
      } else {
        // We are skipping the counterclockwise sector anyway.
        ;
      }
    } else {
      throw std::logic_error(
          "sectors can only be skipped when a saddle connection has been "
          "reported");
    }
  }

  void apply(const Move m) {
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

  void applyMoves() {
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
        case Move::GOTO_OTHER_FACE:
          switch (n) {
            case Move::GOTO_NEXT_EDGE:
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wimplicit-fallthrough"
              moves.push_front(Move::GOTO_PREVIOUS_EDGE);
#pragma GCC diagnostic pop
              // fallthrough to the next case intended here
            case Move::GOTO_PREVIOUS_EDGE:
              nextEdge = -nextEdge;
              nextEdge = surface->nextAtVertex(nextEdge);
              nextEdge = -nextEdge;
              continue;
            case Move::GOTO_OTHER_FACE:
              continue;
          }
      }
    }
  }

  Classification classifyHalfEdgeEnd() const {
    switch (sector[0].ccw(nextEdgeEnd)) {
      case CCW::CLOCKWISE:
      case CCW::COLLINEAR:
        return Classification::OUTSIDE_SEARCH_SECTOR_CLOCKWISE;
      case CCW::COUNTERCLOCKWISE:
        switch (sector[1].ccw(nextEdgeEnd)) {
          case CCW::CLOCKWISE:
            return Classification::SADDLE_CONNECTION;
          case CCW::COUNTERCLOCKWISE:
          case CCW::COLLINEAR:
            return Classification::OUTSIDE_SEARCH_SECTOR_COUNTERCLOCKWISE;
        }
    }
    throw std::logic_error("impossible to happen");
  }
};

template <typename Vector, typename VectorAlongTriangulation>
SaddleConnectionsIterator<Vector,
                          VectorAlongTriangulation>::SaddleConnectionsIterator()
    : impl(nullptr) {}

template <typename Vector, typename VectorAlongTriangulation>
bool SaddleConnectionsIterator<Vector, VectorAlongTriangulation>::equal(
    const SaddleConnectionsIterator<Vector, VectorAlongTriangulation> &other)
    const {
  if (!impl || impl->atEnd()) {
    return !other.impl || other.impl->atEnd();
  } else {
    return false;
  }
}

template <typename Vector, typename VectorAlongTriangulation>
void SaddleConnectionsIterator<Vector, VectorAlongTriangulation>::skipSector(
    CCW ccw) {
  impl->skipSector(ccw);
}

template <typename Vector, typename VectorAlongTriangulation>
void SaddleConnectionsIterator<Vector, VectorAlongTriangulation>::increment() {
  while (!impl->increment())
    ;
}

template <typename Vector, typename VectorAlongTriangulation>
std::optional<HalfEdge> SaddleConnectionsIterator<
    Vector, VectorAlongTriangulation>::incrementWithCrossings() {
  while (true) {
    if (impl->state.top() == Implementation::State::START) {
      impl->applyMoves();
      auto ret = impl->nextEdge;
      impl->increment();
      return ret;
    } else if (impl->state.top() ==
               Implementation::State::SADDLE_CONNECTION_FOUND) {
      return {};
    } else {
      impl->increment();
    }
  }
}

template <typename Vector, typename VectorAlongTriangulation>
std::unique_ptr<SaddleConnection<Vector, VectorAlongTriangulation>>
SaddleConnectionsIterator<Vector, VectorAlongTriangulation>::dereference()
    const {
  auto ret = std::make_unique<SaddleConnection<Vector, VectorAlongTriangulation>>();
  ret->impl = spimpl::make_impl<typename SaddleConnection<Vector, VectorAlongTriangulation>::Implementation>(
      *impl->surface, impl->nextEdgeEnd, impl->source, impl->nextEdge);
  return ret;
}
}  // namespace flatsurf

// Instantions of templates so implementations are generated for the linker
#include <exact-real/number_field_traits.hpp>

#include "flatsurf/vector_eantic.hpp"
#include "flatsurf/vector_exactreal.hpp"
#include "flatsurf/vector_longlong.hpp"
using namespace flatsurf;

template struct flatsurf::SaddleConnectionsIterator<VectorLongLong>;
template struct flatsurf::SaddleConnectionsIterator<VectorEAntic>;
template struct flatsurf::SaddleConnectionsIterator<
    VectorExactReal<exactreal::NumberFieldTraits>>;
template struct flatsurf::SaddleConnectionsIterator<
    VectorEAntic, VectorAlongTriangulation<VectorEAntic>>;
