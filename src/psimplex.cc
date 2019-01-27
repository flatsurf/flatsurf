/**********************************************************************
 *  This file is part of Polygon.
 *
 *        Copyright (C) 2018 Alex Eskin
 *        Copyright (C) 2019 Vincent Delecroix
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

#include <fstream>
#include <iostream>
#include <list>
#include <string>

#include "libpolygon/elementary_geometry.h"
#include "libpolygon/globals.h"
#include "libpolygon/my_ostream.h"
#include "libpolygon/pface.h"
#include "libpolygon/psimplex.h"
#include "libpolygon/puedge.h"
#include "libpolygon/pvertex.h"
#include "libpolygon/two_complex.h"

using std::endl;
using std::list;
using std::ofstream;
using std::ostream;
using std::string;

namespace polygon {
string roman_numeral(__attribute__((unused)) int decimalNumber) {
  throw std::logic_error(
      "roman_numeral: The implementation of this function had to be removed "
      "since we're not sure about  its license");
}

my_ostream::my_ostream(const string& filename) {
  tri_stream.open((filename + ".tri").c_str());
  if (tikz_output) {
    tex_stream.open((filename + ".tex").c_str());
    tex_stream << "\\documentclass[12pt]{article}" << endl;
    tex_stream << "\\usepackage{tikz}" << endl;
    tex_stream << "\\begin{document}" << endl;
    tex_stream << "\\begin{tikzpicture}[scale=0.50]" << endl;

    int count = 1;
    for (auto i = S->vertices.begin(); i != S->vertices.end(); ++i) {
      int v_id = (*i)->id();
      tex_stream << "\\def\\v" << roman_numeral(v_id) << "color{[white!"
                 << 20 * (count) << "!green]}" << endl;
      count++;
    }
    for (int j = 0; j < 10; j++) {
      tex_stream << "\\def\\c" << roman_numeral(j) << "color{[red!"
                 << 10 * (j + 1) << "!white]}" << endl;
    }
  }
}

ofstream& my_ostream::tri() { return tri_stream; }

ofstream& my_ostream::tex() { return tex_stream; }

void my_ostream::close() {
  tri_stream.close();
  if (tikz_output) {
    tex_stream << "\\end{tikzpicture}" << endl;
    tex_stream << "\\end{document}" << endl;
    tex_stream.close();
  }
}

PSimplex::~PSimplex() { /* empty destructor */
}

void PSimplex::Draw(__attribute__((unused)) my_ostream& output_stream,
                    __attribute__((unused)) COORD d) {
  ERR_RET("call to PSimplex::Draw");
}

PSimplex::PSimplex(Point p0, size_t i) : p(p0) { in_pcomplex = i; }

Simplex* PSimplex::sp() { ERR_RET("call to Psimplex::sp()"); }

PVertex::PVertex(Vertex* v0, Point p0, size_t i) : PSimplex(p0, i) { s = v0; }

Simplex* PVertex::sp() { return static_cast<Simplex*>(s); }

PUEdge::PUEdge(OEdge& oe, Point p0, size_t i) : PSimplex(p0, i) {
  Point t;

  //  p has to be position of v0
  // p0 is the position of the head of oe.

  s = oe.ue;

  if (oe.direction == -1) {
    t = oe.vec_cx();
    p = p0 + t;
  }
}

PUEdge::PUEdge(UEdge* e0, Point p0, size_t i) : PSimplex(p0, i) { s = e0; }

Simplex* PUEdge::sp() { return static_cast<Simplex*>(s); }

PFace::PFace(Face* f0, Point p0, size_t i) : PSimplex(p0, i) { s = f0; }

Simplex* PFace::sp() { return static_cast<Simplex*>(s); }

void PVertex::Draw(my_ostream& output_stream, __attribute__((unused)) COORD d) {
  output_stream.tri() << s->tag << s->id() << " " << s->color << " ";
  output_stream.tri() << p.real() << " " << p.imag() << "\n";

  if (tikz_output && !billiard_mode) {
    output_stream.tex() << "%" << s->tag << s->id() << endl;
    output_stream.tex() << "\\filldraw \\v" << roman_numeral(s->id())
                        << "color (" << p.real() << "," << p.imag()
                        << ") circle (0.2cm); " << endl;
  }
}

