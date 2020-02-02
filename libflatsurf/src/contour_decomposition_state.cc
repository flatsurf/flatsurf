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
#include <memory>

#include "../flatsurf/vertical.hpp"
#include "../flatsurf/half_edge.hpp"

#include "util/assert.ipp"

#include "impl/contour_decomposition_state.hpp"

namespace flatsurf {

template <typename Surface>
ContourDecompositionState<Surface>::ContourDecompositionState(std::unique_ptr<Surface> surface, const Vector<typename Surface::Coordinate>& vert) :
  surface([&]() {
    if constexpr (std::is_same_v<Surface, FlatTriangulationCollapsed<typename Surface::Coordinate>>) {
      CHECK_ARGUMENT(surface->vertical().vertitcal() == vert, "can only decompose with respect to the existing vertical " << surface->vertical().vertical() << " of this surface");
      return surface;
    } else {
      auto collapsed = Surface::Collapsed::make(std::move(surface), vert);
      IntervalExchangeTransformation<typename Surface::Collapsed>::makeUniqueLargeEdges(*collapsed, vert);
      return collapsed;
    }
  }()),
  components([&]() {
    std::list<ComponentState> components;
    for (auto& component : this->surface->vertical().components()) {
      components.push_back(ComponentState{component});
    }
    return components;
  }()) {
}

template <typename Surface>
ContourComponent<Surface> ContourDecompositionState<Surface>::make(ComponentState* component) {
  return ContourComponent<Surface>::Implementation::make(this->shared_from_this(), component);
}

template <typename Surface>
std::ostream& operator<<(std::ostream&, const ContourDecompositionState<Surface>&) {
  throw std::logic_error("not implemented: ostream << ContourDecompositionState");
}

}

// Instantiations of templates so implementations are generated for the linker
#include "util/instantiate.ipp"

LIBFLATSURF_INSTANTIATE_MANY_WRAPPED((LIBFLATSURF_INSTANTIATE_WITHOUT_IMPLEMENTATION), ContourDecompositionState, LIBFLATSURF_FLAT_TRIANGULATION_TYPES)
