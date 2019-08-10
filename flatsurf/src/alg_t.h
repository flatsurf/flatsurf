/**********************************************************************
 *  This file is part of flatsurf.
 *
 *        Copyright (C) 2018 Alex Eskin
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

#ifndef FLATSURF_ALG_T_H
#define FLATSURF_ALG_T_H

#include <exact-real/element.hpp>
#include <exact-real/number_field.hpp>
#include <flatsurf/vector.hpp>
#include <ostream>
#include <vector>

#include "./algebraic.h"
#include "./defs.h"

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
class [[deprecated("Use flatsurf VectorExactReal instead.")]] alg_t {
 public:
  alg_t();  // initialize to 0
  explicit alg_t(std::vector<algebraic<T>> coeffs);
  template <typename Ring>
  alg_t(const flatsurf::Vector<exactreal::Element<Ring>>& v);

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

  exactreal::Element<exactreal::NumberField> real() const;
  exactreal::Element<exactreal::NumberField> imag() const;

  friend bool colinear<>(const alg_t<T>& u, const alg_t<T>& v);
  // p1 divided by p2. Only works if it algebraic.
  friend bool is_alg_quotient(const alg_t<int64_t>& p1,
                              const alg_t<int64_t>& p2,
                              algebraic<bigrat>& ratio);

  bool get_direction(
      algebraic<T> & direction);  // is direction in C independent of parameters?
                                  // If yes, return true, and return the
                                  // direction in direction. If no, return false

 private:
  std::vector<algebraic<T>> coeffs;
};

typedef alg_t<bigrat> alg_tQ;
typedef alg_t<int64_t> alg_tI;
typedef algebraic<bigrat> algebraicQ;
typedef algebraic<int64_t> algebraicI;
}  // namespace polygon

#endif  // FLATSURF_ALG_T_H
