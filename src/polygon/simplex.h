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

#ifndef POLYGON_SIMPLEX_H
#define POLYGON_SIMPLEX_H

#include <ostream>
#include <string>

#include "./defs.h"

namespace polygon {

class Simplex {
 public:
  Simplex();
  void SetColor();
  void Print(std::ostream &);
  int id();

  std::string color;

  int visit_id;

  int ID;
  char tag;

 private:
  Simplex(Simplex &);
};
}  // namespace polygon

#endif  // POLYGON_SIMPLEX_H
