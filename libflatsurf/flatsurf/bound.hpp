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

#ifndef LIBFLATSURF_BOUND_HPP
#define LIBFLATSURF_BOUND_HPP

#include <gmpxx.h>

#include <boost/operators.hpp>
#include <iosfwd>

#include "forward.hpp"

namespace flatsurf {

// A bound for vectors in ℝ², i.e., a ball in ℝ² whose radius is given by
// the length of a vector with integer coordinates.
class Bound : boost::totally_ordered<Bound>, boost::multipliable<Bound, mpz_class> {
 public:
  Bound() noexcept;
  Bound(const mpz_class& x);
  Bound(char x);
  Bound(unsigned char x);
  Bound(short x);
  Bound(unsigned short x);
  Bound(int x);
  Bound(unsigned int x);
  Bound(long x);
  Bound(unsigned long x);
  Bound(long long x);
  Bound(unsigned long long x);
  Bound(char, const mpz_class&);
  Bound(char, char);
  Bound(char, unsigned char);
  Bound(char, short);
  Bound(char, unsigned short);
  Bound(char, int);
  Bound(char, unsigned int);
  Bound(char, long);
  Bound(char, unsigned long);
  Bound(char, long long);
  Bound(char, unsigned long long);
  Bound(unsigned char, const mpz_class&);
  Bound(unsigned char, char);
  Bound(unsigned char, unsigned char);
  Bound(unsigned char, short);
  Bound(unsigned char, unsigned short);
  Bound(unsigned char, int);
  Bound(unsigned char, unsigned int);
  Bound(unsigned char, long);
  Bound(unsigned char, unsigned long);
  Bound(unsigned char, long long);
  Bound(unsigned char, unsigned long long);
  Bound(short, const mpz_class&);
  Bound(short, char);
  Bound(short, unsigned char);
  Bound(short, short);
  Bound(short, unsigned short);
  Bound(short, int);
  Bound(short, unsigned int);
  Bound(short, long);
  Bound(short, unsigned long);
  Bound(short, long long);
  Bound(short, unsigned long long);
  Bound(unsigned short, const mpz_class&);
  Bound(unsigned short, char);
  Bound(unsigned short, unsigned char);
  Bound(unsigned short, short);
  Bound(unsigned short, unsigned short);
  Bound(unsigned short, int);
  Bound(unsigned short, unsigned int);
  Bound(unsigned short, long);
  Bound(unsigned short, unsigned long);
  Bound(unsigned short, long long);
  Bound(unsigned short, unsigned long long);
  Bound(int, const mpz_class&);
  Bound(int, char);
  Bound(int, unsigned char);
  Bound(int, short);
  Bound(int, unsigned short);
  Bound(int, int);
  Bound(int, unsigned int);
  Bound(int, long);
  Bound(int, unsigned long);
  Bound(int, long long);
  Bound(int, unsigned long long);
  Bound(unsigned int, const mpz_class&);
  Bound(unsigned int, char);
  Bound(unsigned int, unsigned char);
  Bound(unsigned int, short);
  Bound(unsigned int, unsigned short);
  Bound(unsigned int, int);
  Bound(unsigned int, unsigned int);
  Bound(unsigned int, long);
  Bound(unsigned int, unsigned long);
  Bound(unsigned int, long long);
  Bound(unsigned int, unsigned long long);
  Bound(long, const mpz_class&);
  Bound(long, char);
  Bound(long, unsigned char);
  Bound(long, short);
  Bound(long, unsigned short);
  Bound(long, int);
  Bound(long, unsigned int);
  Bound(long, long);
  Bound(long, unsigned long);
  Bound(long, long long);
  Bound(long, unsigned long long);
  Bound(unsigned long, const mpz_class&);
  Bound(unsigned long, char);
  Bound(unsigned long, unsigned char);
  Bound(unsigned long, short);
  Bound(unsigned long, unsigned short);
  Bound(unsigned long, int);
  Bound(unsigned long, unsigned int);
  Bound(unsigned long, long);
  Bound(unsigned long, unsigned long);
  Bound(unsigned long, long long);
  Bound(unsigned long, unsigned long long);
  Bound(long long, const mpz_class&);
  Bound(long long, char);
  Bound(long long, unsigned char);
  Bound(long long, short);
  Bound(long long, unsigned short);
  Bound(long long, int);
  Bound(long long, unsigned int);
  Bound(long long, long);
  Bound(long long, unsigned long);
  Bound(long long, long long);
  Bound(long long, unsigned long long);
  Bound(unsigned long long, const mpz_class&);
  Bound(unsigned long long, char);
  Bound(unsigned long long, unsigned char);
  Bound(unsigned long long, short);
  Bound(unsigned long long, unsigned short);
  Bound(unsigned long long, int);
  Bound(unsigned long long, unsigned int);
  Bound(unsigned long long, long);
  Bound(unsigned long long, unsigned long);
  Bound(unsigned long long, long long);
  Bound(unsigned long long, unsigned long long);
  Bound(const mpz_class&, const mpz_class&);
  Bound(const mpz_class&, char);
  Bound(const mpz_class&, unsigned char);
  Bound(const mpz_class&, short);
  Bound(const mpz_class&, unsigned short);
  Bound(const mpz_class&, int);
  Bound(const mpz_class&, unsigned int);
  Bound(const mpz_class&, long);
  Bound(const mpz_class&, unsigned long);
  Bound(const mpz_class&, long long);
  Bound(const mpz_class&, unsigned long long);

  template <typename T>
  static Bound lower(const Vector<T>&);

  template <typename T>
  static Bound upper(const Vector<T>&);

  const mpz_class& squared() const;

  bool operator==(const Bound&) const;
  bool operator<(const Bound&) const;

  Bound& operator*=(const mpz_class&);

  // Return the floor division by this bound.
  mpz_class operator/(const Bound&) const;

  // Return whether this is not the zero bound.
  explicit operator bool() const;

  friend std::ostream& operator<<(std::ostream&, const Bound&);

 private:
  mpz_class square;

  friend cereal::access;

  template <typename Archive>
  void save(Archive&) const;
  template <typename Archive>
  void load(Archive&);
};

}  // namespace flatsurf

#endif
