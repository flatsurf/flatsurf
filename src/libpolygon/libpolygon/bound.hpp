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

#ifndef LIBPOLYGON_BOUND_HPP
#define LIBPOLYGON_BOUND_HPP

#include "libpolygon/libpolygon.hpp"

namespace flatsurf {
	// A rough measure for lengths of vectors in R^2.
	// In a type safe wrapper so we do not mix actual lengths and their squares.
	struct Bound {
		explicit Bound(long long lengthSquared);
		const long long squared;
	};
}

#endif
