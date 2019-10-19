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

#ifndef LIBFLATSURF_VERTEX_HPP
#define LIBFLATSURF_VERTEX_HPP

#include <boost/operators.hpp>

#include "flatsurf/forward.hpp"
#include "flatsurf/half_edge.hpp"

namespace flatsurf {
// A thin wrapper of a HalfEdge that gives us a notion of
// a vertex, i.e., the end point of a half edge.
// We could use HalfEdge instead of Vertex with the
// implied convention that we always mean the starting
// vertex of that half edge. However, it is semantically
// nicer to have a distinct notion. Also this gives us a
// proper operator== for vertices.
class Vertex : boost::equality_comparable<Vertex> {
 public:
  static Vertex source(const HalfEdge &, const FlatTriangulationCombinatorial &);
  static Vertex target(const HalfEdge &, const FlatTriangulationCombinatorial &);

  // Note that this operator fails to distinguish equally labelled vertices on different surfaces.
  bool operator==(const Vertex &) const;

  friend std::ostream &operator<<(std::ostream &, const Vertex &);

 private:
  Vertex(const HalfEdge &);

  // The actual vertex is the start of this half edge.
  // Made a unique representative by taking the one whose negative is a
  // boundary or the half edge with this property with the smallest id if there
  // is no boundary.
  HalfEdge representative;

  friend FlatTriangulationCombinatorial;

  friend cereal::access;
  template <typename Archive>
  void save(Archive &) const;
  template <typename Archive>
  void load(Archive &);
};
}  // namespace flatsurf

#endif
