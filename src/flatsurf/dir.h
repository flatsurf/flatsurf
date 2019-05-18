/**********************************************************************
 *  This file is part of flatsurf.
 *
 *        Copyright (C) 2018 Alex Eskin
 *        Copyright (C) 2019 Julian Rüth
 *
 *  Flatsurf is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 of the License, or
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

#ifndef FLATSURF_DIR_H
#define FLATSURF_DIR_H

#include <list>

#include <flatsurf/forward.hpp>
#include "./alg_t.h"
#include "./defs.h"
#include "./oedge.h"

namespace polygon {
class Vertex;

// The vector vec understood as pointing from v in the sector next to ep.
// Note that two vectors with the same vec and v may not describe the same
// vector on the surface if the total angle around v exceeds 2π.
template <typename PointT>
class[[deprecated("Use flatsurf::VectorAlongTriangulation or some other vector type in flatsurf instead.")]] Dir {
 public:
  PointT vec;
  Vertex* v;
  std::list<std::list<OEdge>::iterator>::iterator ep;

  Dir();
  // A Dir corresponding to an edge in the triangulation of the surface.
  explicit Dir(std::list<OEdge>::iterator e);
  // Construct a Dir that points from vp in direction p. Note that the input
  // does not uniquely determine a dir as there are several ep that produce
  // such a Dir() in a non-equivalent way; see comment at the top.
  Dir(Vertex * vp, const PointT& p);
  // Construct the unique Dir that is in the sector counterclockwise next to
  // e, points from the source of e in direction p.
  Dir(const flatsurf::HalfEdge& e, const PointT& p);
  void Check();
  Point vec_cx();
  alg_tI vec_algtI();

  Dir RotateCwToVec(PointT p);
  Dir RotateCCwToVec(PointT p);

  Dir RotateF(COORD theta);
  Dir RotateF_general(COORD theta);
  Dir RotateI(COORD theta);
  // The angle between this Dir and d2, possibly exceeding 2π.
  COORD AngleF(Dir & d2);
  int AngleI(Dir & d1, Dir & d2); /* returns angle over pi */
};
}  // namespace polygon

#endif  // FLATSURF_DIR_H
