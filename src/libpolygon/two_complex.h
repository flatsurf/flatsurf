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

#include "libpolygon.h"

#include <stdint.h>
#include <stdio.h>
#include <cerrno>
#include <complex>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <istream>
#include <list>
#include <ostream>
#include <set>
#include <string>
#include <vector>

#include "defs.h"
extern bool field_arithmetic;
extern COORD EPSILON;

#include "alg_t.h"
#include "algebraic.h"

#define CONG_SAME 0
#define CONG_REVERSE 1
#define CONG_FALSE 2

//#define EPSILON 1E-13
#define MAX_VERTEX_DEGREE 50
#define MAX_SADDLES 102
#define MAX_VERTICES 10
#define MAX_SADDLECONFS 100

#define MAX_HOSTNAME 50

#define UNDEFINED -1000

#define NOT_HIT 0
#define HIT_RELEVANT 1

using namespace std;

typedef ofstream ostdiostream;

class Simplex {
 public:
  Simplex();
  void SetColor();
  void Print(ostream &);
  int id();

  string color;

  int visit_id;

  int ID;
  char tag;

 private:
  Simplex(Simplex &);
};

typedef Simplex *SimplexPtr;

class PSimplex;
typedef PSimplex *PSimplexPtr;
typedef list<PSimplexPtr> DrawList;
typedef list<PSimplexPtr>::iterator DrawListIter;

class Vertex;
typedef Vertex *VertexPtr;
class UEdge;
typedef UEdge *UEdgePtr;
class Face;
typedef Face *FacePtr;
class OEdge;
typedef OEdge *OEdgePtr;

template <typename PointT>
class Dir;
// typedef Dir* DirPtr;

class PFace;
typedef PFace *PFacePtr;

typedef list<OEdge>::iterator OEdgeIter;
typedef list<OEdgeIter>::iterator OEdgePtrIter;
typedef list<VertexPtr>::iterator VertexPtrIter;
typedef list<UEdgePtr>::iterator UEdgePtrIter;
typedef list<FacePtr>::iterator FacePtrIter;

typedef list<OEdgeIter> OEdgePtrList;

class Vertex : public Simplex {
 public:
  Vertex();

  UEdgePtr e;
  int order;  // number of incident edges? Default value 0.
  bool euclidean;
  int int_angle; /* for speed: total_angle_over_pi() */
                 /* Stored by BuildNeighborLists */
  void Check();
  bool boundary();
  bool deleted();
  COORD total_angle();
  int total_angle_over_pi();
  void MoveVertex(Point p);
  bool relevant();

  bool CanRemove();
  void Delete();

  OEdgePtrList out_edges;
};

class UEdge : public Simplex {  // unoriented edge
  friend class OEdge;
  friend class Vertex;
  friend class Face;
  friend class PUEdge;
  friend class TwoComplex;

 public:
  //    UEdge();
  UEdge(VertexPtr a, VertexPtr b,
        Point vec);  // Edge begining at vertex a, ending at b with holonomy vec
  UEdge(VertexPtr a, VertexPtr b, BigPointQ vec);
  void Print(ostream &);
  bool deleted();
  void Delete();
  bool boundary();  // true if UEdge is a boundary edge
  FacePtr boundary_face();
  OEdgeIter boundary_edge();
  OEdgeIter this_edge(FacePtr f);
  void set_null_face(FacePtr f);
  COORD len();

  int index;
  int internal; /* for reflect */
  int label;    /* for reflect */

  int get_priority();  /* for drawing */
  int from_triang;     /* for drawing */
  void set_priority(); /* for drawing */

 private:
  VertexPtr v0;
  VertexPtr v1;
  FacePtr f0;  // left face, going from v0 to v1
  FacePtr f1;  // right face

  BigPointQ ue_vecQ;  // holonomy
  BigPointI ue_vecI;  // holonomy: BigPointI version

  BigPointQ minus_ue_vecQ;  //-holonomy, set by StoreVertexOffsets
  BigPointI minus_ue_vecI;
};

class OEdge {  // oriented edge.
  friend class Face;
  friend class PUEdge;
  friend class UEdge;
  friend class TwoComplex;

 public:
  OEdge(UEdgePtr, int);

  int id();
  VertexPtr head();
  VertexPtr tail();
  FacePtr in_face();     // face to right of OEdge
  FacePtr other_face();  // face to left of OEdge

  OEdgeIter this_iter();  // returns iterator for OEdge in list of edges around
                          // face in which OEdge lies.
  OEdgePtrIter
  this_vert_iter();  // returns iterator for this edge in v.out_edges, where v
                     // is the head of this OEdge
  OEdgeIter next_edge();  // next edge in face in which OEdge lies
  OEdgeIter prev_edge();
  OEdgeIter pair_edge();  // iterator for same edge in opposite face
  template <typename PointT>
  PointT vec();
  Point vec_cx();
  alg_tQ vec_algt();
  BigPointQ vecQ();
  BigPointI vecI();

