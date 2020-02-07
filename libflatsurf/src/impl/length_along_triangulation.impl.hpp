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

#ifndef LIBFLATSURF_LENGTH_ALONG_TRIANGULATION_IMPL_HPP
#define LIBFLATSURF_LENGTH_ALONG_TRIANGULATION_IMPL_HPP

#include <functional>
#include <memory>
#include <optional>

#include <exact-real/arb.hpp>

#include "../../flatsurf/edge_map.hpp"
#include "../../flatsurf/length_along_triangulation.hpp"
#include "../../flatsurf/vertical.hpp"

namespace flatsurf {

template <typename Surface>
class Implementation<LengthAlongTriangulation<Surface>> {
  using T = typename Surface::Coordinate;

 public:
  enum class CLASSIFICATION {
    BOTH_ARE_NIL,
    LEFT_IS_NIL,
    RIGHT_IS_NIL,
    NONE_IS_NIL,
  };

  using Coefficient = std::conditional_t<std::is_same_v<long long, T>, long long, mpz_class>;

  Implementation();
  Implementation(HalfEdge, std::shared_ptr<const Vertical<Surface>> vertical);

  operator T() const noexcept;

  static int sgn(HalfEdge e) noexcept;

  void apply(const std::function<void(HalfEdge, const Coefficient&)>&) const;

  CLASSIFICATION classify(const Implementation& rhs) const;

  std::shared_ptr<const Vertical<Surface>> vertical;
  // The coefficients attached to the half edges with positive ids
  std::optional<EdgeMap<Coefficient>> coefficients;
  mutable exactreal::Arb approximation;
};

}  // namespace flatsurf

#endif
