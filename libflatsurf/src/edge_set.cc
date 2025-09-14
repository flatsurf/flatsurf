/**********************************************************************
 *  This file is part of flatsurf.
 *
 *        Copyright (C) 2020-2025 Julian Rüth
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
#include <fmt/ranges.h>

#include <ostream>

#include "../flatsurf/edge.hpp"
#include "../flatsurf/edge_set.hpp"
#include "../flatsurf/edge_set_iterator.hpp"
#include "../flatsurf/flat_triangulation_combinatorial.hpp"
#include "../flatsurf/fmt.hpp"
#include "impl/edge_set.impl.hpp"

namespace flatsurf {

using std::begin;
using std::end;

EdgeSet::EdgeSet() noexcept :
  self(spimpl::make_impl<ImplementationOf<EdgeSet>>()) {}

EdgeSet::EdgeSet(const std::vector<Edge>& items) :
  self(spimpl::make_impl<ImplementationOf<EdgeSet>>(items)) {}

bool EdgeSet::contains(Edge e) const {
  return self->contains(e);
}

void EdgeSet::insert(Edge e) {
  self->insert(e);
}

bool EdgeSet::operator==(const EdgeSet& rhs) const {
  return self == rhs.self;
}

bool EdgeSet::disjoint(const EdgeSet& rhs) const {
  return self->disjoint(*rhs.self);
}

bool EdgeSet::empty() const {
  return self->empty();
}

size_t EdgeSet::size() const {
  return self->size();
}

void EdgeSet::erase(Edge e) {
  return self->erase(e);
}

std::ostream& operator<<(std::ostream& os, const EdgeSet& self) {
  return os << fmt::format("{{{}}}", fmt::join(begin(self), end(self), ", "));
}

}  // namespace flatsurf
