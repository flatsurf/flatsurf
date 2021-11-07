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

#ifndef LIBFLATSURF_CONTOUR_CONNECTION_HPP
#define LIBFLATSURF_CONTOUR_CONNECTION_HPP

#include <boost/operators.hpp>
#include <iosfwd>

#include "copyable.hpp"

namespace flatsurf {

// A non-vertical connection on the perimeter of a Contour Component.
// Note that this object is immutable, all its members are const.
template <typename Surface>
class ContourConnection : boost::equality_comparable<ContourComponent<Surface>> {
 private:
  // Connections can not be created directly (other than by copying & moving
  // them.) They are byproducts of a ContourDecomposition.
  template <typename... Args>
  ContourConnection(PrivateConstructor, Args &&...);

  static_assert(std::is_same_v<Surface, std::decay_t<Surface>>, "type must not have modifiers such as const");

  using T = typename Surface::Coordinate;
  using Path = ::flatsurf::Path<FlatTriangulation<T>>;

 public:
  // The saddle connection in the surface corresponding to this contour
  // connection. Going from left to right.
  const SaddleConnection<FlatTriangulation<T>> &horizontal() const;

  // The saddle connections of left(), connection(), and right() in
  // contourclockwise order, i.e., either -left() + connection() + right() or
  // -right() - connection() + left().
  Path perimeter() const;

  // The vertical connections on the left of this non-vertical connection; from
  // top to bottom.
  Path left() const;

  // The vertical connections on the right of this non-vertical connection;
  // from bottom to top.
  Path right() const;

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
  Copyable<ContourConnection> self;

  friend ImplementationOf<ContourConnection>;
};
}  // namespace flatsurf

#endif
