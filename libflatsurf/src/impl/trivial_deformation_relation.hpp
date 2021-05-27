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

#ifndef LIBFLATSURF_IMPL_TRIVIAL_DEFORMATION_RELATION_HPP
#define LIBFLATSURF_IMPL_TRIVIAL_DEFORMATION_RELATION_HPP

#include "deformation_relation.hpp"

namespace flatsurf {

template <typename Surface>
class TrivialDeformationRelation : public DeformationRelation<Surface> {
 public:
  explicit TrivialDeformationRelation(const Surface&);

  std::optional<Path<Surface>> operator()(const Path<Surface>&) const override;

  std::unique_ptr<DeformationRelation<Surface>> clone() const override;

  std::unique_ptr<DeformationRelation<Surface>> section() const override;

  bool trivial() const override;

  std::ostream& operator>>(std::ostream& os) const override;
};

}  // namespace flatsurf

#endif
