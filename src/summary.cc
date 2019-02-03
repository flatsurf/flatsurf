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

#include <boost/math/constants/constants.hpp>
#include <boost/numeric/conversion/cast.hpp>
#include <fstream>
#include <ostream>

#include "libpolygon/globals.h"
#include "libpolygon/summary.h"
#include "libpolygon/two_complex.h"

using boost::numeric_cast;
using boost::math::constants::pi;
using std::endl;
using std::flush;
using std::ofstream;
using std::ostream;

namespace polygon {

Summary::Summary() {
  scf.clear();

  weird_count = 0;
  bad_angle_count = 0;
  close_count = 0;
  reject_count = 0;
  tag_count = 0;
}

void Summary::normalize() {
  for (unsigned int i = 0; i < scf.size(); i++) {
    scf[i].normalize();
  }
}

void Summary::print(ostream& output_stream, COORD part_total, COORD part_group,
                    COORD volume, COORD depth_) {
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
    int j = scf[i].find_vert(S->GetVertex(start_vertex));
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
  output_stream << " weird " << weird_count;
  output_stream << " bad_angle " << bad_angle_count;
  output_stream << " unusual " << count_unusual;
  output_stream << " close " << close_count;
  output_stream << " reject " << reject_count;
  //    output_stream << " threshold " << threshold;
  output_stream << "\n";

  for (unsigned int i = 0; i < scf.size(); i++) {
    if (final_report && closure && scf[i].output_f != nullptr) {
      fclose(scf[i].output_f);
      scf[i].output_f = nullptr;
    }

    if (final_report) {
      if (cyls_only && scf[i].n_cyl <= 0) {
        continue;
      }
      if ((!allow_long_cyls) && scf[i].shortest_cyl_occurence > depth_) {
        continue;
      }

      scf[i].print(output_stream);
      int j = scf[i].find_vert(S->GetVertex(start_vertex));
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
    output_stream << " raw = " << cyl_count << " rejected = " << reject_count
                  << "\n";
  }
}

void Summary::clear_group() {
  for (unsigned int i = 0; i < scf.size(); i++) {
    scf[i].group_count = 0;
  }
}

/* following function assumes that sc is not already present in summary */
size_t Summary::add_new_conf(SaddleConf& sc) {
  //    if( (unsigned int)scf.size() >= scf.capacity() ) {
  //	scf.reserve(2*scf.capacity());
  //    }

  //    scf.insert(scf.end(), *sc);

  if (show_cyls && !closure) {
    sc.calculate_cylinders();
    if (sc.n_cyl) {
      sc.renorm_cylinders();
    }
  }

  if (show_length_list) {
    sc.lengths_set.insert(sc.get_orig_min_saddle_length());
  }

  /* DEBUGGING TOOL */

  /*

                   //move this to add_one_conf, but calculate cylinders first


  //     std::cout << sc.get_orig_min_cyl_length() <<endl;

  //     if( abs(sc.get_orig_min_cyl_length() -4.35978) < 0.01 ) {
                   if( abs(sc.get_orig_min_cyl_length() -9.04971) < 0.01 ) {
                                   // DEBUGGING TOOL

                                   std::cout << "Found it: n_saddles = " <<
  sc.n_saddles() << endl; std::cout << "Drawing Saddles" << endl;


  //	 sc.DrawSaddles();

  //	 S->make_pcomplexes();
  //	 my_ostream saddle_stream("saddle");
  //	 S->NewDraw(saddle_stream);
  //	 saddle_stream.close();

                                   sc.DrawCylinders();

                                   S->make_pcomplexes();
                                   my_ostream saddle_stream("cylinders");
                                   S->NewDraw(saddle_stream);
                                   saddle_stream.close();




                                   exit(0);
                   }

  */

  if (closure) {
    char filename[100];
    /* open file here */
    sprintf(filename, "closure.%zu", scf.size());

    ofstream output_stream(filename);
    S->StatPrint(output_stream);
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
  // check maybe bad copying

  return (scf.size());
}

size_t Summary::add_one_conf(SaddleConf& sc) {
  int s_matched[MAX_SADDLES];

  //    assert( scf.size() == scf.size() );
  //    std::cout << "----------------------\n";

  for (unsigned int i = 0; i < scf.size(); i++) {
    //              sc.print(std::cout);
    //              scf[i].print(std::cout);
    //	      std::cout << endl;

    if (scf[i].isom(sc, s_matched)) {
      //	  	    std::cout << "isom" << "\n";

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
      scf[i].group_count++;
      return numeric_cast<size_t>(scf[i].tag);
    } else {
      //   std::cout << "not isom" << "\n";
    }
  }

  sc.count = 1;
  sc.group_count = 1;
  sc.tag = numeric_cast<int>(tag_count++);

  // if ( show_length_list ) {
  // 	sc.lengths_set.insert(sc.get_orig_min_saddle_length());
  // }
  add_new_conf(sc);

  return numeric_cast<size_t>(sc.tag);
}

#ifdef USE_PARALLEL
void Summary::merge(Summary& sm2) {
  Summary* summary1 = this;
  Summary* summary2 = &sm2;

  int s_matched[MAX_SADDLES];

  fprintf(out_f, "-------------------------------------In merge:\n");

  summary1->normalize();
  summary2->normalize();

  fprintf(out_f, "finished %d of %d slices\n", SA.n_finished(), n_slices);
  fprintf(out_f, "in progress = %d, unassigned = %d\n", SA.n_in_progress(),
          SA.n_unassigned());

  fprintf(out_f, "merging. summary1:\n");
  summary1->print(std::cout, 1.0 / n_slices, 1.0, S->volume(), depth);
  //    std::cout << "summary 1 scf.size() = " << summary1->scf.size() <<endl;
  //    fprintf(out_f,"summary2:\n");
  //    summary2->print(std::cout, 1.0, 1.0, S->volume(), depth);
  //    std::cout << "summary 2 scf.size() = " << summary2->scf.size() <<endl;;

  for (int i = 0; i < summary1->scf.size(); i++) {
    int j;
    for (j = 0; j < summary2->scf.size(); j++) {
      if (summary1->scf[i].isom(summary2->scf[j], s_matched)) {
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
#endif

void Summary::clear() { scf.clear(); }
}  // namespace polygon
