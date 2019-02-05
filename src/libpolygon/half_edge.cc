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

#include <cassert>
#include <ostream>

#include "libpolygon/half_edge.hpp"
#include "libpolygon/half_edge_map.hpp"
#include "libpolygon/permutation.hpp"

using std::ostream;

namespace flatsurf {
HalfEdge::HalfEdge(const int id) : id(id) { assert(id != 0); }

HalfEdge::HalfEdge() : id(0) {}

HalfEdge HalfEdge::operator-() const {
  assert(id != 0);
  return HalfEdge(-id);
}

HalfEdge& HalfEdge::operator=(const HalfEdge& rhs) {
  this->id = rhs.id;
  assert(id != 0);
  return *this;
}

bool HalfEdge::operator==(const HalfEdge& rhs) const {
  return this->id == rhs.id;
}

bool HalfEdge::operator<(const HalfEdge& rhs) const {
  return this->id < rhs.id;
}

ostream& operator<<(ostream& os, const HalfEdge& self) { return os << self.id; }

template <>
size_t Permutation<HalfEdge>::index(const HalfEdge& e) const {
  return HalfEdgeMap<int>::index(e);
}
}  // namespace flatsurf
