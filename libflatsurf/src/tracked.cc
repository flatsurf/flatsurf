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

#include "../flatsurf/tracked.hpp"

#include <fmt/format.h>

#include <boost/type_index.hpp>
#include <ostream>

#include "../flatsurf/odd_half_edge_map.hpp"
#include "external/rx-ranges/include/rx/ranges.hpp"
#include "impl/deformation.impl.hpp"
#include "impl/flip_deformation_relation.hpp"
#include "impl/read_only.hpp"
#include "impl/tracked.impl.hpp"
#include "impl/weak_read_only.hpp"
#include "util/assert.ipp"

namespace flatsurf {

namespace {
template <typename T>
struct is_edge_map : std::false_type {};

template <typename T>
struct is_edge_map<EdgeMap<T>> : std::true_type {};

template <typename T>
struct is_half_edge_map : std::false_type {};

template <typename T>
struct is_half_edge_map<HalfEdgeMap<T>> : std::true_type {};

template <typename T>
struct is_odd_half_edge_map : std::false_type {};

template <typename T>
struct is_odd_half_edge_map<OddHalfEdgeMap<T>> : std::true_type {};

template <typename T>
struct is_deformation : std::false_type {};

template <typename T>
struct is_deformation<Deformation<T>> : std::true_type {};
}  // namespace

template <typename T>
Tracked<T>::Tracked(const FlatTriangulationCombinatorial& parent, T value, const FlipHandler& updateAfterFlip, const CollapseHandler& updateBeforeCollapse, const SwapHandler& updateBeforeSwap, const EraseHandler& updateBeforeErase, const DestructionHandler& updateBeforeDestruction) :
  self(spimpl::make_unique_impl<ImplementationOf<Tracked>>(ImplementationOf<FlatTriangulationCombinatorial>::self(parent).state.get(), std::move(value), updateAfterFlip, updateBeforeCollapse, updateBeforeSwap, updateBeforeErase, updateBeforeDestruction)) {
}

template <typename T>
Tracked<T>::Tracked(const Tracked& rhs) noexcept :
  self(spimpl::make_unique_impl<ImplementationOf<Tracked>>(rhs.self->parent.get(), T(rhs.self->value), rhs.self->updateAfterFlip, rhs.self->updateBeforeCollapse, rhs.self->updateBeforeSwap, rhs.self->updateBeforeErase, rhs.self->updateBeforeDestruction)) {}

template <typename T>
Tracked<T>::Tracked(Tracked&& rhs) noexcept :
  self(std::move(rhs.self)) {}

template <typename T>
Tracked<T>::~Tracked() {
  // The implementation destructor takes care of this.
}

template <typename T>
void Tracked<T>::defaultFlip(T& self, const FlatTriangulationCombinatorial& surface, HalfEdge e) {
  if constexpr (std::is_same_v<T, HalfEdge>) {
    if (Edge(self) == Edge(e))
      throw std::logic_error("This Tracked<HalfEdge> cannot be flipped.");
  } else if constexpr (is_deformation<T>::value) {
    auto flipped = self.codomain().clone();
    flipped.flip(e);
    LIBFLATSURF_ASSERT(flipped.combinatorial() == surface, "Codomain of deformation must be equal to the tracked surface.");
    self = ImplementationOf<T>::make(std::make_unique<FlipDeformationRelation<decltype(flipped)>>(self.codomain(), flipped, e)) * self;
  } else {
    (void)e;
    throw std::logic_error(fmt::format("This Tracked<{}> of a FlatTriangulationCombinatorial does not support flipping of edges.", boost::typeindex::type_id<T>().pretty_name()));
  }
}

template <typename T>
void Tracked<T>::defaultCollapse(T& self, const FlatTriangulationCombinatorial&, Edge e) {
  if constexpr (std::is_same_v<T, HalfEdge>) {
    if (Edge(self) == e)
      throw std::logic_error("This Tracked<HalfEdge> cannot be collapsed.");
  } else {
    (void)e;
    throw std::logic_error(fmt::format("This Tracked<{}> of a FlatTriangulationCombinatorial does not support collapsing of edges.", boost::typeindex::type_id<T>().pretty_name()));
  }
}

template <typename T>
void Tracked<T>::defaultSwap(T& self, const FlatTriangulationCombinatorial&, HalfEdge a, HalfEdge b) {
  LIBFLATSURF_ASSERT(a != b, "cannot swap HalfEdge with itself");
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
  } else if constexpr (is_edge_map<T>::value) {
    if (a == b) return;
    using std::swap;
    swap(self[a], self[b]);
  } else if constexpr (is_half_edge_map<T>::value) {
    if (a == b) return;
    using std::swap;
    swap(self[a], self[b]);
    swap(self[-a], self[-b]);
  } else if constexpr (is_odd_half_edge_map<T>::value) {
    if (a == b) return;
    if (a == -b)
      self.set(a, self.get(b));
    else {
      auto tmp = self.get(a);
      self.set(a, self.get(b));
      self.set(b, tmp);
    }
  } else {
    throw std::logic_error(fmt::format("This Tracked<{}> of a FlatTriangulationCombinatorial does not support swapping of half edges.", boost::typeindex::type_id<T>().pretty_name()));
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
  } else if constexpr (is_edge_map<T>::value) {
    LIBFLATSURF_ASSERT(erase | rx::all_of([&](const auto& e) { return e.index() >= self.size() - erase.size(); }), "Can only erase Edges of maximal index from Tracked<EdgeMap>. But the given edges are not maximal.");
    for (auto e : erase) {
      (void)e;
      self.pop();
    }
  } else if constexpr (is_half_edge_map<T>::value || is_odd_half_edge_map<T>::value) {
    LIBFLATSURF_ASSERT(erase | rx::all_of([&](const auto& e) { return e.positive().index() >= self.size() - 2 * erase.size(); }), "Can only erase HalfEdges of maximal index from Tracked<(Odd)HalfEdgeMap>. But the given edges are not maximal.");
    for (auto e : erase) {
      (void)e;
      self.pop();
    }
  } else {
    throw std::logic_error(fmt::format("This Tracked<{}> of a FlatTriangulationCombinatorial does not support removal of edges.", boost::typeindex::type_id<T>().pretty_name()));
  }
}

