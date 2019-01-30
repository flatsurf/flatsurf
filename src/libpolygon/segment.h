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

#ifndef LIBPOLYGON_SEGMENT_H
#define LIBPOLYGON_SEGMENT_H

#include "libpolygon/defs.h"

namespace polygon {
/* for drawing saddle connections */
class Segment {
 public:
  Point head;
  Point tail;
  int cyl_on_left;
  COORD cyl_on_left_length;
};
}  // namespace polygon

#endif  // LIBPOLYGON_SEGMENT_H
