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

#include <boost/math/constants/constants.hpp>
#include <cassert>
#include <complex>
#include <list>

#include "./defs.h"
#include "./elementary_geometry.h"
#include "./globals.h"
#include "./oedge.h"
#include "./two_complex.h"

using namespace polygon;

using boost::math::constants::pi;
using std::abs;
using std::complex;
using std::list;

namespace polygon {
/* square of distance from point to line */
COORD d_point_line2(const Point &u, const Point &v) {
  COORD p = Dot(u, v) / Dot(v, v);
  return (Dist2(u, p * v));
}
COORD d_point_line2(__attribute__((unused)) const BigPointI &u,
                    __attribute__((unused)) const BigPointI &v) {
  ERR_RET("Call to d_point_line2 (BigPointI, BigPointI)");
}

/* fix this function */
#if defined(USE_LONG_DOUBLE) || defined(USE_QUAD)
#include <boost/multiprecision/mpfr.hpp>
COORD my_mpq_get_d(bigrat op) {
  mpq_t num, denom;

  mpq_init(num);
  mpq_init(denom);
  mpq_set_z(num, op.get_num_mpz_t());
  mpq_set_z(denom, op.get_den_mpz_t());

  mpq_t tmp2;
  mpq_init(tmp2);
  mpq_div(tmp2, num, denom);

  mpfr_t q;

  mpfr_init2(q, 53);
  mpfr_set_q(q, tmp2, GMP_RNDN);

  boost::multiprecision::mpfr_float_50 s(q);

  COORD ret = s.convert_to<COORD>();

  mpfr_clear(q);
  mpq_clear(tmp2);

  return ret;
}
#else
COORD my_mpq_get_d(bigrat op) { return op.get_d(); }
#endif

COORD my_mpq_get_d(int op) { return static_cast<COORD>(op); }

COORD my_mpq_get_d(int64_t op) { return static_cast<COORD>(op); }

void reflect_vector(BigPointQ p, BigPointQ vec, BigPointQ &reflection) {
  COORD ratio;

  vec.Check();

  /* reflection = 2 (p . vec)/(vec.vec)) vec - p */
  ratio = 2.0 * (p.cx.real() * vec.cx.real() + p.cx.imag() * vec.cx.imag()) /
          (norm(vec.cx));

  reflection.cx = ratio * vec.cx - p.cx;

  if (field_arithmetic) {
    p.Check();
    vec.Check();

    algebraicQ xi;
    bool rigid = vec.algt.get_direction(xi);

    if (!rigid) {
      ERR_RET("reflect_vector: not rigid");
    }

    /* z . w = (1/4)(z+\bar{z})(w+\bar{w}) -(1/4)(z-\bar{z})(w - \bar{w}) =
                                     = (1/4)(z w + \bar{z}w + z \bar{w} +\bar{z}
       \bar{w} - -z w +\bar{z} w + z \bar{w}
             -\bar{z} \bar{w}) = = (1/2) ( z \bar{w} + \bar{z} w ).
    */

    alg_tQ p_a = p.algt;

    alg_tQ ratio_alg = p_a * xi.conjugate() + p_a.conjugate() * xi;
    ratio_alg /= xi.norm();

    reflection.algt = ratio_alg * xi - p_a;

    if (abs(reflection.cx - reflection.algt.tocomplex()) < 100 * EPSILON) {
      reflection.cx = reflection.algt.tocomplex();
    } else {
      ERR_RET("Reflection does not match");
    }

    reflection.Check();
  }
}

int test_congruent(list<OEdge>::iterator e0, list<OEdge>::iterator e1) {
  Point vec0, vec1;

  vec0 = e0->vec_cx();
  vec1 = e1->vec_cx();

  if (abs(vec0.real() - vec1.real()) < EPSILON &&
      abs(vec0.imag() - vec1.imag()) < EPSILON)
    return (CONG_SAME);

  if (abs(vec0.real() + vec1.real()) < EPSILON &&
      abs(vec0.imag() + vec1.imag()) < EPSILON)
    return (CONG_REVERSE);

  return (CONG_FALSE);
}

COORD angle(Point p, Point q) {
  COORD x =
      (p.real() * q.real() + p.imag() * q.imag()) / sqrt(norm(p) * norm(q));
  if (x > 1) {
    x = 1;
  } else if (x < -1) {
    x = -1;
  }

  return (acos(x));
}

COORD angle(__attribute__((unused)) const BigPointI &p,
            __attribute__((unused)) const BigPointI &q) {
  ERR_RET("angle called on BigPointI arguments");
}

/* following function works for angles > \pi */
COORD true_angle(Point p, Point q) {
  if (colinear(p, q)) {
    if (aligned(p, q)) {
      return (0);
    } else {
      return pi<COORD>();
    }
  }

  COORD a = angle(p, q);

  if (CCW_(p, q))
    return a;
  else
    return 2 * pi<COORD>() - a;
}

bool intersect_segment(Point pb, Point pv, Point qb, Point qv,
                       Point &intersection) {
  COORD D;
  COORD x1, x1p, y1, y1p, x2, x2p, y2, y2p, t, tp;

  x1 = pb.real();
  x1p = qb.real();
  y1 = pb.imag();
  y1p = qb.imag();

  x2 = x1 + pv.real();
  x2p = x1p + qv.real();
  y2 = y1 + pv.imag();
  y2p = y1p + qv.imag();

  D = (x1 - x2) * (y2p - y1p) - (x2p - x1p) * (y1 - y2);

  if (abs(D) < EPSILON) return (false);

  t = ((y2p - y1p) * (x2p - x2) + (x1p - x2p) * (y2p - y2)) / D;
  tp = ((y2 - y1) * (x2p - x2) + (x1 - x2) * (y2p - y2)) / D;

  //    out_s << t << " " << tp;

  if (t < -EPSILON || t > 1 + EPSILON) return (false);

  if (tp < -EPSILON || tp > 1 + EPSILON) return (false);

  intersection = Point(t * x1 + (1 - t) * x2, t * y1 + (1 - t) * y2);

  // the following does not scale
  //     if ( abs( intersection.real() - (tp*x1p + (1-tp)*x2p)) > 5*EPSILON )
  // 	ERR_RET("intersection: condtradiction_x");

  //     if ( abs( intersection.imag() - (tp*y1p + (1-tp)*y2p)) > 5*EPSILON )
  // 	ERR_RET("intersection: condtradiction_y");

  return (true);
}

bool intersect_segment_interior(Point pb, Point pv, Point qb, Point qv,
                                Point &intersection) {
  COORD D;
  COORD x1, x1p, y1, y1p, x2, x2p, y2, y2p, t, tp;

  x1 = pb.real();
  x1p = qb.real();
  y1 = pb.imag();
  y1p = qb.imag();

  x2 = x1 + pv.real();
  x2p = x1p + qv.real();
  y2 = y1 + pv.imag();
  y2p = y1p + qv.imag();

  D = (x1 - x2) * (y2p - y1p) - (x2p - x1p) * (y1 - y2);

  if (abs(D) < EPSILON) return (false);

  t = ((y2p - y1p) * (x2p - x2) + (x1p - x2p) * (y2p - y2)) / D;
  tp = ((y2 - y1) * (x2p - x2) + (x1 - x2) * (y2p - y2)) / D;

  //    out_s << t << " " << tp;

  if (t < 5 * EPSILON || t > 1 - 5 * EPSILON) return (false);

  if (tp < 5 * EPSILON || tp > 1 - 5 * EPSILON) return (false);

  intersection = Point(t * x1 + (1 - t) * x2, t * y1 + (1 - t) * y2);

  // the following does not scale
  //     if ( abs( intersection.real() - (tp*x1p + (1-tp)*x2p)) > 5*EPSILON )
  // 	ERR_RET("intersection: condtradiction_x");

  //     if ( abs( intersection.imag() - (tp*y1p + (1-tp)*y2p)) > 5*EPSILON )
  // 	ERR_RET("intersection: condtradiction_y");

  return (true);
}

COORD norm(const BigPointI &p) { return norm(p.cx); }

/****************************COLINEAR AND ALIGNED *************************/

bool colinear_slow(Point p, Point q) {
  smry.close_count++;

  Point p1 = p / abs(p);
  Point p2 = q / abs(q);

  if (norm(p1 - p2) < EPSILON * EPSILON) {
    return (true);
  }
  if (norm(p1 + p2) < EPSILON * EPSILON) {
    return (true);
  }
  if (norm(p1 - p2) < DELTA * DELTA || norm(p1 + p2) < DELTA * DELTA) {
    too_close_flag = true;
    smry.weird_count++;
  }
  return (false);
}

bool colinear(const Point &p, const Point &q) {
  COORD threshold = 1E-8;

  //    COORD threshold = 1E-2;

  COORD c = p.real() * q.imag() - p.imag() * q.real();

  if (c * c > threshold * norm(p) * norm(q)) {
    // fast path
    return (false);
  }
  return (colinear_slow(p, q));
}

bool colinear(const BigPointI &p, const BigPointI &q) {
  COORD threshold = 1E-8;

  //    COORD threshold = 1E-2;

  COORD c = p.cx.real() * q.cx.imag() - p.cx.imag() * q.cx.real();

  if (c * c > threshold * norm(p.cx) * norm(q.cx)) {
    // fast path
    return (false);
  }

  // slow path
  if (colinear(p.algt, q.algt)) {
    return true;
  } else {
    smry.close_count++;
    return false;
  }
}

bool aligned(const Point &p, const Point &q) {
  if (!colinear(p, q)) {
    return (false);
  }
  if (Dot(p, q) < 0) {
    return (false);
  }
  return (true);
}

bool aligned(const BigPointI &p, const BigPointI &q) {
  if (!colinear(p, q)) {
    return (false);
  }
  if (Dot(p.cx, q.cx) < 0) {
    return (false);
  }
  return true;
}

/****************************** CCW ***************************************/

bool CCW_(const Point &u,
          const Point &v)  // check if u and v are linearly indep. and (u, v) is
                           // positively oriented
{
  if (colinear(u, v)) {
    ERR_RET("Call to CCW when colinear");
  }

  COORD s = -u.imag() * v.real() + u.real() * v.imag();

  /*
                                                                  if ( verbose
     >= 5 && abs(s) < 0.01 ) { out_s << "\nCCW: s = " << s << endl;
                                                                  }
  */

  if (s > 0) {
    return (true);
  }
  return (false);
}

bool CCW_(const BigPointI &bu, const BigPointI &bv) {
  COORD threshold = 1E-8;

  Point u = bu.cx;
  Point v = bv.cx;

  COORD s = -u.imag() * v.real() + u.real() * v.imag();

  if (s * s > threshold * norm(u) * norm(v)) {  // fast path
    if (s > 0) {
      return true;
    } else {
      return false;
    }
  }

  // slow path

  if (colinear(bu, bv)) {
    ERR_RET("Call to CCW when colinear");
  }

  Point p1 = u / abs(u);
  Point p2 = v / abs(v);

  if (norm(p1 - p2) < DELTA * DELTA || norm(p1 + p2) < DELTA * DELTA) {
    // we can't be sure of return. Set flag.
    too_close_flag = true;
    smry.weird_count++;
  }

  if (s > 0) {
    return true;
  } else {
    return false;
  }
}

bool CCW3(const Point &p, const Point &q, const Point &r) {
  if (aligned(p, r)) {
    ERR_RET("CCW3: p and r aligned");
  }

  if (aligned(p, q)) {
    return (true);
  }

  if (aligned(q, r)) {
    return (false);
  }

  if (!colinear(p, r) && CCW_(p, r)) {
    if ((!colinear(p, q)) && CCW_(p, q) && (!colinear(q, r)) && CCW_(q, r)) {
      return (true);
    }
    return (false);
  }
  return (!CCW3(r, q, p));
}

bool CCW3(const BigPointI &p, const BigPointI &q, const BigPointI &r) {
  //    out_s << "In CCW3: p = " << p << " q = " << q << " r = " << r <<endl;

  if (aligned(p, r)) {
    ERR_RET("CCW3: p and r aligned");
  }

  if (aligned(p, q)) {
    return (true);
  }

  if (aligned(q, r)) {
    return (false);
  }

  if (!colinear(p, r) && CCW_(p, r)) {
    if ((!colinear(p, q)) && CCW_(p, q) && (!colinear(q, r)) && CCW_(q, r)) {
      return (true);
    }
    return (false);
  }
  return (!CCW3(r, q, p));
}

/****************************** MISC **************************************/

COORD area(const Point &p, const Point &q) {
  return (p.real() * q.imag() - q.real() * p.imag());
}
}  // namespace polygon
