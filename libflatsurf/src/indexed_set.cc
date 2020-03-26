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

#include "../flatsurf/flat_triangulation_combinatorial.hpp"
#include "../flatsurf/fmt.hpp"
#include "../flatsurf/half_edge.hpp"
#include "../flatsurf/half_edge_set.hpp"
#include "../flatsurf/half_edge_set_iterator.hpp"

#include "impl/indexed_set.hpp"
#include "impl/indexed_set_iterator.hpp"

#include "util/assert.ipp"

namespace flatsurf {

template <typename T>
IndexedSet<T>::IndexedSet() :
  set() {}

template <typename T>
IndexedSet<T>::IndexedSet(const std::vector<T>& items) :
  IndexedSet() {
  for (const auto& item : items)
    insert(item);
}

template <typename T>
bool IndexedSet<T>::contains(const T& t) const {
  if (set.size() <= t.index())
    return false;
  return set.test(t.index());
}

template <typename T>
void IndexedSet<T>::insert(const T& t) {
  if (set.size() <= t.index())
    set.resize(t.index() + 1);
  set.set(t.index());
}

template <typename T>
bool IndexedSet<T>::operator==(const IndexedSet& rhs) const {
  set.resize(std::max(set.size(), rhs.set.size()));
  rhs.set.resize(std::max(set.size(), rhs.set.size()));

  return set == rhs.set;
}

template <typename T>
bool IndexedSet<T>::disjoint(const IndexedSet& rhs) const {
  set.resize(std::max(set.size(), rhs.set.size()));
  rhs.set.resize(std::max(set.size(), rhs.set.size()));

  return not set.intersects(rhs.set);
}

template <typename T>
bool IndexedSet<T>::empty() const {
  return begin() == end();
}

template <typename T>
size_t IndexedSet<T>::size() const {
  return set.count();
}

template <typename T>
void IndexedSet<T>::erase(const T& t) {
  if (set.size() <= t.index()) return;
  set.reset(t.index());
}

template <typename T>
IndexedSetIterator<T> IndexedSet<T>::begin() const {
  return IndexedSetIterator<T>(this, set.find_first());
}

template <typename T>
IndexedSetIterator<T> IndexedSet<T>::end() const {
  return IndexedSetIterator<T>(this, set.size());
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const IndexedSet<T>& self) {
  return os << fmt::format("{{{0}}}", fmt::join(self, ", "));
}

}  // namespace flatsurf

// Instantiations of templates so implementations are generated for the linker
#include "util/instantiate.ipp"

#include "../flatsurf/edge.hpp"

LIBFLATSURF_INSTANTIATE_MANY_WRAPPED((LIBFLATSURF_INSTANTIATE_WITHOUT_IMPLEMENTATION), IndexedSet, (HalfEdge)(Edge))
