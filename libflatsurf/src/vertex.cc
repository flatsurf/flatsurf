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

#include <boost/algorithm/string/join.hpp>
#include <boost/lexical_cast.hpp>

#include "../flatsurf/flat_triangulation_combinatorial.hpp"
#include "../flatsurf/half_edge.hpp"
#include "../flatsurf/vertex.hpp"

#include "impl/vertex.impl.hpp"

using std::ostream;
using std::string;

namespace flatsurf {

using HalfEdges = std::set<HalfEdge>;

namespace {
inline void check(const HalfEdges& a) {
  assert(a.size() && "vertex not created from any surface");
}

inline void check(const HalfEdges& a, const HalfEdges& b) {
  check(a);
  check(b);
  assert((
    (a == b)
    || all_of(a.begin(), a.end(), [&](const auto& e) { return b.find(e) == b.end(); })
    || all_of(b.begin(), b.end(), [&](const auto& e) { return a.find(e) == a.end(); })
  ) && "cannot compare vertices that were created from different surfaces");
}
}

Vertex::Vertex() :
  impl(spimpl::make_impl<Implementation>(HalfEdges())) {
}

bool Vertex::operator==(const Vertex& rhs) const {
  check(impl->sources, rhs.impl->sources);
  return impl->sources == rhs.impl->sources;
}

bool Vertex::operator<(const Vertex& rhs) const {
  check(impl->sources, rhs.impl->sources);
  return impl->sources < rhs.impl->sources;
}

Vertex Vertex::source(const HalfEdge &e, const FlatTriangulationCombinatorial &surface) {
  for (const auto& v : surface.vertices())
    if (v.impl->sources.find(e) != v.impl->sources.end())
      return v;
  assert(false && "half edge not in the surface");
}

Vertex Vertex::target(const HalfEdge &e, const FlatTriangulationCombinatorial &surface) {
  return Vertex::source(-e, surface);
}

Vertex::Implementation::Implementation(const HalfEdges& sources) :
  sources(sources) {
}

ostream& operator<<(ostream& os, const Vertex& self) {
  check(self.impl->sources);
  std::vector<string> items;
  for (auto e : self.impl->sources)
    items.push_back(boost::lexical_cast<string>(e));
  return os << "Vertex(" << boost::algorithm::join(items, ", ") << ")";
}

} 
