/**********************************************************************
 *  This file is part of flatsurf.
 *
 *        Copyright (C) 2019-2020 Julian Rüth
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

#ifndef LIBFLATSURF_HALF_EDGE_SET_ITERATOR_IMPL_HPP
#define LIBFLATSURF_HALF_EDGE_SET_ITERATOR_IMPL_HPP

#include <boost/dynamic_bitset/dynamic_bitset.hpp>

#include "../../flatsurf/half_edge.hpp"
#include "../../flatsurf/half_edge_set_iterator.hpp"

#include "indexed_set.hpp"
#include "indexed_set_iterator.hpp"

namespace flatsurf {

template <>
class ImplementationOf<HalfEdgeSetIterator> : public IndexedSetIterator<HalfEdge> {
 public:
  using IndexedSetIterator<HalfEdge>::IndexedSetIterator;

  ImplementationOf(IndexedSetIterator<HalfEdge>&&);
};

}  // namespace flatsurf

#endif
