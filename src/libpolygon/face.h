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

#ifndef LIBPOLYGON_FACE_H
#define LIBPOLYGON_FACE_H

#include <list>
#include <ostream>

#include "big_point.h"
#include "defs.h"
#include "segment.h"
#include "simplex.h"

namespace polygon {
class OEdge;

class Face : public Simplex {
 public:
  Face();
  virtual ~Face();
  explicit Face(const std::list<OEdge>& L);  // sets oedges=L

  void SetOEdgeList(std::list<OEdge>);

  size_t order();
  bool deleted();
  void Delete();
  void Print(std::ostream&);
  void Check();
  COORD volume();
  Point barycenter();

  bool can_bisect(std::list<OEdge>::iterator, std::list<OEdge>::iterator);
  bool self_intersecting();

  std::list<OEdge>::iterator next_iter(std::list<OEdge>::iterator);

  std::list<OEdge>::iterator prev_iter(std::list<OEdge>::iterator);

  Point GetOffsetV0();
  algebraicQ GetOffsetV0_alg();
  /* bad in non-elementary situation */
  //    Point GetOffsetV(VertexPtr);

  bool intersects(Point pb, Point pv);
  bool intersects(Face* f, Point offset);
  bool contains(Point p);

  COORD perimeter();
  int parent_face_ID;  // for debugging triangulation

  BigPointQ vec;

  std::list<OEdge> oedges;

  void AddSegmentToDraw(Segment segment);
  void ClearSegmentsToDraw();
  std::list<Segment> segments_to_draw;
  // for billiard mode
  BigPointQ reflection_vector;
  Face* from_face;

  Face* transform_to_original(BigPointQ v, BigPointQ& v_orig);
};
}  // namespace polygon

#endif  // LIBPOLYGON_FACE_H
