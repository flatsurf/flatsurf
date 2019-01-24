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

#include "libpolygon/two_complex.h"

/*
 * This file contains global variables that are set by the command line
 * interface. Eventually this file should be empty. There should be no
 * global state.
 */

int start_vertex = UNDEFINED;
int end_vertex = UNDEFINED;
bool perturb_euclidean = true;
bool perturb_all = false;
COORD perturb_magnitude = 0.0;
bool perturb_conjugate = false;
COORD depth = 0.0;
int perturb_vertex = UNDEFINED;
Point offset = Point(UNDEFINED, UNDEFINED);
int verbose = 0;
COORD DELTA = 1E-9;
COORD EPSILON = 1E-13;
char filename_[1000];
int mc_number = 0;
bool new_sweep = true;
int mc_group = 100;
int max_priority = 10;
COORD follow_depth = -1000.0;
bool quiet = false;
bool retriangulate = true;
bool norescale = false;
bool individual = false;
bool show_lengths = false;
bool show_cyls = false;
bool cyls_only = false;
bool allow_long_cyls = false;
bool heights_and_twists = false;
bool show_moduli = false;
int nice_inc = 0;
int n_slices = 100;
bool no_field_arithmetic = false;
bool closure = false;
bool tikz_output = false;
bool show_length_list = false;
bool draw_cylinders = false;
bool draw_saddles = false;
int draw_tag = -1;
COORD draw_saddle_length = 0.0;
bool billiard_mode = false;

/* global */
bool field_arithmetic = false;
bool int_field_arithmetic = false;
bool too_close_flag = false;
// bool draw_cylinders = false;

TwoComplex *S;
int random_seed;
FILE *out_f;

ostdiostream out_stream;

/* more global vars to get around compiler changes */
list<OEdge> stupid_OEdge_list;
list<OEdgeIter> stupid_OEdgePtr_list;
list<VertexPtr> stupid_VertexPtr_list;
list<UEdgePtr> stupid_UEdgePtr_list;
list<FacePtr> stupid_FacePtr_list;

OEdgeIter NULL_OEdgeIter = stupid_OEdge_list.end();
OEdgePtrIter NULL_OEdgePtrIter = stupid_OEdgePtr_list.end();
VertexPtrIter NULL_VertexPtrIter = stupid_VertexPtr_list.end();
UEdgePtrIter NULL_UEdgePtrIter = stupid_UEdgePtr_list.end();
FacePtrIter NULL_FacePtrIter = stupid_FacePtr_list.end();

#ifdef USE_PARALLEL
int numprocs;
int my_id;
#endif
