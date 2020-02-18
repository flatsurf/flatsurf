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

#ifndef LIBFLATSURF_CONTOUR_CONNECTION_HPP
#define LIBFLATSURF_CONTOUR_CONNECTION_HPP

#include <list>

#include <boost/operators.hpp>

#include "external/spimpl/spimpl.h"

#include "forward.hpp"

namespace flatsurf {

// A non-vertical connection.
// Note that this object is immutable, all its members are const.
template <typename Surface>
class ContourConnection : boost::equality_comparable<ContourComponent<Surface>> {
 private:
  // Connections can not be created directly (other than by copying & moving
  // them.) They are byproducts of a ContourDecomposition.
  ContourConnection();

  static_assert(std::is_same_v<Surface, std::decay_t<Surface>>, "type must not have modifiers such as const");

  using T = typename Surface::Coordinate;

 public:
  // TODO: Unify naming of saddle connection returning methods.
  // The saddle connection in the surface corresponding to this contour
  // connection. Going from left to right if this is a bottom connection,
  // otherwise going from right to left.
  const SaddleConnection<FlatTriangulation<T>>& connection() const;

  // The saddle connections of left(), connection(), and right() in
  // contourclockwise order, i.e., either -left() + connection() + right() or
  // -right() - connection() + left().
  std::list<SaddleConnection<FlatTriangulation<T>>> perimeter() const;

  // The vertical connections on the left of this non-vertical connection going
  // from the left end of `connection` towards the interior.
  std::list<SaddleConnection<FlatTriangulation<T>>> left() const;
  // The vertical connections on the right of this non-vertical connection going
  // from the right end of `connection` towards the interior.
  std::list<SaddleConnection<FlatTriangulation<T>>> right() const;

  ContourConnection nextInPerimeter() const;
  ContourConnection previousInPerimeter() const;

  ContourComponent<Surface> component() const;

  bool top() const;
  bool bottom() const;

  ContourConnection<Surface> operator-() const;

  bool operator==(const ContourConnection<Surface> &) const;

  template <typename S>
  friend std::ostream &operator<<(std::ostream &, const ContourConnection<S> &);

 private:
  using Implementation = ::flatsurf::Implementation<ContourConnection>;
  spimpl::impl_ptr<Implementation> impl;

  friend Implementation;
};
}  // namespace flatsurf

#endif
