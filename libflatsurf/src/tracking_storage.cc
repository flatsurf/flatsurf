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

#include "../flatsurf/edge.hpp"
#include "../flatsurf/flat_triangulation_combinatorial.hpp"
#include "../flatsurf/half_edge.hpp"
#include "../flatsurf/vertex.hpp"

#include "impl/tracking_storage.hpp"
#include "impl/vertex.impl.hpp"

#include "util/assert.ipp"

namespace flatsurf {

template <typename SELF, typename K, typename V>
TrackingStorage<SELF, K, V>::TrackingStorage(SELF* self, const FlatTriangulationCombinatorial* parent, const std::function<V(const K&)>& values, const typename SELF::FlipHandler& updateAfterFlip, const typename SELF::CollapseHandler& updateBeforeCollapse) :
  updateAfterFlip(updateAfterFlip),
  updateBeforeCollapse(updateBeforeCollapse),
  tracker(parent, self, wrappedUpdateAfterFlip, wrappedUpdateBeforeCollapse, updateBeforeSwap, updateBeforeErase) {
  const auto& keys = TrackingStorage::keys(*parent);
  if constexpr (hasIndex) {
    data.reserve(keys.size());
    for (const auto& key : keys) {
      ASSERT(key.index() == data.size(), "sort order not consistent with index()");
      data.emplace_back(values(key));
    }
  } else {
    for (const auto& key : keys) {
      ASSERT(data.find(key) == data.end(), "keys of these items are not unique");
      data.emplace(key, values(key));
    }
  }
}

template <typename SELF, typename K, typename V>
typename TrackingStorage<SELF, K, V>::Reference TrackingStorage<SELF, K, V>::get(const K& key) {
  if constexpr (hasIndex) {
    return data[key.index()];
  } else {
    return data.at(key);
  }
}

template <typename SELF, typename K, typename V>
void TrackingStorage<SELF, K, V>::set(const K& key, const V& value) {
  set(key, V(value));
}

template <typename SELF, typename K, typename V>
void TrackingStorage<SELF, K, V>::set(const K& key, V&& value) {
  if constexpr (hasIndex) {
    ASSERT(key.index() < data.size(), "not in this surface");
    data[key.index()] = {value};
    if constexpr (odd)
      data[(-key).index()] = {-value};
  } else {
    auto it = data.find(key);
    ASSERT(it != data.end(), "not in this surface");
    it->second = value;
    if constexpr (odd) {
      it = data.find(-key);
      ASSERT(it != data.end(), "not in this surface");
      it->second = -std::move(value);
    }
  }
}

template <typename SELF, typename K, typename V>
void TrackingStorage<SELF, K, V>::rekey(const std::function<bool(const K&)>& search, const std::function<bool(K&)>& adapt) {
  if constexpr (std::is_same_v<K, Vertex>) {
    std::vector<typename Data::iterator> matches;
    for (auto it = data.begin(); it != data.end(); it++) {
      if (search(it->first))
        matches.push_back(it);
    }

    std::vector<typename Data::node_type> extracted;
    for (auto& it : matches)
      extracted.emplace_back(data.extract(it));

    for (auto& it : extracted)
      if (adapt(it.key()))
        data.insert(std::move(it));
  } else {
    throw std::logic_error("not implemented: rekey()");
  }
}

template <typename SELF, typename K, typename V>
const std::vector<K>& TrackingStorage<SELF, K, V>::keys(const FlatTriangulationCombinatorial& parent) {
  if constexpr (std::is_same_v<K, HalfEdge>) {
    return parent.halfEdges();
  } else if constexpr (std::is_same_v<K, Edge>) {
    return parent.edges();
  } else if constexpr (std::is_same_v<K, Vertex>) {
    return parent.vertices();
  } else {
    throw std::logic_error("not implemented: keys()");
  }
}

template <typename SELF, typename K, typename V>
std::vector<K> TrackingStorage<SELF, K, V>::keys() const {
  std::vector<K> keys;
  // TODO: Maybe faster if we use the parent's specialized method when the
  // parent is still around (and it is almost always.)
  if constexpr (std::is_same_v<K, HalfEdge>) {
    for (size_t i = 0; i < data.size(); i++) {
      const int edge = static_cast<int>(i) / 2 + 1;
      keys.push_back(HalfEdge(i % 2 ? -edge : edge));
      ASSERT(rbegin(keys)->index() == i, "HalfEdge indexing out of sync with TrackingStorage");
    }
  } else if constexpr (std::is_same_v<K, Edge>) {
    for (size_t i = 0; i < data.size(); i++) {
      const int edge = static_cast<int>(i) + 1;
      keys.push_back(Edge(edge));
      ASSERT(rbegin(keys)->index() == i, "Edge indexing out of sync with TrackingStorage");
    }
  } else if constexpr (std::is_same_v<K, Vertex>) {
    for (auto keyValue : data)
      keys.push_back(keyValue.first);
  } else {
    throw std::logic_error("not implemented: keys()");
  }

  sort(keys.begin(), keys.end());

  return keys;
}

template <typename SELF, typename K, typename V>
void TrackingStorage<SELF, K, V>::wrappedUpdateAfterFlip(SELF& self, const FlatTriangulationCombinatorial& surface, HalfEdge flip) {
  if constexpr (std::is_same_v<K, HalfEdge>) {
    // all half edges remain valid when a flip is performed, no special action necessary
  } else if constexpr (std::is_same_v<K, Edge>) {
    // all edges remain valid when a flip is performed, no special action necessary
  } else if constexpr (std::is_same_v<K, Vertex>) {
    /*
    // The labels of the vertices attached to the old and new edge have
    // changed. So we need to migrate the data to new keys.
    std::vector<HalfEdge> boundary{
        surface.previousAtVertex(flip),
        -surface.nextAtVertex(flip),
        surface.previousAtVertex(-flip),
        -surface.nextAtVertex(-flip)};

    self.impl->rekey([&](const Vertex& vertex) {
      for (const auto& e : vertex.impl->sources)
        if (std::find(begin(boundary), end(boundary), e) != end(boundary)) return true;
      return false; }, [&](Vertex& vertex) {
      for (const auto& e : vertex.impl->sources) {
        if (std::find(begin(boundary), end(boundary), e) != end(boundary)) {
          vertex = Vertex::source(e, surface);
          return true;
        }
      }
      throw std::logic_error("failed to rename vertex after flip");
    });
    */
    throw std::logic_error("not implemented: wrappedUpdateAfterFlip()");
  } else {
    throw std::logic_error("not implemented: wrappedUpdateAfterFlip()");
  }

  self.impl->updateAfterFlip(self, flip);
}

template <typename SELF, typename K, typename V>
void TrackingStorage<SELF, K, V>::wrappedUpdateBeforeCollapse(SELF& self, const FlatTriangulationCombinatorial&, Edge collapse) {
  if constexpr (std::is_same_v<K, HalfEdge>) {
    // all half edges remain valid when a collapse is performed since the
    // deletion of the half edges is reported separatedly; no special action
    // necessary.
  } else if constexpr (std::is_same_v<K, Edge>) {
    // all edges remain valid when a collapse is performed since the deletion
    // of the half edges is reported separatedly; no special action necessary.
  } else {
    throw std::logic_error("not implemented: wrappedUpdateBeforeCollapse()");
  }

  self.impl->updateBeforeCollapse(self, collapse);
}

template <typename SELF, typename K, typename V>
void TrackingStorage<SELF, K, V>::updateBeforeSwap(SELF& self, const FlatTriangulationCombinatorial& surface, HalfEdge a, HalfEdge b) {
  if (a == b) return;

  // Enable ADL, see https://stackoverflow.com/questions/28130671/how-does-using-stdswap-enable-adl
  using std::swap;

  if constexpr (std::is_same_v<K, HalfEdge>) {
    swap(self.impl->get(a), self.impl->get(b));
    if (a != -b) {
      swap(self.impl->get(-a), self.impl->get(-b));
    }
  } else if constexpr (std::is_same_v<K, Edge>) {
    if (a != -b)
      swap(self.impl->get(Edge(a)), self.impl->get(Edge(b)));
  } else if constexpr (std::is_same_v<K, Vertex>) {
    /*
    Vertex v = Vertex::source(a, surface);
    Vertex w = Vertex::source(b, surface);
    Vertex vv = Vertex::source(-a, surface);
    Vertex ww = Vertex::source(-b, surface);

    self.impl->rekey([&](const Vertex& vertex) {
      if (v != w) {
        if (vertex == v || vertex == w) return true;
      }

      if (vv != ww) {
        if (vertex == vv || vertex == ww) return true;
      }

      return false; }, [&](Vertex& vertex) {
      if (v != w) {
        if (vertex == v) {
          vertex.impl->sources.extract(a);
          vertex.impl->sources.insert(b);
        } else if (vertex == w) {
          vertex.impl->sources.extract(b);
          vertex.impl->sources.insert(a);
        }
      }
      if (vv != ww) {
        if (vertex == vv) {
          vertex.impl->sources.extract(-a);
          vertex.impl->sources.insert(-b);
        } else if (vertex == ww) {
          vertex.impl->sources.extract(-b);
          vertex.impl->sources.insert(-a);
        }
      }
      return true; });
    */
    throw std::logic_error("not implemented: updateBeforeSwap()");
  } else {
    throw std::logic_error("not implemented: updateBeforeSwap()");
  }
}

template <typename SELF, typename K, typename V>
void TrackingStorage<SELF, K, V>::updateBeforeErase(SELF& self, const FlatTriangulationCombinatorial&, const std::vector<Edge>& erase) {
  if (erase.size() == 0)
    return;

  auto& data = self.impl->data;

  if constexpr (std::is_same_v<K, HalfEdge>) {
    assert(std::all_of(erase.begin(), erase.end(), [&](const auto& e) { return e.positive().index() >= data.size() - 2 * erase.size() && e.negative().index() >= data.size() - 2 * erase.size(); }));
    data.resize(data.size() - 2 * erase.size(), data[0] /* ignored */);
  } else if constexpr (std::is_same_v<K, Edge>) {
    assert(std::all_of(erase.begin(), erase.end(), [&](const auto& e) { return e.index() >= data.size() - erase.size(); }));
    data.resize(data.size() - erase.size(), data[0] /* ignored */);
  } else {
    throw std::logic_error("not implemented: updateBeforeErase()");
  }
}

template <typename SELF, typename K, typename V>
bool TrackingStorage<SELF, K, V>::operator==(const TrackingStorage<SELF, K, V>& rhs) const {
  return data == rhs.data;
}

template <typename SELF, typename K, typename V>
std::ostream& operator<<(std::ostream&, const TrackingStorage<SELF, K, V>&) {
  throw std::logic_error("not implemented: os << TrackingStorage");
}

}  // namespace flatsurf

