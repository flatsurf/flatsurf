/**********************************************************************
 *  This file is part of flatsurf.
 *
 *        Copyright (C) 2019 Julian Rüth
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

#include <ostream>

#include "flatsurf/flat_triangulation_combinatorial.hpp"
#include "flatsurf/half_edge.hpp"
#include "flatsurf/vertex.hpp"

using std::ostream;

namespace flatsurf {
Vertex::Vertex(const HalfEdge &canonical) : representative(canonical) {}

Vertex Vertex::source(const HalfEdge &e,
                      const FlatTriangulationCombinatorial &surface) {
  HalfEdge best = e;
  HalfEdge test = e;
  do {
    if (surface.boundary(-test)) {
      return Vertex(test);
    }
    test = surface.previousAtVertex(test);
    if (test.id < best.id) {
      best = test;
    }
  } while (test != e);
  return Vertex(best);
}

Vertex Vertex::target(const HalfEdge &e,
                      const FlatTriangulationCombinatorial &surface) {
  return Vertex::source(-e, surface);
}

bool Vertex::operator==(const Vertex &v) const {
  return v.representative == this->representative;
}

ostream &operator<<(ostream &os, const Vertex &self) {
  return os << "Vertex(" << self.representative << ")";
}
}  // namespace flatsurf

size_t std::hash<flatsurf::Vertex>::operator()(const flatsurf::Vertex& v) const noexcept {
  return std::hash<flatsurf::HalfEdge>{}(v.representative);
}
