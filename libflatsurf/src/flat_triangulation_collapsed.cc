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

#include "../flatsurf/flat_triangulation_collapsed.hpp"

#include <fmt/format.h>
#include <fmt/ostream.h>
#include <gmpxx.h>

#include <gmpxxll/mpz_class.hpp>
#include <intervalxt/sample/element_floor_division.hpp>
#include <intervalxt/sample/integer_floor_division.hpp>
#include <intervalxt/sample/mpq_floor_division.hpp>
#include <intervalxt/sample/mpz_floor_division.hpp>
#include <intervalxt/sample/renf_elem_floor_division.hpp>
#include <ostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "../flatsurf/bound.hpp"
#include "../flatsurf/ccw.hpp"
#include "../flatsurf/edge.hpp"
#include "../flatsurf/flat_triangulation.hpp"
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
#include "external/rx-ranges/include/rx/ranges.hpp"
#include "impl/assert_connection.hpp"
#include "impl/collapsed_half_edge.hpp"
#include "impl/flat_triangulation.impl.hpp"
#include "impl/flat_triangulation_collapsed.impl.hpp"
#include "impl/flat_triangulation_combinatorial.impl.hpp"
#include "impl/saddle_connection.impl.hpp"
#include "util/assert.ipp"
#include "util/union_find.ipp"

