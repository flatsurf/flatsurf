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

#ifndef LIBFLATSURF_VECTOR_ALGORITHM_BASE_EXTENSION_IPP
#define LIBFLATSURF_VECTOR_ALGORITHM_BASE_EXTENSION_IPP

#include "flatsurf/detail/vector_base.hpp"

#include "../../util/type_traits.ipp"

// Vector::Implementation classes can implement the following
// extension points to provide specialized implementations for the operations supported by VectorBase.

namespace flatsurf {
namespace {
template <typename Implementation>
using ostream_lshift_t = decltype(std::declval<std::ostream&> << std::declval<Implementation>());
template <typename Implementation>
static constexpr bool has_ostream_lshift = is_detected_v<ostream_lshift_t, const Implementation>;

template <typename Implementation>
using vector_arb_t = decltype(static_cast<flatsurf::Vector<exactreal::Arb>>(std::declval<const Implementation&>()));
template <typename Implementation>
static constexpr bool has_vector_arb = is_detected_v<vector_arb_t, Implementation>;

template <typename Implementation>
using unary_negate_t = decltype(-std::declval<Implementation>());
template <typename Implementation>
static constexpr bool has_unary_negate = is_detected_exact_v<typename Implementation::Vector, unary_negate_t, const Implementation>;

template <typename Implementation>
using binary_inplace_plus_t = decltype(std::declval<Implementation>() += std::declval<const typename Implementation::Vector&>());
template <typename Implementation>
static constexpr bool has_binary_inplace_plus = is_detected_exact_v<Implementation&, binary_inplace_plus_t, Implementation>;

template <typename Implementation>
using binary_inplace_times_int_t = decltype(std::declval<Implementation>() *= std::declval<int>());
template <typename Implementation>
static constexpr bool has_binary_inplace_times_int = is_detected_exact_v<Implementation&, binary_inplace_times_int_t, Implementation>;

template <typename Implementation>
using binary_inplace_times_mpz_t = decltype(std::declval<Implementation>() *= std::declval<mpz_class>());
template <typename Implementation>
static constexpr bool has_binary_inplace_times_mpz = is_detected_exact_v<Implementation&, binary_inplace_times_mpz_t, Implementation>;

template <typename Implementation>
using perpendicular_t = decltype(std::declval<const Implementation>().perpendicular());
template <typename Implementation>
static constexpr bool has_perpendicular = is_detected_exact_v<typename Implementation::Vector, perpendicular_t, Implementation>;

template <typename Implementation>
using projection_t = decltype(std::declval<const Implementation>().projection(std::declval<const typename Implementation::Vector&>()));
template <typename Implementation>
static constexpr bool has_projection = is_detected_exact_v<typename Implementation::Vector, projection_t, Implementation>;
}  // namespace
}  // namespace flatsurf

#endif