  bool deleted();
  void Delete();

  template <typename PointT>
  PointT headOffset();

  template <typename PointT>
  PointT tailOffset();

  void set_direction(int);
  void set_vec_cx(Point);
  void set_vec_algt(alg_tQ);
  void set_vec(BigPointQ);

  void Print(ostream &);
  void Check();

  bool operator==(OEdge &);

  /* next 5 for speed: assigned in StoreVertexOffsets */
  template <typename PointT>
  PointT &_head_offset();

  template <typename PointT>
  PointT &_tail_offset();

  template <typename PointT>
  PointT &_vec();

  OEdgeIter next_e;  // value of next_edge()
  OEdgeIter pair_e;

  OEdgeIter from_edge; /* for reflect_face */

  // make next four private
  Point head_offst;
  Point tail_offst;
  BigPointI head_offstI;
  BigPointI tail_offstI;

 private:
  int direction;  // direction ==1 means ue->v0 is the head
  UEdgePtr ue;    // underlying unoriented edge

  //    OEdge(OEdge&);
};

/* for drawing saddle connections */
class Segment {
 public:
  Point head;
  Point tail;
  //    int    id;
  int cyl_on_left;
  COORD cyl_on_left_length;
};
typedef list<Segment>::iterator SegmentListIter;

class Face : public Simplex {
 public:
  Face();
  explicit Face(const list<OEdge> &L);  // sets oedges=L

  void SetOEdgeList(list<OEdge>);

  int order();
  bool deleted();
  void Delete();
  void Print(ostream &);
  void Check();
  COORD volume();
  Point barycenter();

  bool can_bisect(OEdgeIter, OEdgeIter);
  bool self_intersecting();

  OEdgeIter next_iter(OEdgeIter);

  OEdgeIter prev_iter(OEdgeIter);

  Point GetOffsetV0();
  algebraicQ GetOffsetV0_alg();
  /* bad in non-elementary situation */
  //    Point GetOffsetV(VertexPtr);

  bool intersects(Point pb, Point pv);
  bool intersects(FacePtr f, Point offset);
  bool contains(Point p);

  COORD perimeter();
  int parent_face_ID;  // for debugging triangulation

  //    OEdgeIter NextEdgeHit(Point cf_offset, Point dir_vec, OEdgeIter
  //			       base_edge, Point &hit_pos);

  BigPointQ vec;

  list<OEdge> oedges;

  void AddSegmentToDraw(Segment segment);
  void ClearSegmentsToDraw();
  list<Segment> segments_to_draw;
  // for billiard mode
  BigPointQ reflection_vector;
  FacePtr from_face;

  FacePtr transform_to_original(BigPointQ v, BigPointQ &v_orig);
};

inline OEdgeIter next_edge(OEdgeIter i) {
  FacePtr f = i->in_face();
  ++i;
  if (i == f->oedges.end()) {
    i = f->oedges.begin();
  }
  return (i);
}

inline OEdgeIter prev_edge(OEdgeIter i) {
  FacePtr f = i->in_face();
  if (i == f->oedges.begin()) {
    i = f->oedges.end();
  }
  --i;
  return (i);
}

inline OEdgePtrIter next_vert_edge(
    OEdgePtrIter i)  // returns next edge leaving head of *i
{
  VertexPtr v = (*i)->head();
  ++i;
  if (i == v->out_edges.end()) {
    i = v->out_edges.begin();
  }
  return (i);
}

inline OEdgePtrIter prev_vert_edge(OEdgePtrIter i) {
  VertexPtr v = (*i)->head();

  if (i == v->out_edges.begin()) {
    i = v->out_edges.end();
  }
  --i;
  return (i);
}

class Summary;
class SaddleConf;
class my_ostream;

class TwoComplex {
 public:
  TwoComplex();
  int nedges();
  int nfaces();
  int nvertices();
  void set_area();
  COORD get_area();
  COORD get_scale_factor();
  void set_scale_factor(COORD scale_factor);
  //    void Rescale(COORD scale);

  void ReadComplex(string filename);

  void BuildTriangle(int a, int b,
                     int c);  // adds a single triangle with angles
                              // pi*a/(a+b+c), pi*b/(a+b+c), pi*c/(a+b+c)
  void BuildQuad(int a, int b, int c,
                 int d);  // adds a quadrilateral with angles  2*pi*a/(a+b+c+d),
                          // 2*pi*b/(a+b+c+d), 2*pi*c/(a+b+c+d), 2*pi*d/(a+b+c+d)

