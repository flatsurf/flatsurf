/**********************************************************************
 *  This file is part of flatsurf.
 *
 *        Copyright (C) 2018 Alex Eskin
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

#ifndef FLATSURF_VERTPATTERN_H
#define FLATSURF_VERTPATTERN_H

#include <iostream>

#include "./defs.h"
#include "./dir.h"

namespace polygon {
class SaddleConf;

class VertPattern {
  friend class SaddleConf;

 public:
  VertPattern();

  void add(Dir<Point>& start_or_end, int id);
  // - id is incoming, + id outgoing

  void clear();

  void print(std::ostream& output_stream, SaddleConf& sc);

  int len(); /* number of saddles attached */

  bool is_empty(); /* i.e. base does not exist */
  int get_id(
      int i); /* returns the id of the
                                                                                           saddle connection at base+MY_PI*i */

  COORD get_length(
      int i); /* returns the length of the
                                                                                     saddle connection at base+MY_PI*i */

  int shortest_saddle_id(); /*id of shortest saddle */

  void divide_lengths(COORD factor); /* divide all lengths by factor */

  Vertex* get_v(); /* returns the vertex this is a pattern for */

  int nbr_same();      /* number saddles returning to same vertex */
  int nbr_different(); /* number of saddles going to different vertices */
  int nbr_at_pi();     /* number of saddles returning at \pm \pi */
  void simple_print(std::ostream& output_steam);

  void normalize();
  void pack();
  void unpack();

 private:
  Dir<Point> base;
  //	base.v is null if empty

  //    Dir<BigPointI> baseI;            //finish later

  int v_id;
  // this is supposed to be the id of base.v
  // it may get out of sync when structure is passed
  // the normalize() function will make sure that it is back in sync....

  int at[MAX_SADDLES];

  COORD length[MAX_SADDLES];  // used only for --show_lengths
  Point vec[MAX_SADDLES];     // used only for --closure
                              //    BigPointI vecI[MAX_SADDLES];
};

}  // namespace polygon

#endif  // FLATSURF_VERTPATTERN_H
