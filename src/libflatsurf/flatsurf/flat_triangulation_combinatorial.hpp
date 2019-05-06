/**********************************************************************
 *  This file is part of flatsurf.
 *
 *        Copyright (C) 2019 Julian Rüth
 *
 *  Flatsurf is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 of the License, or
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

#include "external/spimpl/spimpl.h"
#include <iosfwd>
#include <memory>
#include <vector>

#include "flatsurf/flatsurf.hpp"
#include "flatsurf/forward.hpp"

namespace flatsurf {
struct FlatTriangulationCombinatorial {
  FlatTriangulationCombinatorial(const std::vector<std::vector<int>> &vertices);
  FlatTriangulationCombinatorial(const Permutation<HalfEdge> &vertices);
  FlatTriangulationCombinatorial(FlatTriangulationCombinatorial &&);
  virtual ~FlatTriangulationCombinatorial();

  HalfEdge nextAtVertex(HalfEdge e) const;
  HalfEdge nextInFace(HalfEdge e) const;

  const std::vector<HalfEdge> &edges() const;
  const std::vector<Vertex> &vertices() const;

  void flip(HalfEdge);

  friend std::ostream &operator<<(std::ostream &, const FlatTriangulationCombinatorial &);
  const size_t nedges;

private:
  struct Implementation;
  spimpl::unique_impl_ptr<Implementation> impl;

  template <typename T>
  friend struct HalfEdgeMap;

  template <typename T>
  void registerMap(const HalfEdgeMap<T>&) const;
  template <typename T>
  void deregisterMap(const HalfEdgeMap<T>&) const;
};
} // namespace flatsurf

#endif
