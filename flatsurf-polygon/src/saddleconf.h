/**********************************************************************
 *  This file is part of flatsurf.
 *
 *        Copyright (C) 2018 Alex Eskin
 *        Copyright (C) 2019 Julian Rüth
 *
 *  Flatsurf is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
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

#ifndef FLATSURF_SADDLECONF_H
#define FLATSURF_SADDLECONF_H

#include <ostream>
#include <set>

#include "./cylinder.h"
#include "./defs.h"
#include "./dir.h"
#include "./vert_pattern.h"

namespace polygon {
class Vertex;
class SaddleConf;

class SaddleConf {
  friend class VertPattern;

 public:
  /* WARNING: check renumber_saddles before adding fields to this
           class */

  SaddleConf();
  void clear();
  int add_saddle(Dir<Point> start, Dir<Point> end, const alg_tI& algt);
  bool isom(SaddleConf& sc, int* s_matched);
  void print(std::ostream& output_stream);
  int n_saddles();
  int find_vert(Vertex* v); /* the index of the vert */

  VertPattern vp[MAX_VERTICES]; /* add to check */
  int n_vp;                     /* nbr of vertex patterns this saddleconf */

  int count;       /* number of occurences of this conf */
  int group_count; /* number of occurences in of conf in this group */

  void normalize();
  void pack();
  void unpack();

  void renorm_lengths();  // for --show_lengths
  COORD get_orig_min_saddle_length();
  void renumber_saddles(int* s_matched);  // for --closure

  Cylinder cyl[MAX_SADDLES];

  // for calculating cylinders
  int n_cyl;
  void calculate_cylinders();
  void renorm_cylinders();
  COORD get_orig_min_cyl_length();

  int shortest_saddle_id(); /*id of shortest saddle */
  void get_saddle_by_id(int id, int& in_vp_index, int& in_at, int& out_vp_index,
      int& out_at);

  COORD get_length_by_id(int id);
  //    int follow_right(int i);
  int follow_left(int i);
  void output_cylinders(FILE* fp);

  void check_cross_saddle(COORD cyl_len, Dir<Point> cross_saddle_Dir);
  void CheckCrossSaddles();

  // for long and short
  COORD shortest_occurence;
  COORD shortest_cyl_occurence;
  COORD longest_occurence;

  COORD smallest_total_area;
  COORD largest_total_area;

  // for heights_and_twists
  COORD total_area_of_cyls;
  Dir<Point> get_Dir_by_id(int id);

  // for closure
  FILE* output_f;

  // for sorting
  bool operator<(const SaddleConf& scf) const { return (count > scf.count); }

  // for visualization
  int tag;  // -1 if uninitialezd
  void DrawSaddles();
  void DrawCylinders();

  std::set<COORD> lengths_set;

 private:
  COORD original_min_cylinder_length;
  COORD
  original_shortest_length;      /*original length of shortest saddle,
                                                                                                                                                 before dividing */
  alg_tI original_shortest_algt; /* the same as above, but algebraically */

  int next_id; /* id of next saddle connection */
  bool isomInternal(SaddleConf& sc, int n_matched, int* v_match, int* s_match);
  Dir<Point> start_Dir[MAX_SADDLES];
  alg_tI start_algt[MAX_SADDLES];
};

typedef SaddleConf* SaddleConfPtr;
}  // namespace polygon

#endif  // FLATSURF_SADDLECONF_H
