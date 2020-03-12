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

#include <ostream>
#include <unordered_set>
#include <unordered_map>
#include <vector>

#include <fmt/format.h>
#include <fmt/ostream.h>
#include <gmpxx.h>
#include <intervalxt/sample/e-antic-arithmetic.hpp>
#include <intervalxt/sample/exact-real-arithmetic.hpp>
#include <intervalxt/sample/rational-arithmetic.hpp>

#include "external/gmpxxll/gmpxxll/mpz_class.hpp"
#include "external/rx-ranges/include/rx/ranges.hpp"

#include "../flatsurf/bound.hpp"
#include "../flatsurf/ccw.hpp"
#include "../flatsurf/edge.hpp"
#include "../flatsurf/flat_triangulation.hpp"
#include "../flatsurf/flat_triangulation_collapsed.hpp"
#include "../flatsurf/flat_triangulation_combinatorial.hpp"
#include "../flatsurf/half_edge.hpp"
#include "../flatsurf/half_edge_map.hpp"
#include "../flatsurf/orientation.hpp"
#include "../flatsurf/path.hpp"
#include "../flatsurf/path_iterator.hpp"
#include "../flatsurf/saddle_connection.hpp"
#include "../flatsurf/vector.hpp"
#include "../flatsurf/vertex.hpp"
#include "../flatsurf/vertical.hpp"

#include "impl/flat_triangulation_collapsed.impl.hpp"
// TODO: Maybe better use subdirectories
#include "impl/collapsed_half_edge.hpp"
#include "impl/flat_triangulation.impl.hpp"
#include "impl/saddle_connection.impl.hpp"

#include "util/assert.ipp"
#include "util/union_find.ipp"

