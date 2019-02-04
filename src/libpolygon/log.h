/**********************************************************************
 *  This file is part of Polygon.
 *
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

#ifndef LIBPOLYGON_LOG_H
#define LIBPOLYGON_LOG_H

namespace polygon {
// Encapsulates some things that are mostly of interest for debugging.
// TODO: This is a global singleton. Do we care about these numbers?
// Then we should turn them into some thread_local context that we
// swap in/out whenever we call into this library from Cython or the like.
class Log {
 public:
  static Log log;

  int bad_angle_count;
  int weird_count;
  int close_count;
  int reject_count;
};
}  // namespace polygon

#endif  // LIBPOLYGON_LOG_H
