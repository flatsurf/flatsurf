/**********************************************************************
 *  This file is part of flatsurf.
 *
 *        Copyright (C) 2022 Julian Rüth
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
#include <stdexcept>
#include <fmt/format.h>

#include "../flatsurf/point.hpp"
#include "../flatsurf/ccw.hpp"
#include "../flatsurf/chain.hpp"
#include "../flatsurf/deformation.hpp"
#include "../flatsurf/edge.hpp"
#include "../flatsurf/fmt.hpp"
#include "../flatsurf/vertex.hpp"
#include "../flatsurf/half_edge.hpp"
#include "../flatsurf/half_edge_set.hpp"
#include "../flatsurf/half_edge_set_iterator.hpp"
#include "../flatsurf/vector.hpp"
#include "../flatsurf/saddle_connection.hpp"
#include "../flatsurf/saddle_connections.hpp"
#include "../flatsurf/saddle_connections_iterator.hpp"
#include "../flatsurf/orientation.hpp"
#include "../flatsurf/path.hpp"
#include "../flatsurf/path_iterator.hpp"
#include "../flatsurf/segment.hpp"
#include "../flatsurf/segment_iterator.hpp"
#include "util/assert.ipp"
#include "util/hash.ipp"
#include "util/streamed.ipp"
#include "impl/point.impl.hpp"
#include "impl/linear_deformation_relation.hpp"

namespace flatsurf {

namespace {
template<typename>
struct size;

template<typename T, size_t N>
struct size<std::array<T, N> > {
    static size_t const value = N;
};

template <typename A, size_t... I>
auto as_tuple(const A& array, std::index_sequence<I...>) {
  return std::make_tuple(array[I]...);
}

template <typename A, size_t N = size<A>::value>
auto as_tuple(const A& array) {
  return as_tuple(array, std::make_index_sequence<N>{});
}

}

template <typename Surface>
Point<Surface>::Point(const Surface& surface, const Vertex& vertex) : Point(surface, *vertex.outgoing().begin(), T(1), T(), T()) {}

template <typename Surface>
Point<Surface>::Point(const Surface& surface, HalfEdge face, const T& a, const T& b, const T& c) : self(spimpl::make_impl<ImplementationOf<Point>>(surface, face, a, b, c)) {
  self->normalize();
}

template <typename Surface>
Point<Surface>::Point(const Surface& surface, HalfEdge face, const Vector<T>& xy) : self(spimpl::make_impl<ImplementationOf<Point>>(surface, face, xy)) {
  self->normalize();
}

template <typename Surface>
Point<Surface>::Point(const Surface& surface, HalfEdge face, const std::array<T, 3>& coordinates) : Point(surface, face, std::move(coordinates[0]), std::move(coordinates[1]), std::move(coordinates[2])) {
}

template <typename Surface>
std::array<typename Surface::Coordinate, 3> Point<Surface>::coordinates(HalfEdge face) const {
  LIBFLATSURF_CHECK_ARGUMENT(in(face), "point has no coordinates in this face");

  // If the point is specified with respect to face, return its coordinates as stored.
  for (const auto side : self->surface->face(face))
    if (side == self->face)
      return self->rotated(face);

  // If this is a vertex or on an edge of the face, translate the coordinates
  // into ``face``.
  for (const auto side : self->surface->face(face)) {
    if (self->face == -side and !self->c)
      return ImplementationOf<Point>{*self->surface, side, self->b, self->a, self->c}.rotated(face);
    if (self->surface->previousInFace(self->face) == -side and !self->b and !self->c)
      return ImplementationOf<Point>{*self->surface, side, self->a, self->b, self->c}.rotated(face);
  }

  const auto vertex = this->vertex();

  LIBFLATSURF_ASSERT(vertex, "point is in this face but not in the interior or on an edge so it must be a vertex");

  for (const auto side : self->surface->face(face))
    if (vertex == Vertex::source(side, *self->surface))
      return ImplementationOf<Point>{*self->surface, side, T(1), T(), T()}.rotated(face);

  LIBFLATSURF_UNREACHABLE("point is in this face so it must be either inside the face or on its boundary");
}

template <typename Surface>
const Surface& Point<Surface>::surface() const {
  return *self->surface;
}

template <typename Surface>
Point<Surface>& Point<Surface>::operator+=(const Vector<T>& delta) {
  if (!delta)
    return *this;

  LIBFLATSURF_CHECK_ARGUMENT(self->surface->angle(*this) == 1, "can only translate a marked point");

  ImplementationOf<Point>::translate(*this, delta);
  return *this;
}

template <typename Surface>
Point<Surface>& Point<Surface>::operator-=(const Vector<T>& delta) {
  return *this += -delta;
}

template <typename Surface>
bool Point<Surface>::operator==(const Point& rhs) const {
  if (!rhs.in(self->face))
    return false;

  if (vertex())
    return vertex() == rhs.vertex();

  const auto [a_, b_, c_] = rhs.coordinates(self->face);

  const T λ = self->a + self->b + self->c;
  const T μ_ = a_ + b_ + c_;

  return λ * a_ == μ_ * self->a && λ * b_ == μ_ * self->b && λ * c_ == μ_ * self->c;
}

template <typename Surface>
HalfEdge Point<Surface>::face() const {
  return self->face;
}

template <typename Surface>
std::optional<Vertex> Point<Surface>::vertex() const {
  if (self->b || self->c)
    // normalize() makes sure that vertices are of the form {1, 0, 0}.
    return std::nullopt;

  return Vertex::source(self->face, *self->surface);
}

template <typename Surface>
std::optional<Edge> Point<Surface>::edge() const {
  // normalize() makes sure that edge points are of the form {a, b, 0}.
  if (self->c)
    return std::nullopt;

  return self->face;
}

template <typename Surface>
bool Point<Surface>::in(HalfEdge face) const {
  for (const auto& side : self->surface->face(face))
    if (self->face == side)
      return true;

  for (const auto& side : self->surface->face(face))
    if (on(side))
      return true;

  return false;
}

template <typename Surface>
bool Point<Surface>::on(Edge edge) const {
  const auto vertex = this->vertex();
  if (vertex)
    return *vertex == Vertex::source(edge.positive(), *self->surface) || *vertex == Vertex::source(edge.negative(), *self->surface);

  return this->edge() == edge;
}

template <typename Surface>
bool Point<Surface>::at(const Vertex& vertex) const {
  return this->vertex() == vertex;
}

template <typename Surface>
Vector<typename Surface::Coordinate> Point<Surface>::vector(HalfEdge origin) const {
  auto [a, b, c] = this->coordinates(origin);
  return ImplementationOf<Point>{*self->surface, origin, a, b, c}.cartesian();
}

template <typename Surface>
ImplementationOf<Point<Surface>>::ImplementationOf(const Surface& surface, HalfEdge face, T a, T b, T c) : surface(surface), face(face), a(std::move(a)), b(std::move(b)), c(std::move(c)) {
  LIBFLATSURF_ASSERT_ARGUMENT(face.edge().index() < surface.size(), "no such face in surface")
}

template <typename Surface>
ImplementationOf<Point<Surface>>::ImplementationOf(const Surface& surface, HalfEdge face, const Vector<T>& xy) : ImplementationOf(surface, face, T(1), T(), T()) {
  this->translate(xy);
}

template <typename Surface>
void ImplementationOf<Point<Surface>>::normalize() {
  if (a + b + c == 0)
    throw std::invalid_argument("cannot create point from barycentric coordinates that sum to zero");

  // Normalize if outside a face.
  {
    if (a < 0 || b < 0 || c < 0)
      // This is not hard to do actually but it's hard to make this roundtrip with coordinates().
      throw std::logic_error("not implemented: cannot normalize point outside of a face");
  }

  // Normalize strange inputs.
  {
    if (a + b + c < 0) {
      a *= -1;
      b *= -1;
      c *= -1;
    }
  }

  // Normalize inside a face.
  {
    if (a > 0 && b > 0 && c > 0)
      return;
  }

  // Normalize if on an edge or vertex.
  {
    while (c || !a) {
      const auto face = surface->nextInFace(this->face);
      std::tie(a, b, c) = as_tuple(rotated(face));
      this->face = face;
    }
  }

}

template <typename Surface>
std::array<typename Surface::Coordinate, 3> ImplementationOf<Point<Surface>>::rotated(HalfEdge face) const {
  if (face == this->face)
    return {a, b, c};

  if (face == surface->nextInFace(this->face))
    return {b, c, a};

  LIBFLATSURF_ASSERT_ARGUMENT(face == surface->previousInFace(this->face), "half edge is not delimiting this face");

  return {c, a, b};
}

template <typename Surface>
void ImplementationOf<Point<Surface>>::rotate(HalfEdge face) {
  if (face == surface->nextInFace(this->face)) {
    // Shift (a, b, c) to (b, c, a) i.e., apply (a c b)=(b c)(a b)
    std::swap(a, b);
    std::swap(b, c);
  } else if (face == surface->previousInFace(this->face)) {
    // Shift (a, b, c) to (c, a, b) i.e., apply (a b c)=(a b)(b c)
    std::swap(b, c);
    std::swap(a, b);
  } else
    LIBFLATSURF_ASSERT(face == this->face, "cannot rotate to a side not delimiting this face");

  this->face = face;
}

template <typename Surface>
std::array<typename Surface::Coordinate, 3> ImplementationOf<Point<Surface>>::crossed() const {
  // Consider the face (A, B, C) where our ``face`` goes from A to B.
  // Let (B, A, D) be the face on the other side of ``face``.

  // Let us first write C with coordinates in (B, A, D).
  // If we pretend that D is the origin, we can write C = a_c A + b_c B + d_c D with an arbitrary d_c.
  // For any λ if we set d_c such that λ a_c + λ b_c + d_c = λ, this gives
  // barycentric coordinates for C. (Classically, one would pick λ=1 but
  // since we want to avoid divisions, we are going to collect the divisors
  // into the λ.)
  // To determine a_c and b_c we solve the system:
  // ⎡ B_x A_x ⎤ ⎛ b_c ⎞   ⎛ C_x ⎞
  // ⎢         ⎥ ⎜     ⎟ = ⎜     ⎟
  // ⎣ B_y A_y ⎦ ⎝ a_c ⎠   ⎝ C_y ⎠
  const auto A = -surface->fromHalfEdge(surface->nextInFace(-face));
  const auto B = surface->fromHalfEdge(surface->previousInFace(-face));
  const auto C = B + surface->fromHalfEdge(surface->nextInFace(face));
  const T det = B.x() * A.y() - B.y() * A.x();

  const T λb_c =  A.y() * C.x() - A.x() * C.y();
  const T λa_c = -B.y() * C.x() + B.x() * C.y();
  const T d_c = det - λa_c - λb_c;

  // Now we can rewrite this point in terms of the other face:
  // P = (aA + bB + cC) / (a + b + c)
  //   = (aA + bB + c(λ a_c A + λ b_c B + d_c D) / λ) / (a + b + c)
  // So in (denormalized) barycentric coordinates with respect to (B, A, D) we get:
  // P = (λ b + c λ b_c, λ a + c λ a_c, c d_c)
  return {
    det * b + c * λb_c,
    det * a + c * λa_c,
    c * d_c,
  };
}

template <typename Surface>
Vector<typename Surface::Coordinate> ImplementationOf<Point<Surface>>::cartesian() const {
  const auto AC = -surface->fromHalfEdge(surface->previousInFace(face));
  const auto BC = surface->fromHalfEdge(face);

  const T abc = a + b + c;

  T β = this->b;
  T γ = this->c;

  LIBFLATSURF_ASSERT(abc, "invalid point with barycentric coordinates [" << a << ", " << b << ", " << c << "]");

  const auto xy = γ * AC + β * BC;

  T x = xy.x();
  T y = xy.y();

  if (!LinearDeformationRelation<Surface>::truediv(x, abc))
    throw std::invalid_argument(fmt::format("Cartesian coordinates of point are not in the base ring. Cannot divide {} by {}", streamed(x), streamed(abc)));

  if (!LinearDeformationRelation<Surface>::truediv(y, abc))
    throw std::invalid_argument(fmt::format("Cartesian coordinates of point are not in the base ring. Cannot divide {} by {}", streamed(y), streamed(abc)));

  return Vector{x, y};
}

template <typename Surface>
HalfEdge ImplementationOf<Point<Surface>>::translate(const Vector<T>& Δ) {
  if (c)
    LIBFLATSURF_ASSERT(a && b, "barycentric coordinates of point not normalized");
  if (b)
    LIBFLATSURF_ASSERT(a, "barycentric coordinates of point not normalized");

  // Things are easy if the point remains inside the same face and Δ does not
  // cross any half edges. We first try to see if that's the case and solve for
  // this case.
  {
    // Represent the point with respect to a face so that Δ points into the face.
    if (c) {
      // Point is inside a face, all half edges surrounding that face are
      // equally well suited.
      ;
    } else if (b) {
      // Point is on the interior of an edge, chose either half edge to make
      // Δ point into the face. The resulting point is still normalized.
      if (surface->fromHalfEdge(face).ccw(Δ) == CCW::CLOCKWISE) {
        std::swap(a, b);
        face = -face;
      }
    } else {
      // Point is at a vertex. If the given face is not good, we refuse to make
      // a choice here (we could if the point is marked.)
      LIBFLATSURF_CHECK_ARGUMENT(surface->fromHalfEdge(this->face).ccw(Δ) != CCW::CLOCKWISE && surface->fromHalfEdge(surface->nextAtVertex(this->face)).ccw(Δ) != CCW::COUNTERCLOCKWISE, "Point must be represented with respect to a face that contains the translation vector Δ");
    }

    const auto xy0 = cartesian();

    const auto xy = xy0 + Δ;

    const auto [a, b, c] = barycentric(*surface, face, xy);

    if (a >= 0 && b >= 0 && c >= 0) {
      this->a = a;
      this->b = b;
      this->c = c;
      normalize();
      return face;
    }
  }

  // The point is not inside this face anymore so we need to cross some half edges.
  // Things are getting complicated now. In particular because we do not want
  // to perform divisions which are needed to compute the intersection of a ray
  // and a triangle.

  // We first reduce to the case that our point is actually a vertex of the triangulation:
  if (c || b) {
    // This point is in the interior of a face or in the interior of an edge.
    // We insert a vertex for the point, solve in the modified surface and then
    // pull the result back.
    const auto p = Point{*surface, face, a ,b, c};
    const auto insertion = surface->insert(p);
    auto shifted = insertion(p);
    const auto target = translate(shifted, Δ);
    *this = *insertion.section()(shifted).self;

    if (c)
      // The shifted point is in the interior of a face, any half edge adjacent
      // to that face will do to describe -Δ from here.
      return face;

    if (b)
      // The shifted point is on an edge. We might have to flip the defining
      // edge to make sense of -Δ here.
      return surface->fromHalfEdge(face).ccw(-Δ) == CCW::CLOCKWISE ? -face : face;

    // The shifted point is at a vertex. We only know from where Δ hit that
    // shifted point on a surface with an additional marked point so we need to
    // reconstruct from where Δ comes on the surface without that point.
    const auto preimage = insertion.section()(SaddleConnection(insertion.domain(), target).segment());

    LIBFLATSURF_ASSERT(preimage, "preimage of a half edge after insertion of a marked point must exist; it's either a saddle connection or a segment");
    LIBFLATSURF_ASSERT(preimage->size() == 1, "preimage of a half edge after insertion of a marked point must be a single segment or a single saddle connection");

    SegmentIterator<Surface> segment = preimage->begin();
    return segment->source();
  }

  // We can now assume that this point is a vertex of the triangulation. We use
  // the fast search for saddle connections to figure out in which face our
  // point ends up eventually.
  LIBFLATSURF_ASSERT(surface->inSector(face, Δ), "can only move vertex point in direction contained in selected face");

  const auto connections = surface->connections().sector(face);

  auto it = connections.begin();

  while (true) {
    const auto intersection = it.incrementWithIntersections();
    if (intersection) {
      // The search is crossing an edge. We determine whether our point is in
      // the face on the other side of the edge.
      const auto [chain, cross] = *intersection;

      const auto face = surface->nextInFace(-cross);
      const auto [a, b, c] = barycentric(*surface, face, Δ - chain);

      if (a >= 0 && b >= 0 && c >= 0) {
        this->a = a;
        this->b = b;
        this->c = c;
        this->face = face;
        normalize();
        return face;
      }
    } else {
      // We found a saddle connection in the search. It cannot be the final
      // point since we would have detected this when crossing into this face.
      // So, we only prune one branch of the search tree.
      const auto ccw = Δ.ccw(*it);

      if (ccw == CCW::COLLINEAR) {
        LIBFLATSURF_ASSERT(Δ != *it, "Found saddle connection after shifting point by Δ=" << Δ << " however, that vertex should have been detected when crossing the previous half edge");
        LIBFLATSURF_ASSERT(Δ.orientation(Δ - *it) == ORIENTATION::SAME, "Found saddle connection beyond Δ=" << Δ << " however, we should have found that Δ exists as an interior point in that face when crossing the previous half edge");

        const auto marked = Point(*surface, Vertex::source(it->target(), *surface));

        LIBFLATSURF_CHECK_ARGUMENT(surface->angle(marked) == 1, "Cannot translate point over a vertex with angle >2π");

        // Shift the point across this marked point.
        *this = *marked.self;
        const auto δ = Δ - *it;
        while(!surface->inSector(this->face, δ))
          this->face = surface->nextAtVertex(this->face);
        return translate(δ);
      }
      
      it.skipSector(Δ.ccw(*it)); 
    }
  }
}

template <typename Surface>
HalfEdge ImplementationOf<Point<Surface>>::translate(Point& p, const Vector<T>& Δ) {
  // Ensure that a delta move can be performed in the currently used
  // representation of this point.
  if (p.vertex()) {
    LIBFLATSURF_ASSERT(!p.self->b && !p.self->c, "incorrect normalization for barycentric coordinates of point at vertex");
    while(!p.self->surface->inSector(p.self->face, Δ))
      p.self->face = p.self->surface->nextAtVertex(p.self->face);
  } else if (p.edge()) {
    LIBFLATSURF_ASSERT(!p.self->c, "incorrect normalization for barycentric coordinates of point on edge");
    if (!p.self->surface->inSector(p.self->face, Δ)) {
      p.self->face = -p.self->face;
      std::swap(p.self->a, p.self->b);
    }
  }

  return p.self->translate(Δ);
}

template <typename Surface>
HalfEdge ImplementationOf<Point<Surface>>::translate(Point& p, HalfEdge face, const Vector<T>& Δ) {
  LIBFLATSURF_CHECK_ARGUMENT(p.in(face), "can only translate point with respect to a half edge defining its face");
  LIBFLATSURF_CHECK_ARGUMENT(p.surface().inSector(face, Δ), "translation must be in given sector but " << Δ << " is not in " << face);
  LIBFLATSURF_CHECK_ARGUMENT(p.vertex(), "translation of a point with a specific face only necessary for vertices but trying to translate " << p << " by " << Δ << " with respect to face " << face);
  LIBFLATSURF_CHECK_ARGUMENT(p.vertex() == Vertex::source(face, p.surface()), "cannot translate point with respect to half edge not adjacent to it");

  // Since this is a vertex, its barycentric coordinates are (1, 0, 0). We can
  // safely shift this to any other half edge starting at this vertex.
  p.self->face = face;

  const auto target = p.self->translate(Δ);
  p.self->normalize();

  return target;
}

template <typename Surface>
std::array<typename Surface::Coordinate, 3> ImplementationOf<Point<Surface>>::barycentric(const Surface& surface, HalfEdge face, const Vector<T>& xy) {
  const auto A = Vertex::source(face, surface);
  const auto AB = surface.fromHalfEdge(face);
  const auto AC = -surface.fromHalfEdge(surface.previousInFace(face));

  // Solve the linear system
  // ⎡ xAB xAC ⎤ ⎛ b ⎞   ⎛ x ⎞
  // ⎢         ⎥ ⎜   ⎟ = ⎜   ⎟
  // ⎣ yAB yAC ⎦ ⎝ c ⎠   ⎝ y ⎠
  // to write (x + x0, y + y0) barycentric in terms of (A, B, C).

  const T det = AB.x() * AC.y() - AC.x() * AB.y();
  LIBFLATSURF_ASSERT(det, "triangles describing the faces must not be degenerate");

  const T b = AC.y() * xy.x() - AC.x() * xy.y(); // divided by det
  const T c = - AB.y() * xy.x() + AB.x() * xy.y(); // divided by det
  const T a = det - b - c; // divided by det

  return {a, b, c};
}

template <typename Surface>
std::ostream& operator<<(std::ostream& os, const Point<Surface>& point) {
  return os << "(" << point.self->a << ", " << point.self->b << ", " << point.self->c << ") in " << "(" << point.self->face << ", " << point.self->surface->nextInFace(point.self->face) << ", " << point.self->surface->previousInFace(point.self->face) << ")";
}

}

template <typename Surface>
size_t std::hash<::flatsurf::Point<Surface>>::operator()(const ::flatsurf::Point<Surface>& point) const {
  using T = typename Surface::Coordinate;

  const auto vertex = point.vertex();
  if (vertex)
    return std::hash<::flatsurf::Vertex>{}(*vertex);

  const auto face = point.face();
  const auto [a, b, c] = point.coordinates(face);

  const auto edge = point.edge();
  const auto max = std::max({a, b, c});

  // Computes a hash value for the quotient a/b.
  const auto quot = [](const auto& a, const auto& b) -> size_t {
    if constexpr (std::is_same_v<T, mpq_class>) {
      mpq_class c = a / b;
      return std::hash<double>{}(c.get_d());
    } else if constexpr (std::is_same_v<T, mpz_class>) {
      mpq_class c = mpq_class{a} / mpq_class{b};
      return std::hash<double>{}(c.get_d());
    } else {
      LIBFLATSURF_ASSERT_ARGUMENT(a <= b && a >= 0 && b > 0, "hashing only implemented for quotients a / b with non-negative a < b.");

      // Compute the binary expansion of the quotient a/b.
      // This should probably be implemented by exact-real instead.
      size_t hash = 0;

      auto x = a;
      auto y = b;

      for (size_t i = 0; i < sizeof(size_t); i++) {
        if (x >= y) {
          x -= y;
          LIBFLATSURF_ASSERT(x < y, "in the binary expansion, each digit must be 0 or 1");
          hash++;
        }
        x *= 2;
        hash <<= 1;
      }

      return hash;
    }
  };

  if (edge) {
    // We need to hash a point on an edge such that we do not distinguish its
    // representative (a, b, 0) on face and its representative (b, a, 0) on
    // -face.
    LIBFLATSURF_ASSERT(!c, "point on an edge has coordinates (a, b, 0)");
    return ::flatsurf::hash_combine(::flatsurf::Edge(face), a == max ? quot(b, a) : quot(a, b));
  }


  // We need to hash the coordinates (a, b, c) such that hash(a, b, c) =
  // hash(λa, λb, λc) for a positive λ. we also need the hash to be invariant
  // under rotation of the coordinates (a, b, c).
  const auto boundary = point.surface().face(face);

  if (a == max && b == max && c == max) {
    const ::flatsurf::HalfEdge f = *std::min_element(boundary.begin(), boundary.end(), [](const auto& e, const auto& f) { return e.index() < f.index(); });
    return ::flatsurf::hash_combine(f, point.surface().nextInFace(f));
  } else if (a == max && b == max)
    return ::flatsurf::hash_combine(face, quot(c, max));
  else if (a == max && c == max)
    return ::flatsurf::hash_combine(boundary[2], quot(b, max));
  else if (b == max && c == max)
    return ::flatsurf::hash_combine(boundary[1], quot(a, max));
  else if (a == max)
    return ::flatsurf::hash_combine(face, quot(b, max), quot(c, max));
  else if (b == max)
    return ::flatsurf::hash_combine(boundary[1], quot(c, max), quot(a, max));
  else if (c == max)
    return ::flatsurf::hash_combine(boundary[2], quot(a, max), quot(b, max));

  LIBFLATSURF_UNREACHABLE("Impossible coordinates of point in face: " << point);
}

// Instantiations of templates so implementations are generated for the linker
#include "util/instantiate.ipp"

LIBFLATSURF_INSTANTIATE_MANY_WRAPPED((LIBFLATSURF_INSTANTIATE_WITH_IMPLEMENTATION)(LIBFLATSURF_INSTANTIATE_HASH), Point, LIBFLATSURF_FLAT_TRIANGULATION_TYPES)
