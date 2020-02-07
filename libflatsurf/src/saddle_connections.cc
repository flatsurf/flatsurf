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
#include <stack>

#include "../flatsurf/bound.hpp"
#include "../flatsurf/ccw.hpp"
#include "../flatsurf/chain.hpp"
#include "../flatsurf/flat_triangulation.hpp"
#include "../flatsurf/half_edge.hpp"
#include "../flatsurf/saddle_connection.hpp"
#include "../flatsurf/saddle_connections.hpp"
#include "../flatsurf/vector.hpp"

#include "util/assert.ipp"

using std::vector;
namespace {
enum class Classification {
  OUTSIDE_SEARCH_SECTOR_CLOCKWISE,
  SADDLE_CONNECTION,
  OUTSIDE_SEARCH_SECTOR_COUNTERCLOCKWISE,
};

enum class State {
  // nextEdgeEnd is likely inside the search radius
  START_INSIDE_SEARCH_RADIUS,
  // nextEdgeEnd is certainly outside the search radius
  START_OUTSIDE_SEARCH_RADIUS,
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
  Implementation(Iterator&& begin) noexcept :
    begin(std::move(begin)) {}

  Iterator begin;
};

template <typename Surface>
class SaddleConnections<Surface>::Iterator::Implementation {
 public:
  Implementation(const std::shared_ptr<const Surface>& surface, const Bound searchRadius, const vector<HalfEdge> searchSectors) :
    surface(std::move(surface)),
    searchRadius(searchRadius),
    sectors(std::make_shared<vector<HalfEdge>>(searchSectors)),
    sector(this->sectors->begin()),
    nextEdgeEnd(this->surface),
    connection(SaddleConnection<Surface>::fromEdge(this->surface, surface->halfEdges()[0])) {
    prepareSearch();
  }

