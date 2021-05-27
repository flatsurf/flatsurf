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

#include "../flatsurf/flat_triangulation.hpp"
#include "../flatsurf/path.hpp"
#include "../flatsurf/path_iterator.hpp"
#include "../flatsurf/saddle_connection.hpp"
#include "../flatsurf/odd_half_edge_map.hpp"
#include "../flatsurf/vector.hpp"

#include "impl/shift_deformation_relation.hpp"

namespace flatsurf {

template <typename Surface>
ShiftDeformationRelation<Surface>::ShiftDeformationRelation(const Surface& domain, const Surface& codomain, OddHalfEdgeMap<Path<Surface>> shift) : DeformationRelation<Surface>(domain, codomain), shifted(shift) {
}

template <typename Surface>
std::optional<Path<Surface>> ShiftDeformationRelation<Surface>::operator()(const Path<Surface>& path) const {
  Path<Surface> image;

  for (const auto& connection : path) {
    if (connection.source() == -connection.target() && connection.vector() == this->domain->fromHalfEdge(connection.source())) {
      image += shifted.get(connection.source());
    } else {
      // TODO: Map half edges on path, then tighten.
      throw std::logic_error("not implemented: ShiftDeformationRelation::operator()");
    }
  }

  return image;
}

template <typename Surface>
std::unique_ptr<DeformationRelation<Surface>> ShiftDeformationRelation<Surface>::clone() const {
  return std::make_unique<ShiftDeformationRelation>(*this);
}

template <typename Surface>
std::unique_ptr<DeformationRelation<Surface>> ShiftDeformationRelation<Surface>::section() const {
  // TODO
  throw std::logic_error("not implemented: ShiftDeformationRelation::section()");
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

}

// Instantiations of templates so implementations are generated for the linker
#include "util/instantiate.ipp"

LIBFLATSURF_INSTANTIATE_MANY_WRAPPED((LIBFLATSURF_INSTANTIATE_STATIC), ShiftDeformationRelation, LIBFLATSURF_FLAT_TRIANGULATION_TYPES)

