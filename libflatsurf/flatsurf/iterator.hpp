/**********************************************************************
 *  This file is part of flatsurf.
 *
 *        Copyright (C) 2022 Julian Rüth
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

#include "copyable.hpp"

namespace flatsurf {

// TODO: Implement an iterator facade? In particular, we want to compute the
// length of iterator - iterator to count automorphisms modulo equivalence.
template <typename T>
class Iterator {
 public:
  template <typename S>
  Iterator(S&& impl) : self(impl) {}

 private:
  Copyable<Iterator> self;
};

template <typename T>
struct ImplementationOf<Iterator<T>> {
  virtual ~ImplementationOf();
};

}