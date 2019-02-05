/**********************************************************************
 *  This file is part of Polygon.
 *
 *        Copyright (C) 2018 Alex Eskin
 *        Copyright (C) 2019 Julian Rüth
 *
 *  Polygon is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  Polygon is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Polygon. If not, see <https://www.gnu.org/licenses/>.
 *********************************************************************/

#ifndef POLYGON_DIR_H
#define POLYGON_DIR_H

#include <list>

#include "./alg_t.h"
#include "./defs.h"
#include "./oedge.h"

namespace polygon {
class Vertex;

template <typename PointT>
class Dir {
 public:
  PointT vec;
  Vertex *v;
  std::list<std::list<OEdge>::iterator>::iterator ep;

  Dir();
  explicit Dir(std::list<OEdge>::iterator e);
  Dir(Vertex *vp, const PointT &p);
  void Check();
  Point vec_cx();
  alg_tI vec_algtI();

  Dir RotateCwToVec(PointT p);
  Dir RotateCCwToVec(PointT p);

  Dir RotateF(COORD theta);
  Dir RotateF_general(COORD theta);
  Dir RotateI(COORD theta);
  COORD AngleF(Dir &d2);
  int AngleI(Dir &d1, Dir &d2); /* returns angle over pi */
};
}  // namespace polygon

#endif  // POLYGON_DIR_H
