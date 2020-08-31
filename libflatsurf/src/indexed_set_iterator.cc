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

#include "impl/indexed_set_iterator.hpp"

#include "../flatsurf/half_edge.hpp"
#include "../flatsurf/half_edge_set.hpp"
#include "../flatsurf/half_edge_set_iterator.hpp"
#include "impl/indexed_set.hpp"
#include "util/assert.ipp"

namespace flatsurf {

template <typename T>
void IndexedSetIterator<T>::increment() {
  ASSERT(*this != parent->end(), "iterator already at end");
  current = makeT(parent, parent->set.find_next(current.index()));
}

template <typename T>
bool IndexedSetIterator<T>::equal(const IndexedSetIterator& rhs) const {
  return parent == rhs.parent && current == rhs.current;
}

template <typename T>
const T& IndexedSetIterator<T>::dereference() const {
  return current;
}

template <typename T>
IndexedSetIterator<T>::IndexedSetIterator(const IndexedSet<T>* parent, size_t current) :
  parent(parent),
  current(makeT(parent, current)) {}

template <typename T>
IndexedSetIterator<T>::IndexedSetIterator(const IndexedSet<T>* parent, const T& current) :
  parent(parent),
  current(current) {
  ASSERT(parent->set.size() == current.index() || parent->contains(current), "Current HalfEdge " << current << " not in set " << *parent);
}

template <typename T>
T IndexedSetIterator<T>::makeT(const IndexedSet<T>* parent, typename IndexedSet<T>::Bitset::size_type pos) {
  if (pos == decltype(parent->set)::npos)
    pos = parent->set.size();
  return T::fromIndex(pos);
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const IndexedSetIterator<T>&) {
  return os << "IndexedSetIterator()";
}

}  // namespace flatsurf

// Instantiations of templates so implementations are generated for the linker
#include "../flatsurf/edge.hpp"
#include "util/instantiate.ipp"

LIBFLATSURF_INSTANTIATE_MANY_WRAPPED((LIBFLATSURF_INSTANTIATE_WITHOUT_IMPLEMENTATION), IndexedSetIterator, (HalfEdge)(Edge))