void PUEdge::Draw(my_ostream& output_stream, COORD scale_factor) {
  Point tmp;

  output_stream.tri() << s->tag << s->id() << " " << s->color << " ";
  output_stream.tri() << p.real() << " " << p.imag() << " ";
  tmp = s->ue_vecQ.cx;
  tmp = tmp * scale_factor;
  tmp = tmp + p;
  output_stream.tri() << tmp.real() << " " << tmp.imag() << "\n";

  if (tikz_output && !billiard_mode) {
    output_stream.tex() << "%" << s->tag << s->id() << endl;
    if (!retriangulate && (s->internal || s->from_triang)) {
      output_stream.tex() << "% internal edge -- skipping" << endl;
    } else {
      output_stream.tex() << "\\draw (" << p.real() << "," << p.imag()
                          << ") -- (" << tmp.real() << "," << tmp.imag() << ");"
                          << endl;
    }
  }
}

void PFace::Draw(my_ostream& output_stream, COORD scale_factor) {
  Point q(0.0, 0.0), t;

  output_stream.tri() << s->tag << s->id() << " " << s->color << " ";
  if (tikz_output && (!billiard_mode || s->from_face == nullptr)) {
    output_stream.tex() << "%" << s->tag << s->id() << endl;
    output_stream.tex() << "\\fill[blue!20!white] ";
  }

  q = s->GetOffsetV0();
  q = q * scale_factor;
  q = q + p;

  for (auto i = s->oedges.begin(); i != s->oedges.end(); ++i) {
    output_stream.tri() << q.real() << " " << q.imag() << " ";
    if (tikz_output && (!billiard_mode || s->from_face == nullptr)) {
      output_stream.tex() << "(" << q.real() << "," << q.imag() << ") -- ";
    }
    t = (*i).vec_cx();
    t = t * scale_factor;
    q = q + t;
  }
  output_stream.tri() << "\n";
  if (tikz_output && (!billiard_mode || s->from_face == nullptr)) {
    output_stream.tex() << "cycle;" << endl;
  }

  //  Point p1 = p+s->barycenter()*scale_factor;
  // draw normal
  // q = (s->vec.cx);
  // q *= 0.05*scale_factor;
  // q += p1;
  // output_stream << "N" << s->id() <<" "<< p1.real() << " " <<p1.imag() <<"
  // "<< q.real()<< " " <<q.imag() << "\n";

  /* draw extra segments (for displaying saddle connections ) */
  for (auto i = s->segments_to_draw.begin(); i != s->segments_to_draw.end();
       ++i) {
    Point q1 = p + (*i).head * scale_factor;
    Point q2 = p + (*i).tail * scale_factor;
    output_stream.tri() << "N" << s->id() << " " << q1.real() << " "
                        << q1.imag() << " " << q2.real() << " " << q2.imag()
                        << "\n";
    if (tikz_output) {
      output_stream.tex() << "%N"
                          << " cyl = " << i->cyl_on_left
                          << " length = " << i->cyl_on_left_length << endl;
      output_stream.tex() << "\\draw \\c" << roman_numeral(i->cyl_on_left)
                          << "color (" << q1.real() << "," << q1.imag()
                          << ") -- (" << q2.real() << "," << q2.imag() << ");"
                          << endl;
    }
  }
  if (draw_cylinders && !billiard_mode) {
    DrawCylinders(output_stream, scale_factor);
  }
}

class tFace {
 public:
  list<Point> verts;
  list<Point>::iterator FindPoint(Point q);
  void Draw(my_ostream& output_stream, int cyl_nbr);
  void Print(ostream& out);
};

typedef list<Point>::iterator PointListIter;
typedef list<tFace>::iterator tFaceListIter;

void tFace::Print(ostream& out) {
  for (PointListIter k = verts.begin(); k != verts.end(); ++k) {
    out << *k;
  }
  out << endl;
}

