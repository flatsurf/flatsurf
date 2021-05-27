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
    if constexpr (is_optional<T>::value) {
      if (value) {
        // We do not use -*value here since some versions of GCC report an
        // annoying warning that value might be uninitialized then.
        values[-he] = -*values[he];
      } else
        values[-he] = std::nullopt;
    } else {
      values[-he] = -value;
    }
  }

  size_t size() const { return values.size(); }

  void pop() { values.pop(); }

  friend std::ostream& operator<<(std::ostream& os, const OddHalfEdgeMap& self) {
    bool first = true;
    os << "{";
    for (size_t i = 0; i < self.values.size(); i++) {
      const HalfEdge he = HalfEdge::fromIndex(i);

      if constexpr (is_optional<T>::value)
        if (!self.values[he])
          continue;

      if (he == Edge(he).negative())
        continue;

      if (!first) os << ", ";
      os << he << ": ";
      if constexpr (is_optional<T>::value)
        if constexpr (is_printable<typename T::value_type>::value) {
          os << *self.values[he];
        } else {
          os << "?";
        }
      else
        if constexpr (is_printable<T>::value) {
          os << self.values[he];
        } else {
          os << "?";
        }
      first = false;
    }
    return os << "}";
  }

 private:
  HalfEdgeMap<T> values;
};

}  // namespace flatsurf

#endif
