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

#ifndef LIBFLATSURF_VECTOR_ALGORITHM_WITH_ERROR_EXTENSION_IPP
#define LIBFLATSURF_VECTOR_ALGORITHM_WITH_ERROR_EXTENSION_IPP

#include <optional>

#include "flatsurf/detail/vector_base.hpp"

#include "../../util/type_traits.ipp"
#include "exact.extension.ipp"

// Vector::Implementation classes can implement the following
// extension points to provide specialized implementations for the operations supported by VectorWithError.

namespace flatsurf::detail {
namespace {
template <typename Implementation>
static constexpr bool has_optional_ccw = is_detected_exact_v<std::optional<CCW>, ccw_t, Implementation>;

template <typename Implementation>
static constexpr bool has_optional_orientation = is_detected_exact_v<std::optional<ORIENTATION>, orientation_t, Implementation>;

template <typename Implementation>
static constexpr bool has_optional_eq = is_detected_exact_v<std::optional<bool>, eq_t, Implementation>;

template <typename Implementation>
using optional_bool_t = decltype(static_cast<std::optional<bool>>(std::declval<const Implementation&>()));
template <typename Implementation>
static constexpr bool has_optional_bool = is_detected_v<optional_bool_t, Implementation>;

template <typename Implementation>
static constexpr bool has_optional_lt_bound = is_detected_exact_v<std::optional<bool>, lt_bound_t, Implementation>;

template <typename Implementation>
static constexpr bool has_optional_gt_bound = is_detected_exact_v<std::optional<bool>, gt_bound_t, Implementation>;
}  // namespace
}  // namespace flatsurf::detail

#endif
