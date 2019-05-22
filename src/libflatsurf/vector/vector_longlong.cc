/**********************************************************************
 *  This file is part of flatsurf.
 *
 *        Copyright (C) 2019 Julian Rüth
 *
 *  Flatsurf is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 of the License, or
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

#include <ostream>

#include "flatsurf/bound.hpp"
#include "flatsurf/flat_triangulation.hpp"
#include "flatsurf/half_edge.hpp"
#include "flatsurf/vector_longlong.hpp"

using std::ostream;

namespace flatsurf {
VectorLongLong::VectorLongLong() : VectorLongLong(0ll, 0ll) {}
VectorLongLong::VectorLongLong(const long long x, const long long y)
    : cx(x), cy(y) {}

VectorLongLong VectorLongLong::operator-() const {
  return VectorLongLong(-cx, -cy);
}

VectorLongLong &VectorLongLong::operator+=(const VectorLongLong &rhs) {
  cx += rhs.cx;
  cy += rhs.cy;
  return *this;
}

VectorLongLong &VectorLongLong::operator-=(const VectorLongLong &rhs) {
  cx -= rhs.cx;
  cy -= rhs.cy;
  return *this;
}
bool VectorLongLong::operator>(const Bound bound) const {
  return cx * cx + cy * cy > bound.squared;
}
bool VectorLongLong::operator<(const Bound bound) const {
  return cx * cx + cy * cy < bound.squared;
}

bool VectorLongLong::operator==(const VectorLongLong &rhs) const {
  return cx == rhs.cx && cy == rhs.cy;
}

VectorLongLong &VectorLongLong::operator*=(const int rhs) {
  cx *= rhs;
  cy *= rhs;
  return *this;
}

VectorLongLong::operator bool() const { return cx != 0 || cy != 0; }

long long VectorLongLong::x() const { return cx; }

long long VectorLongLong::y() const { return cy; }

CCW VectorLongLong::ccw(const VectorLongLong &other) const {
  const auto a = cx * other.cy;
  const auto b = cy * other.cx;

  if (a > b) {
    return CCW::COUNTERCLOCKWISE;
  } else if (a < b) {
    return CCW::CLOCKWISE;
  } else {
    return CCW::COLLINEAR;
  }
}

ORIENTATION VectorLongLong::orientation(const VectorLongLong &rhs) const {
  auto dot = cx * rhs.cx + cy * rhs.cy;
  if (dot > 0) {
    return ORIENTATION::SAME;
  } else if (dot == 0) {
    return ORIENTATION::ORTHOGONAL;
  } else {
    return ORIENTATION::OPPOSITE;
  }
}

ostream &operator<<(ostream &os, const VectorLongLong &self) {
  return os << "(" << self.cx << "," << self.cy << ")";
}
}  // namespace flatsurf
