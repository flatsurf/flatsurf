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

#define LENGTH_THRESHOLD 1E-7

// above used to decide if two saddles have the same length

#include <algorithm>
#include <boost/math/constants/constants.hpp>
#include <boost/math/special_functions/round.hpp>
#include <boost/numeric/conversion/cast.hpp>
#include <cassert>
#include <fstream>
#include <ostream>

#include "libpolygon/cylinder.h"
#include "libpolygon/defs.h"
#include "libpolygon/elementary_geometry.h"
#include "libpolygon/globals.h"
#include "libpolygon/number_field.h"
#include "libpolygon/saddleconf.h"
#include "libpolygon/two_complex.h"
#include "libpolygon/vert_pattern.h"

using boost::numeric_cast;
using boost::math::iround;
using boost::math::round;
using boost::math::constants::pi;
using std::abs;
using std::endl;
using std::flush;
using std::ofstream;
using std::ostream;
using std::sort;

namespace polygon {
/* and add max_vertex_degree to Check */

VertPattern::VertPattern() {
  base.v = nullptr;
  v_id = -1;
  for (int i = 0; i < MAX_SADDLES; i++) {
    at[i] = 0;
    length[i] = -1;
  }
}

bool VertPattern::is_empty() {
  if (base.v == nullptr) {
    return (true);
  }
  return (false);
}

Vertex* VertPattern::get_v() { return (base.v); }

int VertPattern::get_id(int j) {
  if (is_empty()) {
    ERR_RET("get_id: empty vertpattern");
  }

  if (j < 0 || j > base.v->int_angle) {
    ERR_RET("get_id: bad index");
  }

  return (at[j]);
}

COORD VertPattern::get_length(int j) {
  if (is_empty()) {
    ERR_RET("get_id: empty vertpattern");
  }

  if (j < 0 || j > base.v->int_angle) {
    ERR_RET("get_id: bad index");
  }

  return (length[j]);
}

void VertPattern::clear() {
  base.v = nullptr;
  for (int i = 0; i < MAX_SADDLES; i++) {
    at[i] = 0;
    length[i] = 0.0;
  }
  /* later remove above loop for speed */
}

void VertPattern::add(Dir<Point>& d, int id) {
  if (this->is_empty()) {
    base = d;
    v_id = d.v->id();
    for (int i = 0; i < d.v->int_angle; i++) {
      at[i] = 0;
    }
    if (d.v->int_angle >= MAX_SADDLES) {
      ERR_RET("Vertex degree too large...");
    }
    at[0] = id;
    if (show_lengths || show_cyls) {
      length[0] = sqrt(norm(d.vec));
    }

  } else {
    if (!colinear(d.vec, base.vec)) {
      ERR_RET("VertPattern::add: not colinear");
    }

    COORD a = base.AngleF(d);
    int j = iround(a / pi<COORD>());
    if (abs(j * pi<COORD>() - a) > EPSILON) {
      smry.bad_angle_count++;
    }

    if (j == d.v->int_angle) {
      std::cout << "**WARNING: j == d.v->int_angle" << endl << flush;
      j = 0;
    }

    if (j >= d.v->int_angle) {
      std::cout << "a = " << a << " j = " << j
                << " diff = " << abs(j * pi<COORD>() - a) << " V" << d.v->id()
                << " int angle = " << d.v->int_angle << endl
                << flush;

      ERR_RET("VertPattern::add: bad index");
    }
    if (at[j] != 0) {
      fprintf(out_f, "VertPattern::add: index already taken");
      throw vert_index_taken();
    }
    at[j] = id;
    if (show_lengths || show_cyls) {
      length[j] = sqrt(norm(d.vec));
    }
    vec[j] = d.vec;
  }
}

void VertPattern::print(ostream& output_stream, SaddleConf& sc) {
  algebraicQ ratio(NumberField<bigrat>::F);

  for (int i = 0; i < base.v->int_angle; i++) {
    if (at[i] == 0) {
      output_stream << "X";
    } else if (at[i] > 0) {
      output_stream << static_cast<char>(abs(at[i]) - 1 + 'A');
    } else { /* at[i] < 0 */
      output_stream << static_cast<char>(abs(at[i]) - 1 + 'a');
    }
  }
  if (show_lengths) {
    output_stream << " |";
    for (int i = 0; i < base.v->int_angle; i++) {
      if (at[i] == 0) {
        output_stream << "X|";
      } else {
        output_stream << length[i];
        if (int_field_arithmetic) {
          int k = abs(at[i]);
          int j = sc.shortest_saddle_id();

          if (is_alg_quotient(sc.start_algt[k], sc.start_algt[j], ratio)) {
            output_stream << "=" << ratio;
          } else {
            output_stream << "=nonalgebraic";

            //			    output_stream << "(" << sc.start_algt[k];
            //			    output_stream << ")/(" << sc.start_algt[j];
            //			    output_stream << "|";
          }
        }
        output_stream << "|";
      }
    }
  }
}

int VertPattern::len() {
  int count = 0;
  for (int i = 0; i < base.v->int_angle; i++) {
    if (at[i] != 0) {
      count++;
    }
  }
  return (count);
}

/*
COORD VertPattern::shortest_length()
{

                COORD min_length = -1;
                for( int i = 0; i < base.v->int_angle; i++ ) {
                                if ( at[i] != 0 && (min_length < 0 || length[i]
< min_length )) { min_length = length[i];
                                }
                }

                return(min_length);
}
*/

int VertPattern::shortest_saddle_id() {
  COORD min_length = -1;
  int min_id = -1;
  for (int i = 0; i < base.v->int_angle; i++) {
    if (at[i] != 0 && (min_length < 0 || length[i] < min_length)) {
      min_length = length[i];
      min_id = abs(at[i]);
    }
  }
  if (min_id < 0) {
    ERR_RET("shortest_saddle_id: empty VertPattern");
  }
  return min_id;
}

void VertPattern::divide_lengths(COORD factor) {
  for (int i = 0; i < base.v->int_angle; i++) {
    if (at[i] != 0) {
      length[i] = length[i] / factor;
    }
  }
}

int VertPattern::nbr_same() {
  int count = 0;
  for (int i = 0; i < base.v->int_angle; i++) {
    if (at[i] == 0) continue;
    int k = at[i];
    for (int j = 0; j < base.v->int_angle; j++) {
      if (at[j] == -k) {
        count++;
      }
    }
  }
  assert(count % 2 == 0);
  return (count / 2);
}

void VertPattern::normalize() { base.v = S->GetVertex(v_id); }

SaddleConf::SaddleConf()
    : shortest_occurence(-1.0),
      shortest_cyl_occurence(-1.0),
      longest_occurence(-1.0),
      smallest_total_area(-1.0),
      largest_total_area(-1.0),
      original_min_cylinder_length(-1.0),
      original_shortest_length(-1.0) {
  n_vp = 0;
  next_id = 1;
  count = 0;

  // added later
  for (int i = 0; i < MAX_VERTICES; i++) {
    vp[i].clear();
  }
  output_f = nullptr;
}

void SaddleConf::clear() {
  for (int i = 0; i < MAX_VERTICES; i++) {
    vp[i].clear();
  }
  n_vp = 0;
  next_id = 1;
  count = 0;

  if (show_length_list) {
    lengths_set.clear();
  }
  original_shortest_length = -1.0;
  shortest_occurence = -1.0;
  longest_occurence = -1.0;
  original_min_cylinder_length = -1.0;
  shortest_cyl_occurence = -1.0;
  smallest_total_area = -1.0;
  largest_total_area = -1.0;
  tag = -1;
}

/* returns the index of the vertex in this saddleconf */
/* returns -1 if vertex is not found */
int SaddleConf::find_vert(Vertex* v) {
  int i;
  for (i = 0; i < n_vp; i++) {
    if (vp[i].get_v() == v) {
      return (i);
    }
  }
  return (-1);
}

void SaddleConf::normalize() {
  int i;
  for (i = 0; i < n_vp; i++) {
    vp[i].normalize();
  }
}

int SaddleConf::shortest_saddle_id() {
  COORD min_length = -1;
  int min_saddle_id = -1;

  for (int i = 0; i < n_vp; i++) {
    COORD c = get_length_by_id(vp[i].shortest_saddle_id());

    if (min_length < 0 || c < min_length) {
      min_length = c;
      min_saddle_id = vp[i].shortest_saddle_id();
    }
  }
  return (min_saddle_id);
}

void SaddleConf::renorm_lengths() {
  COORD min_length = get_length_by_id(shortest_saddle_id());
  if (min_length < 0) {
    return;
  }
  if (original_shortest_length < 0) {
    original_shortest_length = min_length;
    original_shortest_algt = start_algt[shortest_saddle_id()];
  }
  for (int i = 0; i < n_vp; i++) {
    vp[i].divide_lengths(min_length);
  }
}

COORD SaddleConf::get_orig_min_saddle_length() {
  return (original_shortest_length);
}

void SaddleConf::get_saddle_by_id(int id, int& in_vp_index, int& in_at,
                                  int& out_vp_index, int& out_at) {
  bool found_in = false;
  bool found_out = false;

  for (int i = 0; i < n_vp; i++) {
    for (int j = 0; j < vp[i].base.v->int_angle; j++) {
      if (vp[i].at[j] == id) {
        found_out = true;
        out_vp_index = i;
        out_at = j;
      }
      if (vp[i].at[j] == -id) {
        found_in = true;
        in_vp_index = i;
        in_at = j;
      }
    }
  }
  if (!found_in || !found_out) {
    ERR_RET("get_saddle_by_id");
  }
  return;
}

void SaddleConf::renorm_cylinders() {
  COORD min_length = -1;

  for (int i = 0; i < n_cyl; i++) {
    if (min_length < 0 || cyl[i].length < min_length) {
      min_length = cyl[i].length;
    }
  }
  if (n_cyl && original_min_cylinder_length < 0) {
    if (get_orig_min_saddle_length() <
        0) { /* saddles were not renormed:
                                                                                                                              FIXME */
      original_min_cylinder_length = min_length;
    } else { /* rescale by the original minimal length of a saddle */
      original_min_cylinder_length = min_length * get_orig_min_saddle_length();
    }
  }
  for (int i = 0; i < n_cyl; i++) {
    cyl[i].length = cyl[i].length / min_length;
  }
  if (show_moduli) {
    COORD min_modulus = -1;
    for (int i = 0; i < n_cyl; i++) {
      if (min_modulus < 0 || cyl[i].modulus < min_modulus) {
        min_modulus = cyl[i].modulus;
      }
    }
    for (int i = 0; i < n_cyl; i++) {
      cyl[i].modulus = cyl[i].modulus / min_modulus;
    }
  }
}

COORD SaddleConf::get_orig_min_cyl_length() {
  return (original_min_cylinder_length);
}

COORD SaddleConf::get_length_by_id(int id) {
  int in_vp, in_at, out_vp, out_at;
  get_saddle_by_id(id, in_vp, in_at, out_vp, out_at);
  return (vp[in_vp].length[in_at]);
}

Dir<Point> SaddleConf::get_Dir_by_id(int id) { return (start_Dir[id]); }

/*
int SaddleConf::follow_right(int i)
{

// follow with cylinder on right (may actually be left)

                int in_vp, in_at, out_vp, out_at;


                if ( i > 0 ) {
                                get_saddle_by_id(i, in_vp, in_at, out_vp, out_at
); in_at--; if (in_at < 0) { in_at = vp[in_vp].base.v->int_angle -1;
                                }
                                return( vp[in_vp].at[in_at] );
                } else {
                                get_saddle_by_id(-i, in_vp, in_at, out_vp,
out_at ); out_at--; if (out_at < 0) { out_at = vp[out_vp].base.v->int_angle -1;
                                }
                                return( vp[out_vp].at[out_at] );
                }
}
*/

int SaddleConf::follow_left(int i) {
  /* follow with cylinder on left */

  int in_vp, in_at, out_vp, out_at;

  if (i > 0) {
    get_saddle_by_id(i, in_vp, in_at, out_vp, out_at);
    in_at--;
    if (in_at < 0) {
      in_at = vp[in_vp].base.v->int_angle - 1;
    }

    return (vp[in_vp].at[in_at]);

  } else {
    get_saddle_by_id(-i, in_vp, in_at, out_vp, out_at);
    out_at--;
    if (out_at < 0) {
      out_at = vp[out_vp].base.v->int_angle - 1;
    }

    return (vp[out_vp].at[out_at]);
  }
}

bool operator<(Cylinder cyl1, Cylinder cyl2) {
  return (cyl1.length < cyl2.length);
}

/*
int cyl_compare(const void *ap, const void *bp)
{

                COORD a = *(const COORD*)ap;
                COORD b = *(const COORD*)bp;

//     printf("%Lg %Lg\n",a,b);

                 if( abs(a - b ) < LENGTH_THRESHOLD ) {
                                        return 0;
                 } else if ( a > b ) {
                                        return(1);
                 } else {
                                        return(-1);
                 }
}
*/

void SaddleConf::renumber_saddles(int* s_matched) {
  Dir<Point> tmp_start_Dir[MAX_SADDLES];
  alg_tI tmp_start_algt[MAX_SADDLES];
  int in_vp_index[MAX_SADDLES];
  int in_at[MAX_SADDLES];
  int out_vp_index[MAX_SADDLES];
  int out_at[MAX_SADDLES];

  for (int i = 1; i <= n_saddles(); i++) {
    tmp_start_Dir[i] = start_Dir[i];
    tmp_start_algt[i] = start_algt[i];
  }

  for (int i = 1; i <= n_saddles(); i++) {
    get_saddle_by_id(i, in_vp_index[i], in_at[i], out_vp_index[i], out_at[i]);
  }
  for (int i = 1; i <= n_saddles(); i++) {
    int j = s_matched[i];
    //	  assert(vp[in_vp_index[i]].at[in_at[i]] == -i);
    vp[in_vp_index[j]].at[in_at[j]] = -i;
    //	  assert(vp[out_vp_index[i]].at[out_at[i]] == i);
    vp[out_vp_index[j]].at[out_at[j]] = i;
    start_Dir[j] = tmp_start_Dir[i];
    start_algt[j] = tmp_start_algt[i];
  }
}

void SaddleConf::DrawSaddles() {
  for (int i = 1; i <= n_saddles(); i++) {
    S->DrawSaddle(start_Dir[i], 1.0E10, 0, 0.0);
  }
}

void SaddleConf::calculate_cylinders() {
  n_cyl = 0;
  Dir<Point> cross_saddle_Dir;

  int i;
  bool used[MAX_SADDLES];

  for (i = 1; i <= n_saddles(); i++) {
    used[i] = false;
  }

  while (true) {
    /* find unused saddle */
    for (i = 1; i <= n_saddles(); i++) {
      if (!(used[i])) {
        break;
      }
    }
    if (i == n_saddles() + 1) {
      /* all saddles have been used */
      break;
    }

    int init_saddle = i;
    COORD cyl_len = 0;
    Dir<Point> cyl_Dir = get_Dir_by_id(i);
    cyl_Dir.vec = Point(0, 0);
    /* follow possible cylinder containing saddle with id i */
    while (1) {
      used[i] = true;

      //	    cout << "i = " << i << " get_length_by_id(i) " <<
      // get_length_by_id(i) << " ::sqrt(norm(get_Dir_by_id(i).vec)) " <<
      //::sqrt(norm(get_Dir_by_id(i).vec)) << "\n";

      cyl_len += get_length_by_id(i); /* fix ...*/
      cyl_Dir.vec += get_Dir_by_id(i).vec;

      //	    i = follow_right(i);
      i = follow_left(i);
      if (i < 0) {
        ERR_RET("CalculateCylinders: Orientation Problem");
      }
      if (i == 0) {
        /* no cylinder */
        break;
      }
      if (i == init_saddle) {
        /* found cylinder */
        break;
      }
    }
    if (i) {
      //	    cout << "got cylinder" <<endl;
      /* got a cylinder */
      cyl[n_cyl].vec = cyl_Dir.vec;
      cyl[n_cyl].random_saddle_on_left = i;  // for drawing cylinders
      if (heights_and_twists || show_moduli) {
        // FIXME: when drawing cylinders this should be on

        S->FindCrossSaddle(cyl_Dir, cross_saddle_Dir);

        while (cross_saddle_Dir.v->euclidean) {
          std::cout << "Warining: CrossSaddle Euclidean Vertex" << endl;

          Dir<Point> new_cross_saddle_Dir;

          Dir<Point> new_Dir = cross_saddle_Dir.RotateCCwToVec(cyl_Dir.vec);
          S->FindCrossSaddle(new_Dir, new_cross_saddle_Dir);
          Point tmp_vec = new_cross_saddle_Dir.vec;
          new_cross_saddle_Dir =
              new_cross_saddle_Dir.RotateCCwToVec(cyl_Dir.vec);
          cross_saddle_Dir = new_cross_saddle_Dir.RotateCwToVec(
              tmp_vec + cross_saddle_Dir.vec);
        }

        cyl[n_cyl].cross_saddle = cross_saddle_Dir;
        COORD area_of_cyl = abs(area(cyl[n_cyl].vec, -cross_saddle_Dir.vec));
        cyl[n_cyl].modulus = area_of_cyl / (cyl_len * cyl_len);
      }
      cyl[n_cyl++].length = cyl_len;

    } else {
      //	    cout << "no cylinder" << endl;
    }
  }

  /* check area */
  total_area_of_cyls = 0;
  if (heights_and_twists || show_moduli) {
    for (i = 0; i < n_cyl; i++) {
      /*
                                       cout << "cyl_vec: " << cyl[i].vec << "
         cross: " << -cyl_cross_saddles[i] << " area " <<
               area(cyl[i].vec,-cyl_cross_saddles[i]) << "\n";
      */

      total_area_of_cyls +=
          abs(area(cyl[i].vec, cyl[i].cross_saddle.vec)) / S->get_area();
    }
    /*
                                     cout << "\n" << "Area: " <<
       total_area_of_cyls << "\n";
    */
  }

  COORD min_length = -1;

  for (int ii = 0; ii < n_cyl; ii++) {
    if (min_length < 0 || cyl[ii].length < min_length) {
      min_length = cyl[ii].length;
    }
  }

  /* FIXME: make cylinder a struct */
  if (n_cyl && !closure) {
    //	if ( min_length < depth ) {
    //	    CheckCrossSaddles();
    //	}
    renorm_cylinders();
    sort(cyl, cyl + n_cyl);

    //	qsort(cyl_moduli,n_cyl,sizeof(COORD), cyl_compare);
  }
}

void SaddleConf::DrawCylinders() {
  for (int k = 0; k < n_cyl; k++) {
    /* DEBUGGING */
    //	if( k > 0 ) {
    //	    break;
    //	}
    /* END DEBUGGING */

    int init_saddle = cyl[k].random_saddle_on_left;
    int i = init_saddle;
    /* follow possible cylinder containing saddle with id i */
    while (1) {
      S->DrawSaddle(get_Dir_by_id(i), 1.0E10, k + 1, cyl[k].length);
      i = follow_left(i);
      if (i < 0) {
        ERR_RET("DrawCylinders: Orientation Problem");
      }
      if (i == 0) {
        ERR_RET("DrawCylinders: bad cylinder");
      }
      if (i == init_saddle) {
        /* found cylinder */
        break;
      }
    }

    if (billiard_mode) {
      continue;
    }

    int vert = find_vert(cyl[k].cross_saddle.v);
    COORD a = vp[vert].base.AngleF(cyl[k].cross_saddle);

    //    std::cout << "a/MY_PI = " << a/MY_PI << endl;

#ifdef USE_QUAD
    int j = floor(a / pi<COORD>()).convert_to<int>();
#else
    int j = static_cast<int>(floor(a / pi<COORD>()));
#endif

    //    std::cout << "j = " << j << endl;

    if (j >= vp[vert].base.v->int_angle) {
      ERR_RET("DrawCylinders:CheckCrossSaddle: bad index");
    }
    i = vp[vert].at[j];
    init_saddle = i;
    int _count = 0;

    if (i >= 0) {
      ERR_RET("DrawCylinder: bad initial cross_saddle");
    }
    while (1) {
      S->DrawSaddle(get_Dir_by_id(-i), 1.0E10, -(k + 1), cyl[k].length);
      i = follow_left(i);
      if (i > 0) {
        ERR_RET("DrawCylinder: Orientation Problem");
      }
      if (i == 0) {
        /* no cylinder */
        ERR_RET(
            "DrawCylinder:CheckCrossSaddle: Did not find other end of "
            "cylinder");
      }
      if (_count++ > MAX_SADDLES) {
        ERR_RET("DrawCylinder:CheckCrossSaddle: Infinite Loop");
      }
      if (i == init_saddle) {
        /* found cylinder */
        break;
      }
    }
  }
  if (!billiard_mode) {
    draw_cylinders = true;
  }
  return;
}

void SaddleConf::output_cylinders(FILE* fp) {
  for (int i = 0; i < n_cyl; i++) {
#ifdef USE_QUAD
    fprintf(fp, "|(%s,%s)(%s,%s)", cyl[i].vec.real().str(24).c_str(),
            cyl[i].vec.imag().str(24).c_str(),
            cyl[i].cross_saddle.vec.real().str(24).c_str(),
            cyl[i].cross_saddle.vec.imag().str(24).c_str());
#elif defined USE_LONG_DOUBLE
    fprintf(fp, "|(%.24Lf,%.24Lf)(%.24Lf,%.24Lf)", cyl[i].vec.real(),
            cyl[i].vec.imag(), cyl[i].cross_saddle.vec.real(),
            cyl[i].cross_saddle.vec.imag());
#else
    fprintf(fp, "|(%.24f,%.24f)(%.24f,%.24f)", cyl[i].vec.real(),
            cyl[i].vec.imag(), cyl[i].cross_saddle.vec.real(),
            cyl[i].cross_saddle.vec.imag());
#endif
  }
  fprintf(fp, "|\n");
}

// cppcheck-suppress unusedFunction
void SaddleConf::CheckCrossSaddles() {
  for (int i = 0; i < n_cyl; i++) {
    check_cross_saddle(cyl[i].length, cyl[i].cross_saddle);
  }
  return;
}

void SaddleConf::check_cross_saddle(COORD cyl_len,
                                    Dir<Point> cross_saddle_Dir) {
  int vert = find_vert(cross_saddle_Dir.v);
  COORD a = vp[vert].base.AngleF(cross_saddle_Dir);

  //    std::cout << "a/MY_PI = " << a/MY_PI << endl;

#ifdef USE_QUAD
  int j = floor(a / pi < COORDE()).convert_to<int>();
#else
  int j = static_cast<int>(floor(a / pi<COORD>()));
#endif
  //    std::cout << "j = " << j << endl;

  if (abs(j * pi<COORD>() - a) > EPSILON) {
    smry.bad_angle_count++;
    //	  std::cout << "angle = " << abs(angle(d.vec, base.vec)) -MY_PI << " "
    //		<< abs(a - j*MY_PI) << " at V" << base.v->id();

    //	  fprintf(out_f,"angle = %lg %lg at V%d ",
    //		  abs(angle(d.vec, base.vec)) -MY_PI,
    //		  abs(a - j*MY_PI),base.v->id());
    //	    std::cout << " " << base.vec <<"\n";
    //	    throw vert_bad_angle(abs(j*MY_PI-a));
    //	    ERR_RET("VertPattern::add: bad angle");
  }

  if (j >= vp[vert].base.v->int_angle) {
    std::cout << "a = " << a << " j = " << j
              << " diff = " << abs(j * pi<COORD>() - a) << " V"
              << vp[vert].base.v->id()
              << " int angle = " << vp[vert].base.v->int_angle << endl
              << flush;

    ERR_RET("CheckCrossSaddle: bad index");
  }
  COORD new_cyl_len = 0;
  int i = vp[vert].at[j];
  int init_saddle = i;
  int _count = 0;

  if (i >= 0) {
    std::cout << "a = " << a << " j = " << j
              << " diff = " << abs(j * pi<COORD>() - a) << " V"
              << vp[vert].base.v->id()
              << " int angle = " << vp[vert].base.v->int_angle << endl
              << flush;

    for (int k = 0; k < n_vp; k++) {
      std::cout << 'V' << vp[k].get_v()->id() << ": ";
      vp[k].print(std::cout, *this);
      std::cout << ' ';
    }
    std::cout << endl;

    std::cout << "a/MY_PI = " << a / pi<COORD>() << endl;
    std::cout << "j = " << j << endl;
    std::cout << "id = " << i << endl;
    ERR_RET("CheckCrossSaddle: bad initial saddle");
  }
  while (1) {
    //	    cout << "i = " << i << " get_length_by_id(i) " <<
    // get_length_by_id(i) << " ::sqrt(norm(get_Dir_by_id(i).vec)) " <<
    //::sqrt(norm(get_Dir_by_id(i).vec)) << "\n";

    new_cyl_len += get_length_by_id(-i); /* fix ...*/
    //	std::cout << "In CheckCrossSaddle, i = " << i << " new_cyl_len = " <<
    // new_cyl_len << endl;

    i = follow_left(i);
    if (i > 0) {
      ERR_RET("CheckCrossSaddle: Orientation Problem");
    }
    if (i == 0) {
      /* no cylinder */
      ERR_RET("CheckCrossSaddle: Did not find other end of cylinder");
    }
    if (_count++ > MAX_SADDLES) {
      ERR_RET("CheckCrossSaddle: Infinite Loop");
    }
    if (i == init_saddle) {
      /* found cylinder */
      break;
    }
  }
  COORD c = abs(new_cyl_len - cyl_len);
  if (c > LENGTH_THRESHOLD) {
    vp[vert].print(std::cout, *this);
    std::cout << "i = " << i << endl;
    std::cout << c << endl;
    ERR_RET("CheckCrossSaddle: cylinder lengths did not match");
  }
  return;
}

int SaddleConf::add_saddle(Dir<Point> start, Dir<Point> end, alg_tI& algt) {
  start_Dir[next_id] = start;
  start_algt[next_id] = algt;
  int i = find_vert(start.v);
  if (i < 0) { /* vertex was not visited in this saddleconf */
    vp[n_vp++].add(start, next_id);
  } else {
    vp[i].add(start, next_id);
  }
  int j = find_vert(end.v);
  if (j < 0) {
    vp[n_vp++].add(end, -next_id);
  } else {
    vp[j].add(end, -next_id);
  }
  if (n_vp >= MAX_VERTICES) {
    ERR_RET("SaddleConf::add: too many vertices");
  }
  return (next_id++);
}

int SaddleConf::n_saddles() { return (next_id - 1); }

void SaddleConf::print(ostream& output_stream) {
  for (int i = 0; i < n_vp; i++) {
    output_stream << 'V' << vp[i].get_v()->id() << ": ";
    vp[i].print(output_stream, *this);
    output_stream << ' ';
  }
  output_stream << " ";

  if (show_cyls && !closure) {
    calculate_cylinders();
    if (n_cyl) {
      renorm_cylinders();
      output_stream << " cylinders |";
      for (int i = 0; i < n_cyl; i++) {
        output_stream << cyl[i].length << "|";
      }
    }
    if ((show_moduli || heights_and_twists) && n_cyl) {
      output_stream << "\nModuli: |";
      for (int i = 0; i < n_cyl; i++) {
        output_stream << cyl[i].modulus << "|";
      }
      output_stream << " Min Area: " << smallest_total_area
                    << " Max Area: " << largest_total_area;
    }
  }
}

#define S_UNMATCHED 0
#define V_UNMATCHED -1000

bool SaddleConf::isom(SaddleConf& sc, int* s_matched) {
  //     this->print(std::cout);
  //   sc.print(std::cout);
  //   std::cout << "\n";

  if (this->n_vp != sc.n_vp) {
    return (false);
  }
  if (this->n_saddles() != sc.n_saddles()) {
    return (false);
  }

  //    int s_matched[MAX_SADDLES];
  int i;

  /* saddle id's go from 1 to sc->len */
  for (i = 1; i <= sc.n_saddles(); i++) {
    s_matched[i] = 0; /* 0 means unmatched */
  }
  int v_matched[MAX_VERTICES];
  int j;
  for (j = 0; j < sc.n_vp; j++) {
    v_matched[j] = V_UNMATCHED;
  }
  bool retval = this->isomInternal(sc, 0, v_matched, s_matched);

  if (retval && show_cyls && !closure) {
    sc.calculate_cylinders();
    sc.renorm_cylinders();
    this->calculate_cylinders();
    this->renorm_cylinders();

    if (this->n_cyl != sc.n_cyl) {
      /*
              printf("*******ruled out by number of cyls***\n");
              printf("*********%d %d\n",this->n_cyl,sc.n_cyl);
              this->print(std::cout);
              std::cout << " n_cyl:" << this->n_cyl << "\n";
              sc.print(std::cout);
              std::cout << " n_cyl:" << sc.n_cyl << "\n";
      */

      return (false);
    }

    for (int ii = 0; ii < n_cyl; ii++) {
      if (abs(this->cyl[ii].length - sc.cyl[ii].length) > LENGTH_THRESHOLD) {
        return (false);
      }
      if (show_moduli &&
          abs(this->cyl[ii].modulus - sc.cyl[ii].modulus) > LENGTH_THRESHOLD) {
        return (false);
      }
    }

    if (heights_and_twists &&
        abs(total_area_of_cyls / S->get_area() -
            sc.total_area_of_cyls / S->get_area()) > LENGTH_THRESHOLD) {
      return (false);
    }
  }
  return (retval);
}

bool SaddleConf::isomInternal(SaddleConf& sc, int next_vp, int* v_matched,
                              int* s_matched) {
  if (next_vp >= sc.n_vp) {
    return (true);
  }
  int old_s_matched[MAX_SADDLES];
  int j;
  /* backup the matching table */
  for (int i = 1; i <= sc.n_saddles(); i++) {
    old_s_matched[i] = s_matched[i];
  }

  bool busy_v[MAX_VERTICES];
  for (j = 0; j < sc.n_vp; j++) {
    busy_v[j] = false;
  }
  for (j = 0; j < sc.n_vp; j++) {
    if (v_matched[j] != V_UNMATCHED) {
      busy_v[v_matched[j]] = true;
    }
  }
  VertPattern* current = &(this->vp[next_vp]);
  int cur_deg = current->get_v()->int_angle;

  for (j = 0; j < sc.n_vp; j++) {
    if (busy_v[j]) {
      continue;
    }
    VertPattern* candidate = &(sc.vp[j]);

    if (cur_deg != candidate->get_v()->int_angle) {
      continue;
    }
    int rot, k, m;
    for (rot = 0; rot < cur_deg; rot++) { /* rot = rotation */

      /* reset the saddle matching table */
      //	    int s_matched[MAX_SADDLES];
      int i;

      /* saddle id's go from 1 to sc->len */
      for (i = 1; i <= sc.n_saddles(); i++) {
        s_matched[i] = old_s_matched[i];
      }
      bool busy_s[MAX_SADDLES];
      for (i = 1; i <= sc.n_saddles(); i++) {
        busy_s[i] = false;
      }
      for (i = 1; i <= sc.n_saddles(); i++) {
        if (s_matched[i] != S_UNMATCHED) {
          busy_s[s_matched[i]] = true;
        }
      }

      bool failed = false;
      for (k = 0; k < cur_deg; k++) {
        m = k + rot;
        if (m >= cur_deg) {
          m = m - cur_deg;
        } /* m is the index corresponding to k in the 2nd vp */
        int s1 = current->get_id(k);
        int s2 = candidate->get_id(m);

        if (show_lengths && abs(current->get_length(k) -
                                candidate->get_length(m)) > LENGTH_THRESHOLD) {
          failed = true;
          break;
        }

        if ((s1 > 0 && s2 < 0) || (s1 < 0 && s2 > 0) || (s1 == 0 && s2 != 0) ||
            (s1 != 0 && s2 == 0)) {
          failed = true;
          break;
        }
        if (s1 == 0 && s2 == 0) { /* empty slot */
          continue;
        }

        if (s1 < 0) {
          s1 = -s1;
        }
        if (s2 < 0) {
          s2 = -s2;
        }
        if (s_matched[s1] != S_UNMATCHED) {
          if (s_matched[s1] != s2) {
            failed = true;
            break;
          } else { /* saddles already match */
            continue;
          }
        } else { /* first one unmatched */
          if (busy_s[s2]) {
            failed = true;
            break;
          } else { /* now make match */
            s_matched[s1] = s2;
            busy_s[s2] = true;
          }
        }
      } /* next k*/

      if (!failed) { /* this vertex ok, now check later ones */
        v_matched[next_vp] = j;
        if (this->isomInternal(sc, next_vp + 1, v_matched, s_matched)) {
          return (true);
        }
        /* something later on failed so clean up */
        v_matched[next_vp] = V_UNMATCHED;
      }

    } /* next rot */

  } /* next j */

  return (false);
}

#ifdef USE_MPI

MPI_Datatype mpi_smry_type;
MPI_Op mpi_merge;

void mpi_merge_smry(void* sm1, void* sm2, int* len,
                    MPI_Datatype* mpi_smry_type) {
  if (*len != 1) {
    ERR_RET("merge: *len not 1");
  }

  Summary* summary1 = static_cast<Summary*>(sm1);
  Summary* summary2 = static_cast<Summary*>(sm2);

  fprintf(out_f, "-------------------------------------In merge:\n");

  summary1->normalize();
  summary2->normalize();

  fprintf(out_f, "merging. summary1:\n");
  summary1->print(std::cout, 1.0 / n_slices, 1.0, S->volume(), depth);
  //    std::cout << "summary 1 scf.size() = " << summary1->scf.size() <<endl;

  //    fprintf(out_f,"summary2:\n");
  //    summary2->print(std::cout, 1.0, 1.0, S->volume(), depth);

  //    std::cout << "summary 2 scf.size() = " << summary2->scf.size() <<endl;;

  for (int i = 0; i < summary1->scf.size(); i++) {
    int j;
    for (j = 0; j < summary2->scf.size(); j++) {
      if (summary1->scf[i].isom(summary2->scf[j])) {
        summary2->scf[j].count += summary1->scf[i].count;

        break;
      }
    }
    if (j == summary2->scf.size()) { /* new summary not found */
      summary2->add_new_conf(summary1->scf[i]);
    }
  }
  fprintf(out_f, "merged: ");
  summary2->print(std::cout, 1.0 * SA.n_finished() / n_slices,
                  1.0 * SA.n_finished() / n_slices, S->volume(), depth);
  fprintf(out_f, "-----------------------------------------\n");
}

// MPI_Type_vector( count, blocklen, stride, oldtype,  &newtype );

void my_mpi_init() {
  MPI_Type_vector(sizeof(Summary), 1, 1, MPI_CHAR, &mpi_smry_type);
  MPI_Type_commit(&mpi_smry_type);
  MPI_Op_create(mpi_merge_smry, true, &mpi_merge);
}
#endif

#ifdef USE_PVM
void VertPattern::pack() {
  pvm_pkint(&v_id, 1, 1);
  pvm_pkint(at, MAX_SADDLES, 1);
}

void VertPattern::unpack() {
  pvm_upkint(&v_id, 1, 1);
  pvm_upkint(at, MAX_SADDLES, 1);

  normalize();
}

void SaddleConf::pack() {
  pvm_pkint(&n_vp, 1, 1);
  pvm_pkint(&count, 1, 1);
  pvm_pkint(&group_count, 1, 1);
  pvm_pkint(&next_id, 1, 1);

  for (int i = 0; i < MAX_VERTICES; i++) {
    vp[i].pack();
  }
}

void SaddleConf::unpack() {
  pvm_upkint(&n_vp, 1, 1);
  pvm_upkint(&count, 1, 1);
  pvm_upkint(&group_count, 1, 1);
  pvm_upkint(&next_id, 1, 1);

  for (int i = 0; i < MAX_VERTICES; i++) {
    vp[i].unpack();
  }
}

void Summary::pack() {
  pvm_pkint(&bad_angle_count, 1, 1);
  pvm_pkint(&weird_count, 1, 1);
  pvm_pkint(&close_count, 1, 1);
  int scf_count = scf.size();
  pvm_pkint(&scf_count, 1, 1);
  std::cout << "scf_count = " << scf_count << endl << flush;
  for (int i = 0; i < scf_count; i++) {
    scf[i].pack();
  }
};

void Summary::unpack() {
  pvm_upkint(&bad_angle_count, 1, 1);
  pvm_upkint(&weird_count, 1, 1);
  pvm_upkint(&close_count, 1, 1);
  int scf_count;
  pvm_upkint(&scf_count, 1, 1);
  scf.resize(scf_count);
  for (int i = 0; i < scf_count; i++) {
    scf[i].unpack();
  }
  normalize();
};
#endif
}  // namespace polygon
