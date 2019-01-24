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

#ifndef LIBPOLYGON_PSIMPLEX_H
#define LIBPOLYGON_PSIMPLEX_H

#include "libpolygon.h"

class my_ostream {
 public:
  explicit my_ostream(const string& filename);
  ofstream& tri();
  ofstream& tex();
  void close();

 private:
  ofstream tri_stream;
  ofstream tex_stream;
};

// included from two_complex.h
class PSimplex {
 public:
  PSimplex(Point, int);

  Point p;
  int in_pcomplex;

  virtual SimplexPtr sp();
  virtual void Draw(my_ostream&, COORD);
  virtual ~PSimplex();

 private:
  PSimplex(PSimplex&);
};

class PVertex : public PSimplex {
 public:
  PVertex(VertexPtr, Point, int);

  void Draw(my_ostream&, COORD d) override;

  SimplexPtr sp() override;
  VertexPtr s;
};

class PUEdge : public PSimplex {
 public:
  PUEdge(UEdgePtr, Point, int);
  PUEdge(OEdge&, Point, int);

  void Draw(my_ostream&, COORD) override;

  UEdgePtr s;
  SimplexPtr sp() override;
};

class PFace : public PSimplex {
 public:
  PFace(FacePtr, Point, int);

  void Draw(my_ostream&, COORD) override;

  SimplexPtr sp() override;
  FacePtr s;
  void DrawCylinders(my_ostream&, COORD);
};

typedef PVertex* PVertexPtr;
typedef PUEdge* PUEdgePtr;
typedef PFace* PFacePtr;

#endif  // LIBPOLYGON_PSIMPLEX_H
