/**********************************************************************
 *  This file is part of Polygon.
 *
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

#ifndef LIBPOLYGON_CCW_HPP
#define LIBPOLYGON_CCW_HPP

#include <boost/operators.hpp>
#include "external/spimpl/spimpl.h"

#include "libpolygon/libpolygon.hpp"

namespace flatsurf {
	enum class CCW {
		COUNTERCLOCKWISE = -1,
		COLLINEAR = 0,
		CLOCKWISE = 1,
	 };

	CCW operator-(CCW ccw);
}

#endif
