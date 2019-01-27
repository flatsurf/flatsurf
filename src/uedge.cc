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

#include "libpolygon/big_point.h"
#include "libpolygon/globals.h"
#include "libpolygon/uedge.h"
#include "libpolygon/vertex.h"

using std::list;
using std::ostream;

namespace polygon {
UEdge::UEdge(Vertex* a, Vertex* b, BigPointQ vec) : Simplex(), ue_vecQ(vec) {
  tag = 'E';
  v0 = a;
  v1 = b;
  f0 = f1 = nullptr;
  internal = false;
  from_triang = false;

  vec.Check();
}

UEdge::UEdge(Vertex* a, Vertex* b, Point vec_cx) : Simplex() {
  if (field_arithmetic) {
    ERR_RET("UEdge constructor called without alg part");
  }
  tag = 'E';
  v0 = a;
  v1 = b;
  f0 = f1 = nullptr;
  ue_vecQ.cx = vec_cx;
  //    ue_vecQ_algt = algt_vec;
  internal = false;
  from_triang = false;
}

bool UEdge::deleted() {
  if (f0 == nullptr && f1 == nullptr) {
    return (true);
  }
  return (false);
}

void UEdge::Delete() {
  /* warning: does not take care of case where some vertices e ptr */
  /* points at this edge */

  if (deleted()) return;

  f0 = nullptr;
  f1 = nullptr;
}

bool UEdge::boundary() {
  if (deleted()) return (false);

  if (f0 == nullptr || f1 == nullptr) {
    return (true);
  }
  return (false);
}

COORD UEdge::len() { return (abs(ue_vecQ.cx)); }

void UEdge::Print(ostream& out) {
  Simplex::Print(out);
  out << " ( ";
  if (v0 != nullptr) {
    v0->Print(out);
  } else {
    out << "NULL";
  };

  out << " ";
  if (v1 != nullptr) {
    v1->Print(out);
  } else {
    out << "NULL";
  };

  out << " ";
  if (f0 != nullptr) {
    f0->Simplex::Print(out);
  } else {
    out << "NULL";
  };

  out << " ";
  if (f1 != nullptr) {
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
}

Face* UEdge::boundary_face() {
  if (!(this->boundary())) ERR_RET("boundary_face: not boundary");

  if (f0 == nullptr) return (f1);

  if (f1 == nullptr) return (f0);

  ERR_RET("boundary_face:deleted edge");
}

list<OEdge>::iterator UEdge::boundary_edge() {
  Face* f;

  f = boundary_face();
  return (this_edge(f));
}

list<OEdge>::iterator UEdge::this_edge(Face* f) {
  for (auto i = f->oedges.begin(); i != f->oedges.end(); ++i) {
    if ((*i).ue == this) return (i);
  }

  ERR_RET("this_edge: edge not found");
}

void UEdge::set_null_face(Face* f) {
  if (!(this->boundary())) ERR_RET("set_null_face: not boundary edge");

  if (f0 == nullptr) {
    f0 = f;
  } else if (f1 == nullptr) {
    f1 = f;
  } else
    ERR_RET("set_null_face: deleted edge");
}
}  // namespace polygon
