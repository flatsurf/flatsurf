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

#include "impl/transformation_deformation.hpp"

#include "../flatsurf/delaunay.hpp"
#include "../flatsurf/edge.hpp"
#include "../flatsurf/half_edge_set.hpp"
#include "external/rx-ranges/include/rx/ranges.hpp"
#include "util/assert.ipp"

namespace flatsurf {

template <typename Surface>
TransformationDeformation<Surface>::TransformationDeformation(Surface&& surface, HalfEdgeMap<HalfEdge>&& halfEdgeMap) :
  ImplementationOf<Deformation<Surface>>(std::move(surface)),
  isomorphism(std::move(halfEdgeMap)) {
  LIBFLATSURF_ASSERT_ARGUMENT(this->surface.halfEdges() | rx::all_of([&](const auto he) {
    return this->isomorphism[he] != HalfEdge() || this->surface.delaunay(he.edge()) == DELAUNAY::AMBIGUOUS;
  }),
      "half edge map is not a total map");
  LIBFLATSURF_ASSERT_ARGUMENT([&]() {
    HalfEdgeSet im;
    for (auto he : this->surface.halfEdges()) {
      const auto image = this->isomorphism[he];
      if (image != HalfEdge()) {
        if (im.contains(image))
          return false;
        im.insert(image);
      }
    }
    return true;
  }(),
      "half edge map is not a bijection");
}

template <typename Surface>
std::optional<HalfEdge> TransformationDeformation<Surface>::operator()(HalfEdge he) const {
  return isomorphism[he];
}

template <typename Surface>
Deformation<Surface> TransformationDeformation<Surface>::make(Surface&& surface, HalfEdgeMap<HalfEdge>&& halfEdgeMap) {
  return ImplementationOf<Deformation<Surface>>::make(std::move(spimpl::unique_impl_ptr<ImplementationOf<Deformation<Surface>>>(new TransformationDeformation<Surface>(std::move(surface), std::move(halfEdgeMap)), spimpl::details::default_delete<ImplementationOf<Deformation<Surface>>>)));
}

}  // namespace flatsurf

// Instantiations of templates so implementations are generated for the linker
#include "util/instantiate.ipp"

LIBFLATSURF_INSTANTIATE_MANY_WRAPPED((LIBFLATSURF_INSTANTIATE_STATIC), TransformationDeformation, LIBFLATSURF_FLAT_TRIANGULATION_TYPES)
