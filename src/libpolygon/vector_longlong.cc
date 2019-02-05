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

#include <ostream>

#include "libpolygon/bound.hpp"
#include "libpolygon/flat_triangulation.hpp"
#include "libpolygon/half_edge.hpp"
#include "libpolygon/vector_longlong.hpp"

using std::ostream;

namespace flatsurf {
VectorLongLong::VectorLongLong() : VectorLongLong(0ll, 0ll) {}
VectorLongLong::VectorLongLong(const long long x, const long long y)
    : x(x), y(y) {}

VectorLongLong VectorLongLong::operator-() const {
  return VectorLongLong(-x, -y);
}

VectorLongLong& VectorLongLong::operator+=(const VectorLongLong& rhs) {
  x += rhs.x;
  y += rhs.y;
  return *this;
}

VectorLongLong& VectorLongLong::operator-=(const VectorLongLong& rhs) {
  x -= rhs.x;
  y -= rhs.y;
  return *this;
}
bool VectorLongLong::operator>(const Bound bound) const {
  return x * x + y * y > bound.squared;
}
bool VectorLongLong::operator<(const Bound bound) const {
  return x * x + y * y < bound.squared;
}
VectorLongLong& VectorLongLong::operator*=(const int rhs) {
  x *= rhs;
  y *= rhs;
  return *this;
}

VectorLongLong::operator bool() const { return x != 0 || y != 0; }

CCW VectorLongLong::ccw(const VectorLongLong& other) const {
  const auto a = x * other.y;
  const auto b = y * other.x;

  if (a > b) {
    return CCW::COUNTERCLOCKWISE;
  } else if (a < b) {
    return CCW::CLOCKWISE;
  } else {
    return CCW::COLLINEAR;
  }
}
ostream& operator<<(ostream& os, const VectorLongLong& self) {
  return os << "(" << self.x << "," << self.y << ")";
}
}  // namespace flatsurf
