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

#ifndef LIBFLATSURF_IMPL_GENERIC_DEFORMATION_RELATION_HPP
#define LIBFLATSURF_IMPL_GENERIC_DEFORMATION_RELATION_HPP

#include <vector>
#include <utility>

#include "../../flatsurf/saddle_connection.hpp"

#include "retriangulation_deformation_relation.hpp"

namespace flatsurf {

// TODO: Should this rather be a GenericRetriangulationDeformation?
template <typename Surface>
class GenericRetriangulationDeformationRelation : public RetriangulationDeformationRelation<Surface> {
  using T = typename Surface::Coordinate;

 public:
  // TODO: For the callers it is enough to pass a single relation. The
  // implementation could then compute all the relations the first time they
  // are needed. In particular, there should be constructor that just takes
  // two paths. Or even Half Edges instead.
  GenericRetriangulationDeformationRelation(const Surface& domain, const Surface& codomain, std::vector<std::pair<Path<Surface>, Path<Surface>>> relation);

  std::optional<Path<Surface>> operator()(const Path<Surface>&) const override;

  std::unique_ptr<DeformationRelation<Surface>> clone() const override;

  std::unique_ptr<DeformationRelation<Surface>> section() const override;

  bool trivial() const override;
  
  std::ostream& operator>>(std::ostream&) const override;

  std::vector<std::pair<Path<Surface>, Path<Surface>>> relation;
};

}

#endif
