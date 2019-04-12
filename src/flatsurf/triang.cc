/**********************************************************************
 *  This file is part of Flatsurf.
 *
 *        Copyright (C) 2018 Alex Eskin
 *        Copyright (C) 2019 Vincent Delecroix
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
 *  along with Flatsurf. If not, see <https://www.gnu.org/licenses/>.
 *********************************************************************/

#include <boost/math/constants/constants.hpp>
#include <cassert>
#include <iostream>
#include <list>

#include "./elementary_geometry.h"
#include "./globals.h"
#include "./my_ostream.h"
#include "./two_complex.h"
#include "./uedge.h"

using boost::math::constants::pi;
using std::abs;
using std::cout;
using std::list;
using std::min;

namespace polygon {
void TwoComplex::TriangulateAll() {
  bool did_something;

  do {
    did_something = false;

    for (auto i = faces.begin(); i != faces.end(); ++i) {
      if ((!(*i)->deleted()) && (*i)->order() != 3) {
        TriangulateFace(*i);
        did_something = true;
        break;
      }
    }

  } while (did_something);
}

static int nbr = 0;
static my_ostream *movie_stream, *dbg_stream;
static char buf[1000];

void TwoComplex::TriangulateFace(Face* f) {
  list<OEdge>::iterator Candidate1 = NULL_OEdgeIter;
  list<OEdge>::iterator Candidate2 = NULL_OEdgeIter;

  COORD best_angle = 0;
  COORD a;

  if (f->order() == 3) {
    return;
  }

  //    if ( f->order > MAX_FACE_ORDER -1 ) {
  //	ERR_RET("TriangulateFace: order of face too large");
  //    }
  if (f->deleted()) {
    ERR_RET("TriangulateFace: face already deleted");
  }

  for (auto i = f->oedges.begin(); i != f->oedges.end(); ++i) {
    Point bs = (*i).vec_cx();
    for (auto j = i; j != f->oedges.end(); ++j) {
      if (i == j) {
        continue;
      }

      //	    a = 0;
      a = min(true_angle((*i).vec_cx(), bs),
              true_angle(bs, -(*i).prev_edge()->vec_cx()));

      a = min(a, true_angle(-bs, -(*j).prev_edge()->vec_cx()));
      a = min(a, true_angle((*j).vec_cx(), -bs));

      if (a < 0) {
        ERR_RET("triangluate face: a < 0 \n");
      }

      // 	    // a = true_angle((*i).vec_cx(),
      // -(*i).prev_edge()->vec_cx()) +
      // 	    // 	true_angle((*j).vec_cx(), -(*j).prev_edge()->vec_cx());

#ifdef USE_LONG_DOUBLE
      printf("F%d V%d(E%d) V%d(E%d) a=%Lg, can_bisect=%d\n", f->id(),
#else
      printf("F%d V%d(E%d) V%d(E%d) a=%g, can_bisect=%d\n", f->id(),
#endif
             (*i).head()->id(), (*i).id(), (*j).head()->id(), (*j).id(), a,
             f->can_bisect(i, j));

      if (a > best_angle && f->can_bisect(i, j)) {
        Candidate1 = i;
        Candidate2 = j;
        best_angle = a;
      }
      bs += (*j).vec_cx();
    }
  }

#ifdef USE_LONG_DOUBLE
  printf("F%d best_angle = %Lg\n", f->id(), best_angle);
#else
  printf("F%d best_angle = %g\n", f->id(), best_angle);
#endif

  //	sprintf(buf,"dbg_bisect");
  //	dbg_stream = new my_ostream(buf);
  //	make_pface(f);
  //	NewDraw(*dbg_stream);
  //	dbg_stream->close();
  //	delete dbg_stream;

  // checking best_angle == 0 in a way that the compiler is not unhappy about
  if (std::abs(best_angle) <= 0 || (!f->can_bisect(Candidate1, Candidate2))) {
#ifdef USE_LONG_DOUBLE
    printf("F%d: Parent Face: F%d best_angle=%Lg", f->id(),
#else
    printf("F%d: Parent Face: F%d best_angle=%g", f->id(),
#endif
           f->parent_face_ID, best_angle);
    if (Candidate1 != NULL_OEdgeIter && Candidate2 != NULL_OEdgeIter) {
      printf(" Candidate1=E%d Candidate2=E%d", Candidate1->id(),
             Candidate2->id());
    }
    printf("\n");
    f->Print(cout);
    printf("\n");
    for (auto i1 = f->oedges.begin(); i1 != f->oedges.end(); ++i1) {
      (*i1).ue->Print(cout);
      printf("\n");
    }

    sprintf(buf, "dbg");
    dbg_stream = new my_ostream(buf);
    make_pface(f);
    NewDraw(*dbg_stream);
    dbg_stream->close();
    delete dbg_stream;

    ERR_RET("TriangulateFace: cannot find candidate");
  }

  auto new_edge = BisectFace(f, Candidate1, Candidate2);
  new_edge->ue->from_triang = true;

  if (verbose >= 1) {
    nbr++;
    sprintf(buf, "movie_triang%d", nbr);
    movie_stream = new my_ostream(buf);
    make_pcomplexes();
    NewDraw(*movie_stream);
    movie_stream->close();
    delete movie_stream;
  }

  //    CheckAllFaces();
  //    CheckAllVertices();

  TriangulateFace(new_edge->pair_edge()->in_face());
  TriangulateFace(new_edge->in_face());
}

list<OEdge>::iterator TwoComplex::BisectFace(Face* f,
                                             list<OEdge>::iterator e1_iter,
                                             list<OEdge>::iterator e2_iter) {
  /* head of e1 to head of e2 */
  /* return new oedge in face 1 (containing e1 ) */

  list<OEdge> new_face1_oedge_list;
  list<OEdge> new_face2_oedge_list;

  BigPointQ bp;
  BigPointQ bs;

  Vertex* v1 = (*e1_iter).head();
  Vertex* v2 = (*e2_iter).head();

  if (f->order() <= 3) {
    ERR_RET("Bisect Face: order of face at most 3");
  }
  if (e1_iter == e2_iter) {
    ERR_RET("Bisect Face: edges the same");
  }

  /* assume face is on the left of edges */

  auto i = e1_iter;

  while (i != e2_iter) {
    new_face1_oedge_list.insert(new_face1_oedge_list.end(), *i);
    bs += (*i).vecQ();

    /* clean up */
    ++i;
    if (i == f->oedges.end()) {
      i = f->oedges.begin();
    }
    if (i == e1_iter) {
      ERR_RET("Bisect Face: e2 not found");
    }
  }

  bp = bs; /* p from v1 to v2 */
  bp.Check();

  if (i != e2_iter) {
    ERR_RET("Bisect Face: this is impossible");
  }

  bs = BigPointQ();  // set to 0;

  while (i != e1_iter) {
    new_face2_oedge_list.insert(new_face2_oedge_list.end(), *i);
    bs += (*i).vecQ();

    /* clean up */
    ++i;
    if (i == f->oedges.end()) {
      i = f->oedges.begin();
    }
    if (i == e2_iter) {
      ERR_RET("Bisect Face: e1 not found");
    }
  }
  bs.Check();

  if (abs(bp.cx + bs.cx) > EPSILON) {
    ERR_RET("Bisect Face: total vec not zero");
  }

  BigPointQ bq = bp + bs;

  if (field_arithmetic && !(bq.algt == alg_tQ())) {  // bq is not zero
    std::cout << bq.algt;
    ERR_RET("Bisect Face: total vec_algt not zero");
  }

  UEdge* new_uedge = AddUEdge(UNDEFINED, v1, v2, bp);
  OEdge* new_oedge1 = new OEdge(new_uedge, -1);
  OEdge* new_oedge2 = new OEdge(new_uedge, 1);

  new_face1_oedge_list.insert(new_face1_oedge_list.end(), *new_oedge1);
  new_face2_oedge_list.insert(new_face2_oedge_list.end(), *new_oedge2);

  if (new_face1_oedge_list.size() < 3) {
    ERR_RET("BisectFace: new_face1 has less then 3 edges");
  }

  if (new_face2_oedge_list.size() < 3) {
    ERR_RET("BisectFace: new_face2 has less then 3 edges");
  }

  AddFace(UNDEFINED, new_face1_oedge_list);
  AddFace(UNDEFINED, new_face2_oedge_list);

  if (new_uedge->boundary()) {
    ERR_RET("BisectFace: bad hookup");
  }

  /* do not know what this does, but seems needed */
  new_uedge->internal = true;
  new_oedge1->from_edge = new_oedge1->pair_edge();
  new_oedge2->from_edge = new_oedge2->pair_edge();
  new_oedge1->in_face()->parent_face_ID = f->id();
  new_oedge2->in_face()->parent_face_ID = f->id();

  f->Delete(); /* this deletes the old face f */
               /* it will be expunged later */

  delete new_oedge2;
  auto ret = new_oedge1->this_iter();
  delete new_oedge1;
  return ret;
}

bool Face::can_bisect(list<OEdge>::iterator i, list<OEdge>::iterator j) {
  if (j == i || j == prev_edge(i) || j == next_edge(i)) {
    return (false);
  }

  Point v1_offset, v2_offset;

  bool i_found = false;
  bool j_found = false;

  Point s = Point(0, 0);
  COORD diameter = 0;

  for (auto k = oedges.begin(); k != oedges.end(); ++k) {
    if (k == i) {
      v1_offset = s;
      i_found = true;
    }
    if (k == j) {
      v2_offset = s;
      j_found = true;
    }
    s += (*k).vec_cx();
    diameter += abs(s);
  }

  if ((!i_found) || (!j_found)) {
    ERR_RET("can_bisect:i or j not found");
  }

  if (abs(s) > EPSILON) {
    ERR_RET("can_bisect: sum of face not zero");
  }

  Point intersection;

  s = Point(0, 0);

  /* Check for intersecting edges */
  for (auto k = oedges.begin(); k != oedges.end(); ++k) {
    if (k != i && k != j && k != prev_edge(i) && k != prev_edge(j) &&
        intersect_segment_interior(v1_offset, v2_offset - v1_offset, s,
                                   (*k).vec_cx(), intersection)) {
      return (false);
    }
    s += (*k).vec_cx();
  }

  /* check that the new edge is inside the face */

  Point midpoint = Point(0.5) * (v1_offset + v2_offset);

  if (contains(midpoint + GetOffsetV0())) {
    return (true);
  }
  return (false);
}

/* for debugging */
// cppcheck-suppress unusedFunction
void TwoComplex::AddSteinerPoint(Face* f) {
  if (f->order() != 3) {
    ERR_RET("AddSteinerPoint: not triangle");
  }

  auto i = f->oedges.begin();
  OEdge* edge01 = &(*i++);
  OEdge* edge12 = &(*i++);
  OEdge* edge20 = &(*i++);
  if (i != f->oedges.end()) {
    ERR_RET("AddSteinerPoint: error");
  }

  Point v0_pos = Point(0, 0);
  Point v1_pos = edge01->vec_cx();
  Point v2_pos = edge01->vec_cx() + edge12->vec_cx();

  Point p_steiner = Point(1.0 / 3) * (v0_pos + v1_pos + v2_pos);
  //  Point p_steiner = 0.1*v0_pos+0.45*v1_pos+0.45*v2_pos;

  std::cout << v0_pos << v1_pos << v2_pos << p_steiner << "\n";

  Vertex* v0 = edge01->head();
  Vertex* v1 = edge12->head();
  Vertex* v2 = edge20->head();

  Vertex* v_steiner = AddVertex(UNDEFINED);
  UEdge* new_uedge0 = AddUEdge(UNDEFINED, v0, v_steiner, p_steiner);
  UEdge* new_uedge1 =
      AddUEdge(UNDEFINED, v1, v_steiner, p_steiner - edge01->vec_cx());
  UEdge* new_uedge2 = AddUEdge(UNDEFINED, v2, v_steiner,
                               p_steiner - edge01->vec_cx() - edge12->vec_cx());

  list<OEdge> tmp_oedge_list;
  OEdge* e0_in = new OEdge(new_uedge0, 1);
  OEdge* e0_out = new OEdge(new_uedge0, -1);
  OEdge* e1_in = new OEdge(new_uedge1, 1);
  OEdge* e1_out = new OEdge(new_uedge1, -1);
  OEdge* e2_in = new OEdge(new_uedge2, 1);
  OEdge* e2_out = new OEdge(new_uedge2, -1);

  tmp_oedge_list.clear();
  tmp_oedge_list.insert(tmp_oedge_list.end(), *edge01);
  tmp_oedge_list.insert(tmp_oedge_list.end(), *e1_in);
  tmp_oedge_list.insert(tmp_oedge_list.end(), *e0_out);
  AddFace(UNDEFINED, tmp_oedge_list);

  tmp_oedge_list.clear();
  tmp_oedge_list.insert(tmp_oedge_list.end(), *edge12);
  tmp_oedge_list.insert(tmp_oedge_list.end(), *e2_in);
  tmp_oedge_list.insert(tmp_oedge_list.end(), *e1_out);
  AddFace(UNDEFINED, tmp_oedge_list);

  tmp_oedge_list.clear();
  tmp_oedge_list.insert(tmp_oedge_list.end(), *edge20);
  tmp_oedge_list.insert(tmp_oedge_list.end(), *e0_in);
  tmp_oedge_list.insert(tmp_oedge_list.end(), *e2_out);
  AddFace(UNDEFINED, tmp_oedge_list);

  /* do not know what this does, but seems needed */
  new_uedge0->internal = true;
  new_uedge1->internal = true;
  new_uedge2->internal = true;

  e0_in->from_edge = e0_in->pair_edge();
  e1_in->from_edge = e1_in->pair_edge();
  e2_in->from_edge = e2_in->pair_edge();

  e0_out->from_edge = e0_out->pair_edge();
  e1_out->from_edge = e1_out->pair_edge();
  e2_out->from_edge = e2_out->pair_edge();

  f->Delete();
}

bool Vertex::CanRemove() {
  if (!euclidean) {
    return (false);
  }

  for (auto i = out_edges.begin(); i != out_edges.end(); ++i) {
    if ((*i)->tail()->id() == id()) {
      return (false);
    }
  }
  return (true);
}

Face* TwoComplex::RemoveVertex(Vertex* v0) {
  if (!v0->euclidean) {
    ERR_RET("Remove Vertex: not euclidean");
  }
  list<OEdge> tmp_oedge_list;

  for (auto i = v0->out_edges.begin(); i != v0->out_edges.end(); ++i) {
    tmp_oedge_list.insert(tmp_oedge_list.end(), *((*i)->next_edge()));
    (*i)->tail()->e = (*i)->next_edge()->ue;
  }
  Face* f = AddFace(UNDEFINED, tmp_oedge_list);

  for (auto i = v0->out_edges.begin(); i != v0->out_edges.end(); ++i) {
    (*i)->tail()->order = (*i)->tail()->order - 1;
    if ((*i)->tail()->e == (*i)->ue) {
      ERR_RET("RemoveVertex: ->e illegal");
    }

    Face* tmp = (*i)->in_face();
    (*i)->Delete();
    tmp->Delete();
  }
  v0->Delete();
  return (f);
}

void TwoComplex::RetriangulateSurface() {
  list<Vertex*>::iterator i;
  list<UEdge*>::iterator j;

  fprintf(out_f, "In RetriangulateSurface\n");

  bool did_something = true;

  while (did_something) {
    did_something = false;
    for (j = uedges.begin(); j != uedges.end(); ++j) {
      if ((*j)->deleted()) {
        continue;
      }
      if ((*j)->v0->id() == (*j)->v1->id() && (*j)->v0->euclidean) {
        /* FIXME: should also check if others are different */
        fprintf(out_f, "Flipping E%d\n", (*j)->id());
        FlipEdge(*j);

        BuildNeighborLists();
        CheckAllFaces();
        CheckAllVertices();

        if (verbose >= 1) {
          nbr++;
          sprintf(buf, "movie_flip%d.tri", nbr);
          movie_stream = new my_ostream(buf);
          make_pcomplexes();
          NewDraw(*movie_stream);
          movie_stream->close();
          delete movie_stream;
        }
        did_something = true;
        break;
      }
    }

    for (i = vertices.begin(); i != vertices.end(); ++i) {
      if ((*i)->deleted()) {
        continue;
      }
      if ((*i)->CanRemove()) {
        break;
      }
    }
    if (i == vertices.end()) { /* no euclidean vertex found */
      continue;
    } else {
      did_something = true;
    }
    fprintf(out_f, "Removing V%d\n", (*i)->id());
    Face* f = RemoveVertex(*i);

    if (verbose >= 1) {
      nbr++;
      sprintf(buf, "movie_triang%d", nbr);
      movie_stream = new my_ostream(buf);
      make_pcomplexes();
      NewDraw(*movie_stream);
      movie_stream->close();
      delete movie_stream;
    }

    //	BuildNeighborLists();
    //	CheckAllVertices();
    CheckAllFaces();

    TriangulateFace(f);
    BuildNeighborLists();
    CheckAllVertices();
    CheckAllFaces();

    if (verbose >= 1) {
      nbr++;
      sprintf(buf, "movie_triang%d", nbr);
      movie_stream = new my_ostream(buf);
      make_pcomplexes();
      NewDraw(*movie_stream);
      movie_stream->close();
      delete movie_stream;
    }
  }

  BuildNeighborLists();
  StoreVertexOffsets();
}

UEdge* TwoComplex::FlipEdge(UEdge* u) {
  /*           vt
                                                  / |\
                                           / u| \
                                          /   |  \
                           w0  f0 |   w1
                                          \   |f1/
                                           \  | /
                                                  \ |/
                                                   vh

  */

  Face* f0 = u->f0;
  Face* f1 = u->f1;

  auto oe = u->this_edge(f0);

  Vertex* vh = oe->head();
  Vertex* vt = oe->tail();

  auto vt_w0 = oe->next_edge();
  auto w0_vh = oe->prev_edge();

  auto vh_w1 = oe->pair_edge()->next_edge();
  auto w1_vt = oe->pair_edge()->prev_edge();

  Vertex* w0 = vt_w0->tail();
  Vertex* w1 = vh_w1->tail();

  BigPointQ w0_w1_p = w0_vh->vecQ() + vh_w1->vecQ();
  if (field_arithmetic) {
    w0_w1_p.SetCxFromAlgebraic();
  }
  UEdge* new_u = AddUEdge(UNDEFINED, w0, w1, w0_w1_p);
  u->internal = true;

  OEdge* w0_w1 = new OEdge(new_u, 1);
  OEdge* w1_w0 = new OEdge(new_u, -1);

  list<OEdge> tmp_oedge_list;

  tmp_oedge_list.clear();
  tmp_oedge_list.insert(tmp_oedge_list.end(), *w0_w1);
  tmp_oedge_list.insert(tmp_oedge_list.end(), *w1_vt);
  tmp_oedge_list.insert(tmp_oedge_list.end(), *vt_w0);

  AddFace(UNDEFINED, tmp_oedge_list);

  tmp_oedge_list.clear();
  tmp_oedge_list.insert(tmp_oedge_list.end(), *w1_w0);
  tmp_oedge_list.insert(tmp_oedge_list.end(), *w0_vh);
  tmp_oedge_list.insert(tmp_oedge_list.end(), *vh_w1);

  AddFace(UNDEFINED, tmp_oedge_list);

  vh->e = vh_w1->ue;
  vt->e = vt_w0->ue;

  /* really out to be part of Delete */
  vh->order--;
  vt->order--;

  u->Delete();
  f0->Delete();
  f1->Delete();

  return (new_u);
}

bool TwoComplex::shouldFlip(UEdge* u) {
  /*           vt
                                                  / |\
                                           / u| \
                                          /   |  \
                           w0  f0 |   w1
                                          \   |f1/
                                           \  | /
                                                  \ |/
                                                   vh

  */

  if (u->deleted()) {
    return (false);
  }

  Face* f0 = u->f0;
  //    Face* f1 = u->f1;

  auto oe = u->this_edge(f0);

  //    Vertex* vh = oe->head();
  //    Vertex* vt = oe->tail();

  auto vt_w0 = oe->next_edge();
  auto w0_vh = oe->prev_edge();

  auto vh_w1 = oe->pair_edge()->next_edge();
  auto w1_vt = oe->pair_edge()->prev_edge();

  //    Vertex* w0 = vt_w0->tail();
  //    Vertex* w1 = vh_w1->tail();

  // the delaunay condition. Eventually will replace by in_circle() (in math.cc)

  if (angle(-vt_w0->vec_cx(), w0_vh->vec_cx()) +
          angle(-vh_w1->vec_cx(), w1_vt->vec_cx()) <
      pi<COORD>() + 100 * EPSILON) {
    return (false);
  }

  // if( norm( OpposingDiagonal(u) ) >= norm(oe->vec_cx()) ) {
  // 	return(false);
  // }

  if (abs(angle(-w0_vh->vec_cx(), oe->vec_cx())) +
          abs(angle(oe->vec_cx(), vh_w1->vec_cx())) >=
      pi<COORD>()) {
    return (false);
  }

  if (abs(angle(-w1_vt->vec_cx(), -oe->vec_cx())) +
          abs(angle(-oe->vec_cx(), vt_w0->vec_cx())) >=
      pi<COORD>()) {
    return (false);
  }

  return (true);
}

void TwoComplex::doExtraFlips() {
  bool did_something;

  do {
    did_something = false;
    for (auto i = uedges.begin(); i != uedges.end(); ++i) {
      if (shouldFlip((*i))) {
        fprintf(out_f, "Extra: Flipping E%d\n", (*i)->id());
        FlipEdge(*i);

        BuildNeighborLists();

        CheckAllFaces();
        CheckAllVertices();

        did_something = true;
        break;
      }
    }
  } while (did_something);
}
}  // namespace polygon
