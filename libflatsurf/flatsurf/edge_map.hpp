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

#ifndef LIBFLATSURF_EDGE_MAP_HPP
#define LIBFLATSURF_EDGE_MAP_HPP

#include <stdexcept>
#include <vector>

#include "edge.hpp"
#include "flat_triangulation_combinatorial.hpp"
#include "half_edge_map.hpp"

namespace flatsurf {

// A dictionary mapping each edge of a triangulation to a T.
template <typename T>
class EdgeMap {
 public:
  EdgeMap(const FlatTriangulationCombinatorial& surface) :
    values(surface.size()) {}

  T& operator[](Edge e) {
    return values[e.index()];
  }

  const T& operator[](Edge e) const {
    return const_cast<EdgeMap&>(*this)[e];
  }

  void apply(std::function<void(Edge, const T&)> f) const {
    for (size_t i = 0; i < values.size(); i++)
      f(Edge::fromIndex(i), values[i]);
  }

  void pop() {
    values.pop_back();
  }

  size_t size() const { return values.size(); }

  friend std::ostream& operator<<(std::ostream& os, const EdgeMap& self) {
    bool first = true;
    os << "{";
    for (size_t i = 0; i < self.values.size(); i++) {
      if constexpr (is_optional<T>::value)
        if (!self.values[i])
          continue;

      if (!first) os << ", ";

      os << Edge::fromIndex(i) << ": ";

      if constexpr (is_optional<T>::value)
        os << *self.values[i];
      else
        os << self.values[i];

      first = false;
    }
    return os << "}";
  }

 private:
  std::vector<T> values;
};

}  // namespace flatsurf

#endif
