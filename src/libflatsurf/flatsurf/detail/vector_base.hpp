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

#ifndef LIBFLATSURF_DETAIL_VECTOR_BASE_HPP
#define LIBFLATSURF_DETAIL_VECTOR_BASE_HPP

#include <gmpxx.h>
#include <boost/operators.hpp>
#include <complex>
#include <exact-real/forward.hpp>
#include <iosfwd>

#include "flatsurf/forward.hpp"

namespace flatsurf::detail {
// Shared base class for vectors in ℝ².
template <typename Vector>
class VectorBase : private boost::additive<Vector>,
                   private boost::multipliable<Vector, int> {
 public:
  template <typename V>
  friend std::ostream &operator<<(std::ostream &, const VectorBase<V> &);
  Vector operator-() const noexcept;
  Vector &operator+=(const Vector &);
  Vector &operator-=(const Vector &);
  Vector &operator*=(const int);
  Vector &operator*=(const mpz_class &);

  // Return the perpendicular vector in CCW direction
  // to this vector.
  Vector perpendicular() const;

  // See https://github.com/flatsurf/flatsurf/issues/66
  explicit operator std::complex<double>() const noexcept;
  explicit operator flatsurf::Vector<exactreal::Arb>() const noexcept;
};

template <typename V>
std::ostream &operator<<(std::ostream &, const VectorBase<V> &);
}  // namespace flatsurf::detail

#endif
