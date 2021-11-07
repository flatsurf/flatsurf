/**********************************************************************
 *  This file is part of flatsurf.
 *
 *        Copyright (C) 2020-2021 Julian Rüth
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

#ifndef LIBFLATSURF_TRACKED_HPP
#define LIBFLATSURF_TRACKED_HPP

#include <functional>
#include <iosfwd>

#include "movable.hpp"

namespace flatsurf {

// Makes a T react to changes in a surface such as flipping of edges.
template <typename T>
class Tracked {
  template <typename... Args>
  Tracked(PrivateConstructor, Args&&...);

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

  [[deprecated("Do not copy a Tracked<T>. If you really need a copy, create a copy from the underlying value.")]] Tracked(const Tracked&) noexcept;
  Tracked(Tracked&&) noexcept;
  Tracked(const FlatTriangulationCombinatorial&, T value, const FlipHandler& updateAfterFlip = defaultFlip, const CollapseHandler& updateBeforeCollapse = defaultCollapse, const SwapHandler& updateBeforeSwap = defaultSwap, const EraseHandler& updateBeforeErase = defaultErase, const DestructionHandler& updateBeforeDestruction = forgetParent);

  operator T&();
  operator const T&() const;
  operator T&&() &&;
  const T* operator->() const;
  T* operator->();
  const T& operator*() const;
  T& operator*();

  [[deprecated("Do not copy a Tracked<T>. If you really need a copy, create a copy from the underlying value.")]] Tracked& operator=(const Tracked&) noexcept;
  Tracked& operator=(Tracked&&) noexcept;

  Tracked& operator=(T&&);

  ~Tracked();

  template <typename S>
  friend std::ostream& operator<<(std::ostream&, const Tracked<S>&);

  static void defaultFlip(T&, const FlatTriangulationCombinatorial&, HalfEdge);
  static void defaultCollapse(T&, const FlatTriangulationCombinatorial&, Edge);
  static void defaultSwap(T&, const FlatTriangulationCombinatorial&, HalfEdge, HalfEdge);
  static void defaultErase(T&, const FlatTriangulationCombinatorial&, const std::vector<Edge>&);
  static void forgetParent(T&, const FlatTriangulationCombinatorial&);

 private:
  Movable<Tracked> self;

  friend ImplementationOf<Tracked>;
};

}  // namespace flatsurf

#endif
