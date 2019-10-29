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

#ifndef LIBFLATSURF_VECTOR_STORAGE_CARTESIAN_IPP
#define LIBFLATSURF_VECTOR_STORAGE_CARTESIAN_IPP

#include <type_traits>
#include <utility>

using std::is_base_of_v;

namespace flatsurf {
namespace {

// A Vector::Implementation base class for vectors that rely on data stored in
// Cartesian coordinates.
template <typename T>
class Cartesian {
 public:
  using CartesianCoordinate = T;

  Cartesian(const T& x, const T& y) : x(x), y(y) {}
  Cartesian(T&& x, T&& y) : x(std::move(x)), y(std::move(y)) {}

  T x, y;
};

template <typename Implementation, typename = void>
struct is_cartesian_t : std::false_type {};
template <typename Implementation>
struct is_cartesian_t<Implementation, std::void_t<typename Implementation::CartesianCoordinate>> : std::true_type {};

template <typename Implementation>
inline constexpr bool is_cartesian_v = is_cartesian_t<Implementation>::value;

static_assert(is_cartesian_v<Cartesian<int>>);
}  // namespace
}  // namespace flatsurf

#endif
