/**********************************************************************
 *  This file is part of flatsurf.
 *
 *        Copyright (C) 2019 Julian Rüth
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

#ifndef LIBFLATSURF_UTIL_UNION_FIND_IPP
#define LIBFLATSURF_UTIL_UNION_FIND_IPP

#include <boost/operators.hpp>

namespace flatsurf {
namespace {
// The classic union-find data structure to decide set membership in
// (practically) O(1).
template <typename T>
class UnionFind : boost::equality_comparable<UnionFind<T>> {
  mutable UnionFind const* parent;
  const T repr;

  const UnionFind& top() {
    if (parent == this)
      return *this;
    return *(parent = &parent->top());
  }

  const UnionFind& top() const {
    if (parent == this)
      return *this;
    return *(parent = &parent->top());
  }

 public:
  UnionFind(const T representative) :
    repr(representative) {
    parent = this;
  }

  UnionFind(const UnionFind&) = delete;
  UnionFind(UnionFind&&) = delete;
  UnionFind& operator=(const UnionFind&) = delete;
  UnionFind& operator=(UnionFind&&) = delete;

  bool operator==(const UnionFind& rhs) const {
    return &top() == &rhs.top();
  }

  void join(const UnionFind& rhs) {
    top().parent = &rhs.top();
    assert(*this == rhs);
  }

  const T& representative() const {
    return top().repr;
  }
};
}  // namespace
}  // namespace flatsurf

#endif
