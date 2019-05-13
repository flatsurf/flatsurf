/**********************************************************************
 *  This file is part of flatsurf.
 *
 *        Copyright (C) 2019 Julian Rüth
 *
 *  Flatsurf is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 of the License, or
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

#ifndef LIBFLATSURF_VECTOR_LONGLONG_HPP
#define LIBFLATSURF_VECTOR_LONGLONG_HPP

#include <boost/operators.hpp>
#include <iosfwd>

#include "flatsurf/flatsurf.hpp"
#include "flatsurf/ccw.hpp"
#include "flatsurf/orientation.hpp"
#include "flatsurf/vector_along_triangulation.hpp"

namespace flatsurf {
struct Bound;
template <typename V> struct VectorAlongTriangulation;

// An exact vector in ℝ² whose coordinates are integers in the range of a long
// long. For operations such as ccw() to work, the square of the coordinates
// should still fit into a long long as well.
struct VectorLongLong : boost::additive<VectorLongLong>,
                        boost::multipliable<VectorLongLong, int>,
                        boost::less_than_comparable<VectorLongLong, Bound>,
                        boost::equality_comparable<VectorLongLong> {
  using V = VectorLongLong;

  VectorLongLong();
  VectorLongLong(long long x, long long y);

  CCW ccw(const V &rhs) const;
  ORIENTATION orientation(const V &) const;

  friend std::ostream &operator<<(std::ostream &, const V &);
  V operator-() const;
  bool operator>(const Bound) const;
  bool operator<(const Bound) const;
  bool operator==(const VectorLongLong &) const;
  explicit operator bool() const;
  V &operator+=(const V &);
  V &operator-=(const V &);
  V &operator*=(const int);

  using AlongTriangulation = VectorAlongTriangulation<VectorLongLong>;

private:
  long long x;
  long long y;
};
} // namespace flatsurf

#endif
