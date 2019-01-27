/**********************************************************************
 *  This file is part of Polygon.
 *
 *        Copyright (C) 2018 Alex Eskin
 *        Copyright (C) 2019 Julian Rüth
 *
 *  Polygon is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  Polygon is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Polygon. If not, see <https://www.gnu.org/licenses/>.
 *********************************************************************/

#ifndef LIBPOLYGON_ALG_T_H
#define LIBPOLYGON_ALG_T_H

#include <ostream>
#include <vector>

#include "algebraic.h"
#include "defs.h"
#include "libpolygon.h"

namespace polygon {
template <typename T>
class alg_t;

template <typename T>
bool colinear(const alg_t<T>& u, const alg_t<T>& v);

template <typename T>
alg_t<T> operator+(alg_t<T>, const alg_t<T>&);
template <typename T>
alg_t<T> operator-(alg_t<T>, const alg_t<T>&);
template <typename T>
alg_t<T> operator/(alg_t<T>&, const algebraic<T>&);
template <typename T>
alg_t<T> operator/(alg_t<T>&, int);
template <typename T>
alg_t<T> operator*(alg_t<T>&, int);
template <typename T>
alg_t<T> operator*(int, alg_t<T>&);
template <typename T>
alg_t<T> operator*(alg_t<T>, const algebraic<T>&);
template <typename T>
alg_t<T> operator*(const algebraic<T>&, alg_t<T>&);
template <typename T>
std::ostream& operator<<(std::ostream& outputStream, const alg_t<T>& num);
template <typename T>
bool operator==(const alg_t<T>&, const alg_t<T>&);
template <typename T>
bool operator!=(const alg_t<T>&, const alg_t<T>&);

template <typename T>
class alg_t {
 public:
  alg_t();  // initialize to 0
  explicit alg_t(std::vector<algebraic<T> > coeffs);

  void pad_coeffs(int n);  // need to call this when increasing nbr_params
  algebraic<T> get_coeff(size_t n);
  void set_coeff(size_t n, const algebraic<T>& a);

  alg_t<T> operator-() const;
  alg_t<T>& operator+=(const alg_t<T>&);
  alg_t<T>& operator-=(const alg_t<T>&);
  alg_t<T>& operator*=(const algebraic<T>&);
  alg_t<T>& operator*=(int);
  alg_t<T>& operator/=(const algebraic<T>&);
  alg_t<T>& operator/=(int);

  friend std::ostream& operator<<<>(std::ostream& outputStream,
                                    const alg_t<T>& num);
  friend bool operator==<>(const alg_t<T>&, const alg_t<T>&);
  friend bool operator!=<>(const alg_t<T>&, const alg_t<T>&);

  Point tocomplex() const;
  alg_t<T> conjugate() const;

  friend bool colinear<>(const alg_t<T>& u, const alg_t<T>& v);
  friend bool is_alg_quotient(const alg_t<int64_t>& p1,
                              const alg_t<int64_t>& p2,
                              algebraic<bigrat>& ratio);
  // p1 divided by p2. Only works if it algebraic

  bool get_direction(
      algebraic<T>& direction);  // is direction in C independent of parameters?
                                 // If yes, return true, and return the
                                 // direction in direction. If no, return false

 private:
  std::vector<algebraic<T> > coeffs;
};

typedef alg_t<bigrat> alg_tQ;
typedef alg_t<int64_t> alg_tI;
typedef algebraic<bigrat> algebraicQ;
typedef algebraic<int64_t> algebraicI;
}  // namespace polygon

#endif  // LIBPOLYGON_ALG_T_H
