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

#ifndef FLATSURF_VERTEX_H
#define FLATSURF_VERTEX_H

#include <list>

#include "./defs.h"
#include "./simplex.h"
#include "flatsurf/forward.hpp"

namespace polygon {
class UEdge;
class OEdge;

class [[deprecated("Use flatsurf::Vertex instead.")]] Vertex : public Simplex {
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

#endif  // FLATSURF_PVERTEX_H
