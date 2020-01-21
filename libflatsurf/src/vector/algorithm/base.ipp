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

#ifndef LIBFLATSURF_VECTOR_ALGORITHM_BASE_IPP
#define LIBFLATSURF_VECTOR_ALGORITHM_BASE_IPP

#include <iosfwd>

#include "../../../flatsurf/detail/vector_base.hpp"
#include "../../../flatsurf/vector.hpp"

#include <exact-real/arb.hpp>

#include "../../util/false.ipp"
#include "../storage/cartesian.ipp"
#include "../storage/forward.ipp"
#include "./base.extension.ipp"
#include "./extension.ipp"

// Generic Algorithms on vectors in ℝ².

using namespace flatsurf::detail;

namespace flatsurf {
template <typename Vector>
Vector& VectorBase<Vector>::operator+=(const Vector& rhs) {
  using Implementation = typename Vector::Implementation;
  Vector& self = static_cast<Vector&>(*this);

  if constexpr (has_binary_inplace_plus<Implementation>) {
    *self.impl += rhs;
  } else if constexpr (is_cartesian_v<Implementation>) {
    self.impl->x += rhs.impl->x;
    self.impl->y += rhs.impl->y;
  } else if constexpr (is_forward_v<Implementation>) {
    self.impl->vector += rhs.impl->vector;
  } else {
    static_assert(false_type_v<Implementation>, "Implementation is missing operator+=.");
  }

  return self;
}

template <typename Vector>
Vector& VectorBase<Vector>::operator-=(const Vector& rhs) {
  return *this += -rhs;
}

template <typename Vector>
Vector VectorBase<Vector>::operator-() const noexcept {
  using Implementation = typename Vector::Implementation;
  const Vector& self = static_cast<const Vector&>(*this);

  if constexpr (has_unary_negate<Implementation>) {
    return -*self.impl;
  } else if constexpr (is_cartesian_v<Implementation>) {
    return Implementation::make(-self.impl->x, -self.impl->y);
  } else if constexpr (is_forward_v<Implementation>) {
    return Implementation::make(-self.impl->vector);
  } else {
    static_assert(false_type_v<Implementation>, "Implementation is missing operator-.");
  }
}

template <typename Vector>
VectorBase<Vector>::operator flatsurf::Vector<exactreal::Arb>() const noexcept {
  using Implementation = typename Vector::Implementation;
  const Vector& self = static_cast<const Vector&>(*this);

  if constexpr (std::is_same_v<Vector, flatsurf::Vector<exactreal::Arb>>) {
    return self;
  } else if constexpr (has_vector_arb<Implementation>) {
    return static_cast<flatsurf::Vector<exactreal::Arb>>(*self.impl);
  } else if constexpr (is_forward_v<Implementation>) {
    return static_cast<flatsurf::Vector<exactreal::Arb>>(self.impl->vector);
  } else if constexpr (is_cartesian_v<Implementation>) {
    return flatsurf::Vector<exactreal::Arb>(exactreal::Arb(self.impl->x), exactreal::Arb(self.impl->y));
  } else {
    static_assert(false_type_v<Implementation>, "Implementation is missing operator<Vector<Arb>>.");
  }
}

template <typename Vector>
VectorBase<Vector>::operator std::complex<double>() const noexcept {
  const Vector& self = static_cast<const Vector&>(*this);

  if constexpr (std::is_same_v<Vector, flatsurf::Vector<exactreal::Arb>>) {
    return std::complex<double>(static_cast<double>(self.impl->x), static_cast<double>(self.impl->y));
  } else {
    return static_cast<std::complex<double>>(static_cast<flatsurf::Vector<exactreal::Arb>>(self));
  }
}

template <typename Vector>
Vector& VectorBase<Vector>::operator*=(const int c) {
  using Implementation = typename Vector::Implementation;
  Vector& self = static_cast<Vector&>(*this);

  if constexpr (has_binary_inplace_times_int<Implementation>) {
    *self.impl *= c;
  } else if constexpr (is_cartesian_v<Implementation>) {
    self.impl->x *= c;
    self.impl->y *= c;
  } else if constexpr (is_forward_v<Implementation>) {
    self.impl->vector *= c;
  } else {
    static_assert(false_type_v<Implementation>, "Implementation is missing operator*=.");
  }

  return self;
}

template <typename Vector>
Vector& VectorBase<Vector>::operator*=(const mpz_class& c) {
  using Implementation = typename Vector::Implementation;
  Vector& self = static_cast<Vector&>(*this);

  if constexpr (has_binary_inplace_times_mpz<Implementation>) {
    *self.impl *= c;
  } else if constexpr (is_cartesian_v<Implementation>) {
    self.impl->x *= c;
    self.impl->y *= c;
  } else if constexpr (is_forward_v<Implementation>) {
    self.impl->vector *= c;
  } else {
    static_assert(false_type_v<Implementation>, "Implementation is missing operator*=.");
  }

  return self;
}

template <typename Vector>
Vector VectorBase<Vector>::perpendicular() const {
  using Implementation = typename Vector::Implementation;
  const Vector& self = static_cast<const Vector&>(*this);

  if constexpr (has_perpendicular<Implementation>) {
    return self.impl->perpendicular();
  } else if constexpr (is_cartesian_v<Implementation>) {
    return Implementation::make(-self.impl->y, self.impl->x);
  } else {
    static_assert(false_type_v<Implementation>, "Implementation is missing perpendicular().");
  }
}

}  // namespace flatsurf

#endif
