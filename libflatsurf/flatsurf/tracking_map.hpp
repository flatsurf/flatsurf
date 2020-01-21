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

#ifndef LIBFLATSURF_TRACKING_MAP_HPP
#define LIBFLATSURF_TRACKING_MAP_HPP

#include <functional>
#include <iosfwd>

#include <boost/operators.hpp>
#include <boost/type_traits/is_detected_exact.hpp>

#include "flat_triangulation_combinatorial.hpp"
#include "edge.hpp"

namespace flatsurf {
template<typename K, typename V>
class TrackingMap : boost::equality_comparable<TrackingMap<K, V>> {
  template <typename T>
  using negate_t = decltype(-std::declval<T>());

 public:
  static_assert(!std::is_same_v<V, bool>, "use TrackingSet instead");

  // A callback of this type is invoked after an edge is flipped.
  using FlipHandler = std::function<void(TrackingMap&, HalfEdge flip)>;
  // A callback of this type is invoked before an edge is collapsed.
  using CollapseHandler = std::function<void(TrackingMap&, Edge collapse)>;

  // In an odd map, map[-k] == -map[k] automatically holds; most prominently
  // this happens for a HalfEdgeMap<Vector> since both the keys (HalfEdge) and
  // the values (Vector) support the operator-.
  static constexpr bool odd = boost::is_detected_exact_v<K, negate_t, K> 
    && boost::is_detected_exact_v<V, negate_t, V>;

  // The parent does not need to remain valid. If it is destructed, it will
  // signal so that we removes its reference to it.
  TrackingMap(const FlatTriangulationCombinatorial *parent, const std::function<V(const K&)>& values, const FlipHandler &updateAfterFlip=noFlip, const CollapseHandler &updateBeforeCollapse=noCollapse);

  TrackingMap(const TrackingMap&);
  TrackingMap(TrackingMap&&);
  TrackingMap& operator=(TrackingMap&&);
  TrackingMap& operator=(const TrackingMap&) = delete;

  ~TrackingMap();

  void set(const K&, const V&);
  std::conditional_t<odd, const V&, V&> get(const K&) const;

  void apply(std::function<void(const K&, const V&)>) const;

  template <typename KK, typename VV>
  friend std::ostream &operator<<(std::ostream&, const TrackingMap<KK, VV>&);

  bool operator==(const TrackingMap&) const;

  const FlatTriangulationCombinatorial& parent() const;

 // TODO: I need to find a better solution for this.
 public:
  using Implementation = ::flatsurf::Implementation<TrackingMap>;
  spimpl::unique_impl_ptr<Implementation> impl;

  static void noFlip(TrackingMap&, HalfEdge);
  static void noCollapse(TrackingMap&, Edge);
};
}  // namespace flatsurf

namespace std {

template <typename K, typename V>
struct hash<::flatsurf::TrackingMap<K, V>> { size_t operator()(const ::flatsurf::TrackingMap<K, V>&) const noexcept; };

}

#endif



