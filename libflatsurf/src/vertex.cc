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
#include <string>

#include <fmt/format.h>

#include "../flatsurf/flat_triangulation_combinatorial.hpp"
#include "../flatsurf/half_edge.hpp"
#include "../flatsurf/vertex.hpp"
#include "../flatsurf/half_edge_set.hpp"
#include "../flatsurf/half_edge_set_iterator.hpp"
#include "../flatsurf/fmt.hpp"

#include "impl/vertex.impl.hpp"

#include "util/assert.ipp"

using std::ostream;
using std::string;

namespace flatsurf {

template <typename ...Args>
Vertex::Vertex(PrivateConstructor, Args&&... args) :
  impl(spimpl::make_impl<Implementation>(std::forward<Args>(args)...)) {
}

bool Vertex::operator==(const Vertex& rhs) const {
  ASSERT(Implementation::comparable(impl->sources, rhs.impl->sources), "Cannot compare unrelated vertices; maybe the surface has changed since these vertices were created?");

  return *begin(impl->sources) == *begin(rhs.impl->sources);
}

const Vertex& Vertex::source(const HalfEdge& e, const FlatTriangulationCombinatorial& surface) {
  return *std::find_if(begin(surface.vertices()), end(surface.vertices()), [&](const auto& v) { return v.impl->sources.contains(e); });
}

const Vertex& Vertex::target(const HalfEdge& e, const FlatTriangulationCombinatorial& surface) {
  return Vertex::source(-e, surface);
}

HalfEdgeSet Vertex::outgoing() const {
  return impl->sources;
}

HalfEdgeSet Vertex::incoming() const {
  HalfEdgeSet incoming({});
  for (auto he : outgoing())
    incoming.insert(-he);
  return incoming;
}

ostream& operator<<(ostream& os, const Vertex& self) {
  return os << fmt::format("{}", self.impl->sources);
}

bool ImplementationOf<Vertex>::comparable(const HalfEdgeSet& a, const HalfEdgeSet& b) {
  return a == b || a.disjoint(b);
}

void ImplementationOf<Vertex>::afterFlip(Vertex& v, const FlatTriangulationCombinatorial& parent, HalfEdge flipped) {
  auto& sources = v.impl->sources;

  HalfEdge b = parent.previousAtVertex(-flipped);
  HalfEdge d = parent.previousAtVertex(flipped);

  if (sources.contains(-b)) sources.erase(flipped);
  if (sources.contains(-d)) sources.erase(-flipped);

  if (sources.contains(b)) sources.insert(-flipped);
  if (sources.contains(d)) sources.insert(flipped);
}

ImplementationOf<Vertex>::ImplementationOf(const HalfEdgeSet& sources) :
  sources(sources) {}

const HalfEdgeSet& ImplementationOf<Vertex>::outgoing(const Vertex& v) {
  return v.impl->sources;
}

Vertex ImplementationOf<Vertex>::make(const std::vector<HalfEdge> sources) {
  return Vertex(PrivateConstructor{}, HalfEdgeSet(std::move(sources)));
}

}  // namespace flatsurf

size_t std::hash<flatsurf::Vertex>::operator()(const flatsurf::Vertex &v) const noexcept {
  return std::hash<flatsurf::HalfEdge>()(*begin(v.impl->sources));
}