template <typename T>
void Tracked<T>::forgetParent(T&, const FlatTriangulationCombinatorial&) {
  // the wrapper disconnects from the parent, so this is intentionally empty
}

template <typename T>
Tracked<T>::operator T&() {
  return self->value;
}

template <typename T>
Tracked<T>::operator const T&() const {
  return self->value;
}

template <typename T>
Tracked<T>::operator T&&() && {
  return std::move(self->value);
}

template <typename T>
const T* Tracked<T>::operator->() const {
  return &self->value;
}

template <typename T>
T* Tracked<T>::operator->() {
  return &self->value;
}

template <typename T>
const T& Tracked<T>::operator*() const {
  return self->value;
}

template <typename T>
T& Tracked<T>::operator*() {
  return self->value;
}

template <typename T>
Tracked<T>& Tracked<T>::operator=(const Tracked& rhs) noexcept {
  return *this = Tracked(rhs);
}

template <typename T>
Tracked<T>& Tracked<T>::operator=(Tracked&& rhs) noexcept {
  self = std::move(rhs.self);
  return *this;
}

template <typename T>
Tracked<T>& Tracked<T>::operator=(T&& value) {
  self->value = std::move(value);
  return *this;
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const Tracked<T>& self) {
  if constexpr (is_optional<T>::value) {
    if (static_cast<const T&>(self)) {
      return os << *static_cast<const T&>(self);
    } else {
      return os << "{}";
    }
  } else {
    return os << static_cast<const T&>(self);
  }
}

template <typename T>
ImplementationOf<Tracked<T>>::ImplementationOf(ImplementationOf<FlatTriangulationCombinatorial>* parent, T&& value, const FlipHandler& updateAfterFlip, const CollapseHandler& updateBeforeCollapse, const SwapHandler& updateBeforeSwap, const EraseHandler& updateBeforeErase, const DestructionHandler& updateBeforeDestruction) :
  parent(parent),
  value(std::move(value)),
  updateAfterFlip(updateAfterFlip),
  updateBeforeCollapse(updateBeforeCollapse),
  updateBeforeSwap(updateBeforeSwap),
  updateBeforeErase(updateBeforeErase),
  updateBeforeDestruction(updateBeforeDestruction) {
  if (parent != nullptr)
    connect();
}