namespace flatsurf {

using std::begin;
using std::end;

template <typename T>
FlatTriangulationCollapsed<T>::FlatTriangulationCollapsed() noexcept :
  FlatTriangulationCollapsed(FlatTriangulation<T>(), Vector<T>(0, 1)) {}

template <typename T>
FlatTriangulationCollapsed<T>::FlatTriangulationCollapsed(const FlatTriangulation<T>& surface, const Vector<T>& vertical) :
  FlatTriangulationCombinatorics<FlatTriangulationCollapsed>(ProtectedConstructor{}, std::make_shared<ImplementationOf<FlatTriangulationCollapsed>>(surface, vertical)) {
  while ([&]() {
    for (auto e : this->halfEdges()) {
      if (this->vertical().ccw(e) == CCW::COLLINEAR) {
        self->collapse(e);
        return true;
      }
    }
    return false;
  }())
    ;
}

template <typename T>
Vertical<FlatTriangulationCollapsed<T>> FlatTriangulationCollapsed<T>::vertical() const {
  return Vertical(*this, self->vertical);
}

template <typename T>
bool FlatTriangulationCollapsed<T>::inSector(HalfEdge sector, const Vector<T>& vector) const {
  return fromHalfEdge(sector).vector().ccw(vector) != CCW::CLOCKWISE && (-fromHalfEdge(this->previousInFace(sector))).vector().ccw(vector) == CCW::CLOCKWISE;
}

template <typename T>
mpz_class relativeCost(const Vector<T>& dividend, const Vector<T>& divisor) {
  const auto abs = [](const auto& x) { return x < 0 ? -x : x; };

  return gmpxxll::mpz_class(::intervalxt::sample::FloorDivision<T>()(dividend * dividend, abs(dividend * divisor)));
}

template <typename T>
T FlatTriangulationCollapsed<T>::area() const {
  return uncollapsed().area();
}

template <typename T>
const SaddleConnection<FlatTriangulation<T>>& FlatTriangulationCollapsed<T>::fromHalfEdge(HalfEdge e) const {
  return self->vectors->operator[](e);
}

template <typename T>
Path<FlatTriangulation<T>> FlatTriangulationCollapsed<T>::cross(HalfEdge e) const {
  const auto connections = self->collapsedHalfEdges->operator[](e).connections;
  return connections | rx::to_vector();
}

template <typename T>
Path<FlatTriangulation<T>> FlatTriangulationCollapsed<T>::turn(HalfEdge from, HalfEdge to) const {
  std::vector<SaddleConnection> connections;

  LIBFLATSURF_CHECK_ARGUMENT(Vertex::source(from, *this) == Vertex::source(to, *this), "can only turn between half edges starting at the same vertex but " << from << " and " << to << " do not start at the same vertex");

  while (from != to) {
    for (auto connection : cross(from))
      connections.push_back(connection);
    from = this->previousAtVertex(from);
  }

  LIBFLATSURF_ASSERT(std::unordered_set<SaddleConnection>(begin(connections), end(connections)).size() == connections.size(), "collapsed connections cannot appear twice when turning around a vertex");

  return connections;
}

template <typename T>
const FlatTriangulation<T>& FlatTriangulationCollapsed<T>::uncollapsed() const {
  return self->original;
}

template <typename T>
bool FlatTriangulationCollapsed<T>::operator==(const FlatTriangulationCollapsed<T>& rhs) const {
  return uncollapsed() == rhs.uncollapsed() && vertical().ccw(rhs.vertical()) == CCW::COLLINEAR && vertical().orientation(rhs.vertical()) == ORIENTATION::SAME;
}

template <typename T>
ImplementationOf<FlatTriangulationCollapsed<T>>::ImplementationOf(const FlatTriangulation<T>& surface, const Vector<T>& vertical) :
  ImplementationOf<FlatTriangulationCombinatorial>(
      ImplementationOf<FlatTriangulationCombinatorial>::self(static_cast<const FlatTriangulationCombinatorial&>(surface))->vertices,
      surface.halfEdges() | rx::filter([&](HalfEdge he) { return surface.boundary(he); }) | rx::to_vector()),
  original(surface),
  vertical(vertical),
  collapsedHalfEdges([&]() {
    // We keep track of collapsed half edges in a Tracked<> object. To
    // construct such an object, we need the surface it is tracking. However,
    // that surface is the surface we are in the process of building. In
    // particular it is not referenced by shared pointer yet. Therefore, we now
    // build a combinatorial triangulation from the data we are building and
    // wrap it in a shared pointer that does *not* free its memory when it goes
    // out of scope.
    auto self = from_this(std::shared_ptr<ImplementationOf>(this, [](auto*) {}));
    auto ret = Tracked<HalfEdgeMap<CollapsedHalfEdge>>(
        self,
        HalfEdgeMap<CollapsedHalfEdge>(
            surface,
            [](const auto&) { return CollapsedHalfEdge(); }),
        CollapsedHalfEdge::updateAfterFlip,
        CollapsedHalfEdge::updateBeforeCollapse);
    // The shared pointer we used to build the Tracked is not going to remain
    // valid so we assert that noone else is holding on to it because it won't
    // work for other use cases than Tracked<>.
    LIBFLATSURF_ASSERT(self.self.state.use_count() == 1, "Something is holding to an short lived shared pointer to a surface. This shared pointer is not actually valid and should not be used outside of Tracked<>.");
    return ret;
  }()),
  vectors([&]() {
    // We keep track of collapsed half edges in a Tracked<> object. To
    // construct such an object, we need the surface it is tracking. However,
    // that surface is the surface we are in the process of building. In
    // particular it is not referenced by shared pointer yet. Therefore, we now
    // build a combinatorial triangulation from the data we are building and
    // wrap it in a shared pointer that does *not* free its memory when it goes
    // out of scope.
    auto self = from_this(std::shared_ptr<ImplementationOf>(this, [](auto*) {}));
    auto ret = Tracked<HalfEdgeMap<SaddleConnection>>(
        self,
        HalfEdgeMap<SaddleConnection>(
            surface,
            [&](HalfEdge e) { return SaddleConnection(original, e); }),
        updateAfterFlip, updateBeforeCollapse);
    // The shared pointer we used to build the Tracked is not going to remain
    // valid so we assert that noone else is holding on to it because it won't
    // work for other use cases than Tracked<>.
    LIBFLATSURF_ASSERT(self.self.state.use_count() == 1, "Something is holding to an short lived shared pointer to a surface. This shared pointer is not actually valid and should not be used outside of Tracked<>.");
    return ret;
  }()) {}

template <typename T>
T ImplementationOf<FlatTriangulationCollapsed<T>>::area() {
  auto self = from_this();

  T area = T();

  for (auto e : self.halfEdges()) {
    if (self.boundary(e)) throw std::logic_error("not implemented: area with boundary");

    for (auto connection : self.cross(e)) {
      area += Vector<T>::area({connection, static_cast<Vector<T>>(self.fromHalfEdge(e)) - connection, -self.fromHalfEdge(e)});
    }

    // Fully collapsed triangles have no area themselves.
    if (self.nextInFace(e) == self.previousInFace(e)) continue;
    // Do not count triangles three times.
    if (e.index() > self.nextInFace(e).index()) continue;
    if (e.index() > self.previousInFace(e).index()) continue;

    area += Vector<T>::area({self.fromHalfEdge(e), self.fromHalfEdge(self.nextInFace(e)), self.fromHalfEdge(self.previousInFace(e))});
  }

  return area;
}

template <typename T>
bool ImplementationOf<FlatTriangulationCollapsed<T>>::faceClosed(HalfEdge face) {
  auto self = from_this();

  if (self.boundary(face))
    return true;
  if (self.nextInFace(face) == -face)
    return true;
  T zero = self.vertical().projectPerpendicular(face) + self.vertical().projectPerpendicular(self.nextInFace(face)) + self.vertical().projectPerpendicular(self.previousInFace(face));
  return zero == 0;
}

template <typename T>
void ImplementationOf<FlatTriangulationCollapsed<T>>::updateAfterFlip(HalfEdgeMap<SaddleConnection>& vectors, const FlatTriangulationCombinatorial& combinatorial, HalfEdge flip) {
  auto& surface = const_cast<FlatTriangulationCollapsed<T>&>(reinterpret_cast<const FlatTriangulationCollapsed<T>&>(combinatorial));

  const HalfEdge a = surface.previousInFace(-flip);
  const HalfEdge b = surface.nextInFace(flip);
  const HalfEdge c = surface.previousInFace(flip);
  const HalfEdge d = surface.nextInFace(-flip);

  const auto sum = [&](const auto& lhs, const auto& rhs) {
    return SaddleConnection::counterclockwise(surface.self->original, lhs.source(), rhs.target(), static_cast<const Chain<FlatTriangulation<T>>&>(lhs) + static_cast<const Chain<FlatTriangulation<T>>&>(rhs));
  };

  auto& collapsedHalfEdges = surface.self->collapsedHalfEdges;

  // The flip turned (a b flip)(c d -flip) into (a -flip d)(c flip b)

  // We pull b down over the connections hidden in flip …
  for (const auto& connection : collapsedHalfEdges->operator[](flip).connections) {
    vectors[b] = sum(vectors[b], connection);
  }

  // … and push d up over the connections hidden in -flip.
  for (const auto& connection : collapsedHalfEdges->operator[](-flip).connections) {
    vectors[d] = sum(vectors[d], connection);
  }

  // Now the connections stored at flip actually belong into -b …
  {
    auto& source = collapsedHalfEdges->operator[](flip).connections;
    auto& target = collapsedHalfEdges->operator[](-b).connections;
    target.splice(end(target), source);
  }

  // … and the connections stored at -flip actually belong into -d.
  {
    auto& source = collapsedHalfEdges->operator[](-flip).connections;
    auto& target = collapsedHalfEdges->operator[](-d).connections;
    target.splice(end(target), source);
  }

  // Since no connections are hidden inside flip and -flip anymore, we have a
  // regular pair of faces and can deduce their vectors:
  vectors[flip] = sum(vectors[d], vectors[a]);
  vectors[-flip] = -vectors[flip];

  LIBFLATSURF_ASSERT(vectors[-flip] == sum(vectors[b], vectors[c]), "face not closed after flip");
}

template <typename T>
void ImplementationOf<FlatTriangulationCollapsed<T>>::updateBeforeCollapse(HalfEdgeMap<SaddleConnection>& vectors, const FlatTriangulationCombinatorial& combinatorial, Edge collapse_) {
  auto& surface = const_cast<FlatTriangulationCollapsed<T>&>(reinterpret_cast<const FlatTriangulationCollapsed<T>&>(combinatorial));

  HalfEdge collapse = collapse_.positive();

  LIBFLATSURF_ASSERT(surface.vertical().ccw(collapse) == CCW::COLLINEAR, "cannot collapse non-vertical edge " << collapse);

  if (surface.vertical().orientation(collapse) == ORIENTATION::OPPOSITE)
    collapse = -collapse;

  auto& collapsedHalfEdges = surface.self->collapsedHalfEdges;

  // Consider the faces (a -collapse d) and (c collapse b)
  const HalfEdge a = surface.previousInFace(-collapse);
  const HalfEdge b = surface.nextInFace(collapse);
  const HalfEdge c = surface.previousInFace(collapse);
  const HalfEdge d = surface.nextInFace(-collapse);

  // The new connection we need to record
  auto connection = vectors[collapse];

  LIBFLATSURF_ASSERT(-connection == vectors[-collapse], "the vertical half edge cannot hide collapsed saddle connections so it must be the same in both of its faces but " << collapse << " is " << connection << " and " << -collapse << " is " << vectors[-collapse]);

  collapsedHalfEdges->operator[](b).connections.push_front(connection);
  collapsedHalfEdges->operator[](d).connections.push_front(-connection);

  auto set = [&](HalfEdge target, HalfEdge source) {
    vectors[target] = vectors[source];
    collapsedHalfEdges->operator[](target) = collapsedHalfEdges->operator[](source);
  };

  auto connections = [&](HalfEdge e) -> auto& {
    return collapsedHalfEdges->operator[](e).connections;
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
    vectors[-a] = -vectors[a];

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
      vectors[d] = -vectors[-d];
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
      vectors[b] = -vectors[-b];
    } else {
      // The left side does not collapse
      connections(-b).splice(connections(-b).end(), connections(c));
      connections(-c).splice(connections(-c).end(), connections(b));

      set(b, -c);
      set(c, -b);
    }
  } else {
    LIBFLATSURF_ASSERT(std::unordered_set<HalfEdge>({a, b, c, d, -a, -b, -c, -d}).size() == 8, "Unhandled identification in gadget (" << collapse << " " << b << " " << c << ")(" << -collapse << " " << d << " " << a << ")");

    connections(-a).splice(connections(-a).end(), connections(d));
    connections(-b).splice(connections(-b).end(), connections(c));
    connections(-c).splice(connections(-c).end(), connections(b));
    connections(-d).splice(connections(-d).end(), connections(a));

    set(d, -a);
    set(b, -c);
    set(a, -d);
    set(c, -b);
  }

