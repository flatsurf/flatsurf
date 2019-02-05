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

#ifndef POLYGON_GEOMETRY_H
#define POLYGON_GEOMETRY_H

#include <list>
#include <vector>

#include "./big_point.h"
#include "./defs.h"
#include "./oedge.h"

namespace polygon {
extern void reflect_vector(BigPointQ p, BigPointQ vec, BigPointQ &reflection);
extern void reflect_point(const Point &p, const Point &base, const Point &vec,
                          Point &reflection);
extern int test_congruent(std::list<OEdge>::iterator e0,
                          std::list<OEdge>::iterator e1);
extern COORD angle(Point, Point);
extern COORD angle(const BigPointI &, const BigPointI &);
extern COORD true_angle(Point, Point);
extern bool intersect_segment(Point pb, Point pv, Point qb, Point qv,
                              Point &intersection);
extern bool intersect_segment_interior(Point pb, Point pv, Point qb, Point qv,
                                       Point &intersection);
extern bool CCW3(const Point &p, const Point &q, const Point &r);
extern bool CCW3(const BigPointI &p, const BigPointI &q, const BigPointI &r);

extern bool CCW_(const Point &p, const Point &q);
extern bool CCW_(const BigPointI &p, const BigPointI &q);

extern COORD norm(const BigPointI &p);
extern bool colinear(const Point &p, const Point &q);
extern bool colinear(const BigPointI &p, const BigPointI &q);
extern bool aligned(const Point &p, const Point &q);
extern bool aligned(const BigPointI &p, const BigPointI &q);
extern COORD area(const Point &p, const Point &q);
extern COORD d_point_line2(const Point &p, const Point &line);
extern COORD d_point_line2(const BigPointI &p, const BigPointI &line);

extern COORD det3(const std::vector<COORD> &x, const std::vector<COORD> &y,
                  const std::vector<COORD> &z);
extern bool in_circle(Point a, Point b, Point c, Point d);

/* exceptions */
struct vert_index_taken {};
struct vert_bad_angle {
  COORD x;
  explicit vert_bad_angle(COORD q) : x(q) {}
};

inline COORD Dist2(Point p, Point q) { return (norm(p - q)); }

inline COORD Dot(Point p, Point q) {
  return (p.real() * q.real() + p.imag() * q.imag());
}

}  // namespace polygon
#endif
