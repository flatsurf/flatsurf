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

#include "../flatsurf/edge.hpp"
#include "../flatsurf/saddle_connection.hpp"
#include "../flatsurf/path.hpp"

#include "impl/deformation_relation.hpp"

namespace flatsurf {

template <typename Surface>
bool DeformationRelation<Surface>::trivial() const {
  if (*domain != *codomain)
    return false;

  for (const auto& e : domain->edges())
    if (this->operator()(SaddleConnection<Surface>(*domain, e.positive())) != SaddleConnection<Surface>(*codomain, e.positive()))
      return false;

  return true;
}

}

// Instantiations of templates so implementations are generated for the linker
#include "util/instantiate.ipp"

LIBFLATSURF_INSTANTIATE_MANY_WRAPPED((LIBFLATSURF_INSTANTIATE_STATIC), DeformationRelation, LIBFLATSURF_FLAT_TRIANGULATION_TYPES)
