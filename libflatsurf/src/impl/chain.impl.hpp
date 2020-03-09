/**********************************************************************
 *  This file is part of flatsurf.
 *
 *        Copyright (C) 2019-2020 Julian Rüth
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

#ifndef LIBFLATSURF_CHAIN_IMPL_HPP
#define LIBFLATSURF_CHAIN_IMPL_HPP

#include <memory>
#include <optional>

#include <gmpxx.h>
#include <exact-real/arb.hpp>

#include "../../flatsurf/chain.hpp"
#include "../../flatsurf/edge_map.hpp"
#include "../../flatsurf/vector.hpp"
#include "../../flatsurf/tracking.hpp"

#include "chain_vector.hpp"

namespace flatsurf {

template <typename Surface>
class ImplementationOf<Chain<Surface>> {
  using T = typename Surface::Coordinate;

 public:
  ImplementationOf(const ImplementationOf&);
  ImplementationOf(ImplementationOf&&);
  ImplementationOf(std::shared_ptr<const Surface>);

  ~ImplementationOf();

  ImplementationOf& operator=(const ImplementationOf&);
  ImplementationOf& operator=(ImplementationOf&&);

  operator const Vector<T>&() const;
  operator const Vector<exactreal::Arb>&() const;

  std::optional<const mpz_class*> operator[](size_t) const;

  std::shared_ptr<const Surface> surface;

  fmpz* coefficients;

  mutable ChainVector<Surface, T> vector;
  mutable ChainVector<Surface, exactreal::Arb> approximateVector;
};

}  // namespace flatsurf

#endif