void tFace::Draw(my_ostream& output_stream, int cyl_nbr) {
  if (tikz_output) {
    output_stream.tex() << "% cyl #" << cyl_nbr << endl;
    //	output_stream.tex() << "\\fill [red!" << 10*(cyl_nbr) << "!white] ";
    output_stream.tex() << "\\fill \\c" << roman_numeral(cyl_nbr) << "color ";
    for (PointListIter k = verts.begin(); k != verts.end(); ++k) {
      output_stream.tex() << "(" << k->real() << "," << k->imag() << ") -- ";
    }
    output_stream.tex() << "cycle;" << endl;
  }
}

bool fake_colinear(const Point& p, const Point& q) {
  COORD threshold = 1E-8;

  //    COORD threshold = 1E-2;

  COORD c = p.real() * q.imag() - p.imag() * q.real();

  if (c * c > threshold * norm(p) * norm(q)) {
    // fast path
    return (false);
  }
  return (true);
}

bool fake_aligned(const Point& p, const Point& q) {
  if (!fake_colinear(p, q)) {
    return (false);
  }
  /*
                  if ( angle(p,q) > 0.5*MY_PI ) {
                                  return(false);
                  }
  */

  if (Dot(p, q) < 0) {
    return (false);
  }
  return (true);
}

bool point_in_segment(Point p, Point q, Point r) {
  // is p contained in the segment qr?
  COORD THRESHOLD = 1E-8;

  if (Dist2(p, q) < THRESHOLD * THRESHOLD ||
      Dist2(p, r) < THRESHOLD * THRESHOLD) {
    return true;
  }
  if (!fake_colinear(q - p, r - p)) {
    return false;
  }
  if (fake_aligned(p - q, r - q) && fake_aligned(p - r, q - r)) {
    return true;
  }
  return false;
}

PointListIter tFace::FindPoint(Point q) {
  // return verts.end() if not found

  COORD THRESHOLD = 1E-8;

  for (PointListIter k = verts.begin(); k != verts.end(); ++k) {
    // see if q is in the seqment
    PointListIter k_next = k;
    ++k_next;
    if (k_next == verts.end()) {
      k_next = verts.begin();
    }
    if (point_in_segment(q, *k, *k_next)) {
      if (Dist2(q, *k_next) < THRESHOLD * THRESHOLD) {
        return (k_next);
      }
      return (k);
    }
  }
  return (verts.end());
}

