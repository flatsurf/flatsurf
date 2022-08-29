/**********************************************************************
 *  This file is part of flatsurf.
 *
 *        Copyright (C) 2021-2022 Julian Rüth
 *        Copyright (C)      2022 Sam Freedman
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
#include "impl/point.impl.hpp"
#include "../flatsurf/edge.hpp"
#include "../flatsurf/path_iterator.hpp"
#include "../flatsurf/saddle_connection.hpp"
#include "../flatsurf/vector.hpp"
#include "../flatsurf/vertex.hpp"
#include "../flatsurf/ccw.hpp"

namespace flatsurf {

template <typename Surface>
FlipDeformationRelation<Surface>::FlipDeformationRelation(const Surface& domain, const Surface& codomain, HalfEdge flip) :
  DeformationRelation<Surface>(domain, codomain), flip(flip) {
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

    path_.push_back(SaddleConnection<Surface>(
      this->codomain,
      this->codomain->sector(source, CCW::COUNTERCLOCKWISE, segment),
      this->codomain->sector(target, CCW::COUNTERCLOCKWISE, -segment),
      segment));
  }
  return path_;
}

template <typename Surface>
Point<Surface> FlipDeformationRelation<Surface>::operator()(const Point<Surface>& point) const {
  if (!point.in(flip) && !point.in(-flip)) {
    // The coordinates remain valid after the flip since the face still exists afterwards.
    const auto face = point.face();
    return Point(*this->codomain, face, point.coordinates(face));
  } else {
    const HalfEdge AB = point.in(flip) ? flip : -flip;

    // Rewrite the coordinates with respect to a face in the codomain, i.e.,
    // the surface after the flip.

    // Consider the following picture:
    //
    // A - C
    // | \ |
    // D - B
    //
    // We now rewrite the barycentric coordinates from the system ABC to the system ADC.
    // Our point is P = (μa*A + μb*B + μc*C) / μ with μ = μa + μb + μc.
    const auto [μa, μb, μc] = point.coordinates(AB);

    // Write B in the system ADC as B = (νd*D + νc*C + νa*A) / ν with ν = νd + νc + νa.
    const auto BC = this->domain->nextInFace(AB);
    const auto CD = this->codomain->nextInFace(BC);
    const auto [νd, νc, νa] = ImplementationOf<Point<Surface>>{*this->codomain, CD, T(), T(), T(1)}.crossed();
    const T ν = νd + νc + νa;

    // Write the point in the system ADC.
    const auto Q = ImplementationOf<Point<Surface>>{*this->codomain, -CD, μb * νd, ν * μc + μb * νc, ν * μa + μb * νa};

    // Express the point in the system ADC or DCB so that all coordinates are non-negative.
    return (Q.a < 0 || Q.b < 0 || Q.c < 0) ? Point{*this->codomain, CD, Q.crossed()} : Point{*this->codomain, -CD, Q.rotated(-CD)};
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
