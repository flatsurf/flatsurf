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

#ifndef LIBFLATSURF_FLAT_TRIANGULATION_COLLAPSED_HPP
#define LIBFLATSURF_FLAT_TRIANGULATION_COLLAPSED_HPP

#include <boost/operators.hpp>
#include <iosfwd>
#include <memory>
#include <vector>

#include "flat_triangulation_combinatorial.hpp"
#include "moveable.hpp"
#include "serializable.hpp"

namespace flatsurf {

template <class T>
class FlatTriangulationCollapsed : public FlatTriangulationCombinatorial,
                                   public Serializable<FlatTriangulationCollapsed<T>>,
                                   boost::equality_comparable<FlatTriangulationCollapsed<T>> {
  static_assert(std::is_same_v<T, std::decay_t<T>>, "type must not have modifiers such as const");

  FlatTriangulationCollapsed(std::unique_ptr<FlatTriangulation<T>>, const flatsurf::Vector<T> &vertical);

 public:
  using Coordinate = T;
  // TODO: Remove everything but Coordinate from public interface
  using Vector = flatsurf::Vector<T>;
  using SaddleConnection = ::flatsurf::SaddleConnection<FlatTriangulation<T>>;
  using Collapsed = FlatTriangulationCollapsed;

  static std::shared_ptr<FlatTriangulationCollapsed<T>> make(std::unique_ptr<FlatTriangulation<T>>, const Vector &vertical);

  Vertical<FlatTriangulationCollapsed<T>> vertical() const;

  std::shared_ptr<const FlatTriangulation<T>> uncollapsed() const;

  // Create an independent clone of this triangulation that is built from the
  // same data. There is no copy-constructor since it is too likely that this
  // would not update the associated HalfEdgeMaps in the way that the caller
  // expects.
  std::unique_ptr<FlatTriangulationCollapsed<T>> clone() const;

  virtual std::pair<HalfEdge, HalfEdge> collapse(HalfEdge) override;

  virtual void flip(HalfEdge) override;

  T area() const noexcept;

  // Note that only the horizontal part of this vector is meaningful. Rather,
  // the vertical part of the vectors are only consistent inside their faces.
  const SaddleConnection &fromEdge(HalfEdge) const;

  bool inSector(HalfEdge, const Vector &) const;
  bool inSector(HalfEdge, const Vertical<FlatTriangulationCollapsed<T>> &) const;

  // Return the saddle connections to go from this half edge's source to its negative's target.
  std::vector<SaddleConnection> cross(HalfEdge) const;

  // Return the collapsed saddle connections to turn from one half edge clockwise to the other.
  std::vector<SaddleConnection> turn(HalfEdge, HalfEdge) const;

  FlatTriangulationCollapsed<T> &operator=(FlatTriangulationCollapsed<T> &&) noexcept;

  std::shared_ptr<const FlatTriangulationCollapsed<T>> shared_from_this() const;
  std::shared_ptr<FlatTriangulationCollapsed<T>> shared_from_this();

  bool operator==(const FlatTriangulationCollapsed<T> &) const noexcept;

  template <typename S>
  friend std::ostream &operator<<(std::ostream &, const FlatTriangulationCollapsed<S> &);

 private:
  using Implementation = ::flatsurf::Implementation<FlatTriangulationCollapsed>;
  Moveable<Implementation> impl;
  friend Implementation;

  // TODO: Remove
  friend FlowComponent<FlatTriangulation<T>>;
  // TODO: Remove
  friend IntervalExchangeTransformation<FlatTriangulationCollapsed<T>>;
};

}  // namespace flatsurf

#endif
