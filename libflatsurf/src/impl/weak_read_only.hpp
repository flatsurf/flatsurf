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

#ifndef LIBFLATSURF_WEAK_READ_ONLY_HPP
#define LIBFLATSURF_WEAK_READ_ONLY_HPP

#include <iosfwd>

#include "forward.hpp"

namespace flatsurf {

// A read-only reference (weak pointer) to a type T that uses the ManagedMovable<T> pimpl.
template <typename T>
class WeakReadOnly {
  ImplementationOf<T>* value;

 public:
  WeakReadOnly() = default;
  WeakReadOnly(const T&);
  WeakReadOnly(ImplementationOf<T>*);

  operator ReadOnly<T>() const;

  bool expired() const;
  void reset();
  ImplementationOf<T>* get();

  template <typename S>
  friend std::ostream& operator<<(std::ostream&, const WeakReadOnly<S>&);
};

}  // namespace flatsurf

#endif
