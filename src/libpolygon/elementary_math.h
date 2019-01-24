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

#ifndef LIBPOLYGON_MATH_H
#define LIBPOLYGON_MATH_H

#include <complex>

#include "defs.h"

namespace polygon {
std::complex<COORD> exp(std::complex<COORD>, int);

#ifdef USE_QUAD
inline int round_to_int(float128 v) {
  v = v + v.sign() / 2.0;
  return floor(v).convert_to<int>();
}
#elif defined USE_LONG_DOUBLE
inline int round_to_int(long double v) { return (int)rint(v); }
#else
inline int round_to_int(double v) { return (int)rint(v); }
#endif

}  // namespace polygon

#endif
