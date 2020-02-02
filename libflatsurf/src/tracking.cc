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

#include <set>
#include <ostream>

#include "impl/tracking.impl.hpp"
#include "impl/flat_triangulation_combinatorial.impl.hpp"

namespace flatsurf {

template <typename T>
Tracking<T>::Tracking(const FlatTriangulationCombinatorial *parent, T *value, const FlipHandler &updateAfterFlip, const CollapseHandler &updateBeforeCollapse, const SwapHandler& updateBeforeSwap, const EraseHandler& updateBeforeErase, const DestructionHandler& updateBeforeDestruction) :
  impl(spimpl::make_unique_impl<Implementation>(parent, value, updateAfterFlip, updateBeforeCollapse, updateBeforeSwap, updateBeforeErase, updateBeforeDestruction)) {
}

template <typename T>
Tracking<T>::~Tracking() {}

template <typename T>
const FlatTriangulationCombinatorial& Tracking<T>::parent() const {
  assert(impl->parent != nullptr && "parent has been destructed already");
  return *impl->parent;
}

template <typename T>
void Tracking<T>::noFlip(T&, const FlatTriangulationCombinatorial&, HalfEdge) {
  throw std::logic_error("this tracker of a FlatTriangulationCombinatorial does not support flipping of edges");
}

template <typename T>
void Tracking<T>::noCollapse(T&, const FlatTriangulationCombinatorial&, Edge) {
  throw std::logic_error("this tracker of a FlatTriangulationCombinatorial does not support collapsing of edges");
}

template <typename T>
void Tracking<T>::noSwap(T&, const FlatTriangulationCombinatorial&, HalfEdge, HalfEdge) {
  throw std::logic_error("this tracker of a FlatTriangulationCombinatorial does not support swapping of half edges");
}

template <typename T>
void Tracking<T>::noErase(T&, const FlatTriangulationCombinatorial&, const std::set<Edge>&) {
  throw std::logic_error("this tracker of a FlatTriangulationCombinatorial does not support removal of edges");
}

template <typename T>
void Tracking<T>::forgetParent(T&, const FlatTriangulationCombinatorial&) {
  // the wrapper disconnects from the parent, so this is intentionally empty
}

template <typename T>
T& Tracking<T>::get() const {
  return *impl->value;
}

template <typename T>
Implementation<Tracking<T>>::Implementation(const FlatTriangulationCombinatorial *parent, T* value, const FlipHandler &updateAfterFlip, const CollapseHandler &updateBeforeCollapse, const SwapHandler& updateBeforeSwap, const EraseHandler& updateBeforeErase, const DestructionHandler& updateBeforeDestruction) :
  parent(parent),
  value(value),
  updateAfterFlip(updateAfterFlip),
  updateBeforeCollapse(updateBeforeCollapse),
  updateBeforeSwap(updateBeforeSwap),
  updateBeforeErase(updateBeforeErase),
  updateBeforeDestruction(updateBeforeDestruction) {
  connect();
}

template <typename T>
Implementation<Tracking<T>>::~Implementation() {
  disconnect();
}

template <typename T>
void Implementation<Tracking<T>>::disconnect() {
  if (parent != nullptr) {
    onAfterFlip.disconnect();
    onBeforeCollapse.disconnect();
    onBeforeSwap.disconnect();
    onBeforeErase.disconnect();
    onAfterMove.disconnect();
    parent = nullptr;
  }
}

template <typename T>
void Implementation<Tracking<T>>::connect() {
  assert(parent != nullptr && "cannot connect without a parent FlatTriangulationCombinatorial");

  // All these callbacks hold a reference to "this". This reference cannot be
  // dangling since we explicitly disconnect in ~Implementation.
  onAfterFlip = parent->impl->afterFlip.connect([&](HalfEdge e){
    updateAfterFlip(*value, *parent, e);
  });
  onBeforeCollapse = parent->impl->beforeCollapse.connect([&](Edge e){
    updateBeforeCollapse(*value, *parent, e);
  });
  onBeforeSwap = parent->impl->beforeSwap.connect([&](HalfEdge a, HalfEdge b) {
    updateBeforeSwap(*value, *parent, a, b);
  });
  onBeforeErase = parent->impl->beforeErase.connect([&](const std::set<Edge>& erase){
    updateBeforeErase(*value, *parent, erase);
  });
  onAfterMove = parent->impl->afterMove.connect([&](FlatTriangulationCombinatorial* parent) {
    if (parent == nullptr) {
      updateBeforeDestruction(*value, *this->parent);
    }
    disconnect();
    if (parent != nullptr) {
      this->parent = parent;
      connect();
    }
  });
}

template <typename T>
std::ostream& operator<<(std::ostream&, const Tracking<T>&) {
  throw std::logic_error("not implemented: ostream << Tracking");
}

}

