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

#include "impl/slit_deformation_relation.hpp"

#include <ostream>

#include "../flatsurf/flat_triangulation.hpp"
#include "../flatsurf/path.hpp"
#include "../flatsurf/point.hpp"
#include "../flatsurf/ray.hpp"

namespace flatsurf {

template <typename Surface>
SlitDeformationRelation<Surface>::SlitDeformationRelation(const Surface& domain, const Surface& codomain) :
  DeformationRelation<Surface>(domain, codomain) {}

template <typename Surface>
std::optional<Path<Surface>> SlitDeformationRelation<Surface>::operator()(const Path<Surface>&) const {
  throw std::logic_error("not implemented: cannot map a path when a slit has been inserted");
}

template <typename Surface>
Point<Surface> SlitDeformationRelation<Surface>::operator()(const Point<Surface>&) const {
  throw std::logic_error("not implemented: cannot map points when a slit has been inserted");
}

template <typename Surface>
Ray<Surface> SlitDeformationRelation<Surface>::operator()(const Ray<Surface>&) const {
  throw std::logic_error("not implemented: cannot map rays when a slit has been inserted");
}

template <typename Surface>
std::unique_ptr<DeformationRelation<Surface>> SlitDeformationRelation<Surface>::clone() const {
  return std::make_unique<SlitDeformationRelation<Surface>>(*this);
}

template <typename Surface>
std::unique_ptr<DeformationRelation<Surface>> SlitDeformationRelation<Surface>::section() const {
  return std::make_unique<SlitDeformationRelation<Surface>>(this->codomain, this->domain);
}

template <typename Surface>
bool SlitDeformationRelation<Surface>::trivial() const {
  return false;
}

template <typename Surface>
std::ostream& SlitDeformationRelation<Surface>::operator>>(std::ostream& os) const {
  return os << this->domain << " → " << this->codomain;
}

}  // namespace flatsurf

// Instantiations of templates so implementations are generated for the linker
#include "util/instantiate.ipp"

LIBFLATSURF_INSTANTIATE_MANY_WRAPPED((LIBFLATSURF_INSTANTIATE_STATIC), SlitDeformationRelation, LIBFLATSURF_FLAT_TRIANGULATION_TYPES)
