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

#include "impl/trivial_deformation_relation.hpp"

#include <ostream>

#include "../flatsurf/path.hpp"
#include "../flatsurf/point.hpp"
#include "impl/deformation_relation.hpp"

namespace flatsurf {

template <typename Surface>
TrivialDeformationRelation<Surface>::TrivialDeformationRelation(const Surface& surface) :
  DeformationRelation<Surface>(surface, surface) {}

template <typename Surface>
std::optional<Path<Surface>> TrivialDeformationRelation<Surface>::operator()(const Path<Surface>& path) const {
  return path;
}

template <typename Surface>
Point<Surface> TrivialDeformationRelation<Surface>::operator()(const Point<Surface>& point) const {
  return point;
}

template <typename Surface>
std::unique_ptr<DeformationRelation<Surface>> TrivialDeformationRelation<Surface>::clone() const {
  return std::make_unique<TrivialDeformationRelation>(this->domain);
}

template <typename Surface>
std::unique_ptr<DeformationRelation<Surface>> TrivialDeformationRelation<Surface>::section() const {
  return clone();
}

template <typename Surface>
bool TrivialDeformationRelation<Surface>::trivial() const {
  return true;
}

template <typename Surface>
std::ostream& TrivialDeformationRelation<Surface>::operator>>(std::ostream& os) const {
  return os << this->domain;
}

}  // namespace flatsurf

// Instantiations of templates so implementations are generated for the linker
#include "util/instantiate.ipp"

LIBFLATSURF_INSTANTIATE_MANY_WRAPPED((LIBFLATSURF_INSTANTIATE_STATIC), TrivialDeformationRelation, LIBFLATSURF_FLAT_TRIANGULATION_TYPES)
