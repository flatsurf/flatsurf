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

#ifndef FLATSURF_OEDGE_H
#define FLATSURF_OEDGE_H

#include <list>
#include <ostream>

#include "./alg_t.h"
#include "./big_point.h"
#include "./defs.h"
#include "./face.h"
#include "./vertex.h"
#include "flatsurf/forward.hpp"

namespace polygon {
class UEdge;

class OEdge {  // oriented edge.
  friend class Face;
  friend class PUEdge;
  friend class UEdge;
  friend class TwoComplex;

 public:
  OEdge(UEdge*, int);
  explicit OEdge(const flatsurf::HalfEdge& e);

  int id();
  explicit operator flatsurf::HalfEdge() const;
  Vertex* head();
  Vertex* tail();
  Face* in_face();     // face to right of OEdge
  Face* other_face();  // face to left of OEdge

  std::list<OEdge>::iterator
  this_iter();  // returns iterator for OEdge in list of edges around
                // face in which OEdge lies.
  std::list<std::list<OEdge>::iterator>::iterator
  this_vert_iter();  // returns iterator for this edge in v.out_edges, where v
                     // is the head of this OEdge
  std::list<OEdge>::iterator
  next_edge();  // next edge in face in which OEdge lies
  std::list<OEdge>::iterator prev_edge();
  std::list<OEdge>::iterator
  pair_edge();  // iterator for same edge in opposite face
  template <typename PointT>
  PointT vec();
  Point vec_cx();
  alg_tQ vec_algt();
  BigPointQ vecQ();
  BigPointI vecI();

  bool deleted();
  void Delete();

  template <typename PointT>
  PointT headOffset();

  template <typename PointT>
  PointT tailOffset();

  void set_direction(int);
  void set_vec_cx(Point);
  void set_vec_algt(alg_tQ);
  void set_vec(BigPointQ);

  friend std::ostream& operator<<(std::ostream&, const OEdge&);
  void Check();

  bool operator==(OEdge&);

  /* next 5 for speed: assigned in StoreVertexOffsets */
  template <typename PointT>
  PointT& _head_offset();

  template <typename PointT>
  PointT& _tail_offset();

  template <typename PointT>
  PointT& _vec();

  std::list<OEdge>::iterator next_e;  // value of next_edge()
  std::list<OEdge>::iterator pair_e;

  std::list<OEdge>::iterator from_edge; /* for reflect_face */

  // make next four private
  Point head_offst;
  Point tail_offst;
  BigPointI head_offstI;
  BigPointI tail_offstI;

 private:
  int direction;  // direction ==1 means ue->v0 is the head
  UEdge* ue;      // underlying unoriented edge
};

inline std::list<OEdge>::iterator next_edge(std::list<OEdge>::iterator i) {
  Face* f = i->in_face();
  ++i;
  if (i == f->oedges.end()) {
    i = f->oedges.begin();
  }
  return (i);
}

inline std::list<OEdge>::iterator prev_edge(std::list<OEdge>::iterator i) {
  Face* f = i->in_face();
  if (i == f->oedges.begin()) {
    i = f->oedges.end();
  }
  --i;
  return (i);
}

inline std::list<std::list<OEdge>::iterator>::iterator next_vert_edge(
    std::list<std::list<OEdge>::iterator>::iterator
        i)  // returns next edge leaving head of *i
{
  Vertex* v = (*i)->head();
  ++i;
  if (i == v->out_edges.end()) {
    i = v->out_edges.begin();
  }
  return (i);
}

inline std::list<std::list<OEdge>::iterator>::iterator prev_vert_edge(
    std::list<std::list<OEdge>::iterator>::iterator i) {
  Vertex* v = (*i)->head();

  if (i == v->out_edges.begin()) {
    i = v->out_edges.end();
  }
  --i;
  return (i);
}
}  // namespace polygon

#endif  // FLATSURF_OEDGE_H
