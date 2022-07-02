/**********************************************************************
 *  This file is part of flatsurf.
 *
 *        Copyright (C) 2021-2022 Julian Rüth
 *                           2022 Sam Freedman
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

#include "impl/flip_deformation_relation.hpp"

#include <ostream>

#include "../flatsurf/flat_triangulation.hpp"
#include "../flatsurf/path.hpp"
#include "../flatsurf/point.hpp"
#include "../flatsurf/edge.hpp"
#include "../flatsurf/path_iterator.hpp"
#include "../flatsurf/saddle_connection.hpp"
#include "../flatsurf/vector.hpp"
#include "../flatsurf/vertex.hpp"
#include "../flatsurf/ccw.hpp"

namespace flatsurf {

template <typename Surface>
FlipDeformationRelation<Surface>::FlipDeformationRelation(const Surface& domain, const Surface& codomain, HalfEdge flip) :
  RetriangulationDeformationRelation<Surface>(domain, codomain), flip(flip) {
}

template <typename Surface>
std::optional<Path<Surface>> FlipDeformationRelation<Surface>::operator()(const Path<Surface>& path) const {
  Path<Surface> path_;
  for (const auto& segment : path) {
    HalfEdge source = segment.source();
    if (source == flip || source == -flip)
      source = this->domain->previousAtVertex(source);

    HalfEdge target = segment.target();
    if (target == flip || target == -flip)
      target = this->domain->previousAtVertex(target);

    path_.push_back(SaddleConnection<Surface>::counterclockwise(this->codomain, source, target, segment));
  }
  return path_;
}

template <typename Surface>
Point<Surface> FlipDeformationRelation<Surface>::operator()(const Point<Surface>& point) const {
  HalfEdge face = point.face();

  if (point.in(flip) || point.in(-flip)) {
    // Rewrite the coordinates with respect to a face in the codomain, i.e.,
    // the surface after the flip.

    // Let flip be the half edge AB in the following picture:
    //
    // A - C
    // | \ |
    // D - B
    //
    // We now rewrite the barycentric coordinates from the system ABC to the system ADC.

    auto [μa, μb, μc] = point.coordinates(flip);

    // Write B in the system ADC.
    auto [νa, νd, νc] = Point(*this->codomain, Vertex::source(this->codomain->previousInFace(-flip), *this->codomain)).coordinates(flip);

    // Write the point in the system ADC.
    return Point{*this->codomain, flip, μa + μb * νa, μb * νd, μc + μb * νc};
  } else {
    // The coordinates remain valid after the flip since the face still exists afterwards.
    return Point(*this->codomain, face, point.coordinates(face));
  }
}

template <typename Surface>
std::unique_ptr<DeformationRelation<Surface>> FlipDeformationRelation<Surface>::clone() const {
  return std::make_unique<FlipDeformationRelation>(*this);
}

template <typename Surface>
std::unique_ptr<DeformationRelation<Surface>> FlipDeformationRelation<Surface>::section() const {
  return std::make_unique<FlipDeformationRelation>(this->codomain, this->domain, flip);
}

template <typename Surface>
bool FlipDeformationRelation<Surface>::trivial() const {
  return false;
}

template <typename Surface>
std::ostream& FlipDeformationRelation<Surface>::operator>>(std::ostream& os) const {
  return os << this->domain << " → " << this->codomain << " flipping " << this->flip;
}

}  // namespace flatsurf

// Instantiations of templates so implementations are generated for the linker
#include "util/instantiate.ipp"

LIBFLATSURF_INSTANTIATE_MANY_WRAPPED((LIBFLATSURF_INSTANTIATE_STATIC), FlipDeformationRelation, LIBFLATSURF_FLAT_TRIANGULATION_TYPES)
