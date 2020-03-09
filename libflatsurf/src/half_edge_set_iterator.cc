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
  ASSERT(*this != impl->parent->end(), "iterator already at end");
  impl->current = Implementation::makeHalfEdge(impl->parent, impl->parent->impl->set.find_next(impl->current.index()));
}

bool HalfEdgeSetIterator::equal(const HalfEdgeSetIterator& rhs) const {
  return impl->parent == rhs.impl->parent && impl->current == rhs.impl->current;
}

const HalfEdge& HalfEdgeSetIterator::dereference() const {
  return impl->current;
}

HalfEdgeSetIterator HalfEdgeSet::begin() const {
  return HalfEdgeSetIterator{PrivateConstructor{}, this, impl->set.find_first()};
}

HalfEdgeSetIterator HalfEdgeSet::end() const {
  return HalfEdgeSetIterator{PrivateConstructor{}, this, impl->set.size()};
}

ImplementationOf<HalfEdgeSetIterator>::ImplementationOf(const HalfEdgeSet* parent, size_t current) :
  ImplementationOf(parent, makeHalfEdge(parent, current)) {}

ImplementationOf<HalfEdgeSetIterator>::ImplementationOf(const HalfEdgeSet* parent, HalfEdge current) :
  parent(parent),
  current(current) {
  ASSERT(parent->impl->set.size() == current.index() || parent->contains(current), "Current HalfEdge " << current << " not in set " << *parent);
}

HalfEdge ImplementationOf<HalfEdgeSetIterator>::makeHalfEdge(const HalfEdgeSet* parent, typename decltype(::flatsurf::ImplementationOf<HalfEdgeSet>::set)::size_type pos) {
  if (pos == decltype(parent->impl->set)::npos)
    pos = parent->impl->set.size();
  return HalfEdge::fromIndex(pos);
}
}
