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

#include "../flatsurf/edge_set.hpp"

#include <fmt/format.h>

#include <ostream>

#include "../flatsurf/edge.hpp"
#include "../flatsurf/edge_set_iterator.hpp"
#include "../flatsurf/flat_triangulation_combinatorial.hpp"
#include "../flatsurf/fmt.hpp"
#include "impl/edge_set.impl.hpp"
#include "util/assert.ipp"

namespace flatsurf {

EdgeSet::EdgeSet() :
  impl(spimpl::make_impl<Implementation>()) {}

EdgeSet::EdgeSet(const std::vector<Edge>& items) :
  impl(spimpl::make_impl<Implementation>(items)) {}

bool EdgeSet::contains(Edge e) const {
  return impl->contains(e);
}

void EdgeSet::insert(Edge e) {
  impl->insert(e);
}

bool EdgeSet::operator==(const EdgeSet& rhs) const {
  return impl == rhs.impl;
}

bool EdgeSet::disjoint(const EdgeSet& rhs) const {
  return impl->disjoint(*rhs.impl);
}

bool EdgeSet::empty() const {
  return impl->empty();
}

size_t EdgeSet::size() const {
  return impl->size();
}

void EdgeSet::erase(Edge e) {
  return impl->erase(e);
}

EdgeSetIterator begin(const EdgeSet& self) { return self.begin(); }

EdgeSetIterator end(const EdgeSet& self) { return self.end(); }

std::ostream& operator<<(std::ostream& os, const EdgeSet& self) {
  return os << fmt::format("{{{}}}", fmt::join(begin(self), end(self), ", "));
}

}  // namespace flatsurf
