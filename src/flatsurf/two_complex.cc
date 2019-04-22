/**********************************************************************
 *  This file is part of flatsurf.
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
 *  along with flatsurf. If not, see <https://www.gnu.org/licenses/>.
 *********************************************************************/

#include <boost/math/constants/constants.hpp>
#include <list>
#include <ostream>
#include <vector>
#include "exact-real/element.hpp"
#include "exact-real/number_field_traits.hpp"
#include "external/boolinq/include/boolinq/boolinq.h"

#include "./elementary_geometry.h"
#include "./globals.h"
#include "./number_field.h"
#include "./params.h"
#include "./pface.h"
#include "./puedge.h"
#include "./pvertex.h"
#include "./two_complex.h"
#include "./uedge.h"
#include "./vertex.h"

#include "flatsurf/flat_triangulation.hpp"
#include "flatsurf/half_edge.hpp"
#include "flatsurf/half_edge_map.hpp"
#include "flatsurf/permutation.hpp"
#include "flatsurf/vector_eantic.hpp"
#include "flatsurf/vector_exactreal.hpp"

using boolinq::from;
using boost::math::constants::pi;
using exactreal::Element;
using exactreal::NumberFieldTraits;
using flatsurf::FlatTriangulation;
using flatsurf::FlatTriangulationCombinatorial;
using flatsurf::HalfEdge;
using flatsurf::HalfEdgeMap;
using flatsurf::Permutation;
using flatsurf::VectorExactReal;
using std::endl;
using std::flush;
using std::list;
using std::ostream;
using std::vector;

