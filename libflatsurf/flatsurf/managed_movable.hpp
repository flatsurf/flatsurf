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

#ifndef LIBFLATSURF_MANAGED_MOVABLE_HPP
#define LIBFLATSURF_MANAGED_MOVABLE_HPP

#include <memory>

#include "forward.hpp"

namespace flatsurf {

// A pointer-to-implementation (pimpl) for types that are not copyable but movable.
// Unlike Movable<T>, instances of this type can be safely passed by const reference, i.e.,
// code can turn such const references into read-only wrappers that hold on to
// the same data structure and therefore hold the source of the reference alive.
// In other words, instances of a type which opts for this storage model,
// behave like dereferenced shared pointers that can be turned into the
// equivalent of a shared_ptr<const T> internally.
template <typename T>
class ManagedMovable {
 public:
  ManagedMovable() = delete;
  ManagedMovable(const ManagedMovable&) = delete;
  ManagedMovable(ManagedMovable&& rhs) = default;
  ManagedMovable(std::shared_ptr<ImplementationOf<T>> state) : state(std::move(state)) {}

  ManagedMovable& operator=(const ManagedMovable&) = delete;
  ManagedMovable& operator=(ManagedMovable&&) = default;

  template <typename ...Args>
  ManagedMovable(Args&& ...args) : state(std::make_shared<ImplementationOf<T>>(std::forward<Args>(args)...)) {}

  const auto* operator->() const { return &*state; }
  auto* operator->() { return &*state; }
  const auto& operator*() const { return *state; }
  auto& operator*() { return *state; }

  std::shared_ptr<ImplementationOf<T>> state;
};

}

#endif
