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

#ifndef LIBFLATSURF_DELAUNAY_HPP
#define LIBFLATSURF_DELAUNAY_HPP

#include "forward.hpp"

namespace flatsurf {
enum class DELAUNAY {
  // This edge does not satisfy the usual Delaunay condition, i.e., considering
  // the faces attached to this edge embedded in R², the circumcircle of a face
  // contains a vertex of a face in its interior.
  NON_DELAUNAY = 0,
  // This edge does satisfy the usual Delaunay condition, i.e., considering the
  // faces attached to this edge embedded in R², the circumcircle of a face
  // does not contain a vertex of a face in its interior.
  DELAUNAY = 1,
  // This edge satisfies the Delaunay condition but is ambiguous, i.e., considering the
  // faces attached to this edge embedded in R², the circumcircle of a face
  // contains the vertices of the other face in its boundary.
  AMBIGUOUS = 2,
};

}  // namespace flatsurf

#endif
