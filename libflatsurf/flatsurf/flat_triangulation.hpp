/**********************************************************************
 *  This file is part of flatsurf.
 *
 *        Copyright (C) 2019-2022 Julian Rüth
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

#ifndef LIBFLATSURF_FLAT_TRIANGULATION_HPP
#define LIBFLATSURF_FLAT_TRIANGULATION_HPP

#include <gmpxx.h>

#include <boost/operators.hpp>
#include <exact-real/forward.hpp>
#include <functional>
#include <iosfwd>
#include <optional>
#include <vector>

#include "flat_triangulation_combinatorial.hpp"
#include "half_edge.hpp"
#include "managed_movable.hpp"
#include "serializable.hpp"

namespace flatsurf {

// A triangulated translation surface. For most purposes this is the central
// object of the flatsurf library.
template <class T>
class FlatTriangulation : public FlatTriangulationCombinatorics<FlatTriangulation<T>>,
                          Serializable<FlatTriangulation<T>>,
                          boost::equality_comparable<FlatTriangulation<T>> {
  static_assert(std::is_same_v<T, std::decay_t<T>>, "type parameter must not have modifiers such as const");

 public:
  using Coordinate = T;

  FlatTriangulation() noexcept;
  FlatTriangulation(FlatTriangulationCombinatorial &&, const std::vector<Vector<T>> &vectors);
  FlatTriangulation(FlatTriangulationCombinatorial &&, const std::function<Vector<T>(HalfEdge)> &vectors);

  // Create an independent clone of this triangulation that is built from the
  // same data. There is no copy-constructor since it is too likely that this
  // would not update the associated HalfEdgeMaps in the way that the caller
  // expects.
  FlatTriangulation<T> clone() const;

  // Flip edges in this triangulation so that it satisfies the usual
  // l²-Delaunay condition, see delaunay(Edge).
  void delaunay();

  // Return whether this edge satisfies the usual l²-Delaunay condition, i.e.,
  // when embedding just the faces attached to this edge in R², whether the
  // circumcircle of any of its faces does not contain any of the vertices of
  // the faces in its interior. (Note that this condition is symmetric, so it
  // holds for one of the circumcircles iff it holds for all of them.)
  DELAUNAY delaunay(Edge) const;

  // Create an independent clone of this triangulation with an added vertex
  // next to e at v from e's source. If the vector does not fit into the face
  // next to e, the necessary edge flips are performed to accomodate it.
  Deformation<FlatTriangulation<T>> insertAt(HalfEdge &e, const Vector<T> &v) const;

  // Create an independent clone of this triangulation with an added vertex at
  // ``p`` which is not a vertex already.
  Deformation<FlatTriangulation<T>> insert(const Point<FlatTriangulation>& p) const;

  // Create an independent clone of this triangulation with relabeled edges.
  Deformation<FlatTriangulation<T>> relabel(const Permutation<HalfEdge>&) const;

  // Create an independent clone of this triangulation with all vectors scaled
  // by c.
  [[deprecated("Use applyMatrix instead")]]
  FlatTriangulation<T> scale(const mpz_class &c) const;

  // Create an independent clone of this triangulation with all vectors
  // transformed by this 2×2.
  Deformation<FlatTriangulation<T>> applyMatrix(const T& a, const T& b, const T& c, const T& d) const;

  // Create an independent clone of this triangulation with an edded boundary
  // at the half edge e by removing the identification of the two corresponding
  // half edges there.
  Deformation<FlatTriangulation<T>> slit(const HalfEdge e) const;

  // Return the saddle connections of this translations surface.
  SaddleConnections<FlatTriangulation<T>> connections() const;

  // Create an independent clone of this triangulation with the vector
  // v(h) associated to a half edge replaced by v(h) + shift[h].
  Deformation<FlatTriangulation<T>> operator+(const OddHalfEdgeMap<Vector<T>> &shift) const;

  // Return a simplified flat triangulation with marked points, i.e., verticas
  // with a total angle of 2π, eliminated.
  Deformation<FlatTriangulation<T>> eliminateMarkedPoints() const;

  // Return an isomorphism from this surface to the given surface, i.e., a
  // bijection on some (depending on the selected "kind") of the half edges
  // transforming them subject to the same linear transformation (note that
  // that transformation might have negative determinant, i.e., the order of
  // half edges in a face might change under this map.)
  [[deprecated("Use Equivalence::isomorphisms() instead")]]
  std::optional<Deformation<FlatTriangulation<T>>> isomorphism(
      const FlatTriangulation &,
      ISOMORPHISM kind,
      std::function<bool(const T &, const T &, const T &, const T &)> = [](const T &a, const T &b, const T &c, const T &d) { return a == 1 && b == 0 && c == 0 && d == 1; },
      std::function<bool(HalfEdge, HalfEdge)> = [](HalfEdge, HalfEdge) { return true; }) const;

  // Return the vector corresponding to a shortest edge in this surface.
  Vector<T> shortest() const;

  // Return the shortest vector relative to this direction which is not orthogonal to it.
  Vector<T> shortest(const Vector<T> &) const;

  // Return whether the two triangles attached to this half edge form a (strictly) convex quadrilateral.
  bool convex(HalfEdge, bool strict = false) const;

  // Return twice the area of this triangulation.
  T area() const;

  // Return the total angle at this vertex as a multiple of 2π.
  int angle(const Vertex &) const;

  // Return the total angle at this point as a multiple of 2π.
  int angle(const Point<FlatTriangulation>&) const;

  // Return whether this direction is in the sector counterclockwise next to
  // the half edge (including the half edge but not including the following
  // half edge.)
  bool inSector(HalfEdge, const Vector<T> &) const;

  // Return whether this direction is in the half plane defined by the half
  // edge, i.e., the angle with the half edge is strictly less than π/2.
  bool inHalfPlane(HalfEdge, const Vector<T> &) const;

  // Return whether this direction is in the plane defined by the half edge,
  // i.e., the direction and the half edge are not opposite.
  bool inPlane(HalfEdge, const Vector<T> &) const;

  // Return the sector containing this direction in the plane defined by the
  // given half edge. I.e., consider a vector in the given direction that
  // starts at the source of the given half edge and has an angle strictly less
  // than π with the provided half edge; return the half edge that describes
  // the sector this direction lives in, i.e., the direction is collinear or
  // counterclockwise with that half edge but clockwise from the following half
  // edge.
  HalfEdge sector(HalfEdge, const Vector<T> &) const;

  // Return the first sector containing the direction found by walking the
  // sectors in direction ``ccw``.
  // More precisely, if the direction coincides with the half edge direction,
  // return the half edge, i.e., the initial sector, unless ``exclude`` is set.
  // Otherwise, if walking ounterclockwise and the initial sector contains the
  // direction, it is returned.
  // If walking clockwise, walk at least one step, i.e., if the initial sector
  // contains the direction in it interior, it is ignored.
  // We cannot walk in "collinear" direction.
  HalfEdge sector(HalfEdge, CCW ccw, const Vector<T>& direction, bool exclude=false) const;

  // Return the first sector containing ``direction`` walking in direction
  // ``ccw`` from ``start`` (in sector ``sector``.)
  // If ``start`` and ``direction`` are equal, return ``sector``, unless
  // ``exclude`` is set.
  HalfEdge sector(HalfEdge sector, const Vector<T>& start, CCW ccw, const Vector<T>& direction, bool exclude=false) const;

  // Return the sector containing this direction in the half plane defined by
  // the vertical which has an angle strictly less than π with the provided
  // half edge.
  HalfEdge sector(HalfEdge, const Vertical<FlatTriangulation>&, const Vector<T>&) const;

  const Vector<T> &fromHalfEdge(HalfEdge) const;

  const ::flatsurf::Vector<exactreal::Arb> &fromHalfEdgeApproximate(HalfEdge) const;

  // Return whether the surfaces are indistinguishable, i.e.,
  // each half edge has a corresponding half edge with the same label whose
  // face is made up of equally labeled half edges with the same vectors.
  bool operator==(const FlatTriangulation<T> &) const;

  template <typename W>
  friend std::ostream &operator<<(std::ostream &, const FlatTriangulation<W> &);

 private:
  using FlatTriangulationCombinatorics<FlatTriangulation<T>>::self;

  friend ImplementationOf<FlatTriangulation<T>>;
  friend ImplementationOf<ManagedMovable<FlatTriangulation<T>>>;
};

template <typename Vector>
FlatTriangulation(const std::vector<std::vector<int>> &, const std::vector<Vector> &) -> FlatTriangulation<typename Vector::Coordinate>;

}  // namespace flatsurf

#endif
