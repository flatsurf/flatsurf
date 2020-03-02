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

#ifndef LIBFLATSURF_FLAT_TRIANGULATION_HPP
#define LIBFLATSURF_FLAT_TRIANGULATION_HPP

#include <boost/operators.hpp>
#include <functional>
#include <iosfwd>
#include <memory>
#include <vector>

#include <gmpxx.h>

#include <exact-real/forward.hpp>

#include "flat_triangulation_combinatorial.hpp"
#include "moveable.hpp"
#include "serializable.hpp"

namespace flatsurf {
template <class T>
class FlatTriangulation : public FlatTriangulationCombinatorial,
                          Serializable<FlatTriangulation<T>>,
                          boost::equality_comparable<FlatTriangulation<T>> {
  static_assert(std::is_same_v<T, std::decay_t<T>>, "type must not have modifiers such as const");

 public:
  using Coordinate = T;

  FlatTriangulation() noexcept;
  FlatTriangulation(FlatTriangulationCombinatorial &&, const std::vector<Vector<T>> &vectors);
  FlatTriangulation(FlatTriangulationCombinatorial &&, const std::function<Vector<T>(HalfEdge)> &vectors);
  FlatTriangulation(FlatTriangulation<T> &&rhs) noexcept;

  // Create an independent clone of this triangulation that is built from the
  // same data. There is no copy-constructor since it is too likely that this
  // is would not update the associated HalfEdgeMaps in the way that the caller
  // expects.
  std::unique_ptr<FlatTriangulation<T>> clone() const;

  // Create an independent clone of this triangulation with an added vertex
  // next to e at v from e's source. If the vector does not fit into the face
  // next to e, the necessary edge flips are performed to accomodate it.
  std::unique_ptr<FlatTriangulation<T>> insertAt(HalfEdge &e, const Vector<T> &v) const;

  // Create an independent clone of this triangulation with all vectors scaled
  // by c.
  std::unique_ptr<FlatTriangulation<T>> scale(const mpz_class &c) const;

  // Create an independent clone of this triangulation with an edded boundary
  // at the half edge e by removing the identification of the two corresponding
  // half edges there.
  std::unique_ptr<FlatTriangulation<T>> slot(const HalfEdge e) const;

  Vector<T> shortest() const;
  // Return the shortest vector relative to this direction which is not orthogonal to it.
  Vector<T> shortest(const Vector<T> &) const;

  void flip(HalfEdge);

  T area() const;

  // Return the total angle at this vertex as a multiple of 2π.
  int angle(const Vertex&) const;

  // Return whether the vector is in the sector counterclockwise next to the
  // half edge (including the half edge but not including the following half
  // edge.)
  bool inSector(HalfEdge, const Vector<T> &) const;
  bool inSector(HalfEdge, const Vertical<FlatTriangulation<T>> &) const;

  const Vector<T> &fromEdge(HalfEdge) const;
  const ::flatsurf::Vector<exactreal::Arb> &fromEdgeApproximate(HalfEdge) const;

  std::shared_ptr<const FlatTriangulation<T>> shared_from_this() const;
  std::shared_ptr<FlatTriangulation<T>> shared_from_this();

  FlatTriangulation<T> &operator=(FlatTriangulation<T> &&) noexcept;

  bool operator==(const FlatTriangulation<T> &) const noexcept;

  template <typename W>
  friend std::ostream &operator<<(std::ostream &, const FlatTriangulation<W> &);

 private:
  using Implementation = ::flatsurf::Implementation<FlatTriangulation>;
  Moveable<Implementation> impl;

  friend Implementation;
};

template <typename Vector>
FlatTriangulation(const std::vector<std::vector<int>> &, const std::vector<Vector> &)->FlatTriangulation<typename Vector::Coordinate>;
}  // namespace flatsurf

#endif
