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

#ifndef LIBFLATSURF_TEST_GENERATORS_REAL_GENERATOR_HPP
#define LIBFLATSURF_TEST_GENERATORS_REAL_GENERATOR_HPP

#include <e-antic/renfxx.h>

#include <exact-real/element.hpp>
#include <exact-real/integer_ring.hpp>
#include <exact-real/number_field.hpp>
#include <exact-real/rational_field.hpp>

namespace flatsurf::test {

template <typename T>
constexpr bool hasFractions = std::is_same_v<T, mpq_class> || std::is_same_v<T, eantic::renf_elem_class> || std::is_same_v<T, exactreal::Element<exactreal::RationalField>> || std::is_same_v<T, exactreal::Element<exactreal::NumberField>>;

template <typename T>
constexpr bool hasNumberFieldElements = std::is_same_v<T, eantic::renf_elem_class> || std::is_same_v<T, exactreal::Element<exactreal::NumberField>>;

template <typename T, typename S>
T frac(const S& num, const S& den) {
  return T(num) / den;
}

}  // namespace flatsurf::test

#define LIBFLATSURF_TEST_REAL_TYPES (long long), (mpz_class), (mpq_class), (eantic::renf_elem_class), (exactreal::Element<exactreal::IntegerRing>), (exactreal::Element<exactreal::RationalField>), (exactreal::Element<exactreal::NumberField>)

#endif
