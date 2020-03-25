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

#ifndef LIBFLATSURF_IMPL_INDEXED_SET_ITERATOR_HPP
#define LIBFLATSURF_IMPL_INDEXED_SET_ITERATOR_HPP

#include <boost/iterator/iterator_categories.hpp>
#include <boost/iterator/iterator_facade.hpp>

#include "forward.hpp"

namespace flatsurf {

template <typename T>
class IndexedSetIterator : public boost::iterator_facade<IndexedSetIterator<T>, const T&, boost::forward_traversal_tag> {
 public:
  IndexedSetIterator(const IndexedSet<T>* parent, const T& current);
  IndexedSetIterator(const IndexedSet<T>* parent, size_t current);

  using value_type = T;

  void increment();
  const value_type& dereference() const;
  bool equal(const IndexedSetIterator& other) const;

  template <typename S>
  friend std::ostream& operator<<(std::ostream&, const IndexedSetIterator&);

 private:
  const IndexedSet<T>* parent;
  T current;

  static T makeT(const IndexedSet<T>* parent, typename IndexedSet<T>::Bitset::size_type pos);
};

}  // namespace flatsurf

#endif
