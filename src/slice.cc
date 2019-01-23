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

#ifdef USE_MPI

#include "libpolygon/slice.h"

CSliceArray SA; 
CNodeArray NA;
Summary fsm;


CNode::CNode() 
{

    NStatus = N_FREE; 

}


CNodeArray::CNodeArray()
{

    for(int  i =0; i < MAX_NODES; i++ ) {
	request_recv0[i] = MPI_REQUEST_NULL; 
	request_send0[i] = MPI_REQUEST_NULL; 
    }

    //finish


}

CSlice::CSlice() 
{

    status = S_UNASSIGNED; 
    handling_node = N_UNDEFINED; 

}


CSliceArray::CSliceArray() 
{
}

void CSliceArray::init() 
    for(int i = 0; i < n_slices; i++ ) {
	slice[i].status = S_UNASSIGNED; 
	unassigned.push_back(i);
    }

}

bool CSliceArray::have_unassigned()
{
    return( ! unassigned.empty()  );
}

int CSliceArray::n_unassigned()
{
    return( unassigned.size()  );
}

int CSliceArray::n_in_progress()
{
    return( in_progress.size()  );
}

int CSliceArray::n_finished() 
{
    return ( n_slices - n_in_progress() - n_unassigned()); 
}



bool CSliceArray::all_finished()
{

    if( unassigned.empty() && in_progress.empty() ) {
	return(true); 
    }
    return(false); 
}

int CSliceArray::assign_next_unassigned(int n_node)
{
    
    if( n_node <= 0 || n_node > numprocs ) {
	ERR_RET("assign: bad n_node"); 
    }

    if( all_finished() ) {
	ERR_RET("assign: no unfinished left"); 
    }


    int i = unassigned.front();

    out_s << "Assigning slice " << i << " to node " << n_node <<endl<<flush;
    slice[i].status = S_IN_PROGRESS; 
    slice[i].handling_node = n_node; 
    in_progress.push_back(i); 
    unassigned.pop_front(); 

    NA.node[n_node].NStatus = N_WORKING; 
    NA.node[n_node].my_slice = i; 
    NA.node[n_node].send_buf0[0] = i;

    // timestamp


    //MPI SEND (non-blocking).....  

    MPI_Issend(NA.node[n_node].send_buf0,1, 
	       MPI_INT, n_node, TAG_SLICENUM,
	       MPI_COMM_WORLD, &(NA.request_send0[n_node])); 


    //call Irecv on the node


    MPI_Irecv(NA.node[n_node].recv_buf0, 1, mpi_smry_type, n_node, 
	      MPI_ANY_TAG, MPI_COMM_WORLD, &(NA.request_recv0[n_node]));  
	      

    return(n_node);

}

void CNode::complete()
{

    NStatus = N_FREE;
    //handle timestamp
    int i = my_slice; 
    SA.set_finished(i); 
    my_slice = S_UNDEFINED; 

    int k =1; 

    Summary* rsm = (Summary *)recv_buf0; 
    rsm->merge(fsm);
}

void CSliceArray::set_finished(int i)
{

    slice[i].status = S_FINISHED; 
    in_progress.remove(i); 
}


void CNodeArray::init0()
{

    //node = 0 only
 
    SA.init(); 

    for(int i=1; i < numprocs; i++ ) {
	node[i].recv_buf0 = (char *)malloc(sizeof(Summary)+1); 
    }

    //assign initial slices
    for(int i =1; i < numprocs; i++ ) {
	SA.assign_next_unassigned(i); 
    }


}


