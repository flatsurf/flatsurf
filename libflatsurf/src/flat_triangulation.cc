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

#include <intervalxt/sample/arithmetic.hpp>
#include <map>
#include <ostream>
#include <vector>

#include <exact-real/arb.hpp>

#include "../flatsurf/bound.hpp"
#include "../flatsurf/ccw.hpp"
#include "../flatsurf/edge.hpp"
#include "../flatsurf/flat_triangulation.hpp"
#include "../flatsurf/half_edge.hpp"
#include "../flatsurf/half_edge_map.hpp"
#include "../flatsurf/orientation.hpp"
#include "../flatsurf/saddle_connection.hpp"
#include "../flatsurf/saddle_connections.hpp"
#include "../flatsurf/vector.hpp"
#include "../flatsurf/vertical.hpp"

#include <functional>
#include <iosfwd>
#include "util/assert.ipp"

#include "impl/flat_triangulation.impl.hpp"

using std::map;
using std::ostream;
using std::vector;

namespace flatsurf {

template <typename T>
Vector<T> FlatTriangulation<T>::shortest() const {
  const auto edges = this->edges();
  Edge shortest = *std::min_element(begin(edges), end(edges), [&](const auto &a, const auto &b) {
    const Vector x = fromEdge(a.positive());
    const Vector y = fromEdge(b.positive());
    return x * x < y * y;
  });
  return fromEdge(shortest.positive());
}

// TODO: Move to the appropriate places
template <typename T>
T abs(const T &x) {
  return x < 0 ? -x : x;
}

template <typename T>
Vector<T> FlatTriangulation<T>::shortest(const Vector<T> &direction) const {
  const auto edges = this->edges();
  Edge shortest = *std::min_element(begin(edges), end(edges), [&](const auto &a, const auto &b) {
    const Vector<T> x = fromEdge(a.positive());
    const Vector<T> y = fromEdge(b.positive());

    const auto xlen = x * direction;
    const auto ylen = y * direction;

    if (!xlen) return false;
    if (!ylen) return true;
    return abs(xlen) < abs(ylen);
  });
  return fromEdge(shortest.positive());
}

template <typename T>
Implementation<FlatTriangulation<T>>::Implementation(const FlatTriangulationCombinatorial &combinatorial, const std::function<Vector<T>(HalfEdge)> &vectors) :
  vectors(&combinatorial, vectors, Implementation::updateAfterFlip),
  approximations(
      &combinatorial,
      [&](const HalfEdge e) {
        return static_cast<flatsurf::Vector<exactreal::Arb>>(this->vectors.get(e));
      },
      Implementation::updateApproximationAfterFlip) {
}

template <typename T>
void Implementation<FlatTriangulation<T>>::updateAfterFlip(HalfEdgeMap<Vector<T>> &vectors, HalfEdge flip) {
  const FlatTriangulationCombinatorial &parent = vectors.parent();
  vectors.set(flip, vectors.get(-parent.nextInFace(flip)) + vectors.get(-parent.previousInFace(flip)));
}

template <typename T>
void Implementation<FlatTriangulation<T>>::updateApproximationAfterFlip(HalfEdgeMap<flatsurf::Vector<exactreal::Arb>> &vectors, HalfEdge flip) {
  const auto &surface = static_cast<const FlatTriangulation<T> &>(vectors.parent());
  vectors.set(flip, static_cast<flatsurf::Vector<exactreal::Arb>>(surface.fromEdge(-surface.nextInFace(flip)) + surface.fromEdge(-surface.previousInFace(flip))));
}

template <typename T>
void Implementation<FlatTriangulation<T>>::check(const FlatTriangulation<T> &self) {
  // check that faces are closed
  for (auto edge : self.halfEdges()) {
    if (self.boundary(edge)) continue;
    auto zero = self.fromEdge(edge);
    CHECK_ARGUMENT(zero, "edges must not be trivial but " << edge << " is zero in " << self);
    edge = self.nextInFace(edge);
    zero += self.fromEdge(edge);
    edge = self.nextInFace(edge);
    zero += self.fromEdge(edge);
    CHECK_ARGUMENT(!zero, "face at " << edge << " is not closed in " << self);
  }
  // check that faces are oriented correctly
  for (auto edge : self.halfEdges()) {
    if (self.boundary(edge)) continue;
    auto next = self.nextInFace(edge);
    CHECK_ARGUMENT(self.fromEdge(edge).ccw(self.fromEdge(next)) == CCW::COUNTERCLOCKWISE, "face at " << edge << " is not oriented correctly in " << self);
  }
}

template <typename T>
const Vector<T> &FlatTriangulation<T>::fromEdge(const HalfEdge e) const {
  return impl->vectors.get(e);
}

template <typename T>
const flatsurf::Vector<exactreal::Arb> &FlatTriangulation<T>::fromEdgeApproximate(HalfEdge e) const {
  return impl->approximations.get(e);
}

template <typename T>
FlatTriangulation<T>::FlatTriangulation() noexcept :
  FlatTriangulation(FlatTriangulationCombinatorial(), vector<Vector<T>>{}) {}

template <typename T>
FlatTriangulation<T>::FlatTriangulation(FlatTriangulationCombinatorial &&combinatorial, const vector<Vector<T>> &vectors) :
  FlatTriangulation(std::move(combinatorial), [&](const HalfEdge he) {
    Edge e = he;
    if (he == e.positive())
      return vectors.at(e.index());
    else
      return -vectors.at(e.index());
  }) {
  CHECK_ARGUMENT(vectors.size() == this->edges().size(), "there must be exactly one vector for each edge");
}

template <typename T>
FlatTriangulation<T>::FlatTriangulation(FlatTriangulationCombinatorial &&combinatorial, const std::function<Vector<T>(HalfEdge)> &vectors) :
  FlatTriangulationCombinatorial(std::move(combinatorial)),
  impl(spimpl::make_unique_impl<Implementation>(*this, vectors)) {
  Implementation::check(*this);
}

template <typename T>
FlatTriangulation<T>::FlatTriangulation(FlatTriangulation<T> &&rhs) noexcept :
  FlatTriangulation() {
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
  return std::make_unique<FlatTriangulation>(std::move(*combinatorial), [&](HalfEdge e) { return fromEdge(e); });
}

template <typename T>
std::unique_ptr<FlatTriangulation<T>> FlatTriangulation<T>::slot(HalfEdge slot) const {
  auto combinatorial = FlatTriangulationCombinatorial::slot(slot);
  return std::make_unique<FlatTriangulation>(std::move(*combinatorial->clone()), [&](HalfEdge e) {
    HalfEdge newEdge = HalfEdge(static_cast<int>(halfEdges().size()) / 2 + 1);
    if (e == newEdge) return fromEdge(slot);
    if (e == -newEdge) return -fromEdge(slot);
    return fromEdge(e);
  });
}

template <typename T>
std::shared_ptr<FlatTriangulation<T>> FlatTriangulation<T>::shared_from_this() {
  return std::static_pointer_cast<FlatTriangulation<T>>(FlatTriangulationCombinatorial::shared_from_this());
}

template <typename T>
std::shared_ptr<const FlatTriangulation<T>> FlatTriangulation<T>::shared_from_this() const {
  return std::static_pointer_cast<const FlatTriangulation<T>>(FlatTriangulationCombinatorial::shared_from_this());
}

template <typename T>
std::unique_ptr<FlatTriangulation<T>> FlatTriangulation<T>::insertAt(HalfEdge &nextTo, const Vector<T> &slot) const {
  CHECK_ARGUMENT(inSector(nextTo, slot), "vector must be contained in the sector next to the half edge");

  std::shared_ptr<FlatTriangulation<T>> surface = clone();

  auto check_orientation = [&](const Vector<T> &saddle_connection) {
    auto orient = (saddle_connection - slot).orientation(slot);
    CHECK_ARGUMENT(orient != ORIENTATION::OPPOSITE, "cannot insert half edge that crosses over an existing vertex");
    if (orient == ORIENTATION::ORTHOGONAL) {
      // It is a bit unclear what to do if the new edge should end at a
      // vertex, in particular if it is collinear with an existing half
      // edge (after fliping.)
      throw std::logic_error("insertion of half edges that end at an existing vertex not implemented yet");
    }
  };

  // Search for half edges that slot would be crossing and flip them.
  [&]() {
    while (true) {
      if (surface->fromEdge(nextTo).ccw(slot) == CCW::COLLINEAR) {
        check_orientation(surface->fromEdge(nextTo));
        // Slot is on an existing HalfEdge but does not cross a vertex.
        return;
      }
      assert(surface->fromEdge(nextTo).ccw(slot) == CCW::COUNTERCLOCKWISE);

      // The half edge that slot is potentially crossing
      const HalfEdge crossing = surface->nextInFace(nextTo);
      // The base point of crossing half edge
      const Vector<T> base = surface->fromEdge(nextTo);

      // Check whether slot is actually crossing crossing. It would be enough
      // to check whether this is != CLOCKWISE. However, we do not allow slot
      // to end on an edge other than nextTo. So we perform one additional
      // flip in that case so slot is actually inside of a face.
      if (surface->fromEdge(crossing).ccw(slot - base) == CCW::COUNTERCLOCKWISE)
        return;

      std::function<void(HalfEdge)> flip = [&](HalfEdge e) {
        assert(e != nextTo && e != -nextTo && e != surface->nextAtVertex(nextTo) && e != -surface->nextAtVertex(nextTo));

        auto canFlip = [&](HalfEdge g) {
          return e != nextTo && e != -nextTo && e != surface->nextAtVertex(nextTo) && e != -surface->nextAtVertex(nextTo) &&
                 surface->fromEdge(surface->previousAtVertex(g)).ccw(surface->fromEdge(surface->nextAtVertex(g))) == CCW::COUNTERCLOCKWISE && surface->fromEdge(surface->previousAtVertex(-g)).ccw(surface->fromEdge(surface->nextAtVertex(-g))) == CCW::COUNTERCLOCKWISE;
        };

        while (!canFlip(e)) {
          // f is blocked by a forward triangle on top of it so we flip its top edge.
          if (slot.ccw(surface->fromEdge(surface->previousAtVertex(e))) != CCW::COUNTERCLOCKWISE) {
            flip(-surface->nextAtVertex(-e));
            continue;
          } else {
            assert(slot.ccw(surface->fromEdge(surface->nextAtVertex(-e))) != CCW::CLOCKWISE);
            flip(surface->previousAtVertex(e));
            continue;
          }
        }

        surface->flip(e);
      };

      // slot crosses crossing, so flip it and replace nextTo if slot is then not next to nextTo anymore.
      flip(crossing);
      assert(surface->fromEdge(nextTo).ccw(slot) == CCW::COUNTERCLOCKWISE);
      while (surface->fromEdge(surface->nextAtVertex(nextTo)).ccw(slot) != CCW::CLOCKWISE)
        nextTo = surface->nextAtVertex(nextTo);
    }
  }();

  auto symmetric = [](HalfEdge x, HalfEdge e, const Vector<T> &v) {
    assert(x == e || x == -e);
    return x == e ? v : -v;
  };

  if (surface->fromEdge(nextTo).ccw(slot) != CCW::COLLINEAR) {
    // After the flips we did, v is now completely inside a face.
    assert(surface->fromEdge(nextTo).ccw(slot) == CCW::COUNTERCLOCKWISE);

    auto combinatorial = static_cast<FlatTriangulationCombinatorial *>(surface.get())->insertAt(nextTo);

    return std::make_unique<FlatTriangulation>(std::move(*combinatorial->clone()), [&](const HalfEdge e) {
      HalfEdge a = -combinatorial->nextAtVertex(nextTo);
      HalfEdge b = combinatorial->nextAtVertex(a);
      HalfEdge c = combinatorial->nextAtVertex(b);

      if (Edge(e) == a) return symmetric(e, a, -slot);
      if (Edge(e) == b) return symmetric(e, b, surface->fromEdge(nextTo) - slot);
      if (Edge(e) == c) return symmetric(e, c, surface->fromEdge(surface->nextAtVertex(nextTo)) - slot);
      return surface->fromEdge(e);
    });
  } else {
    // After the flips we did, v is collinear with the half edge e (but shorter.)

    // Insert our half edge ee next to e
    auto combinatorial = static_cast<FlatTriangulationCombinatorial *>(surface.get())->insertAt(nextTo);
    auto nextAtSlot = combinatorial->nextAtVertex(nextTo);
    // After a flip of slot the original slot can be recovered as nextAtSlot + eee.
    combinatorial->flip(nextTo);
    auto eee = combinatorial->nextAtVertex(combinatorial->nextAtVertex(-nextAtSlot));

    // The combinatorics are correct now, but we still have to patch up the
    // vectors, namely the four half edges meeting at the new vertex all need
    // updating.
    auto ret = std::make_unique<FlatTriangulation>(std::move(*combinatorial->clone()), [&](const HalfEdge e) {
      if (Edge(e) == nextAtSlot) return symmetric(e, nextAtSlot, slot);
      if (Edge(e) == eee) return symmetric(e, eee, surface->fromEdge(nextTo) - slot);
      if (Edge(e) == combinatorial->nextAtVertex(-nextAtSlot)) return symmetric(e, combinatorial->nextAtVertex(-nextAtSlot), surface->fromEdge(surface->previousAtVertex(nextTo)) - slot);
      if (Edge(e) == combinatorial->nextAtVertex(eee)) return symmetric(e, combinatorial->nextAtVertex(eee), surface->fromEdge(surface->nextAtVertex(nextTo)) - slot);
      return surface->fromEdge(e);
    });

    nextTo = combinatorial->previousAtVertex(nextAtSlot);

    return ret;
  }
}

template <typename T>
T FlatTriangulation<T>::area() const {
  T area = T();
  for (auto e : halfEdges()) {
    if (boundary(e)) continue;

    // Do not count every triangle three times.
    if (e.index() > nextInFace(e).index()) continue;
    if (e.index() > previousInFace(e).index()) continue;

    area += Vector<T>::area({fromEdge(e), fromEdge(nextInFace(e)), fromEdge(nextInFace(nextInFace(e)))});
  }
  return area;
}

template <typename T>
std::unique_ptr<FlatTriangulation<T>> FlatTriangulation<T>::scale(const mpz_class &scalar) const {
  return std::make_unique<FlatTriangulation>(std::move(*FlatTriangulationCombinatorial::clone()), [&](HalfEdge e) {
    return scalar * fromEdge(e);
  });
}

template <typename T>
void FlatTriangulation<T>::flip(HalfEdge e) {
  CHECK_ARGUMENT(
      fromEdge(previousAtVertex(e)).ccw(fromEdge(nextAtVertex(e))) == CCW::COUNTERCLOCKWISE &&
          fromEdge(previousAtVertex(-e)).ccw(fromEdge(nextAtVertex(-e))) == CCW::COUNTERCLOCKWISE,
      "cannot flip this edge as a resulting face would not be strictly convex");

  FlatTriangulationCombinatorial::flip(e);

  Implementation::check(*this);
}

template <typename T>
bool FlatTriangulation<T>::inSector(const HalfEdge sector, const Vector<T> &vector) const {
  return fromEdge(sector).ccw(vector) != CCW::CLOCKWISE && fromEdge(nextAtVertex(sector)).ccw(vector) == CCW::CLOCKWISE;
}

template <typename T>
bool FlatTriangulation<T>::inSector(const HalfEdge sector, const Vertical<FlatTriangulation<T>> &vector) const {
  return inSector(sector, vector.vertical());
}

template <typename T>
bool FlatTriangulation<T>::operator==(const FlatTriangulation<T> &rhs) const noexcept {
  if (this == &rhs)
    return true;

  if (static_cast<const FlatTriangulationCombinatorial &>(*this) != static_cast<const FlatTriangulationCombinatorial &>(rhs))
    return false;
  for (auto &edge : halfEdges()) {
    if (this->impl->vectors.get(edge) != rhs.impl->vectors.get(edge))
      return false;
  }
  return true;
}

template <typename T>
int FlatTriangulation<T>::angle(const Vertex& vertex) const {
  int angle = 0;

  const HalfEdge first = *begin(outgoing(vertex));
  HalfEdge current = first;
  do {
    const HalfEdge next = nextAtVertex(current);

    if (fromEdge(current).x() >= 0 && fromEdge(next).x() < 0)
      angle++;

    current = next;
  } while(current != first);

  return angle;
}

template <typename T>
ostream &operator<<(ostream &os, const FlatTriangulation<T> &self) {
  return os << static_cast<const FlatTriangulationCombinatorial &>(self)
            << " with vectors " << self.impl->vectors;
}
}  // namespace flatsurf

// Instantiations of templates so implementations are generated for the linker
#include "util/instantiate.ipp"

LIBFLATSURF_INSTANTIATE_MANY_WRAPPED((LIBFLATSURF_INSTANTIATE_WITH_IMPLEMENTATION), FlatTriangulation, LIBFLATSURF_REAL_TYPES)
