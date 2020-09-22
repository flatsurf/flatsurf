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

#ifndef LIBFLATSURF_READ_ONLY_HPP
#define LIBFLATSURF_READ_ONLY_HPP

#include <memory>

#include "forward.hpp"

namespace flatsurf {

// A read-only reference (feels like a shared pointer to a const object) of a
// type T that uses the ManagedMovable<T> pimpl.
// This allows objects to hold a reference to an object that is only movable
// and not copyable. They could of course just store a T* but then that would
// not keep the target alive. This construct does keep the target alive by
// implicitly holding on to the shared pointer that is used internally.
template <typename T>
class ReadOnly {
  // A clone of the object we are holding on to. Since T is not copyable, a
  // user cannot create two Ts pointing to the same object but internally we
  // can do this. While given a `T x` and a `ReadOnly<T>(x) y` we have that `&x
  // != &y.value`, we still get that `x == y.value` and in particular modifying
  // one, modifies the other.
  // One should think of this as an implicitly dereferenced shared pointer.
  T value;

 public:
  ReadOnly() = default;
  ReadOnly(const T&);
  ReadOnly(const ReadOnly&);
  ReadOnly(ReadOnly&&);

  const T* operator->() const;
  const T& operator*() const;

  operator const T&() const;

  ReadOnly& operator=(const ReadOnly&);
  ReadOnly& operator=(ReadOnly&&);

  template <typename S>
  friend std::ostream& operator<<(std::ostream&, const ReadOnly<S>&);
};

}

#endif
