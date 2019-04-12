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

#ifndef FLATSURF_UEDGE_H
#define FLATSURF_UEDGE_H

#include <ostream>

#include "./big_point.h"
#include "./defs.h"
#include "./simplex.h"
#include "./vertex.h"

namespace polygon {
class Face;

class UEdge : public Simplex {  // unoriented edge
  friend class OEdge;
  friend class Vertex;
  friend class Face;
  friend class PUEdge;
  friend class TwoComplex;

 public:
  UEdge(Vertex* a, Vertex* b,
        Point vec);  // Edge begining at vertex a, ending at b with holonomy vec
  UEdge(Vertex* a, Vertex* b, BigPointQ vec);
  void Print(std::ostream&);
  bool deleted();
  void Delete();
  bool boundary();  // true if UEdge is a boundary edge
  Face* boundary_face();
  std::list<OEdge>::iterator boundary_edge();
  std::list<OEdge>::iterator this_edge(Face* f);
  void set_null_face(Face* f);
  COORD len();

  int index;
  int internal; /* for reflect */
  int label;    /* for reflect */

  int get_priority();  /* for drawing */
  int from_triang;     /* for drawing */
  void set_priority(); /* for drawing */

 private:
  Vertex* v0;
  Vertex* v1;
  Face* f0;  // left face, going from v0 to v1
  Face* f1;  // right face

  BigPointQ ue_vecQ;  // holonomy
  BigPointI ue_vecI;  // holonomy: BigPointI version

  BigPointQ minus_ue_vecQ;  //-holonomy, set by StoreVertexOffsets
  BigPointI minus_ue_vecI;
};
}  // namespace polygon

#endif  // FLATSURF_UEDGE_H
