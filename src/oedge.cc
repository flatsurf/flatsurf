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

#include <list>
#include <ostream>

#include "libpolygon/face.h"
#include "libpolygon/globals.h"
#include "libpolygon/two_complex.h"
#include "libpolygon/uedge.h"

using std::list;
using std::ostream;

namespace polygon {
OEdge::OEdge(UEdge* e, int dir) : from_edge(NULL_OEdgeIter) {
  ue = e;
  direction = dir;
}

bool OEdge::operator==(OEdge& b) {
  if (this->ue == b.ue && this->direction == b.direction) {
    return (true);
  }
  return (false);
}

int OEdge::id() { return (ue->id()); }

void OEdge::Print(ostream& out) {
  if (direction == 1)
    out << " +";
  else
    out << " -";

  //    out << (direction == 1) ? '+' : '-';
  //    fprintf(out_f,"%c", (direction == 1) ? '+' : '-');
  ue->Simplex::Print(out);
}

Vertex* OEdge::head() {
  if (direction == 1) {
    return (ue->v0);
  }
  return (ue->v1);
}

Vertex* OEdge::tail() {
  if (direction == 1) {
    return (ue->v1);
  }
  return (ue->v0);
}

Face* OEdge::in_face() {
  if (direction == 1) {
    return (ue->f0);
  }
  return (ue->f1);
}

Face* OEdge::other_face() {
  if (direction == 1) {
    return (ue->f1);
  }
  return (ue->f0);
}

bool OEdge::deleted() { return (ue->deleted()); }

void OEdge::Delete() { ue->Delete(); }

list<OEdge>::iterator OEdge::this_iter() {
  Face* f;

  f = in_face();

  for (auto i = f->oedges.begin(); i != f->oedges.end(); ++i) {
    if ((*i) == *this) {
      return (i);
    }
  }
  ERR_RET("this_iter: not found");
}

list<OEdge>::iterator OEdge::pair_edge() {
  Face* f;

  f = other_face();

  for (auto i = f->oedges.begin(); i != f->oedges.end(); ++i) {
    if ((*i).ue == this->ue && (*i).direction == -this->direction) {
      return (i);
    }
  }
  ERR_RET("PairEdge: not found");
}

template <>
BigPointQ OEdge::vec<BigPointQ>() {
  if (direction == 1) {
    return (ue->ue_vecQ);
  } else {
    return (-ue->ue_vecQ);
  }
}

BigPointQ OEdge::vecQ() { return (vec<BigPointQ>()); }

template <>
BigPointI OEdge::vec<BigPointI>() {
  if (direction == 1) {
    return (ue->ue_vecI);
  } else {
    return (-ue->ue_vecI);
  }
}

template <>
BigPointI& OEdge::_vec<BigPointI>() {
  if (direction == 1) {
    return (ue->ue_vecI);
  } else {
    return (ue->minus_ue_vecI);
  }
}

template <>
Point OEdge::vec<Point>() {
  if (direction == 1) {
    return (ue->ue_vecQ.cx);
  } else {
    return (-ue->ue_vecQ.cx);
  }
}

template <>
Point& OEdge::_vec<Point>() {
  if (direction == 1) {
    return (ue->ue_vecQ.cx);
  } else {
    return (ue->minus_ue_vecQ.cx);
  }
}

Point OEdge::vec_cx() { return (vec<Point>()); }

/*
BigPointQ OEdge::vecQ()
{
                BigPointQ q;

                if( direction == 1 ) {
                                return( ue->ue_vecQ );
                } else {
                                return( -ue->ue_vecQ );
                }
}
*/

// Point OEdge::vec_cx()
// {
//     if( direction == 1 ) {
// 	return( ue->ue_vecQ.cx );
//     } else {
// 	return( -ue->ue_vecQ.cx );
//     }
// }

alg_tQ OEdge::vec_algt() {
  if (direction == 1) {
    return (ue->ue_vecQ.algt);
  } else {
    return (-ue->ue_vecQ.algt);
  }
}

void OEdge::set_vec_cx(Point p) {
  p *= direction;
  ue->ue_vecQ.cx = p;
}

void OEdge::set_vec_algt(alg_tQ p) {
  p *= direction;
  ue->ue_vecQ.algt = p;
}

/* the next function is stupid */
list<OEdge>::iterator OEdge::prev_edge() {
  list<OEdge>::iterator i;
  Face* f;

  f = in_face();

  i = this_iter();

  if (i == f->oedges.begin()) {
    i = f->oedges.end();
  }
  --i;
  return (i);
}

/* this is also stupid */
list<OEdge>::iterator OEdge::next_edge() {
  Face* f;

  f = in_face();

  auto i = this_iter();
  ++i;

  if (i == f->oedges.end()) {
    i = f->oedges.begin();
  }
  return (i);
}

list<list<OEdge>::iterator>::iterator OEdge::this_vert_iter() {
  Vertex* v;

  v = head();

  for (auto i = v->out_edges.begin(); i != v->out_edges.end(); ++i) {
    if (*(*i) == *this) {
      return (i);
    }
  }
  return (NULL_OEdgePtrIter);
}

template <>
Point OEdge::headOffset<Point>() {
  Point q, t;

  Face* f = in_face();

  //    q = f->GetOffsetV0();
  q = Point(0.0);

  for (auto i = f->oedges.begin(); i != f->oedges.end(); ++i) {
    if ((*i) == *this) {
      return (q);
    }
    t = (*i).vec_cx();
    q = q + t;
  };
  ERR_RET("headOffset: oedge not found");
}

template <>
BigPointI OEdge::headOffset<BigPointI>() {
  BigPointI q, t;

  Face* f = in_face();

  //    q = f->GetOffsetV0();
  q = BigPointI();
  q.Check();

  for (auto i = f->oedges.begin(); i != f->oedges.end(); ++i) {
    if ((*i) == *this) {
      return (q);
    }
    t = (*i).vec<BigPointI>();
    q = q + t;
  };
  ERR_RET("headOffset: oedge not found");
}

template <>
Point OEdge::tailOffset<Point>() {
  Point q, t;

  Face* f = in_face();
  //    q = f->GetOffsetV0();
  q = Point(0.0);

  for (auto i = f->oedges.begin(); i != f->oedges.end(); ++i) {
    if ((*i) == *this) {
      return (q + (*i).vec_cx());
    }
    t = (*i).vec_cx();
    q = q + t;
  };
  ERR_RET("tailOffset: oedge not found");
}

template <>
BigPointI OEdge::tailOffset<BigPointI>() {
  BigPointI q, t;

  Face* f = in_face();
  //    q = f->GetOffsetV0();
  q = BigPointI();
  q.Check();

  for (auto i = f->oedges.begin(); i != f->oedges.end(); ++i) {
    if ((*i) == *this) {
      return (q + (*i).vec<BigPointI>());
    }
    t = (*i).vec<BigPointI>();
    q = q + t;
  };
  ERR_RET("tailOffset: oedge not found");
}

template <>
Point& OEdge::_head_offset<Point>() {
  return head_offst;
}

template <>
BigPointI& OEdge::_head_offset<BigPointI>() {
  return head_offstI;
}

template <>
Point& OEdge::_tail_offset<Point>() {
  return tail_offst;
}

template <>
BigPointI& OEdge::_tail_offset<BigPointI>() {
  return tail_offstI;
}
}  // namespace polygon
