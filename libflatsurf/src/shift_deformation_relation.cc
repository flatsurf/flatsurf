/**********************************************************************
 *  This file is part of flatsurf.
 *
 *        Copyright (C) 2021 Julian Rüth
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

#include "impl/shift_deformation_relation.hpp"

#include <ostream>

#include "../flatsurf/flat_triangulation.hpp"
#include "../flatsurf/odd_half_edge_map.hpp"
#include "../flatsurf/path.hpp"
#include "../flatsurf/point.hpp"
#include "../flatsurf/path_iterator.hpp"
#include "../flatsurf/saddle_connection.hpp"
#include "../flatsurf/vector.hpp"
#include "../flatsurf/half_edge_intersection.hpp"
#include "../flatsurf/point.hpp"

#include "util/assert.ipp"

namespace flatsurf {

template <typename Surface>
ShiftDeformationRelation<Surface>::ShiftDeformationRelation(const Surface& domain, const Surface& codomain, OddHalfEdgeMap<Path<Surface>> shift) :
  DeformationRelation<Surface>(domain, codomain), shifted(shift) {
}

template <typename Surface>
std::optional<Path<Surface>> ShiftDeformationRelation<Surface>::operator()(const Path<Surface>& path) const {
  Path<Surface> image;

  for (const auto& connection : path) {
    // To map a saddle connection, we rewrite it as a homotopic sequence of
    // half edges and then map these half edges.
    const auto& surface = connection.surface();

    // To construct this sequence we pretend that we are currently inside the
    // face which has `face` as one of its half edges and actually that we
    // are at the vertex that has this half edge as an outgoing half edge.
    auto face = connection.source();

    std::vector<HalfEdge> targets;

    for (const auto& intersection : connection.path())
      targets.push_back(intersection.halfEdge());

    for (const auto& target : targets) {
      // Compute the sequence of half edges that we have to traverse to go to
      // the vertex which has "target" as an outgoing vertex.
      LIBFLATSURF_ASSERT(face != target, "A saddle connection cannot cross the same half edge twice in a row but " << connection << " is crossing from " << face << " to " << target);

      if (target == surface.previousInFace(face)) {
        // Cross over the target half edge into the opposite face.
        face = -target;
      } else if (target == surface.nextInFace(face)) {
        // Cross over the target half edge into the opposite face.
        image += shifted.get(face);
        image += shifted.get(target);
        face = -target;
      } else {
        LIBFLATSURF_UNREACHABLE("Target half edge " << target << " must be in the same face as " << face);
      }
    }

    image += shifted.get(-connection.target());
  }

  return image;
}

template <typename Surface>
Point<Surface> ShiftDeformationRelation<Surface>::operator()(const Point<Surface>& point) const {
  throw std::logic_error("not implemented: cannot map points with this shift deformation yet");
}

template <typename Surface>
std::unique_ptr<DeformationRelation<Surface>> ShiftDeformationRelation<Surface>::clone() const {
  return std::make_unique<ShiftDeformationRelation>(*this);
}

template <typename Surface>
std::unique_ptr<DeformationRelation<Surface>> ShiftDeformationRelation<Surface>::section() const {
  throw std::logic_error("not implemented: cannot determine section of a shift deformation");
}

template <typename Surface>
bool ShiftDeformationRelation<Surface>::trivial() const {
  if (*this->domain != *this->codomain)
    return false;

  for (auto e : this->domain->edges())
    if (shifted.get(e.positive()) != SaddleConnection<Surface>(*this->domain, e.positive()))
      return false;

  return true;
}

template <typename Surface>
std::ostream& ShiftDeformationRelation<Surface>::operator>>(std::ostream& os) const {
  return os << this->domain << " → " << this->codomain << " given by " << shifted;
}

}  // namespace flatsurf

// Instantiations of templates so implementations are generated for the linker
#include "util/instantiate.ipp"

LIBFLATSURF_INSTANTIATE_MANY_WRAPPED((LIBFLATSURF_INSTANTIATE_STATIC), ShiftDeformationRelation, LIBFLATSURF_FLAT_TRIANGULATION_TYPES)
