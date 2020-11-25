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

#include "../flatsurf/flat_triangulation.hpp"

#include <boost/type_traits/is_detected.hpp>
#include <exact-real/arb.hpp>
#include <exact-real/integer_ring.hpp>
#include <exact-real/number_field.hpp>
#include <exact-real/rational_field.hpp>
#include <exact-real/yap/arb.hpp>
#include <functional>
#include <iosfwd>
#include <map>
#include <ostream>
#include <type_traits>
#include <vector>

#include "../flatsurf/bound.hpp"
#include "../flatsurf/ccw.hpp"
#include "../flatsurf/deformation.hpp"
#include "../flatsurf/delaunay.hpp"
#include "../flatsurf/edge.hpp"
#include "../flatsurf/edge_set.hpp"
#include "../flatsurf/half_edge.hpp"
#include "../flatsurf/half_edge_set.hpp"
#include "../flatsurf/isomorphism.hpp"
#include "../flatsurf/odd_half_edge_map.hpp"
#include "../flatsurf/orientation.hpp"
#include "../flatsurf/saddle_connection.hpp"
#include "../flatsurf/saddle_connections.hpp"
#include "../flatsurf/vector.hpp"
#include "../flatsurf/vertical.hpp"
#include "external/rx-ranges/include/rx/ranges.hpp"
#include "impl/approximation.hpp"
#include "impl/deformation.impl.hpp"
#include "impl/flat_triangulation.impl.hpp"
#include "impl/flat_triangulation_combinatorial.impl.hpp"
#include "impl/quadratic_polynomial.hpp"
#include "impl/transformation_deformation.hpp"
#include "util/assert.ipp"

