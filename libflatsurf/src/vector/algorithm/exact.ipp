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

#include <fmt/format.h>
#include <fmt/ostream.h>

#include <optional>

#include "../../../flatsurf/bound.hpp"
#include "../../../flatsurf/ccw.hpp"
#include "../../../flatsurf/fmt.hpp"
#include "../../../flatsurf/orientation.hpp"
#include "../../external/gmpxxll/gmpxxll/mpz_class.hpp"
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
  using Implementation = ImplementationOf<Vector>;
  const Vector& self = static_cast<const Vector&>(*this);

  if constexpr (has_bool<Implementation>) {
    return static_cast<bool>(*self.self);
  } else if constexpr (is_forward_v<Implementation>) {
    return static_cast<bool>(self.self->vector);
  } else if constexpr (has_approximation_v<Implementation>) {
    auto maybe = static_cast<optional<bool>>(self.self->approximation());
    if (maybe)
      return *maybe;
    return static_cast<bool>(static_cast<const typename Implementation::Exact>(*self.self));
  } else {
    return self.x() || self.y();
  }
}

template <typename Vector, typename T>
bool VectorExact<Vector, T>::operator<(Bound bound) const noexcept {
  using Implementation = ImplementationOf<Vector>;
  const Vector& self = static_cast<const Vector&>(*this);

  if constexpr (has_lt_bound<Implementation>) {
    return *self.self < bound;
  } else if constexpr (is_forward_v<Implementation>) {
    return self.self->vector < bound;
  } else if constexpr (has_approximation_v<Implementation>) {
    auto maybe = static_cast<optional<bool>>(self.self->approximation() < bound);
    if (maybe)
      return *maybe;
    return static_cast<const typename Implementation::Exact>(*self.self) < bound;
  } else {
    return self.x() * self.x() + self.y() * self.y() < ::gmpxxll::mpz_class(bound.squared());
  }
}

template <typename Vector, typename T>
bool VectorExact<Vector, T>::operator>(Bound bound) const noexcept {
  using Implementation = ImplementationOf<Vector>;
  const Vector& self = static_cast<const Vector&>(*this);

  if constexpr (has_gt_bound<Implementation>) {
    return *self.self > bound;
  } else if constexpr (is_forward_v<Implementation>) {
    return self.self->vector > bound;
  } else if constexpr (has_approximation_v<Implementation>) {
    auto maybe = static_cast<optional<bool>>(self.self->approximation() > bound);
    if (maybe)
      return *maybe;
    return static_cast<const typename Implementation::Exact>(*self.self) > bound;
  } else {
    return self.x() * self.x() + self.y() * self.y() > ::gmpxxll::mpz_class(bound.squared());
  }
}

template <typename Vector, typename T>
bool VectorExact<Vector, T>::operator==(const Vector& rhs) const noexcept {
  using Implementation = ImplementationOf<Vector>;
  const Vector& self = static_cast<const Vector&>(*this);

  if constexpr (has_eq<Implementation>) {
    return *self.self == *rhs.self;
  } else if constexpr (is_forward_v<Implementation>) {
    return self.self->vector == rhs.self->vector;
  } else if constexpr (has_approximation_v<Implementation>) {
    auto maybe = static_cast<optional<bool>>(self.self->approximation() == rhs.self->approximation());
    if (maybe)
      return *maybe;
    return static_cast<const typename Implementation::Exact>(*self.self) == static_cast<const typename Implementation::Exact>(*rhs.self);
  } else {
    return self.x() == rhs.x() && self.y() == rhs.y();
  }
}

