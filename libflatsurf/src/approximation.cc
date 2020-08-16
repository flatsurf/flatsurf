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

#include <exact-real/integer_ring.hpp>
#include <exact-real/number_field.hpp>
#include <exact-real/rational_field.hpp>

#include "impl/approximation.hpp"
#include "util/false.ipp"

namespace flatsurf {

template <typename T>
exactreal::Arb Approximation<T>::arb(const T& x, slong prec) {
  if constexpr (std::is_same_v<T, mpz_class>) {
    (void)(prec);
    return exactreal::Arb(x);
  } else if constexpr (std::is_same_v<T, long long>) {
    (void)(prec);
    return exactreal::Arb(x);
  } else if constexpr (std::is_same_v<T, mpq_class>) {
    return exactreal::Arb(x, prec);
  } else if constexpr (std::is_same_v<T, eantic::renf_elem_class>) {
    return exactreal::Arb(x, prec);
  } else if constexpr (std::is_same_v<T, exactreal::Element<exactreal::IntegerRing>>) {
    return x.arb(prec);
  } else if constexpr (std::is_same_v<T, exactreal::Element<exactreal::RationalField>>) {
    return x.arb(prec);
  } else if constexpr (std::is_same_v<T, exactreal::Element<exactreal::NumberField>>) {
    return x.arb(prec);
  } else {
    static_assert(false_type_v<T>, "not implemented: arb() for this type");
  }
}

}  // namespace flatsurf

// Instantiations of templates so implementations are generated for the linker
#include "util/instantiate.ipp"

LIBFLATSURF_INSTANTIATE_MANY_WRAPPED((LIBFLATSURF_INSTANTIATE_STATIC), Approximation, LIBFLATSURF_REAL_TYPES)
