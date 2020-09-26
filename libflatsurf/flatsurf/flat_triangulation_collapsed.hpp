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

#ifndef LIBFLATSURF_FLAT_TRIANGULATION_COLLAPSED_HPP
#define LIBFLATSURF_FLAT_TRIANGULATION_COLLAPSED_HPP

#include <boost/operators.hpp>
#include <iosfwd>

#include "flat_triangulation_combinatorics.hpp"
#include "managed_movable.hpp"
#include "serializable.hpp"

namespace flatsurf {

// A flat triangulation of a translation surface whose combinatorial structure
// is obtained by collapsing edges that are parallel to a selected vertical
// direction.
// The geometry of such a triangulation is somewhat subtle. Since some edges
// have been identified, only the horizontal geometry is immediately
// meaningful, i.e., the projection of the vector associated to a half edge
// that is perpendicular to the vertical direction.
template <class T>
class FlatTriangulationCollapsed : public FlatTriangulationCombinatorics<FlatTriangulationCollapsed<T>>,
                                   Serializable<FlatTriangulationCollapsed<T>>,
                                   boost::equality_comparable<FlatTriangulationCollapsed<T>> {
  static_assert(std::is_same_v<T, std::decay_t<T>>, "type must not have modifiers such as const");

 public:
  using Coordinate = T;
  using SaddleConnection = ::flatsurf::SaddleConnection<FlatTriangulation<T>>;

  FlatTriangulationCollapsed() noexcept;
  FlatTriangulationCollapsed(const FlatTriangulation<T> &, const Vector<T> &vertical);

  Vertical<FlatTriangulationCollapsed<T>> vertical() const;

  const FlatTriangulation<T> &uncollapsed() const;

  // Create an independent clone of this triangulation that is built from the
  // same data. There is no copy-constructor since it is too likely that this
  // would not update the associated HalfEdgeMaps in the way that the caller
  // expects.
  FlatTriangulationCollapsed<T> clone() const;

  T area() const;

  // Note that only the horizontal part of this vector is meaningful. Rather,
  // the vertical part of the vectors are only consistent inside their faces.
  const SaddleConnection &fromHalfEdge(HalfEdge) const;

  bool inSector(HalfEdge, const Vector<T> &) const;

  // Return the saddle connections to go from this half edge's source to its negative's target.
  Path<FlatTriangulation<T>> cross(HalfEdge) const;

  // Return the collapsed saddle connections to turn from one half edge clockwise to the other.
  Path<FlatTriangulation<T>> turn(HalfEdge, HalfEdge) const;

  // Return whether two collapsed surfaces are equivalent, i.e., they were
  // constructed from equal surfaces that have been collapsed with respect to a
  // parallel vertical direction.
  bool operator==(const FlatTriangulationCollapsed<T> &) const;

  template <typename S>
  friend std::ostream &operator<<(std::ostream &, const FlatTriangulationCollapsed<S> &);

 private:
  using FlatTriangulationCombinatorics<FlatTriangulationCollapsed<T>>::self;

  friend ImplementationOf<FlatTriangulationCollapsed<T>>;
  friend ImplementationOf<ManagedMovable<FlatTriangulationCollapsed<T>>>;
};

}  // namespace flatsurf

#endif
