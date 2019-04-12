/**********************************************************************
 *  This file is part of Flatsurf.
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
 *  along with Flatsurf. If not, see <https://www.gnu.org/licenses/>.
 *********************************************************************/

#include "libflatsurf/edge.hpp"

namespace flatsurf {
Edge::Edge(size_t id) : id(id) {}

std::pair<HalfEdge, HalfEdge> Edge::halfEdges() const {
  const int halfEdgeId = static_cast<int>(id) + 1;
  return std::make_pair(HalfEdge(halfEdgeId), HalfEdge(-halfEdgeId));
}

std::vector<Edge> Edge::makeEdges(size_t nedges) {
  std::vector<Edge> ret;
  while (ret.size() < nedges) {
    ret.push_back(Edge(ret.size()));
  }
  return ret;
}
}  // namespace flatsurf