// Instantiations of templates so implementations are generated for the linker
#include <e-antic/renfxx.h>
#include <gmpxx.h>
#include <exact-real/element.hpp>
// TODO: Remove these headers everywhere once they have been added to forward.
#include <exact-real/integer_ring.hpp>
#include <exact-real/number_field.hpp>
#include <exact-real/rational_field.hpp>

#include "../flatsurf/saddle_connection.hpp"
#include "../flatsurf/tracking_map.hpp"
#include "../flatsurf/tracking_set.hpp"
#include "impl/collapsed_half_edge.hpp"
#include "impl/flat_triangulation_collapsed.impl.hpp"
#include "impl/tracking_map.impl.hpp"
#include "impl/tracking_set.impl.hpp"

#include "util/instantiate.ipp"

#define LIBFLATSURF_INSTANTIATE_THIS(T)                                                                                                                                      \
  LIBFLATSURF_INSTANTIATE_WITHOUT_IMPLEMENTATION((TrackingStorage<TrackingMap<HalfEdge, Vector<T>>, HalfEdge, Vector<T>>))                                                   \
  LIBFLATSURF_INSTANTIATE_WITHOUT_IMPLEMENTATION((TrackingStorage<TrackingMap<Edge, Vector<T>>, Edge, Vector<T>>))                                                           \
  LIBFLATSURF_INSTANTIATE_WITHOUT_IMPLEMENTATION((TrackingStorage<TrackingMap<HalfEdge, T>, HalfEdge, T>))                                                                   \
  LIBFLATSURF_INSTANTIATE_WITHOUT_IMPLEMENTATION((TrackingStorage<TrackingMap<Edge, T>, Edge, T>))                                                                           \
  LIBFLATSURF_INSTANTIATE_WITHOUT_IMPLEMENTATION((TrackingStorage<TrackingMap<Edge, SaddleConnection<FlatTriangulation<T>>>, Edge, SaddleConnection<FlatTriangulation<T>>>)) \
  LIBFLATSURF_INSTANTIATE_WITHOUT_IMPLEMENTATION((TrackingStorage<TrackingMap<HalfEdge, CollapsedHalfEdge<T>>, HalfEdge, CollapsedHalfEdge<T>>))                             \
  LIBFLATSURF_INSTANTIATE_WITHOUT_IMPLEMENTATION((TrackingStorage<TrackingMap<HalfEdge, typename Implementation<FlatTriangulationCollapsed<T>>::AsymmetricConnection>, HalfEdge, typename Implementation<FlatTriangulationCollapsed<T>>::AsymmetricConnection>))

