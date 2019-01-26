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

#ifndef LIBPOLYGON_NUMBER_FIELD_H
#define LIBPOLYGON_NUMBER_FIELD_H

#include <complex>
#include <ostream>
#include <vector>

#include "algebraic.h"
#include "defs.h"

namespace polygon {
template <typename T>
class NumberField {
 public:
  NumberField<T>();
  NumberField<T>(T minimal_poly[], size_t degree);
  NumberField<T>(T minimal_poly[], size_t degree, std::complex<COORD> embedding);

  void store_conjugate(algebraic<T>);

  /*
      inline algebraic<T> zero() { return algebraic<T>(this); } //0 in F
      inline algebraic<T> one() { return algebraic<T>(0,this); } //1 in F
  */
  friend class algebraic<T>;
  void print_mt();  // for debugging: prints multiplication table
  void print(std::ostream &);

  // return here for more const stuff
  friend algebraic<T> operator*<>(algebraic<T>, const algebraic<T> &);
  friend algebraic<T> cross_product<>(const algebraic<T> &u,
                                      const algebraic<T> &v);

  poly<T> min_poly();
  static NumberField<T> *F;    // everybody is in this field.
  static algebraic<T> F_zero;  // 0 in F
  static algebraic<T> F_one;   // 1 in F

  friend algebraic<bigrat> to_rational(
      const algebraic<int64_t> &p);  // only works with given F

 private:
  algebraic<T> conjugate;         // a conjugate of generator of field.  Used by
                                  // algebraic<T>::conjugate().
  std::complex<COORD> embedding;  // numerical value for generator of field.
                                  // Used by algebraic<T>::to_complex().
  void build_multiplication_table();
  void build_cross_product_table();
  poly<T> minimal_poly;  // minimal polynomial of field.  {a_0, ..., a_n}
                         // represents a_0 + ... + a_n x^n
  std::vector<std::vector<T> > multiplication_table;
  size_t degree;
  std::vector<algebraic<T> > conjugate_table;
  boost::numeric::ublas::matrix<algebraic<T> > cross_product_table;
};

template <typename T>
NumberField<T> *NumberField<T>::F;

template <typename T>
algebraic<T> NumberField<T>::F_zero;  // 0 in F
template <typename T>
algebraic<T> NumberField<T>::F_one;  // 1 in F
template <typename T>
NumberField<T> *InitCyclotomic(int n);
}  // namespace polygon

#endif  // LIBPOLYGON_NUMBER_FIELD_H
