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

#ifndef LIBFLATSURF_VECTOR_ALGORITHM_WITH_ERROR_IPP
#define LIBFLATSURF_VECTOR_ALGORITHM_WITH_ERROR_IPP

#include <optional>
#include <exact-real/arb.hpp>

#include "flatsurf/bound.hpp"

#include "../storage/cartesian.ipp"
#include "../storage/forward.ipp"

#include "./base.ipp"
#include "./extension.ipp"
#include "./with_error.extension.ipp"

// Generic Algorithms on vectors in ℝ² with a controlled error.

namespace flatsurf {
using std::optional;

template <typename Vector>
std::optional<bool> VectorWithError<Vector>::operator==(const Vector& rhs) const noexcept {
  using Implementation = typename Vector::Implementation;
  const Vector& self = static_cast<const Vector&>(*this);

  if constexpr (has_optional_eq<Implementation>) {
    return *self.impl == *rhs.impl;
  } else if constexpr (is_forward_v<Implementation>) {
    return self.impl->vector == rhs.impl->vector;
  } else if constexpr (is_cartesian_v<Implementation>) {
    auto maybe_x = self.impl->x == rhs.impl->x;
    if (!maybe_x || !*maybe_x)
      return maybe_x;
    auto maybe_y = self.impl->x == rhs.impl->y;
    if (!maybe_y || !*maybe_y)
      return maybe_y;
    return true;
  } else {
    static_assert(false_type_v<Implementation>, "Implementation is missing operator==.");
  }
}

template <typename Vector>
std::optional<CCW> VectorWithError<Vector>::ccw(const Vector& rhs) const noexcept {
  using Implementation = typename Vector::Implementation;
  const Vector& self = static_cast<const Vector&>(*this);
  if constexpr (has_optional_ccw<Implementation>) {
    return self.impl->ccw(rhs);
  } else {
    static_assert(false_type_v<Implementation>, "Implementation is missing ccw().");
  }
}

template <typename Vector>
std::optional<ORIENTATION> VectorWithError<Vector>::orientation(const Vector& rhs) const noexcept {
  using Implementation = typename Vector::Implementation;
  const Vector& self = static_cast<const Vector&>(*this);
  if constexpr (has_optional_orientation<Implementation>) {
    return self.impl->orientation(rhs);
  } else {
    static_assert(false_type_v<Implementation>, "Implementation is missing orientation().");
  }
}

template <typename Vector>
VectorWithError<Vector>::operator std::optional<bool>() const noexcept {
  using Implementation = typename Vector::Implementation;
  const Vector& self = static_cast<const Vector&>(*this);

  if constexpr (has_optional_bool<Implementation>) {
    return static_cast<std::optional<bool>>(*self.impl);
  } else if constexpr (is_forward_v<Implementation>) {
    return static_cast<std::optional<bool>>(self.impl->vector);
  } else if constexpr (is_cartesian_v<Implementation>) {
    auto maybe_x = static_cast<std::optional<bool>>(self.impl->x);
    if (!maybe_x || !*maybe_x)
      return maybe_x;
    auto maybe_y = static_cast<std::optional<bool>>(self.impl->x);
    if (!maybe_y || !*maybe_y)
      return maybe_y;
    return true;
  } else {
    static_assert(false_type_v<Implementation>, "Implementation is missing operator<optional<bool>>.");
  }
}

template <typename Vector>
optional<bool> VectorWithError<Vector>::operator<(Bound bound) const noexcept {
  using Implementation = typename Vector::Implementation;
  const Vector& self = static_cast<const Vector&>(*this);

  if constexpr (has_optional_lt_bound<Implementation>) {
    return *self.impl < bound;
  } else if constexpr (is_forward_v<Implementation>) {
    return self.impl->vector < bound;
  } else if constexpr (is_cartesian_v<Implementation>) {
    return self.impl->x * self.impl->x + self.impl->y * self.impl->y < bound.squared;
  } else {
    static_assert(false_type_v<Implementation>, "Implementation is missing operator<(Bound).");
  }
}

template <typename Vector>
optional<bool> VectorWithError<Vector>::operator>(Bound bound) const noexcept {
  using Implementation = typename Vector::Implementation;
  const Vector& self = static_cast<const Vector&>(*this);

  if constexpr (has_optional_gt_bound<Implementation>) {
    return *self.impl > bound;
  } else if constexpr (is_forward_v<Implementation>) {
    return self.impl->vector > bound;
  } else if constexpr (is_cartesian_v<Implementation>) {
    return self.impl->x * self.impl->x + self.impl->y * self.impl->y > bound.squared;
  } else {
    static_assert(false_type_v<Implementation>, "Implementation is missing operator>(Bound).");
  }
}

template <typename Vector>
exactreal::Arb VectorWithError<Vector>::operator*(const Vector& rhs) const noexcept {
  using Implementation = typename Vector::Implementation;
  const Vector& self = static_cast<const Vector&>(*this);

  if constexpr (has_arb_scalar_product<Implementation>) {
    return *self.impl * rhs;
  } else if constexpr (is_forward_v<Implementation>) {
    return self.impl->vector * rhs.impl->vector;
  } else if constexpr (is_cartesian_v<Implementation>) {
    return self.impl->x * rhs.impl->x + self.impl->y * rhs.impl->y;
  } else {
    static_assert(false_type_v<Implementation>, "Implementation is missing scalar product operator*.");
  }
}

}  // namespace flatsurf

#endif
