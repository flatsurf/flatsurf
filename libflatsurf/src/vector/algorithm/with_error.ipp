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

#ifndef LIBFLATSURF_VECTOR_ALGORITHM_WITH_ERROR_IPP
#define LIBFLATSURF_VECTOR_ALGORITHM_WITH_ERROR_IPP

#include <exact-real/arb.hpp>
#include <optional>

#include "../../../flatsurf/bound.hpp"
#include "../storage/cartesian.ipp"
#include "../storage/forward.ipp"
#include "./base.ipp"
#include "./extension.ipp"
#include "./with_error.extension.ipp"

// Generic Algorithms on vectors in ℝ² with a controlled error.

namespace flatsurf {
using std::optional;

template <typename Vector>
std::optional<bool> VectorWithError<Vector>::operator==(const Vector& rhs) const {
  using Implementation = ImplementationOf<Vector>;
  const Vector& self = static_cast<const Vector&>(*this);

  if constexpr (has_optional_eq<Implementation>) {
    return *self.self == *rhs.self;
  } else if constexpr (is_forward_v<Implementation>) {
    return self.self->vector == rhs.self->vector;
  } else if constexpr (is_cartesian_v<Implementation>) {
    auto maybe_x = self.self->x == rhs.self->x;
    if (!maybe_x || !*maybe_x)
      return maybe_x;
    auto maybe_y = self.self->x == rhs.self->y;
    if (!maybe_y || !*maybe_y)
      return maybe_y;
    return true;
  } else {
    static_assert(false_type_v<Implementation>, "Implementation is missing operator==.");
  }
}

template <typename Vector>
std::optional<bool> VectorWithError<Vector>::operator!=(const Vector& rhs) const {
  using Implementation = ImplementationOf<Vector>;
  const Vector& self = static_cast<const Vector&>(*this);

  if constexpr (has_optional_ne<Implementation>) {
    return *self.self != *rhs.self;
  } else if constexpr (is_forward_v<Implementation>) {
    return self.self->vector != rhs.self->vector;
  } else if constexpr (is_cartesian_v<Implementation>) {
    const auto eq = self == rhs;
    if (eq) return !*eq;
    return std::nullopt;
  } else {
    static_assert(false_type_v<Implementation>, "Implementation is missing operator!=.");
  }
}

template <typename Vector>
std::optional<CCW> VectorWithError<Vector>::ccw(const Vector& rhs) const {
  using Implementation = ImplementationOf<Vector>;
  const Vector& self = static_cast<const Vector&>(*this);
  if constexpr (has_optional_ccw<Implementation>) {
    return self.self->ccw(rhs);
  } else {
    static_assert(false_type_v<Implementation>, "Implementation is missing ccw().");
  }
}

template <typename Vector>
std::optional<ORIENTATION> VectorWithError<Vector>::orientation(const Vector& rhs) const {
  using Implementation = ImplementationOf<Vector>;
  const Vector& self = static_cast<const Vector&>(*this);
  if constexpr (has_optional_orientation<Implementation>) {
    return self.self->orientation(rhs);
  } else {
    static_assert(false_type_v<Implementation>, "Implementation is missing orientation().");
  }
}

template <typename Vector>
VectorWithError<Vector>::operator std::optional<bool>() const {
  using Implementation = ImplementationOf<Vector>;
  const Vector& self = static_cast<const Vector&>(*this);

  if constexpr (has_optional_bool<Implementation>) {
    return static_cast<std::optional<bool>>(*self.self);
  } else if constexpr (is_forward_v<Implementation>) {
    return static_cast<std::optional<bool>>(self.self->vector);
  } else if constexpr (is_cartesian_v<Implementation>) {
    auto maybe_x = static_cast<std::optional<bool>>(self.self->x);
    if (!maybe_x || !*maybe_x)
      return maybe_x;
    auto maybe_y = static_cast<std::optional<bool>>(self.self->x);
    if (!maybe_y || !*maybe_y)
      return maybe_y;
    return true;
  } else {
    static_assert(false_type_v<Implementation>, "Implementation is missing operator<optional<bool>>.");
  }
}

template <typename Vector>
optional<bool> VectorWithError<Vector>::operator<(Bound bound) const {
  using Implementation = ImplementationOf<Vector>;
  const Vector& self = static_cast<const Vector&>(*this);

  if constexpr (has_optional_lt_bound<Implementation>) {
    return *self.self < bound;
  } else if constexpr (is_forward_v<Implementation>) {
    return self.self->vector < bound;
  } else if constexpr (is_cartesian_v<Implementation>) {
    return std::make_pair(self.self->x, self.self->y) < bound;
  } else {
    static_assert(false_type_v<Implementation>, "Implementation is missing operator<(Bound).");
  }
}

template <typename Vector>
optional<bool> VectorWithError<Vector>::operator>(Bound bound) const {
  using Implementation = ImplementationOf<Vector>;
  const Vector& self = static_cast<const Vector&>(*this);

  if constexpr (has_optional_gt_bound<Implementation>) {
    return *self.self > bound;
  } else if constexpr (is_forward_v<Implementation>) {
    return self.self->vector > bound;
  } else if constexpr (is_cartesian_v<Implementation>) {
    return std::make_pair(self.self->x, self.self->y) > bound;
  } else {
    static_assert(false_type_v<Implementation>, "Implementation is missing operator>(Bound).");
  }
}

template <typename Vector>
exactreal::Arb VectorWithError<Vector>::operator*(const Vector& rhs) const {
  using Implementation = ImplementationOf<Vector>;
  const Vector& self = static_cast<const Vector&>(*this);

  if constexpr (has_arb_scalar_product<Implementation>) {
    return *self.self * rhs;
  } else if constexpr (is_forward_v<Implementation>) {
    return self.self->vector * rhs.self->vector;
  } else if constexpr (is_cartesian_v<Implementation>) {
    return self.self->x * rhs.self->x + self.self->y * rhs.self->y;
  } else {
    static_assert(false_type_v<Implementation>, "Implementation is missing scalar product operator*.");
  }
}

}  // namespace flatsurf

#endif
