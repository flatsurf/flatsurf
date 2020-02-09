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

#ifndef LIBFLATSURF_TRACKING_IMPL_HPP
#define LIBFLATSURF_TRACKING_IMPL_HPP

#include "../external/slimsig/include/slimsig/slimsig.h"

#include "../../flatsurf/tracking.hpp"

#include "flat_triangulation_combinatorial.impl.hpp"

namespace flatsurf {

template <typename T>
class Implementation<Tracking<T>> {
  void disconnect();
  void connect();

  using Message = typename ::flatsurf::Implementation<FlatTriangulationCombinatorial>::Message;

 public:
  using FlipHandler = typename Tracking<T>::FlipHandler;
  using CollapseHandler = typename Tracking<T>::CollapseHandler;
  using SwapHandler = typename Tracking<T>::SwapHandler;
  using EraseHandler = typename Tracking<T>::EraseHandler;
  using DestructionHandler = typename Tracking<T>::DestructionHandler;

  Implementation(const FlatTriangulationCombinatorial* parent, T* value, const FlipHandler& updateAfterFlip = Tracking<T>::noFlip, const CollapseHandler& updateBeforeCollapse = Tracking<T>::noCollapse, const SwapHandler& updateBeforeSwap = Tracking<T>::noSwap, const EraseHandler& updateBeforeErase = Tracking<T>::noErase, const DestructionHandler& updateBeforeDestruction = Tracking<T>::forgetParent);
  ~Implementation();

  // We keep a reference to the triangulation that we were created with so that
  // we can notify it on destruction that we do not need to be informed about
  // future flips/collapses anymore.
  // Note that this reference can never be dangling as our parent will instruct
  // us about its destruction in its destructor.
  mutable const FlatTriangulationCombinatorial* parent;

  T* value;

  const FlipHandler updateAfterFlip;
  const CollapseHandler updateBeforeCollapse;
  const SwapHandler updateBeforeSwap;
  const EraseHandler updateBeforeErase;
  const DestructionHandler updateBeforeDestruction;

  typename slimsig::signal<void(Message)>::connection onChange;
};

}  // namespace flatsurf

#endif
