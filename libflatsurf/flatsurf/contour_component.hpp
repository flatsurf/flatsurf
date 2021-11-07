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

#ifndef LIBFLATSURF_CONTOUR_COMPONENT_HPP
#define LIBFLATSURF_CONTOUR_COMPONENT_HPP

#include <boost/operators.hpp>
#include <iosfwd>
#include <vector>

#include "copyable.hpp"

namespace flatsurf {

// A component of a Contour Decomposition, i.e., a connected component whose
// graph of faces when only considering faces adjacent that share a
// non-vertical edge.
// Note that this object is immutable, all its members are const.
template <class Surface>
class ContourComponent : boost::equality_comparable<ContourComponent<Surface>> {
 private:
  static_assert(std::is_same_v<Surface, std::decay_t<Surface>>, "type must not have modifiers such as const");

  // Components can not be created directly (other than by copying & moving
  // them.) They are byproducts of a ContourDecomposition.
  template <typename... Args>
  ContourComponent(PrivateConstructor, Args &&...);

 public:
  using T = typename Surface::Coordinate;
  using Contour = std::vector<ContourConnection<Surface>>;

  // Return an interval exchange transformation corresponding to this component.
  IntervalExchangeTransformation<FlatTriangulationCollapsed<T>> intervalExchangeTransformation() const;

  // The connections walking the top of this component from right to left.
  Path<FlatTriangulation<T>> top() const;

  // The contour connections at the top of this component, from right to left.
  // The naming here is a bit unfortunate, see https://github.com/flatsurf/flatsurf/issues/146.
  Contour topContour() const;

  // The connections going along the bottom of this component from left to right.
  Path<FlatTriangulation<T>> bottom() const;

  // The contour connections at the bottom of this component, from left to right.
  // The naming here is a bit unfortunate, see https://github.com/flatsurf/flatsurf/issues/146.
  Contour bottomContour() const;

  // The contour connections going around this component in counter-clockwise order, i.e., bottomContour() + topContour().
  // The naming here is a bit unfortunate, see https://github.com/flatsurf/flatsurf/issues/146.
  Contour perimeterContour() const;

  // The connections going around this component in counterclockwise order, i.e., bottom() + top().
  Path<FlatTriangulation<T>> perimeter() const;

  T width() const;
  T area() const;

  bool operator==(const ContourComponent<Surface> &) const;

  template <typename S>
  friend std::ostream &operator<<(std::ostream &, const ContourComponent<S> &);

 private:
  Copyable<ContourComponent> self;

  friend ImplementationOf<ContourComponent>;
};

}  // namespace flatsurf

#endif