  /*
  void BuildTriangleAlgebraic();  //Builds 5, 6, 7 triangle with algebraic edge
  vectors
  */

  void CompleteSurface();
  void
  SetupReflect();  // sets uedges.internal to true if boundary edges, labels
                   // oedges, sets from_edge to pair_edge for each uedge

  void PrintAll(ostream &);  // Prints data of vertices, edges, and faces
  void CheckAllFaces();
  void CheckAllVertices();
  void StatPrint(ostream &);
  void MakeDrawList();
  void ClearSegmentsToDraw();
  void AddPFace(FacePtr, Point, int);
  //    void Draw(ostream&);
  void NewDraw(my_ostream &);
  void Expunge();
  void
  BuildNeighborLists();  // Assign the list of outgoing UEdges vertex.e (whose
                         // head is vertex) and vertex.euclidean for each vertex.
  void PerturbAll(COORD max_perturb);
  void PerturbConjugates(COORD max_perturb);
  bool is_conjugate_deformation(int j, int k);

  UEdgePtr MaxEdge();  // returns pointer to longest edge
  UEdgePtr MinEdge();
  COORD MinSaddle(Dir<Point> &);

  void TriangulateFace(FacePtr f);
  void TriangulateAll();
  OEdgeIter BisectFace(FacePtr f, OEdgeIter i, OEdgeIter j);
  void AddSteinerPoint(FacePtr f); /* for debug */
  FacePtr RemoveVertex(VertexPtr v0);
  void RetriangulateSurface();

  UEdgePtr FlipEdge(UEdgePtr u);
  Point OpposingDiagonal(UEdgePtr u);
  bool shouldFlip(UEdgePtr u);
  void doExtraFlips();

  int64_t get_global_denom();
  void ClearDenominators();
  void StoreVertexOffsets();

  //    COORD Sweep(VertexPtr v0, COORD length);
  //    VertexPtr FollowDir(VertexPtr v0, OEdgePtrIter start_iter,
  //			Point c_vec,COORD len_2);

  template <typename PointT>
  int SweepNew(COORD length, Dir<PointT> start_dir, COORD GoalTotalAngle);
  void issueFinalReport(Summary &fsm, ostream &out, double part_done = 1.0,
                        double part_group = 1.0);
  template <typename PointT>
  VertexPtr SweepNextLeft(const Dir<PointT> &start, Dir<PointT> &end,
                          COORD len2, COORD threshold);
  template <typename PointT>
  void DrawSaddle(const Dir<PointT> &start, COORD len2, int id,
                  COORD cyl_length);
  COORD GetSaddles(Dir<Point> &start, COORD depth, int N);  // for RandomShoot
  COORD RandomShoot(VertexPtr v0, COORD depth, int M);      // not really used
  template <typename PointT>
  bool NewFollowDir(const Dir<PointT> &start, Dir<PointT> &end, COORD len2);
  template <typename PointT>
  void InvestigateVec(PointT vec, COORD len2, SaddleConf &sc, Summary &smry);
  template <typename PointT>
  void FindCrossSaddle(const Dir<PointT> &start, Dir<PointT> &cross_saddle);

  //    int TestSweep(VertexPtr v0, COORD depth);

  FacePtr ReflectFace(UEdgePtr e, FacePtr f);
  UEdgePtr IdentifyEdges(UEdgePtr e1, UEdgePtr e2);
  VertexPtr IdentifyVertices(VertexPtr v0,
                             VertexPtr v1);  // any UEdge pointing to v1 now
                                             // points to v0.  v1 is deleted.
  UEdgePtr FindMatchingEdge(UEdgePtr e);

  VertexPtr AddVertex(
      int id);  // creates new vertex with ID=cur_vertex_id if id=UNDEFINED or
                // ID=id otherwise. Increments cur_vertex_id or ssts it to id+1.
                // Appends new vertex pointer to vertices.
  UEdgePtr AddUEdge(
      int id, VertexPtr v0, VertexPtr v1,
      BigPointQ vec);  // Creates new UEdge from v0 to v1 with holonomy vec,
                       // ID=id or cur_uedge_id if id=UNDEFINED.
  UEdgePtr AddUEdge(int id, VertexPtr v0, VertexPtr v1, Point vec);
  FacePtr AddFace(int id,
                  list<OEdge>);  // Adds new face with oedges= list<OEdge>

  UEdgePtr GetUEdge(int id);
  VertexPtr GetVertex(int id);
  FacePtr GetFace(int id);

  PFace *get_pface(FacePtr f);
  bool can_merge(UEdgePtr ue);
  void relocate(int i, Point offset);
  void merge(UEdgePtr ue);
  void make_pcomplexes();
  void make_pface(FacePtr f);