template <typename Vector, typename T>
CCW VectorExact<Vector, T>::ccw(const Vector& rhs) const noexcept {
  using Implementation = ImplementationOf<Vector>;
  const Vector& self = static_cast<const Vector&>(*this);

  if constexpr (has_ccw<Implementation>) {
    return self.self->ccw(*rhs.self);
  } else if constexpr (is_forward_v<Implementation>) {
    return self.self->vector.ccw(rhs.self->vector);
  } else if constexpr (has_approximation_v<Implementation>) {
    auto maybe = self.self->approximation().ccw(rhs.self->approximation());
    if (maybe)
      return *maybe;
    return static_cast<const typename Implementation::Exact>(*self.self).ccw(static_cast<const typename Implementation::Exact>(*rhs.self));
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
  using Implementation = ImplementationOf<Vector>;
  const Vector& self = static_cast<const Vector&>(*this);

  if constexpr (has_x<Implementation>) {
    return self.self->x();
  } else if constexpr (is_forward_v<Implementation>) {
    return self.self->vector.x();
  } else if constexpr (is_cartesian_v<Implementation>) {
    return self.self->x;
  } else {
    static_assert(false_type_v<Implementation>, "Implementation is missing x().");
  }
}

template <typename Vector, typename T>
T VectorExact<Vector, T>::y() const noexcept {
  using Implementation = ImplementationOf<Vector>;
  const Vector& self = static_cast<const Vector&>(*this);

  if constexpr (has_y<Implementation>) {
    return self.self->y();
  } else if constexpr (is_forward_v<Implementation>) {
    return self.self->vector.y();
  } else if constexpr (is_cartesian_v<Implementation>) {
    return self.self->y;
  } else {
    static_assert(false_type_v<Implementation>, "Implementation is missing y().");
  }
}

template <typename Vector, typename T>
ORIENTATION VectorExact<Vector, T>::orientation(const Vector& rhs) const noexcept {
  using Implementation = ImplementationOf<Vector>;
  const Vector& self = static_cast<const Vector&>(*this);

  if constexpr (has_orientation<Implementation>) {
    return self.self->orientation(*rhs.self);
  } else if constexpr (is_forward_v<Implementation>) {
    return self.self->vector.orientation(rhs.self->vector);
  } else if constexpr (has_approximation_v<Implementation>) {
    auto maybe = self.self->approximation().orientation(rhs.self->approximation());
    if (maybe)
      return *maybe;
    return static_cast<const typename Implementation::Exact>(*self.self).orientation(static_cast<const typename Implementation::Exact>(*rhs.self));
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
T VectorExact<Vector, T>::area(const std::vector<Vector>& perimeter) {
  Vector current;

  T area = T();

  for (const auto& v : perimeter) {
    Vector next = current + v;

    area += current.x() * next.y() - current.y() * next.x();

    current = next;
  }

  ASSERT(!current, fmt::format("Polygon must be closed but this polygon's sides [{}] summed to {}", fmt::join(perimeter, ", "), current));

  ASSERT(area >= 0, fmt::format("Area of polygon must be positive but the area of this polygon [{}] was {}; maybe the polygon was not oriented counterclockwise?", fmt::join(perimeter, ", "), area));

  return area;
}

template <typename Vector, typename T>
T VectorExact<Vector, T>::operator*(const Vector& rhs) const noexcept {
  using Implementation = ImplementationOf<Vector>;
  const Vector& self = static_cast<const Vector&>(*this);

  if constexpr (has_scalar_product<Implementation>) {
    return self.self->operator*(rhs);
  } else if constexpr (is_cartesian_v<Implementation>) {
    return self.self->x * rhs.self->x + self.self->y * rhs.self->y;
  } else if constexpr (is_forward_v<Implementation>) {
    return self.self->vector * rhs.self->vector;
  } else {
    static_assert(false_type_v<Implementation>, "Implementation is missing scalar product operator*().");
  }
}

template <typename Vector, typename T>
bool VectorExact<Vector, T>::inSector(const Vector& begin, const Vector& end) const {
  const Vector& self = static_cast<const Vector&>(*this);

  CHECK_ARGUMENT(begin, "zero vector cannot define a sector");
  CHECK_ARGUMENT(end, "zero vector cannot define a sector");
  CHECK_ARGUMENT(self, "zero vector containement in sector is not well defined");

  switch (begin.ccw(end)) {
    case CCW::COLLINEAR:
      switch (begin.orientation(end)) {
        case ORIENTATION::SAME:
          return ccw(begin) == CCW::COLLINEAR && orientation(begin) == ORIENTATION::SAME;
        case ORIENTATION::OPPOSITE:
          switch (begin.ccw(self)) {
            case CCW::COUNTERCLOCKWISE:
              return true;
            case CCW::CLOCKWISE:
              return false;
            case CCW::COLLINEAR:
              return begin.orientation(self) == ORIENTATION::SAME;
          }
        default:
          UNREACHABLE("non-zero collinear vectors cannot be orthogonal");
      }
    case CCW::CLOCKWISE:
      return !inSector(end, begin);
    case CCW::COUNTERCLOCKWISE:
      return begin.ccw(self) != CCW::CLOCKWISE && self.ccw(end) == CCW::COUNTERCLOCKWISE;
  }

  UNREACHABLE("two vectors must be clockwise, counter-clockwise, or collinear");
}

template <typename Vector, typename T>
bool VectorExact<Vector, T>::CompareSlope::operator()(const Vector& lhs, const Vector& rhs) const {
  ASSERT(lhs.x() || lhs.y(), "zero vector has no slope");
  ASSERT(rhs.x() || rhs.y(), "zero vector has no slope");

  const int lhs_infinite = lhs.x() ? 0 : (lhs.y() < 0 ? -1 : 1);
  const int rhs_infinite = rhs.x() ? 0 : (rhs.y() < 0 ? -1 : 1);

  if (lhs_infinite || rhs_infinite)
    return lhs_infinite < rhs_infinite;
  else {
    const T a = lhs.y() * rhs.x();
    const T b = rhs.y() * lhs.x();

    if ((lhs.x() < 0) == (rhs.x() < 0))
      return a < b;
    else
      return a > b;
  }
}

}  // namespace flatsurf

#endif
