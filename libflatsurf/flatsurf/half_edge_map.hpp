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

#ifndef LIBFLATSURF_HALF_EDGE_MAP_HPP
#define LIBFLATSURF_HALF_EDGE_MAP_HPP

#include <functional>
#include <ostream>
#include <vector>

#include "flat_triangulation_combinatorial.hpp"
#include "half_edge.hpp"

namespace flatsurf {

namespace {
template <typename T>
struct is_optional : std::false_type {};

template <typename T>
struct is_optional<std::optional<T>> : std::true_type {};
}  // namespace

// A dictionary mapping each half edge of a triangulation to a T.
template <typename T>
class HalfEdgeMap {
 public:
  HalfEdgeMap(const FlatTriangulationCombinatorial& surface) :
    values(surface.size() * 2) {}

  HalfEdgeMap(const FlatTriangulationCombinatorial& surface, std::function<T(HalfEdge)> values) {
    for (auto he : surface.halfEdges()) {
      assert(he.index() == this->values.size() && "halfEdges() must be sorted by index");
      this->values.push_back(values(he));
    }
  }

  HalfEdgeMap(const std::vector<T>& values) :
    values(values) {}

  T& operator[](HalfEdge he) {
    return values[he.index()];
  }

  const T& operator[](HalfEdge e) const {
    return const_cast<HalfEdgeMap&>(*this)[e];
  }

  void apply(std::function<void(HalfEdge, const T&)> f) const {
    for (size_t i = 0; i < values.size(); i++)
      f(HalfEdge::fromIndex(i), values[i]);
  }

  void pop() {
    values.pop_back();
    values.pop_back();
  }

  size_t size() const { return values.size(); }

  friend std::ostream& operator<<(std::ostream& os, const HalfEdgeMap& self) {
    bool first = true;
    os << "{";
    for (size_t i = 0; i < self.values.size(); i++) {
      if constexpr (is_optional<T>::value)
        if (!self.values[i])
          continue;

      if (!first) os << ", ";

      os << HalfEdge::fromIndex(i) << ": ";

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
