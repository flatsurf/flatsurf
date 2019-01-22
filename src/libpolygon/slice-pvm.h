#ifndef LIBPOLYGON_SLICE_PVM_H
#define LIBPOLYGON_SLICE_PVM_H

#include "libpolygon.h"
#include "two_complex.h"
#include <time.h>


#define MAX_NODES 600
#define MAX_SLICES 6000

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
#define N_UNINITIALIZED 4
#define N_SPAWNING 5


// if change this, change also tagname[] in slice-pvm.cc
#define TAG_SLICENUM 1 
#define TAG_FINISH 2
#define TAG_INIT 3
#define TAG_ACK 4
#define TAG_TASKDIED 5
#define TAG_NODEDEAD 6
#define TAG_DATA 7



extern char hostname[]; 

extern Summary fsm; 

class CNode {

public:

    CNode(); 
    void retire(); // retire this node
    void complete(); 
    double average_time;

    int NStatus; 
    time_t timestamp; 
    int my_slice; /* the slice we are now computing */
    int n_done; /* number of slices done already */
    char hostname[MAX_HOSTNAME]; 
    int node_num; /* my index in the array */
};

    ostream& operator<<( ostream& out, CNode& node);



class CNodeArray {

public: 

    CNodeArray(); 

    CNode node[MAX_NODES];

    /*number of tasks to spawn */
    int ntasks;
    
    /*number of hosts in the PVM */
    int nhosts;

    /* return code from pvm calls */
    int info;
    /* my task id */
    int mytid;
    /* my parents task id */
    int myparent;
    /* children task id array */
    int child[MAX_NODES];

//    char recv_buf0[sizeof(Summary)+1]; 

    void spawn(int& argc,char** &argv);

    int get_node(int tid); 
    void init0();
    void init1(); 
    void main_loop0(); 
    void main_loop1(); 
    void finish0();
    void finish1(); 
    void print_status(ostream& out);

};





class CSlice {

public:

    CSlice(); 

    int get_status(); 

    int status; 
    int handling_node; 

};


class CSliceArray {
    friend class CNode; 

public:

    CSliceArray();  
    void init(); /* be sure to init the lists correctly */


    int assign_next_unassigned(int n_node); 
    void assign(int n_node, int n_slice);
    int assign_to_slowest(int n_node); 

    bool all_finished(); 
    bool have_unassigned(); 
    void set_finished(int n_slice); 

    Dir get_dir(VertexPtr v0, int n_slice); 

    CSlice    slice[MAX_SLICES];

    int n_unassigned(); 
    int n_in_progress();
    int n_finished(); 

private:
    list<int> unassigned;
    list<int> in_progress;

};

extern CSliceArray SA; 
extern CNodeArray NA; 

#endif // LIBPOLYGON_SLICE_PVM_H