void PFace::DrawCylinders(my_ostream& output_stream, COORD scale_factor) {
  COORD THRESHOLD = 1E-8;

  if (!tikz_output) {
    return;
  }

  Point q(0.0, 0.0), t;

  q = s->GetOffsetV0();
  q = q * scale_factor;
  q = q + p;

  list<tFace> tFaces;
  tFace tmp;

  tmp.verts.clear();

  for (auto i = s->oedges.begin(); i != s->oedges.end(); ++i) {
    tmp.verts.push_back(q);
    t = (*i).vec_cx();
    t = t * scale_factor;
    q = q + t;
  }

  tFaces.clear();
  tFaces.push_back(tmp);

  std::cout << "Face " << s->id() << endl;
  std::cout << "tmp=";
  tmp.Print(std::cout);

  std::cout << "Segments: ";
  for (auto i = s->segments_to_draw.begin(); i != s->segments_to_draw.end();
       ++i) {
    Point q1 = p + (*i).head * scale_factor;
    Point q2 = p + (*i).tail * scale_factor;

    std::cout << q1 << q2 << " cyl_on_left =" << i->cyl_on_left << "; ";
  }
  std::cout << endl;

  for (auto i = s->segments_to_draw.begin(); i != s->segments_to_draw.end();
       ++i) {
    Point q1 = p + (*i).head * scale_factor;
    Point q2 = p + (*i).tail * scale_factor;

    tFaceListIter j;
    // find tFace containing q1
    PointListIter k1;

    std::cout << "q1=" << q1 << endl;
    for (j = tFaces.begin(); j != tFaces.end(); ++j) {
      k1 = (*j).FindPoint(q1);
      if (k1 != (*j).verts.end()) {
        // found q1
        break;
      }
    }
    if (j == tFaces.end()) {
      ERR_RET("DrawCylinders: Point not found\n");
    }
    PointListIter k2 = (*j).FindPoint(q2);
    if (k2 == (*j).verts.end()) {
      ERR_RET("DrawCylinders: q1 and q2 not in same tFace");
    }
    // swap q1 and q2 if in wrong order
    for (PointListIter m = (*j).verts.begin(); m != (*j).verts.end(); ++m) {
      if (m == k1) {
        break;
      }

      if (m == k2) {
        // swap k1 and k2, q1 and q2
        PointListIter k_tmp = k1;
        k1 = k2;
        k2 = k_tmp;
        Point q_tmp = q1;
        q1 = q2;
        q2 = q_tmp;
      }
    }

    // bisect tface
    tFace ntf[2];
    ntf[0].verts.clear();
    ntf[1].verts.clear();
    int r = 0;
    for (PointListIter m = (*j).verts.begin(); m != (*j).verts.end(); ++m) {
      PointListIter m_next = m;
      ++m_next;
      if (m_next == (*j).verts.end()) {
        m_next = (*j).verts.begin();
      }

      ntf[r].verts.push_back((*m));
      if (m == k1) {
        if (Dist2(q1, *m) > THRESHOLD * THRESHOLD) {
          ntf[r].verts.push_back(q1);
        }
        ntf[1 - r].verts.push_back(q1);
        r = 1 - r;
      }

      if (m == k2) {
        if (Dist2(q2, *m) > THRESHOLD * THRESHOLD) {
          ntf[r].verts.push_back(q2);
        }
        ntf[1 - r].verts.push_back(q2);
        r = 1 - r;
      }
    }
    // add new tfaces and remove old tface
    tFaces.erase(j);
    tFaces.push_back(ntf[0]);
    tFaces.push_back(ntf[1]);

    std::cout << "Finished Segment. Current tFaces:" << endl;

    for (tFaceListIter jj = tFaces.begin(); jj != tFaces.end(); ++jj) {
      jj->Print(std::cout);
    }
  }

  std::cout << "Final tFaces: ";

  for (tFaceListIter j = tFaces.begin(); j != tFaces.end(); ++j) {
    j->Print(std::cout);
  }

  // now draw the  tfaces which belong to the cylinder

  for (auto i = s->segments_to_draw.begin(); i != s->segments_to_draw.end();
       ++i) {
    if (i->cyl_on_left <= 0) {
      continue;
    }
    Point q1 = p + (*i).head * scale_factor;
    Point q2 = p + (*i).tail * scale_factor;

    for (tFaceListIter j = tFaces.begin(); j != tFaces.end(); ++j) {
      int in_cyl = 0;
      for (PointListIter k = j->verts.begin(); k != j->verts.end(); ++k) {
        PointListIter k_next = k;
        ++k_next;
        if (k_next == j->verts.end()) {
          k_next = j->verts.begin();
        }
        if (Dist2(*k, q1) < THRESHOLD * THRESHOLD &&
            Dist2(*k_next, q2) < THRESHOLD * THRESHOLD) {
          // found cyl
          in_cyl = i->cyl_on_left;
          // draw face using color in_cyl
          j->Draw(output_stream, in_cyl);
        }
      }
    }
  }
}

int UEdge::get_priority() {
  /* later will change this to have a priority field in the uedge */

  if (from_triang) {
    return (1);
  } else if (internal) {
    return (2);
  } else if (v0->euclidean && v1->euclidean) {
    return (3);
  } else if (v0->euclidean || v1->euclidean) {
    return (4);
  } else {
    return (5);
  }
}

