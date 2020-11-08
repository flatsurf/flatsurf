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

#ifndef LIBFLATSURF_VECTOR_IMPL_HPP
#define LIBFLATSURF_VECTOR_IMPL_HPP

#include "../../flatsurf/vector.hpp"

namespace flatsurf {

template <typename T>
class ImplementationOf<Vector<T>> {
 public:
  ImplementationOf() noexcept;

  ImplementationOf(const T& x, const T& y);

  ImplementationOf(T&& x, T&& y);

  T x, y;
};

}  // namespace flatsurf

#endif
