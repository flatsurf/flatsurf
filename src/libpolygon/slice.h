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

#ifndef LIBPOLYGON_SLICE_H
#define LIBPOLYGON_SLICE_H

#include "libpolygon.h"
#include "two_complex.h"
#include <time.h>

#define MAX_SLICES 3000
#define MAX_NODES 600

#define S_UNDEFINED -1000

#define N_UNDEFINED -2000


/* slice status */
#define S_UNASSIGNED 1
#define S_IN_PROGRESS 2
#define S_FINISHED 3






/*node status */
#define N_FREE 1
#define N_TIMED_OUT 2
#define N_WORKING 3


#define TAG_SLICENUM 1 
#define TAG_FINISH 2
#define TAG_DATA 3



extern Summary fsm; 

class CNode {

public:

    CNode(); 
    void time_out(); // retire this node
    void complete(); 


    int NStatus; 
    time_t timestamp; 
    time_t average_time; 
    int my_slice; /* the slice we are now computing */

    char *recv_buf0; /* should be malloc'd in node 0 */


    int send_buf0[1]; //should be big enough for an MPI_INT

    MPI_Status recv_status0; 
    MPI_Status send_status0; 

};



class CNodeArray {

public: 

    CNodeArray(); 

    CNode node[MAX_NODES];

    MPI_Request request_recv0[MAX_NODES]; 
    MPI_Request request_send0[MAX_NODES]; 
    
    MPI_Status send_status_array0[MAX_NODES]; // for WaitAll

    void init0();
    void init1(); 
    void main_loop0(); 
    void main_loop1(); 
    void finish0();
    void finish1(); 

    MPI_Status recv_status1; 
    int recv_buf1[1]; //should be big enough for an MPI_INT

//    MPI_Status send_status1; 

//    char send_buf1[sizeof(Summary)+1];

};





class CSlice {

public:

    CSlice(); 

    int get_status(); 

    int status; 
    int handling_node; 

};


class CSliceArray {

public:

    CSliceArray();  
    void init(); /* be sure to init the lists correctly */

    int assign_next_unassigned(int n_node); 
    bool all_finished(); 
    bool have_unassigned(); 
    void set_finished(int n_slice); 

    int n_unassigned(); 
    int n_in_progress();
    int n_finished(); 


    Dir get_dir(VertexPtr v0, int n_slice); 

    CSlice    slice[MAX_SLICES];

private:
    list<int> unassigned;
    list<int> in_progress;

};

extern CSliceArray SA; 
extern CNodeArray NA; 

#endif // LIBPOLYGON_SLICE_H
