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

#include "libpolygon.h"

/*
 * This file contains global variables that are set by the command line
 * interface. Eventually this file should be empty. There should be no
 * global state.
 */ 

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
extern int random_seed; 
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


/* global variables */
extern bool field_arithmetic;
extern bool int_field_arithmetic;
extern Summary smry;
extern TwoComplex* S;
extern int random_seed;
extern FILE* out_f; 
extern ostdiostream out_stream; 
extern ostdiostream out_s; 
extern bool too_close_flag;
extern bool draw_cylinders;


extern char filename_[];  
extern COORD perturb_magnitude; 

/* to get around compiler changes */
/* setting iterators to NULL no longer allowed, so have to do this */
extern  list<OEdge>      stupid_OEdge_list;
extern  list<OEdgeIter>  stupid_OEdgePtr_list;
extern  list<VertexPtr>  stupid_VertexPtr_list;
extern  list<UEdgePtr>   stupid_UEdgePtr_list;
extern  list<FacePtr>    stupid_FacePtr_list;

extern OEdgeIter NULL_OEdgeIter;
extern OEdgePtrIter NULL_OEdgePtrIter; 
extern VertexPtrIter NULL_VertexPtrIter;
extern UEdgePtrIter NULL_UEdgePtrIter;
extern FacePtrIter NULL_FacePtrIter;

#ifdef USE_PARALLEL
extern int numprocs;
extern int my_id;
#endif

#ifdef USE_MPI
extern void my_mpi_init(); 
extern MPI_Datatype mpi_smry_type; 
extern MPI_Op mpi_merge; 
#endif

#endif // LIBPOLYGON_GLOBALS_H
