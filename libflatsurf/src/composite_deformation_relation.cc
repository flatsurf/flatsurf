/**********************************************************************
 *  This file is part of flatsurf.
 *
 *        Copyright (C) 2021-2022 Julian Rüth
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

#include "impl/composite_deformation_relation.hpp"

#include <ostream>

#include "../flatsurf/flat_triangulation.hpp"
#include "../flatsurf/path.hpp"
#include "../flatsurf/point.hpp"
#include "impl/deformation.impl.hpp"

namespace flatsurf {

template <typename Surface>
CompositeDeformationRelation<Surface>::CompositeDeformationRelation(const DeformationRelation<Surface>& lhs, const DeformationRelation<Surface>& rhs) :
  DeformationRelation<Surface>(rhs.domain, lhs.codomain), lhs(lhs.clone()), rhs(rhs.clone()) {}

template <typename Surface>
std::optional<Path<Surface>> CompositeDeformationRelation<Surface>::operator()(const Path<Surface>& path) const {
  auto ret = rhs->operator()(path);
  if (ret)
    ret = lhs->operator()(*ret);
  return ret;
}

template <typename Surface>
Point<Surface> CompositeDeformationRelation<Surface>::operator()(const Point<Surface>& point) const {
  return lhs->operator()(rhs->operator()(point));
}

template <typename Surface>
std::unique_ptr<DeformationRelation<Surface>> CompositeDeformationRelation<Surface>::clone() const {
  return std::make_unique<CompositeDeformationRelation<Surface>>(*lhs, *rhs);
}

template <typename Surface>
std::unique_ptr<DeformationRelation<Surface>> CompositeDeformationRelation<Surface>::section() const {
  return std::make_unique<CompositeDeformationRelation<Surface>>(*rhs->section(), *lhs->section());
}

template <typename Surface>
bool CompositeDeformationRelation<Surface>::trivial() const {
  if (lhs->trivial())
    return rhs->trivial();
  if (rhs->trivial())
    return lhs->trivial();

  return DeformationRelation<Surface>::trivial();
}

template <typename Surface>
std::ostream& CompositeDeformationRelation<Surface>::operator>>(std::ostream& os) const {
  return os << "(" << *rhs << ") then (" << *lhs << ")";
}

}  // namespace flatsurf

// Instantiations of templates so implementations are generated for the linker
#include "util/instantiate.ipp"

LIBFLATSURF_INSTANTIATE_MANY_WRAPPED((LIBFLATSURF_INSTANTIATE_STATIC), CompositeDeformationRelation, LIBFLATSURF_FLAT_TRIANGULATION_TYPES)