bool Face::intersects(Point pb, Point pv) {
  /* measures if the face intersects the segment */
  /* face is drawn with center of mass at 0 */

  /* intersect segment interior should return false in the case of
           colinear edges */

  Point intersection;

  for (auto k = oedges.begin(); k != oedges.end(); ++k) {
    if (colinear(pv, (*k).vec_cx())) {
      //	    std::cout << "Parallel: " << pb <<"--" << pb+pv <<
      //		(*k).headOffset<Point>() << "--" <<
      //(*k).headOffset<Point>() +
      //		(*k).vec_cx() << "\n";

      Point p_ort = pb - Dot(pb, pv) * pv / norm(pv);
      Point e_ort = (*k).headOffset<Point>() -
                    Dot((*k).headOffset<Point>(), pv) * pv / norm(pv);

      //	    std::cout << "p_ort: " << p_ort << " e_ort " << e_ort;

      if (abs(p_ort - e_ort) > 5 * EPSILON) {
        //		fprintf(out_f,"...Not colinear\n");
        continue;
      }
      //	    fprintf(out_f,"...Colinear");

      COORD p_start = Dot(pb, pv) / abs(pv);
      COORD p_end = Dot(pb + pv, pv) / abs(pv);
      COORD e_start = Dot((*k).headOffset<Point>(), pv) / abs(pv);
      COORD e_end = Dot((*k).headOffset<Point>() + (*k).vec_cx(), pv) / abs(pv);

      if (e_start > e_end) {
        COORD tmp = e_end;
        e_end = e_start;
        e_start = tmp;
      }
      if (e_start > p_end - 5 * EPSILON || e_end < p_start + 5 * EPSILON) {
        //		fprintf(out_f,"...Not overlapping\n");
        continue;
      }
      //	    fprintf(out_f,"...Overlapping\n");
      return (true);
    }

    if (intersect_segment_interior(pb, pv, (*k).headOffset<Point>(),
                                   (*k).vec_cx(), intersection)) {
      // std::cout << "Intersecting segments" << pb <<  pb+pv <<
      // 	(*k).headOffset<Point>() <<
      // 	(*k).headOffset<Point>() + (*k).vec_cx() << "\n";

      return (true);
    }
  }
  if (contains(pb + Point(0.5) * pv)) {
    //	std::cout << "contains" << pb+0.5*pv << "\n";
    return (true);
  }
  return (false);
}

bool Face::intersects(Face* f, Point offset_) {
  //    fprintf(out_f,"Face F%d: ",this->id());
  //    for ( OEdgeIter k = oedges.begin(); k!= oedges.end(); k++ ) {
  //	std::cout << (*k).headOffset<Point>() << " ";
  //    }
  //    fprintf(out_f,"\n");

  //    fprintf(out_f,"Face F%d: ",f->id());
  //    for ( OEdgeIter k = f->oedges.begin(); k!= f->oedges.end() ; k++ ) {
  //	std::cout << (*k).headOffset<Point>() +offset;
  //    }
  //    fprintf(out_f,"\n");

  for (auto k = f->oedges.begin(); k != f->oedges.end(); ++k) {
    if ((*k).other_face() == this) {
      continue;
    }
    if (this->intersects((*k).headOffset<Point>() + offset_, (*k).vec_cx())) {
      //	    fprintf(out_f,"Intersects: F%d F%d because of E%d",
      //		   this->id(), f->id(), (*k).id());
      //	    std::cout << " Offset: " << offset;
      //	    std::cout << " Edge :" << (*k).headOffset<Point>()+offset <<
      //(*k).vec_cx() << endl;
      return (true);
    }
  }
  if (this->contains(offset_ + f->barycenter())) {
    //	fprintf(out_f,"F%d contains F%d", this->id(), f->id());
    //	std::cout << " Offset " << offset <<"\n";
    return (true);
  }
  if (f->contains(-offset_ + this->barycenter())) {
    //	fprintf(out_f,"F%d contains F%d", f->id(), this->id());
    //	std::cout << " (-)Offset " << offset <<"\n";
    return (true);
  }
  //    fprintf(out_f,"Do not Intersect: F%d F%d", this->id(), f->id());
  //    std::cout << " Offset " << offset << "\n";
  return (false);
}

bool Face::contains(Point p) {
  Point intersection;

  /* hack */
  Point far_out_vector =
      1000.0 * (this->perimeter()) * Point(0.34344, 0.565667);

  int count = 0;

  for (auto k = oedges.begin(); k != oedges.end(); ++k) {
    if (intersect_segment_interior(p, far_out_vector, (*k).headOffset<Point>(),
                                   (*k).vec_cx(), intersection)) {
      count++;
    }
  }
  //    fprintf(out_f,"V%d V%d: count = %d\n",v1->id(), v2->id(), count);

  if (count & 1) {
    return (true);
  }
  return (false);
}

