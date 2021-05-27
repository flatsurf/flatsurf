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

#include "impl/insert_marked_deformation_relation.hpp"

#include "../flatsurf/half_edge.hpp"
#include "../flatsurf/path.hpp"

namespace flatsurf {

template <typename Surface>
InsertMarkedDeformationRelation<Surface>::InsertMarkedDeformationRelation(const Surface& domain, const Surface& codomain, const Vertex& inserted) :
  DeformationRelation<Surface>(domain, codomain), inserted(inserted) {}

template <typename Surface>
InsertMarkedDeformationRelation<Surface>::InsertMarkedDeformationRelation(const Surface& domain, const Surface& codomain, const Vertex& inserted, HalfEdge, HalfEdge, HalfEdge) :
  DeformationRelation<Surface>(domain, codomain), inserted(inserted) {}

template <typename Surface>
std::optional<Path<Surface>> InsertMarkedDeformationRelation<Surface>::operator()(const Path<Surface>& path) const {
  if (this->codomain->atVertex(inserted).size() == 3)
    return path;
  else
    throw std::logic_error("not implemented: cannot map paths when a vertex was inserted in the interior of an existing edge");
}

template <typename Surface>
std::unique_ptr<DeformationRelation<Surface>> InsertMarkedDeformationRelation<Surface>::clone() const {
  return std::make_unique<InsertMarkedDeformationRelation>(this->domain, this->codomain, inserted);
}

template <typename Surface>
std::unique_ptr<DeformationRelation<Surface>> InsertMarkedDeformationRelation<Surface>::section() const {
  throw std::logic_error("not implemented: cannot compute section of an insertion of a marked point");
}

template <typename Surface>
bool InsertMarkedDeformationRelation<Surface>::trivial() const {
  return false;
}

template <typename Surface>
std::ostream& InsertMarkedDeformationRelation<Surface>::operator>>(std::ostream& os) const {
  return os << this->domain << " → " << this->codomain << " inserting " << this->inserted;
}

}  // namespace flatsurf

// Instantiations of templates so implementations are generated for the linker
#include "util/instantiate.ipp"

LIBFLATSURF_INSTANTIATE_MANY_WRAPPED((LIBFLATSURF_INSTANTIATE_STATIC), InsertMarkedDeformationRelation, LIBFLATSURF_FLAT_TRIANGULATION_TYPES)