void CNodeArray::main_loop0()
{

    //only for node 0

    int n_node; 
    MPI_Status status; 

    while( ! SA.all_finished() ) {

	MPI_Waitany(numprocs, request_recv0, &n_node, &status); 

	out_s <<"Message from node " << n_node 
	      <<" aka node " << status.MPI_SOURCE
	      <<" tag = " << status.MPI_TAG << flush; 


	if( n_node == 0 ) {
	    ERR_RET("Message from node 0 to itself"); 
	}
	if( n_node != status.MPI_SOURCE ) {
	    ERR_RET("message from wrong node"); 
	}
	if( status.MPI_TAG < TAG_DATA ) {
	    ERR_RET("waitany: bad tag"); 
	}
	out_s << " slice " << status.MPI_TAG - TAG_DATA << endl << flush; 

	node[n_node].complete(); //this includes merge	
	
	if( SA.have_unassigned() ) {
	    SA.assign_next_unassigned(n_node);
	}


    //check for timeouts
    }
    finish0();
}

void CNodeArray::finish0()
{

    //send FINISH message to all nodes; 
    //except the ones which timed out
    //or maybe just mpi_abort...

    out_s << "Sending finish messages..." << flush; 

    for(int i=1; i < numprocs; i++) {

	MPI_Issend(NA.node[i].send_buf0,1, MPI_INT, i, TAG_FINISH,
		  MPI_COMM_WORLD, &(NA.request_send0[i])); 

    }
 
    //maybe wait??
    MPI_Waitall(numprocs-1, request_send0+1, send_status_array0+1); 
    out_s << "done" << endl << flush; 
    
    out_s << "trying to call Barrier" << endl << flush; 
    MPI_Barrier(MPI_COMM_WORLD); 
    out_s << "trying to call finilize" << endl << flush; 
    MPI_Finalize();
}




void CNodeArray::finish1()
{

    //this should just be mpi_finalize
    out_s << "Trying to call barrier" << endl << flush; 
    MPI_Barrier(MPI_COMM_WORLD); 
    out_s << "Trying to call finalize" << endl << flush; 
    MPI_Finalize();

}

void CNodeArray::main_loop1()
{

    //for nodes other then node 0


    //synchronous recieve for messages from node 0
    while(1) {
	MPI_Recv(recv_buf1, 1, MPI_INT, 0, MPI_ANY_TAG, 
		 MPI_COMM_WORLD, &recv_status1); 


	out_s << "Message from " << recv_status1.MPI_SOURCE
	      << " tag = " << recv_status1.MPI_TAG;
    
	int slice_num = recv_buf1[0]; 

	out_s << " slice_num = " << slice_num << endl; 

	if( recv_status1.MPI_TAG == TAG_FINISH ) {
	    break; 
	}
	if( recv_status1.MPI_TAG != TAG_SLICENUM ) {
	    ERR_RET("Illegal Tag: aborting");
	}
	VertexPtr v0 = S->GetVertex(start_vertex); 
	Dir start_dir = SA.get_dir(v0, slice_num); 
	
	smry.clear(); 
	out_s << "Sweeping from " << start_dir.vec 
	      << " for " << v0->total_angle()/n_slices << " depth = " 
	      << depth << " follow_depth = " << follow_depth << endl<< flush; 

	S->SweepNew(depth, start_dir, v0->total_angle()/n_slices); 

	out_s << "finished computing slice" << slice_num << endl; 
	S->issueFinalReport(smry,out_s,1.0/n_slices);
	out_s << flush; 

	//synchonously send the summary to node 0
	MPI_Send(&smry, 1, mpi_smry_type, 0, TAG_DATA+slice_num, MPI_COMM_WORLD); 
	
    }

    finish1(); 

}

void CNodeArray::init1()
{

    //for nodes other then node 0

    SA.init(); 

    //set up send buffer...???

}

Dir CSliceArray::get_dir(VertexPtr v0, int n_slice)
{

	Dir start_dir = Dir(v0, Point(1,0.0000123));
	COORD GoalTotalAngle = v0->total_angle(); 

	GoalTotalAngle = 1.0*GoalTotalAngle/n_slices;
	Dir tmp = start_dir;
	tmp.RotateF_general(n_slice*GoalTotalAngle,start_dir);
	return(start_dir);
}

#endif        //ifdef USE_MPI
