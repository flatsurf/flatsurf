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

#include <iostream> // TODO

#include "half_edge.hpp"
#include "flat_triangulation_combinatorial.hpp"

namespace flatsurf {

template <typename T>
class HalfEdgeMap {
 public:
  HalfEdgeMap(const FlatTriangulationCombinatorial& surface, std::function<T(HalfEdge)> values) {
    for (auto he : surface.halfEdges()) {
      assert(he.index() == this->values.size() && "halfEdges() must be sorted by index");
      this->values.push_back(values(he));
    }
  }

  HalfEdgeMap(const std::vector<T>& values) : values(values) {}

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
    os << "{";
    for (size_t i = 0; i < self.values.size(); i++)
      os << (i ? ", " : "") << HalfEdge::fromIndex(i) << ": " << self.values[i];
    return os;
  }

 private:
  std::vector<T> values;
};

}

#endif
