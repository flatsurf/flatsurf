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

#ifndef LIBFLATSURF_CCW_HPP
#define LIBFLATSURF_CCW_HPP

#include "forward.hpp"

namespace flatsurf {
// The result of deciding whether two vectors in ℝ² are clockwise,
// counterclockwise, or collinear.
enum class CCW {
  COUNTERCLOCKWISE = -1,
  COLLINEAR = 0,
  CLOCKWISE = 1,
};

CCW operator-(CCW ccw);
}  // namespace flatsurf

#endif
