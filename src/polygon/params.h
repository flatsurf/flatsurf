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

#ifndef POLYGON_PARAMS_H
#define POLYGON_PARAMS_H

#include <exact-real/module.hpp>
#include <exact-real/number_field_traits.hpp>
#include <exact-real/real_number.hpp>
#include <ostream>
#include <vector>

#include "./defs.h"

using exactreal::Module;
using exactreal::NumberFieldTraits;
using exactreal::RealNumber;

namespace polygon {
// A collection of transcendental real numbers used as "random" parameters.
class Params {
 public:
  static void AddParams(size_t n, COORD parameters[]);
  static size_t nbr_params();
  static void print(std::ostream&);

  static size_t n_params;
  static std::vector<COORD> params;

  static std::shared_ptr<const Module<NumberFieldTraits>> module();
  static std::shared_ptr<RealNumber> exact(size_t i);
};
}  // namespace polygon

#endif  // POLYGON_PARAMS_H
