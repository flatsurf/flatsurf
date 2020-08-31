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

#ifndef LIBFLATSURF_TRACKING_STORAGE_HPP
#define LIBFLATSURF_TRACKING_STORAGE_HPP

#include <boost/operators.hpp>
#include <boost/type_traits/is_detected_exact.hpp>
#include <functional>
#include <type_traits>
#include <unordered_map>
#include <vector>

#include "../../flatsurf/flat_triangulation_combinatorial.hpp"
#include "../../flatsurf/tracking.hpp"

namespace flatsurf {

template <typename K>
using index_t = decltype(std::declval<K>().index());

template <typename SELF, typename K, typename V>
class TrackingStorage {
  static constexpr bool hasIndex = boost::is_detected_exact_v<size_t, index_t, K>;
  using Data = std::conditional_t<hasIndex, std::vector<V>, std::unordered_map<K, V>>;
  static constexpr bool odd = SELF::odd;
  using Tracker = Tracking<SELF>;

  using Reference = std::conditional_t<std::is_same_v<Data, std::vector<V>>, typename std::vector<V>::reference, V&>;

 public:
  TrackingStorage(SELF* self, const FlatTriangulationCombinatorial* parent, const std::function<V(const K&)>& values, const typename SELF::FlipHandler& updateAfterFlip, const typename SELF::CollapseHandler& updateBeforeCollapse);

  bool operator==(const TrackingStorage&) const;

  Reference get(const K&);
  typename Data::const_reference get(const K&) const;
  void set(const K&, V&&);
  void set(const K&, const V&);
  void rekey(const std::function<bool(const K&)>& search, const std::function<bool(K&)>& adapt);
  void swap(const K&, const K&);

  std::vector<K> keys() const;
  static const std::vector<K>& keys(const FlatTriangulationCombinatorial&);

  const typename SELF::FlipHandler updateAfterFlip;
  const typename SELF::CollapseHandler updateBeforeCollapse;

  Tracker tracker;

 private:
  static void wrappedUpdateAfterFlip(SELF&, const FlatTriangulationCombinatorial&, HalfEdge);
  static void wrappedUpdateBeforeCollapse(SELF&, const FlatTriangulationCombinatorial&, Edge);
  static void updateBeforeSwap(SELF&, const FlatTriangulationCombinatorial&, HalfEdge, HalfEdge);
  static void updateBeforeErase(SELF&, const FlatTriangulationCombinatorial&, const std::vector<Edge>&);

  Data data;
};

}  // namespace flatsurf

#endif
