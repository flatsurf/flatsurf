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

#include <exact-real/arb.hpp>
#include <intervalxt/sample/arithmetic.hpp>
#include <map>
#include <ostream>
#include <vector>

#include <exact-real/yap/arb.hpp>
#include <exact-real/integer_ring.hpp>
#include <exact-real/rational_field.hpp>
#include <exact-real/number_field.hpp>

#include "../flatsurf/bound.hpp"
#include "../flatsurf/ccw.hpp"
#include "../flatsurf/edge.hpp"
#include "../flatsurf/edge_set.hpp"
#include "../flatsurf/flat_triangulation.hpp"
#include "../flatsurf/half_edge.hpp"
#include "../flatsurf/odd_half_edge_map.hpp"
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

// Return an approximation of x with precision prec
// TODO: I should probably move that to a shared place.
template <typename T>
auto arb(const T& x, const long prec) {
  if constexpr (std::is_same_v<T, mpz_class>) {
    (void)(prec);
    return exactreal::Arb(x);
  } else if constexpr (std::is_same_v<T, long long>) {
    (void)(prec);
    return exactreal::Arb(x);
  } else if constexpr (std::is_same_v<T, mpq_class>) {
    return exactreal::Arb(x, prec);
  } else if constexpr (std::is_same_v<T, eantic::renf_elem_class>) {
    return exactreal::Arb(x, prec);
  } else if constexpr (std::is_same_v<T, exactreal::Element<exactreal::IntegerRing>>) {
    return x.arb(prec);
  } else if constexpr (std::is_same_v<T, exactreal::Element<exactreal::RationalField>>) {
    return x.arb(prec);
  } else if constexpr (std::is_same_v<T, exactreal::Element<exactreal::NumberField>>) {
    return x.arb(prec);
  } else {
    // TODO: static assert this must not happen.
    return x;
  }
};

// Return the smallest approximate solution of a*t^2 - b*t + c = 0 for t in [0, 1]
// TODO: Move this to a more generic place.
template <typename T>
std::optional<exactreal::Arb> root(const T& a, const T& b, const T& c, const long prec) {
  const auto validate = [&](const exactreal::Arb& solution) -> std::optional<exactreal::Arb> {
    auto lt0 = solution < 0;
    auto gt1 = solution > 1;

    if (!lt0.has_value() || !gt1.has_value()) return root(a, b, c, 2*prec);
    if (*lt0) return {};
    if (*gt1) return {};

    return solution;
  };

  const exactreal::Arb a_ = arb(a, prec),
        b_ = arb(b, prec),
        c_ = arb(c, prec);

  if (a == 0) {
    if (c == 0)
      return exactreal::Arb();
    CHECK_ARGUMENT(b != 0, "equation c = 0 has no solutions");
    exactreal::Arb solution = (c_ / b_)(prec);
    return validate(solution);
  }

  const T discriminant = b*b - 4*a*c;
  if (discriminant < 0) {
    return {};
  } else if (discriminant == 0) {
    return validate((b_ / (2 * a_))(prec));
  } else {
    exactreal::Arb sqrt_discriminant = (b_*b_ - 4*a_*c_)(prec);
    arb_sqrt(sqrt_discriminant.arb_t(), sqrt_discriminant.arb_t(), prec);

    const auto t0 = validate(((b_ + sqrt_discriminant) / (2*a_))(prec));
    const auto t1 = validate(((b_ - sqrt_discriminant) / (2*a_))(prec));

    if (t0 && t1) {
      const auto lt = *t0 < *t1;
      if (!lt.has_value() || *lt)
        return t0;
      return t1;
    } else if (t0)
      return t0;
    else if (t1)
      return t1;
    else
      return {};
  }
};

