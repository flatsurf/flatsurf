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

#ifndef LIBFLATSURF_INTERVAL_EXCHANGE_TRANSFORMATION_IMPL_HPP
#define LIBFLATSURF_INTERVAL_EXCHANGE_TRANSFORMATION_IMPL_HPP

#include <map>
#include <vector>

#include <intervalxt/interval_exchange_transformation.hpp>

#include "../../flatsurf/interval_exchange_transformation.hpp"
#include "lengths.hpp"

namespace flatsurf {

template <typename Surface>
class ImplementationOf<IntervalExchangeTransformation<Surface>> {
  using T = typename Surface::Coordinate;

 public:
  ImplementationOf(std::shared_ptr<const Surface> surface, const Vector<T>& vertical, const std::vector<HalfEdge>& top, const std::vector<HalfEdge>& bottom);

  // TODO: This is a hack.
  static void registerDecomposition(const IntervalExchangeTransformation<Surface>&, std::shared_ptr<FlowDecompositionState<FlatTriangulation<T>>>);

  std::shared_ptr<const Surface> surface;
  intervalxt::IntervalExchangeTransformation iet;
  // A (correctly casted) pointer to the actual lengths stored inside iet.
  Lengths<Surface>* lengths;
};

}  // namespace flatsurf

#endif