  assert(collapsedHalfEdges->operator[](-a).connections.size());
  assert(collapsedHalfEdges->operator[](b).connections.size());
  assert(collapsedHalfEdges->operator[](-c).connections.size());
  assert(collapsedHalfEdges->operator[](d).connections.size());
}

template <typename T>
void ImplementationOf<FlatTriangulationCollapsed<T>>::flip(HalfEdge e) {
  auto self = from_this();

  LIBFLATSURF_CHECK_ARGUMENT(self.vertical().large(e), "in a CollapsedSurface, only large edges can be flipped");
  LIBFLATSURF_CHECK_ARGUMENT(self.nextInFace(self.nextInFace(self.nextInFace(e))) == e && self.nextInFace(self.nextInFace(self.nextInFace(-e))) == -e, "in a CollapsedSurface, only edges that are not in a collapsed face can be fliped");

  if (self.vertical().ccw(e) == CCW::COUNTERCLOCKWISE)
    e = -e;

  ImplementationOf<FlatTriangulationCombinatorial>::flip(e);

  LIBFLATSURF_ASSERT(area() == self.area(), "Area inconsistent after flip of edge. Area is " << area() << " but should still be " << self.area());
  LIBFLATSURF_ASSERT(faceClosed(e), "Face attached to " << e << " not closed after flip in " << self);
  LIBFLATSURF_ASSERTIONS(([&]() {
    static thread_local AssertConnection<T> assertion;

    LIBFLATSURF_ASSERT(assertion(self.fromHalfEdge(e)), "Edges of Triangulation inconsistent after flip. The half edge " << e << " in the collapsed surface " << self << " claims to correspond to the " << self.fromHalfEdge(e) << ", however, there is no such saddle connection in the original surface " << original << ".");
  }));

  LIBFLATSURF_ASSERTIONS(([&]() {
    std::unordered_map<Vertex, int> vertices;
    for (auto& vertex : original->vertices())
      vertices[vertex] = 0;

    for (auto halfEdge : self.halfEdges()) {
      const auto& connection = self.fromHalfEdge(halfEdge);
      vertices[Vertex::source(connection.source(), *original)]++;

      for (auto& collapsed : self.cross(halfEdge))
        vertices[Vertex::source(collapsed.source(), *original)]++;
    }

    for (auto& count : vertices) {
      LIBFLATSURF_ASSERT(count.second != 0, "Vertex " << count.first << " disappeared from surface after flip; the vertex was still there in the original surface " << original << " but is gone in the collapsed surface " << self);
      LIBFLATSURF_ASSERT(count.second >= 2, "Vertex " << count.first << " almost disappeared from surface after flip; the vertex was still there in the original surface " << original << " but it has only one outgoing edge in the collapsed surface " << self);
    }
  }));

  if (self.vertical().ccw(e) == CCW::COLLINEAR) {
    collapse(e);
  }
}

