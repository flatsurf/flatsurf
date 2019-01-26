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

#ifndef LIBPOLYGON_TWOCOMPLEX_H
#define LIBPOLYGON_TWOCOMPLEX_H

#include <list>
#include <ostream>
#include <string>

#include "defs.h"
#include "dir.h"
#include "saddleconf.h"
#include "summary.h"

namespace polygon {
extern bool field_arithmetic;
extern COORD EPSILON;

class my_ostream;
class Face;
class Vertex;
class UEdge;
class PFace;
class PSimplex;

class TwoComplex {
 public:
  TwoComplex();
  size_t nedges();
  size_t nfaces();
  size_t nvertices();
  void set_area();
  COORD get_area();
  COORD get_scale_factor();
  void set_scale_factor(COORD scale_factor);

  void ReadComplex(std::string filename);

  void BuildTriangle(int a, int b,
                     int c);  // adds a single triangle with angles
                              // pi*a/(a+b+c), pi*b/(a+b+c), pi*c/(a+b+c)
  void BuildQuad(
      int a, int b, int c,
      int d);  // adds a quadrilateral with angles  2*pi*a/(a+b+c+d),
               // 2*pi*b/(a+b+c+d), 2*pi*c/(a+b+c+d), 2*pi*d/(a+b+c+d)

  void CompleteSurface();
  void
  SetupReflect();  // sets uedges.internal to true if boundary edges, labels
                   // oedges, sets from_edge to pair_edge for each uedge

  void PrintAll(std::ostream&);  // Prints data of vertices, edges, and faces
  void CheckAllFaces();
  void CheckAllVertices();
  void StatPrint(std::ostream&);
  void MakeDrawList();
  void ClearSegmentsToDraw();
  void AddPFace(Face*, Point, size_t);
  void NewDraw(my_ostream&);
  void Expunge();
  void BuildNeighborLists();  // Assign the list of outgoing UEdges vertex.e
                              // (whose head is vertex) and vertex.euclidean for
                              // each vertex.
  void PerturbAll(COORD max_perturb);
  void PerturbConjugates(COORD max_perturb);
  bool is_conjugate_deformation(size_t j, size_t k);

  UEdge* MaxEdge();  // returns pointer to longest edge
  UEdge* MinEdge();
  COORD MinSaddle(Dir<Point>&);

  void TriangulateFace(Face* f);
  void TriangulateAll();
  std::list<OEdge>::iterator BisectFace(Face* f, std::list<OEdge>::iterator i,
                                        std::list<OEdge>::iterator j);
  void AddSteinerPoint(Face* f); /* for debug */
  Face* RemoveVertex(Vertex* v0);
  void RetriangulateSurface();

  UEdge* FlipEdge(UEdge* u);
  Point OpposingDiagonal(UEdge* u);
  bool shouldFlip(UEdge* u);
  void doExtraFlips();

  int64_t get_global_denom();
  void ClearDenominators();
  void StoreVertexOffsets();

  template <typename PointT>
  int SweepNew(COORD length, Dir<PointT> start_dir, COORD GoalTotalAngle);
  void issueFinalReport(Summary& fsm, std::ostream& out, double part_done = 1.0,
                        double part_group = 1.0);
  template <typename PointT>
  Vertex* SweepNextLeft(const Dir<PointT>& start, Dir<PointT>& end, COORD len2,
                        COORD threshold);
  template <typename PointT>
  void DrawSaddle(const Dir<PointT>& start, COORD len2, int id,
                  COORD cyl_length);
  COORD GetSaddles(Dir<Point>& start, COORD depth, int N);  // for RandomShoot
  COORD RandomShoot(Vertex* v0, COORD depth, int M);        // not really used
  template <typename PointT>
  bool NewFollowDir(const Dir<PointT>& start, Dir<PointT>& end, COORD len2);
  template <typename PointT>
  void InvestigateVec(PointT vec, COORD len2, SaddleConf& sc, Summary& smry);
  template <typename PointT>
  void FindCrossSaddle(const Dir<PointT>& start, Dir<PointT>& cross_saddle);

  Face* ReflectFace(UEdge* e, Face* f);
  UEdge* IdentifyEdges(UEdge* e1, UEdge* e2);
  Vertex* IdentifyVertices(Vertex* v0,
                           Vertex* v1);  // any UEdge pointing to v1 now
                                         // points to v0.  v1 is deleted.
  UEdge* FindMatchingEdge(UEdge* e);

  Vertex* AddVertex(
      int id);  // creates new vertex with ID=cur_vertex_id if id=UNDEFINED or
                // ID=id otherwise. Increments cur_vertex_id or ssts it to id+1.
                // Appends new vertex pointer to vertices.
  UEdge* AddUEdge(
      int id, Vertex* v0, Vertex* v1,
      BigPointQ vec);  // Creates new UEdge from v0 to v1 with holonomy vec,
                       // ID=id or cur_uedge_id if id=UNDEFINED.
  UEdge* AddUEdge(int id, Vertex* v0, Vertex* v1, Point vec);
  Face* AddFace(int id,
                std::list<OEdge>);  // Adds new face with oedges= list<OEdge>

  UEdge* GetUEdge(int id);
  Vertex* GetVertex(int id);
  Face* GetFace(int id);

  PFace* get_pface(Face* f);
  bool can_merge(UEdge* ue);
  void relocate(size_t i, Point offset);
  void merge(UEdge* ue);
  void make_pcomplexes();
  void make_pface(Face* f);

  std::list<Vertex*> vertices;
  std::list<Face*> faces;
  std::list<UEdge*> uedges;

  void InitCyclotomicField(int n);  // Set F to Q(primitive nth root of unity)
  void check_algebraicQ();          // check that algebraic vectors match
                                    // numerical counterparts
  void check_algebraicI();          // check that algebraic interger versions of
                                    // edge vectors match numerical counterparts
 private:
  int cur_face_id;
  int cur_vertex_id;
  int cur_uedge_id;
  COORD area;
  COORD scale_factor;

  int VISIT_ID;

  std::list<PSimplex*> dl;
  void MakeDrawListInternal(Face*, Point);
  COORD SweepInternal(
      Face* cf, Point cf_offset, std::list<OEdge>::iterator c_edge, Point c_vec,
      Point upper_bound_vec, COORD len_2,
      std::list<std::list<OEdge>::iterator>::iterator start_iter, Vertex* v0);

  void CompleteInternal(UEdge* e);
  void SetInternalEdges();

  void read_edge_line(FILE* fp, int line_number);
  void read_face_line(FILE* fp, int line_number);
};
}  // namespace polygon

#endif  // LIBPOLYGON_TWOCOMPLEX_H
