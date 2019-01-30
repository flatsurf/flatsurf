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

#ifndef LIBPOLYGON_BIGPOINT_H
#define LIBPOLYGON_BIGPOINT_H

#include <ostream>

#include "libpolygon/alg_t.h"
#include "libpolygon/defs.h"

namespace polygon {
template <typename T>
class BigPoint;

template <typename T>
BigPoint<T> operator+(BigPoint<T>, const BigPoint<T>&);
template <typename T>
BigPoint<T> operator-(BigPoint<T>, const BigPoint<T>&);
template <typename T>
BigPoint<T> operator/(BigPoint<T>, const algebraic<T>&);
template <typename T>
BigPoint<T> operator/(BigPoint<T>, int);
template <typename T>
BigPoint<T> operator*(BigPoint<T>, int);
template <typename T>
BigPoint<T> operator*(int, BigPoint<T>);
template <typename T>
BigPoint<T> operator*(BigPoint<T>, const algebraic<T>&);
template <typename T>
BigPoint<T> operator*(const algebraic<T>&, BigPoint<T>);
template <typename T>
std::ostream& operator<<(std::ostream& outputStream, const BigPoint<T>& num);
template <typename T>
bool operator==(const BigPoint<T>&, const BigPoint<T>&);
template <typename T>
bool operator!=(const BigPoint<T>&, const BigPoint<T>&);

template <typename T>
class BigPoint {
 public:
  BigPoint();          // initialize to 0
  BigPoint(int, int);  // only works if int's are 0; initialized to 0.
  explicit BigPoint(alg_t<T> p_alg_t);

  BigPoint<T> operator-();
  BigPoint<T>& operator+=(const BigPoint<T>&);
  BigPoint<T>& operator-=(const BigPoint<T>&);
  BigPoint<T>& operator*=(const algebraic<T>&);
  BigPoint<T>& operator*=(int);
  BigPoint<T>& operator/=(const algebraic<T>&);
  BigPoint<T>& operator/=(int);

  friend BigPoint<T> operator+<>(BigPoint<T>, const BigPoint<T>&);

  friend std::ostream& operator<<<>(std::ostream& outputStream,
                                    const BigPoint<T>& num);
  friend bool operator==<>(const BigPoint<T>&, const BigPoint<T>&);
  friend bool operator!=<>(const BigPoint<T>&, const BigPoint<T>&);

  BigPoint<T> conjugate();

  friend bool colinear<>(const alg_t<T>& u, const alg_t<T>& v);

  void SetCxFromAlgebraic();
  void Check();

  Point cx;
  alg_t<T> algt;
};

template <typename T>
inline Point to_cx(BigPoint<T> bp) {
  return bp.cx;
}

typedef BigPoint<bigrat> BigPointQ;
typedef BigPoint<int64_t> BigPointI;
}  // namespace polygon

#endif  // LIBPOLYGON_BIGPOINT_H