template <typename T>
ImplementationOf<Tracked<T>>::~ImplementationOf() {
  if (!parent.expired())
    onChange.disconnect();
}

template <typename T>
void ImplementationOf<Tracked<T>>::connect() {
  LIBFLATSURF_ASSERT(!parent.expired(), "cannot connect without a parent FlatTriangulationCombinatorial");

  // This callback uses a reference to "this->parent". This reference will not
  // be dangling since we explicitly disconnect in ~Implementation.
  onChange = ImplementationOf<FlatTriangulationCombinatorial>::connect(parent.get(), [this](const Message& message) {
    if (auto flipMessage = std::get_if<ImplementationOf<FlatTriangulationCombinatorial>::MessageAfterFlip>(&message)) {
      const auto surface = static_cast<ReadOnly<FlatTriangulationCombinatorial>>(parent);
      updateAfterFlip(value, surface, flipMessage->e);
    } else if (auto collapseMessage = std::get_if<ImplementationOf<FlatTriangulationCombinatorial>::MessageBeforeCollapse>(&message)) {
      const auto surface = static_cast<ReadOnly<FlatTriangulationCombinatorial>>(parent);
      updateBeforeCollapse(value, surface, collapseMessage->e);
    } else if (auto swapMessage = std::get_if<ImplementationOf<FlatTriangulationCombinatorial>::MessageBeforeSwap>(&message)) {
      const auto surface = static_cast<ReadOnly<FlatTriangulationCombinatorial>>(parent);
      updateBeforeSwap(value, surface, swapMessage->a, swapMessage->b);
    } else if (auto eraseMessage = std::get_if<ImplementationOf<FlatTriangulationCombinatorial>::MessageBeforeErase>(&message)) {
      const auto surface = static_cast<ReadOnly<FlatTriangulationCombinatorial>>(parent);
      updateBeforeErase(value, surface, eraseMessage->erase);
    } else if (auto moveMessage = std::get_if<ImplementationOf<FlatTriangulationCombinatorial>::MessageAfterMove>(&message)) {
      if (moveMessage->target == nullptr) {
        const auto surface = static_cast<ReadOnly<FlatTriangulationCombinatorial>>(parent);
        updateBeforeDestruction(value, surface);
      }
      if (!parent.expired())
        onChange.disconnect();
      this->parent = moveMessage->target;
      if (!parent.expired())
        connect();
    } else {
      throw std::logic_error("unknown Message");
    }
  });
}

}  // namespace flatsurf

// Instantiations of templates so implementations are generated for the linker
#include "../flatsurf/ccw.hpp"
#include "../flatsurf/deformation.hpp"
#include "../flatsurf/edge_map.hpp"
#include "../flatsurf/edge_set.hpp"
#include "../flatsurf/half_edge_map.hpp"
#include "../flatsurf/half_edge_set.hpp"
#include "../flatsurf/odd_half_edge_map.hpp"
#include "../flatsurf/orientation.hpp"
#include "../flatsurf/vector.hpp"
#include "impl/collapsed_half_edge.hpp"
#include "impl/flat_triangulation_collapsed.impl.hpp"
#include "util/instantiate.ipp"

LIBFLATSURF_INSTANTIATE((LIBFLATSURF_INSTANTIATE_WITH_IMPLEMENTATION), (Tracked<HalfEdge>))
LIBFLATSURF_INSTANTIATE((LIBFLATSURF_INSTANTIATE_WITH_IMPLEMENTATION), (Tracked<HalfEdgeSet>))
LIBFLATSURF_INSTANTIATE((LIBFLATSURF_INSTANTIATE_WITH_IMPLEMENTATION), (Tracked<EdgeSet>))
LIBFLATSURF_INSTANTIATE((LIBFLATSURF_INSTANTIATE_WITH_IMPLEMENTATION), (Tracked<OddHalfEdgeMap<HalfEdge>>))
LIBFLATSURF_INSTANTIATE((LIBFLATSURF_INSTANTIATE_WITH_IMPLEMENTATION), (Tracked<HalfEdgeMap<HalfEdge>>))

