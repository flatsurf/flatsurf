/**********************************************************************
 *  This file is part of Flatsurf.
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
 *  along with Flatsurf. If not, see <https://www.gnu.org/licenses/>.
 *********************************************************************/

#ifndef FLATSURF_PFACE_H
#define FLATSURF_PFACE_H

#include "./defs.h"
#include "./my_ostream.h"
#include "./psimplex.h"

namespace polygon {
class Simplex;
class Face;

class PFace : public PSimplex {
 public:
  PFace(Face*, Point, size_t);

  void Draw(my_ostream&, COORD) override;

  Simplex* sp() override;
  Face* s;
  void DrawCylinders(my_ostream&, COORD);
};
}  // namespace polygon

#endif  // FLATSURF_PFACE_H
