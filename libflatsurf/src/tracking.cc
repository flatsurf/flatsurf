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

#include <ostream>
#include <vector>

#include "impl/flat_triangulation_combinatorial.impl.hpp"
#include "impl/tracking.impl.hpp"

namespace flatsurf {

template <typename T>
Tracking<T>::Tracking(const FlatTriangulationCombinatorial* parent, T* value, const FlipHandler& updateAfterFlip, const CollapseHandler& updateBeforeCollapse, const SwapHandler& updateBeforeSwap, const EraseHandler& updateBeforeErase, const DestructionHandler& updateBeforeDestruction) :
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
void Tracking<T>::noErase(T&, const FlatTriangulationCombinatorial&, const std::vector<Edge>&) {
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
Implementation<Tracking<T>>::Implementation(const FlatTriangulationCombinatorial* parent, T* value, const FlipHandler& updateAfterFlip, const CollapseHandler& updateBeforeCollapse, const SwapHandler& updateBeforeSwap, const EraseHandler& updateBeforeErase, const DestructionHandler& updateBeforeDestruction) :
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
    onChange.disconnect();
    parent = nullptr;
  }
}

template <typename T>
void Implementation<Tracking<T>>::connect() {
  assert(parent != nullptr && "cannot connect without a parent FlatTriangulationCombinatorial");

  // This callback holds a reference to "this". This reference cannot be
  // dangling since we explicitly disconnect in ~Implementation.
  onChange = parent->impl->change.connect([&](const Message& message) {
    if (auto flipMessage = std::get_if<::flatsurf::Implementation<FlatTriangulationCombinatorial>::MessageAfterFlip>(&message)) {
      updateAfterFlip(*value, *parent, flipMessage->e);
    } else if (auto collapseMessage = std::get_if<::flatsurf::Implementation<FlatTriangulationCombinatorial>::MessageBeforeCollapse>(&message)) {
      updateBeforeCollapse(*value, *parent, collapseMessage->e);
    } else if (auto swapMessage = std::get_if<::flatsurf::Implementation<FlatTriangulationCombinatorial>::MessageBeforeSwap>(&message)) {
      updateBeforeSwap(*value, *parent, swapMessage->a, swapMessage->b);
    } else if (auto eraseMessage = std::get_if<::flatsurf::Implementation<FlatTriangulationCombinatorial>::MessageBeforeErase>(&message)) {
      updateBeforeErase(*value, *parent, eraseMessage->erase);
    } else if (auto moveMessage = std::get_if<::flatsurf::Implementation<FlatTriangulationCombinatorial>::MessageAfterMove>(&message)) {
      if (moveMessage->target == nullptr) {
        updateBeforeDestruction(*value, *this->parent);
      }
      disconnect();
      if (parent != nullptr) {
        this->parent = moveMessage->target;
        connect();
      }
    } else {
      throw std::logic_error("unknown Message");
    }
  });
}

template <typename T>
std::ostream& operator<<(std::ostream&, const Tracking<T>&) {
  throw std::logic_error("not implemented: ostream << Tracking");
}

}  // namespace flatsurf

// Instantiations of templates so implementations are generated for the linker
#include "impl/collapsed_half_edge.hpp"

#include "util/instantiate.ipp"

#define LIBFLATSURF_INSTANTIATE_THIS(T)                                                                                                   \
  LIBFLATSURF_INSTANTIATE_WITH_IMPLEMENTATION((Tracking<TrackingMap<HalfEdge, Vector<T>>>))                                               \
  LIBFLATSURF_INSTANTIATE_WITH_IMPLEMENTATION((Tracking<TrackingMap<Edge, Vector<T>>>))                                                   \
  LIBFLATSURF_INSTANTIATE_WITH_IMPLEMENTATION((Tracking<TrackingMap<HalfEdge, T>>))                                                       \
  LIBFLATSURF_INSTANTIATE_WITH_IMPLEMENTATION((Tracking<TrackingMap<Edge, T>>))                                                           \
  LIBFLATSURF_INSTANTIATE_WITH_IMPLEMENTATION((Tracking<TrackingMap<Edge, SaddleConnection<FlatTriangulation<T>>>>))                      \
  LIBFLATSURF_INSTANTIATE_WITH_IMPLEMENTATION((Tracking<TrackingMap<Edge, std::optional<SaddleConnection<FlatTriangulation<T>>>>>))                      \
  LIBFLATSURF_INSTANTIATE_WITH_IMPLEMENTATION((Tracking<TrackingMap<HalfEdge, CollapsedHalfEdge<T>>>))                                    \
  LIBFLATSURF_INSTANTIATE_WITH_IMPLEMENTATION((Tracking<TrackingMap<HalfEdge, LengthAlongTriangulation<FlatTriangulation<T>>>>))          \
  LIBFLATSURF_INSTANTIATE_WITH_IMPLEMENTATION((Tracking<TrackingMap<HalfEdge, LengthAlongTriangulation<FlatTriangulationCollapsed<T>>>>)) \
  LIBFLATSURF_INSTANTIATE_WITH_IMPLEMENTATION((Tracking<TrackingMap<HalfEdge, typename Implementation<FlatTriangulationCollapsed<T>>::AsymmetricConnection>>))

LIBFLATSURF_INSTANTIATE_MANY((LIBFLATSURF_INSTANTIATE_THIS), LIBFLATSURF_REAL_TYPES)

LIBFLATSURF_INSTANTIATE((LIBFLATSURF_INSTANTIATE_WITH_IMPLEMENTATION), (Tracking<TrackingMap<Edge, int>>))
LIBFLATSURF_INSTANTIATE((LIBFLATSURF_INSTANTIATE_WITH_IMPLEMENTATION), (Tracking<TrackingMap<HalfEdge, int>>))
LIBFLATSURF_INSTANTIATE((LIBFLATSURF_INSTANTIATE_WITH_IMPLEMENTATION), (Tracking<TrackingMap<HalfEdge, Vector<exactreal::Arb>>>))

LIBFLATSURF_INSTANTIATE((LIBFLATSURF_INSTANTIATE_WITH_IMPLEMENTATION), (Tracking<TrackingSet<HalfEdge>>))
LIBFLATSURF_INSTANTIATE((LIBFLATSURF_INSTANTIATE_WITH_IMPLEMENTATION), (Tracking<TrackingSet<Edge>>))
LIBFLATSURF_INSTANTIATE((LIBFLATSURF_INSTANTIATE_WITH_IMPLEMENTATION), (Tracking<TrackingSet<Vertex>>))
