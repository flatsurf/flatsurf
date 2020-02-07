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

#include <set>
#include <vector>

#include "external/spimpl/spimpl.h"
#include "intervalxt/forward.hpp"

#include "forward.hpp"

namespace flatsurf {

template <class Surface>
class IntervalExchangeTransformation {
  using Label = intervalxt::Label;
  using IET = intervalxt::IntervalExchangeTransformation;
  using Vector = typename Surface::Vector;
  using SaddleConnection = typename Surface::SaddleConnection;

  static_assert(std::is_same_v<Surface, std::decay_t<Surface>>, "type must not have modifiers such as const");

 public:
  IntervalExchangeTransformation(std::shared_ptr<const Surface>, const Vector& vertical, const std::vector<HalfEdge>& top, const std::vector<HalfEdge>& bottom);

  IntervalExchangeTransformation(std::shared_ptr<const Surface>, const Vector& vertical, const HalfEdge);

  // Modify the surface such that each component has a unique large edge
  // and the components are such that their contours do not begin with the same
  // half edge and not end with the same half edge (unless their contours are
  // trivial.)
  static void makeUniqueLargeEdges(Surface&, const Vector&);

  static std::set<HalfEdge> makeUniqueLargeEdge(Surface&, const Vector&, HalfEdge&);

  IET& intervalExchangeTransformation() noexcept;
  const IET& intervalExchangeTransformation() const noexcept;

  // The Edge in the (collapsed) surface from which this label was created originally.
  Edge edge(const Label&) const;

  // TODO: Does this actually make sense here? We cannot in general determine
  // the connection without knowing the  FlowDecomposition it seems.
  SaddleConnection connection(const Label&) const;

  template <typename S>
  friend std::ostream& operator<<(std::ostream&, const IntervalExchangeTransformation<S>&);

 private:
  using Implementation = ::flatsurf::Implementation<IntervalExchangeTransformation>;
  spimpl::unique_impl_ptr<Implementation> impl;

  friend Implementation;
};

}  // namespace flatsurf

#endif
