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

template <typename T>
class Iterable {
 public:
  template <typename S>
  Iterable(S&& impl) : self(impl) {}

  Iterator<T> begin();
  Iterator<T> end();

 private:
  Copyable<Iterable> self;
};

template <typename T>
struct ImplementationOf<Iterable<T>> {
  virtual ~ImplementationOf();
  virtual typename Iterable<T>::Iterator begin();
  virtual typename Iterable<T>::Iterator end();
};

}
