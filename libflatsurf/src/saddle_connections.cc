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

#include <exact-real/arb.hpp>
#include <intervalxt/length.hpp>
#include <stack>

#include "flatsurf/flat_triangulation.hpp"
#include "flatsurf/half_edge.hpp"
#include "flatsurf/saddle_connection.hpp"
#include "flatsurf/saddle_connections.hpp"
#include "flatsurf/vector.hpp"
#include "flatsurf/vector_along_triangulation.hpp"

#include "util/assert.ipp"

using std::vector;
namespace {
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
}  // namespace

namespace flatsurf {
template <typename Surface>
class SaddleConnections<Surface>::Implementation {
 public:
  Implementation(Iterator&& begin) noexcept : begin(std::move(begin)) {}

  Iterator begin;
};

template <typename Surface>
class SaddleConnections<Surface>::Iterator::Implementation {
  using AlongTriangulation = VectorAlongTriangulation<typename Surface::Vector::Coordinate, std::conditional_t<std::is_same_v<typename Surface::Vector::Coordinate, long long>, void, exactreal::Arb>>;

 public:
  Implementation(const std::shared_ptr<const Surface>& surface, const Bound searchRadius, const vector<HalfEdge> searchSectors) : surface(std::move(surface)), searchRadius(searchRadius), sectors(std::move(searchSectors)), sector(0), boundary{AlongTriangulation(this->surface), AlongTriangulation(this->surface)}, nextEdgeEnd(AlongTriangulation(this->surface)) {
    prepareSearch();
  }

  void prepareSearch() {
    assert(state.size() == 0);
    assert(tmp.size() == 0);
    assert(moves.size() == 0);

    if (sector == sectors.size()) {
      return;
    }

    const HalfEdge e = sectors[sector];

    if (surface->boundary(e)) {
      sector++;
      prepareSearch();
      return;
    }

    boundary[0] = AlongTriangulation(surface, vector<HalfEdge>{e});
    nextEdge = surface->nextInFace(e);
    boundary[1] = boundary[0] + nextEdge;
    nextEdgeEnd = boundary[1];
    state.push(State::END);
    state.push(State::START);

    // Report nextEdgeEnd as a saddle connection unless it's already outside
    // of the search radius.
    if (nextEdgeEnd > searchRadius) {
      while (!increment())
        ;
    }
  }

  std::shared_ptr<const Surface> surface;
  const Bound searchRadius;
  const vector<HalfEdge> sectors;
  // The half edge nextEdge, to which we are currently changing, points into
  // sectors. Advanced when we are done searching an entire such sector for all
  // saddle connections. (An index into sectors.)
  size_t sector;

  // The rays that enclose the search sector, in counterclockwise order. They
  // themselves come from saddle connections, starting at the search origin and
  // pointing to a vertex of the flat triangulation.
  AlongTriangulation boundary[2];
  // The half-edge that we are about to cross, seen from the search origin,
  // i.e., oriented so that it starts on the side of boundary[0]
  HalfEdge nextEdge;
  // The vector to the target of nextEdge
  AlongTriangulation nextEdgeEnd;

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
  std::stack<State> state;

  // We collect pending moves across the surface here (adding half edges to
  // nextEdgeEnd mostly.) When the exact value of nextEdgeEnd is required, we
  // can often combine several move more efficiently, see applyMoves().
  std::deque<Move> moves;

  // Storage space for temporary values of boundary, when we descend
  // recursively into a subsector.
  std::stack<AlongTriangulation> tmp;

