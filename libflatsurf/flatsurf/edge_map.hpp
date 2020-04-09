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

#include "edge.hpp"
#include "flat_triangulation_combinatorial.hpp"

namespace flatsurf {

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

 private:
  std::vector<T> values;
};

}  // namespace flatsurf

#endif
