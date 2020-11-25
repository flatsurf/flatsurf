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

#ifndef LIBFLATSURF_ISOMORPHISM_HPP
#define LIBFLATSURF_ISOMORPHISM_HPP

#include "forward.hpp"

namespace flatsurf {
// The nature of an isomorphism of a flat triangulated surface.
enum class ISOMORPHISM {
  // An isomorphism coming from a bijection of all half edges of the
  // triangulated surfaces.
  FACES = 0,
  // An isomorphism coming from a bijection of the half edges that for the
  // Delaunay cells, i.e., only those edges that are not ambiguous in a
  // Delaunay triangulation.
  // For this kind of isomorphism to make any sense, the surface should be
  // Delaunay triangulated by calling `.delaunay()` first.
  DELAUNAY_CELLS = 1,
};

}  // namespace flatsurf

#endif