namespace polygon {
TwoComplex::TwoComplex() : area(-1.0), scale_factor(1.0) {
  cur_vertex_id = 0;
  cur_face_id = 0;
  cur_uedge_id = 0;
  VISIT_ID = 0;
}

TwoComplex::~TwoComplex() {
  for (auto d : dl) {
    delete d;
  }
}

TwoComplex::operator FlatTriangulation<
    VectorExactReal<exactreal::NumberFieldTraits>>() const {
  VectorExactReal<NumberFieldTraits> zero{alg_t<bigrat>().real(),
                                          alg_t<bigrat>().imag()};

  HalfEdgeMap<VectorExactReal<NumberFieldTraits>> vectors(
      vector<VectorExactReal<NumberFieldTraits>>(uedges.size(), zero));

  for (auto uedge : uedges) {
    auto oedge = OEdge(uedge, 1);
    auto x = oedge.vec_algt().real();
    auto y = oedge.vec_algt().imag();
    vectors.set(static_cast<HalfEdge>(oedge), {x, y});
  }

  return {static_cast<FlatTriangulationCombinatorial>(*this), vectors};
}

TwoComplex::operator FlatTriangulationCombinatorial() const {
  auto permutation = Permutation<HalfEdge>(
      from(vertices)
          .select([](auto v) {
            return from(v->out_edges)
                .select([](auto e) { return static_cast<HalfEdge>(*e); })
                .toVector();
          })
          .toVector());

  return {permutation};
}

size_t TwoComplex::nedges() { return (uedges.size()); }

size_t TwoComplex::nfaces() { return (faces.size()); }

size_t TwoComplex::nvertices() { return (vertices.size()); }

void TwoComplex::PrintAll(ostream &out) {
  if (field_arithmetic) {
    NumberField<bigrat>::F->print(out);
    Params::print(out);
  }

  std::list<Vertex *>::iterator i;

  out << "# Vertices:";
  for (i = vertices.begin(); i != vertices.end(); ++i) {
    out << " ";
    (*i)->Print(out);
  }
  out << endl;

  out << "# UEdges: " << endl;
  for (auto j = uedges.begin(); j != uedges.end(); ++j) {
    (*j)->Print(out);
    out << endl;
  }

  out << "# Faces: " << endl;
  for (auto k = faces.begin(); k != faces.end(); ++k) {
    if ((*k)->deleted()) {
      out << "#";
    }
    (*k)->Print(out);
    out << endl;
  }

  if (field_arithmetic) {
    out << "#UEdges Algebraic " << endl;
    for (auto j = uedges.begin(); j != uedges.end(); ++j) {
      out << "#E" << (*j)->id() << ": " << (*j)->ue_vecQ.algt;
      out << endl;
    }
  }
}

void TwoComplex::CheckAllFaces() {
  for (auto i = faces.begin(); i != faces.end(); ++i) {
    (*i)->Check();
  }
}

void TwoComplex::CheckAllVertices() {
  for (auto i = vertices.begin(); i != vertices.end(); ++i) {
    (*i)->Check();
  }
}

Vertex *TwoComplex::AddVertex(int id) {
  Vertex *v = new Vertex;
  if (id >= 0) {
    v->ID = id;
    if (id >= cur_vertex_id) {
      cur_vertex_id = id + 1;
    }
  } else {
    v->ID = cur_vertex_id++;
  }
  vertices.insert(vertices.end(), v);
  return (v);
}

UEdge *TwoComplex::AddUEdge(int id, Vertex *v0, Vertex *v1, Point vec) {
  UEdge *ue = new UEdge(v0, v1, vec);
  if (id < 0) {
    ue->ID = cur_uedge_id++;
  } else {
    ue->ID = id;
    if (id >= cur_uedge_id) {
      cur_uedge_id = id + 1;
    }
  }
  ue->v0->order++;
  ue->v0->e = ue;

  ue->v1->order++;
  ue->v1->e = ue;

  uedges.insert(uedges.end(), ue);
  return (ue);
}

UEdge *TwoComplex::AddUEdge(int id, Vertex *v0, Vertex *v1, BigPointQ vec) {
  UEdge *ue = new UEdge(v0, v1, vec);
  if (id < 0) {
    ue->ID = cur_uedge_id++;
  } else {
    ue->ID = id;
    if (id >= cur_uedge_id) {
      cur_uedge_id = id + 1;
    }
  }
  ue->v0->order++;
  ue->v0->e = ue;

  ue->v1->order++;
  ue->v1->e = ue;

  uedges.insert(uedges.end(), ue);
  return (ue);
}

Face *TwoComplex::AddFace(int id, list<OEdge> L) {
  Face *f = new Face(L);
  if (id >= 0) {
    f->ID = id;
    if (id >= cur_face_id) {
      cur_face_id = id + 1;
    }
  } else {
    f->ID = cur_face_id++;
  }
  faces.insert(faces.end(), f);
  return (f);
}

void TwoComplex::StatPrint(ostream &out) {
  out << "# File = " << filename_ << " perturb = " << perturb_magnitude
      << " rescale = " << !norescale << endl;

  out << "# vertices: " << nvertices() << endl;
  out << "## ";
  for (auto i = vertices.begin(); i != vertices.end(); ++i) {
    (*i)->Print(out);
    out << "(" << (*i)->order << ") (" << (*i)->total_angle() / pi<COORD>()
        << " PI) ";
  }
  out << endl;

  out << "# edges: " << nedges() << endl;
  out << "## ";
  for (auto i = uedges.begin(); i != uedges.end(); ++i) {
    (*i)->Simplex::Print(out);
    if ((*i)->deleted())
      out << "(0) ";
    else if ((*i)->boundary())
      out << "(1) ";
    else
      out << "(2) ";
  }
  out << endl;

  out << "# faces: " << nfaces() << endl;
  out << "# genus: " << (2 - nvertices() + nedges() - nfaces()) / 2 << endl;
  out << "# area: " << get_area() << endl;
}

void TwoComplex::AddPFace(Face *f, Point q, size_t pcomplex) {
  Point r, t;
  PFace *sp;

  sp = new PFace(f, q, pcomplex);
  dl.insert(dl.end(), sp);

  r = sp->s->GetOffsetV0();
  r = r + q;

  for (auto i = f->oedges.begin(); i != f->oedges.end(); ++i) {
    PVertex *vp;
    vp = new PVertex((*i).head(), r, pcomplex);
    dl.insert(dl.end(), vp);

    PUEdge *ep;
    ep = new PUEdge((*i), r, pcomplex);
    dl.insert(dl.end(), ep);

    t = (*i).vec_cx();
    r = r + t;
  }
}

void TwoComplex::MakeDrawListInternal(Face *f, Point q) {
  list<OEdge>::iterator i, j;
  Face *candidate;
  Vertex *v;
  Point t;

  if (f->deleted()) return;

  AddPFace(f, q, 0);
  f->visit_id = VISIT_ID;

  for (i = f->oedges.begin(); i != f->oedges.end(); ++i) {
    if ((*i).ue->boundary()) continue;

    candidate = (*i).other_face();
    if (candidate->visit_id == VISIT_ID) continue;

    j = (*i).pair_edge();

    if (j == NULL_OEdgeIter) ERR_RET("draw_list_internal: no pair edge");

    v = (*i).head();

    if (v != (*j).tail()) {
      ERR_RET("drawlist_internal:edges don't match");
    }
    // q + offset(v) = q' + offset(v')
    // hence q' = q + offset(v) - offset(v')

    t = q + (*i).headOffset<Point>() - (*i).pair_edge()->tailOffset<Point>();

    MakeDrawListInternal(candidate, t);
  }
  return;
}

void TwoComplex::ClearSegmentsToDraw() {
  for (auto i = faces.begin(); i != faces.end(); ++i) {
    (*i)->ClearSegmentsToDraw();
  }
}

void TwoComplex::Expunge() {
  bool did_something;

  do {
    did_something = false;
    for (auto i = vertices.begin(); i != vertices.end(); ++i) {
      if ((*i)->deleted()) {
        delete *i;
        vertices.erase(i);
        did_something = true;
        break;
      }
    }
  } while (did_something);

  do {
    did_something = false;
    for (auto i = uedges.begin(); i != uedges.end(); ++i) {
      if ((*i)->deleted()) {
        delete *i;
        uedges.erase(i);
        did_something = true;
        break;
      }
    }
  } while (did_something);

  do {
    did_something = false;
    for (auto i = faces.begin(); i != faces.end(); ++i) {
      if ((*i)->deleted()) {
        delete *i;
        faces.erase(i);
        did_something = true;
        break;
      }
    }
  } while (did_something);
}

void TwoComplex::BuildNeighborLists() {
  list<OEdge>::iterator ep, ep_first;

  for (auto i = vertices.begin(); i != vertices.end(); ++i) {
    if ((*i)->deleted()) {
      continue;
    }

    (*i)->out_edges.clear();

    ep = (*i)->e->this_edge((*i)->e->f0);

    if (ep->tail() == (*i)) ep = ep->pair_edge();

    if (ep->head() != (*i))
      ERR_RET("neighborlist: e pointer not adjacent to vertex");

    if (ep->deleted()) {
      ERR_RET("BuildNeighborLists: deleted ->e\n");
    }

    ep_first = ep;

    do {
      (*i)->out_edges.insert((*i)->out_edges.begin(), ep);
      ep = ep->pair_edge()->next_edge();

    } while (ep != ep_first);

    (*i)->int_angle = (*i)->total_angle_over_pi();

    if ((*i)->int_angle == 2) {
      (*i)->euclidean = true;
    } else {
      (*i)->euclidean = false;
    }
  }

  return;
}

void TwoComplex::StoreVertexOffsets() {
  Face *f;

  for (auto i = faces.begin(); i != faces.end(); ++i) {
    f = (*i);
    if (f->deleted()) continue;

    for (auto j = f->oedges.begin(); j != f->oedges.end(); ++j) {
      (*j).next_e = (*j).next_edge();
      (*j).pair_e = (*j).pair_edge();

      (*j).head_offst = (*j).headOffset<Point>();
      (*j).head_offstI = (*j).headOffset<BigPointI>();

      (*j).tail_offst = (*j).tailOffset<Point>();
      (*j).tail_offstI = (*j).tailOffset<BigPointI>();
    }
  }
  for (auto i = uedges.begin(); i != uedges.end(); ++i) {
    (*i)->minus_ue_vecQ = -(*i)->ue_vecQ;
    (*i)->minus_ue_vecI = -(*i)->ue_vecI;
  }
  return;
}

void TwoComplex::set_area() {
  COORD s = 0.0;
  for (auto i = faces.begin(); i != faces.end(); ++i) {
    s = s + (*i)->volume();
  }
  area = s;
}

COORD TwoComplex::get_area() {
  if (area > 0) {
    return (area);
  } else {
    ERR_RET("TwoComplex::get_area called before area was set");
  }
}

COORD TwoComplex::get_scale_factor() { return scale_factor; }

void TwoComplex::set_scale_factor(COORD scale) { scale_factor = scale; }

UEdge *TwoComplex::MaxEdge() {
  UEdge *longest_edge = nullptr;
  COORD longest_length = 0;

  for (auto i = uedges.begin(); i != uedges.end(); ++i) {
    if ((*i)->len() > longest_length) {
      longest_edge = (*i);
      longest_length = (*i)->len();
    }
  }
  return (longest_edge);
}

COORD TwoComplex::MinSaddle(Dir<Point> &the_shortest) {
  Dir<Point> old_dir, new_dir, tmp_dir;

  COORD depth_ = MaxEdge()->len() + 0.1;
  COORD shortest = depth_;

  for (auto i = vertices.begin(); i != vertices.end(); ++i) {
    Vertex *v0;
    v0 = (*i);
    old_dir = Dir<Point>(v0, Point(1.12, 1.3));
    COORD TotalAngle;
    TotalAngle = 0;

    while (TotalAngle < v0->total_angle()) {
      SweepNextLeft(old_dir, new_dir, depth_ * depth_, 10);
      TotalAngle += true_angle(old_dir.vec, -new_dir.vec);

      old_dir = old_dir.RotateCCwToVec(-new_dir.vec);

      /* check for non-euclideanness ? */

      if (abs(old_dir.vec) < shortest) {
        shortest = abs(old_dir.vec);
        the_shortest = old_dir;
      }
    }
  }

  return (shortest);
}

void TwoComplex::issueFinalReport(Summary &fsm, ostream &out, int start_vertex,
                                  double part_done, double part_group) {
  out << "File = " << filename_ << " depth = " << depth
      << " follow_depth = " << follow_depth
      << " perturb = " << perturb_magnitude << endl;

  out << "Final Report:\n";
  fsm.print(out, part_done, part_group, get_area(), depth, start_vertex);
  out << flush;
}

void TwoComplex::check_algebraicQ() {
  for (auto i = uedges.begin(); i != uedges.end(); ++i) {
    if (abs((**i).ue_vecQ.cx - (**i).ue_vecQ.algt.tocomplex()) > EPSILON)
      ERR_RET("ue_vecQ_alg doesn't match ue_vecQ");
  }
}

void TwoComplex::check_algebraicI() {
  for (auto i = uedges.begin(); i != uedges.end(); ++i) {
    if (abs((**i).ue_vecI.cx - (**i).ue_vecI.algt.tocomplex()) > EPSILON)
      ERR_RET("ue_vecI_alg doesn't match ue_vecI");
  }
}

int64_t TwoComplex::get_global_denom() {
  mpz_class d(1);

  for (auto i = uedges.begin(); i != uedges.end(); ++i) {
    for (size_t j = 0; j <= Params::nbr_params(); j++) {
      algebraic<bigrat> a = (*i)->ue_vecQ.algt.get_coeff(j);
      vector<bigrat> c = a.get_coords();
      for (unsigned int k = 0; k < c.size(); k++) {
        mpz_class t = c[k].get_den();
        if (!mpz_divisible_p(d.get_mpz_t(), t.get_mpz_t())) {
          d *= t;  // d should really be lcm(d,t)
        }
      }
    }
  }
  std::cout << "Clearing denominators: multiplying by " << d << endl;

  if (!d.fits_sint_p()) {
    ERR_RET("Denominator too large");
  }
  return static_cast<int>(d.get_si());
}

alg_tI convert_to_algtI(alg_tQ p) {
  vector<algebraicI> new_coeffs;
  new_coeffs.clear();
  for (size_t i = 0; i <= Params::nbr_params(); i++) {
    vector<int64_t> new_coords;
    new_coords.clear();
    algebraicQ a = p.get_coeff(i);
    vector<bigrat> c = a.get_coords();

    for (unsigned int j = 0; j < c.size(); j++) {
      //`	    bigrat r = c[j];
      assert(c[j].get_den() == mpz_class(1));
      mpz_class tmp = c[j].get_num();
      if (!tmp.fits_slong_p()) {
        ERR_RET("cannot convert alg_tQ to alg_tI");
      }

      new_coords.push_back(tmp.get_si());
    }
    new_coeffs.push_back(algebraicI(new_coords, NumberField<int64_t>::F));
  }
  return (alg_tI(new_coeffs));
}

void TwoComplex::ClearDenominators() {
  int64_t d = get_global_denom();

  for (auto i = uedges.begin(); i != uedges.end(); ++i) {
    (*i)->ue_vecQ *= boost::numeric_cast<int>(d);
    (*i)->ue_vecI.cx = (*i)->ue_vecQ.cx;

    (*i)->ue_vecI.algt = convert_to_algtI((*i)->ue_vecQ.algt);
  }
  set_scale_factor(static_cast<double>(d) * get_scale_factor());
}
}  // namespace polygon
