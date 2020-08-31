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

#ifndef LIBFLATSURF_QUADRATIC_POLYNOMIAL_HPP
#define LIBFLATSURF_QUADRATIC_POLYNOMIAL_HPP

#include <exact-real/arb.hpp>
#include <optional>

namespace flatsurf {

template <typename T>
class QuadraticPolynomial {
  T a, b, c;

 public:
  QuadraticPolynomial(const T& a, const T& b, const T& c);

  // Return an approximation of the smallest solution of a*t^2 + b*t + c = 0
  // for t in [0, 1]. Returns nothing if there are no roots in [0, 1].
  std::optional<exactreal::Arb> root(slong prec = exactreal::ARB_PRECISION_FAST) const;

  // Return whether the smallest root of a*t^2 + b*t + c in [0, 1] is smaller
  // than the smallest corresponding root of rhs.
  bool operator<(const QuadraticPolynomial& rhs) const;

  // Return whether the polynomial is positive for all t in [0, 1].
  bool positive() const;

  // Return the value of the polynomial at t.
  T operator()(const T& t) const;

  template <typename S>
  friend std::ostream& operator<<(std::ostream&, const QuadraticPolynomial<S>&);
};

}  // namespace flatsurf

#endif
