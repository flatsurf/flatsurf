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

#include "../flatsurf/edge_set_iterator.hpp"

#include "../flatsurf/edge.hpp"
#include "../flatsurf/edge_set.hpp"
#include "impl/edge_set.impl.hpp"
#include "impl/edge_set_iterator.impl.hpp"

namespace flatsurf {

void EdgeSetIterator::increment() {
  return self->increment();
}

bool EdgeSetIterator::equal(const EdgeSetIterator& rhs) const {
  return self->equal(*rhs.self);
}

const Edge& EdgeSetIterator::dereference() const {
  return self->dereference();
}

EdgeSetIterator EdgeSet::begin() const {
  return EdgeSetIterator{PrivateConstructor{}, self->begin()};
}

EdgeSetIterator EdgeSet::end() const {
  return EdgeSetIterator{PrivateConstructor{}, self->end()};
}

ImplementationOf<EdgeSetIterator>::ImplementationOf(IndexedSetIterator<Edge>&& self) :
  IndexedSetIterator(std::move(self)) {}

}  // namespace flatsurf
