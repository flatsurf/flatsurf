/**********************************************************************
 *  This file is part of flatsurf.
 *
 *        Copyright (C) 2020 Julian Rüth
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

#include <ostream>

#include "impl/quadratic_polynomial.hpp"

#include <exact-real/yap/arb.hpp>

#include "impl/approximation.hpp"

namespace flatsurf {

template <typename T>
QuadraticPolynomial<T>::QuadraticPolynomial(const T& a, const T& b, const T& c) :
  a(a),
  b(b),
  c(c) {}

template <typename T>
std::optional<exactreal::Arb> QuadraticPolynomial<T>::root(const long prec) const {
  const auto validate = [&](const exactreal::Arb& solution) -> std::optional<exactreal::Arb> {
    auto lt0 = solution < 0;
    auto gt1 = solution > 1;

    if (!lt0) {
      if ((*this)(T()) == 0) return exactreal::Arb();
      return root(2 * prec);
    }
    if (!gt1) {
      if ((*this)(T(1)) == 0) return exactreal::Arb(1);
      return root(2 * prec);
    }
    if (*lt0) return std::nullopt;
    if (*gt1) return std::nullopt;

    return solution;
  };

  const exactreal::Arb a_ = Approximation<T>::arb(a, prec),
                       b_ = Approximation<T>::arb(b, prec),
                       c_ = Approximation<T>::arb(c, prec);

  if (c == 0)
    return exactreal::Arb();

  if (a == 0) {
    if (b == 0)
      return std::nullopt;

    if (c == -b)
      return exactreal::Arb(1);

    exactreal::Arb solution = (-c_ / b_)(prec);
    return validate(solution);
  }

  const T discriminant = b * b - 4 * a * c;
  if (discriminant < 0) {
    return std::nullopt;
  } else if (discriminant == 0) {
    return validate((-b_ / (2 * a_))(prec));
  } else {
    exactreal::Arb sqrt_discriminant = (b_ * b_ - 4 * a_ * c_)(prec);
    arb_sqrt(sqrt_discriminant.arb_t(), sqrt_discriminant.arb_t(), prec);

    const auto t0 = validate(((-b_ + sqrt_discriminant) / (2 * a_))(prec));
    const auto t1 = validate(((-b_ - sqrt_discriminant) / (2 * a_))(prec));

    if (t0 && t1) {
      const auto lt = *t0 < *t1;
      if (!lt.has_value() || *lt)
        return t0;
      return t1;
    } else if (t0)
      return t0;
    else if (t1)
      return t1;
    else
      return std::nullopt;
  }
}

template <typename T>
bool QuadraticPolynomial<T>::operator<(const QuadraticPolynomial<T>& rhs) const {
  // When the two polynomials are just multiples of each other, the critical time is the same.
  if (a * rhs.b == b * rhs.a && a * rhs.c == c * rhs.a && b * rhs.c == c * rhs.b)
    return false;

  // Compute approximate values for the respective critical times t and compare them.
  for (slong prec = exactreal::ARB_PRECISION_FAST;; prec *= 2) {
    const auto t = *this->root(prec);
    const auto s = *rhs.root(prec);

    const auto lt = t < s;
    if (lt)
      return *lt;

    // We computed balls around the roots t and s of the two quadratic
    // equations but they overlappped so we cannot decide which one is
    // first. (Typically, this happens because they are actually equal.)

    // If the quadratic polynomials have no common root, then we can
    // eventually separate the times s and t.

    // If both polynomials are only linear, then we decide whether they
    // have the same root by comparing coefficients: namely if c/b = c'/b'.
    if (!a && !rhs.a) {
      if (c * rhs.b == rhs.c * b)
        return false;
      continue;
    }

    // In the non-linear case, they have a common root iff their
    // resultant is zero.
    if (c * c * rhs.a * rhs.a - b * c * rhs.a * rhs.b + a * c * rhs.b * rhs.b + b * b * rhs.a * rhs.c - 2 * a * c * rhs.a * rhs.c - a * b * rhs.b * rhs.c + a * a * rhs.c * rhs.c != 0)
      continue;

    // When they have a common root, it does not necessarily have to be the
    // root we care about.
    // We already know that not both of their roots can be the same since
    // then the polynomials would be multiples of each other which we
    // checked before. We could now probably compute the gcd of the
    // polynomials and then look at the common linear factor but for all
    // cases that we can imagine it is probably safe to assume that the
    // root that is the same is actually the root that also looks like it's
    // approximately the same, i.e., the two critical times are equal.
    return false;
  }
}

template <typename T>
T QuadraticPolynomial<T>::operator()(const T& t) const {
  if (t == 0) return c;
  if (t == 1) return a + b + c;
  return a * t * t + b * t + c;
}

template <typename T>
bool QuadraticPolynomial<T>::positive() const {
  const T det0 = (*this)(T(0));

  if (det0 <= 0)
    return false;

  const T det1 = (*this)(T(1));
  if (det1 > 0) {
    if (
        // The critical point of a*t^2 + b*t + c is outside [0, 1].
        (a == 0 && (c > -b && c > 0)) ||
        // The critical point is a maximum
        (a < 0) ||
        // The critical point is positive so there can be no root in [0, 1].
        (a > 0 && (-b < 0 || -b > 2 * a || b * b - 4 * a * c < 0))) {
      return true;
    }
  }

  return false;
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const QuadraticPolynomial<T>& self) {
  return os << self.a << " * t^2 + " << self.b << " * t + " << self.c;
}

}  // namespace flatsurf

// Instantiations of templates so implementations are generated for the linker
#include "util/instantiate.ipp"

LIBFLATSURF_INSTANTIATE_MANY_WRAPPED((LIBFLATSURF_INSTANTIATE_WITHOUT_IMPLEMENTATION), QuadraticPolynomial, LIBFLATSURF_REAL_TYPES)
