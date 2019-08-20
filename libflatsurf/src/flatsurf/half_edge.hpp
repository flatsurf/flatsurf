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

#ifndef LIBFLATSURF_HALF_EDGE_HPP
#define LIBFLATSURF_HALF_EDGE_HPP

#include <stddef.h>
#include <boost/operators.hpp>
#include <iosfwd>
#include <string>

#include "flatsurf/forward.hpp"

namespace flatsurf {
// Similar to Edge this is a wrapper to get type-safe HalfEdges without any
// runtime overhead (at least when compiling with -flto.)
class HalfEdge : boost::equality_comparable<HalfEdge>,
                 boost::less_than_comparable<HalfEdge> {
 public:
  friend class Edge;
  template <typename T>
  friend class HalfEdgeMap;
  friend class FlatTriangulationCombinatorial;
  friend class Vertex;

  HalfEdge();
  HalfEdge(const HalfEdge &edge) = default;
  explicit HalfEdge(const int id);

  HalfEdge operator-() const;
  HalfEdge &operator=(const HalfEdge &other);
  bool operator==(const HalfEdge &other) const;
  bool operator<(const HalfEdge &other) const;
  friend std::ostream &operator<<(std::ostream &, const HalfEdge &);
  Edge edge() const;

 private:
  int id;

  friend cereal::access;
  template <typename Archive>
  int save_minimal(Archive &) const;
  template <typename Archive>
  void load_minimal(Archive &, const int &);
};
}  // namespace flatsurf

#endif
