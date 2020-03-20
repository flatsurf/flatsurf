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

#ifndef LIBFLATSURF_IMPL_INDEXED_SET_HPP
#define LIBFLATSURF_IMPL_INDEXED_SET_HPP

#include <boost/dynamic_bitset.hpp>
#include <boost/dynamic_bitset/dynamic_bitset.hpp>

#include "forward.hpp"

namespace flatsurf {

template <typename T>
class IndexedSet {
 public:
  IndexedSet();
  IndexedSet(const std::vector<T>&);

  using Bitset = boost::dynamic_bitset<>;

  bool contains(const T&) const;
  void insert(const T&);

  bool operator==(const IndexedSet&) const;

  bool disjoint(const IndexedSet&) const;
  bool empty() const;

  size_t size() const;
  void erase(const T&);

  IndexedSetIterator<T> begin() const;
  IndexedSetIterator<T> end() const;

  template <typename S>
  friend std::ostream& operator<<(std::ostream&, const IndexedSet<S>&);

 private:
  mutable Bitset set {};

  friend IndexedSetIterator<T>;
};

}

#endif