  list<VertexPtr> vertices;
  list<FacePtr> faces;
  list<UEdgePtr> uedges;

  void InitCyclotomicField(int n);  // Set F to Q(primitive nth root of unity)
  void check_algebraicQ();          // check that algebraic vectors match
                            // numerical counterparts
  void check_algebraicI();  // check that algebraic interger versions of
                            // edge vectors match numerical counterparts
 private:
  int cur_face_id;
  int cur_vertex_id;
  int cur_uedge_id;
  COORD area;
  COORD scale_factor;

  int VISIT_ID;

  DrawList dl;
  void MakeDrawListInternal(FacePtr, Point);
  COORD SweepInternal(FacePtr cf, Point cf_offset, OEdgeIter c_edge,
                      Point c_vec, Point upper_bound_vec, COORD len_2,
                      OEdgePtrIter start_iter, VertexPtr v0);

  void CompleteInternal(UEdgePtr e);
  void SetInternalEdges();

  void read_edge_line(FILE *fp, int line_number);
  void read_face_line(FILE *fp, int line_number);
};

#include "psimplex.h"

template <typename PointT>
class Dir {
 public:
  PointT vec;
  VertexPtr v;
  OEdgePtrIter ep;

  Dir();
  explicit Dir(OEdgeIter e);
  Dir(VertexPtr vp, const PointT &p);
  void Check();
  Point vec_cx();
  alg_tI vec_algtI();

  Dir RotateCwToVec(PointT p);
  Dir RotateCCwToVec(PointT p);

  Dir RotateF(COORD theta);
  Dir RotateF_general(COORD theta);
  Dir RotateI(COORD theta);
  COORD AngleF(Dir &d2);
  int AngleI(Dir &d1, Dir &d2); /* returns angle over pi */
};

#include "saddleconf.h"

extern void reflect_vector(BigPointQ p, BigPointQ vec, BigPointQ &reflection);
// extern void reflect_vector_algebraicQ(Point p, algebraicQ p_alg, Point vec,
// algebraicQ vec_alg, Point &reflection, algebraicQ &reflection_alg);
extern void reflect_point(const Point &p, const Point &base, const Point &vec,
                          Point &reflection);
extern int test_congruent(OEdgeIter e0, OEdgeIter e1);
// extern COORD Dist(Point, Point);
extern COORD angle(Point, Point);
extern COORD angle(const BigPointI &, const BigPointI &);
extern COORD true_angle(Point, Point);
extern bool intersect_segment(Point pb, Point pv, Point qb, Point qv,
                              Point &intersection);
extern bool intersect_segment_interior(Point pb, Point pv, Point qb, Point qv,
                                       Point &intersection);
extern bool CCW3(const Point &p, const Point &q, const Point &r);
extern bool CCW3(const BigPointI &p, const BigPointI &q, const BigPointI &r);

extern bool CCW(const Point &p, const Point &q);
extern bool CCW(const BigPointI &p, const BigPointI &q);

// extern COORD min(COORD a, COORD b);

extern COORD norm(const BigPointI &p);
extern bool colinear(const Point &p, const Point &q);
extern bool colinear(const BigPointI &p, const BigPointI &q);
extern bool aligned(const Point &p, const Point &q);
extern bool aligned(const BigPointI &p, const BigPointI &q);
extern COORD area(const Point &p, const Point &q);
extern COORD d_point_line2(const Point &p, const Point &line);
extern COORD d_point_line2(const BigPointI &p, const BigPointI &line);

extern int gcd(int a, int b);
extern COORD det3(const vector<COORD> &x, const vector<COORD> &y,
                  const vector<COORD> &z);
extern bool in_circle(Point a, Point b, Point c, Point d);

/* inlines */

#ifdef USE_QUAD
inline int round_to_int(float128 v) {
  v = v + v.sign() / 2.0;
  return floor(v).convert_to<int>();
}
#elif defined USE_LONG_DOUBLE
inline int round_to_int(long double v) { return (int)rint(v); }
#else
inline int round_to_int(double v) { return (int)rint(v); }
#endif

inline COORD min(COORD a, COORD b) {
  if (a < b) return (a);

  return (b);
}

inline COORD Dist2(Point p, Point q) { return (norm(p - q)); }

inline COORD Dot(Point p, Point q) {
  return (p.real() * q.real() + p.imag() * q.imag());
}

#include "globals.h"

/* exceptions */

struct vert_index_taken {};
struct vert_bad_angle {
  COORD x;
  explicit vert_bad_angle(COORD q) : x(q) {}
};

#endif  // LIBPOLYGON_TWOCOMPLEX_H
