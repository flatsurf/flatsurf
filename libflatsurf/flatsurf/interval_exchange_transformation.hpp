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

#ifndef LIBFLATSURF_INTERVAL_EXCHANGE_TRANSFORMATION_HPP
#define LIBFLATSURF_INTERVAL_EXCHANGE_TRANSFORMATION_HPP

#include <unordered_set>
#include <vector>

#include "external/spimpl/spimpl.h"
#include "intervalxt/forward.hpp"

#include "forward.hpp"

namespace flatsurf {

template <typename Surface>
class IntervalExchangeTransformation {
  using T = typename Surface::Coordinate;
  using Label = intervalxt::Label;

  static_assert(std::is_same_v<Surface, std::decay_t<Surface>>, "type must not have modifiers such as const");

 public:
  IntervalExchangeTransformation(std::shared_ptr<const Surface>, const Vector<T>& vertical, const std::vector<HalfEdge>& top, const std::vector<HalfEdge>& bottom);

  IntervalExchangeTransformation(std::shared_ptr<const Surface>, const Vector<T>& vertical, const HalfEdge);

  // Modify the surface such that each component has a unique large edge
  // and the components are such that their contours do not begin with the same
  // half edge and not end with the same half edge (unless their contours are
  // trivial.)
  static void makeUniqueLargeEdges(Surface&, const Vector<T>&);

  static std::unordered_set<HalfEdge> makeUniqueLargeEdge(Surface&, const Vector<T>&, HalfEdge&);

  intervalxt::IntervalExchangeTransformation& intervalExchangeTransformation() noexcept;
  const intervalxt::IntervalExchangeTransformation& intervalExchangeTransformation() const noexcept;

  // The Edge in the (collapsed) surface from which this label was created originally.
  Edge edge(const Label&) const;

  // TODO: Does this actually make sense here? We cannot in general determine
  // the connection without knowing the  FlowDecomposition it seems.
  SaddleConnection<FlatTriangulation<T>> connection(const Label&) const;

  template <typename S>
  friend std::ostream& operator<<(std::ostream&, const IntervalExchangeTransformation<S>&);

 private:
  using Implementation = ImplementationOf<IntervalExchangeTransformation>;
  spimpl::unique_impl_ptr<Implementation> impl;

  friend Implementation;
};

template <typename Surface, typename... Args>
IntervalExchangeTransformation(std::shared_ptr<const Surface>, Args&&...)->IntervalExchangeTransformation<Surface>;

template <typename Surface, typename... Args>
IntervalExchangeTransformation(std::shared_ptr<Surface>, Args&&...)->IntervalExchangeTransformation<Surface>;

}  // namespace flatsurf

#endif