#define LIBFLATSURF_WRAP_ODD_HALF_EDGE_MAP_VECTOR(R, TYPE, T) (TYPE<OddHalfEdgeMap<Vector<T>>>)
LIBFLATSURF_INSTANTIATE_MANY_FROM_TRANSFORMATION((LIBFLATSURF_INSTANTIATE_WITH_IMPLEMENTATION), Tracked, LIBFLATSURF_REAL_TYPES(exactreal::Arb), LIBFLATSURF_WRAP_ODD_HALF_EDGE_MAP_VECTOR)

#define LIBFLATSURF_WRAP_EDGE_MAP_OPTIONAL(R, TYPE, T) (TYPE<EdgeMap<std::optional<T>>>)
LIBFLATSURF_INSTANTIATE_MANY_FROM_TRANSFORMATION((LIBFLATSURF_INSTANTIATE_WITH_IMPLEMENTATION), Tracked, LIBFLATSURF_REAL_TYPES(bool), LIBFLATSURF_WRAP_EDGE_MAP_OPTIONAL)

#define LIBFLATSURF_WRAP_HALF_EDGE_MAP_OPTIONAL(R, TYPE, T) (TYPE<HalfEdgeMap<std::optional<T>>>)
LIBFLATSURF_INSTANTIATE_MANY_FROM_TRANSFORMATION((LIBFLATSURF_INSTANTIATE_WITH_IMPLEMENTATION), Tracked, LIBFLATSURF_REAL_TYPES, LIBFLATSURF_WRAP_HALF_EDGE_MAP_OPTIONAL)

#define LIBFLATSURF_WRAP_ODD_HALF_EDGE_MAP_OPTIONAL(R, TYPE, T) (TYPE<OddHalfEdgeMap<std::optional<T>>>)
LIBFLATSURF_INSTANTIATE_MANY_FROM_TRANSFORMATION((LIBFLATSURF_INSTANTIATE_WITH_IMPLEMENTATION), Tracked, LIBFLATSURF_REAL_TYPES(flatsurf::CCW)(flatsurf::ORIENTATION), LIBFLATSURF_WRAP_ODD_HALF_EDGE_MAP_OPTIONAL)

#define LIBFLATSURF_WRAP_HALF_EDGE_MAP_SADDLE_CONNECTION(R, TYPE, T) (TYPE<HalfEdgeMap<SaddleConnection<T>>>)
LIBFLATSURF_INSTANTIATE_MANY_FROM_TRANSFORMATION((LIBFLATSURF_INSTANTIATE_WITH_IMPLEMENTATION), Tracked, LIBFLATSURF_FLAT_TRIANGULATION_TYPES, LIBFLATSURF_WRAP_HALF_EDGE_MAP_SADDLE_CONNECTION)

#define LIBFLATSURF_WRAP_HALF_EDGE_MAP_COLLAPSED(R, TYPE, T) (TYPE<HalfEdgeMap<CollapsedHalfEdge<T>>>)
LIBFLATSURF_INSTANTIATE_MANY_FROM_TRANSFORMATION((LIBFLATSURF_INSTANTIATE_WITH_IMPLEMENTATION), Tracked, LIBFLATSURF_REAL_TYPES, LIBFLATSURF_WRAP_HALF_EDGE_MAP_COLLAPSED)

#define LIBFLATSURF_WRAP_DEFORMATION(R, TYPE, T) (TYPE<Deformation<T>>)
LIBFLATSURF_INSTANTIATE_MANY_FROM_TRANSFORMATION((LIBFLATSURF_INSTANTIATE_WITH_IMPLEMENTATION), Tracked, LIBFLATSURF_FLAT_TRIANGULATION_TYPES, LIBFLATSURF_WRAP_DEFORMATION)