LIBFLATSURF_INSTANTIATE_MANY((LIBFLATSURF_INSTANTIATE_THIS), LIBFLATSURF_REAL_TYPES)

LIBFLATSURF_INSTANTIATE((LIBFLATSURF_INSTANTIATE_WITHOUT_IMPLEMENTATION), (TrackingStorage<TrackingSet<HalfEdge>, HalfEdge, bool>))
LIBFLATSURF_INSTANTIATE((LIBFLATSURF_INSTANTIATE_WITHOUT_IMPLEMENTATION), (TrackingStorage<TrackingSet<Edge>, Edge, bool>))
LIBFLATSURF_INSTANTIATE((LIBFLATSURF_INSTANTIATE_WITHOUT_IMPLEMENTATION), (TrackingStorage<TrackingMap<Edge, int>, Edge, int>))
LIBFLATSURF_INSTANTIATE((LIBFLATSURF_INSTANTIATE_WITHOUT_IMPLEMENTATION), (TrackingStorage<TrackingMap<HalfEdge, int>, HalfEdge, int>))
LIBFLATSURF_INSTANTIATE((LIBFLATSURF_INSTANTIATE_WITHOUT_IMPLEMENTATION), (TrackingStorage<TrackingMap<HalfEdge, Vector<exactreal::Arb>>, HalfEdge, Vector<exactreal::Arb>>))
