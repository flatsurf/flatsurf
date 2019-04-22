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

#ifndef LIBFLATSURF_VECTOR_ARB_HPP
#define LIBFLATSURF_VECTOR_ARB_HPP

#include <boost/operators.hpp>
#include <complex>
#include <exact-real/arb.hpp>
#include <iosfwd>
#include <optional>

#include "flatsurf/ccw.hpp"
#include "flatsurf/flatsurf.hpp"
#include "flatsurf/orientation.hpp"

namespace flatsurf {
struct Bound;
struct VectorEAntic;

struct VectorArb : boost::additive<VectorArb>,
                   boost::multipliable<VectorArb, int> {
  VectorArb();
  VectorArb(const exactreal::Arb &x, const exactreal::Arb &y);

  std::optional<CCW> ccw(const VectorArb &) const;
  std::optional<ORIENTATION> orientation(const VectorArb &) const;
  bool isExact() const;

  friend std::ostream &operator<<(std::ostream &, const VectorArb &);
  VectorArb operator-() const;
  std::optional<bool> operator>(const Bound bound) const;
  std::optional<bool> operator<(const Bound bound) const;
  VectorArb &operator+=(const VectorArb &rhs);
  VectorArb &operator-=(const VectorArb &rhs);
  VectorArb &operator*=(const int rhs);

  explicit operator std::complex<double>() const;

  // TODO: Drop default precision
  static mp_limb_signed_t prec;

private:
  exactreal::Arb x;
  exactreal::Arb y;
};
} // namespace flatsurf

#endif