COORD Face::perimeter() {
  COORD per = 0;
  Point s = Point(0, 0);

  for (auto k = oedges.begin(); k != oedges.end(); ++k) {
    s += (*k).vec_cx();
    per += abs(s);
  }
  return (per);
}

PFace* TwoComplex::get_pface(Face* f) {
  for (auto k = dl.begin(); k != dl.end(); ++k) {
    if ((*k)->sp()->tag == 'F' && (*k)->sp() == f) {
      return static_cast<PFace*>(*k);
    }
  }
  ERR_RET("get_pface: not found");
}

bool TwoComplex::can_merge(UEdge* ue) {
  if (ue->deleted()) {
    return (false);
  }

  if (ue->boundary()) {
    return (false);
  }

  PFace* pf0 = get_pface(ue->f0);
  PFace* pf1 = get_pface(ue->f1);
  auto oe0 = ue->this_edge(ue->f0); /* ue in f0 */
  auto oe1 = ue->this_edge(ue->f1); /* ue in f1 */

  if (pf0->in_pcomplex == pf1->in_pcomplex) { /* already merged */
    return (false);
  }
  // fprintf(out_f,"Trying to merge along E%d\n", ue->id());
  // fprintf(out_f,"F%d (F%d) in pcomplex %d, F%d (F%d)in pcomplex %d\n",
  // 	   ue->f0->id(), pf0->s->id(), pf0->in_pcomplex,
  // 	   ue->f1->id(), pf1->s->id(), pf1->in_pcomplex);

  Point offset_ =
      pf0->p + oe0->headOffset<Point>() - pf1->p - oe1->tailOffset<Point>();

  for (auto i = dl.begin(); i != dl.end(); ++i) {
    if ((*i)->in_pcomplex != pf0->in_pcomplex) {
      continue;
    }
    for (auto j = dl.begin(); j != dl.end(); ++j) {
      if ((*j)->in_pcomplex != pf1->in_pcomplex) {
        continue;
      }
      /*enough to do face_face */
      if ((*i)->sp()->tag != 'F' || (*j)->sp()->tag != 'F') {
        continue;
      }
      PFace* pfi = static_cast<PFace*>(*i);
      PFace* pfj = static_cast<PFace*>(*j);

      /* don't forget offset */
      if (pfi->s->intersects(pfj->s, offset_ + pfj->p - pfi->p)) {
        //		fprintf(out_f,"can't merge \n");
        return (false);
      }
    }
  }
  //    fprintf(out_f,"can merge \n");
  return (true);
}

void TwoComplex::relocate(size_t i, Point offset_) {
  for (auto k = dl.begin(); k != dl.end(); ++k) {
    if ((*k)->in_pcomplex == i) {
      (*k)->p += offset_;
    }
  }
}

void TwoComplex::merge(UEdge* ue) {
  /* already assumes can merge */

  PFace* pf0 = get_pface(ue->f0);
  PFace* pf1 = get_pface(ue->f1);
  auto oe0 = ue->this_edge(ue->f0); /* ue in f0 */
  auto oe1 = ue->this_edge(ue->f1); /* ue in f1 */

  size_t i = pf0->in_pcomplex;
  size_t j = pf1->in_pcomplex;

  Point offset_ =
      pf0->p + oe0->headOffset<Point>() - pf1->p - oe1->tailOffset<Point>();

  relocate(j, offset_);

  for (auto k = dl.begin(); k != dl.end(); ++k) {
    if ((*k)->in_pcomplex == j) {
      (*k)->in_pcomplex = i;
    }
  }
}

bool is_longer(UEdge* a, UEdge* b) { return (a->len() > b->len()); }

void TwoComplex::make_pface(Face* f) {
  dl.clear();

  AddPFace(f, Point(0, 0), 0);
}