namespace flatsurf {

using fmt::format;
using rx::to_vector;
using rx::transform;
using std::ostream;
using std::string;

template <typename T>
FlatTriangulationCollapsed<T>::FlatTriangulationCollapsed(std::unique_ptr<FlatTriangulation<T>> surface, const Vector<T>& vertical) :
  FlatTriangulationCombinatorial(std::move(*surface->clone())),
  impl(spimpl::make_unique_impl<Implementation>(*this, std::move(surface), vertical)) {
}

template <typename T>
std::shared_ptr<FlatTriangulationCollapsed<T>> FlatTriangulationCollapsed<T>::make(std::unique_ptr<FlatTriangulation<T>> surface, const Vector<T>& vertical) {
  auto ret = std::shared_ptr<FlatTriangulationCollapsed<T>>(new FlatTriangulationCollapsed<T>(std::move(surface), vertical));

  while ([&]() {
    for (auto e : ret->halfEdges()) {
      if (ret->vertical().parallel(e)) {
        ret->collapse(e);
        return true;
      }
    }
    return false;
  }())
    ;

  return ret;
}

template <typename T>
Vertical<FlatTriangulationCollapsed<T>> FlatTriangulationCollapsed<T>::vertical() const {
  return Vertical(std::static_pointer_cast<const FlatTriangulationCollapsed>(shared_from_this()), impl->vertical);
}

template <typename T>
bool FlatTriangulationCollapsed<T>::inSector(HalfEdge sector, const Vector<T>& vector) const {
  return fromEdge(sector).ccw(vector) != CCW::CLOCKWISE && (-fromEdge(previousInFace(sector))).ccw(vector) == CCW::CLOCKWISE;
}

template <typename T>
bool FlatTriangulationCollapsed<T>::inSector(HalfEdge sector, const Vertical<FlatTriangulationCollapsed<T>>& vector) const {
  return inSector(sector, vector.vertical());
}

// TODO: Move to the appropriate places
template <typename T>
T abs(const T& x) {
  return x < 0 ? -x : x;
}

// TODO: Move to vector?
template <typename T>
mpz_class relativeLength(const Vector<T>& divident, const Vector<T>& divisor) {
  // TODO: Arithmetic is not a good pattern mostly. Instead, these should be free functions in a fixed namespace.
  return sqrt(::intervalxt::sample::Arithmetic<T>::floorDivision((divident * divident) * (divident * divident), abs(divisor * divident)));
}

template <typename T>
void FlatTriangulationCollapsed<T>::flip(HalfEdge e) {
  CHECK_ARGUMENT(vertical().large(e), "in a CollapsedSurface, only large edges can be flipped");
  CHECK_ARGUMENT(nextInFace(nextInFace(nextInFace(e))) == e && nextInFace(nextInFace(nextInFace(-e))) == -e, "in a CollapsedSurface, only edges that are not in a collapsed face can be fliped");

  if (vertical().perpendicular(fromEdge(e)) < 0)
    e = -e;

  FlatTriangulationCombinatorial::flip(e);

  ASSERT(Implementation::area(*this) == area(), "Area inconsistent after flip of edge. Area is " << Implementation::area(*this) << " but should still be " << area());
  ASSERT(Implementation::faceClosed(*this, e), "Face attached to " << e << " not closed after flip in " << *this);
  ASSERTIONS([&]() {
    static Amortized cost;
    // TODO: It would maybe be better to estimate the cost by looking at the shortest vector in direction of e.
    const auto connection = fromEdge(e);

    if (!cost.pay(relativeLength(static_cast<const Vector<T>&>(connection), impl->original->shortest(connection)) + 1)) return;

    ASSERT(connection == SaddleConnection::inSector(impl->original, connection.source(), connection),
      "Edges of Triangulation inconsistent after flip. The half edge " << e << " in the collapsed surface " << *this << " claims to correspond to the " << fromEdge(e) << ", however, there is no such saddle connection in the original surface " << *impl->original << ".");
  });
  ASSERTIONS(([&]() {
    std::unordered_map<Vertex, int> vertices;
    for (auto& vertex : uncollapsed()->vertices())
      vertices[vertex] = 0;

    for (auto halfEdge : halfEdges()) {
      const auto& connection = fromEdge(halfEdge);
      vertices[Vertex::source(connection.source(), *uncollapsed())]++;

      for (auto& collapsed: cross(halfEdge))
        vertices[Vertex::source(collapsed.source(), *uncollapsed())]++;
    }

    for (auto& count : vertices) {
      ASSERT(count.second != 0, "Vertex " << count.first << " disappeared from surface after flip; the vertex was still there in the original surface " << *uncollapsed() << " but is gone in the collapsed surface " << *this);
      ASSERT(count.second >= 2, "Vertex " << count.first << " almost disappeared from surface after flip; the vertex was still there in the original surface " << *uncollapsed() << " but it has only one outgoing edge in the collapsed surface " << *this);
    }
  }));

  if (vertical().parallel(e)) {
    collapse(e);
  }
}

template <typename T>
std::pair<HalfEdge, HalfEdge> FlatTriangulationCollapsed<T>::collapse(HalfEdge e) {
  auto ret = FlatTriangulationCombinatorial::collapse(e);

  ASSERT(Implementation::area(*this) == area(), "Area inconsistent after collapse of edge. Area is " << Implementation::area(*this) << " but should still be " << area());
  ASSERT(halfEdges() | rx::all_of([&](const auto e) { return Implementation::faceClosed(*this, e); }), "Some faces are not closed after collapse of edge in " << *this);
  ASSERTIONS([&]() {
    static Amortized cost;

    for (auto halfEdge : {ret.first, ret.second}) {
      const auto connection = fromEdge(halfEdge);
      if (!cost.pay(relativeLength(static_cast<const Vector<T>&>(connection), impl->original->shortest(connection)) + 1)) return true;
      const auto reconstruction = SaddleConnection::inSector(impl->original, connection.source(), connection);
      ASSERT(connection == reconstruction, "Edges of Triangulation inconsistent after collapse. The half edge " << e << " in the collapsed surface " << *this << " claims to correspond to the " << connection << ", however, there is no such saddle connection in the original surface " << *impl->original << " instead it should probably be " << reconstruction);
    }
    return true;
  });
  ASSERTIONS(([&]() {
    std::unordered_map<Vertex, int> vertices;
    for (auto& vertex : uncollapsed()->vertices())
      vertices[vertex] = 0;

    for (auto halfEdge : halfEdges()) {
      const auto& connection = fromEdge(halfEdge);
      vertices[Vertex::source(connection.source(), *uncollapsed())]++;

      for (auto& collapsed: cross(halfEdge))
        vertices[Vertex::source(collapsed.source(), *uncollapsed())]++;
    }

    for (auto& count : vertices) {
      ASSERT(count.second != 0, "Vertex " << count.first << " disappeared from surface after collapse; the vertex was still there in the original surface " << *uncollapsed() << " but is gone in the collapsed surface " << *this);
      ASSERT(count.second >= 2, "Vertex " << count.first << " almost disappeared from surface after collapse; the vertex was still there in the original surface " << *uncollapsed() << " but it has only one outgoing edge in the collapsed surface " << *this);
    }
  }));

  return ret;
}

template <typename T>
T FlatTriangulationCollapsed<T>::area() const noexcept {
  return impl->original->area();
}

template <typename T>
const SaddleConnection<FlatTriangulation<T>>& FlatTriangulationCollapsed<T>::fromEdge(HalfEdge e) const {
  return impl->vectors.get(e).value;
}

template <typename T>
Path<FlatTriangulation<T>> FlatTriangulationCollapsed<T>::cross(HalfEdge e) const {
  return impl->collapsedHalfEdges.get(e).connections | rx::to_vector();
}

template <typename T>
Path<FlatTriangulation<T>> FlatTriangulationCollapsed<T>::turn(HalfEdge from, HalfEdge to) const {
  std::vector<SaddleConnection> connections;

  CHECK_ARGUMENT(Vertex::source(from, *this) == Vertex::source(to, *this), "can only turn between half edges starting at the same vertex but " << from << " and " << to << " do not start at the same vertex");

  while (from != to) {
    for (auto connection : cross(from))
      connections.push_back(connection);
    from = previousAtVertex(from);
  }

  ASSERT(std::unordered_set<SaddleConnection>(begin(connections), end(connections)).size() == connections.size(), "collapsed connections cannot appear twice when turning around a vertex");

  return connections;
}

template <typename T>
std::shared_ptr<FlatTriangulationCollapsed<T>> FlatTriangulationCollapsed<T>::shared_from_this() {
  return std::static_pointer_cast<FlatTriangulationCollapsed<T>>(FlatTriangulationCombinatorial::shared_from_this());
}

template <typename T>
std::shared_ptr<const FlatTriangulation<T>> FlatTriangulationCollapsed<T>::uncollapsed() const {
  return impl->original;
}

template <typename T>
std::shared_ptr<const FlatTriangulationCollapsed<T>> FlatTriangulationCollapsed<T>::shared_from_this() const {
  return std::static_pointer_cast<const FlatTriangulationCollapsed<T>>(FlatTriangulationCombinatorial::shared_from_this());
}

template <typename T>
ImplementationOf<FlatTriangulationCollapsed<T>>::ImplementationOf(const FlatTriangulationCombinatorial& combinatorial, std::unique_ptr<FlatTriangulation<T>> surface, const Vector<T>& vertical) :
  original(std::move(surface)),
  vertical(vertical),
  collapsedHalfEdges(
      &combinatorial,
      [&](HalfEdge) {
        return CollapsedHalfEdge();
      },
      CollapsedHalfEdge::updateAfterFlip,
      CollapsedHalfEdge::updateBeforeCollapse),
  vectors(
      &combinatorial,
      [&](HalfEdge e) {
        return AsymmetricConnection{SaddleConnection(original, e)};
      },
      updateAfterFlip, updateBeforeCollapse) {
}

template <typename T>
template <typename M>
void ImplementationOf<FlatTriangulationCollapsed<T>>::handleFlip(M& map, HalfEdge flip, const std::function<void(const FlatTriangulationCollapsed<T>&, HalfEdge, HalfEdge, HalfEdge, HalfEdge)>& handler) {
  const auto& surface = static_cast<const FlatTriangulationCollapsed<T>&>(map.parent());

  // The flip turned (a b flip)(c d -flip) into (a -flip d)(c flip b)
  const HalfEdge a = surface.previousInFace(-flip);
  const HalfEdge b = surface.nextInFace(flip);
  const HalfEdge c = surface.previousInFace(flip);
  const HalfEdge d = surface.nextInFace(-flip);

  handler(surface, a, b, c, d);
}

template <typename T>
template <typename M>
void ImplementationOf<FlatTriangulationCollapsed<T>>::handleCollapse(M& map, Edge collapse_, const std::function<void(const FlatTriangulationCollapsed<T>&, HalfEdge)>& handler) {
  const auto& surface = static_cast<const FlatTriangulationCollapsed<T>&>(map.parent());
  HalfEdge collapse = collapse_.positive();

  ASSERT(surface.vertical().parallel(collapse), "cannot collapse non-vertical edge " << collapse);

  if (surface.vertical().parallel(surface.fromEdge(collapse)) < 0)
    collapse = -collapse;

  handler(surface, collapse);
}

template <typename T>
T ImplementationOf<FlatTriangulationCollapsed<T>>::area(const FlatTriangulationCollapsed<T>& self) {
  T area = T();

  for (auto e : self.halfEdges()) {
    if (self.boundary(e)) throw std::logic_error("not implemented: area with boundary");

    for (auto connection : self.cross(e)) {
      area += Vector<T>::area({connection, static_cast<Vector<T>>(self.fromEdge(e)) - connection, -self.fromEdge(e)});
    }

    // Fully collapsed triangles have no area themselves.
    if (self.nextInFace(e) == self.previousInFace(e)) continue;
    // Do not count triangles three times.
    if (e.index() > self.nextInFace(e).index()) continue;
    if (e.index() > self.previousInFace(e).index()) continue;
    
    area += Vector<T>::area({self.fromEdge(e), self.fromEdge(self.nextInFace(e)), self.fromEdge(self.previousInFace(e))});
  }

  return area;
}

template <typename T>
bool ImplementationOf<FlatTriangulationCollapsed<T>>::faceClosed(const FlatTriangulationCollapsed<T>& self, HalfEdge face) {
  if (self.boundary(face))
    return true;
  if (self.nextInFace(face) == -face)
    return true;
  T zero = self.vertical().perpendicular(self.fromEdge(face)) + self.vertical().perpendicular(self.fromEdge(self.nextInFace(face))) + self.vertical().perpendicular(self.fromEdge(self.previousInFace(face)));
  return zero == 0;
}

// TODO: Can I get rid of all of the .value here?

template <typename T>
void ImplementationOf<FlatTriangulationCollapsed<T>>::updateAfterFlip(HalfEdgeMap<AsymmetricConnection>& vectors, HalfEdge flip) {
  // TODO: Speed this up. We can very often get away without creating a new
  // Chain/SaddleConnection by using more move and inplace operations. There
  // should probably be a destructive static_cast from SaddleConnection&& to
  // Chain.
  ImplementationOf::handleFlip(vectors, flip, [&](const auto& surface, HalfEdge a, HalfEdge b, HalfEdge c, HalfEdge d) {
    const auto sum = [&](const auto& lhs, const auto& rhs) {
      return SaddleConnection(surface.impl->original, lhs.source(), rhs.target(), static_cast<const Chain<FlatTriangulation<T>>&>(lhs) + static_cast<const Chain<FlatTriangulation<T>>&>(rhs));
    };

    auto& collapsedHalfEdges = surface.impl->collapsedHalfEdges;

    // The flip turned (a b flip)(c d -flip) into (a -flip d)(c flip b)

    // We pull b down over the connections hidden in flip …
    for (const auto& connection : collapsedHalfEdges.get(flip).connections) {
      vectors.set(b, sum(vectors.get(b).value, connection));
    }

    // … and push d up over the connections hidden in -flip.
    for (const auto& connection : collapsedHalfEdges.get(-flip).connections) {
      vectors.set(d, sum(vectors.get(d).value, connection));
    }

    // Now the connections stored at flip actually belong into -b …
    {
      auto& source = collapsedHalfEdges.get(flip).connections;
      auto& target = collapsedHalfEdges.get(-b).connections;
      target.splice(target.end(), source);
    }

    // … and the connections stored at -flip actually belong into -d.
    {
      auto& source = collapsedHalfEdges.get(-flip).connections;
      auto& target = collapsedHalfEdges.get(-d).connections;
      target.splice(target.end(), source);
    }

    // Since no connections are hidden inside flip and -flip anymore, we have a
    // regular pair of faces and can deduce their vectors:
    vectors.set(flip, sum(vectors.get(d).value, vectors.get(a).value));
    vectors.set(-flip, -vectors.get(flip).value);

    ASSERT(vectors.get(-flip).value == sum(vectors.get(b).value, vectors.get(c).value), "face not closed after flip");
  });
}

template <typename T>
void ImplementationOf<FlatTriangulationCollapsed<T>>::updateBeforeCollapse(HalfEdgeMap<AsymmetricConnection>& vectors, Edge collapse_) {
  ImplementationOf::handleCollapse(vectors, collapse_, [&](const auto& surface, HalfEdge collapse) {
    auto& collapsedHalfEdges = surface.impl->collapsedHalfEdges;

    // Consider the faces (a -collapse d) and (c collapse b)
    const HalfEdge a = surface.previousInFace(-collapse);
    const HalfEdge b = surface.nextInFace(collapse);
    const HalfEdge c = surface.previousInFace(collapse);
    const HalfEdge d = surface.nextInFace(-collapse);

    // The new connection we need to record
    auto connection = vectors.get(collapse).value;

    ASSERT(-connection == vectors.get(-collapse), "the vertical half edge cannot hide collapsed saddle connections so it must be the same in both of its faces but " << collapse << " is " << connection << " and " << -collapse << " is " << vectors.get(-collapse));

    collapsedHalfEdges.get(b).connections.push_front(connection);
    collapsedHalfEdges.get(d).connections.push_front(-connection);

    auto set = [&](HalfEdge target, HalfEdge source) {
      vectors.set(target, vectors.get(source));
      collapsedHalfEdges.set(target, collapsedHalfEdges.get(source));
    };

    auto connections = [&](HalfEdge e) -> auto& {
      return collapsedHalfEdges.get(e).connections;
    };

    // The idea is to take the outer half edges of the collapsed gadget and
    // reset the vectors attached to the inner edges by flowing through the
    // gadget, e.g. we replace the inner edge a by flowing through the
    // collapsed gadget to -d, i.e., a := -d …

    // However, things get more complicated when some of the edges are identified.
    // (Attempts to squeeze this into a generic piece of code, always ran into
    // some weird troubles. So we just special case everything unfortunately.)
    if (a == -c && b == -d) {
      // Opposite sides are identified so the entire gadget collapses to a
      // single pair of half edges.
      // We squash everything into the half edge a here:
      vectors.set(-a, -vectors.get(a).value);

      connections(a).splice(connections(a).end(), connections(b));
      connections(-a).splice(connections(-a).begin(), connections(-b));

      set(b, a);
      set(-b, -a);
    } else if (a == -c) {
      // There is an inner edge (a - c) that needs to be collapsed.
      connections(-b).splice(connections(-b).end(), connections(c));
      connections(-b).splice(connections(-b).end(), connections(d));
      connections(-d).splice(connections(-d).end(), connections(a));
      connections(-d).splice(connections(-d).end(), connections(b));

      set(a, -d);
      set(b, -d);
      set(c, -b);
      set(d, -b);
    } else if (b == -d) {
      // There is an inner edge (b - d) that needs to be collapsed.
      connections(-a).splice(connections(-a).end(), connections(d));
      connections(-a).splice(connections(-a).end(), connections(c));
      connections(-c).splice(connections(-c).end(), connections(b));
      connections(-c).splice(connections(-c).end(), connections(a));

      set(a, -c);
      set(b, -c);
      set(c, -a);
      set(d, -a);
    } else if (a == -d || b == -c) {
      // The left and/or right side collapses to a single edge.
      if (a == -d) {
        // The right side collapses. Since we pushed the SaddleConnection to
        // the front of d, we need to account for that connection in d.
        vectors.set(d, -vectors.get(-d).value);
      } else {
        // The right side does not collapse
        connections(-a).splice(connections(-a).end(), connections(d));
        connections(-d).splice(connections(-d).end(), connections(a));

        set(a, -d);
        set(d, -a);
      }
      if (b == -c) {
        // The left side collapses. Since we pushed the SaddleConnection to the
        // front of b, we need to account for that connection in b.
        vectors.set(b, -vectors.get(-b).value);
      } else {
        // The left side does not collapse
        connections(-b).splice(connections(-b).end(), connections(c));
        connections(-c).splice(connections(-c).end(), connections(b));

        set(b, -c);
        set(c, -b);
      }
    } else {
      ASSERT(std::unordered_set<HalfEdge>({a, b, c, d, -a, -b, -c, -d}).size() == 8, "Unhandled identification in gadget (" << collapse << " " << b << " " << c << ")(" << -collapse << " " << d << " " << a << ")");

      connections(-a).splice(connections(-a).end(), connections(d));
      connections(-b).splice(connections(-b).end(), connections(c));
      connections(-c).splice(connections(-c).end(), connections(b));
      connections(-d).splice(connections(-d).end(), connections(a));

      set(d, -a);
      set(b, -c);
      set(a, -d);
      set(c, -b);
    }

    assert(collapsedHalfEdges.get(-a).connections.size());
    assert(collapsedHalfEdges.get(b).connections.size());
    assert(collapsedHalfEdges.get(-c).connections.size());
    assert(collapsedHalfEdges.get(d).connections.size());
  });
}

template <typename T>
ostream& operator<<(ostream& os, const FlatTriangulationCollapsed<T>& self) {
  os << static_cast<const FlatTriangulationCombinatorial&>(self);
  os << " with vectors ";

  std::unordered_map<HalfEdge, std::string> vectors;
  for (auto e : self.halfEdges()) {
    if (self.impl->vectors.get(e).value == -self.impl->vectors.get(-e).value && e == Edge(e).negative())
      continue;
    auto connection = self.impl->vectors.get(e).value;
    if (connection.source() == e && connection.target() == -e)
      vectors[e] = format("{}", static_cast<Vector<T>>(connection));
    else
      vectors[e] = format("{}", connection);
  }
  os << format("{}", fmt::join(vectors | transform([](const auto& ev) { return format("{}: {}", ev.first, ev.second); }) | to_vector(), ", "));

  if (boost::lexical_cast<std::string>(self.impl->collapsedHalfEdges).size())
    os << ", collapsed half edges " << self.impl->collapsedHalfEdges;
  return os << " with respect to " << self.impl->vertical;
}

}  // namespace flatsurf

// Instantiations of templates so implementations are generated for the linker
#include "util/instantiate.ipp"

LIBFLATSURF_INSTANTIATE_MANY_WRAPPED((LIBFLATSURF_INSTANTIATE_WITH_IMPLEMENTATION), FlatTriangulationCollapsed, LIBFLATSURF_REAL_TYPES)
