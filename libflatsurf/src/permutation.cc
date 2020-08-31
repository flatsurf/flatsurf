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

#include "../flatsurf/permutation.hpp"

#include <boost/range/numeric.hpp>
#include <cassert>
#include <ostream>
#include <unordered_set>

#include "util/assert.ipp"

using boost::accumulate;
using std::function;
using std::ostream;
using std::pair;
using std::vector;

namespace flatsurf {

namespace {
template <typename T>
size_t index(const T &t) { return t.index(); }

template <typename T>
void check(const Permutation<T> &permutation) {
  vector<int> hits(permutation.size());
  for (const auto &target : permutation.domain()) {
    ASSERT_ARGUMENT(index(target) < hits.size(), "permutation maps beyond its domain");
    hits[index(target)]++;
  }
  for (int h : hits)
    CHECK_ARGUMENT(h, "not a permutation, not surjective");
  for (int h : hits)
    CHECK_ARGUMENT(h == 1, "not a permutation, not injective");

  for (const auto &v : permutation.domain()) {
    ASSERT_ARGUMENT(permutation.preimage(permutation(v)) == v, "inverse incorrect");
    ASSERT_ARGUMENT(permutation(permutation.preimage(v)) == v, "inverse incorrect");
  }
}
}  // namespace

template <typename T>
Permutation<T>::Permutation() :
  Permutation(vector<pair<T, T>>()) {}

template <typename T>
Permutation<T>::Permutation(const vector<T> &permutation) :
  permutation(permutation),
  inverse(permutation) {
  for (auto &v : permutation)
    inverse[index((*this)(v))] = v;

  check(*this);
}

template <typename T>
Permutation<T>::Permutation(const vector<vector<T>> &cycles) :
  Permutation([&]() {
    vector<T> data(accumulate(cycles, 0u, [](size_t sum, const auto &cycle) { return sum + cycle.size(); }));
    for (const auto cycle : cycles) {
      for (auto i = 0u; i < cycle.size(); i++) {
        ASSERT_ARGUMENT(index(cycle[i]) < data.size(), "cycle contains an element beyond the size of the permutation");
        data[index(cycle[i])] = cycle[(i + 1) % cycle.size()];
      }
    }
    return data;
  }()) {}

template <typename T>
Permutation<T>::Permutation(const vector<pair<T, T>> &permutation) :
  Permutation([&]() {
    vector<T> data(permutation.size());
    for (auto ab : permutation) {
      ASSERT_ARGUMENT(index(ab.first) < data.size(), "entry of permutation points to an element beoynd the size of the permutation");
      data[index(ab.first)] = ab.second;
    }
    return data;
  }()) {}

template <typename T>
Permutation<T>::Permutation(const std::unordered_map<T, T> &permutation) :
  Permutation([&]() {
    vector<T> data(permutation.size());
    for (auto ab : permutation) {
      ASSERT_ARGUMENT(index(ab.first) < data.size(), "entry of permutation points to an element beoynd the size of the permutation");
      data[index(ab.first)] = ab.second;
    }
    return data;
  }()) {}

template <typename T>
Permutation<T> Permutation<T>::random(const vector<T> &domain) {
  ASSERT_ARGUMENT(std::unordered_set<T>(domain.begin(), domain.end()).size() == domain.size(), "domain must not contain duplicates");
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
  return permutation[index(t)];
}

template <typename T>
const T &Permutation<T>::preimage(const T &t) const {
  return inverse[index(t)];
}

template <typename T>
size_t Permutation<T>::size() const noexcept {
  return permutation.size();
}

template <typename T>
const vector<T> &Permutation<T>::domain() const noexcept {
  return permutation;
}

template <typename T>
vector<vector<T>> Permutation<T>::cycles() const noexcept {
  std::unordered_set<T> seen;
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
void Permutation<T>::drop(const vector<T> &items) {
  for (auto &item : items)
    CHECK_ARGUMENT(std::find(begin(items), end(items), this->operator()(item)) != end(items), "items to remove must be in isolated cycles");

  for (auto &item : items)
    CHECK_ARGUMENT(index(item) >= permutation.size() - items.size(), "items to remove must have maximal index");

  permutation.resize(permutation.size() - items.size());
  inverse.resize(inverse.size() - items.size());
}

template <typename T>
Permutation<T> Permutation<T>::operator~() const {
  return Permutation<T>(inverse);
}

template <typename T>
Permutation<T> &Permutation<T>::operator*=(const Permutation<T> &rhs) {
  CHECK_ARGUMENT(size() == rhs.size(), "permutations must have the same domain");

  vector<T> permutation(rhs.permutation);
  for (auto it = permutation.begin(); it != permutation.end(); it++)
    *it = (*this)(*it);
  return *this = Permutation<T>(permutation);
}

template <typename T>
bool Permutation<T>::trivial() const noexcept {
  for (auto &v : domain())
    if ((*this)(v) != v)
      return false;
  return true;
}

template <typename T>
std::vector<T> Permutation<T>::cycle(const T &first) const noexcept {
  std::vector<T> cycle{first};

  do {
    cycle.push_back(this->operator()(*rbegin(cycle)));
  } while (*begin(cycle) != *rbegin(cycle));

  cycle.pop_back();

  return cycle;
}

template <typename T>
Permutation<T> &operator*=(const vector<T> &cycle, Permutation<T> &self) {
  if (std::unordered_set<T>(cycle.begin(), cycle.end()).size() <= 1) return self;

  CHECK_ARGUMENT(std::unordered_set<T>(cycle.begin(), cycle.end()).size() == cycle.size(), "cycle must consist of distinct entries");

  T tmp = self.preimage(cycle[0]);
  for (auto it = cycle.begin(); it != cycle.end() - 1; it++) {
    self.inverse[index(*it)] = self.preimage(*(it + 1));
    self.permutation[index(self.preimage(*(it + 1)))] = *it;
  }
  self.inverse[index(*(cycle.end() - 1))] = tmp;
  self.permutation[index(tmp)] = *(cycle.end() - 1);

  return self;
}

template <typename T>
Permutation<T> &operator*=(Permutation<T> &self, const vector<T> &cycle) {
  if (std::unordered_set<T>(cycle.begin(), cycle.end()).size() <= 1) return self;

  CHECK_ARGUMENT(std::unordered_set<T>(cycle.begin(), cycle.end()).size() == cycle.size(), "cycle must consist of distinct entries");

  T tmp = self(cycle[0]);
  for (auto it = cycle.begin(); it != cycle.end() - 1; it++) {
    self.permutation[index(*it)] = self(*(it + 1));
    self.inverse[index(self(*(it + 1)))] = *it;
  }
  self.permutation[index(*(cycle.end() - 1))] = tmp;
  self.inverse[index(tmp)] = *(cycle.end() - 1);

  return self;
}

template <typename T>
bool Permutation<T>::operator==(const Permutation &rhs) const noexcept {
  return permutation == rhs.permutation;
}

template <typename T>
ostream &operator<<(ostream &os, const Permutation<T> &self) {
  if (self.size() == 0) {
    return os << "()";
  }

  std::unordered_set<T> remaining;
  for (auto t : self.permutation) {
    remaining.insert(t);
  }
  assert(remaining.size() == self.permutation.size() && "data must not contain duplicates");
  while (remaining.size()) {
    os << "(";
    // Start from smallest element in remaining so we get a consistent
    // printing. (We could do this a bit smarter by using a std::set and fmt
    // instead.)
    const T start = *std::min_element(begin(remaining), end(remaining), [](const auto &lhs, const auto &rhs) { return lhs.index() < rhs.index(); });
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
#include "../flatsurf/half_edge.hpp"

template class flatsurf::Permutation<HalfEdge>;
template ostream &flatsurf::operator<<(ostream &os, const Permutation<HalfEdge> &self);
template Permutation<HalfEdge> &flatsurf::operator*=(const vector<HalfEdge> &, Permutation<HalfEdge> &);
template Permutation<HalfEdge> &flatsurf::operator*=(Permutation<HalfEdge> &, const vector<HalfEdge> &);
