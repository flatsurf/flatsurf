/**********************************************************************
 *  This file is part of Polygon.
 *
 *        Copyright (C) 2019 Julian Rüth
 *
 *  Polygon is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  Polygon is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Polygon. If not, see <https://www.gnu.org/licenses/>.
 *********************************************************************/

#include <cassert>
#include <ostream>
#include <set>
#include "external/boolinq/include/boolinq/boolinq.h"

#include "libpolygon/permutation.hpp"

using boolinq::from;
using std::function;
using std::ostream;
using std::pair;
using std::set;
using std::vector;

namespace flatsurf {

template <typename T>
Permutation<T>::Permutation(const vector<vector<T>>& cycles)
    : data(from(cycles).sum(
          [](const vector<T>& cycle) { return cycle.size(); })) {
  for (const auto cycle : cycles) {
    for (auto i = 0u; i < cycle.size(); i++) {
      data[index(cycle[i])] = cycle[(i + 1) % cycle.size()];
    }
  }
}

template <typename T>
Permutation<T>::Permutation(const vector<pair<T, T>>& permutation)
    : data(permutation.size()) {
  for (auto ab : permutation) {
    data[index(ab.first)] = ab.second;
  }
}

template <typename T>
T Permutation<T>::operator()(const T& t) const {
  return data[index(t)];
}

template <typename T>
size_t Permutation<T>::size() const {
  return data.size();
}

template <typename T>
const vector<T>& Permutation<T>::domain() const {
  return data;
}

template <typename T>
template <typename S>
Permutation<T> Permutation<T>::create(const vector<vector<S>>& cycles,
                                      const function<T(S)>& converter) {
  return Permutation(from(cycles)
                         .select([&](const vector<S>& cycle) {
                           return from(cycle)
                               .select([&](const S s) { return converter(s); })
                               .toVector();
                         })
                         .toVector());
}

template <typename T>
ostream& operator<<(ostream& os, const Permutation<T>& self) {
  set<T> remaining;
  for (auto t : self.data) {
    remaining.insert(t);
  }
  assert(remaining.size() == self.data.size());
  while (remaining.size()) {
    os << "(";
    const auto start = *remaining.begin();
    auto current = start;
    do {
      remaining.erase(current);
      if (current != start) {
        os << ", ";
      }
      os << current;
      current = self(current);
    } while (current != start);
    os << ")";
  }
  return os;
}
}  // namespace flatsurf

using namespace flatsurf;

// Instantiations of templates so implementations are generated for the linker
#include "libpolygon/half_edge.hpp"
template struct flatsurf::Permutation<HalfEdge>;
template Permutation<HalfEdge> Permutation<HalfEdge>::create<int>(
    const vector<vector<int>>& cycles,
    const function<HalfEdge(int)>& converter);
template ostream& flatsurf::operator<<(ostream& os,
                                       const Permutation<HalfEdge>& self);
