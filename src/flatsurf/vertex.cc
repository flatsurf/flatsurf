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
#include <boost/math/special_functions/round.hpp>
#include <iostream>
#include <list>
#include <ostream>
#include <string>

#include "./elementary_geometry.h"
#include "./globals.h"
#include "./shared.h"
#include "./simplex.h"
#include "./two_complex.h"
#include "./vertex.h"
#include "flatsurf/flat_triangulation_combinatorial.hpp"
#include "flatsurf/vertex.hpp"

using boost::math::constants::pi;
using boost::math::iround;
using std::abs;
using std::endl;
using std::list;
using std::ostream;
using std::string;

namespace polygon {
Simplex::Simplex() : color("#") {
  int i;
  char tmp[10];
  string tmp2;
  visit_id = -1;
  ID = -1;
  tag = 0;

  for (i = 0; i < 12; i++) {
    sprintf(tmp, "%x",
            std::uniform_int_distribution<int>(0, 15)(random_engine));
    tmp2 = tmp;
    color += tmp2;
  }
}

int Simplex::id() const { return ID; }

void Simplex::Print(ostream &out) { out << tag << id(); }

Vertex::Vertex() : Simplex() {
  tag = 'V';
  order = 0;
  euclidean = UNDEFINED;
  e = nullptr;
  int_angle = -1;
}

bool Vertex::relevant() const { return !euclidean; }

bool Vertex::deleted() const {
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

    j = next_vert_edge(i);

    if (!CCW_((*i)->vec_cx(), (*j)->vec_cx())) {
      ERR_RET("vertex check: edges not CCW");
    }
  }
  if (abs(q) > EPSILON) {
    ERR_RET("vertex check: star of vertex does not add up");
  }
}

Vertex &Vertex::from(const flatsurf::Vertex &v) {
  for (Vertex *w : S->vertices) {
    if (static_cast<flatsurf::Vertex>(*w) == v) {
      return *w;
    }
  }
  throw std::logic_error(
      "libflatsurf's vertex not corresponding to any vertices here");
}

Vertex::operator flatsurf::Vertex() const {
  // This is extremely slow. But maybe it does not matter since we are trying
  // to get rid of code paths crossing the polygon/libflatsurf boundary anyway?
  assert(out_edges.begin() != out_edges.end());
  return flatsurf::Vertex::source(
      static_cast<flatsurf::HalfEdge>(**out_edges.begin()),
      static_cast<flatsurf::FlatTriangulationCombinatorial>(*S));
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
