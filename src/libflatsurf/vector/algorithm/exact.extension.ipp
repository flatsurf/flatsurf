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

#ifndef LIBFLATSURF_VECTOR_ALGORITHM_EXACT_EXTENSION_IPP
#define LIBFLATSURF_VECTOR_ALGORITHM_EXACT_EXTENSION_IPP

#include <optional>
#include <intervalxt/forward.hpp>

#include "flatsurf/detail/vector_base.hpp"

#include "../../util/type_traits.ipp"

// Vector::Implementation classes can implement the following
// extension points to provide specialized implementations for the operations supported by VectorExact.

namespace flatsurf::detail {
namespace {
template <typename Implementation>
using bool_t = decltype(static_cast<bool>(std::declval<const Implementation&>()));
template <typename Implementation>
static constexpr bool has_bool = is_detected_v<bool_t, Implementation>;

template <typename Implementation>
using lt_bound_t = decltype(std::declval<const Implementation&>() < std::declval<const Bound>());
template <typename Implementation>
static constexpr bool has_lt_bound = is_detected_exact_v<bool, lt_bound_t, Implementation>;

template <typename Implementation>
using gt_bound_t = decltype(std::declval<const Implementation&>() < std::declval<Bound>());
template <typename Implementation>
static constexpr bool has_gt_bound = is_detected_exact_v<bool, gt_bound_t, Implementation>;

template <typename Implementation>
using eq_t = decltype(std::declval<const Implementation&>() == std::declval<const Implementation>());
template <typename Implementation>
static constexpr bool has_eq = is_detected_exact_v<bool, eq_t, Implementation>;

template <typename Implementation>
using ccw_t = decltype(std::declval<Implementation>().ccw(std::declval<const typename Implementation::Vector&>()));
template <typename Implementation>
static constexpr bool has_ccw = is_detected_exact_v<bool, ccw_t, Implementation>;

template <typename Implementation>
using orientation_t = decltype(std::declval<Implementation>().orientation(std::declval<const typename Implementation::Vector&>()));
template <typename Implementation>
static constexpr bool has_orientation = is_detected_exact_v<bool, orientation_t, Implementation>;

template <typename Implementation>
using x_t = decltype(std::declval<const Implementation&>().x());
template <typename Implementation>
static constexpr bool has_x = is_detected_exact_v<typename Implementation::Vector::Coordinate, x_t, Implementation>;

template <typename Implementation>
using y_t = decltype(std::declval<const Implementation&>().y());
template <typename Implementation>
static constexpr bool has_y = is_detected_exact_v<typename Implementation::Vector::Coordinate, y_t, Implementation>;

template <typename Implementation>
using length_t = decltype(std::declval<const Implementation&>().length());
template <typename Implementation>
static constexpr bool has_length = is_detected_exact_v<intervalxt::Length<typename Implementation::Vector::Coordinate> ,length_t, Implementation>;
}  // namespace
}  // namespace flatsurf::detail

#endif
