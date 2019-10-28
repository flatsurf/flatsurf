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

#include <ostream>
#include <vector>

#include "flatsurf/flat_triangulation.hpp"
#include "flatsurf/half_edge.hpp"
#include "flatsurf/half_edge_map.hpp"
#include "flatsurf/vector.hpp"
#include "flatsurf/saddle_connections.hpp"
#include "flatsurf/saddle_connection.hpp"
#include "intervalxt/length.hpp"
#include "util/assert.ipp"

using std::map;
using std::ostream;
using std::vector;

namespace flatsurf {
namespace {
template <typename T>
void updateAfterFlip(HalfEdgeMap<T> &map, HalfEdge halfEdge,
                     const FlatTriangulationCombinatorial &parent) {
  map.set(halfEdge, map.get(-parent.nextInFace(halfEdge)) +
                        map.get(parent.nextAtVertex(halfEdge)));
}
}  // namespace

template <typename T>
class FlatTriangulation<T>::Implementation {
 public:
  Implementation(HalfEdgeMap<Vector> &&vectors) : vectors(std::move(vectors)) {}

  const HalfEdgeMap<Vector> vectors;
};

template <typename T>
const Vector<T> &FlatTriangulation<T>::fromEdge(const HalfEdge e) const {
  return impl->vectors.get(e);
}

template <typename T>
FlatTriangulation<T>::FlatTriangulation() noexcept : FlatTriangulation(FlatTriangulationCombinatorial(), vector<Vector>{}) {}

template <typename T>
FlatTriangulation<T>::FlatTriangulation(FlatTriangulationCombinatorial &&combinatorial, const vector<Vector> &vectors)
    : FlatTriangulation(std::move(combinatorial), HalfEdgeMap<Vector>(&combinatorial, vectors, updateAfterFlip<Vector>)) {}

template <typename T>
FlatTriangulation<T>::FlatTriangulation(FlatTriangulationCombinatorial &&combinatorial, const HalfEdgeMap<Vector> &vectors)
    : FlatTriangulationCombinatorial(std::move(combinatorial)),
      impl(spimpl::make_unique_impl<Implementation>(HalfEdgeMap<Vector>(vectors, updateAfterFlip<Vector>))) {
  // check that faces are closed
  for (auto edge : halfEdges()) {
    if (this->boundary(edge)) continue;
    auto zero = fromEdge(edge);
    edge = nextInFace(edge);
    zero += fromEdge(edge);
    edge = nextInFace(edge);
    zero += fromEdge(edge);
    CHECK_ARGUMENT(!zero, "some face is not closed");
  }
  // check that faces are oriented correctly
  for (auto edge : halfEdges()) {
    if (this->boundary(edge)) continue;
    auto next = nextInFace(edge);
    CHECK_ARGUMENT(fromEdge(edge).ccw(fromEdge(next)) == CCW::COUNTERCLOCKWISE, "some face is not oriented correctly");
  }
}

template <typename T>
FlatTriangulation<T>::FlatTriangulation(FlatTriangulation<T> &&rhs) noexcept : FlatTriangulation() {
  *this = std::move(rhs);
}

template <typename T>
FlatTriangulation<T> &FlatTriangulation<T>::operator=(FlatTriangulation<T> &&rhs) noexcept {
  impl = std::move(rhs.impl);
  FlatTriangulationCombinatorial &self = static_cast<FlatTriangulationCombinatorial &>(*this);
  self = std::move(rhs);
  return *this;
}

template <typename T>
std::unique_ptr<FlatTriangulation<T>> FlatTriangulation<T>::clone() const {
  auto combinatorial = FlatTriangulationCombinatorial::clone();

  HalfEdgeMap<Vector> vectors = HalfEdgeMap<Vector>(&*combinatorial, updateAfterFlip<Vector>);
  for (auto edge : halfEdges())
    vectors.set(edge, fromEdge(edge));

  return std::make_unique<FlatTriangulation>(std::move(*combinatorial), vectors);
}

template <typename T>
std::unique_ptr<FlatTriangulation<T>> FlatTriangulation<T>::insertAt(HalfEdge& e, const Vector & v) const {
  CHECK_ARGUMENT(fromEdge(e).ccw(v) == CCW::COUNTERCLOCKWISE && fromEdge(nextAtVertex(e)).ccw(v) == CCW::CLOCKWISE, "vector v must be strictly contained in the sector next to the half edge e");

  std::shared_ptr<FlatTriangulation<T>> surface = clone();

  auto check_orientation = [&](const Vector& saddle_connection) {
    auto orient = (saddle_connection - v).orientation(v);
    CHECK_ARGUMENT(orient != ORIENTATION::OPPOSITE, "cannot insert half edge that crosses over an existing vertex");
    if (orient == ORIENTATION::ORTHOGONAL) {
      // It is a bit unclear what to do if the new edge should end at a
      // vertex, in particular if it is collinear with an existing half
      // edge (after fliping.)
      throw std::logic_error("insertion of half edges that end at an existing vertex not implemented yet");
    }
  };

  // Whether the next half edge that the ray to v crosses is further away
  // than v's length.
  bool nextCrossingBeyondBound = false;
  while(!nextCrossingBeyondBound) {
    if (surface->fromEdge(e).ccw(v) == CCW::COLLINEAR) {
      check_orientation(surface->fromEdge(e));
      break;
    }

    // flip edges so all of v is in one face
    auto connections = SaddleConnections<FlatTriangulation<T>>(surface, Bound(INT_MAX), e);
    auto search = connections.begin();

    while (true) {
      auto ccw = (*search)->vector().ccw(v);
      if (ccw == CCW::COLLINEAR) {
        check_orientation((*search)->vector());
        nextCrossingBeyondBound = true;
        break;
      }

      search.skipSector(-ccw);

      auto base = (*search)->vector() - surface->fromEdge((*search)->target());
      auto crossing = search.incrementWithCrossings();
      if (crossing.has_value()) {
        // potentially crossing *crossing at base

        auto edge = surface->fromEdge(*crossing);
        auto relative = v - base;
        nextCrossingBeyondBound = edge.ccw(relative) == CCW::COUNTERCLOCKWISE;
        if (nextCrossingBeyondBound) break;

        // v crosses crossing, so flip it and replace e if v is then not next to e anymore.
        surface->flip(*crossing);
        if (surface->fromEdge(-*crossing).ccw(v) != CCW::CLOCKWISE)
          e = -*crossing;
        break;
      }
    }
  }

  if (surface->fromEdge(e).ccw(v) != CCW::COLLINEAR) {
    // After the flips we did, v is now completely inside a face.
    assert(surface->fromEdge(e).ccw(v) == CCW::COUNTERCLOCKWISE);

    auto combinatorial = static_cast<FlatTriangulationCombinatorial*>(surface.get())->insertAt(e);

    HalfEdgeMap<Vector> vectors = HalfEdgeMap<Vector>(&*combinatorial, updateAfterFlip<Vector>);
    for (auto edge : surface->halfEdges())
      vectors.set(edge, surface->fromEdge(edge));

    HalfEdge a = -combinatorial->nextAtVertex(e);
    vectors.set(a, -v);
    HalfEdge b = combinatorial->nextAtVertex(a);
    vectors.set(b, surface->fromEdge(e) - v);
    HalfEdge c = combinatorial->nextAtVertex(b);
    vectors.set(c, surface->fromEdge(surface->nextAtVertex(e)) - v);

    return std::make_unique<FlatTriangulation>(std::move(*combinatorial), vectors);
  } else {
    // After the flips we did, v is collinear with the half edge e (but shorter.)

    // Insert our half edge ee next to e
    auto combinatorial = static_cast<FlatTriangulationCombinatorial*>(surface.get())->insertAt(e);
    auto ee = combinatorial->nextAtVertex(e);
    // After a flip of e the original e can be recovered as ee + eee.
    combinatorial->flip(e);
    auto eee = combinatorial->nextAtVertex(combinatorial->nextAtVertex(-ee));

    // The combinatorics are correct now, but we still have to patch up the
    // vectors, namely the four half edges meeting at the new vertex all need
    // updating.
    HalfEdgeMap<Vector> vectors = HalfEdgeMap<Vector>(&*combinatorial, updateAfterFlip<Vector>);
    for (auto edge : surface->halfEdges())
      vectors.set(edge, surface->fromEdge(edge));

    vectors.set(ee, v);
    vectors.set(eee, surface->fromEdge(e) - v);
    vectors.set(combinatorial->nextAtVertex(-ee), surface->fromEdge(surface->previousAtVertex(e)) - v);
    vectors.set(combinatorial->nextAtVertex(eee), surface->fromEdge(surface->nextAtVertex(e)) - v);

    e = combinatorial->previousAtVertex(ee);

    return std::make_unique<FlatTriangulation>(std::move(*combinatorial), vectors);
  }
}

template <typename T>
std::unique_ptr<FlatTriangulation<T>> FlatTriangulation<T>::scale(const mpz_class& c) const {
  auto combinatorial = FlatTriangulationCombinatorial::clone();

  HalfEdgeMap<Vector> vectors = HalfEdgeMap<Vector>(&*combinatorial, updateAfterFlip<Vector>);
  for (auto edge : halfEdges())
    vectors.set(edge, c * fromEdge(edge));

  return std::make_unique<FlatTriangulation>(std::move(*combinatorial), vectors);
}

template <typename T>
bool FlatTriangulation<T>::operator==(const FlatTriangulation<T> &rhs) const noexcept {
  if (static_cast<const FlatTriangulationCombinatorial &>(*this) != static_cast<const FlatTriangulationCombinatorial &>(rhs))
    return false;
  for (auto &edge : halfEdges()) {
    if (this->impl->vectors.get(edge) != rhs.impl->vectors.get(edge))
      return false;
  }
  return true;
}

template <typename T>
ostream &operator<<(ostream &os, const FlatTriangulation<T> &self) {
  return os << static_cast<const FlatTriangulationCombinatorial &>(self)
            << " with vectors " << self.impl->vectors;
}
}  // namespace flatsurf

