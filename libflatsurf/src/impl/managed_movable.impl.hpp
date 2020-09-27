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

#ifndef LIBFLATSURF_MANAGED_MOVABLE_IMPL_HPP
#define LIBFLATSURF_MANAGED_MOVABLE_IMPL_HPP

#include <boost/noncopyable.hpp>
#include <functional>
#include <memory>

#include "../../flatsurf/forward.hpp"
#include "../util/assert.ipp"
#include "read_only.hpp"
#include "weak_read_only.hpp"

namespace flatsurf {

// Utilities to make objects work that hold a Managed Movable pimpl.
// Implementations of such objects inherit from this class which encapsulates
// copying and creating the state underlying the Managed Movable.
template <typename T>
class ImplementationOf<ManagedMovable<T>> : boost::noncopyable {
 public:
  // Given the implementation pimpl of an object, return a corresponding object
  // holding that pimpl.
  T from_this() {
    return from_this(std::dynamic_pointer_cast<ImplementationOf<T>>(static_cast<ImplementationOf<T>*>(this)->shared_from_this()));
  }

  // Given an implementation pimpl of an object, return a corresponding object.
  static T from_this(std::shared_ptr<ImplementationOf<T>> self) {
    ASSERT(self.get() != nullptr, "Cannot create an object from a null pointer implementation.");

    static_assert(sizeof(T) == sizeof(ManagedMovable<T>), "T must have a single field, namely a ManagedMovable<T>");
    static_assert(sizeof(ManagedMovable<T>) == sizeof(self), "ManagedMovable<T> must have a single field, namely a shared pointer to the implementation of T");

    // A class that uses Managed Movable only has a single field, namely that
    // ManagedMovable<T> which again has a single field, a shared pointer to
    // the implementation. All three occupy the same space so we can safely
    // cast between the three by a simple reinteret cast.
    return reinterpret_cast<T&&>(self);
  }

  // Return the managed movable stored in a T.
  // Helper method so that friends of the Implementation can pretend to be friends of T.
  template <typename O, typename S = T>
  static ManagedMovable<S>& self(O& obj) { return obj.self; }

  // Return the managed movable stored in a T.
  // Helper method so that friends of the Implementation can pretend to be friends of T.
  template <typename O, typename S = T>
  static const ManagedMovable<S>& self(const O& obj) { return obj.self; }

  // Return the managed movable stored in a T.
  // Helper method so that friends of the Implementation can pretend to be friends of T.
  template <typename O, typename S = T>
  static ManagedMovable<S>&& self(O&& obj) { return std::move(obj.self); }

  friend ReadOnly<T>;
  friend WeakReadOnly<T>;
};

}  // namespace flatsurf

#endif
