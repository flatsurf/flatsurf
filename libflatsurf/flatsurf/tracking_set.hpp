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

#ifndef LIBFLATSURF_TRACKING_SET_HPP
#define LIBFLATSURF_TRACKING_SET_HPP

#include <functional>
#include <iosfwd>

#include <boost/operators.hpp>

#include "external/spimpl/spimpl.h"

#include "forward.hpp"

namespace flatsurf {
template<typename K>
class TrackingSet : boost::equality_comparable<TrackingSet<K>> {
 public:
  // A callback of this type is invoked after an edge is flipped.
  using FlipHandler = std::function<void(TrackingSet&, HalfEdge flip)>;
  // A callback of this type is invoked before an edge is collapsed.
  using CollapseHandler = std::function<void(TrackingSet&, Edge collapse)>;

  static constexpr bool odd = false;

  // The parent does not need to remain valid. If it is destructed, it will
  // signal so that we removes its reference to it.
  TrackingSet(const FlatTriangulationCombinatorial *parent, const std::function<bool(const K&)>& values, const FlipHandler &updateAfterFlip=noFlip, const CollapseHandler &updateBeforeCollapse=noCollapse);

  TrackingSet(const TrackingSet&);
  TrackingSet(TrackingSet&&);
  TrackingSet& operator=(TrackingSet&&);
  TrackingSet& operator=(const TrackingSet&) = delete;

  ~TrackingSet();

  void insert(const K&);
  void erase(const K&);
  bool contains(const K&) const;

  void apply(std::function<void(const K&)>) const;

  template <typename KK>
  friend std::ostream &operator<<(std::ostream&, const TrackingSet<KK>&);

  bool operator==(const TrackingSet&) const;

  const FlatTriangulationCombinatorial &parent() const;

 // TODO: I need to find a better solution for this.
 public:
  class Implementation;
  spimpl::unique_impl_ptr<Implementation> impl;

  static void noFlip(TrackingSet&, HalfEdge);
  static void noCollapse(TrackingSet&, Edge);
};
}  // namespace flatsurf

#endif




