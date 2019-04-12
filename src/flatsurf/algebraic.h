/**********************************************************************
 *  This file is part of Flatsurf.
 *
 *        Copyright (C) 2018 Alex Eskin
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
 *  along with Flatsurf. If not, see <https://www.gnu.org/licenses/>.
 *********************************************************************/

#ifndef FLATSURF_ALGEBRAIC_H
#define FLATSURF_ALGEBRAIC_H

#include <cmath>
#include <complex>
#include <iostream>
#include <vector>

#include <e-antic/renfxx_fwd.h>
#include <boost/numeric/conversion/cast.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/ublas/matrix.hpp>

#include "./defs.h"
#include "./poly.h"

namespace polygon {
template <typename T>
class alg_t;

template <typename T>
class NumberField;

template <typename T>
algebraic<T> operator*(algebraic<T>, T);
template <typename T>
algebraic<T> operator*(T, algebraic<T>);
template <typename T>
algebraic<T> operator-(const algebraic<T> &r);
template <typename T>
std::ostream &operator<<(std::ostream &outputStream, const algebraic<T> &num);
template <typename T>
bool operator==(const algebraic<T> &, const algebraic<T> &);
template <typename T>
bool operator!=(const algebraic<T> &, const algebraic<T> &);

template <class T>
algebraic<T> cross_product(const algebraic<T> &u, const algebraic<T> &v);

template <typename T>
class algebraic {
 public:
  algebraic();
  explicit algebraic(NumberField<T> *);  // zero element of NumberField

  algebraic(size_t n,
            NumberField<T> *);  // standard basis element e_n in NumberField
  algebraic(std::vector<T> coords,
            NumberField<T> *);  // vector with coeffients coords in NumberField
  algebraic(T coords[], NumberField<T> *);
  std::complex<COORD> tocomplex() const;
  algebraic<T> conjugate() const;  // the complex conjugate
  algebraic<T> norm()
      const;  // just the number times its complex conjugate, not the real norm

  algebraic<T> pow(int n) const;  // nth power
  inline algebraic<T> pow(size_t n) const {
    return pow(boost::numeric_cast<int>(n));
  }  // nth power

  algebraic<T> &operator+=(const algebraic<T> &);
  algebraic<T> &operator*=(const algebraic<T> &);
  algebraic<T> &operator-=(const algebraic<T> &);
  algebraic<T> &operator/=(const algebraic<T> &);
  algebraic<T> &operator*=(T);
  algebraic<T> &operator/=(T);

  algebraic<T> reciprocal() const;

  friend algebraic<T> operator*<>(algebraic<T>, T);
  friend algebraic<T> operator*<>(T, algebraic<T>);
  friend algebraic<T> operator-<>(const algebraic<T> &r);
  // friend  algebraic<T> operator ^(algebraic<T>, int);
  friend std::ostream &operator<<<>(std::ostream &outputStream,
                                    const algebraic<T> &num);
  friend bool operator==<>(const algebraic<T> &, const algebraic<T> &);
  friend bool operator!=<>(const algebraic<T> &, const algebraic<T> &);

  friend algebraic<bigrat> to_rational(const algebraic<int64_t> &p);
  friend algebraic<T> cross_product<>(const algebraic<T> &u,
                                      const algebraic<T> &v);

  eantic::renf_elem_class real() const;
  eantic::renf_elem_class imag() const;

  friend class NumberField<T>;
  NumberField<T> *field() const;

  std::vector<T> get_coords() { return coords; }

 private:
  std::vector<T> coords;
  NumberField<T> *in_field;
};

template <typename T>
algebraic<T> operator+(algebraic<T>, const algebraic<T> &);
template <typename T>
algebraic<T> operator*(algebraic<T>, const algebraic<T> &);
template <typename T>
algebraic<T> operator-(algebraic<T>, const algebraic<T> &);
template <typename T>
algebraic<T> operator/(algebraic<T>, const algebraic<T> &);

// what are the next 3 for?
algebraic<bigrat> operator/(const algebraic<bigrat> &, int);
algebraic<bigrat> operator*(const algebraic<bigrat> &, int);
algebraic<bigrat> operator*(int, const algebraic<bigrat> &);

template <typename T>
algebraic<T> operator*(algebraic<T>, const algebraic<T> &);

template <class T>
algebraic<T> operator/(algebraic<T> a, const algebraic<T> &b);

template <class T>
algebraic<T> operator/(algebraic<T> a, T b);

template <class T>
algebraic<T> operator*(algebraic<T> a, const algebraic<T> &b);

template <class T>
algebraic<T> operator*(T a, algebraic<T> b);

template <class T>
algebraic<T> operator*(algebraic<T> b, T a);

template <class T>
algebraic<T> operator/(algebraic<T> a, T b);
}  // namespace polygon

#endif  // FLATSURF_ALGEBRAIC_H
