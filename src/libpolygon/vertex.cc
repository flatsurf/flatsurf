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

#include "libpolygon/flat_triangulation_combinatorial.hpp"
#include "libpolygon/half_edge.hpp"
#include "libpolygon/vertex.hpp"

using std::ostream;

namespace flatsurf {
Vertex::Vertex(const HalfEdge& canonical) : representative(canonical) {}

Vertex Vertex::source(const HalfEdge& e,
                      const FlatTriangulationCombinatorial& surface) {
  HalfEdge best = e;
  for (HalfEdge test = surface.nextAtVertex(e); test != e;
       test = surface.nextAtVertex(test)) {
    if (test.id < best.id) {
      best = test;
    }
  }
  return Vertex(best);
}

Vertex Vertex::target(const HalfEdge& e,
                      const FlatTriangulationCombinatorial& surface) {
  return Vertex::source(-e, surface);
}

bool Vertex::operator==(const Vertex& v) const {
  return v.representative == this->representative;
}

ostream& operator<<(ostream& os, const Vertex& self) {
  return os << "Vertex(" << self.representative << ")";
}
}  // namespace flatsurf
