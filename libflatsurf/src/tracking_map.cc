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

#include <algorithm>
#include <ostream>

#include <boost/algorithm/string/join.hpp>
#include <boost/lexical_cast.hpp>

#include "../flatsurf/flat_triangulation_combinatorial.hpp"
#include "../flatsurf/tracking.hpp"
#include "../flatsurf/tracking_map.hpp"
#include "../flatsurf/vertex.hpp"

#include "util/assert.ipp"
#include "util/instantiate.ipp"

#include "impl/flat_triangulation_combinatorial.impl.hpp"
#include "impl/tracking_map.impl.hpp"
#include "impl/vertex.impl.hpp"

using std::ostream;
using std::string;

namespace flatsurf {

template <typename K, typename V>
TrackingMap<K, V>::TrackingMap(const FlatTriangulationCombinatorial* parent, const std::function<V(const K&)>& values, const FlipHandler& updateAfterFlip, const CollapseHandler& updateBeforeCollapse) :
  impl(spimpl::make_unique_impl<Implementation>(this, parent, values, updateAfterFlip, updateBeforeCollapse)) {
}

template <typename K, typename V>
TrackingMap<K, V>::TrackingMap(const TrackingMap& rhs) :
  impl(spimpl::make_unique_impl<Implementation>(
      this, &rhs.parent(), [&](const K& key) { return rhs.get(key); }, rhs.impl->updateAfterFlip, rhs.impl->updateBeforeCollapse)) {
}

template <typename K, typename V>
TrackingMap<K, V>::TrackingMap(TrackingMap&& rhs) :
  TrackingMap(rhs) {}

template <typename K, typename V>
TrackingMap<K, V>::~TrackingMap() {}

template <typename K, typename V>
void TrackingMap<K, V>::noCollapse(TrackingMap&, Edge) {
  throw std::logic_error("this TrackingMap does not support collapsing of edges");
}

template <typename K, typename V>
const FlatTriangulationCombinatorial& TrackingMap<K, V>::parent() const {
  return impl->tracker.parent();
}

template <typename K, typename V>
void TrackingMap<K, V>::apply(std::function<void(const K&, const V&)> map) const {
  // TODO: The parent has to be alive for apply(). That's annoying.
  for (const auto& key : impl->keys())
    map(key, impl->get(key));
}

template <typename K, typename V>
void TrackingMap<K, V>::set(const K& key, const V& value) {
  impl->set(key, value);
}

template <typename K, typename V>
bool TrackingMap<K, V>::operator==(const TrackingMap<K, V>& rhs) const {
  return *impl == *rhs.impl;
}

template <typename K, typename V>
// TODO: Should be called at()
std::conditional_t<TrackingMap<K, V>::odd, const V&, V&> TrackingMap<K, V>::get(const K& key) const {
  return impl->get(key);
}

template <typename K, typename V>
void TrackingMap<K, V>::noFlip(TrackingMap&, HalfEdge) {
  throw std::logic_error("this TrackingMap does not support edge flips");
}

template <typename K, typename V>
ostream& operator<<(ostream& os, const TrackingMap<K, V>& self) {
  std::vector<string> items;
  self.apply([&](const K& key, const V& value) {
    string v = boost::lexical_cast<string>(value);
    if (v == "" || v == "0") return;

    string k = boost::lexical_cast<string>(key);
    if constexpr (TrackingMap<K, V>::odd) {
      if (k.rfind("-", 0) == 0) {
        assert(value == -self.get(-key) && "map symmetry is broken");
        return;
      }
    }
    items.push_back(k + ": " + v);
  });
  return os << boost::algorithm::join(items, ", ");
}

}  // namespace flatsurf

namespace std {

template <typename K, typename V>
size_t hash<::flatsurf::TrackingMap<K, V>>::operator()(const ::flatsurf::TrackingMap<K, V>&) const noexcept {
  // TODO: Implement something reasonable and fast here
  return 0;
}

}  // namespace std

// Instantiations of templates so implementations are generated for the linker
#include "../flatsurf/edge.hpp"
#include "../flatsurf/vector.hpp"
#include "../flatsurf/vertex.hpp"
#include "impl/collapsed_half_edge.hpp"
#include "impl/flat_triangulation_collapsed.impl.hpp"

#include "util/instantiate.ipp"

#define LIBFLATSURF_INSTANTIATE_ONE(T)           \
  LIBFLATSURF_INSTANTIATE_WITH_IMPLEMENTATION(T) \
  LIBFLATSURF_INSTANTIATE_HASH(T)

#define LIBFLATSURF_INSTANTIATE_THIS(T)                                                    \
  LIBFLATSURF_INSTANTIATE_ONE((TrackingMap<HalfEdge, Vector<T>>))                          \
  LIBFLATSURF_INSTANTIATE_ONE((TrackingMap<Edge, Vector<T>>))                              \
  LIBFLATSURF_INSTANTIATE_ONE((TrackingMap<HalfEdge, T>))                                  \
  LIBFLATSURF_INSTANTIATE_ONE((TrackingMap<Edge, T>))                                      \
  LIBFLATSURF_INSTANTIATE_ONE((TrackingMap<Edge, SaddleConnection<FlatTriangulation<T>>>)) \
  LIBFLATSURF_INSTANTIATE_ONE((TrackingMap<HalfEdge, CollapsedHalfEdge<T>>))               \
  LIBFLATSURF_INSTANTIATE_ONE((TrackingMap<HalfEdge, typename Implementation<FlatTriangulationCollapsed<T>>::AsymmetricConnection>))

LIBFLATSURF_INSTANTIATE_MANY((LIBFLATSURF_INSTANTIATE_THIS), LIBFLATSURF_REAL_TYPES)

LIBFLATSURF_INSTANTIATE((LIBFLATSURF_INSTANTIATE_ONE), (TrackingMap<Edge, int>))
LIBFLATSURF_INSTANTIATE((LIBFLATSURF_INSTANTIATE_ONE), (TrackingMap<HalfEdge, int>))
LIBFLATSURF_INSTANTIATE((LIBFLATSURF_INSTANTIATE_ONE), (TrackingMap<HalfEdge, Vector<exactreal::Arb>>))
