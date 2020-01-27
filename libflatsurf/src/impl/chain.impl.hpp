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

#include <exact-real/arb.hpp>
#include <gmpxx.h>

#include "../../flatsurf/chain.hpp"
#include "../../flatsurf/edge_map.hpp"
#include "../../flatsurf/vector.hpp"

namespace flatsurf {

template <typename Surface>
class Implementation<Chain<Surface>> {
  static void updateAfterFlip(EdgeMap<mpz_class>&, HalfEdge);

  using Move = std::variant<Chain<Surface>, HalfEdge>;

 public:
  Implementation(std::shared_ptr<const Surface> surface);

  void recordMove(const Move&) const;

  std::shared_ptr<const Surface> surface;

  EdgeMap<mpz_class> coefficients;

  mutable std::optional<typename Surface::Vector> vector;
  mutable std::vector<Move> pendingVectorMoves;

  mutable std::optional<Vector<exactreal::Arb>> approximateVector;
  mutable std::vector<Move> pendingApproximateVectorMoves;
};

}

#endif
