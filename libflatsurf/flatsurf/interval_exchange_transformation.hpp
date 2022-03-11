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

#ifndef LIBFLATSURF_INTERVAL_EXCHANGE_TRANSFORMATION_HPP
#define LIBFLATSURF_INTERVAL_EXCHANGE_TRANSFORMATION_HPP

#include <iosfwd>
#include <unordered_set>
#include <vector>

#include "intervalxt/forward.hpp"
#include "movable.hpp"

namespace flatsurf {

// An Interval Exchange Transformation on a connected component of a
// triangulated translation surface with respect to a vertical direction.
// Given a triangulated translation surface one can, after performing a finite
// number of flips find (for each connected component) a pair of top and bottom
// contours, i.e., connected sequences of half edges which describe the flow
// through this component. How the half edges are permutated between the bottom
// and top contour describes an Interval Exchange Transformation which is the
// one this class represents.
template <typename Surface>
class IntervalExchangeTransformation {
  using T = typename Surface::Coordinate;
  using Label = intervalxt::Label;

  static_assert(std::is_same_v<Surface, std::decay_t<Surface>>, "type must not have modifiers such as const");

  template <typename... Args>
  IntervalExchangeTransformation(PrivateConstructor, Args&&... args);

 public:
  IntervalExchangeTransformation(const Surface&, const Vector<T>& vertical, const std::vector<HalfEdge>& top, const std::vector<HalfEdge>& bottom);

  // Create the Interval Exchange Transformation starting from the given unique large edge in its connected component.
  IntervalExchangeTransformation(const Surface&, const Vector<T>& vertical, const HalfEdge large);

  // Modify the surface such that each component has a unique large edge
  // and the components are such that their contours do not begin with the same
  // half edge and not end with the same half edge (unless their contours are
  // trivial.)
  static void makeUniqueLargeEdges(Surface&, const Vector<T>&);

  intervalxt::IntervalExchangeTransformation& intervalExchangeTransformation();
  const intervalxt::IntervalExchangeTransformation& intervalExchangeTransformation() const;

  // Return a clone of this interval exchange transformation ignoring the
  // structure of the underlying surface.
  // Note that the labels are normalized to A, …, in the resulting interval
  // exchange transformation. These labels are unrelated to the original
  // labels.
  // The lengths underlying the returned interval exchange transformation are
  // such that all lengths can be safely `boost::type_erasure::any_cast` to a
  // `T`.
  // In fact, the lengths are of the type `intervalxt::sample::Lengths<T>`.
  // However, since that is a static type (living in an unnamed namespace) one
  // cannot `boost::type_erasure::any_cast` the result to that type. In
  // principle one could `any_cast` to a `void*` and then `reinterpret_cast` to
  // a `intervalxt::sample::Lengths<T>`. However, such a cast is only safe if
  // the headers of intervalxt are binary compatible with the headers that were
  // used to build libflatsurf.
  intervalxt::IntervalExchangeTransformation forget() const;

  // The Edge in the (collapsed) surface from which this label was created originally.
  Edge edge(const Label&) const;

  const SaddleConnection<FlatTriangulation<T>>& operator[](const Label&) const;

  template <typename S>
  friend std::ostream& operator<<(std::ostream&, const IntervalExchangeTransformation<S>&);

 private:
  Movable<IntervalExchangeTransformation> self;

  friend ImplementationOf<IntervalExchangeTransformation>;
};

}  // namespace flatsurf

#endif
