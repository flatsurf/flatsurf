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

#ifndef LIBFLATSURF_FLAT_TRIANGULATION_COMBINATORIAL_HPP
#define LIBFLATSURF_FLAT_TRIANGULATION_COMBINATORIAL_HPP

#include <vector>

#include "flat_triangulation_combinatorics.hpp"
#include "serializable.hpp"

namespace flatsurf {

// The combinatorial structure of a triangulated translation surface. The
// combinatorial structure is represented by a number of permutations such as
// the permutation of half edges at the vertices, i.e., the permutation that
// encodes how half edges are given sequentially when going around a vertex in
// counter-clockwise order.
class FlatTriangulationCombinatorial : public FlatTriangulationCombinatorics<FlatTriangulationCombinatorial>,
                                       Serializable<FlatTriangulationCombinatorial> {
  template <typename... Args>
  FlatTriangulationCombinatorial(PrivateConstructor, Args &&... args);

 public:
  FlatTriangulationCombinatorial() noexcept;
  FlatTriangulationCombinatorial(const std::vector<std::vector<int>> &vertices, const std::vector<int> &boundaries = std::vector<int>());
  FlatTriangulationCombinatorial(const Permutation<HalfEdge> &vertices);
  FlatTriangulationCombinatorial(const std::vector<std::tuple<HalfEdge, HalfEdge, HalfEdge>> &faces);

  friend ImplementationOf<FlatTriangulationCombinatorial>;
  friend Serialization<ManagedMovable<FlatTriangulationCombinatorial>>;

  friend std::ostream &operator<<(std::ostream &, const FlatTriangulationCombinatorial &);
};

}  // namespace flatsurf

#endif