template <typename T>
std::unique_ptr<FlatTriangulation<T>> FlatTriangulation<T>::operator+(const OddHalfEdgeMap<Vector<T>> &shift) const {
  // Half edges that collapse at the end of the shift.
  EdgeSet collapse;

  // Records that the half edge e needs to be flipped at a time t in (0, 1]
  // that is given by a solution to a*t^2 - b*t + c = 0.
  // Unfortunately, we can in general only solve this approximately.
  struct RequiredFlip {
    HalfEdge flip;
    T a, b, c;

    exactreal::Arb t(slong prec = exactreal::ARB_PRECISION_FAST) const {
      const auto ret = root(a, b, c, prec);
      ASSERT(ret, "A required flip must have a critical time in (0, 1]");
      return *ret;
    }

    // Return whether this flip needs to happen before the one required by rhs.
    bool operator<(const RequiredFlip& rhs) const {
      // When the two polynomials are just multiples of each other, the critical time is the same.
      if (a * rhs.b == b * rhs.a && a * rhs.c == c * rhs.a && b * rhs.c == c * rhs.b)
        return false;

      // Compute approximate values for the respective critical times t and compare them.
      for(slong prec = exactreal::ARB_PRECISION_FAST;; prec *= 2) {
        const auto t = this->t(prec);
        const auto s = rhs.t(prec);

        const auto lt = t < s;
        if (lt)
          return *lt;

        // We computed balls around the roots t and s of the two quadratic
        // equations but they overlappped so we cannot decide which one is
        // first. (Typically, this happens because they are actually equal.)

        // If the quadratic polynomials have no common root, then we can
        // eventually separate the times s and t.

        // If both polynomials are only linear, then we decide whether they
        // have the same root by comparing coefficients: namely if c/b = c'/b'.
        if (!a && !rhs.a) {
          if (c * rhs.b == rhs.c * b)
            return false;
          continue;
        }

        // In the non-linear case, they have a common root iff their
        // resultant is zero.
        if (c*c*rhs.a*rhs.a - b*c*rhs.a*rhs.b + a*c*rhs.b*rhs.b + b*b*rhs.a*rhs.c - 2*a*c*rhs.a*rhs.c - a*b*rhs.b*rhs.c + a*a*rhs.c*rhs.c != 0)
          continue;

        // When they have a common root, it does not necessarily have to be the
        // root we care about.
        // We already know that not both of their roots can be the same since
        // then the polynomials would be multiples of each other which we
        // checked before. We could now probably compute the gcd of the
        // polynomials and then look at the common linear factor but for all
        // cases that we can imagine it is probably safe to assume that the
        // root that is the same is actually the root that also looks like it's
        // approximately the same, i.e., the two critical times are equal.
        return false;
      }
    }
  };

  std::optional<RequiredFlip> flip;

  for (auto vertex : vertices()) {
    const auto outgoing = atVertex(vertex);

    // The x, y coordinates of the half edge he
    const auto x = [&](const HalfEdge he) { return fromEdge(he).x(); };
    const auto y = [&](const HalfEdge he) { return fromEdge(he).y(); };
    // The x, y shifts of the half edge he at time t = 1
    const auto u = [&](const HalfEdge he) { return shift.get(he).x(); };
    const auto v = [&](const HalfEdge he) { return shift.get(he).y(); };

    // One reason why the area of a triangle is zero for a time t in [0, 1] is
    // that two singularities were shifted into each other. We can make sense
    // of this when it happens at time t=1 by collapsing triangles.
    for (size_t i = 0; i < outgoing.size(); i++) {
      const auto he = outgoing.at(i);

      if (fromEdge(he).ccw(shift.get(he)) == CCW::COLLINEAR) {
        switch (fromEdge(he).orientation(fromEdge(he) + shift.get(he))) {
          case ORIENTATION::SAME:
            // The critical time t is not in [0, 1]
            break;
          case ORIENTATION::OPPOSITE:
            throw std::invalid_argument("shift must not collapse half edges for a time t in (0, 1)");
          case ORIENTATION::ORTHOGONAL:
            collapse.insert(he);
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
      const T a = u(he) * v(he_) - u(he_) * v(he);
      const T b = -u(he) * y(he_) + u(he_) * y(he) - x(he) * v(he_) + x(he_) * v(he);
      const T c = x(he) * y(he_) - x(he_) * y(he);

      // If the determinant has a zero for any t in [0, 1], the area of a
      // triangle vanishes or becomes negative.
      const T det0 = c;
      ASSERT(det0 > 0, "Original surface " << *this << " already had a triangle with non-positive area before applying any shift to it.");

      const T det1 = a - b + c;
      if (det1 > 0) {
        // We handle the easiest case first: the area remains positive for all
        // times t in [0, 1].
        if (
          // The critical point of a*t^2 - b*t + c is outside [0, 1].
          (a == 0 && (c > b && c > 0)) ||
          // The critical point is a maximum
          (a < 0) ||
          // The critical point is positive so there can be no root in [0, 1].
          (a > 0 && (b < 0 || b > 2 * a || b * b - 4 * a * c < 0))) {
          continue;
        }
      }

      // We can now assume that the determinant is zero for some t in (0, 1].
      // We need to flip a half edge of this triangle if it has a vertex on its
      // interior at that critical time t.

      // But first we exclude the case that
      // the vertex ends up on the boundary of the half edge, i.e., a half edge
      // collapses.
      if (collapse.contains(he) || collapse.contains(he_)) {
        continue;
      }

      // Determine whether our vertex moves onto the half edge opposite to it,
      // i.e., the one following e in this triangle.
      const auto vertex_hits_interior = [&]() {
        for (long  prec = exactreal::ARB_PRECISION_FAST;; prec *= 2) {
          const auto t = root(a, b, c, prec);
          ASSERT(t, "determinant " << a << " * t^2 - " << b << " * t + " << c << " must have a root in [0, 1]");
          const auto et = Vector<exactreal::Arb>(
              (arb(fromEdge(he).x(), prec) + *t * arb(shift.get(he).x(), prec))(prec),
              (arb(fromEdge(he).y(), prec) + *t * arb(shift.get(he).y(), prec))(prec));
          const auto e_t = Vector<exactreal::Arb>(
              (arb(fromEdge(he_).x(), prec) + *t * arb(shift.get(he_).x(), prec))(prec),
              (arb(fromEdge(he_).y(), prec) + *t * arb(shift.get(he_).y(), prec))(prec));

          const auto orientation = et.orientation(e_t);

          if (orientation) {
            switch(*orientation) {
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

      const RequiredFlip proposedFlip{nextInFace(he), a, b, c};

      // Record that a half edge needs to be flipped at time t. We'll later
      // actually flip the one that needs to be flipped first and recurse.
      if (!flip || proposedFlip < *flip)
        flip = proposedFlip;
    }
  }

  if (flip) {
    // We want to flip the half edge that we found needs to be flipped first.
    // However, just flipping that edge right now might lead to infinite loops
    // where the same edge gets flipped again and again without making any
    // progress. So instead we get a bit closer to the critical time and
    // perform the flip just then.
    // Note that this also solves the problem that the flip might not actually
    // be possibly as it might lead to a non-convex triangulation since
    // eventually, when we are close enough to the critical time, the flip will
    // be valid.
    // TODO: This leads to quite some coefficient blow-up along the way though
    // which eventually goes away.
    const auto t = flip->t();

    for (auto s = mpq_class(1, 2);; s /= 2) {
      const auto lt = exactreal::Arb(s, exactreal::ARB_PRECISION_FAST) < t;
      if (lt && *lt) {
        const OddHalfEdgeMap<Vector<T>> partial(*this, [&](const HalfEdge he) { return shift.get(he) / s.get_den(); });
        const auto closer = *this + partial;

        const Tracked<OddHalfEdgeMap<Vector<T>>> remaining(&*closer, OddHalfEdgeMap<Vector<T>>(*closer, [&](const HalfEdge he) { return shift.get(he) - partial.get(he); }), Implementation::updateAfterFlip);

        // TODO: This was copied from flip(). Break it out instead.
        if (fromEdge(previousAtVertex(flip->flip)).ccw(fromEdge(nextAtVertex(flip->flip))) == CCW::COUNTERCLOCKWISE &&
          fromEdge(previousAtVertex(-flip->flip)).ccw(fromEdge(nextAtVertex(-flip->flip))) == CCW::COUNTERCLOCKWISE) {
          closer->flip(flip->flip);
        }

        return *closer + remaining;
      }
    }
  } else {
    // Now we perform shifts of half edges on a copy of the surface's vector
    // structure and collapse on the combinatorial structure.
    auto combinatorial = static_cast<const FlatTriangulationCombinatorial &>(*this).clone(); 
    Tracked<OddHalfEdgeMap<Vector<T>>> vectors(&*combinatorial, OddHalfEdgeMap<Vector<T>>(*combinatorial, [&](const HalfEdge he) { return fromEdge(he) + shift.get(he); }),
      Tracked<OddHalfEdgeMap<Vector<T>>>::defaultFlip,
      [](OddHalfEdgeMap<Vector<T>>& vectors, const FlatTriangulationCombinatorial&, Edge e) {
        ASSERT(!vectors.get(e.positive()), "can only collapse half edges that have become trivial");
      });
    Tracked<EdgeSet> collapse_(&*combinatorial, collapse,
      Tracked<EdgeSet>::defaultFlip,
      [](EdgeSet& collapse, const FlatTriangulationCombinatorial&, Edge e) {
        ASSERT(collapse.contains(e), "will only collapse edges that have been found to collapse at t=1");
      });

    while (!collapse_->empty())
      combinatorial->collapse(begin(static_cast<const EdgeSet&>(collapse_))->positive());

    return std::make_unique<FlatTriangulation<T>>(
        std::move(*combinatorial),
        [&](const HalfEdge he) { return vectors->get(he); });
  }
}

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

template <typename T>
Vector<T> FlatTriangulation<T>::shortest(const Vector<T> &direction) const {
  const auto edges = this->edges();
  Edge shortest = *std::min_element(begin(edges), end(edges), [&](const auto &a, const auto &b) {
    const Vector<T> av = fromEdge(a.positive());
    const Vector<T> bv = fromEdge(b.positive());

    const auto alen = av * direction;
    const auto blen = bv * direction;

    if (!alen) return false;
    if (!blen) return true;

    const auto abs = [](const auto &x) { return x < 0 ? -x : x; };

    return abs(alen) < abs(blen);
  });
  return fromEdge(shortest.positive());
}

template <typename T>
const Vector<T> &FlatTriangulation<T>::fromEdge(const HalfEdge e) const {
  return impl->vectors->get(e);
}

template <typename T>
const flatsurf::Vector<exactreal::Arb> &FlatTriangulation<T>::fromEdgeApproximate(HalfEdge e) const {
  return impl->approximations->get(e);
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
  // TODO: Use operator+ here instead: Insert the vertex at a short distance
  // without flips, then shift it to its final position with operator+.
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
    if (this->impl->vectors->get(edge) != rhs.impl->vectors->get(edge))
      return false;
  }
  return true;
}

template <typename T>
int FlatTriangulation<T>::angle(const Vertex &vertex) const {
  int angle = 0;

  const HalfEdge first = *begin(atVertex(vertex));
  HalfEdge current = first;
  do {
    const HalfEdge next = nextAtVertex(current);

    if (fromEdge(current).x() >= 0 && fromEdge(next).x() < 0)
      angle++;

    current = next;
  } while (current != first);

  return angle;
}

template <typename T>
ImplementationOf<FlatTriangulation<T>>::ImplementationOf(const FlatTriangulationCombinatorial &combinatorial, const std::function<Vector<T>(HalfEdge)> &vectors) :
  vectors(&combinatorial, OddHalfEdgeMap(combinatorial, vectors), ImplementationOf::updateAfterFlip),
  approximations(&combinatorial,
      OddHalfEdgeMap<Vector<exactreal::Arb>>(combinatorial, [&](const HalfEdge e) {
        return static_cast<flatsurf::Vector<exactreal::Arb>>(this->vectors->get(e));
      }),
      ImplementationOf::updateApproximationAfterFlip) {
}

template <typename T>
void ImplementationOf<FlatTriangulation<T>>::updateAfterFlip(OddHalfEdgeMap<Vector<T>> &vectors, const FlatTriangulationCombinatorial &parent, HalfEdge flip) {
  vectors.set(flip, vectors.get(-parent.nextInFace(flip)) + vectors.get(-parent.previousInFace(flip)));
}

template <typename T>
void ImplementationOf<FlatTriangulation<T>>::updateApproximationAfterFlip(OddHalfEdgeMap<flatsurf::Vector<exactreal::Arb>> &vectors, const FlatTriangulationCombinatorial &combinatorial, HalfEdge flip) {
  const auto &surface = static_cast<const FlatTriangulation<T> &>(combinatorial);
  vectors.set(flip, static_cast<flatsurf::Vector<exactreal::Arb>>(surface.fromEdge(-surface.nextInFace(flip)) + surface.fromEdge(-surface.previousInFace(flip))));
}

template <typename T>
void ImplementationOf<FlatTriangulation<T>>::check(const FlatTriangulation<T> &self) {
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
ostream &operator<<(ostream &os, const FlatTriangulation<T> &self) {
  return os << static_cast<const FlatTriangulationCombinatorial &>(self)
            << " with vectors " << self.impl->vectors;
}
}  // namespace flatsurf

// Instantiations of templates so implementations are generated for the linker
#include "util/instantiate.ipp"

LIBFLATSURF_INSTANTIATE_MANY_WRAPPED((LIBFLATSURF_INSTANTIATE_WITH_IMPLEMENTATION), FlatTriangulation, LIBFLATSURF_REAL_TYPES)
