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

#ifndef LIBFLATSURF_VECTOR_ALGORITHM_EXTENSION_IPP
#define LIBFLATSURF_VECTOR_ALGORITHM_EXTENSION_IPP

#include <boost/type_traits/is_detected_exact.hpp>

#include "../../../flatsurf/vector.hpp"

// Vector::Implementation classes can implement the following
// extension points to provide specialized implementations for some operations.

namespace flatsurf {
namespace {
// Note: This could be more fine grained. Approximations might be differently
// expensive to get a hold of or have differing (parametrized) levels of
// quality.  The correct question might be: Wanting to decide a predicate with
// a certain structure of arithmetic, is it worth to run it on this
// approximation first?
template <typename Implementation>
using approximation_t = decltype(std::declval<const Implementation&>().approximation());
template <typename Implementation>
static constexpr bool has_approximation_v = boost::is_detected_exact_v<Vector<exactreal::Arb>, approximation_t, Implementation>;
}  // namespace
}  // namespace flatsurf

#endif
