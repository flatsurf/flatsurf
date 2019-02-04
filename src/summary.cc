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

#include <error.h>
#include <boost/math/constants/constants.hpp>
#include <boost/numeric/conversion/cast.hpp>
#include <fstream>
#include <sstream>

#include "libpolygon/globals.h"
#include "libpolygon/log.h"
#include "libpolygon/summary.h"
#include "libpolygon/two_complex.h"

using boost::numeric_cast;
using boost::math::constants::pi;
using std::endl;
using std::flush;
using std::ofstream;
using std::string;
using std::stringstream;

namespace polygon {

string Summary::to_string(COORD part_total, COORD part_group, COORD volume,
                          COORD depth_, const Log& log) {
  stringstream output_stream;
  int symmetry_factor;

  bool final_report = false;
  if (part_total >= 1.0) {
    final_report = true;
    part_total = 1.0;
  }

  if (final_report) {
    sort(scf.begin(), scf.end());
  }

  int total_count = 0;
  int cyl_count = 0;

  for (unsigned int i = 0; i < scf.size(); i++) {
    int j = scf[i].find_vert(scf[i].complex.GetVertex(start_vertex));
    symmetry_factor = scf[i].vp[j].len();
    if (symmetry_factor == 0) {
      symmetry_factor = 1;
    }

    total_count += scf[i].count / symmetry_factor;
    cyl_count += scf[i].count * scf[i].n_cyl / symmetry_factor;
  }
  int count_unusual = 0;
  /*    int threshold = total_count/100000;
  for(unsigned int i = 0; i < scf.size() ; i++ ) {
                  if( scf[i].count <= threshold ) {
                                  count_unusual += scf[i].count;
                  }
  }
  */
  // FOR DEBUGGING:::
  // threshold = -1;

  output_stream << "\n part done:" << round(100 * part_group) << "%";
  output_stream << " weird " << log.weird_count;
  output_stream << " bad_angle " << log.bad_angle_count;
  output_stream << " unusual " << count_unusual;
  output_stream << " close " << log.close_count;
  output_stream << " reject " << log.reject_count;
  //    output_stream << " threshold " << threshold;
  output_stream << "\n";

  for (unsigned int i = 0; i < scf.size(); i++) {
    if (final_report && closure && scf[i].output_f != nullptr) {
      if (fclose(scf[i].output_f) != 0) {
        error(1, errno, "could not flush output file");
      }
    }

    if (final_report) {
      if (cyls_only && scf[i].n_cyl <= 0) {
        continue;
      }
      if ((!allow_long_cyls) && scf[i].shortest_cyl_occurence > depth_) {
        continue;
      }

      scf[i].print(output_stream);
      int j = scf[i].find_vert(scf[i].complex.GetVertex(start_vertex));
      symmetry_factor = scf[i].vp[j].len();
      if (symmetry_factor == 0) {
        symmetry_factor = 1;
      }
      output_stream << "\n    total: "
                    << scf[i].count * volume * pi<COORD>() /
                           (6.0 * depth_ * depth_ * part_total *
                            symmetry_factor)
                    << " "
                    << scf[i].count * volume /
                           (pi<COORD>() * depth_ * depth_ * part_total *
                            symmetry_factor);
      output_stream << " sym = " << symmetry_factor << endl;
      output_stream << "    raw = " << scf[i].count
                    << " short saddle = " << scf[i].shortest_occurence;

      if (scf[i].n_cyl > 0) {
        output_stream << " short cyl = " << scf[i].shortest_cyl_occurence;
      }
      output_stream << " tag #" << scf[i].tag;
      output_stream << "\n";
      if (show_length_list) {
        for (auto jj = scf[i].lengths_set.begin();
             jj != scf[i].lengths_set.end(); ++jj) {
          output_stream << *jj << " ";
        }
        output_stream << "\n";
      }
    }
  }
  if (show_cyls) {
    output_stream << "\ntotal cylinders: "
                  << cyl_count * volume * pi<COORD>() /
                         (6.0 * depth_ * depth_ * part_total)
                  << " "
                  << cyl_count * volume /
                         (pi<COORD>() * depth_ * depth_ * part_total);
    output_stream << " raw = " << cyl_count
                  << " rejected = " << log.reject_count << "\n";
  }

  return output_stream.str();
}

/* following function assumes that sc is not already present in summary */
void Summary::add_new(SaddleConf& sc) {
  if (show_cyls && !closure) {
    sc.calculate_cylinders();
    if (sc.n_cyl) {
      sc.renorm_cylinders();
    }
  }

  if (show_length_list) {
    sc.lengths_set.insert(sc.get_orig_min_saddle_length());
  }

  if (closure) {
    char filename[100];
    /* open file here */
    sprintf(filename, "closure.%zu", scf.size());

    ofstream output_stream(filename);
    // sc.complex.StatPrint(output_stream);
    sc.print(output_stream);
    output_stream << endl << "------------------------" << endl;
    output_stream.close();

    sc.output_f = fopen(filename, "a");
    sc.calculate_cylinders();

    sc.output_cylinders(sc.output_f);
  }

  sc.shortest_occurence = sc.get_orig_min_saddle_length();
  sc.longest_occurence = sc.get_orig_min_saddle_length();
  sc.shortest_cyl_occurence = sc.get_orig_min_cyl_length();
  sc.smallest_total_area = sc.total_area_of_cyls;
  sc.largest_total_area = sc.total_area_of_cyls;

  scf.push_back(sc);
}

void Summary::add(SaddleConf& sc) {
  int s_matched[MAX_SADDLES];

  for (unsigned int i = 0; i < scf.size(); i++) {
    if (scf[i].isom(sc, s_matched)) {
      if (closure) {
        sc.renumber_saddles(s_matched);
        sc.calculate_cylinders();

        assert(sc.n_cyl == scf[i].n_cyl);
        // some extra debugging
        if (!sc.isom(scf[i], s_matched)) {
          ERR_RET("renumber_saddles messed up");
        }
        for (int j = 1; j < sc.n_saddles(); j++) {
          if (s_matched[j] != j) {
            ERR_RET("matching table messed up");
          }
        }

        /* print out the cylinder data into file */
        sc.output_cylinders(scf[i].output_f);
      }

      if (show_length_list) {
        scf[i].lengths_set.insert(sc.get_orig_min_saddle_length());
      }

      if (show_lengths || show_cyls) {
        if (sc.get_orig_min_saddle_length() < 0) {
          sc.print(std::cout);
          std::cout << flush;
          ERR_RET("original_shortest_length < 0");
        }
        if (scf[i].shortest_occurence > sc.get_orig_min_saddle_length()) {
          scf[i].shortest_occurence = sc.get_orig_min_saddle_length();
        }
        if (scf[i].longest_occurence < sc.get_orig_min_saddle_length()) {
          scf[i].longest_occurence = sc.get_orig_min_saddle_length();
        }
        if (scf[i].shortest_cyl_occurence > sc.get_orig_min_cyl_length()) {
          scf[i].shortest_cyl_occurence = sc.get_orig_min_cyl_length();
        }
        if (show_moduli || heights_and_twists) {
          if (scf[i].total_area_of_cyls > sc.total_area_of_cyls) {
            scf[i].smallest_total_area = sc.total_area_of_cyls;
          }
          if (scf[i].largest_total_area < sc.total_area_of_cyls) {
            scf[i].largest_total_area = sc.total_area_of_cyls;
          }
        }
      }

      scf[i].count++;
      return;
    }
  }

  sc.count = 1;
  sc.tag = numeric_cast<int>(scf.size());

  add_new(sc);
}
}  // namespace polygon
