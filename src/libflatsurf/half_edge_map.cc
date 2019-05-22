/**********************************************************************
 *  This file is part of flatsurf.
 *
 *        Copyright (C) 2019 Julian Rüth
 *
 *  Flatsurf is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 of the License, or
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

#include <boost/algorithm/string/join.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/range/combine.hpp>
#include <cassert>
#include <ostream>

#include "flatsurf/flat_triangulation_combinatorial.hpp"
#include "flatsurf/half_edge_map.hpp"

using std::function;
using std::ostream;
using std::string;
using std::vector;

namespace flatsurf {
template <typename T>
HalfEdgeMap<T>::HalfEdgeMap(const FlatTriangulationCombinatorial &parent,
                            const vector<T> &values,
                            const FlipHandler &updateAfterFlip)
    : parent(&parent), updateAfterFlip(updateAfterFlip) {
  assert(values.size() == parent.halfEdges().size() / 2 &&
         "values must contain one entry for each pair of half edges");
  for (size_t i = 0; i < values.size(); i++) {
    this->values.emplace_back(values[i]);
    this->values.emplace_back(-values[i]);
  }

  parent.registerMap(*this);
}

template <typename T>
HalfEdgeMap<T>::HalfEdgeMap(const HalfEdgeMap &rhs)
    : parent(rhs.parent),
      values(rhs.values),
      // Note that we silently assume that updateAfterFlip has no weird side
      // effects so that it's fine to run it twice when there are two copies.
      updateAfterFlip(rhs.updateAfterFlip) {
  if (parent != nullptr) {
    parent->registerMap(*this);
  }
}

template <typename T>
HalfEdgeMap<T>::HalfEdgeMap(HalfEdgeMap &&rhs)
    : parent(rhs.parent),
      values(std::move(rhs.values)),
      updateAfterFlip(rhs.updateAfterFlip) {
  if (parent != nullptr) {
    parent->registerMap(*this);
  }
}

template <typename T>
HalfEdgeMap<T>::~HalfEdgeMap() {
  // When the parent gets destructed, it sets the parent pointer in every
  // HalfEdgeMap to null. That's fine, since we only need that information when
  // performing a flip which then cannot happen anymore.
  if (parent != nullptr) {
    parent->deregisterMap(*this);
  }
}

template <typename T>
const T &HalfEdgeMap<T>::get(const HalfEdge key) const {
  return values.at(index(key));
}

template <typename T>
void HalfEdgeMap<T>::apply(function<void(HalfEdge, const T &)> callback) const {
  for (int i = 1; i <= static_cast<int>(values.size()) / 2; i++) {
    const HalfEdge e(i);
    callback(e, get(e));
  }
}

template <typename T>
void HalfEdgeMap<T>::set(const HalfEdge key, const T &value) {
  values.at(index(key)) = value;
  values.at(index(-key)) = -value;
}

template <typename T>
size_t HalfEdgeMap<T>::index(const HalfEdge e) {
  const int id = e.id;
  assert(id != 0);
  if (id < 0) {
    return static_cast<size_t>(-2 * id - 1);
  } else {
    return static_cast<size_t>(2 * id - 2);
  }
}

template <typename T>
ostream &operator<<(ostream &os, const flatsurf::HalfEdgeMap<T> &self) {
  std::vector<string> items;
  for (auto it = self.values.begin(); it != self.values.end(); it++) {
    long i = it - self.values.begin();
    if (i % 2 == 0) continue;
    string v = boost::lexical_cast<string>(*it);
    if (v == "0") continue;
    items.push_back(boost::lexical_cast<string>(i / 2 + 1) + ": " + v);
  }

  return os << boost::algorithm::join(items, ", ");
}
}  // namespace flatsurf

using namespace flatsurf;

// Instantiations of templates so implementations are generated for the linker
#include <exact-real/integer_ring_traits.hpp>
#include <exact-real/number_field_traits.hpp>
#include <exact-real/rational_field_traits.hpp>
#include "flatsurf/vector_eantic.hpp"
#include "flatsurf/vector_exactreal.hpp"
#include "flatsurf/vector_longlong.hpp"

template class flatsurf::HalfEdgeMap<VectorLongLong>;
template ostream &flatsurf::operator<<(ostream &, const HalfEdgeMap<VectorLongLong> &);
template class flatsurf::HalfEdgeMap<VectorEAntic>;
template ostream &flatsurf::operator<<(ostream &, const HalfEdgeMap<VectorEAntic> &);
template class flatsurf::HalfEdgeMap<int>;
template ostream &flatsurf::operator<<(ostream &, const HalfEdgeMap<int> &);
template class flatsurf::HalfEdgeMap<VectorExactReal<exactreal::IntegerRingTraits>>;
template ostream &flatsurf::operator<<(ostream &, const HalfEdgeMap<VectorExactReal<exactreal::IntegerRingTraits>> &);
template class flatsurf::HalfEdgeMap<VectorExactReal<exactreal::RationalFieldTraits>>;
template ostream &flatsurf::operator<<(ostream &, const HalfEdgeMap<VectorExactReal<exactreal::RationalFieldTraits>> &);
template class flatsurf::HalfEdgeMap<VectorExactReal<exactreal::NumberFieldTraits>>;
template ostream &flatsurf::operator<<(ostream &, const HalfEdgeMap<VectorExactReal<exactreal::NumberFieldTraits>> &);
