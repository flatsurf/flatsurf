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

#ifndef LIBFLATSURF_FLAT_TRIANGULATION_HPP
#define LIBFLATSURF_FLAT_TRIANGULATION_HPP

#include <functional>
#include <iosfwd>
#include <vector>

#include <gmpxx.h>
#include <boost/operators.hpp>
#include <exact-real/forward.hpp>

#include "flat_triangulation_combinatorial.hpp"
#include "managed_movable.hpp"
#include "serializable.hpp"

namespace flatsurf {

// A triangulated translation surface. For most purposes this is the central
// object of the flatsurf library.
template <class T>
class FlatTriangulation : public FlatTriangulationCombinatorics<FlatTriangulation<T>>,
                          Serializable<FlatTriangulation<T>>,
                          boost::equality_comparable<FlatTriangulation<T>> {
  static_assert(std::is_same_v<T, std::decay_t<T>>, "type must not have modifiers such as const");

 public:
  using Coordinate = T;

  FlatTriangulation() noexcept;
  FlatTriangulation(FlatTriangulationCombinatorial&&, const std::vector<Vector<T>> &vectors);
  FlatTriangulation(FlatTriangulationCombinatorial&&, const std::function<Vector<T>(HalfEdge)> &vectors);

  // Create an independent clone of this triangulation that is built from the
  // same data. There is no copy-constructor since it is too likely that this
  // is would not update the associated HalfEdgeMaps in the way that the caller
  // expects.
  FlatTriangulation<T> clone() const;

  // Flip edges in this triangulation so that all faces satisfy the l²-Delaunay condition.
  void delaunay();

  // Return whether this half edge satisfies the l²-Delaunay condition, i.e.,
  // whether the circumcircles of the face attached to this edge does not
  // contain the face attached to the reverse half edge.
  bool delaunay(HalfEdge) const;

  // Create an independent clone of this triangulation with an added vertex
  // next to e at v from e's source. If the vector does not fit into the face
  // next to e, the necessary edge flips are performed to accomodate it.
  FlatTriangulation<T> insertAt(HalfEdge &e, const Vector<T> &v) const;

  // Create an independent clone of this triangulation with all vectors scaled
  // by c.
  FlatTriangulation<T> scale(const mpz_class &c) const;

  // Create an independent clone of this triangulation with an edded boundary
  // at the half edge e by removing the identification of the two corresponding
  // half edges there.
  FlatTriangulation<T> slit(const HalfEdge e) const;

  // Return the saddle connections of this translations surface.
  SaddleConnections<FlatTriangulation<T>> connections() const;

  // Create an independent clone of this triangulation with the vector
  // v(h) associated to a half edge replaced by v(h) + shift[h].
  FlatTriangulation<T> operator+(const OddHalfEdgeMap<Vector<T>> &shift) const;

  // Return a simplified flat triangulation with marked points, i.e., verticas
  // with a total angle of 2π, eliminated.
  FlatTriangulation<T> eliminateMarkedPoints() const;

  Vector<T> shortest() const;

  // Return the shortest vector relative to this direction which is not orthogonal to it.
  Vector<T> shortest(const Vector<T> &) const;

  // Return whether the two triangles attached to this half edge form a (strictly) convex quadrilateral.
  bool convex(HalfEdge, bool strict = false) const;

  // Return twice the area of this triangulation.
  T area() const;

  // Return the total angle at this vertex as a multiple of 2π.
  int angle(const Vertex &) const;

  // Return whether the vector is in the sector counterclockwise next to the
  // half edge (including the half edge but not including the following half
  // edge.)
  bool inSector(HalfEdge, const Vector<T> &) const;
  bool inSector(HalfEdge, const Vertical<FlatTriangulation<T>> &) const;

  const Vector<T> &fromHalfEdge(HalfEdge) const;

  const ::flatsurf::Vector<exactreal::Arb> &fromHalfEdgeApproximate(HalfEdge) const;

  bool operator==(const FlatTriangulation<T> &) const noexcept;

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
