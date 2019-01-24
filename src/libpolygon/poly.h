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

#ifndef LIBPOLYGON_POLY_H
#define LIBPOLYGON_POLY_H

#include <ostream>
#include <vector>

namespace polygon {
template <typename T>
class poly;

template <typename T>
class algebraic;

template <typename T>
void divide(poly<T> &quotient, poly<T> &remainder, const poly<T> &f,
            const poly<T> &g);  // f divided by g.
template <typename T>
poly<T> operator*(poly<T>, const poly<T> &);
template <typename T>
std::ostream &operator<<(std::ostream &outputStream, const poly<T> &poly);
template <typename T>
bool operator==(const poly<T> &, const poly<T> &);
template <typename T>
bool operator!=(const poly<T> &, const poly<T> &);

template <typename T>
void gcd_extended(
    poly<T> &u, poly<T> &v, const poly<T> &p,
    const poly<T> &q);  // finds u and v such that up+vq = gcd(p, q)
template <typename T>
poly<T> operator+(poly<T>, const poly<T> &);
template <typename T>
poly<T> operator-(poly<T>, const poly<T> &);
template <typename T>
poly<T> operator*(T, poly<T>);
template <typename T>
poly<T> operator%(poly<T>, const poly<T> &);

template <class T>
bool is_unit(const poly<T>);

template <typename T>
class poly {
 public:
  poly();                           // zero
  explicit poly(std::vector<T> a);  // a_0 + a_1 x + ... + a_n x^n
  poly(T coefficients[], int degree);
  int degree() const;
  T leading_coefficient() const;

  friend void divide<>(poly<T> &quotient, poly<T> &remainder, const poly<T> &f,
                       const poly<T> &g);  // f divided by g

  void print();  // for debugging: prints coefficients

  poly<T> &operator+=(const poly<T> &);
  poly<T> &operator-=(const poly<T> &);
  poly<T> &operator*=(T);
  friend poly<T> operator*<>(poly<T>, const poly<T> &);
  algebraic<T> operator()(const algebraic<T> &);

  friend std::ostream &operator<<<>(std::ostream &outputStream,
                                    const poly<T> &poly);
  friend bool operator==<>(const poly<T> &, const poly<T> &);
  friend bool operator!=<>(const poly<T> &, const poly<T> &);

  std::vector<T> coefficients;
  void reduce();          // remove trailing zero coefficients
  void fill_back(int n);  // add trailing zero coefficients until there are at
                          // least n coefficients
};

template <typename T>
poly<T> cyclotomic_poly(int n);

template <class T>
poly<T> operator%(const poly<T> &p, const poly<T> &q);

template <class T>
void gcd_extended(poly<T> &x, poly<T> &y, const poly<T> &a, const poly<T> &b);
}  // namespace polygon
#endif
