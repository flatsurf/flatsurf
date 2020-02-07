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

#ifndef LIBFLATSURF_FLAT_TRIANGULATION_IMPL_HPP
#define LIBFLATSURF_FLAT_TRIANGULATION_IMPL_HPP

#include <memory>

#include "../../flatsurf/flat_triangulation.hpp"
#include "../../flatsurf/half_edge_map.hpp"
#include "../../flatsurf/vector.hpp"

namespace flatsurf {

template <typename T>
class Implementation<FlatTriangulation<T>> {
 public:
  using Vector = typename FlatTriangulation<T>::Vector;

  Implementation(const FlatTriangulationCombinatorial&, const std::function<Vector(HalfEdge)>&);

  static void updateAfterFlip(HalfEdgeMap<Vector>& vectors, HalfEdge flip);
  static void updateApproximationAfterFlip(HalfEdgeMap<flatsurf::Vector<exactreal::Arb>>& vectors, HalfEdge flip);

  static void check(const FlatTriangulation<T>&);

  static T area(const Vector& a, const Vector& b, const Vector& c);

  const HalfEdgeMap<Vector> vectors;
  // A cache of approximations for improved performance
  const HalfEdgeMap<flatsurf::Vector<exactreal::Arb>> approximations;
};

}  // namespace flatsurf

#endif
