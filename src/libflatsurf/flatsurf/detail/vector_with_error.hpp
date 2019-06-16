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

#ifndef LIBFLATSURF_VECTOR_WITH_ERROR_HPP
#define LIBFLATSURF_VECTOR_WITH_ERROR_HPP

#include <optional>

#include "flatsurf/detail/vector_base.hpp"

namespace flatsurf::detail {
// A vector in ℝ² with explicit error bounds.
template <typename Vector>
class VectorWithError : public VectorBase<Vector> {
 public:
  std::optional<CCW> ccw(const Vector &) const noexcept;
  std::optional<ORIENTATION> orientation(const Vector &) const noexcept;
  std::optional<bool> insideCircumcircle(std::initializer_list<Vector>) const noexcept;

  std::optional<bool> operator>(const Bound) const noexcept;
  std::optional<bool> operator>=(const Bound) const noexcept;
  std::optional<bool> operator<(const Bound) const noexcept;
  std::optional<bool> operator<=(const Bound) const noexcept;
  // Return true if both vectors are the same exact vector, false if the
  // respective balls do not overlap.
  std::optional<bool> operator==(const Vector &) const noexcept;
  // Return true if the respective balls do not overlap, false if both vectors
  // are the same exact vector.
  std::optional<bool> operator!=(const Vector &) const noexcept;
  // Return true if the respective ball does not contain the zero vector, false
  // if this is exactly the zero vector.
  explicit operator std::optional<bool>() const noexcept;

  // Return the scalar product with the argument.
  exactreal::Arb operator*(const Vector&) const noexcept;
};
}  // namespace flatsurf::detail

#endif