template <typename T>
std::pair<HalfEdge, HalfEdge> ImplementationOf<FlatTriangulationCollapsed<T>>::collapse(HalfEdge e) {
  auto self = from_this();

  auto ret = ImplementationOf<FlatTriangulationCombinatorial>::collapse(e);

  LIBFLATSURF_ASSERT(area() == self.area(), "Area inconsistent after collapse of edge. Area is " << area() << " but should still be " << self.area());
  LIBFLATSURF_ASSERT(self.halfEdges() | rx::all_of([&](const auto e) { return faceClosed(e); }), "Some faces are not closed after collapse of edge in " << self);
  LIBFLATSURF_ASSERTIONS(([&]() {
    static thread_local AssertConnection<T> assertion;

    for (auto halfEdge : {ret.first, ret.second}) {
      const auto connection = self.fromHalfEdge(halfEdge);
      LIBFLATSURF_ASSERT(assertion(connection), "Edges of Triangulation inconsistent after collapse. The half edge " << e << " in the collapsed surface " << self << " claims to correspond to the " << connection << ", however, there is no such saddle connection in the original surface " << original << ".");
    }
  }));

  LIBFLATSURF_ASSERTIONS(([&]() {
    std::unordered_map<Vertex, int> vertices;
    for (auto& vertex : original->vertices())
      vertices[vertex] = 0;

    for (auto halfEdge : self.halfEdges()) {
      const auto& connection = self.fromHalfEdge(halfEdge);
      vertices[Vertex::source(connection.source(), *original)]++;

      for (auto& collapsed : self.cross(halfEdge))
        vertices[Vertex::source(collapsed.source(), *original)]++;
    }

    for (auto& count : vertices) {
      LIBFLATSURF_ASSERT(count.second != 0, "Vertex " << count.first << " disappeared from surface after collapse; the vertex was still there in the original surface " << original << " but is gone in the collapsed surface " << self);
      LIBFLATSURF_ASSERT(count.second >= 2, "Vertex " << count.first << " almost disappeared from surface after collapse; the vertex was still there in the original surface " << original << " but it has only one outgoing edge in the collapsed surface " << self);
    }
  }));

  return ret;
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const FlatTriangulationCollapsed<T>& self) {
  os << static_cast<const FlatTriangulationCombinatorial&>(self);
  os << " with vectors ";

  std::unordered_map<HalfEdge, std::string> vectors;
  for (auto e : self.halfEdges()) {
    if (self.self->vectors->operator[](e) == -self.self->vectors->operator[](-e) && e == Edge(e).negative())
      continue;
    auto connection = self.self->vectors->operator[](e);
    if (connection.source() == e && connection.target() == -e)
      vectors[e] = fmt::format("{}", static_cast<Vector<T>>(connection));
    else
      vectors[e] = fmt::format("{}", connection);
  }
  os << fmt::format("{}", fmt::join(vectors | rx::transform([](const auto& ev) { return fmt::format("{}: {}", ev.first, ev.second); }) | rx::to_vector(), ", "));

  if (boost::lexical_cast<std::string>(self.self->collapsedHalfEdges).size())
    os << ", collapsed half edges " << self.self->collapsedHalfEdges;
  return os << " with respect to " << self.self->vertical;
}

}  // namespace flatsurf

// Instantiations of templates so implementations are generated for the linker
#include "util/instantiate.ipp"

LIBFLATSURF_INSTANTIATE_MANY_WRAPPED((LIBFLATSURF_INSTANTIATE_WITH_IMPLEMENTATION), FlatTriangulationCollapsed, LIBFLATSURF_REAL_TYPES)
