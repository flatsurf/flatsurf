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

#include "../flatsurf/edge.hpp"

#include <ostream>

#include "../flatsurf/half_edge.hpp"
#include "util/assert.ipp"

namespace flatsurf {

Edge::Edge() noexcept :
  id(HalfEdge()) {}

Edge::Edge(int id) :
  Edge(HalfEdge(id)) {}

Edge::Edge(HalfEdge e) :
  id(e.index() < (-e).index() ? e : -e) {}

Edge Edge::fromIndex(size_t index) {
  return Edge(HalfEdge(static_cast<int>(index) + 1));
}

HalfEdge Edge::positive() const { return id; }

HalfEdge Edge::negative() const { return -id; }

size_t Edge::index() const {
  LIBFLATSURF_ASSERT(id.index() % 2 == 0, "Edge should have been automatically set to its positive HalfEdge representative");
  return id.index() / 2;
}

bool Edge::operator==(const Edge& rhs) const {
  return id == rhs.id;
}

std::ostream& operator<<(std::ostream& os, const Edge& e) {
  return os << e.id;
}

}  // namespace flatsurf

size_t std::hash<flatsurf::Edge>::operator()(const flatsurf::Edge& e) const {
  return std::hash<flatsurf::HalfEdge>()(e.positive());
}
