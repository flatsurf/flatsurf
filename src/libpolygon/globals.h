/**********************************************************************
 *  This file is part of Polygon.
 *
 *        Copyright (C) 2018 Alex Eskin
 *        Copyright (C) 2019 Vincent Delecroix
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

#ifndef LIBPOLYGON_GLOBALS_H
#define LIBPOLYGON_GLOBALS_H

#include <fstream>
#include <list>
#include <random>

#include "libpolygon/defs.h"
#include "libpolygon/oedge.h"

/*
 * This file contains global variables that are set by the command line
 * interface. Eventually this file should be empty. There should be no
 * global state.
 */

namespace polygon {
class TwoComplex;
class Summary;
class OEdge;
class UEdge;
class Vertex;
class Face;

/* command line options */

extern int start_vertex;
extern int end_vertex;
extern bool perturb_euclidean;
extern bool perturb_all;
extern bool perturb_conjugate;
extern COORD depth;
extern int perturb_vertex;
extern Point offset;
extern COORD DELTA;
extern int mc_number;
extern bool new_sweep;
extern int mc_group;
extern int max_priority;
extern COORD follow_depth;
extern int verbose;
extern bool quiet;
extern bool retriangulate;
extern bool norescale;
extern bool individual;
extern bool norescale;
extern unsigned int random_seed;
extern int nice_inc;
extern int n_slices;
extern bool show_lengths;
extern bool show_cyls;
extern bool heights_and_twists;
extern bool closure;
extern bool allow_long_cyls;
extern bool cyls_only;
extern bool show_moduli;
extern bool no_field_arithmetic;
extern bool tikz_output;
extern bool show_length_list;
extern bool draw_cylinders;
extern bool draw_saddles;
extern int draw_tag;
extern COORD draw_saddle_length;
extern bool billiard_mode;
extern COORD EPSILON;

/* global variables */
extern bool field_arithmetic;
extern bool int_field_arithmetic;
extern Summary smry;
extern TwoComplex* S;
extern FILE* out_f;
extern std::ofstream out_stream;
extern std::ofstream out_s;
extern bool too_close_flag;
extern bool draw_cylinders;

extern char filename_[];
extern COORD perturb_magnitude;

/* to get around compiler changes */
/* setting iterators to NULL no longer allowed, so have to do this */
extern std::list<OEdge> stupid_OEdge_list;
extern std::list<typename std::list<OEdge>::iterator> stupid_OEdgePtr_list;
extern std::list<Vertex*> stupid_VertexPtr_list;
extern std::list<UEdge*> stupid_UEdgePtr_list;
extern std::list<Face*> stupid_FacePtr_list;

extern std::list<OEdge>::iterator NULL_OEdgeIter;
extern std::list<std::list<OEdge>::iterator>::iterator NULL_OEdgePtrIter;
extern std::list<Vertex*>::iterator NULL_VertexPtrIter;
extern std::list<UEdge*>::iterator NULL_UEdgePtrIter;
extern std::list<Face*>::iterator NULL_FacePtrIter;

#ifdef USE_PARALLEL
extern int numprocs;
extern int my_id;
#endif

#ifdef USE_MPI
extern void my_mpi_init();
extern MPI_Datatype mpi_smry_type;
extern MPI_Op mpi_merge;
#endif
}  // namespace polygon

#endif  // LIBPOLYGON_GLOBALS_H
