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

#include <boost/range/adaptors.hpp>
#include <boost/range/numeric.hpp>
#include <cassert>
#include <ostream>
#include <set>

#include "flatsurf/permutation.hpp"
#include "util/as_vector.ipp"
#include "util/assert.ipp"

using boost::accumulate;
using boost::adaptors::transformed;
using std::function;
using std::ostream;
using std::pair;
using std::set;
using std::vector;

namespace flatsurf {

template <typename T>
Permutation<T>::Permutation() : Permutation(std::vector<pair<T, T>>()) {}

template <typename T>
Permutation<T>::Permutation(const vector<vector<T>> &cycles)
    : data(accumulate(cycles, 0u, [](size_t sum, const auto &cycle) {
        return sum + cycle.size();
      })) {
  for (const auto cycle : cycles) {
    for (auto i = 0u; i < cycle.size(); i++) {
      ASSERT_ARGUMENT(index(cycle[i]) < data.size(), "cycle contains an element beyond the size of the permutation");
      data[index(cycle[i])] = cycle[(i + 1) % cycle.size()];
    }
  }
}

template <typename T>
Permutation<T>::Permutation(const vector<pair<T, T>> &permutation)
    : data(permutation.size()) {
  for (auto ab : permutation) {
    ASSERT_ARGUMENT(index(ab.first) < data.size(), "entry of permutation points to an element beoynd the size of the permutation");
    data[index(ab.first)] = ab.second;
  }
}

template <typename T>
Permutation<T> Permutation<T>::random(const vector<T> &domain) {
  ASSERT_ARGUMENT(std::set<T>(domain.begin(), domain.end()).size() == domain.size(), "domain must not contain duplicates");
  vector<T> image = domain;
  std::random_shuffle(image.begin(), image.end());
  vector<pair<T, T>> permutation;
  for (size_t i = 0; i < domain.size(); i++) {
    permutation.push_back(pair(domain[i], image[i]));
  }
  return Permutation<T>(permutation);
}

template <typename T>
const T &Permutation<T>::operator()(const T &t) const {
  return data[index(t)];
}

template <typename T>
size_t Permutation<T>::size() const noexcept {
  return data.size();
}

template <typename T>
const vector<T> &Permutation<T>::domain() const noexcept {
  return data;
}

template <typename T>
vector<vector<T>> Permutation<T>::cycles() const noexcept {
  std::set<T> seen;
  vector<vector<T>> cycles;
  for (const auto &t : domain()) {
    if (seen.find(t) != seen.end())
      continue;

    vector<T> cycle;
    auto s = t;
    do {
      cycle.push_back(s);
      seen.insert(s);
      s = this->operator()(s);
    } while (s != t);

    cycles.push_back(cycle);
  }

  return cycles;
}

template <typename T>
template <typename S>
Permutation<T> Permutation<T>::create(const vector<vector<S>> &cycles,
                                      const function<T(S)> &converter) {
  return Permutation(as_vector(cycles | transformed([&](const auto &cycle) {
                                 return as_vector(cycle |
                                                  transformed(converter));
                               })));
}

template <typename T>
Permutation<T> &operator*=(const vector<T> &cycle, Permutation<T> &self) {
  if (cycle.size() == 0) return self;

  T tmp = self(cycle[0]);
  for (auto it = cycle.begin() + 1; it != cycle.end(); it++) {
    self.data[self.index(*(it - 1))] = self(*it);
  }
  self.data[self.index(cycle[cycle.size() - 1])] = tmp;

  return self;
}

template <typename T>
bool Permutation<T>::operator==(const Permutation &rhs) const noexcept {
  return data == rhs.data;
}

template <typename T>
ostream &operator<<(ostream &os, const Permutation<T> &self) {
  set<T> remaining;
  for (auto t : self.data) {
    remaining.insert(t);
  }
  assert(remaining.size() == self.data.size() && "data must not contain duplicates");
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
#include "flatsurf/half_edge.hpp"

template class flatsurf::Permutation<HalfEdge>;
template Permutation<HalfEdge> Permutation<HalfEdge>::create<int>(const vector<vector<int>> &cycles, const function<HalfEdge(int)> &converter);
template ostream &flatsurf::operator<<(ostream &os, const Permutation<HalfEdge> &self);
template Permutation<HalfEdge> &flatsurf::operator*=(const vector<HalfEdge> &, Permutation<HalfEdge> &);
