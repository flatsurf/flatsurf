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

#include "../flatsurf/half_edge_set_iterator.hpp"
#include "../flatsurf/half_edge.hpp"
#include "../flatsurf/half_edge_set.hpp"

#include "impl/half_edge_set_iterator.impl.hpp"
#include "impl/half_edge_set.impl.hpp"

#include "util/assert.ipp"

namespace flatsurf {

template <typename ...Args>
HalfEdgeSetIterator::HalfEdgeSetIterator(PrivateConstructor, Args&&... args) :
  impl(spimpl::make_impl<Implementation>(std::forward<Args>(args)...)) {
}

void HalfEdgeSetIterator::increment() {
  ASSERT(impl->current.index() != impl->parent->impl->set.size(), "iterator already at end");
  impl->current = HalfEdge::fromIndex(impl->parent->impl->set.find_next(impl->current.index()));
}

bool HalfEdgeSetIterator::equal(const HalfEdgeSetIterator& rhs) const {
  return impl->parent == rhs.impl->parent && impl->current == rhs.impl->current;
}

const HalfEdge& HalfEdgeSetIterator::dereference() const {
  return impl->current;
}

HalfEdgeSetIterator HalfEdgeSet::begin() const {
  return HalfEdgeSetIterator{PrivateConstructor{}, this, HalfEdge::fromIndex(impl->set.find_first())};
}

HalfEdgeSetIterator HalfEdgeSet::end() const {
  return HalfEdgeSetIterator{PrivateConstructor{}, this, HalfEdge::fromIndex(impl->set.size())};
}

}
