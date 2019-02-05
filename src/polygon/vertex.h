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

#ifndef POLYGON_VERTEX_H
#define POLYGON_VERTEX_H

#include <list>

#include "../libpolygon/forward.hpp"
#include "./defs.h"
#include "./simplex.h"

namespace polygon {
class UEdge;
class OEdge;

class Vertex : public Simplex {
 public:
  Vertex();

  UEdge* e;
  size_t order;  // number of incident edges? Default value 0.
  bool euclidean;
  int int_angle; /* for speed: total_angle_over_pi() */
                 /* Stored by BuildNeighborLists */
  void Check();
  bool boundary();
  bool deleted() const;
  COORD total_angle();
  int total_angle_over_pi();
  void MoveVertex(Point p);
  bool relevant() const;

  static Vertex& from(const flatsurf::Vertex&);
  explicit operator flatsurf::Vertex() const;

  bool CanRemove();
  void Delete();

  std::list<std::list<OEdge>::iterator> out_edges;
};
}  // namespace polygon

#endif  // POLYGON_PVERTEX_H
