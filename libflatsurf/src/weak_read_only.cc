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

#include "../flatsurf/managed_movable.hpp"

#include "impl/weak_read_only.hpp"
#include "impl/read_only.hpp"
#include "impl/managed_movable.impl.hpp"

namespace flatsurf {

template <typename T>
WeakReadOnly<T>::WeakReadOnly(const T& value) :
  WeakReadOnly(ImplementationOf<ManagedMovable<T>>::self(value).state.get()) {}

template <typename T>
WeakReadOnly<T>::WeakReadOnly(ImplementationOf<T>* value) :
  value(value) {}

template <typename T>
WeakReadOnly<T>::operator ReadOnly<T>() const {
  return ReadOnly<T>(ImplementationOf<ManagedMovable<T>>::from_this(value->shared_from_this()));
}

template <typename T>
bool WeakReadOnly<T>::expired() const {
  return value == nullptr;
}

template <typename T>
ImplementationOf<T>* WeakReadOnly<T>::get() {
  return value;
}

template <typename T>
void WeakReadOnly<T>::reset() {
  value = nullptr;
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const WeakReadOnly<T>& self) {
  return os << static_cast<ReadOnly<T>>(self);
}

}

#include "util/instantiate.ipp"

#include "impl/flat_triangulation.impl.hpp"
#include "impl/flat_triangulation_collapsed.impl.hpp"

// Instantiations of templates so implementations are generated for the linker
LIBFLATSURF_INSTANTIATE_MANY_WRAPPED((LIBFLATSURF_INSTANTIATE_WITHOUT_IMPLEMENTATION), WeakReadOnly, (FlatTriangulationCombinatorial))
