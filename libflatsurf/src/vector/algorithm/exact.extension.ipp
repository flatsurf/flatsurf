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

#ifndef LIBFLATSURF_VECTOR_ALGORITHM_EXACT_EXTENSION_IPP
#define LIBFLATSURF_VECTOR_ALGORITHM_EXACT_EXTENSION_IPP

#include <intervalxt/forward.hpp>
#include <optional>

#include "flatsurf/detail/vector_base.hpp"

#include <boost/type_traits/is_detected.hpp>
#include <boost/type_traits/is_detected_exact.hpp>

// Vector::Implementation classes can implement the following
// extension points to provide specialized implementations for the operations supported by VectorExact.

namespace flatsurf::detail {
namespace {
template <typename Implementation>
using bool_t = decltype(static_cast<bool>(std::declval<const Implementation&>()));
template <typename Implementation>
static constexpr bool has_bool = boost::is_detected_v<bool_t, Implementation>;

template <typename Implementation>
using bound_t = decltype(std::declval<const Implementation&>().bound());
template <typename Implementation>
static constexpr bool has_bound = boost::is_detected_exact_v<Bound, bound_t, Implementation>;

template <typename Implementation>
using lt_bound_t = decltype(std::declval<const Implementation&>() < std::declval<const Bound>());
template <typename Implementation>
static constexpr bool has_lt_bound = boost::is_detected_exact_v<bool, lt_bound_t, Implementation>;

template <typename Implementation>
using gt_bound_t = decltype(std::declval<const Implementation&>() > std::declval<Bound>());
template <typename Implementation>
static constexpr bool has_gt_bound = boost::is_detected_exact_v<bool, gt_bound_t, Implementation>;

template <typename Implementation>
using eq_t = decltype(std::declval<const Implementation&>() == std::declval<const Implementation>());
template <typename Implementation>
static constexpr bool has_eq = boost::is_detected_exact_v<bool, eq_t, Implementation>;

template <typename Implementation>
using ccw_t = decltype(std::declval<Implementation>().ccw(std::declval<const typename Implementation::Vector&>()));
template <typename Implementation>
static constexpr bool has_ccw = boost::is_detected_exact_v<bool, ccw_t, Implementation>;

template <typename Implementation>
using orientation_t = decltype(std::declval<Implementation>().orientation(std::declval<const typename Implementation::Vector&>()));
template <typename Implementation>
static constexpr bool has_orientation = boost::is_detected_exact_v<bool, orientation_t, Implementation>;

template <typename Implementation>
using x_t = decltype(std::declval<const Implementation&>().x());
template <typename Implementation>
static constexpr bool has_x = boost::is_detected_exact_v<typename Implementation::Vector::Coordinate, x_t, Implementation>;

template <typename Implementation>
using y_t = decltype(std::declval<const Implementation&>().y());
template <typename Implementation>
static constexpr bool has_y = boost::is_detected_exact_v<typename Implementation::Vector::Coordinate, y_t, Implementation>;

template <typename Implementation>
using scalar_product_t = decltype(std::declval<const Implementation>() * (std::declval<const typename Implementation::Vector&>()));
template <typename Implementation>
static constexpr bool has_scalar_product = boost::is_detected_exact_v<typename Implementation::Vector::Coordinate, scalar_product_t, Implementation>;
}  // namespace
}  // namespace flatsurf::detail

#endif
