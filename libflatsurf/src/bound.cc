/**********************************************************************
 *  This file is part of flatsurf.
 *
 *        Copyright (C) 2019-2020 Julian Rüth
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

#include "../flatsurf/bound.hpp"

#include <ostream>

#include "../flatsurf/vector.hpp"
#include "external/gmpxxll/gmpxxll/mpz_class.hpp"

namespace flatsurf {

Bound::Bound() noexcept :
  square() {}

Bound::Bound(int x) :
  Bound(mpz_class(x)) {}

Bound::Bound(const mpz_class& x) :
  square(x * x) {}

Bound::Bound(const mpz_class& x, const mpz_class& y) :
  square(x * x + y * y) {}

const mpz_class& Bound::squared() const {
  return square;
}

Bound::operator bool() const {
  return static_cast<bool>(square);
}

bool Bound::operator==(const Bound& rhs) const {
  return square == rhs.square;
}

bool Bound::operator<(const Bound& rhs) const {
  return square < rhs.square;
}

Bound& Bound::operator*=(const mpz_class& c) {
  square *= (c * c);
  return *this;
}

template <typename T>
Bound Bound::lower(const Vector<T>& v) {
  Bound ret;
  auto square = v * v;
  if constexpr (std::is_integral_v<T>)
    ret.square = gmpxxll::mpz_class(square);
  else if constexpr (std::is_same_v<T, mpq_class>)
    ret.square = square.get_num() / square.get_den();
  else if constexpr (std::is_same_v<T, mpz_class>)
    ret.square = square;
  else
    ret.square = square.floor();
  return ret;
}

template <typename T>
Bound Bound::upper(const Vector<T>& v) {
  Bound ret;
  auto square = v * v;
  if constexpr (std::is_integral_v<T>)
    ret.square = gmpxxll::mpz_class(square);
  else if constexpr (std::is_same_v<T, mpq_class>) {
    ret.square = square.get_num() / square.get_den();
    if (square.get_num() % square.get_den() != 0)
      ret.square++;
  } else if constexpr (std::is_same_v<T, mpz_class>)
    ret.square = square;
  else
    ret.square = square.ceil();
  return ret;
}

std::ostream& operator<<(std::ostream& os, const Bound& self) {
  return os << "√(" << self.squared() << ")";
}

}  // namespace flatsurf

#include "util/instantiate.ipp"

#define LIBFLATSURF_INSTANTIATE_BOUND(T)            \
  namespace flatsurf {                              \
  template Bound Bound::lower<T>(const Vector<T>&); \
  template Bound Bound::upper<T>(const Vector<T>&); \
  }

LIBFLATSURF_INSTANTIATE_MANY((LIBFLATSURF_INSTANTIATE_BOUND), LIBFLATSURF_REAL_TYPES)
