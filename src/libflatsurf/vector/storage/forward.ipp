/**********************************************************************
 *  This file is part of flatsurf.
 *
 *        Copyright (C) 2019 Julian Rüth
 *
 *  Flatsurf is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 of the License, or
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

#ifndef LIBFLATSURF_VECTOR_STORAGE_FORWARD_IPP
#define LIBFLATSURF_VECTOR_STORAGE_FORWARD_IPP

#include <type_traits>
#include <utility>

#include "../../util/type_traits.ipp"

using std::is_base_of_v;

namespace flatsurf {
namespace {

// A Vector::Implementation base class for vectors that simply forward all
// queries to another kind of vector.
template <typename Vector>
class Forward {
 public:
  using ForwardTo = Vector;

  Forward() {}
  explicit Forward(const Vector& vector) : vector(vector) {}
  explicit Forward(Vector&& vector) : vector(std::move(vector)) {}

  Vector vector;
};

template <typename Implementation, typename = void>
struct is_forward_t : std::false_type {};
template <typename Implementation>
struct is_forward_t<Implementation, std::void_t<typename Implementation::ForwardTo>> : std::true_type {};

template <typename Implementation>
inline constexpr bool is_forward_v = is_forward_t<Implementation>::value;

static_assert(is_forward_v<Forward<int>>);
}  // namespace
}  // namespace flatsurf

#endif
