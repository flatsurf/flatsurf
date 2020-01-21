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

#include "../flatsurf/edge.hpp"
#include "../flatsurf/half_edge.hpp"

namespace flatsurf {

Edge::Edge() : Edge(HalfEdge()) {}

Edge::Edge(int id) : Edge(HalfEdge(id)) {}

Edge::Edge(HalfEdge e) : id(e.id >= (-e).id ? e : -e) {}

HalfEdge Edge::positive() const { return id; }

HalfEdge Edge::negative() const { return -id; }

size_t Edge::index() const {
  assert(id.id != 0 && "Edge 0 is not a valid edge");
  assert(id.id > 0 && "Edge should have been automatically set to its positive HalfEdge representative");
  return static_cast<size_t>(id.id) - 1;
}

bool Edge::operator==(const Edge& rhs) const {
  return id == rhs.id;
}

bool Edge::operator<(const Edge& rhs) const {
  return index() < rhs.index();
}

std::ostream& operator<<(std::ostream& os, const Edge& e) {
  return os << e.id;
}

}  // namespace flatsurf
