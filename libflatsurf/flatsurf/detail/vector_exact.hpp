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

#ifndef LIBFLATSURF_DETAIL_VECTOR_EXACT_HPP
#define LIBFLATSURF_DETAIL_VECTOR_EXACT_HPP

#include <gmpxx.h>

#include <boost/core/empty_value.hpp>
#include <intervalxt/forward.hpp>
#include <vector>

#include "vector_base.hpp"

namespace flatsurf::detail {
// An exact vector in ℝ².
template <typename Vector, typename T>
class VectorExact : public VectorBase<Vector>,
                    private boost::less_than_comparable<Vector, Bound>,
                    private boost::equality_comparable<Vector>,
                    private std::conditional_t<std::is_same_v<T, mpz_class>, boost::empty_init_t, boost::multipliable<Vector, T>> {
 public:
  using Coordinate = T;

  CCW ccw(const Vector &) const;
  ORIENTATION orientation(const Vector &) const;
  // Return whether this vector is parallel to the given vector, i.e., whether
  // the vectors are collinear and euqally oriented.
  bool parallel(const Vector&) const;
  bool insideCircumcircle(std::initializer_list<Vector>) const;

  // Return the scalar product with the argument
  T operator*(const Vector &) const;

  bool operator>(const Bound) const;
  bool operator<(const Bound) const;
  bool operator==(const Vector &) const;
  explicit operator bool() const;

  Vector& operator*=(const T&);
  using VectorBase<Vector>::operator*=;

  // Return whether this vector is contained in the sector described by
  // inclusive and exclusive in counter-clockwise order. If inclusive and
  // exclusive are collinear, the sector consists of that single ray.
  bool inSector(const Vector &inclusive, const Vector &exclusive) const;

  Coordinate x() const;
  Coordinate y() const;

  // Return the result of applying this matrix from the left.
  Vector applyMatrix(const T& a, const T& b, const T& c, const T& d) const;

  // Return twice the area of the polygon described by the (counter-clockwise) perimeter.
  static T area(const std::vector<Vector> &perimeter);

  // Implements a less-than operator which orders vectors by their slope,
  // i.e., it orders vectors (x, y) by their quotient y/x (sending y/0 to +∞.)
  struct CompareSlope {
    bool operator()(const Vector &lhs, const Vector &rhs) const;
  };

  // Implements a less-than operator which orders vectors by their length.
  struct CompareLength {
    bool operator()(const Vector &lhs, const Vector &rhs) const;
  };
};
}  // namespace flatsurf::detail

#endif
