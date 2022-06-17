/**********************************************************************
 *  This file is part of flatsurf.
 *
 *        Copyright (C) 2019-2020 Julian Rüth
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

#ifndef LIBFLATSURF_VERTICAL_IMPL_HPP
#define LIBFLATSURF_VERTICAL_IMPL_HPP

#include <functional>
#include <unordered_set>

#include "../../flatsurf/vertical.hpp"
#include "../../flatsurf/tracked.hpp"
#include "managed_movable.impl.hpp"
#include "read_only.hpp"

namespace flatsurf {

template <typename Surface>
class ImplementationOf<Vertical<Surface>> : ImplementationOf<ManagedMovable<Vertical<Surface>>> {
  using Vertical = ::flatsurf::Vertical<Surface>;
  using T = typename Surface::Coordinate;

 public:
  ImplementationOf(const Surface&, const Vector<T>&);

  // Starting at the half edge `start`, walk the surface and collect all its
  // half edges in `component`. We walk the surface by crossing any half
  // edges that are not vertical. For each half edge, `visitor` is called
  // which when returning false, aborts the process. (i.e., `visitor` found
  // a problem in the surface, such as a large edge that needs to be flipped
  // first.)
  static bool visit(const Vertical& self, HalfEdge start, std::unordered_set<HalfEdge>& component, std::function<bool(HalfEdge)> visitor);

  ReadOnly<Surface> surface;
  Vector<T> vertical;
  Vector<T> horizontal;

  mutable Tracked<OddHalfEdgeMap<std::optional<T>>> parallelProjectionCache;
  mutable Tracked<OddHalfEdgeMap<std::optional<T>>> perpendicularProjectionCache;
  mutable Tracked<OddHalfEdgeMap<std::optional<CCW>>> ccwCache;
  mutable Tracked<OddHalfEdgeMap<std::optional<ORIENTATION>>> orientationCache;
  mutable Tracked<EdgeMap<std::optional<T>>> lengthCache;
  mutable Tracked<EdgeMap<std::optional<bool>>> largenessCache;

 private:
  using ImplementationOf<ManagedMovable<Vertical>>::from_this;
  using ImplementationOf<ManagedMovable<Vertical>>::self;

  friend ReadOnly<Vertical>;
  friend ImplementationOf<ManagedMovable<Vertical>>;
};

}  // namespace flatsurf

#endif
