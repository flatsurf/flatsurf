/**********************************************************************
 *  This file is part of flatsurf.
 *
 *        Copyright (C) 2020 Julian Rüth
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

#ifndef LIBFLATSURF_ODD_HALF_EDGE_MAP_HPP
#define LIBFLATSURF_ODD_HALF_EDGE_MAP_HPP

#include <ostream>

#include "edge.hpp"
#include "flat_triangulation_combinatorial.hpp"
#include "half_edge.hpp"
#include "half_edge_map.hpp"

namespace flatsurf {

namespace {
template <typename T>
struct is_optional : std::false_type {};

template <typename T>
struct is_optional<std::optional<T>> : std::true_type {};
}  // namespace

// A dictionary mapping each half edge of a triangulation to a T such that if e
// maps to x, then -e maps to -x.
template <typename T>
class OddHalfEdgeMap {
 public:
  OddHalfEdgeMap(const FlatTriangulationCombinatorial& surface) :
    values(surface) {}

  OddHalfEdgeMap(const FlatTriangulationCombinatorial& surface, std::function<T(HalfEdge)> values) :
    values(surface, values) {}

  const T& get(HalfEdge he) const {
    return values[he];
  }

  void set(HalfEdge he, const T& value) {
    values[he] = value;
    values[-he] = -value;
  }

  size_t size() const { return values.size(); }

  void pop() { values.pop(); }

  friend std::ostream& operator<<(std::ostream& os, const OddHalfEdgeMap& self) {
    bool first = true;
    for (size_t i = 0; i < self.values.size(); i++) {
      const HalfEdge he = HalfEdge::fromIndex(i);

      if (he != Edge(he).negative() || self.values[he] != -self.values[-he]) {
        if (!first) os << ", ";
        os << he << ": " << self.values[he];
        first = false;
      }
    }

    return os;
  }

 private:
  HalfEdgeMap<T> values;
};

}  // namespace flatsurf

#endif
