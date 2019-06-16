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

#ifndef LIBFLATSURF_DETAIL_VECTOR_EXACT_HPP
#define LIBFLATSURF_DETAIL_VECTOR_EXACT_HPP

#include <intervalxt/forward.hpp>

#include "flatsurf/detail/vector_base.hpp"

namespace flatsurf::detail {
// An exact vector in ℝ².
template <typename Vector, typename T>
class VectorExact : public VectorBase<Vector>,
                    private boost::less_than_comparable<Vector, Bound>,
                    private boost::equality_comparable<Vector> {
 public:
  using Coordinate = T;

  CCW ccw(const Vector &) const noexcept;
  ORIENTATION orientation(const Vector &) const noexcept;
  bool insideCircumcircle(std::initializer_list<Vector>) const noexcept;

  // Return the scalar product with the argument
  T operator*(const Vector&) const noexcept;

  bool operator>(const Bound) const noexcept;
  bool operator<(const Bound) const noexcept;
  bool operator==(const Vector &) const noexcept;
  explicit operator bool() const noexcept;

  Coordinate x() const noexcept;
  Coordinate y() const noexcept;
};
}  // namespace flatsurf::detail

#endif
