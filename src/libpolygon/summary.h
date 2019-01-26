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

#ifndef LIBPOLYGON_SUMMARY_H
#define LIBPOLYGON_SUMMARY_H

#include <ostream>

#include "defs.h"
#include "saddleconf.h"

namespace polygon {
class Summary {
 public:
  Summary();
  void print(std::ostream& output_stream, COORD part_total, COORD part_group,
             COORD volume, COORD depth);

  size_t add_new_conf(SaddleConf& sc);
  size_t add_one_conf(SaddleConf& sc);

  void clear_group();
  int bad_angle_count;
  int weird_count;
  int close_count;
  int reject_count;

  void normalize();
  void define_mpi_type();

  void merge(Summary& s2);
  void clear();

  void pack();
  void unpack();

  bool scf_compare(int i, int j);

  size_t tag_count;

  // private:

  std::vector<SaddleConf> scf;
};
}  // namespace polygon

#endif  // LIBPOLYGON_SUMMARY_H
