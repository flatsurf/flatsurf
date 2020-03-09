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

#include <unordered_set>
#include <ostream>

#include <fmt/format.h>

#include "external/rx-ranges/include/rx/ranges.hpp"

#include "../flatsurf/half_edge.hpp"
#include "../flatsurf/vertical.hpp"
#include "../flatsurf/saddle_connection.hpp"
#include "../flatsurf/fmt.hpp"

#include "impl/contour_component.impl.hpp"
#include "impl/contour_component_state.hpp"
#include "impl/contour_decomposition_state.hpp"

namespace flatsurf {

template <typename Surface>
ContourComponentState<Surface>::ContourComponentState(const ContourDecompositionState<Surface>& state, const std::unordered_set<HalfEdge>& halfEdges) :
  halfEdges(halfEdges),
  large([&]() {
    auto vertical = state.surface->vertical();
    HalfEdge large = *std::find_if(begin(halfEdges), end(halfEdges), [&](HalfEdge e) {
      return vertical.large(e);
    });

    if (vertical.perpendicular(state.surface->fromEdge(large)) < 0)
      large = -large;

    return large;
  }()),
  topEdges([&]() {
    std::vector<HalfEdge> topEdges;

    ImplementationOf<ContourComponent<FlatTriangulationCollapsed<T>>>::makeContour(back_inserter(topEdges), large, *state.surface, state.surface->vertical());

    return topEdges | rx::reverse() | rx::to_vector();
  }()),
  bottomEdges([&]() {
    std::vector<HalfEdge> bottomEdges;

    ImplementationOf<ContourComponent<FlatTriangulationCollapsed<T>>>::makeContour(back_inserter(bottomEdges), -large, *state.surface, -state.surface->vertical());

    return bottomEdges | rx::transform([&](const auto e) { return -e; }) | rx::reverse() | rx::to_vector();
  }()) {
}

template <typename Surface>
std::ostream& operator<<(std::ostream& os, const ContourComponentState<Surface>& self) {
  return os << fmt::format("[{}]", fmt::join(self.halfEdges, ", "));
}

}

// Instantiations of templates so implementations are generated for the linker
#include "util/instantiate.ipp"

LIBFLATSURF_INSTANTIATE_MANY_WRAPPED((LIBFLATSURF_INSTANTIATE_WITHOUT_IMPLEMENTATION), ContourComponentState, LIBFLATSURF_FLAT_TRIANGULATION_TYPES)
