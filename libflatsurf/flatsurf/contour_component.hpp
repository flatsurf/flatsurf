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

#ifndef LIBFLATSURF_CONTOUR_COMPONENT_HPP
#define LIBFLATSURF_CONTOUR_COMPONENT_HPP

#include <memory>
#include <vector>
#include <list>

#include <boost/operators.hpp>

#include "external/spimpl/spimpl.h"

#include "interval_exchange_transformation.hpp"

namespace flatsurf {

template <class Surface>
class ContourComponent : boost::equality_comparable<ContourComponent<Surface>> {
 private:
  // Components can not be created directly (other than by copying & moving
  // them.) They are byproducts of a ContourDecomposition.
  ContourComponent();

  static_assert(std::is_same_v<Surface, std::decay_t<Surface>>, "type must not have modifiers such as const");

 public:
  using T = typename Surface::Coordinate;
  using Contour = std::list<ContourConnection<Surface>>;

  IntervalExchangeTransformation<typename Surface::Collapsed> intervalExchangeTransformation() const;

  // The saddle connections at the top of this component, from right to left.
  Contour top() const;

  // The saddle connections at the bottom of this component, from left to right.
  Contour bottom() const;

  // The saddle connections going around this component in counter-clockwise order, i.e., bottom + top.
  Contour perimeter() const;

  bool operator==(const ContourComponent<Surface>&) const;

  template <typename S>
  friend std::ostream &operator<<(std::ostream &, const ContourComponent<S> &);

 // TODO: find a better solution for this
 public:
  using Implementation = ::flatsurf::Implementation<ContourComponent>;
  spimpl::impl_ptr<Implementation> impl;

  friend Implementation;
};

}  // namespace flatsurf

#endif
