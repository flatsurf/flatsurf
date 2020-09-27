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

#include "../flatsurf/chain_iterator.hpp"

#include <flint/fmpz.h>

#include "../flatsurf/chain.hpp"
#include "../flatsurf/edge.hpp"
#include "impl/chain.impl.hpp"
#include "impl/chain_iterator.impl.hpp"
#include "util/assert.ipp"

namespace flatsurf {

template <typename Surface>
void ChainIterator<Surface>::increment() {
  const size_t pos = self->current.first.index();
  _fmpz_demote_val(&self->parent->self->coefficients[pos]);
  self->current = ImplementationOf<ChainIterator>::make(self->parent, ImplementationOf<ChainIterator>::findNext(self->parent, static_cast<int>(pos)));
}

template <typename Surface>
const typename ChainIterator<Surface>::value_type& ChainIterator<Surface>::dereference() const {
  ASSERT(self->current.second != nullptr, "Cannot dereference iterator that is already at the end of Chain.");
  return self->current;
}

template <typename Surface>
bool ChainIterator<Surface>::equal(const ChainIterator& rhs) const {
  return self->parent == rhs.self->parent && self->current.first == rhs.self->current.first;
}

template <typename Surface>
std::ostream& operator<<(std::ostream& os, const ChainIterator<Surface>& self) {
  if (self.self->current.second == nullptr)
    return os << "Chain::end()";
  else if (self.self->current.first.index() == 0)
    return os << "Chain::begin()";
  else
    return os << "Chain::begin() + " << self.self->current.first.index();
}

template <typename Surface>
ImplementationOf<ChainIterator<Surface>>::ImplementationOf(const Chain<Surface>* parent, int pos) :
  parent(parent),
  current(make(parent, findNext(parent, pos - 1))) {
}

template <typename Surface>
ChainIterator<Surface> ImplementationOf<ChainIterator<Surface>>::begin(const Chain<Surface>* chain) {
  return ChainIterator<Surface>(PrivateConstructor{}, chain, 0);
}

template <typename Surface>
ChainIterator<Surface> ImplementationOf<ChainIterator<Surface>>::end(const Chain<Surface>* chain) {
  return ChainIterator<Surface>(PrivateConstructor{}, chain, static_cast<int>(chain->surface().size()));
}

template <typename Surface>
size_t ImplementationOf<ChainIterator<Surface>>::findNext(const Chain<Surface>* parent, int pos) {
  const size_t size = parent->surface().size();

  do {
    pos++;
  } while (pos < size && fmpz_is_zero(&parent->self->coefficients[pos]));

  return pos;
}

template <typename Surface>
std::pair<Edge, const mpz_class*> ImplementationOf<ChainIterator<Surface>>::make(const Chain<Surface>* parent, size_t pos) {
  const Edge e = Edge::fromIndex(pos);
  if (pos != parent->surface().size())
    return std::pair(e, &(*parent)[e]);
  else
    return std::pair(e, nullptr);
}

}  // namespace flatsurf

// Instantiations of templates so implementations are generated for the linker
#include "util/instantiate.ipp"

LIBFLATSURF_INSTANTIATE_MANY_WRAPPED((LIBFLATSURF_INSTANTIATE_WITH_IMPLEMENTATION)(LIBFLATSURF_INSTANTIATE_HASH), ChainIterator, LIBFLATSURF_FLAT_TRIANGULATION_TYPES)
