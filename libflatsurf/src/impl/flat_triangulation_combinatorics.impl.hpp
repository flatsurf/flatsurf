/**********************************************************************
 *  This file is part of flatsurf.
 *
 *        Copyright (C) 2020 Julian Rüth
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

#ifndef LIBFLATSURF_FLAT_TRIANGULATION_COMBINATORICS_IMPL_HPP
#define LIBFLATSURF_FLAT_TRIANGULATION_COMBINATORICS_IMPL_HPP

#include "../../flatsurf/flat_triangulation_combinatorics.hpp"

namespace flatsurf {

template <typename Surface>
template <typename... Args>
FlatTriangulationCombinatorics<Surface>::FlatTriangulationCombinatorics(ProtectedConstructor, Args&&... args)
  : self(std::forward<Args>(args)...) {}

}

#endif
