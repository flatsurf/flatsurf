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

#ifndef LIBFLATSURF_FLAT_TRIANGULATION_COMBINATORIAL_HPP
#define LIBFLATSURF_FLAT_TRIANGULATION_COMBINATORIAL_HPP

#include <iosfwd>
#include <memory>
#include <vector>
#include "external/spimpl/spimpl.h"

#include "flatsurf/forward.hpp"

namespace flatsurf {
class FlatTriangulationCombinatorial {
 public:
  FlatTriangulationCombinatorial(const std::vector<std::vector<int>> &vertices);
  FlatTriangulationCombinatorial(const Permutation<HalfEdge> &vertices);
  FlatTriangulationCombinatorial(FlatTriangulationCombinatorial &&);

  // Create an unrelated clone of this triangulation that is built from the
  // same data. There is no copy-constructor since it is too likely that
  // this is would not update the associated HalfEdgeMaps in the way that the
  // caller expects.
  FlatTriangulationCombinatorial clone() const;

  HalfEdge nextAtVertex(HalfEdge e) const;
  HalfEdge nextInFace(HalfEdge e) const;

  const std::vector<HalfEdge> &halfEdges() const;
  const std::vector<Vertex> &vertices() const;
  // Return the outgoing half edges from this vertex in ccw order.
  std::vector<HalfEdge> atVertex(Vertex) const;

  void flip(HalfEdge);

  friend std::ostream &operator<<(std::ostream &, const FlatTriangulationCombinatorial &);

 private:
  class Implementation;
  spimpl::unique_impl_ptr<Implementation> impl;

  template <typename T>
  friend class HalfEdgeMap;

  template <typename T>
  void registerMap(const HalfEdgeMap<T> &) const;
  template <typename T>
  void deregisterMap(const HalfEdgeMap<T> &) const;
};
}  // namespace flatsurf

#endif
