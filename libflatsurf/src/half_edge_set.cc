/**********************************************************************
 *  This file is part of flatsurf.
 *
 *        Copyright (C) 2020 Julian Rüth
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

#include <fmt/format.h>

#include "../flatsurf/half_edge_set.hpp"
#include "../flatsurf/half_edge.hpp"
#include "../flatsurf/fmt.hpp"
#include "../flatsurf/half_edge_set_iterator.hpp"
#include "../flatsurf/flat_triangulation_combinatorial.hpp"

#include "impl/half_edge_set.impl.hpp"
#include "impl/half_edge_set_iterator.impl.hpp"

#include "util/assert.ipp"

namespace flatsurf {

template <typename ...Args>
HalfEdgeSet::HalfEdgeSet(PrivateConstructor, Args&&...args) :
  impl(spimpl::make_impl<Implementation>(std::forward<Args>(args)...)) {
}

HalfEdgeSet::HalfEdgeSet(const std::vector<HalfEdge>& items) :
  HalfEdgeSet(PrivateConstructor{}) {
  for (const HalfEdge item : items)
    insert(item);
}

bool HalfEdgeSet::contains(HalfEdge e) const {
  ASSERT(e != HalfEdge(), "cannot check containment of invalid HalfEdge(0)");
  if (impl->set.size() <= e.index())
    return false;
  return impl->set.test(e.index());
}

void HalfEdgeSet::insert(HalfEdge e) {
  ASSERT(e != HalfEdge(), "cannot insert invalid HalfEdge(0)");
  if (impl->set.size() <= e.index())
    impl->set.resize(e.index() + 1);
  impl->set.set(e.index());
}

bool HalfEdgeSet::operator==(const HalfEdgeSet& rhs) const {
  impl->set.resize(std::max(impl->set.size(), rhs.impl->set.size()));
  rhs.impl->set.resize(std::max(impl->set.size(), rhs.impl->set.size()));

  return impl->set == rhs.impl->set;
}

bool HalfEdgeSet::disjoint(const HalfEdgeSet& rhs) const {
  impl->set.resize(std::max(impl->set.size(), rhs.impl->set.size()));
  rhs.impl->set.resize(std::max(impl->set.size(), rhs.impl->set.size()));

  return not impl->set.intersects(rhs.impl->set);
}

bool HalfEdgeSet::empty() const {
  return begin() == end();
}

size_t HalfEdgeSet::size() const {
  return impl->set.count();
}

void HalfEdgeSet::erase(HalfEdge e) {
  ASSERT(e != HalfEdge(), "cannot remove invalid HalfEdge(0)");
  if (impl->set.size() <= e.index()) return;
  impl->set.reset(e.index());
}

HalfEdgeSetIterator begin(const HalfEdgeSet& self) { return self.begin(); }

HalfEdgeSetIterator end(const HalfEdgeSet& self) { return self.end(); }

std::ostream& operator<<(std::ostream& os, const HalfEdgeSet& self) {
  return os << fmt::format("{{{}}}", fmt::join(begin(self), end(self), ", "));
}

}
