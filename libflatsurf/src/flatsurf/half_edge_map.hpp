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

#ifndef LIBFLATSURF_HALF_EDGE_MAP_HPP
#define LIBFLATSURF_HALF_EDGE_MAP_HPP

#include <boost/operators.hpp>
#include <functional>
#include <iosfwd>
#include <vector>

#include "flatsurf/forward.hpp"
#include "flatsurf/half_edge.hpp"

namespace flatsurf {
// A map HalfEdge -> T such that map[-edge] == -map[edge] always holds
// automatically. Also, instances are automatically updated when an edge is
// flipped.
template <typename T>
class HalfEdgeMap final : boost::equality_comparable<HalfEdgeMap<T>> {
 public:
  using FlipHandler = std::function<void(HalfEdgeMap &, HalfEdge, const FlatTriangulationCombinatorial &)>;
  // The parent does not need to remain valid. If it is destructed, it will signal the HalfEdgeMap so that it removes its reference to it.
  HalfEdgeMap(const FlatTriangulationCombinatorial *parent, const std::vector<T> &values, const FlipHandler &updateAfterFlip);
  // The parent does not need to remain valid. If it is destructed, it will signal the HalfEdgeMap so that it removes its reference to it.
  HalfEdgeMap(const FlatTriangulationCombinatorial *parent, const FlipHandler &updateAfterFlip);
  HalfEdgeMap(const HalfEdgeMap &, const FlipHandler &updateAfterFlip);
  HalfEdgeMap(const HalfEdgeMap &);
  HalfEdgeMap(HalfEdgeMap &&);
  ~HalfEdgeMap();

  const T &get(HalfEdge key) const;
  void set(HalfEdge key, const T &value);
  void apply(std::function<void(HalfEdge, const T &)>) const;

  template <typename S>
  friend std::ostream &operator<<(std::ostream &, const HalfEdgeMap<S> &);

  HalfEdgeMap &operator=(const HalfEdgeMap &) = delete;
  HalfEdgeMap &operator=(HalfEdgeMap &&) = delete;
  HalfEdgeMap operator-() const noexcept;

  static size_t index(const HalfEdge);

 private:
  friend FlatTriangulationCombinatorial;

  // We keep a reference to the triangulation that we were created with so that
  // we can notify it on costruction that we do not need to be informed about
  // future flips anymore.
  // Note that this reference can never be invalid as our parent will instruct
  // us about its destruction in its destructor.
  mutable const FlatTriangulationCombinatorial *parent;

  mutable std::vector<T> values;
  const FlipHandler updateAfterFlip;
};
}  // namespace flatsurf

#endif
