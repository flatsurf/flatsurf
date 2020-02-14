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

#include "../flatsurf/tracking_set_iterator.hpp"
#include "../flatsurf/tracking_set.hpp"
#include "../flatsurf/half_edge.hpp"
#include "../flatsurf/edge.hpp"
#include "../flatsurf/vertex.hpp"

#include "impl/tracking_set_iterator.impl.hpp"

#include "util/assert.ipp"

namespace flatsurf {

template <typename T>
TrackingSetIterator<T>::TrackingSetIterator() :
  // We expect the caller to initialize impl.
  impl(nullptr) {
}

template <typename T>
void TrackingSetIterator<T>::increment() {
  ASSERT(impl->current != nullptr, "cannot increment iterator at end()");
  // Note that this could be optimized further by using a dynamic_bitset for
  // the TrackingStorage when V==bool and K has index(); using
  // dynamic_bitset::find_next().
  if constexpr (std::is_same_v<T, HalfEdge>) {
    const auto end = (&*rbegin(impl->set->parent().halfEdges())) + 1;
    for (impl->current++; impl->current < end; impl->current++) {
      if (impl->set->contains(*impl->current))
        return;
    }

    impl->current = nullptr;
  } else {
    throw std::logic_error("not implemented: increment() for this type");
  }
}

template <typename T>
bool TrackingSetIterator<T>::equal(const TrackingSetIterator& rhs) const {
  return impl->set == rhs.impl->set && impl->current == rhs.impl->current;
}

template <typename T>
const T& TrackingSetIterator<T>::dereference() const {
  ASSERT(impl->current != nullptr, "cannot dereference iterator at end()");
  return *impl->current;
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const TrackingSetIterator<T>& self) {
  if (self.impl->current == nullptr)
    return os << "TrackingSetIterator::end()";

  return os << "TrackingSetIterator{" << *self.impl->current << "}";
}

template <typename T>
Implementation<TrackingSetIterator<T>>::Implementation(const TrackingSet<T>* set) :
  set(set) {
}

template <typename T>
TrackingSetIterator<T> Implementation<TrackingSetIterator<T>>::begin(const TrackingSet<T>* parent) {
  auto it = end(parent);
  if constexpr (std::is_same_v<T, HalfEdge>) {
    using std::begin;
    it.impl->current = &*begin(it.impl->set->parent().halfEdges());
    if (!it.impl->set->contains(*it.impl->current))
      it.increment();
  } else {
    throw std::logic_error("not implemented: begin() for this kind of TrackingSet");
  }

  return it;
}

template <typename T>
TrackingSetIterator<T> Implementation<TrackingSetIterator<T>>::end(const TrackingSet<T>* parent) {
  auto it = TrackingSetIterator<T>();
  it.impl = spimpl::make_impl<Implementation>(parent);
  return it;
}

}

// Instantiations of templates so implementations are generated for the linker
#include "util/instantiate.ipp"

LIBFLATSURF_INSTANTIATE((LIBFLATSURF_INSTANTIATE_WITH_IMPLEMENTATION), (TrackingSetIterator<Edge>))
LIBFLATSURF_INSTANTIATE((LIBFLATSURF_INSTANTIATE_WITH_IMPLEMENTATION), (TrackingSetIterator<HalfEdge>))
LIBFLATSURF_INSTANTIATE((LIBFLATSURF_INSTANTIATE_WITH_IMPLEMENTATION), (TrackingSetIterator<Vertex>))
