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

#ifndef LIBPOLYGON_DEFS_H
#define LIBPOLYGON_DEFS_H

#include <complex>
#include <string>
#include "libpolygon.h"

// RETURN HERE
#ifdef i686
//#define USE_LONG_DOUBLE
//#define USE_QUAD
#endif

#ifdef x86_64
//#define USE_LONG_DOUBLE
//#define USE_QUAD
#endif

// FIX LATER!!!!
#ifdef sun4m
#define my_random rand
#define RANDOM_MAX 32767
#else
#define my_random random
#endif
#ifndef RANDOM_MAX
#define RANDOM_MAX 2147483647
#endif

#ifdef USE_QUAD
#include <boost/multiprecision/float128.hpp>
//#include <boost/multiprecision/number.hpp>
using namespace boost::multiprecision;
#define COORD float128
#define MY_PI 3.1415926535897932384626433832795028841971693993751058Q
#elif defined USE_LONG_DOUBLE
// typedef long double COORD;
#define COORD long double
#define MY_PI 3.1415926535897932384626433832795029L
#else
// typedef double COORD;
#define COORD double
#define MY_PI M_PI
#endif

// change later
typedef std::complex<COORD> Point;

#ifdef USE_MPI
#define USE_PARALLEL
#endif

#ifdef USE_PVM
#define USE_PARALLEL
#endif

#ifdef USE_MPI
#include <iostream.h>
#include <mpi++.h>
#endif

#ifdef USE_PVM
extern "C" {
#include <pvm3.h>
}
#endif

void ERR_RET(const std::string &s);
void ERR_RET2(const std::string &s, int t);

#endif  // LIBPOLYGON_DEFS_H
