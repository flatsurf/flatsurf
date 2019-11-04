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

#ifndef LIBFLATSURF_VECTOR_ALGORITHM_EXACT_IPP
#define LIBFLATSURF_VECTOR_ALGORITHM_EXACT_IPP

#include <optional>

#include "../../flatsurf/bound.hpp"

#include "../storage/cartesian.ipp"
#include "../storage/forward.ipp"

#include "./base.ipp"
#include "./exact.extension.ipp"
#include "./extension.ipp"

// Generic Algorithms on exact vectors in ℝ².

using namespace flatsurf::detail;

namespace flatsurf {
using std::optional;

template <typename Vector, typename T>
VectorExact<Vector, T>::operator bool() const noexcept {
  using Implementation = typename Vector::Implementation;
  const Vector& self = static_cast<const Vector&>(*this);

  if constexpr (has_bool<Implementation>) {
    return static_cast<bool>(*self.impl);
  } else if constexpr (is_forward_v<Implementation>) {
    return static_cast<bool>(self.impl->vector);
  } else if constexpr (has_approximation_v<Implementation>) {
    auto maybe = static_cast<optional<bool>>(self.impl->approximation());
    if (maybe)
      return *maybe;
    return static_cast<bool>(static_cast<const typename Implementation::Exact>(*self.impl));
  } else {
    return self.x() || self.y();
  }
}

template <typename Vector, typename T>
bool VectorExact<Vector, T>::operator<(Bound bound) const noexcept {
  using Implementation = typename Vector::Implementation;
  const Vector& self = static_cast<const Vector&>(*this);

  if constexpr (has_lt_bound<Implementation>) {
    return *self.impl < bound;
  } else if constexpr (is_forward_v<Implementation>) {
    return self.impl->vector < bound;
  } else if constexpr (has_approximation_v<Implementation>) {
    auto maybe = static_cast<optional<bool>>(self.impl->approximation() < bound);
    if (maybe)
      return *maybe;
    return static_cast<const typename Implementation::Exact>(*self.impl) < bound;
  } else {
    return self.x() * self.x() + self.y() * self.y() < bound.length() * bound.length();
  }
}

template <typename Vector, typename T>
bool VectorExact<Vector, T>::operator>(Bound bound) const noexcept {
  using Implementation = typename Vector::Implementation;
  const Vector& self = static_cast<const Vector&>(*this);

  if constexpr (has_gt_bound<Implementation>) {
    return *self.impl > bound;
  } else if constexpr (is_forward_v<Implementation>) {
    return self.impl->vector > bound;
  } else if constexpr (has_approximation_v<Implementation>) {
    auto maybe = static_cast<optional<bool>>(self.impl->approximation() > bound);
    if (maybe)
      return *maybe;
    return static_cast<const typename Implementation::Exact>(*self.impl) > bound;
  } else {
    return self.x() * self.x() + self.y() * self.y() > bound.length() * bound.length();
  }
}

template <typename Vector, typename T>
bool VectorExact<Vector, T>::operator==(const Vector& rhs) const noexcept {
  using Implementation = typename Vector::Implementation;
  const Vector& self = static_cast<const Vector&>(*this);

  if constexpr (has_eq<Implementation>) {
    return *self.impl == *rhs.impl;
  } else if constexpr (is_forward_v<Implementation>) {
    return self.impl->vector == rhs.impl->vector;
  } else if constexpr (has_approximation_v<Implementation>) {
    auto maybe = static_cast<optional<bool>>(self.impl->approximation() == rhs.impl->approximation());
    if (maybe)
      return *maybe;
    return static_cast<const typename Implementation::Exact>(*self.impl) == static_cast<const typename Implementation::Exact>(*rhs.impl);
  } else {
    return self.x() == rhs.x() && self.y() == rhs.y();
  }
}

template <typename Vector, typename T>
CCW VectorExact<Vector, T>::ccw(const Vector& rhs) const noexcept {
  using Implementation = typename Vector::Implementation;
  const Vector& self = static_cast<const Vector&>(*this);

  if constexpr (has_ccw<Implementation>) {
    return self.impl->ccw(*rhs.impl);
  } else if constexpr (is_forward_v<Implementation>) {
    return self.impl->vector.ccw(rhs.impl->vector);
  } else if constexpr (has_approximation_v<Implementation>) {
    auto maybe = self.impl->approximation().ccw(rhs.impl->approximation());
    if (maybe)
      return *maybe;
    return static_cast<const typename Implementation::Exact>(*self.impl).ccw(static_cast<const typename Implementation::Exact>(*rhs.impl));
  } else {
    // An alternative algorithm (somewhere in the git history) might be much
    // faster: Try to decide with the approximations. If they are not clear,
    // check collinearity (or maybe check it even first if it's cheaper), then
    // increase precision until the approximations are good enough to decide.
    // Presumably, all of this could happen automagically with yap just by
    // looking at the predicate sgn(x*y - x'*y').

    const T a = self.x() * rhs.y();
    const T b = rhs.x() * self.y();

    if (a > b) {
      return CCW::COUNTERCLOCKWISE;
    } else if (a < b) {
      return CCW::CLOCKWISE;
    } else {
      return CCW::COLLINEAR;
    }
  }
}

template <typename Vector, typename T>
T VectorExact<Vector, T>::x() const noexcept {
  using Implementation = typename Vector::Implementation;
  const Vector& self = static_cast<const Vector&>(*this);

  if constexpr (has_x<Implementation>) {
    return self.impl->x();
  } else if constexpr (is_forward_v<Implementation>) {
    return self.impl->vector.x();
  } else if constexpr (is_cartesian_v<Implementation>) {
    return self.impl->x;
  } else {
    static_assert(false_type_v<Implementation>, "Implementation is missing x().");
  }
}

template <typename Vector, typename T>
T VectorExact<Vector, T>::y() const noexcept {
  using Implementation = typename Vector::Implementation;
  const Vector& self = static_cast<const Vector&>(*this);

  if constexpr (has_y<Implementation>) {
    return self.impl->y();
  } else if constexpr (is_forward_v<Implementation>) {
    return self.impl->vector.y();
  } else if constexpr (is_cartesian_v<Implementation>) {
    return self.impl->y;
  } else {
    static_assert(false_type_v<Implementation>, "Implementation is missing y().");
  }
}

template <typename Vector, typename T>
ORIENTATION VectorExact<Vector, T>::orientation(const Vector& rhs) const noexcept {
  using Implementation = typename Vector::Implementation;
  const Vector& self = static_cast<const Vector&>(*this);

  if constexpr (has_orientation<Implementation>) {
    return self.impl->orientation(*rhs.impl);
  } else if constexpr (is_forward_v<Implementation>) {
    return self.impl->vector.orientation(rhs.impl->vector);
  } else if constexpr (has_approximation_v<Implementation>) {
    auto maybe = self.impl->approximation().orientation(rhs.impl->approximation());
    if (maybe)
      return *maybe;
    return static_cast<const typename Implementation::Exact>(*self.impl).orientation(static_cast<const typename Implementation::Exact>(*rhs.impl));
  } else {
    // An alternative algorithm (somewhere in the git history) might be much
    // faster: Try to decide with the approximations. If they are not clear,
    // check collinearity (or maybe check it even first if it's cheaper), then
    // increase precision until the approximations are good enough to decide.
    // Presumably, all of this could happen automagically with yap just by
    // looking at the predicate sgn(x*x' + y*y').
    const T dot = self.x() * rhs.x() + self.y() * rhs.y();

    if (dot > 0) {
      return ORIENTATION::SAME;
    } else if (dot < 0) {
      return ORIENTATION::OPPOSITE;
    } else {
      return ORIENTATION::ORTHOGONAL;
    }
  }
}

template <typename Vector, typename T>
T VectorExact<Vector, T>::operator*(const Vector& rhs) const noexcept {
  using Implementation = typename Vector::Implementation;
  const Vector& self = static_cast<const Vector&>(*this);

  if constexpr (has_scalar_product<Implementation>) {
    return self.impl->operator*(rhs);
  } else if constexpr (is_cartesian_v<Implementation>) {
    return self.impl->x * rhs.impl->x + self.impl->y * rhs.impl->y;
  } else if constexpr (is_forward_v<Implementation>) {
    return self.impl->vector * rhs.impl->vector;
  } else {
    static_assert(false_type_v<Implementation>, "Implementation is missing scalar product operator*().");
  }
}

}  // namespace flatsurf

#endif
