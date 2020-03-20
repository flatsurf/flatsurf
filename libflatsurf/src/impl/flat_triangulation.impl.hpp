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
#include "../../flatsurf/tracked.hpp"

namespace flatsurf {

template <typename T>
class ImplementationOf<FlatTriangulation<T>> {
 public:
  ImplementationOf(const FlatTriangulationCombinatorial&, const std::function<Vector<T>(HalfEdge)>&);

  static void updateAfterFlip(OddHalfEdgeMap<Vector<T>>&, const FlatTriangulationCombinatorial&, HalfEdge);
  static void updateApproximationAfterFlip(OddHalfEdgeMap<Vector<exactreal::Arb>>&, const FlatTriangulationCombinatorial&, HalfEdge);

  static void check(const FlatTriangulation<T>&);

  static T area(const Vector<T>& a, const Vector<T>& b, const Vector<T>& c);

  const Tracked<OddHalfEdgeMap<Vector<T>>> vectors;
  // A cache of approximations for improved performance
  const Tracked<OddHalfEdgeMap<Vector<exactreal::Arb>>> approximations;
};

}  // namespace flatsurf

#endif