  bool increment() {
    assert(state.size());
    assert(sector != sectors.size());
    assert(boundary[0].ccw(boundary[1]) == CCW::COUNTERCLOCKWISE);

    const auto s = state.top();
    state.pop();
    switch (s) {
      case State::END:
        applyMoves();
        sector++;
        if (sector != sectors.size()) {
          prepareSearch();
        }
        return true;
      case State::START:
        moves.push_back(Move::GOTO_OTHER_FACE);
        if (onBoundary()) {
          moves.push_back(Move::GOTO_OTHER_FACE);
          return false;
        }
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
        tmp.push(boundary[1]);
        applyMoves();
        boundary[1] = nextEdgeEnd;
        state.push(State::SADDLE_CONNECTION_FOUND_SEARCHING_SECOND);
        state.push(State::START);
        state.push(State::SADDLE_CONNECTION_FOUND_SEARCHING_FIRST);
        state.push(State::START);
        return false;
      case State::SADDLE_CONNECTION_FOUND_SEARCHING_FIRST:
        // We have just come back from the search in the clockwise sector; now
        // we prepare the recursive descend into the counterclockwise sector.
        boundary[1] = tmp.top();
        tmp.pop();
        tmp.push(boundary[0]);
        applyMoves();
        boundary[0] = nextEdgeEnd;
        moves.push_back(Move::GOTO_NEXT_EDGE);
        return false;
      case State::SADDLE_CONNECTION_FOUND_SEARCHING_SECOND:
        // We have just come back from the search in the counterclockwise
        // sector; we are done here and return in the recursion.
        boundary[0] = tmp.top();
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

  bool onBoundary() {
    // This code path could be optimized away by adding a bool template
    // parameter hasBoundary to increment().
    applyMoves();
    return surface->boundary(nextEdge);
  }

  void skipSector(CCW sector) {
    ASSERT_ARGUMENT(sector != CCW::COLLINEAR,
                    "There is no such thing like a collinear sector.");
    assert(state.size() && "cannot skip a sector in a completed search");

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
    switch (boundary[0].ccw(nextEdgeEnd)) {
      case CCW::CLOCKWISE:
      case CCW::COLLINEAR:
        return Classification::OUTSIDE_SEARCH_SECTOR_CLOCKWISE;
      case CCW::COUNTERCLOCKWISE:
        switch (boundary[1].ccw(nextEdgeEnd)) {
          case CCW::CLOCKWISE:
            return Classification::SADDLE_CONNECTION;
          case CCW::COUNTERCLOCKWISE:
          case CCW::COLLINEAR:
            return Classification::OUTSIDE_SEARCH_SECTOR_COUNTERCLOCKWISE;
        }
    }
    throw std::logic_error("impossible to happen");
  }

  friend std::ostream& operator<<(std::ostream& os, const Implementation& self) {
    if (self.sector == self.sectors.size()) {
      return os << "Iterator(END)";
    }
    return os << "Iterator(sector = " << self.sectors[self.sector] << ", connection = " << self.nextEdgeEnd << ")";
  }
};

template <typename Surface>
SaddleConnections<Surface>::SaddleConnections(const std::shared_ptr<const Surface>& surface, const Bound searchRadius)
    : impl(spimpl::make_impl<Implementation>(spimpl::make_impl<typename Iterator::Implementation>(surface, searchRadius, surface->halfEdges()))) {}

template <typename Surface>
SaddleConnections<Surface>::SaddleConnections(const std::shared_ptr<const Surface>& surface, const Bound searchRadius, const Vertex source)
    : impl(spimpl::make_impl<Implementation>(spimpl::make_impl<typename Iterator::Implementation>(surface, searchRadius, surface->atVertex(source)))) {}

template <typename Surface>
SaddleConnections<Surface>::SaddleConnections(const std::shared_ptr<const Surface>& surface, const Bound searchRadius, const HalfEdge sectorBegin)
    : impl(spimpl::make_impl<Implementation>(spimpl::make_impl<typename Iterator::Implementation>(surface, searchRadius, vector<HalfEdge>{sectorBegin}))) {}

template <typename Surface>
typename SaddleConnections<Surface>::Iterator SaddleConnections<Surface>::begin() const {
  return impl->begin;
}

template <typename Surface>
typename SaddleConnections<Surface>::Iterator SaddleConnections<Surface>::end() const {
  Iterator ret = impl->begin;
  ret.impl->sector = ret.impl->sectors.size();
  return ret;
}

template <typename Surface>
SaddleConnections<Surface>::Iterator::Iterator(spimpl::impl_ptr<Implementation>&& impl) : impl(std::move(impl)) {}

template <typename Surface>
bool SaddleConnections<Surface>::Iterator::equal(const SaddleConnections<Surface>::Iterator& other) const {
  if (impl->surface != other.impl->surface || impl->sectors != other.impl->sectors || impl->searchRadius != other.impl->searchRadius || impl->sector != other.impl->sector)
    return false;

  if (impl->sector == impl->sectors.size())
    return true;

  return impl->boundary[0] == other.impl->boundary[0] && impl->boundary[1] == other.impl->boundary[1] && impl->nextEdgeEnd == other.impl->nextEdgeEnd && impl->nextEdge == other.impl->nextEdge;
}

template <typename Surface>
void SaddleConnections<Surface>::Iterator::increment() {
  while (!impl->increment())
    ;
}

template <typename Surface>
void SaddleConnections<Surface>::Iterator::skipSector(CCW ccw) {
  impl->skipSector(ccw);
}

template <typename Surface>
std::optional<HalfEdge> SaddleConnections<Surface>::Iterator::incrementWithCrossings() {
  assert(impl->sector != impl->sectors.size() && "cannot increment beyond the end");

  while (true) {
    if (impl->sector == impl->sectors.size()) {
      return {};
    } else if (impl->state.top() == State::START) {
      impl->applyMoves();
      auto ret = impl->nextEdge;
      impl->increment();
      return ret;
    } else if (impl->state.top() == State::SADDLE_CONNECTION_FOUND) {
      return {};
    } else {
      impl->increment();
    }
  }
}

template <typename Surface>
std::unique_ptr<SaddleConnection<Surface>> SaddleConnections<Surface>::Iterator::dereference() const {
  if (impl->sector == impl->sectors.size()) {
    throw std::out_of_range("iterator is at end()");
  }
  return std::unique_ptr<SaddleConnection<Surface>>(new SaddleConnection<Surface>(impl->surface, impl->sectors[impl->sector], impl->nextEdge, static_cast<typename Surface::Vector>(impl->nextEdgeEnd)));
}

template <typename Surface>
std::ostream& operator<<(std::ostream& os, const SaddleConnections<Surface>&) {
  return os << "SaddleConnections()";
}
}  // namespace flatsurf

// Instantiations of templates so implementations are generated for the linker
#include <e-antic/renfxx_fwd.h>
#include <exact-real/integer_ring.hpp>
#include <exact-real/number_field.hpp>
#include <exact-real/rational_field.hpp>

namespace flatsurf {
// We need to explicitly list the operator<< implementations here, since we
// cannot use a template:
// https://stackoverflow.com/questions/18823618/overload-operator-for-nested-class-template
std::ostream& operator<<(std::ostream& os, const typename SaddleConnections<FlatTriangulation<long long>>::Iterator& self) { return os << *self.impl; }
std::ostream& operator<<(std::ostream& os, const typename SaddleConnections<FlatTriangulation<eantic::renf_elem_class>>::Iterator& self) { return os << *self.impl; }
std::ostream& operator<<(std::ostream& os, const typename SaddleConnections<FlatTriangulation<exactreal::Element<exactreal::IntegerRing>>>::Iterator& self) { return os << *self.impl; }
std::ostream& operator<<(std::ostream& os, const typename SaddleConnections<FlatTriangulation<exactreal::Element<exactreal::RationalField>>>::Iterator& self) { return os << *self.impl; }
std::ostream& operator<<(std::ostream& os, const typename SaddleConnections<FlatTriangulation<exactreal::Element<exactreal::NumberField>>>::Iterator& self) { return os << *self.impl; }

template class SaddleConnections<FlatTriangulation<long long>>;
template std::ostream& operator<<(std::ostream&, const SaddleConnections<FlatTriangulation<long long>>&);
template class SaddleConnections<FlatTriangulation<eantic::renf_elem_class>>;
template std::ostream& operator<<(std::ostream&, const SaddleConnections<FlatTriangulation<eantic::renf_elem_class>>&);
template class SaddleConnections<FlatTriangulation<exactreal::Element<exactreal::IntegerRing>>>;
template std::ostream& operator<<(std::ostream&, const SaddleConnections<FlatTriangulation<exactreal::Element<exactreal::IntegerRing>>>&);
template class SaddleConnections<FlatTriangulation<exactreal::Element<exactreal::RationalField>>>;
template std::ostream& operator<<(std::ostream&, const SaddleConnections<FlatTriangulation<exactreal::Element<exactreal::RationalField>>>&);
template class SaddleConnections<FlatTriangulation<exactreal::Element<exactreal::NumberField>>>;
template std::ostream& operator<<(std::ostream&, const SaddleConnections<FlatTriangulation<exactreal::Element<exactreal::NumberField>>>&);

}  // namespace flatsurf
