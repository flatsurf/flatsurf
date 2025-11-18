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

#include <gmpxxll/mpz_class.hpp>
#include <ostream>

#include "../flatsurf/vector.hpp"

namespace flatsurf {

Bound::Bound() noexcept :
  square() {}

Bound::Bound(char x) :
  Bound(mpz_class(x)) {}

Bound::Bound(unsigned char x) :
  Bound(mpz_class(x)) {}

Bound::Bound(short x) :
  Bound(mpz_class(x)) {}

Bound::Bound(unsigned short x) :
  Bound(mpz_class(x)) {}

Bound::Bound(int x) :
  Bound(mpz_class(x)) {}

Bound::Bound(unsigned int x) :
  Bound(mpz_class(x)) {}

Bound::Bound(long x) :
  Bound(mpz_class(x)) {}

Bound::Bound(unsigned long x) :
  Bound(mpz_class(x)) {}

Bound::Bound(long long x) :
  Bound(gmpxxll::mpz_class(x)) {}

Bound::Bound(unsigned long long x) :
  Bound(gmpxxll::mpz_class(x)) {}

Bound::Bound(const mpz_class& x) :
  square(x * x) {}

Bound::Bound(char x, const mpz_class& y):
  Bound(mpz_class(x), y) {}

Bound::Bound(char x, char y):
  Bound(mpz_class(x), mpz_class(y)) {}

Bound::Bound(char x, unsigned char y):
  Bound(mpz_class(x), mpz_class(y)) {}

Bound::Bound(char x, short y):
  Bound(mpz_class(x), mpz_class(y)) {}

Bound::Bound(char x, unsigned short y):
  Bound(mpz_class(x), mpz_class(y)) {}

Bound::Bound(char x, int y):
  Bound(mpz_class(x), mpz_class(y)) {}

Bound::Bound(char x, unsigned int y):
  Bound(mpz_class(x), mpz_class(y)) {}

Bound::Bound(char x, long y):
  Bound(mpz_class(x), mpz_class(y)) {}

Bound::Bound(char x, unsigned long y):
  Bound(mpz_class(x), mpz_class(y)) {}

Bound::Bound(char x, long long y):
  Bound(mpz_class(x), gmpxxll::mpz_class(y)) {}

Bound::Bound(char x, unsigned long long y):
  Bound(mpz_class(x), gmpxxll::mpz_class(y)) {}

Bound::Bound(unsigned char x, const mpz_class& y):
  Bound(mpz_class(x), y) {}

Bound::Bound(unsigned char x, char y):
  Bound(mpz_class(x), mpz_class(y)) {}

Bound::Bound(unsigned char x, unsigned char y):
  Bound(mpz_class(x), mpz_class(y)) {}

Bound::Bound(unsigned char x, short y):
  Bound(mpz_class(x), mpz_class(y)) {}

Bound::Bound(unsigned char x, unsigned short y):
  Bound(mpz_class(x), mpz_class(y)) {}

Bound::Bound(unsigned char x, int y):
  Bound(mpz_class(x), mpz_class(y)) {}

Bound::Bound(unsigned char x, unsigned int y):
  Bound(mpz_class(x), mpz_class(y)) {}

Bound::Bound(unsigned char x, long y):
  Bound(mpz_class(x), mpz_class(y)) {}

Bound::Bound(unsigned char x, unsigned long y):
  Bound(mpz_class(x), mpz_class(y)) {}

Bound::Bound(unsigned char x, long long y):
  Bound(mpz_class(x), gmpxxll::mpz_class(y)) {}

Bound::Bound(unsigned char x, unsigned long long y):
  Bound(mpz_class(x), gmpxxll::mpz_class(y)) {}

Bound::Bound(short x, const mpz_class& y):
  Bound(mpz_class(x), y) {}

Bound::Bound(short x, char y):
  Bound(mpz_class(x), mpz_class(y)) {}

Bound::Bound(short x, unsigned char y):
  Bound(mpz_class(x), mpz_class(y)) {}

Bound::Bound(short x, short y):
  Bound(mpz_class(x), mpz_class(y)) {}

Bound::Bound(short x, unsigned short y):
  Bound(mpz_class(x), mpz_class(y)) {}

Bound::Bound(short x, int y):
  Bound(mpz_class(x), mpz_class(y)) {}

Bound::Bound(short x, unsigned int y):
  Bound(mpz_class(x), mpz_class(y)) {}

Bound::Bound(short x, long y):
  Bound(mpz_class(x), mpz_class(y)) {}

Bound::Bound(short x, unsigned long y):
  Bound(mpz_class(x), mpz_class(y)) {}

Bound::Bound(short x, long long y):
  Bound(mpz_class(x), gmpxxll::mpz_class(y)) {}

Bound::Bound(short x, unsigned long long y):
  Bound(mpz_class(x), gmpxxll::mpz_class(y)) {}

Bound::Bound(unsigned short x, const mpz_class& y):
  Bound(mpz_class(x), y) {}

Bound::Bound(unsigned short x, char y):
  Bound(mpz_class(x), mpz_class(y)) {}

Bound::Bound(unsigned short x, unsigned char y):
  Bound(mpz_class(x), mpz_class(y)) {}

Bound::Bound(unsigned short x, short y):
  Bound(mpz_class(x), mpz_class(y)) {}

Bound::Bound(unsigned short x, unsigned short y):
  Bound(mpz_class(x), mpz_class(y)) {}

Bound::Bound(unsigned short x, int y):
  Bound(mpz_class(x), mpz_class(y)) {}

Bound::Bound(unsigned short x, unsigned int y):
  Bound(mpz_class(x), mpz_class(y)) {}

Bound::Bound(unsigned short x, long y):
  Bound(mpz_class(x), mpz_class(y)) {}

Bound::Bound(unsigned short x, unsigned long y):
  Bound(mpz_class(x), mpz_class(y)) {}

Bound::Bound(unsigned short x, long long y):
  Bound(mpz_class(x), gmpxxll::mpz_class(y)) {}

Bound::Bound(unsigned short x, unsigned long long y):
  Bound(mpz_class(x), gmpxxll::mpz_class(y)) {}

Bound::Bound(int x, const mpz_class& y):
  Bound(mpz_class(x), y) {}

Bound::Bound(int x, char y):
  Bound(mpz_class(x), mpz_class(y)) {}

Bound::Bound(int x, unsigned char y):
  Bound(mpz_class(x), mpz_class(y)) {}

Bound::Bound(int x, short y):
  Bound(mpz_class(x), mpz_class(y)) {}

Bound::Bound(int x, unsigned short y):
  Bound(mpz_class(x), mpz_class(y)) {}

Bound::Bound(int x, int y):
  Bound(mpz_class(x), mpz_class(y)) {}

Bound::Bound(int x, unsigned int y):
  Bound(mpz_class(x), mpz_class(y)) {}

Bound::Bound(int x, long y):
  Bound(mpz_class(x), mpz_class(y)) {}

Bound::Bound(int x, unsigned long y):
  Bound(mpz_class(x), mpz_class(y)) {}

Bound::Bound(int x, long long y):
  Bound(mpz_class(x), gmpxxll::mpz_class(y)) {}

Bound::Bound(int x, unsigned long long y):
  Bound(mpz_class(x), gmpxxll::mpz_class(y)) {}

Bound::Bound(unsigned int x, const mpz_class& y):
  Bound(mpz_class(x), y) {}

Bound::Bound(unsigned int x, char y):
  Bound(mpz_class(x), mpz_class(y)) {}

Bound::Bound(unsigned int x, unsigned char y):
  Bound(mpz_class(x), mpz_class(y)) {}

Bound::Bound(unsigned int x, short y):
  Bound(mpz_class(x), mpz_class(y)) {}

Bound::Bound(unsigned int x, unsigned short y):
  Bound(mpz_class(x), mpz_class(y)) {}

Bound::Bound(unsigned int x, int y):
  Bound(mpz_class(x), mpz_class(y)) {}

Bound::Bound(unsigned int x, unsigned int y):
  Bound(mpz_class(x), mpz_class(y)) {}

Bound::Bound(unsigned int x, long y):
  Bound(mpz_class(x), mpz_class(y)) {}

Bound::Bound(unsigned int x, unsigned long y):
  Bound(mpz_class(x), mpz_class(y)) {}

Bound::Bound(unsigned int x, long long y):
  Bound(mpz_class(x), gmpxxll::mpz_class(y)) {}

Bound::Bound(unsigned int x, unsigned long long y):
  Bound(mpz_class(x), gmpxxll::mpz_class(y)) {}

Bound::Bound(long x, const mpz_class& y):
  Bound(mpz_class(x), y) {}

Bound::Bound(long x, char y):
  Bound(mpz_class(x), mpz_class(y)) {}

Bound::Bound(long x, unsigned char y):
  Bound(mpz_class(x), mpz_class(y)) {}

Bound::Bound(long x, short y):
  Bound(mpz_class(x), mpz_class(y)) {}

Bound::Bound(long x, unsigned short y):
  Bound(mpz_class(x), mpz_class(y)) {}

Bound::Bound(long x, int y):
  Bound(mpz_class(x), mpz_class(y)) {}

Bound::Bound(long x, unsigned int y):
  Bound(mpz_class(x), mpz_class(y)) {}

Bound::Bound(long x, long y):
  Bound(mpz_class(x), mpz_class(y)) {}

Bound::Bound(long x, unsigned long y):
  Bound(mpz_class(x), mpz_class(y)) {}

Bound::Bound(long x, long long y):
  Bound(mpz_class(x), gmpxxll::mpz_class(y)) {}

Bound::Bound(long x, unsigned long long y):
  Bound(mpz_class(x), gmpxxll::mpz_class(y)) {}

Bound::Bound(unsigned long x, const mpz_class& y):
  Bound(mpz_class(x), y) {}

Bound::Bound(unsigned long x, char y):
  Bound(mpz_class(x), mpz_class(y)) {}

Bound::Bound(unsigned long x, unsigned char y):
  Bound(mpz_class(x), mpz_class(y)) {}

Bound::Bound(unsigned long x, short y):
  Bound(mpz_class(x), mpz_class(y)) {}

Bound::Bound(unsigned long x, unsigned short y):
  Bound(mpz_class(x), mpz_class(y)) {}

Bound::Bound(unsigned long x, int y):
  Bound(mpz_class(x), mpz_class(y)) {}

Bound::Bound(unsigned long x, unsigned int y):
  Bound(mpz_class(x), mpz_class(y)) {}

Bound::Bound(unsigned long x, long y):
  Bound(mpz_class(x), mpz_class(y)) {}

Bound::Bound(unsigned long x, unsigned long y):
  Bound(mpz_class(x), mpz_class(y)) {}

Bound::Bound(unsigned long x, long long y):
  Bound(mpz_class(x), gmpxxll::mpz_class(y)) {}

Bound::Bound(unsigned long x, unsigned long long y):
  Bound(mpz_class(x), gmpxxll::mpz_class(y)) {}

Bound::Bound(long long x, const mpz_class& y):
  Bound(gmpxxll::mpz_class(x), y) {}

Bound::Bound(long long x, char y):
  Bound(gmpxxll::mpz_class(x), mpz_class(y)) {}

Bound::Bound(long long x, unsigned char y):
  Bound(gmpxxll::mpz_class(x), mpz_class(y)) {}

Bound::Bound(long long x, short y):
  Bound(gmpxxll::mpz_class(x), mpz_class(y)) {}

Bound::Bound(long long x, unsigned short y):
  Bound(gmpxxll::mpz_class(x), mpz_class(y)) {}

Bound::Bound(long long x, int y):
  Bound(gmpxxll::mpz_class(x), mpz_class(y)) {}

Bound::Bound(long long x, unsigned int y):
  Bound(gmpxxll::mpz_class(x), mpz_class(y)) {}

Bound::Bound(long long x, long y):
  Bound(gmpxxll::mpz_class(x), mpz_class(y)) {}

Bound::Bound(long long x, unsigned long y):
  Bound(gmpxxll::mpz_class(x), mpz_class(y)) {}

Bound::Bound(long long x, long long y):
  Bound(gmpxxll::mpz_class(x), gmpxxll::mpz_class(y)) {}

Bound::Bound(long long x, unsigned long long y):
  Bound(gmpxxll::mpz_class(x), gmpxxll::mpz_class(y)) {}

Bound::Bound(unsigned long long x, const mpz_class& y):
  Bound(gmpxxll::mpz_class(x), y) {}

Bound::Bound(unsigned long long x, char y):
  Bound(gmpxxll::mpz_class(x), mpz_class(y)) {}

Bound::Bound(unsigned long long x, unsigned char y):
  Bound(gmpxxll::mpz_class(x), mpz_class(y)) {}

Bound::Bound(unsigned long long x, short y):
  Bound(gmpxxll::mpz_class(x), mpz_class(y)) {}

Bound::Bound(unsigned long long x, unsigned short y):
  Bound(gmpxxll::mpz_class(x), mpz_class(y)) {}

Bound::Bound(unsigned long long x, int y):
  Bound(gmpxxll::mpz_class(x), mpz_class(y)) {}

Bound::Bound(unsigned long long x, unsigned int y):
  Bound(gmpxxll::mpz_class(x), mpz_class(y)) {}

Bound::Bound(unsigned long long x, long y):
  Bound(gmpxxll::mpz_class(x), mpz_class(y)) {}

Bound::Bound(unsigned long long x, unsigned long y):
  Bound(gmpxxll::mpz_class(x), mpz_class(y)) {}

Bound::Bound(unsigned long long x, long long y):
  Bound(gmpxxll::mpz_class(x), gmpxxll::mpz_class(y)) {}

Bound::Bound(unsigned long long x, unsigned long long y):
  Bound(gmpxxll::mpz_class(x), gmpxxll::mpz_class(y)) {}

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
