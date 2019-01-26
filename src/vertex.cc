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

#include <iostream>
#include <list>
#include <ostream>
#include <string>
#include <boost/math/special_functions/round.hpp>
#include <boost/math/constants/constants.hpp>

#include "libpolygon/elementary_geometry.h"
#include "libpolygon/globals.h"
#include "libpolygon/simplex.h"
#include "libpolygon/two_complex.h"
#include "libpolygon/vertex.h"
#include "libpolygon/shared.h"

using std::abs;
using std::endl;
using std::list;
using std::ostream;
using std::string;
using boost::math::iround;
using boost::math::constants::pi;

namespace polygon {
Simplex::Simplex() : color("#") {
  int i;
  char tmp[10];
  string tmp2;
  visit_id = -1;
  ID = -1;
  tag = 0;

  for (i = 0; i < 12; i++) {
    sprintf(tmp, "%x", std::uniform_int_distribution<int>(0,15)(random_engine));
    tmp2 = tmp;
    color += tmp2;
  }
}

int Simplex::id() { return ID; }

void Simplex::Print(ostream& out) {
  //    fprintf(out_f,"%c%d ",tag,id());
  out << tag << id();
}

//
//    simplex(color_t c) {
//	id = 0;
//	visit_id = 0;
//	color = c;

Vertex::Vertex() : Simplex() {
  tag = 'V';
  order = 0;
  euclidean = UNDEFINED;
  e = 0;
  int_angle = -1;
}

bool Vertex::deleted() {
  if (order == 0) return (true);

  return (false);
}

void Vertex::Delete() { order = 0; }

void Vertex::Check() {
  list<list<OEdge>::iterator>::iterator i, j;

  if (deleted()) return;

  if (out_edges.size() != order) {
    std::cout << "V" << ID << ":"
              << "out_edges.size = " << out_edges.size() << " order = " << order
              << endl;
    ERR_RET("vertex_check:vertex order does not match");
  }
  Point q = Point(0, 0);

  for (i = out_edges.begin(); i != out_edges.end(); ++i) {
    if ((*i)->head() != this) {
      ERR_RET("vertex check: vertex not outgoing");
    }
    //	/* assume triangles */
    q += (*i)->next_edge()->vec_cx();

    //	q = q -(*i)->prev_edge()->vecQ() - (*i)->vecQ();

    j = next_vert_edge(i);

    if (!CCW((*i)->vec_cx(), (*j)->vec_cx())) {
      ERR_RET("vertex check: edges not CCW");
    }
  }
  //    std::cout << "V" << id() << ": Total vertex vec: " << q << "\n";
  if (abs(q) > EPSILON) {
    ERR_RET("vertex check: star of vertex does not add up");
  }
}

COORD Vertex::total_angle() {
  list<list<OEdge>::iterator>::iterator i, j;
  COORD s = 0;

  if (deleted()) return (0);

  j = i = out_edges.begin();
  ++j;

  while (j != out_edges.end()) {
    s = s + angle((*i)->vec_cx(), (*j)->vec_cx());

    ++i;
    ++j;
  }

  s = s + angle(out_edges.back()->vec_cx(), out_edges.front()->vec_cx());

  return (s);
}

int Vertex::total_angle_over_pi() {
  COORD a = total_angle();

	int j = iround(a / pi<COORD>());

  if (abs(j * pi<COORD>() - a) > 10 * EPSILON) {
    std::cerr << abs(j * pi<COORD>() - a) << endl;
    ERR_RET("total_angle_over_pi: bad angle");
  }
  return j;
}
}  // namespace polygon
