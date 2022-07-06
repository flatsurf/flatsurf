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

#include "impl/retriangulation_deformation_relation.hpp"

#include "../flatsurf/flat_triangulation.hpp"
#include "../flatsurf/path.hpp"
#include "../flatsurf/point.hpp"

namespace flatsurf {

template <typename Surface>
std::optional<Path<Surface>> RetriangulationDeformationRelation<Surface>::operator()(const Path<Surface>&) const {
  throw std::logic_error("not implemented: cannot compute image of a path when a generic retriangulation happened");
}

template <typename Surface>
Point<Surface> RetriangulationDeformationRelation<Surface>::operator()(const Point<Surface>&) const {
  throw std::logic_error("not implemented: cannot map points with this retriangulation yet");
}

}  // namespace flatsurf

// Instantiations of templates so implementations are generated for the linker
#include "util/instantiate.ipp"

LIBFLATSURF_INSTANTIATE_MANY_WRAPPED((LIBFLATSURF_INSTANTIATE_STATIC), RetriangulationDeformationRelation, LIBFLATSURF_FLAT_TRIANGULATION_TYPES)
