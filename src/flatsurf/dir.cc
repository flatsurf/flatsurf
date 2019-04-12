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
#include <iostream>
#include <list>

#include "./dir.h"
#include "./elementary_geometry.h"
#include "./vertex.h"

using boost::math::constants::pi;
using std::abs;
using std::endl;
using std::flush;
using std::list;

namespace polygon {
template <typename PointT>
Dir<PointT>::Dir() {}

template <typename PointT>
Dir<PointT>::Dir(list<OEdge>::iterator e) {
  v = e->head();
  vec = e->vec<PointT>();
  ep = e->this_vert_iter();
}

template <typename PointT>
Dir<PointT>::Dir(Vertex* vp, const PointT& p) : vec(p) {
  list<list<OEdge>::iterator>::iterator i, j;

  v = vp;

  /* still clean up */

  i = v->out_edges.begin();
  j = next_vert_edge(i);

  while (i != v->out_edges.end()) {
    if (aligned((*i)->template vec<PointT>(), p) ||
        ((!colinear((*i)->vec<PointT>(), p)) && CCW_((*i)->vec<PointT>(), p) &&
         (!aligned((*j)->vec<PointT>(), p)) && CCW_(p, (*j)->vec<PointT>()))) {
      ep = i;
      Check();
      return;
    }

    ++i;
    j = next_vert_edge(i);
  }

  ERR_RET("dir: bad constructor");
}

template <typename PointT>
void Dir<PointT>::Check() {
  auto i = ep;

  if ((!aligned((*i)->template vec<PointT>(), vec)) &&
      CCW_(vec, (*i)->template vec<PointT>())) {
    ERR_RET("dir_check: bottom range");
  }

  i = next_vert_edge(i);

  if (aligned((*i)->template vec<PointT>(), vec)) {
    ERR_RET("dir_check: aligned top range");
  }

  if (CCW_((*i)->template vec<PointT>(), vec)) {
    ERR_RET("dir_check: top range");
  }
}

template <>
Point Dir<Point>::vec_cx() {
  return (vec);
}

template <>
Point Dir<BigPointI>::vec_cx() {
  return vec.cx;
}

template <>
alg_tI Dir<Point>::vec_algtI() {
  return alg_tI();
}

template <>
alg_tI Dir<BigPointI>::vec_algtI() {
  return vec.algt;
}

template <>
COORD Dir<Point>::AngleF(Dir<Point>& d2) {
  Dir<Point> d1 = *this;
  if (d1.v != d2.v) {
    ERR_RET("Angle: vertices unequal");
  }

  list<list<OEdge>::iterator>::iterator i, j;

  COORD rotated = 0;

  i = d1.ep;
  j = next_vert_edge(d1.ep);

  while (i != d2.ep) {
    rotated += angle((*i)->vec_cx(), (*j)->vec_cx());

    i = next_vert_edge(i);
    j = next_vert_edge(j);
  }

  rotated = rotated + angle((*(d2.ep))->vec_cx(), d2.vec) -
            angle((*(d1.ep))->vec_cx(), d1.vec);

  if (rotated < 0) {
    rotated = rotated + d1.v->total_angle();
  }
  return (rotated);
}

template <>
Dir<Point> Dir<Point>::RotateF(COORD theta) {
  Dir<Point> n;

  if (theta < 0) {
    ERR_RET("rotateF: negative angle");
  }

  if (theta >= pi<COORD>()) {
    ERR_RET("RotateF: angle greater then PI");
  }

  Point p = Point(cos(theta) * vec.real() - sin(theta) * vec.imag(),
                  +sin(theta) * vec.real() + cos(theta) * vec.imag());

  n = RotateCCwToVec(p);

  if (abs(AngleF(n) - theta) > 1E-7) {
    std::cerr << "in Angle_F, angle off by " << abs(AngleF(n) - theta) << endl
              << flush;
    ERR_RET("Error in RotateF");
  }

  n.Check();

  return (n);
}

template <>
Dir<Point> Dir<Point>::RotateF_general(COORD theta) {
  Dir<Point> n;

  if (theta < 0) {
    ERR_RET("rotate_general: negative angle");
  }
  std::cout << "RotateF: theta = " << theta << endl << flush;
  std::cout << "starting edge: E" << (*(this->ep))->id() << endl << flush;

  Dir<Point> bak = *this;
  COORD theta_bak = theta;

  //    Dir<Point> tmp =*this;
  n = *this;

#ifdef USE_QUAD
  int count = floor(2.0 * theta / pi<COORD>()).convert_to<int>();
#else
  int count = static_cast<int>(floor(2.0 * theta / pi<COORD>()));
#endif

  std::cout << "RotateF: count = " << count << endl << flush;
  for (int j = 0; j < count; j++) {
    //	tmp = n;
    //	tmp.RotateF(0.5*MY_PI,n);
    n = n.RotateF(0.5 * pi<COORD>());
    theta = theta - 0.5 * pi<COORD>();
  }
  if (theta >= 0) {
    //	tmp = n;
    n = n.RotateF(theta);
  }
  std::cout << "ending edge: E" << (*(n.ep))->id() << endl << flush;
  if (abs(bak.AngleF(n) - theta_bak) > 1E-7) {
    std::cerr << "in Angle_F, angle off by " << abs(bak.AngleF(n) - theta_bak)
              << endl
              << flush;
    ERR_RET("error in RotateF_general");
  }
  return (n);
}

template <typename PointT>
Dir<PointT> Dir<PointT>::RotateCCwToVec(PointT p) {
  Dir n;

  list<list<OEdge>::iterator>::iterator i, j;

  n.v = v;
  n.vec = p;

  i = ep;
  j = next_vert_edge(ep);

  while (!CCW3((*i)->vec<PointT>(), p, (*j)->vec<PointT>())) {
    i = next_vert_edge(i);
    j = next_vert_edge(j);
  }
  n.ep = i;

  n.Check();
  //    if( AngleF(n) < -0.0001 || AngleF(n) > 2*MY_PI + 0.0001 ) {
  //	ERR_RET("Error in RotateCCwToVec");
  //    }
  //    if( AngleF(n) > MY_PI + 0.0001 ) {
  //	std::cerr << "*****WARNING: ROTATE CCW " << AngleF(n) << endl << flush;
  //    }

  return (n);
}

template <typename PointT>
Dir<PointT> Dir<PointT>::RotateCwToVec(PointT p) {
  Dir n;
  list<list<OEdge>::iterator>::iterator i, j;

  n.v = v;
  n.vec = p;

  if (aligned((*ep)->template vec<PointT>(), p)) {
    n.ep = ep;
    n.Check();
    return (n);
  }

  i = prev_vert_edge(ep);
  j = ep;

  while (!CCW3((*i)->vec<PointT>(), p, (*j)->vec<PointT>())) {
    i = prev_vert_edge(i);
    j = prev_vert_edge(j);
  }
  n.ep = i;
  n.Check();
  //    if( n.AngleF(*this) < -0.0001 || n.AngleF(*this) > 2*MY_PI + 0.0001 ) {
  //	ERR_RET("Error in RotateCwToVec");
  //   }
  //    if( n.AngleF(*this) > MY_PI + 0.0001 ) {
  //	std::cerr << "*****WARNING: ROTATE CW " << n.AngleF(*this)<<endl<<flush;
  //    }

  return (n);
}

template class Dir<Point>;
template class Dir<BigPointI>;
}  // namespace polygon