// Instantiations of templates so implementations are generated for the linker
#include <e-antic/renfxx_fwd.h>
#include <exact-real/integer_ring.hpp>
#include <exact-real/number_field.hpp>
#include <exact-real/rational_field.hpp>

using namespace flatsurf;

template class flatsurf::FlatTriangulation<long long>;
template ostream &flatsurf::operator<<(ostream &, const FlatTriangulation<long long> &);
template class flatsurf::FlatTriangulation<eantic::renf_elem_class>;
template ostream &flatsurf::operator<<(ostream &, const FlatTriangulation<eantic::renf_elem_class> &);
template class flatsurf::FlatTriangulation<exactreal::Element<exactreal::IntegerRing>>;
template ostream &flatsurf::operator<<(ostream &, const FlatTriangulation<exactreal::Element<exactreal::IntegerRing>> &);
template class flatsurf::FlatTriangulation<exactreal::Element<exactreal::RationalField>>;
template ostream &flatsurf::operator<<(ostream &, const FlatTriangulation<exactreal::Element<exactreal::RationalField>> &);
template class flatsurf::FlatTriangulation<exactreal::Element<exactreal::NumberField>>;
template ostream &flatsurf::operator<<(ostream &, const FlatTriangulation<exactreal::Element<exactreal::NumberField>> &);
