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

#include "impl/read_only.hpp"

namespace flatsurf {

template <typename T>
ReadOnly<T>::ReadOnly(const T& value) :
  value(ImplementationOf<ManagedMovable<T>>::from_this(ImplementationOf<ManagedMovable<T>>::self(value).state)) {}

template <typename T>
ReadOnly<T>::ReadOnly(const ReadOnly& rhs) :
  ReadOnly(*rhs) {}

template <typename T>
ReadOnly<T>::ReadOnly(ReadOnly&& rhs) :
  value(std::move(rhs.value)) {}

template <typename T>
const T* ReadOnly<T>::operator->() const {
  return &value;
}

template <typename T>
const T& ReadOnly<T>::operator*() const {
  return value;
}

template <typename T>
ReadOnly<T>::operator const T&() const {
  return value;
}

template <typename T>
ReadOnly<T>& ReadOnly<T>::operator=(const ReadOnly& rhs) {
  *this = ImplementationOf<ManagedMovable<T>>::from_this(ImplementationOf<ManagedMovable<T>>::self(rhs.value).state);
  return *this;
}

template <typename T>
ReadOnly<T>& ReadOnly<T>::operator=(ReadOnly&& rhs) {
  value = std::move(rhs.value);
  return *this;
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const ReadOnly<T>& self) {
  return os << *self;
}

}

#include "util/instantiate.ipp"

#include "impl/flat_triangulation_combinatorial.impl.hpp"
#include "impl/flat_triangulation.impl.hpp"
#include "impl/flat_triangulation_collapsed.impl.hpp"

// Instantiations of templates so implementations are generated for the linker
LIBFLATSURF_INSTANTIATE_MANY_WRAPPED((LIBFLATSURF_INSTANTIATE_WITHOUT_IMPLEMENTATION), ReadOnly, (FlatTriangulationCombinatorial))
LIBFLATSURF_INSTANTIATE_MANY_WRAPPED((LIBFLATSURF_INSTANTIATE_WITHOUT_IMPLEMENTATION), ReadOnly, LIBFLATSURF_FLAT_TRIANGULATION_TYPES)
LIBFLATSURF_INSTANTIATE_MANY_WRAPPED((LIBFLATSURF_INSTANTIATE_WITHOUT_IMPLEMENTATION), ReadOnly, LIBFLATSURF_FLAT_TRIANGULATION_COLLAPSED_TYPES)
