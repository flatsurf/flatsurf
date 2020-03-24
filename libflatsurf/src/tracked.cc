/**********************************************************************
 *  This file is part of flatsurf.
 *
 *        Copyright (C) 2020 Julian Rüth
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

#include "../flatsurf/tracked.hpp"

#include "external/rx-ranges/include/rx/ranges.hpp"

#include "impl/tracked.impl.hpp"

#include "util/assert.ipp"

namespace flatsurf {

namespace {
template <typename T>
struct is_half_edge_map : std::false_type {};

template <typename T>
struct is_half_edge_map<HalfEdgeMap<T>> : std::true_type {};
}  // namespace

template <typename T>
Tracked<T>::Tracked(const FlatTriangulationCombinatorial* parent, T value, const FlipHandler& updateAfterFlip, const CollapseHandler& updateBeforeCollapse, const SwapHandler& updateBeforeSwap, const EraseHandler& updateBeforeErase, const DestructionHandler& updateBeforeDestruction) :
  impl(spimpl::make_unique_impl<Implementation>(parent, std::move(value), updateAfterFlip, updateBeforeCollapse, updateBeforeSwap, updateBeforeErase, updateBeforeDestruction)) {
}

template <typename T>
Tracked<T>::~Tracked() {
  // The implementation destructor takes care of this.
}

template <typename T>
const FlatTriangulationCombinatorial& Tracked<T>::parent() const {
  ASSERT(impl->parent != nullptr, "parent has been destructed already");
  return *impl->parent;
}

template <typename T>
void Tracked<T>::defaultFlip(T& self, const FlatTriangulationCombinatorial&, HalfEdge e) {
  if constexpr (std::is_same_v<T, HalfEdge>) {
    if (Edge(self) == Edge(e))
      throw std::logic_error("This Tracked<HalfEdge> cannot be flipped.");
  } else {
    (void)e;
    throw std::logic_error("This Tracked<T> of a FlatTriangulationCombinatorial does not support flipping of edges.");
  }
}

template <typename T>
void Tracked<T>::defaultCollapse(T& self, const FlatTriangulationCombinatorial&, Edge e) {
  if constexpr (std::is_same_v<T, HalfEdge>) {
    if (Edge(self) == e)
      throw std::logic_error("This Tracked<HalfEdge> cannot be collapsed.");
  } else {
    (void)e;
    throw std::logic_error("This Tracked<T> of a FlatTriangulationCombinatorial does not support collapsing of edges.");
  }
}

template <typename T>
void Tracked<T>::defaultSwap(T& self, const FlatTriangulationCombinatorial&, HalfEdge a, HalfEdge b) {
  ASSERT(a != b, "cannot swap HalfEdge with itself");
  if constexpr (std::is_same_v<T, HalfEdge>) {
    if (self == a)
      self = b;
    else if (self == -a)
      self = -b;
    else if (self == b)
      self = a;
    else if (self == -b)
      self = -a;
  } else if constexpr (std::is_same_v<T, EdgeSet>) {
    if (a == b || a == -b) return;
    if (self.contains(a) && !self.contains(b)) {
      self.erase(a);
      self.insert(b);
    } else if (!self.contains(a) && self.contains(b)) {
      self.erase(b);
      self.insert(a);
    }
  } else if constexpr (is_half_edge_map<T>::value) {
    if (a == b) return;
    using std::swap;
    swap(self[a], self[b]);
    swap(self[-a], self[-b]);
  } else {
    throw std::logic_error("This Tracked<T> of a FlatTriangulationCombinatorial does not support swapping of half edges.");
  }
}

template <typename T>
void Tracked<T>::defaultErase(T& self, const FlatTriangulationCombinatorial&, const std::vector<Edge>& erase) {
  if constexpr (std::is_same_v<T, HalfEdge>) {
    if (std::find(begin(erase), end(erase), Edge(self)) != end(erase))
      throw std::logic_error("This Tracked<HalfEdge> cannot be erased.");
  } else if constexpr (std::is_same_v<T, EdgeSet>) {
    for (auto e : erase)
      self.erase(e);
  } else if constexpr (is_half_edge_map<T>::value) {
    ASSERT(erase | rx::all_of([&](const auto& e) { return e.positive().index() >= self.size() - 2 * erase.size(); }), "Can only erase HalfEdges of maximal index from Tracked<HalfEdgeSet>. But the given edges are not maximal.");
    for (auto e: erase) {
      (void)e;
      self.pop();
    }
  } else {
    throw std::logic_error("This Tracked<T> of a FlatTriangulationCombinatorial does not support removal of edges.");
  }
}

template <typename T>
void Tracked<T>::forgetParent(T&, const FlatTriangulationCombinatorial&) {
  // the wrapper disconnects from the parent, so this is intentionally empty
}

template <typename T>
Tracked<T>::operator T&() {
  return impl->value;
}

template <typename T>
Tracked<T>::operator const T&() const {
  return impl->value;
}

template <typename T>
const T* Tracked<T>::operator->() const {
  return &impl->value;
}

template <typename T>
T* Tracked<T>::operator->() {
  return &impl->value;
}

template <typename T>
Tracked<T>& Tracked<T>::operator=(T&& value) {
  impl->value = std::move(value);
  return *this;
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const Tracked<T>& self) {
  return os << static_cast<const T&>(self);
}

template <typename T>
ImplementationOf<Tracked<T>>::ImplementationOf(const FlatTriangulationCombinatorial* parent, T&& value, const FlipHandler& updateAfterFlip, const CollapseHandler& updateBeforeCollapse, const SwapHandler& updateBeforeSwap, const EraseHandler& updateBeforeErase, const DestructionHandler& updateBeforeDestruction) :
  parent(parent),
  value(std::move(value)),
  updateAfterFlip(updateAfterFlip),
  updateBeforeCollapse(updateBeforeCollapse),
  updateBeforeSwap(updateBeforeSwap),
  updateBeforeErase(updateBeforeErase),
  updateBeforeDestruction(updateBeforeDestruction) {
  connect();
}

template <typename T>
ImplementationOf<Tracked<T>>::~ImplementationOf() {
  disconnect();
}

template <typename T>
void ImplementationOf<Tracked<T>>::disconnect() {
  if (parent != nullptr) {
    onChange.disconnect();
    parent = nullptr;
  }
}

template <typename T>
void ImplementationOf<Tracked<T>>::connect() {
  ASSERT(parent != nullptr, "cannot connect without a parent FlatTriangulationCombinatorial");

  // This callback holds a reference to "this". This reference cannot be
  // dangling since we explicitly disconnect in ~Implementation.
  onChange = ImplementationOf<FlatTriangulationCombinatorial>::connect(*parent, [&](const Message& message) {
    if (auto flipMessage = std::get_if<ImplementationOf<FlatTriangulationCombinatorial>::MessageAfterFlip>(&message)) {
      updateAfterFlip(value, *parent, flipMessage->e);
    } else if (auto collapseMessage = std::get_if<ImplementationOf<FlatTriangulationCombinatorial>::MessageBeforeCollapse>(&message)) {
      updateBeforeCollapse(value, *parent, collapseMessage->e);
    } else if (auto swapMessage = std::get_if<ImplementationOf<FlatTriangulationCombinatorial>::MessageBeforeSwap>(&message)) {
      updateBeforeSwap(value, *parent, swapMessage->a, swapMessage->b);
    } else if (auto eraseMessage = std::get_if<ImplementationOf<FlatTriangulationCombinatorial>::MessageBeforeErase>(&message)) {
      updateBeforeErase(value, *parent, eraseMessage->erase);
    } else if (auto moveMessage = std::get_if<ImplementationOf<FlatTriangulationCombinatorial>::MessageAfterMove>(&message)) {
      if (moveMessage->target == nullptr) {
        updateBeforeDestruction(value, *this->parent);
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

}  // namespace flatsurf

// Instantiations of templates so implementations are generated for the linker
#include "util/instantiate.ipp"

#include "../flatsurf/edge_set.hpp"
#include "../flatsurf/half_edge_set.hpp"
#include "../flatsurf/odd_half_edge_map.hpp"
#include "../flatsurf/vector.hpp"

#include "impl/collapsed_half_edge.hpp"
#include "impl/flat_triangulation_collapsed.impl.hpp"

LIBFLATSURF_INSTANTIATE((LIBFLATSURF_INSTANTIATE_WITH_IMPLEMENTATION), (Tracked<HalfEdge>))
LIBFLATSURF_INSTANTIATE((LIBFLATSURF_INSTANTIATE_WITH_IMPLEMENTATION), (Tracked<HalfEdgeSet>))
LIBFLATSURF_INSTANTIATE((LIBFLATSURF_INSTANTIATE_WITH_IMPLEMENTATION), (Tracked<EdgeSet>))

#define LIBFLATSURF_WRAP_ODD_HALF_EDGE_MAP_VECTOR(R, TYPE, T) (TYPE<OddHalfEdgeMap<Vector<T>>>)
LIBFLATSURF_INSTANTIATE_MANY_FROM_TRANSFORMATION((LIBFLATSURF_INSTANTIATE_WITH_IMPLEMENTATION), Tracked, LIBFLATSURF_REAL_TYPES(exactreal::Arb), LIBFLATSURF_WRAP_ODD_HALF_EDGE_MAP_VECTOR)

#define LIBFLATSURF_WRAP_HALF_EDGE_MAP_SADDLE_CONNECTION(R, TYPE, T) (TYPE<HalfEdgeMap<SaddleConnection<T>>>)
LIBFLATSURF_INSTANTIATE_MANY_FROM_TRANSFORMATION((LIBFLATSURF_INSTANTIATE_WITH_IMPLEMENTATION), Tracked, LIBFLATSURF_FLAT_TRIANGULATION_TYPES, LIBFLATSURF_WRAP_HALF_EDGE_MAP_SADDLE_CONNECTION)

#define LIBFLATSURF_WRAP_HALF_EDGE_MAP_COLLAPSED(R, TYPE, T) (TYPE<HalfEdgeMap<CollapsedHalfEdge<T>>>)
LIBFLATSURF_INSTANTIATE_MANY_FROM_TRANSFORMATION((LIBFLATSURF_INSTANTIATE_WITH_IMPLEMENTATION), Tracked, LIBFLATSURF_REAL_TYPES, LIBFLATSURF_WRAP_HALF_EDGE_MAP_COLLAPSED)