void TwoComplex::make_pcomplexes() {
  size_t count = 0;
  int pr;

  dl.clear();

  for (auto i = faces.begin(); i != faces.end(); ++i) {
    if (!(*i)->deleted()) {
      AddPFace((*i), Point(0.0), count++);
    }
  }

  list<UEdge*> candidates;

  /* lower priority better */

  for (pr = 0; pr <= max_priority; pr++) {
    candidates.clear();

    for (auto j = uedges.begin(); j != uedges.end(); ++j) {
      if ((*j)->get_priority() != pr) {
        continue;
      }
      candidates.push_back(*j);
    }
    candidates.sort(is_longer);

    for (auto j = candidates.begin(); j != candidates.end(); ++j) {
      if (can_merge(*j)) {
        merge(*j);
      }
    }
    //	fprintf(out_f,"finished priority %d\n",pr);
    /*FINISHED MERGING PRIORITY pr */
  }
}

void TwoComplex::NewDraw(my_ostream& output_stream) {
  COORD global_max_x = 0;
  COORD global_min_x = 0;
  COORD global_max_y = 0;
  COORD global_min_y = 0;
  COORD max_x = 0;
  COORD max_y = 0;
  COORD min_x = 0;
  COORD min_y = 0;
  bool had_pcomplex = false;

#define SPACING 0.5
#define DIM 16.0
  /* coordinates will go from -DIM to DIM */

  COORD scale_factor_;
  Point mean;
  Point tmp;

  if (dl.empty()) {
    ERR_RET("twocompex::draw: empty draw list");
  }

  for (size_t pc = 0; pc < nfaces(); pc++) {
    bool active = false;

    for (auto i = dl.begin(); i != dl.end(); ++i) {
      if ((*i)->sp()->tag == 'V' && (*i)->in_pcomplex == pc) {
        if (!active) {
          active = true;
          max_x = min_x = (*i)->p.real();
          max_y = min_y = (*i)->p.imag();
        } else {
          if ((*i)->p.real() > max_x) max_x = (*i)->p.real();
          if ((*i)->p.real() < min_x) min_x = (*i)->p.real();
          if ((*i)->p.imag() > max_y) max_y = (*i)->p.imag();
          if ((*i)->p.imag() < min_y) min_y = (*i)->p.imag();
        }
      }
    }
    if (active && (!had_pcomplex)) {
      /* first pcomplex */
      global_max_x = max_x;
      global_min_x = min_x;
      global_max_y = max_y;
      global_min_y = min_y;
      had_pcomplex = true;
    } else if (active) {
      relocate(pc,
               Point(global_max_x - min_x + SPACING * (max_x - min_x), 0.0));
      //	    relocate(pc, Point(global_max_x - min_x
      //+SPACING*(max_x-min_x) 			       ,global_max_y - min_y
      //+SPACING*(max_y-min_y)));
      global_max_x += (max_x - min_x) + SPACING * (max_x - min_x);
      //	    global_max_y += (max_y - min_y)+SPACING*(max_y-min_y);

      if (max_y > global_max_y) {
        global_max_y = max_y;
      }
      if (min_y < global_min_y) {
        global_min_y = min_y;
      }
    }
  }
  /* now finally make the draw list */

  COORD SCALE_HACK = 1;

  if (global_max_x - global_min_x > global_max_y - global_min_y)
    scale_factor_ = SCALE_HACK * DIM * 2.0 / (global_max_x - global_min_x);
  else
    scale_factor_ = SCALE_HACK * DIM * 2.0 / (global_max_y - global_min_y);

  mean = Point((global_max_x + global_min_x) / 2,
               (global_max_y + global_min_y) / 2);

  //    fprintf(out_f,"%lf %lf %lf %lf %lf \n",global_min_x, global_max_x,
  //                         global_min_y, global_max_y, scale_factor);

  for (auto i = dl.begin(); i != dl.end(); ++i) {
    tmp = (*i)->p - mean;
    (*i)->p = tmp * scale_factor_;
  }

  for (auto i = dl.begin(); i != dl.end(); ++i) {
    (*i)->Draw(output_stream, scale_factor_);
  }
}
}  // namespace polygon