namespace flatsurf {

using std::begin;
using std::end;

template <typename T>
Deformation<FlatTriangulation<T>> FlatTriangulation<T>::operator+(const OddHalfEdgeMap<Vector<T>> &shift) const {
  // Half edges that collapse at the end of the shift.
  EdgeSet collapsing;

  // Records that the half edge e needs to be flipped at a time t in (0, 1]
  // that is given by a solution to det(t) = a*t^2 + b*t + c = 0.
  struct Flip {
    HalfEdge flip;
    QuadraticPolynomial<T> det;
  };

  std::optional<Flip> flip;

  for (auto vertex : this->vertices()) {
    const auto outgoing = this->atVertex(vertex);

    // The x, y coordinates of the half edge he
    const auto x = [&](const HalfEdge he) { return fromHalfEdge(he).x(); };
    const auto y = [&](const HalfEdge he) { return fromHalfEdge(he).y(); };
    // The x, y shifts of the half edge he at time t = 1
    const auto u = [&](const HalfEdge he) { return shift.get(he).x(); };
    const auto v = [&](const HalfEdge he) { return shift.get(he).y(); };

    // One reason why the area of a triangle is zero for a time t in [0, 1] is
    // that two singularities were shifted into each other. We can make sense
    // of this when it happens at time t=1 by collapsing triangles.
    for (size_t i = 0; i < outgoing.size(); i++) {
      const auto he = outgoing.at(i);

      if (fromHalfEdge(he).ccw(shift.get(he)) == CCW::COLLINEAR) {
        switch (fromHalfEdge(he).orientation(fromHalfEdge(he) + shift.get(he))) {
          case ORIENTATION::SAME:
            // The critical time t is not in [0, 1]
            break;
          case ORIENTATION::OPPOSITE:
            throw std::invalid_argument("shift must not collapse half edges for a time t in (0, 1)");
          case ORIENTATION::ORTHOGONAL:
            collapsing.insert(he);
        }
      }
    }

    // The more common reason why the area of a triangle is zero is that a
    // singularity is shifted onto the interior of an edge. When this happens
    // we can flip that edge just before to make sure that our triangulation
    // remains valid at all times.
    for (size_t i = 0; i < outgoing.size(); i++) {
      const auto he = outgoing.at(i);
      const auto he_ = outgoing.at((i + 1) % outgoing.size());

      // The determinant of the vectors spanned by the edges he and he_ at time
      // t is given by a*t^2 - b*t + c.
      const auto det = QuadraticPolynomial<T>(
          u(he) * v(he_) - u(he_) * v(he),
          u(he) * y(he_) - u(he_) * y(he) + x(he) * v(he_) - x(he_) * v(he),
          x(he) * y(he_) - x(he_) * y(he));

      ASSERT(det(T()) > 0, "Original surface " << *this << " already had a triangle with non-positive area before applying any shift to it.");

      // If the determinant has a zero for any t in [0, 1], the area of a
      // triangle vanishes or becomes negative.
      // We handle the easiest case first: the area remains positive for all
      // times t in [0, 1].
      if (det.positive())
        continue;

      // We can now assume that the determinant is zero for some t in (0, 1].
      // We need to flip a half edge of this triangle if it has a vertex on its
      // interior at that critical time t.

      // But first we exclude the case that
      // the vertex ends up on the boundary of the half edge, i.e., a half edge
      // collapses.
      if (collapsing.contains(he) || collapsing.contains(he_))
        continue;

      // Determine whether our vertex moves onto the half edge opposite to it,
      // i.e., the one following he in this triangle.
      const auto vertex_hits_interior = [&]() {
        for (long prec = exactreal::ARB_PRECISION_FAST;; prec *= 2) {
          const auto t = det.root(prec);
          const auto arb = Approximation<T>::arb;
          ASSERT(t, "determinant " << det << " must have a root in [0, 1]");
          const auto et = Vector<exactreal::Arb>(
              (arb(fromHalfEdge(he).x(), prec) + *t * arb(shift.get(he).x(), prec))(prec),
              (arb(fromHalfEdge(he).y(), prec) + *t * arb(shift.get(he).y(), prec))(prec));
          const auto e_t = Vector<exactreal::Arb>(
              (arb(fromHalfEdge(he_).x(), prec) + *t * arb(shift.get(he_).x(), prec))(prec),
              (arb(fromHalfEdge(he_).y(), prec) + *t * arb(shift.get(he_).y(), prec))(prec));

          const auto orientation = et.orientation(e_t);

          if (orientation) {
            switch (*orientation) {
              case ORIENTATION::ORTHOGONAL:
                UNREACHABLE("vectors cannot be orthogonal when their determinant is vanishing");
              case ORIENTATION::SAME:
                // The half edges he and he_ meet but the vertex at their source
                // does not end up on the interior of the half edge opposite to
                // it. We can ignore this case as another vertex will take care
                // of this vanishing triangle.
                return false;
              case ORIENTATION::OPPOSITE:
                // The two edges attached to this vertex point in opposite
                // directions at time t so this vertex ends up on the interior
                // of the opposite edge.
                return true;
            }
          }
        }
      };

      if (!vertex_hits_interior())
        // The half edge following e does not need to be flipped.
        continue;

      const Flip proposed{this->nextInFace(he), det};

      // Record that a half edge needs to be flipped at time t. We'll later
      // actually flip the one that needs to be flipped first and recurse.
      if (!flip || proposed.det < flip->det)
        flip = proposed;
    }
  }

  if (flip) {
    // We want to flip the half edge that we found needs to be flipped first.
    // However, just flipping that edge right now might lead to infinite loops
    // where the same edges get flipped again and again without making any
    // progress. So instead we get a bit closer to the critical time and
    // perform the flip just then.
    // Note that this also solves the problem that the flip might not actually
    // be possibly as it might lead to a non-convex triangulation since
    // eventually, when we are close enough to the critical time, the flip will
    // be valid.
    // Note that this leads to quite some coefficient blow-up along the way;
    // every flip introduces a factor of two in the denominators. These
    // coefficients go away in the final surface. It would likely be more
    // efficient not to move before the flip if this still makes the critical
    // time t increase.
    const auto t = *flip->det.root(exactreal::ARB_PRECISION_FAST);

    for (auto s = mpq_class(1, 2);; s /= 2) {
      const auto lt = exactreal::Arb(s, exactreal::ARB_PRECISION_FAST) < t;
      if (lt && *lt) {
        const OddHalfEdgeMap<Vector<T>> partial(*this, [&](const HalfEdge he) { return shift.get(he) / s.get_den(); });
        auto closer = (*this + partial).surface();

        const Tracked<OddHalfEdgeMap<Vector<T>>> remaining(closer, OddHalfEdgeMap<Vector<T>>(closer, [&](const HalfEdge he) { return shift.get(he) - partial.get(he); }), ImplementationOf<FlatTriangulation>::updateAfterFlip);

        if (convex(flip->flip, true))
          closer.flip(flip->flip);

        return closer + remaining;
      }
    }
  } else {
    // Now we perform shifts of half edges on a copy of the surface's vector
    // structure and collapse on the combinatorial structure.
    auto combinatorial = static_cast<const FlatTriangulationCombinatorial &>(*this).clone();
    Tracked<OddHalfEdgeMap<Vector<T>>> vectors(combinatorial, OddHalfEdgeMap<Vector<T>>(combinatorial, [&](const HalfEdge he) { return fromHalfEdge(he) + shift.get(he); }),
        Tracked<OddHalfEdgeMap<Vector<T>>>::defaultFlip,
        [](OddHalfEdgeMap<Vector<T>> &vectors, const FlatTriangulationCombinatorial &, Edge e) {
          ASSERT(!vectors.get(e.positive()), "can only collapse half edges that have become trivial");
        });
    Tracked<EdgeSet> collapsing_(combinatorial, collapsing,
        Tracked<EdgeSet>::defaultFlip,
        [](EdgeSet &self, const FlatTriangulationCombinatorial &, Edge e) {
          ASSERT(self.contains(e), "can only collapse edges that have been found to collapse at t=1");
        });

    while (!collapsing_->empty())
      combinatorial.collapse(begin(static_cast<const EdgeSet &>(collapsing_))->positive());

    return ImplementationOf<Deformation<FlatTriangulation>>::make(FlatTriangulation<T>(
        std::move(combinatorial),
        [&](const HalfEdge he) { return vectors->get(he); }));
  }
}

template <typename T>
Deformation<FlatTriangulation<T>> FlatTriangulation<T>::eliminateMarkedPoints() const {
  std::optional<HalfEdge> collapse;

  for (const auto &vertex : this->vertices()) {
    if (angle(vertex) == 1) {
      for (const auto &outgoing : this->atVertex(vertex)) {
        const auto neighbour = Vertex::target(outgoing, *this);
        if (neighbour != vertex) {
          if (collapse && (fromHalfEdge(*collapse) * fromHalfEdge(*collapse)) < fromHalfEdge(outgoing) * fromHalfEdge(outgoing))
            continue;
          collapse = outgoing;
        }
      }
    }
  }

  if (!collapse)
    return Deformation(clone());

  const auto marked = Vertex::source(*collapse, *this);

  auto simplified = (*this + OddHalfEdgeMap<Vector<T>>(*this, [&](const HalfEdge he) {
    if (Vertex::source(he, *this) == marked && Vertex::target(he, *this) == marked)
      return Vector<T>();
    if (Vertex::source(he, *this) == marked)
      return -fromHalfEdge(*collapse);
    if (Vertex::target(he, *this) == marked)
      return fromHalfEdge(*collapse);

    return Vector<T>();
  })).surface();

  ASSERT(simplified.vertices().size() < this->vertices().size(), "the numbers of vertices is reduced in each step but " << *this << " was simplified to " << simplified);
  return ImplementationOf<Deformation<FlatTriangulation>>::make(simplified.eliminateMarkedPoints().surface());
}

template <typename T>
Vector<T> FlatTriangulation<T>::shortest() const {
  const auto edges = this->edges();
  Edge shortest = *std::min_element(begin(edges), end(edges), [&](const auto &a, const auto &b) {
    const Vector x = fromHalfEdge(a.positive());
    const Vector y = fromHalfEdge(b.positive());
    return x * x < y * y;
  });
  return fromHalfEdge(shortest.positive());
}

template <typename T>
Vector<T> FlatTriangulation<T>::shortest(const Vector<T> &direction) const {
  const auto edges = this->edges();
  Edge shortest = *std::min_element(begin(edges), end(edges), [&](const auto &a, const auto &b) {
    const Vector<T> av = fromHalfEdge(a.positive());
    const Vector<T> bv = fromHalfEdge(b.positive());

    const auto alen = av * direction;
    const auto blen = bv * direction;

    if (!alen) return false;
    if (!blen) return true;

    const auto abs = [](const auto &x) { return x < 0 ? -x : x; };

    return abs(alen) < abs(blen);
  });
  return fromHalfEdge(shortest.positive());
}

template <typename T>
const Vector<T> &FlatTriangulation<T>::fromHalfEdge(const HalfEdge e) const {
  return self->vectors->get(e);
}

template <typename T>
const flatsurf::Vector<exactreal::Arb> &FlatTriangulation<T>::fromHalfEdgeApproximate(HalfEdge e) const {
  return self->approximations->get(e);
}

template <typename T>
FlatTriangulation<T>::FlatTriangulation() noexcept :
  FlatTriangulation(FlatTriangulationCombinatorial(), std::vector<Vector<T>>{}) {}

template <typename T>
FlatTriangulation<T>::FlatTriangulation(FlatTriangulationCombinatorial &&combinatorial, const std::vector<Vector<T>> &vectors) :
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
  FlatTriangulationCombinatorics<FlatTriangulation>(ProtectedConstructor{}, std::make_shared<ImplementationOf<FlatTriangulation<T>>>(std::move(combinatorial), vectors)) {
  self->check();
}

template <typename T>
FlatTriangulation<T> FlatTriangulation<T>::clone() const {
  return FlatTriangulation(static_cast<const FlatTriangulationCombinatorial &>(*this).clone(), [&](HalfEdge e) { return fromHalfEdge(e); });
}

template <typename T>
Deformation<FlatTriangulation<T>> FlatTriangulation<T>::slit(HalfEdge slit) const {
  return ImplementationOf<Deformation<FlatTriangulation>>::make(FlatTriangulation(
      static_cast<const FlatTriangulationCombinatorial &>(*this).slit(slit),
      [&](HalfEdge e) {
        HalfEdge newEdge = HalfEdge(static_cast<int>(this->halfEdges().size()) / 2 + 1);
        if (e == newEdge) return fromHalfEdge(slit);
        if (e == -newEdge) return -fromHalfEdge(slit);
        return fromHalfEdge(e);
      }));
}

template <typename T>
Deformation<FlatTriangulation<T>> FlatTriangulation<T>::insertAt(HalfEdge &nextTo, const Vector<T> &slit) const {
  CHECK_ARGUMENT(inSector(nextTo, slit), "vector must be contained in the sector next to the half edge");

  FlatTriangulation<T> surface = clone();

  auto check_orientation = [&](const Vector<T> &saddle_connection) {
    auto orient = (saddle_connection - slit).orientation(slit);
    CHECK_ARGUMENT(orient != ORIENTATION::OPPOSITE, "cannot insert half edge that crosses over an existing vertex");
    if (orient == ORIENTATION::ORTHOGONAL) {
      // It is a bit unclear what to do if the new edge should end at a
      // vertex, in particular if it is collinear with an existing half
      // edge (after fliping.)
      throw std::logic_error("insertion of half edges that end at an existing vertex not implemented yet");
    }
  };

  // Search for half edges that slit would be crossing and flip them.
  // We should replace all this with a simple call to operator+, see #183.
  [&]() {
    while (true) {
      if (surface.fromHalfEdge(nextTo).ccw(slit) == CCW::COLLINEAR) {
        check_orientation(surface.fromHalfEdge(nextTo));
        // Slot is on an existing HalfEdge but does not cross a vertex.
        return;
      }
      assert(surface.fromHalfEdge(nextTo).ccw(slit) == CCW::COUNTERCLOCKWISE);

      // The half edge that slit is potentially crossing
      const HalfEdge crossing = surface.nextInFace(nextTo);
      // The base point of crossing half edge
      const Vector<T> base = surface.fromHalfEdge(nextTo);

      // Check whether slit is actually crossing crossing. It would be enough
      // to check whether this is != CLOCKWISE. However, we do not allow slit
      // to end on an edge other than nextTo. So we perform one additional
      // flip in that case so slit is actually inside of a face.
      if (surface.fromHalfEdge(crossing).ccw(slit - base) == CCW::COUNTERCLOCKWISE)
        return;

      std::function<void(HalfEdge)> flip = [&](HalfEdge e) {
        assert(e != nextTo && e != -nextTo && e != surface.nextAtVertex(nextTo) && e != -surface.nextAtVertex(nextTo));

        auto canFlip = [&](HalfEdge g) {
          return e != nextTo && e != -nextTo && e != surface.nextAtVertex(nextTo) && e != -surface.nextAtVertex(nextTo) &&
                 surface.fromHalfEdge(surface.previousAtVertex(g)).ccw(surface.fromHalfEdge(surface.nextAtVertex(g))) == CCW::COUNTERCLOCKWISE && surface.fromHalfEdge(surface.previousAtVertex(-g)).ccw(surface.fromHalfEdge(surface.nextAtVertex(-g))) == CCW::COUNTERCLOCKWISE;
        };

        while (!canFlip(e)) {
          // f is blocked by a forward triangle on top of it so we flip its top edge.
          if (slit.ccw(surface.fromHalfEdge(surface.previousAtVertex(e))) != CCW::COUNTERCLOCKWISE) {
            flip(-surface.nextAtVertex(-e));
            continue;
          } else {
            assert(slit.ccw(surface.fromHalfEdge(surface.nextAtVertex(-e))) != CCW::CLOCKWISE);
            flip(surface.previousAtVertex(e));
            continue;
          }
        }

        surface.flip(e);
      };

      // slit crosses crossing, so flip it and replace nextTo if slit is then not next to nextTo anymore.
      flip(crossing);
      assert(surface.fromHalfEdge(nextTo).ccw(slit) == CCW::COUNTERCLOCKWISE);
      while (surface.fromHalfEdge(surface.nextAtVertex(nextTo)).ccw(slit) != CCW::CLOCKWISE)
        nextTo = surface.nextAtVertex(nextTo);
    }
  }();

  auto symmetric = [](HalfEdge x, HalfEdge e, const Vector<T> &v) {
    assert(x == e || x == -e);
    return x == e ? v : -v;
  };

  if (surface.fromHalfEdge(nextTo).ccw(slit) != CCW::COLLINEAR) {
    // After the flips we did, v is now completely inside a face.
    assert(surface.fromHalfEdge(nextTo).ccw(slit) == CCW::COUNTERCLOCKWISE);

    auto combinatorial = static_cast<FlatTriangulationCombinatorial &>(surface).insertAt(nextTo);

    return ImplementationOf<Deformation<FlatTriangulation>>::make(FlatTriangulation(combinatorial.clone(), [&](const HalfEdge e) {
      HalfEdge a = -combinatorial.nextAtVertex(nextTo);
      HalfEdge b = combinatorial.nextAtVertex(a);
      HalfEdge c = combinatorial.nextAtVertex(b);

      if (Edge(e) == a) return symmetric(e, a, -slit);
      if (Edge(e) == b) return symmetric(e, b, surface.fromHalfEdge(nextTo) - slit);
      if (Edge(e) == c) return symmetric(e, c, surface.fromHalfEdge(surface.nextAtVertex(nextTo)) - slit);
      return surface.fromHalfEdge(e);
    }));
  } else {
    // After the flips we did, v is collinear with the half edge e (but shorter.)

    // Insert our half edge ee next to e
    auto combinatorial = static_cast<FlatTriangulationCombinatorial &>(surface).insertAt(nextTo);
    auto nextAtSlot = combinatorial.nextAtVertex(nextTo);
    // After a flip of slit the original slit can be recovered as nextAtSlot + eee.
    combinatorial.flip(nextTo);
    auto eee = combinatorial.nextAtVertex(combinatorial.nextAtVertex(-nextAtSlot));

    // The combinatorics are correct now, but we still have to patch up the
    // vectors, namely the four half edges meeting at the new vertex all need
    // updating.
    auto ret = FlatTriangulation(combinatorial.clone(), [&](const HalfEdge e) {
      if (Edge(e) == nextAtSlot) return symmetric(e, nextAtSlot, slit);
      if (Edge(e) == eee) return symmetric(e, eee, surface.fromHalfEdge(nextTo) - slit);
      if (Edge(e) == combinatorial.nextAtVertex(-nextAtSlot)) return symmetric(e, combinatorial.nextAtVertex(-nextAtSlot), surface.fromHalfEdge(surface.previousAtVertex(nextTo)) - slit);
      if (Edge(e) == combinatorial.nextAtVertex(eee)) return symmetric(e, combinatorial.nextAtVertex(eee), surface.fromHalfEdge(surface.nextAtVertex(nextTo)) - slit);
      return surface.fromHalfEdge(e);
    });

    nextTo = combinatorial.previousAtVertex(nextAtSlot);

    return ImplementationOf<Deformation<FlatTriangulation>>::make(std::move(ret));
  }
}

template <typename T>
void FlatTriangulation<T>::delaunay() {
  bool isDelaunay;
  do {
    isDelaunay = true;
    for (auto edge : this->edges()) {
      if (delaunay(edge) == DELAUNAY::NON_DELAUNAY) {
        isDelaunay = false;
        this->flip(edge.positive());
      }
    }
  } while (!isDelaunay);
}

template <typename T>
auto delaunay(const FlatTriangulation<T> &surface, const HalfEdge edge) {
}

template <typename T>
DELAUNAY FlatTriangulation<T>::delaunay(const Edge edge) const {
  // We could eventually use Vector::insideCircumcircle() so vectors can
  // provide optimized implementations of this. However, at the moment, it
  // does not seem worth it.

  // We use the condition described in Wikipedia (whether a certain
  // determinant is positive.) Using the notation there, the face attached to
  // this half edge is the triangle (a, b, c), and the face attached to the
  // reversed half edge is (a, c, d). We use a coordinate system where
  // d=(0,0).
  const auto ca = this->fromHalfEdge(edge.positive());
  const auto cb = this->fromHalfEdge(this->nextAtVertex(edge.positive()));
  const auto dc = this->fromHalfEdge(-this->nextInFace(edge.negative()));

  const auto a = dc + ca;
  const auto b = dc + cb;
  const auto c = dc;

  const auto det = [](const auto &x00, const auto &x01, const auto &x02, const auto &x10, const auto &x11, const auto &x12, const auto &x20, const auto &x21, const auto &x22) -> T {
    return x00 * (x11 * x22 - x12 * x21) - x10 * (x01 * x22 - x21 * x02) + x20 * (x01 * x12 - x11 * x02);
  };

  const auto del = det(a.x(), a.y(), a.x() * a.x() + a.y() * a.y(), b.x(), b.y(), b.x() * b.x() + b.y() * b.y(), c.x(), c.y(), c.x() * c.x() + c.y() * c.y());

  if (del < 0)
    return DELAUNAY::DELAUNAY;
  else if (del == 0)
    return DELAUNAY::AMBIGUOUS;
  else
    return DELAUNAY::NON_DELAUNAY;
}

template <typename T>
T FlatTriangulation<T>::area() const {
  T area = T();
  for (auto e : this->halfEdges()) {
    if (this->boundary(e)) continue;

    // Do not count every triangle three times.
    if (e.index() > this->nextInFace(e).index()) continue;
    if (e.index() > this->previousInFace(e).index()) continue;

    area += Vector<T>::area({fromHalfEdge(e), fromHalfEdge(this->nextInFace(e)), fromHalfEdge(this->nextInFace(this->nextInFace(e)))});
  }
  return area;
}

template <typename T>
FlatTriangulation<T> FlatTriangulation<T>::scale(const mpz_class &scalar) const {
  return FlatTriangulation(static_cast<const FlatTriangulationCombinatorial &>(*this).clone(), [&](HalfEdge e) {
    return scalar * fromHalfEdge(e);
  });
}

template <typename T>
bool FlatTriangulation<T>::convex(HalfEdge e, bool strict) const {
  if (strict)
    return fromHalfEdge(this->previousAtVertex(e)).ccw(fromHalfEdge(this->nextAtVertex(e))) == CCW::COUNTERCLOCKWISE &&
           fromHalfEdge(this->previousAtVertex(-e)).ccw(fromHalfEdge(this->nextAtVertex(-e))) == CCW::COUNTERCLOCKWISE;
  else
    return fromHalfEdge(this->previousAtVertex(e)).ccw(fromHalfEdge(this->nextAtVertex(e))) != CCW::CLOCKWISE &&
           fromHalfEdge(this->previousAtVertex(-e)).ccw(fromHalfEdge(this->nextAtVertex(-e))) != CCW::CLOCKWISE;
}

template <typename T>
bool FlatTriangulation<T>::inSector(const HalfEdge sector, const Vector<T> &vector) const {
  return fromHalfEdge(sector).ccw(vector) != CCW::CLOCKWISE && fromHalfEdge(this->nextAtVertex(sector)).ccw(vector) == CCW::CLOCKWISE;
}

template <typename T>
bool FlatTriangulation<T>::inSector(const HalfEdge sector, const Vertical<FlatTriangulation<T>> &vector) const {
  return inSector(sector, vector.vertical());
}

template <typename T>
SaddleConnections<FlatTriangulation<T>> FlatTriangulation<T>::connections() const {
  return SaddleConnections(*this);
}

template <typename T>
bool FlatTriangulation<T>::operator==(const FlatTriangulation<T> &rhs) const {
  if (this == &rhs)
    return true;

  if (static_cast<const FlatTriangulationCombinatorial &>(*this) != static_cast<const FlatTriangulationCombinatorial &>(rhs))
    return false;
  for (auto &edge : this->halfEdges()) {
    if (this->self->vectors->get(edge) != rhs.self->vectors->get(edge))
      return false;
  }
  return true;
}

template <typename T>
int FlatTriangulation<T>::angle(const Vertex &vertex) const {
  int angle = 0;

  const HalfEdge first = *begin(this->atVertex(vertex));
  HalfEdge current = first;
  do {
    const HalfEdge next = this->nextAtVertex(current);

    if (fromHalfEdge(current).x() >= 0 && fromHalfEdge(next).x() < 0)
      angle++;

    current = next;
  } while (current != first);

  ASSERT(angle >= 1, "Total angle at vertex cannot be less than 2π");

  return angle;
}

template <typename T>
using truediv_t = decltype(std::declval<T &>() /= std::declval<const T &>());

template <typename T>
std::optional<Deformation<FlatTriangulation<T>>> FlatTriangulation<T>::isomorphism(const FlatTriangulation<T> &other, std::function<bool(const T &, const T &, const T &, const T &)> filterMatrix, std::function<bool(HalfEdge, HalfEdge)> filterHalfEdgeMap) const {
  return this->isomorphism(other, ISOMORPHISM::FACES, filterMatrix, filterHalfEdgeMap);
}

template <typename T>
std::optional<Deformation<FlatTriangulation<T>>> FlatTriangulation<T>::isomorphism(const FlatTriangulation<T> &other, ISOMORPHISM kind, std::function<bool(const T &, const T &, const T &, const T &)> filterMatrix, std::function<bool(HalfEdge, HalfEdge)> filterHalfEdgeMap) const {
  if (this->hasBoundary() != other.hasBoundary())
    return std::nullopt;

  if (this->halfEdges().size() != other.halfEdges().size())
    return std::nullopt;

  if (this->hasBoundary())
    throw std::logic_error("not implemented: isomorphism() not implemented for surfaces with boundary");

  const auto ignore = [&](HalfEdge he) {
    return kind == ISOMORPHISM::FACES ? false : this->delaunay(he.edge()) == DELAUNAY::AMBIGUOUS;
  };
  const auto ignoreImage = [&](HalfEdge he) {
    return kind == ISOMORPHISM::FACES ? false : this->delaunay(he.edge()) == DELAUNAY::AMBIGUOUS;
  };

  if (kind == ISOMORPHISM::DELAUNAY_CELLS) {
    ASSERT(this->edges() | rx::all_of([&](const auto e) { return this->delaunay(e) != DELAUNAY::NON_DELAUNAY; }), "source surface not Delaunay triangulated");
    ASSERT(other.edges() | rx::all_of([&](const auto e) { return other.delaunay(e) != DELAUNAY::NON_DELAUNAY; }), "target surface not Delaunay triangulated");
  }

  // We pick a fixed half edge of this surfaces and try to map it to every
  // other half edge in the other surface. Taking into account another half
  // edge in the same face, we get a single possible 2×2 transformation matrix.
  // (Or rather two possible matrices, if we allow reflections.)
  HalfEdge preimage = [&]() {
    for (HalfEdge he : this->halfEdges())
      if (!ignore(he))
        return he;
    throw std::logic_error("cannot detect isomorphism in surface without Delaunay cells");
  }();

  for (auto image : other.halfEdges()) {
    if (ignoreImage(image))
      continue;

    for (int sgn : {1, -1}) {
      const auto nextInCell = [&](auto e) {
        e = -e;
        do {
          e = this->previousAtVertex(e);
        } while (ignore(e));
        return e;
      };

      const auto nextInImageCell = [&](auto e) {
        e = -e;
        if (sgn == 1) {
          do {
            e = other.previousAtVertex(e);
          } while (ignoreImage(e));
        } else {
          do {
            e = other.nextAtVertex(e);
          } while (ignoreImage(e));
        }
        return e;
      };

      auto v = this->fromHalfEdge(preimage);
      auto w = this->fromHalfEdge(nextInCell(preimage));
      auto v_ = other.fromHalfEdge(image);
      auto w_ = other.fromHalfEdge(nextInImageCell(image));

      // To determine the matrix 2×2 matrix (a b c d) that sends v to v_ and w to w_ note that:
      // ┌ v.x v.y   0   0 ┐ ┌ a ┐   ┌ v_.x ┐
      // | w.x w.y   0   0 | | b |   | w_.x |
      // |   0   0 v.x v.y | | c | = | v_.y |
      // └   0   0 w.x w.y ┘ └ d ┘   └ w_.y ┘
      // Hence, we can determine (a b) and (c d) by solving a 2×2 system for each.
      const T denominator = v.x() * w.y() - v.y() * w.x();
      T a = v_.x() * w.y() - v.y() * w_.x();
      T b = v.x() * w_.x() - v_.x() * w.x();
      T c = v_.y() * w.y() - v.y() * w_.y();
      T d = v.x() * w_.y() - v_.y() * w.x();

      if constexpr (boost::is_detected_v<truediv_t, T>) {
        a /= denominator;
        b /= denominator;
        c /= denominator;
        d /= denominator;
      } else {
        auto maybe = a.truediv(denominator);
        if (!maybe) continue;
        a = *maybe;

        maybe = b.truediv(denominator);
        if (!maybe) continue;
        b = *maybe;

        maybe = c.truediv(denominator);
        if (!maybe) continue;
        c = *maybe;

        maybe = d.truediv(denominator);
        if (!maybe) continue;
        d = *maybe;
      }

      if (!filterMatrix(a, b, c, d))
        continue;

      // The isomorphism of half edges can now be determined by starting with
      // the half edges of this face/cell and depth-first searching through all
      // the adjacent faces/cells until we find a contradiction.
      auto isomorphism = HalfEdgeMap<HalfEdge>(*this);

      const std::function<bool(HalfEdge, HalfEdge)> match = [&](const HalfEdge from, const HalfEdge to) {
        if (!filterHalfEdgeMap(from, to))
          return false;

        if (isomorphism[from] != HalfEdge())
          return isomorphism[from] == to;

        isomorphism[from] = to;

        if (this->fromHalfEdge(from).x() * a + this->fromHalfEdge(from).y() * b != other.fromHalfEdge(to).x())
          return false;

        if (this->fromHalfEdge(from).x() * c + this->fromHalfEdge(from).y() * d != other.fromHalfEdge(to).y())
          return false;

        // We found that this half edge is mapped in a consistent way. Now
        // check its negative.
        if (!match(-from, -to))
          return false;

        // And check that every other edge in its face/cell maps consistently
        if (!match(nextInCell(from), nextInImageCell(to)))
          return false;

        return true;
      };

      if (match(preimage, image))
        return TransformationDeformation<FlatTriangulation>::make(other.clone(), std::move(isomorphism));
    }
  }

  return std::nullopt;
}

template <typename T>
ImplementationOf<FlatTriangulation<T>>::ImplementationOf(FlatTriangulationCombinatorial &&combinatorial, const std::function<Vector<T>(HalfEdge)> &vectors) :
  ImplementationOf<FlatTriangulationCombinatorial>(
      ImplementationOf<FlatTriangulationCombinatorial>::self(combinatorial)->vertices,
      combinatorial.halfEdges() | rx::filter([&](HalfEdge he) { return combinatorial.boundary(he); }) | rx::to_vector()),
  vectors([&]() {
    // We keep track of the vectors attached to the half edges in a Tracked<>
    // object. To construct such an object, we need the surface it is tracking.
    // However, that surface is the surface we are in the process of building.
    // In particular it is not referenced by shared pointer yet. Therefore, we
    // now build a combinatorial triangulation from the data we are building
    // and wrap it in a shared pointer that does *not* free its memory when it
    // goes out of scope.
    auto self = from_this(std::shared_ptr<ImplementationOf>(this, [](auto *) {}));
    auto ret = Tracked<OddHalfEdgeMap<Vector<T>>>(
        self,
        OddHalfEdgeMap<Vector<T>>(self, vectors),
        ImplementationOf::updateAfterFlip);
    // The shared pointer we used to build the Tracked is not going to remain
    // valid so we assert that noone else is holding on to it because it won't
    // work for other use cases than Tracked<>.
    ASSERT(self.self.state.use_count() == 1, "Something is holding to an short lived shared pointer to a surface. This shared pointer is not actually valid and should not be used outside of Tracked<>.");
    return ret;
  }()),
  approximations([&]() {
    // We keep track of the vectors attached to the half edges in a Tracked<>
    // object. To construct such an object, we need the surface it is tracking.
    // However, that surface is the surface we are in the process of building.
    // In particular it is not referenced by shared pointer yet. Therefore, we
    // now build a combinatorial triangulation from the data we are building
    // and wrap it in a shared pointer that does *not* free its memory when it
    // goes out of scope.
    auto self = from_this(std::shared_ptr<ImplementationOf>(this, [](auto *) {}));
    auto ret = Tracked<OddHalfEdgeMap<Vector<exactreal::Arb>>>(
        self,
        OddHalfEdgeMap<Vector<exactreal::Arb>>(
            self,
            [&](const HalfEdge e) {
              return static_cast<flatsurf::Vector<exactreal::Arb>>(this->vectors->get(e));
            }),
        ImplementationOf::updateApproximationAfterFlip);
    // The shared pointer we used to build the Tracked is not going to remain
    // valid so we assert that noone else is holding on to it because it won't
    // work for other use cases than Tracked<>.
    ASSERT(self.self.state.use_count() == 1, "Something is holding to an short lived shared pointer to a surface. This shared pointer is not actually valid and should not be used outside of Tracked<>.");
    return ret;
  }()) {}

template <typename T>
void ImplementationOf<FlatTriangulation<T>>::updateAfterFlip(OddHalfEdgeMap<Vector<T>> &vectors, const FlatTriangulationCombinatorial &parent, HalfEdge flip) {
  vectors.set(flip, vectors.get(-parent.nextInFace(flip)) + vectors.get(-parent.previousInFace(flip)));
}

template <typename T>
void ImplementationOf<FlatTriangulation<T>>::updateApproximationAfterFlip(OddHalfEdgeMap<flatsurf::Vector<exactreal::Arb>> &vectors, const FlatTriangulationCombinatorial &combinatorial, HalfEdge flip) {
  const auto &surface = reinterpret_cast<const FlatTriangulation<T> &>(combinatorial);
  vectors.set(flip, static_cast<flatsurf::Vector<exactreal::Arb>>(surface.fromHalfEdge(-surface.nextInFace(flip)) + surface.fromHalfEdge(-surface.previousInFace(flip))));
}

template <typename T>
void ImplementationOf<FlatTriangulation<T>>::flip(HalfEdge e) {
  const auto self = from_this();

  CHECK_ARGUMENT(self.convex(e, true), "cannot flip this edge as a resulting face would not be strictly convex");

  ImplementationOf<FlatTriangulationCombinatorial>::flip(e);

  check();
}

template <typename T>
void ImplementationOf<FlatTriangulation<T>>::check() {
  const auto self = from_this();

  // check that faces are closed
  for (auto edge : self.halfEdges()) {
    if (self.boundary(edge)) continue;
    auto zero = self.fromHalfEdge(edge);
    CHECK_ARGUMENT(zero, "edges must not be trivial but " << edge << " is zero in " << self);
    edge = self.nextInFace(edge);
    zero += self.fromHalfEdge(edge);
    edge = self.nextInFace(edge);
    zero += self.fromHalfEdge(edge);
    CHECK_ARGUMENT(!zero, "face at " << edge << " is not closed in " << self);
  }
  // check that faces are oriented correctly
  for (auto edge : self.halfEdges()) {
    if (self.boundary(edge)) continue;
    auto next = self.nextInFace(edge);
    CHECK_ARGUMENT(self.fromHalfEdge(edge).ccw(self.fromHalfEdge(next)) != CCW::COLLINEAR, "face at " << edge << " has vanishing area in " << self);
    CHECK_ARGUMENT(self.fromHalfEdge(edge).ccw(self.fromHalfEdge(next)) == CCW::COUNTERCLOCKWISE, "face at " << edge << " is not oriented correctly in " << self);
  }
}

template <typename T>
std::ostream &operator<<(std::ostream &os, const FlatTriangulation<T> &self) {
  return os << static_cast<const FlatTriangulationCombinatorial &>(self)
            << " with vectors " << self.self->vectors;
}

}  // namespace flatsurf

// Instantiations of templates so implementations are generated for the linker
#include "util/instantiate.ipp"

LIBFLATSURF_INSTANTIATE_MANY_WRAPPED((LIBFLATSURF_INSTANTIATE_WITH_IMPLEMENTATION), FlatTriangulation, LIBFLATSURF_REAL_TYPES)