  void prepareSearch() {
    assert(state.size() == 0);
    assert(tmp.size() == 0);
    assert(moves.size() == 0);

    if (sector == sectors->end()) {
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
    state.push(State::END);
    state.push(State::START_INSIDE_SEARCH_RADIUS);

    // Report sector as a saddle connection unless it is already outside of
    // the search radius.
    if (boundary[0] > searchRadius) {
      while (!increment())
        ;
    }
  }

  static CCW ccw(const Chain<Surface>& lhs, const Chain<Surface>& rhs) {
    auto ccw = static_cast<const Vector<exactreal::Arb>&>(lhs).ccw(static_cast<const Vector<exactreal::Arb>&>(rhs));
    if (ccw) return *ccw;
    return static_cast<const typename Surface::Vector&>(lhs).ccw(static_cast<const typename Surface::Vector&>(rhs));
  }

  std::shared_ptr<const Surface> surface;
  const Bound searchRadius;
  const std::shared_ptr<const vector<HalfEdge>> sectors;

  // The half edge nextEdge, to which we are currently changing, points into
  // sectors. Advanced when we are done searching an entire such sector for all
  // saddle connections. (Note: We need to store sectors as a smart pointer so
  // when this class is copied, this iterator does not only point into the
  // sectors of the original.)
  vector<HalfEdge>::const_iterator sector;

  // The rays that enclose the search sector, in counterclockwise order.
  Chain<Surface> boundary[2];
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
  std::stack<State> state;

  // We collect pending moves across the surface here (adding half edges to
  // nextEdgeEnd mostly.) When the exact value of nextEdgeEnd is required, we
  // can often combine several move more efficiently, see applyMoves().
  std::deque<Move> moves;

  // Storage space for temporary values of boundary, when we descend
  // recursively into a subsector.
  std::stack<Chain<Surface>> tmp;

  // The current connection so we can return it in dereference by reference.
  mutable SaddleConnection<Surface> connection;

  // TODO: I introduced the
  // START_OUTSIDE_SEARCH_RADIUS/START_INSIDE_SEARCH_RADIUS to be able to
  // handle very small angles more efficiently. We should count how often we
  // still recurse into a half edge that cannot possibly lead to new saddle
  // connections. And check how much more expensive the new checks are in
  // comparison.
  // Maybe we can sometimes pick the SEARCH_* state heuristically or use only
  // an approximate operator>.

  bool increment() {
    assert(state.size());
    assert(sector != sectors->end());
    assert(ccw(boundary[0], boundary[1]) == CCW::COUNTERCLOCKWISE);

    const auto s = state.top();
    state.pop();
    switch (s) {
      case State::END:
        applyMoves();
        sector++;
        if (sector != sectors->end()) {
          prepareSearch();
        }
        return true;
      case State::START_OUTSIDE_SEARCH_RADIUS:
      case State::START_INSIDE_SEARCH_RADIUS:
        moves.push_back(Move::GOTO_OTHER_FACE);
        if (onBoundary()) {
          moves.push_back(Move::GOTO_OTHER_FACE);
          return false;
        }
        moves.push_back(Move::GOTO_NEXT_EDGE);

        applyMoves();
        switch (classifyHalfEdgeEnd()) {
          case Classification::OUTSIDE_SEARCH_SECTOR_CLOCKWISE: {
            // Since this vertex is outside of the search sector on the
            // clockwise side, we skip the clockwise sector in the recursive
            // search and recurse into the counterclockwise sector.
            moves.push_back(Move::GOTO_NEXT_EDGE);

            applyMoves();
            const bool classifiedVertexIsExceedingSearchRadius = nextEdgeEnd > searchRadius;

            if (s == State::START_OUTSIDE_SEARCH_RADIUS && classifiedVertexIsExceedingSearchRadius) {
              // Both end points of this edge are outside the search radius.
              // Nothing beyond this edge can be inside the search radius.
              moves.push_back(Move::GOTO_NEXT_EDGE);
              moves.push_back(Move::GOTO_OTHER_FACE);
            } else {
              // Note that the following is a nop that just exists for symmetry.
              state.push(State::OUTSIDE_SEARCH_SECTOR_CLOCKWISE_SEARCHING);
              state.push(s);
            }
            return false;
          }
          case Classification::OUTSIDE_SEARCH_SECTOR_COUNTERCLOCKWISE: {
            // Similarly, we skip the counterclockwise sector.
            state.push(State::OUTSIDE_SEARCH_SECTOR_COUNTERCLOCKWISE_SEARCHING);
            state.push(nextEdgeEnd > searchRadius ? State::START_OUTSIDE_SEARCH_RADIUS : State::START_INSIDE_SEARCH_RADIUS);
            return false;
          }
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
        state.push(State::START_INSIDE_SEARCH_RADIUS);
        state.push(State::SADDLE_CONNECTION_FOUND_SEARCHING_FIRST);
        state.push(State::START_INSIDE_SEARCH_RADIUS);
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
    ASSERT_ARGUMENT(sector != CCW::COLLINEAR, "There is no such thing like a collinear sector.");
    assert(state.size() && "cannot skip a sector in a completed search");

    switch (state.top()) {
      case State::SADDLE_CONNECTION_FOUND:
        increment();

        if (sector == CCW::CLOCKWISE) {
          // Go directly to the second sector by skipping the recursive call,
          // i.e., the START.
          assert(state.top() == State::START_INSIDE_SEARCH_RADIUS || state.top() == State::START_OUTSIDE_SEARCH_RADIUS);
          state.pop();

          assert(state.top() == State::SADDLE_CONNECTION_FOUND_SEARCHING_FIRST);
        } else if (sector == CCW::COUNTERCLOCKWISE) {
          assert(state.top() == State::START_INSIDE_SEARCH_RADIUS || state.top() == State::START_OUTSIDE_SEARCH_RADIUS);
          state.pop();

          assert(state.top() == State::SADDLE_CONNECTION_FOUND_SEARCHING_FIRST);
          state.pop();

          assert(state.top() == State::START_INSIDE_SEARCH_RADIUS || state.top() == State::START_OUTSIDE_SEARCH_RADIUS);
          // Skip the second recursive call by dropping its START.
          state.pop();

          // And push the rest back on the stack unchanged.
          state.push(State::SADDLE_CONNECTION_FOUND_SEARCHING_FIRST);
          state.push(State::START_INSIDE_SEARCH_RADIUS);
        }
        break;
      case State::START_INSIDE_SEARCH_RADIUS:
      case State::START_OUTSIDE_SEARCH_RADIUS:
        if (state.size() == 2) {
          if (sector == CCW::COUNTERCLOCKWISE) {
            // We are in the initial state, the reported saddle connection is on
            // the clockwise end of the search sector. If we skip the
            // counterclockwise sector, then we skip everything.
            state.pop();
            assert(state.top() == State::END);
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

  Classification classifyHalfEdgeEnd() const {
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

  friend std::ostream& operator<<(std::ostream& os, const Implementation& self) {
    if (self.sector == self.sectors->end()) {
      return os << "Iterator(END)";
    }
    return os << "Iterator(sector = " << *self.sector << ", connection = " << self.nextEdgeEnd << ")";
  }
};

template <typename Surface>
SaddleConnections<Surface>::SaddleConnections(const std::shared_ptr<const Surface>& surface, const Bound searchRadius) :
  impl(spimpl::make_impl<Implementation>(spimpl::make_impl<typename Iterator::Implementation>(surface, searchRadius, surface->halfEdges()))) {}

template <typename Surface>
SaddleConnections<Surface>::SaddleConnections(const std::shared_ptr<const Surface>& surface, const Bound searchRadius, const Vertex source) :
  impl(spimpl::make_impl<Implementation>(spimpl::make_impl<typename Iterator::Implementation>(surface, searchRadius, surface->atVertex(source)))) {}

template <typename Surface>
SaddleConnections<Surface>::SaddleConnections(const std::shared_ptr<const Surface>& surface, const Bound searchRadius, const HalfEdge sectorBegin) :
  impl(spimpl::make_impl<Implementation>(spimpl::make_impl<typename Iterator::Implementation>(surface, searchRadius, vector<HalfEdge>{sectorBegin}))) {}

template <typename Surface>
typename SaddleConnections<Surface>::Iterator SaddleConnections<Surface>::begin() const {
  return impl->begin;
}

template <typename Surface>
typename SaddleConnections<Surface>::Iterator SaddleConnections<Surface>::end() const {
  Iterator ret = impl->begin;
  ret.impl->sector = ret.impl->sectors->end();
  return ret;
}

template <typename Surface>
SaddleConnections<Surface>::Iterator::Iterator(spimpl::impl_ptr<Implementation>&& impl) :
  impl(std::move(impl)) {}

template <typename Surface>
bool SaddleConnections<Surface>::Iterator::equal(const SaddleConnections<Surface>::Iterator& other) const {
  if (impl->surface != other.impl->surface || impl->sectors != other.impl->sectors || impl->searchRadius != other.impl->searchRadius || (impl->sector - impl->sectors->begin()) != (other.impl->sector - other.impl->sectors->begin()))
    return false;

  if (impl->sector == impl->sectors->end())
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
  ASSERT(impl->sector != impl->sectors->end(), "iterator is at end()");

  while (true) {
    if (impl->sector == impl->sectors->end()) {
      return {};
    } else
      switch (impl->state.top()) {
        case State::START_INSIDE_SEARCH_RADIUS:
        case State::START_OUTSIDE_SEARCH_RADIUS: {
          impl->applyMoves();
          const auto ret = impl->nextEdge;
          impl->increment();
          return ret;
        }
        case State::SADDLE_CONNECTION_FOUND:
          return {};
        default:
          impl->increment();
      }
  }
}

template <typename Surface>
const SaddleConnection<Surface>& SaddleConnections<Surface>::Iterator::dereference() const {
  ASSERT(impl->sector != impl->sectors->end(), "iterator is at end()");

  switch (impl->state.top()) {
    case State::START_INSIDE_SEARCH_RADIUS:
      // This makes the first reported connection work: It is not nextEdgeEnd but the sector boundary.
      impl->connection = SaddleConnection<Surface>::fromEdge(impl->surface, *impl->sector);
      break;
    case State::SADDLE_CONNECTION_FOUND:
      impl->connection = SaddleConnection<Surface>(impl->surface, *impl->sector, -impl->nextEdge, impl->nextEdgeEnd);
      break;
    case State::START_OUTSIDE_SEARCH_RADIUS:
      ASSERT(false, "iterator must be at end when in this state");
    default:
      ASSERT(false, "iterator cannot hold in this state");
  }

  ASSERT(impl->connection <= impl->searchRadius, "Iterator stopped at connection " << impl->connection << " which is beyond the search radius " << impl->searchRadius);

  return impl->connection;
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
std::ostream& operator<<(std::ostream& os, const typename SaddleConnections<FlatTriangulation<mpq_class>>::Iterator& self) { return os << *self.impl; }
std::ostream& operator<<(std::ostream& os, const typename SaddleConnections<FlatTriangulation<eantic::renf_elem_class>>::Iterator& self) { return os << *self.impl; }
std::ostream& operator<<(std::ostream& os, const typename SaddleConnections<FlatTriangulation<exactreal::Element<exactreal::IntegerRing>>>::Iterator& self) { return os << *self.impl; }
std::ostream& operator<<(std::ostream& os, const typename SaddleConnections<FlatTriangulation<exactreal::Element<exactreal::RationalField>>>::Iterator& self) { return os << *self.impl; }
std::ostream& operator<<(std::ostream& os, const typename SaddleConnections<FlatTriangulation<exactreal::Element<exactreal::NumberField>>>::Iterator& self) { return os << *self.impl; }

template class SaddleConnections<FlatTriangulation<mpz_class>>;
template std::ostream& operator<<(std::ostream&, const SaddleConnections<FlatTriangulation<mpz_class>>&);
template class SaddleConnections<FlatTriangulation<mpq_class>>;
template std::ostream& operator<<(std::ostream&, const SaddleConnections<FlatTriangulation<mpq_class>>&);
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
