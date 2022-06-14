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

#ifndef LIBFLATSURF_IMPL_RETRIANGULATION_DEFORMATION_RELATION_HPP
#define LIBFLATSURF_IMPL_RETRIANGULATION_DEFORMATION_RELATION_HPP

#include "deformation_relation.hpp"

namespace flatsurf {

template <typename Surface>
class RetriangulationDeformationRelation : public DeformationRelation<Surface> {
 protected:
  using DeformationRelation<Surface>::DeformationRelation;

 public:
  std::optional<Path<Surface>> operator()(const Path<Surface>&) const override;
  Point<Surface> operator()(const Point<Surface>&) const override;
};

}  // namespace flatsurf

#endif
