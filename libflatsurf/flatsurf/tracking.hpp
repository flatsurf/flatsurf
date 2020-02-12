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

#ifndef LIBFLATSURF_TRACKING_HPP
#define LIBFLATSURF_TRACKING_HPP

#include <functional>
#include <iosfwd>

#include "flat_triangulation_combinatorial.hpp"

// TODO: Should we have more Tracking and non-Tracking versions of things?
// Shouldn't TrackingHalfEdge inherit Tracking<HalfEdge> instead of containing
// it as a field?

namespace flatsurf {
template <typename T>
class Tracking {
 public:
  // A callback of this type is invoked after an edge is flipped.
  using FlipHandler = std::function<void(T&, const FlatTriangulationCombinatorial&, HalfEdge flip)>;
  // A callback of this type is invoked before an edge is collapsed.
  using CollapseHandler = std::function<void(T&, const FlatTriangulationCombinatorial&, Edge collapse)>;
  // A callback of this type is invoked before two edges are swapped.
  using SwapHandler = std::function<void(T&, const FlatTriangulationCombinatorial&, HalfEdge a, HalfEdge b)>;
  // A callback of this type is invoked before an edge is dropped from the surface.
  using EraseHandler = std::function<void(T&, const FlatTriangulationCombinatorial&, const std::vector<Edge>& erase)>;
  // A callback of this type is invoked when the parent surface is destructed.
  using DestructionHandler = std::function<void(T&, const FlatTriangulationCombinatorial&)>;

  // The parent does not need to remain valid. If it is destructed, it will
  // signal so that we removes its reference to it.
  // However, value must remain valid as long as this object is around.
  Tracking(const FlatTriangulationCombinatorial* parent, T* value, const FlipHandler& updateAfterFlip = noFlip, const CollapseHandler& updateBeforeCollapse = noCollapse, const SwapHandler& updateBeforeSwap = noSwap, const EraseHandler& updateBeforeErase = noErase, const DestructionHandler& updateBeforeDestruction = forgetParent);

  Tracking(const Tracking&) = delete;
  Tracking(Tracking&&);
  Tracking& operator=(Tracking&&);
  Tracking& operator=(const Tracking&) = delete;

  ~Tracking();

  T& get() const;

  operator T&();
  operator const T&() const;

  const FlatTriangulationCombinatorial& parent() const;

  template <typename S>
  friend std::ostream& operator<<(std::ostream&, const Tracking<S>&);

 private:
  using Implementation = ::flatsurf::Implementation<Tracking>;
  spimpl::unique_impl_ptr<Implementation> impl;

  static void noFlip(T&, const FlatTriangulationCombinatorial&, HalfEdge);
  static void noCollapse(T&, const FlatTriangulationCombinatorial&, Edge);
  static void noSwap(T&, const FlatTriangulationCombinatorial&, HalfEdge, HalfEdge);
  static void noErase(T&, const FlatTriangulationCombinatorial&, const std::vector<Edge>&);
  static void forgetParent(T&, const FlatTriangulationCombinatorial&);
};
}  // namespace flatsurf

#endif