// Instantiations of templates so implementations are generated for the linker
#include "impl/collapsed_half_edge.hpp"

#include "util/instantiate.ipp"

#define LIBFLATSURF_INSTANTIATE_THIS(T)                                                                                                                     \
LIBFLATSURF_INSTANTIATE_WITH_IMPLEMENTATION((Tracking<TrackingMap<HalfEdge, Vector<T>>>))                                                                    \
LIBFLATSURF_INSTANTIATE_WITH_IMPLEMENTATION((Tracking<TrackingMap<Edge, Vector<T>>>))                                                                        \
LIBFLATSURF_INSTANTIATE_WITH_IMPLEMENTATION((Tracking<TrackingMap<HalfEdge, T>>))                                                                            \
LIBFLATSURF_INSTANTIATE_WITH_IMPLEMENTATION((Tracking<TrackingMap<Edge, T>>))                                                                                \
LIBFLATSURF_INSTANTIATE_WITH_IMPLEMENTATION((Tracking<TrackingMap<Edge, SaddleConnection<FlatTriangulation<T>>>>))                                            \
LIBFLATSURF_INSTANTIATE_WITH_IMPLEMENTATION((Tracking<TrackingMap<HalfEdge, CollapsedHalfEdge<T>>>))                                                         \
LIBFLATSURF_INSTANTIATE_WITH_IMPLEMENTATION((Tracking<TrackingMap<HalfEdge, LengthAlongTriangulation<FlatTriangulation<T>>>>))                               \
LIBFLATSURF_INSTANTIATE_WITH_IMPLEMENTATION((Tracking<TrackingMap<HalfEdge, LengthAlongTriangulation<FlatTriangulationCollapsed<T>>>>))                      \
LIBFLATSURF_INSTANTIATE_WITH_IMPLEMENTATION((Tracking<TrackingMap<HalfEdge, typename Implementation<FlatTriangulationCollapsed<T>>::AsymmetricConnection>>))

LIBFLATSURF_INSTANTIATE_MANY((LIBFLATSURF_INSTANTIATE_THIS), LIBFLATSURF_REAL_TYPES)

LIBFLATSURF_INSTANTIATE((LIBFLATSURF_INSTANTIATE_WITH_IMPLEMENTATION), (Tracking<TrackingMap<Edge, int>>))
LIBFLATSURF_INSTANTIATE((LIBFLATSURF_INSTANTIATE_WITH_IMPLEMENTATION), (Tracking<TrackingMap<HalfEdge, int>>))
LIBFLATSURF_INSTANTIATE((LIBFLATSURF_INSTANTIATE_WITH_IMPLEMENTATION), (Tracking<TrackingMap<HalfEdge, Vector<exactreal::Arb>>>))

LIBFLATSURF_INSTANTIATE((LIBFLATSURF_INSTANTIATE_WITH_IMPLEMENTATION), (Tracking<TrackingSet<HalfEdge>>))
LIBFLATSURF_INSTANTIATE((LIBFLATSURF_INSTANTIATE_WITH_IMPLEMENTATION), (Tracking<TrackingSet<Edge>>))
LIBFLATSURF_INSTANTIATE((LIBFLATSURF_INSTANTIATE_WITH_IMPLEMENTATION), (Tracking<TrackingSet<Vertex>>))