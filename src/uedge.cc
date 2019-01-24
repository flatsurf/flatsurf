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

#include "libpolygon/two_complex.h"

// UEdge::UEdge()
// {
//    tag = 'E';
//    v0 = v1 = NULL;
//    f0 = f1  = NULL;

// };

UEdge::UEdge(VertexPtr a, VertexPtr b, BigPointQ vec) : Simplex() {
  tag = 'E';
  v0 = a;
  v1 = b;
  f0 = f1 = NULL;
  ue_vecQ = vec;
  internal = false;
  from_triang = false;

  vec.Check();
};

UEdge::UEdge(VertexPtr a, VertexPtr b, Point vec_cx) : Simplex() {
  if (field_arithmetic) {
    ERR_RET("UEdge constructor called without alg part");
  }
  tag = 'E';
  v0 = a;
  v1 = b;
  f0 = f1 = NULL;
  ue_vecQ.cx = vec_cx;
  //    ue_vecQ_algt = algt_vec;
  internal = false;
  from_triang = false;
};

bool UEdge::deleted() {
  if (f0 == NULL && f1 == NULL) {
    return (true);
  }
  return (false);
};

void UEdge::Delete() {
  /* warning: does not take care of case where some vertices e ptr */
  /* points at this edge */

  if (deleted()) return;

  f0 = NULL;
  f1 = NULL;
}

bool UEdge::boundary() {
  if (deleted()) return (false);

  if (f0 == NULL || f1 == NULL) {
    return (true);
  }
  return (false);
};

COORD UEdge::len() { return (abs(ue_vecQ.cx)); };

void UEdge::Print(ostream& out) {
  Simplex::Print(out);
  out << " ( ";
  if (v0 != NULL) {
    v0->Print(out);
  } else {
    out << "NULL";
  };

  out << " ";
  if (v1 != NULL) {
    v1->Print(out);
  } else {
    out << "NULL";
  };

  out << " ";
  if (f0 != NULL) {
    f0->Simplex::Print(out);
  } else {
    out << "NULL";
  };

  out << " ";
  if (f1 != NULL) {
    f1->Simplex::Print(out);
  } else {
    out << "NULL";
  };
  out << " ) ";

  //    out_s << ue_vecQ;

#ifdef USE_QUAD
  out << "(" << ue_vecQ.cx.real().str(24) << "," << ue_vecQ.cx.imag().str(24)
      << ")";
#elif defined USE_LONG_DOUBLE
  char tmp[1000];
  sprintf(tmp, "(%.24Lf,%.24Lf)", ue_vecQ.cx.real(), ue_vecQ.cx.imag());
  out << tmp;
#else
  char tmp[1000];
  sprintf(tmp, "(%.24f,%.24f)", ue_vecQ.cx.real(), ue_vecQ.cx.imag());
  out << tmp;
#endif
};

FacePtr UEdge::boundary_face() {
  if (!(this->boundary())) ERR_RET("boundary_face: not boundary");

  if (f0 == NULL) return (f1);

  if (f1 == NULL) return (f0);

  ERR_RET("boundary_face:deleted edge");
  return (NULL);
}

OEdgeIter UEdge::boundary_edge() {
  FacePtr f;

  f = boundary_face();
  return (this_edge(f));
}

OEdgeIter UEdge::this_edge(FacePtr f) {
  for (OEdgeIter i = f->oedges.begin(); i != f->oedges.end(); ++i) {
    if ((*i).ue == this) return (i);
  }

  ERR_RET("this_edge: edge not found");
  return (NULL_OEdgeIter);
}

void UEdge::set_null_face(FacePtr f) {
  if (!(this->boundary())) ERR_RET("set_null_face: not boundary edge");

  if (f0 == NULL) {
    f0 = f;
  } else if (f1 == NULL) {
    f1 = f;
  } else
    ERR_RET("set_null_face: deleted edge");
}
