#ifdef USE_PVM

#include "libpolygon/slice-pvm.h"
#include <string.h>

const char* tagname[] = {
    "TAG_DUMMY", 
    "TAG_SLICENUM",
    "TAG_FINISH",
    "TAG_INIT",
    "TAG_ACK",
    "TAG_TASKDEAD",
    "TAG_NODEDEAD",
    "TAG_DATA",
    "TAG_TASKDIED"
};


Summary fsm;
CSliceArray SA; 
CNodeArray NA;



CNode::CNode() 
{

    NStatus = N_UNINITIALIZED; 
    n_done = 0; 
    average_time = 1E15; 
    hostname[0] = '\0';

}

ostream& operator<<( ostream& out, CNode &n)
{
    out << n.node_num <<"(" <<n.hostname << ",  " << n.n_done << ")";
    return(out); 
}

CNodeArray::CNodeArray()
{

    for(int i =0; i < MAX_NODES; i++ ) {
	node[i].node_num = i; 
    }
    
    struct pvmhostinfo *hostp;
    int narch;

    info = pvm_config( &nhosts, &narch, &hostp );
    for (int i = 0; i < nhosts; i++) {
	strncpy(node[i].hostname,hostp[i].hi_name,MAX_HOSTNAME);
	node[i].hostname[MAX_HOSTNAME-1] = '\0';
//	printf("i= %d, %s\n", i, hostp[i].hi_name);
    }

    //finish

}


int CNodeArray::get_node(int tid)
{
    for(int i =0; i < ntasks; i++ ) {
	if( child[i] == tid ) {
	    return(i+1); 
	}
    }
    return(-1); 
}



void CNodeArray::spawn(int& argc, char** &argv)
{

    /* number of tasks to spawn, use 3 as the default */
    ntasks = 3;
    numprocs = ntasks + 1; 


    printf("in spawn\n"); 
    fflush(stdout); 

    int i, mydata, buf, len, tag, tid;

    /* find out my task id number */
    mytid = pvm_mytid();

    /* check for error */
    if ( mytid < 0) { 
        /* print out the error */
	perror("Cannot find my tid"); 
        /* exit the program */ 
        exit(-1);
        }
    /* find my parent's task id number */
    myparent = pvm_parent();

    /* exit if there is some error other than PvmNoParent */
    if (( myparent < 0) && ( myparent != PvmNoParent)) {
	printf("myparent = %d, PvmNoParent = %d\n", myparent, PvmNoParent); 
        pvm_perror("Cannot find parent");
        pvm_exit();
        exit(-1);
        }

    /* if i don't have a parent then i am the parent */
    if ( myparent == PvmNoParent) {
	my_id = 0; 
        /* find out how many tasks to spawn */
	if( argc < 4 ) {
	    pvm_exit();
	    printf("Usage: polygon -np 4 --depth=50 ....\n");
	    ERR_RET("not enough arguments"); 
	}

	if( strcmp(argv[1], "-np" ) != 0 ) {
	    pvm_exit(); 
	    printf("Usage: polygon -np 4 --depth=50 ....\n");
	    ERR_RET("for PVM operation, second argument must be -np");
	}

	numprocs = atoi(argv[2]); 
	ntasks = numprocs - 1;
	argv[2] = argv[0]; 
	argv = argv+2; 
	argc = argc-2; 




        /* make sure ntask is legal */
        if ((ntasks < 1) || (ntasks > MAX_NODES)) { 
	    pvm_exit(); 
	    ERR_RET("bad number of tasks"); 
	}

        /* spawn the child tasks */
	char *basename = strrchr(argv[0],'/'); 
	if( basename == NULL ) {
	    basename = argv[0];
	} else {
	    basename = basename +1; 
	}
	int j = 0; 
	for( i =0; i < ntasks; i++ ) {
	    j++; 
	    if( j == nhosts) {
		j = 0; 
	    }
	    
	    printf("Spawning node %d on %s ", i+1, node[j].hostname); 
	    if( j != i+1 ) {
		strncpy(node[i+1].hostname,node[j].hostname,MAX_HOSTNAME);
	    }
	    info = pvm_spawn(basename, argv+1, PvmTaskHost|PvmTaskTrace, node[j].hostname,
			     1, child+i);

	    /* print out the task id */
            if ( child[i] < 0) /* print the error code in decimal*/
                printf("error %d\n", child[i]);
            else  /* print the task id in hex */
                printf("tid t%x\n", child[i]);
	    
	    /* make sure spawn succeeded */
	    if ( info != 1 ) { 
		pvm_perror("failed to spawn task"); 
		pvm_exit(); exit(-1); 
	    }
	}

	/* ask for notification when child exits */
        info = pvm_notify(PvmTaskExit, TAG_TASKDIED, ntasks, child);
        if (info < 0) { 
	    pvm_perror("notify"); 
	    pvm_exit(); 
	    exit(-1); 
	}

	/* send messages to children with their ranks */
	for(i=0; i <  ntasks; i++ ) {
	    info = pvm_initsend(PvmDataDefault);
	    if ( info < 0) {
		pvm_perror("calling pvm_initsend"); pvm_exit(); exit(-1);
	    }
	    int j = i+1; 
	    node[j].NStatus = N_SPAWNING; 
	    info = pvm_pkint(&j, 1, 1);
	    if (info < 0) {
		pvm_perror("calling pvm_pkint"); pvm_exit(); exit(-1);
	    }
	    info = pvm_pkint(&numprocs,1,1); 
	    if (info < 0) {
		pvm_perror("calling pvm_pkint"); pvm_exit(); exit(-1);
	    }
	    info = pvm_send(child[i], TAG_INIT);
	    if (info < 0) {
		pvm_perror("calling pvm_send"); pvm_exit(); exit(-1);
	    }

	}

	/* wait for ACK's from children */
	printf("Waiting for ACKS\n"); 
	for (i = 0; i < ntasks; i++) {
            /* recv a message from any child process */
	    struct timeval t;
	    t.tv_sec = 60;
	    t.tv_usec = 0; 
            buf = pvm_recv(-1, -1); 
            if (buf < 0) pvm_perror("calling recv");
	    if ( buf == 0 ) {
		printf("timed out waiting for ACKs"); 
		for(int k = 0; k < ntasks; k++ ) {
		    int m = k+1;
		    printf("Node: %d, tid: t%x, hostname: %s, status: %d\n",
			   m, child[k], node[m].hostname, node[m].NStatus); 
		}

		// FINISH: later retire bad nodes and go on....
		pvm_exit(); 
		ERR_RET("Timed out waiting for ACKs"); 
	    }
            info = pvm_bufinfo(buf, &len, &tag, &tid);
            if (info < 0) pvm_perror("calling pvm_bufinfo");
	    if( tag == TAG_ACK ) {
		info = pvm_upkint(&mydata, 1, 1);
		if (info < 0) pvm_perror("calling pvm_upkint");
		printf("Length %d, Tag %d, Tid t%x, node %d", 
		       len, tag, tid,mydata);
		if (child[mydata-1] != tid) {
		    printf("This should not happen!\n");
		    pvm_exit(); 
		    ERR_RET("child[mydata-1] != tid"); 
		}
		node[mydata].NStatus = N_FREE; 

		info = pvm_upkstr(node[mydata].hostname); 
		if (info < 0) pvm_perror("calling pvm_upkstr");
		printf(" %s\n",node[mydata].hostname); 

	    } else if (tag == TAG_TASKDIED ) {
		printf("Task died...");
		int deadtid; 
		NA.info = pvm_upkint(&deadtid, 1, 1);
		if (info < 0) pvm_perror("calling pvm_upkint");
		printf("tid: t%x", deadtid); 
		int k = get_node(deadtid); 
		if( k < 0 ) {
		    pvm_exit(); 
		    ERR_RET("bad tid");
		}
		printf(" node = %d, status = %d, hostname = %s\n", k, 
		       node[k].NStatus, node[k].hostname); 

		//FINISH: retire node k

	    } else {
		printf("unknown tag t%x\n", tag);
		pvm_exit(); 
		ERR_RET("bad tag"); 
	    }


	}
	    

    } else { /* I am a child */

	/* recv a message from the parent process */
	buf = pvm_recv(myparent, TAG_INIT);
	if (buf < 0) pvm_perror("calling recv for TAG_INIT from parent");
	info = pvm_bufinfo(buf, &len, &tag, &tid);
	if (info < 0) pvm_perror("calling pvm_bufinfo");
	info = pvm_upkint(&my_id, 1, 1);
            if (info < 0) pvm_perror("calling pvm_upkint");
	info = pvm_upkint(&numprocs, 1, 1);
            if (info < 0) pvm_perror("calling pvm_upkint");
	    
	/* send ack message to parent */
	info = pvm_initsend(PvmDataDefault);
	if (info < 0) {
	    pvm_perror("calling pvm_initsend"); pvm_exit(); exit(-1);
	}
	info = pvm_pkint(&my_id, 1, 1);
	if (info < 0) {
	    pvm_perror("calling pvm_pkint"); pvm_exit(); exit(-1);
	}
	info = pvm_pkstr(hostname); 
	if (info < 0) {
	    pvm_perror("calling pvm_pkstr"); pvm_exit(); exit(-1);
	}

	info = pvm_send(myparent, TAG_ACK);
	if (info < 0) {
	    pvm_perror("calling pvm_send TAG_ACK"); pvm_exit(); exit(-1);
	}
    }
    return; 
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
{
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
    in_progress.push_back(i); 
    unassigned.pop_front(); 
    slice[i].handling_node = n_node; 
    assign(n_node, i);
    return(i);
}

int CSliceArray::assign_to_slowest(int n_node)
{

    if( n_node <= 0 || n_node > numprocs ) {
	pvm_exit(); 
	ERR_RET("assign: bad n_node"); 
    }
    out_s << "------------------------------------" << endl << flush;


    out_s << "Node " << NA.node[n_node] << " avg_time = " 
	  << NA.node[n_node].average_time << " is idle" << endl << flush;



    list<int>::iterator candidate;
    const double VERY_LONG_TIME=1E16;
    double slowest_avg_time = VERY_LONG_TIME; 

    for(list<int>::iterator j = in_progress.begin();j!=in_progress.end();j++){ 
	int i = slice[*j].handling_node; 
	if( i == n_node ) {
	    continue; 
	}

	if( NA.node[i].n_done == 0 || 
	    NA.node[i].average_time < slowest_avg_time) {
	    
	    candidate = j;
	    slowest_avg_time = NA.node[i].average_time;
	}
    }
    out_s << "Slowest in progress slice: " << *candidate 
	  << " handled by node " << slice[*candidate].handling_node
	  << " ....." << flush; 
    if( NA.node[n_node].n_done > 0 &&
	NA.node[n_node].average_time < slowest_avg_time ) {
	
	out_s << "we are faster" << endl << flush; 
	slice[*candidate].handling_node = n_node;
    } else {
	out_s << "we are slower" << endl << flush; 
    }
//    out_s << "Slowest slice is " << *candidate << " handled by node " 
//	  << NA.node[slice[*candidate].handling_node] 
//	  << endl <<flush; 

    out_s << "------------------------------------" << endl << flush;
    assign(n_node, *candidate);


    return(*candidate);
}

void CSliceArray::assign(int n_node, int i)
{


    out_s << "Assigning slice " << i << " to node " <<
	NA.node[n_node] << endl << flush;
    out_s << "------------------------------------" << endl << flush;
    slice[i].status = S_IN_PROGRESS; 



    NA.node[n_node].NStatus = N_WORKING; 
    NA.node[n_node].my_slice = i; 

    NA.node[n_node].timestamp = time(NULL); 


    NA.info = pvm_initsend(PvmDataDefault);
    if ( NA.info < 0) {
	pvm_perror("calling pvm_initsend"); pvm_exit(); exit(-1);
    }
    NA.info = pvm_pkint(&i, 1, 1);
    if (NA.info < 0) {
	pvm_perror("calling pvm_pkint"); pvm_exit(); exit(-1);
    }
    NA.info = pvm_send(NA.child[n_node-1], TAG_SLICENUM);
    if (NA.info < 0) {

	//FINISH HANDLE SOME ERRORS HERE....

	pvm_perror("calling pvm_send"); pvm_exit(); exit(-1);
    }

    return;

}

void CNode::complete()
{

    NStatus = N_FREE;
#ifndef sun4m
    double this_time = difftime(time(NULL),timestamp);
#else
    double this_time = time(NULL) - timestamp; 
#endif
    average_time = (n_done*average_time + this_time)/(n_done+1);
    n_done++; 



    int i = my_slice;
    if ( SA.slice[i].status == S_IN_PROGRESS ) {
	SA.set_finished(i); 
	my_slice = S_UNDEFINED; 

	smry.unpack(); 
	smry.merge(fsm);
	out_s << "this time = " << this_time 
	      << " average_time = " << average_time << endl << flush;  
    } else if ( SA.slice[i].status == S_FINISHED ) {
	out_s << "slice was already finished" << endl << flush; 
    } else {
	out_s << "***WARINING: BAD SLICE STATUS: " <<  SA.slice[i].status
	      << endl << flush; 
    }
}

void CNode::retire()
{

    if( NStatus == N_WORKING ) {
	    SA.unassigned.push_front(my_slice); 
	    SA.in_progress.remove(my_slice); 
	    SA.slice[my_slice].status = S_UNASSIGNED; 
	    SA.slice[my_slice].handling_node = N_UNDEFINED; 
    }
    NStatus = N_TIMED_OUT;
    
    //FINISH perhaps kill the process? 

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

    //assign initial slices
    for(int i =1; i < numprocs; i++ ) {
	if( node[i].NStatus == N_FREE ) {
	    SA.assign_next_unassigned(i); 
	}
    }


}

void CNodeArray::print_status(ostream& out)
{

    for(int i =1; i < numprocs; i++ ) {
	char tid_str[200];
	sprintf(tid_str, "%-5d %-30s t%-10x S%-3d N%-3d %-g",
		node[i].n_done,
		node[i].hostname,
		child[i-1], 
		node[i].NStatus,
		i,
		node[i].average_time);
	out << tid_str << endl;
    }
}

void CNodeArray::main_loop0()
{

    //only for node 0

    int n_node; 
    out_s << "in_main_loop0" << endl << flush; 

    while( ! SA.all_finished() ) {

        /* recv a message from any child process */
	int buf = pvm_recv(-1, -1);
	if (buf < 0) {
	    pvm_perror("calling recv");
	}

	int len, tag, tid; 
	NA.info = pvm_bufinfo(buf, &len, &tag, &tid);

	if (NA.info < 0) pvm_perror("calling pvm_bufinfo");

	if( tag == TAG_TASKDIED ) {
	    int deadtid; 
	    NA.info = pvm_upkint(&deadtid, 1, 1);
	    if (info < 0) pvm_perror("calling pvm_upkint");
	    int j = get_node(deadtid); 
	    if( j < 0 ) {
		pvm_exit(); 
		ERR_RET("bad tid");
	    }

	    out_s << "***** RETIRING NODE "  
		  << NA.node[j]  << endl << flush; 

	    NA.node[j].retire();

	    continue; 
	    // FINISH: if last task dies, will never stop!!!
	}

	info = pvm_upkint(&n_node, 1, 1);
	if (NA.info < 0) pvm_perror("calling pvm_upkint");


	out_s <<"Message from node " << NA.node[n_node]  
	      << " tid " << tid 
	      <<" tag = " << tag <<"("<< tagname[tag] << ")" << flush; 

	if ( tid != child[n_node-1] ) {
	    out_s << "\n node # does not match tid\n" << flush;
	    pvm_exit(); 
	    exit(1); 
	}

	if( n_node == 0 ) {
	    ERR_RET("Message from node 0 to itself"); 
	}



	if( tag != TAG_DATA ) { //FIX LATER TO ALLOW ERROR TAGS
	    ERR_RET("waitany: bad tag"); 
	}
	int slice;
	info = pvm_upkint(&slice, 1, 1);
	if (info < 0) pvm_perror("calling pvm_upkint");

	out_s << " slice " << slice << endl << flush; 
	if ( slice != node[n_node].my_slice ) {
	    ERR_RET("computed the wrong slice"); 
	}

	node[n_node].complete(); //this includes summary unpack and merge	

	if( SA.n_finished() %100 == 0 ) {
	    ofstream hstream("host_stats");
	    print_status(hstream);
	    hstream.close();
	}

	
	if( SA.have_unassigned() ) {
	    SA.assign_next_unassigned(n_node);
	} else if ( ! SA.all_finished() ) {
	    SA.assign_to_slowest(n_node); 
	}


    //check for timeouts
    }
    finish0();
}

void CNodeArray::finish0()
{

    //send FINISH message to all nodes; 
    //except the ones which timed out

    out_s << "Sending finish messages..." << flush; 

    for(int i=0; i <  ntasks; i++ ) {
	if( node[i].NStatus != N_TIMED_OUT ) {
	    info = pvm_initsend(PvmDataDefault);
	    if ( info < 0) {
		pvm_perror("calling pvm_initsend"); pvm_exit(); exit(-1);
	    }
	    int j = i+1; 
	    info = pvm_pkint(&j, 1, 1);
	    if (info < 0) {
		pvm_perror("calling pvm_pkint"); pvm_exit(); exit(-1);
	    }
	    info = pvm_send(child[i], TAG_FINISH);
	    if (info < 0) {
		pvm_perror("calling pvm_send"); pvm_exit(); exit(-1);
	    }
	}
	
    }

    out_s << "done" << endl << flush; 
    pvm_exit(); 
    return; 
}




void CNodeArray::finish1()
{

    out_s << "Calling pvm_exit" << endl << flush; 
    pvm_exit(); 

}

void CNodeArray::main_loop1()
{

    //for nodes other then node 0

    out_s << "In main_loop1" << endl << flush; 


    //synchronous recieve for messages from node 0
    while(1) {

	/* recv a message from the parent process */
	int buf = pvm_recv(myparent, -1);
	if (buf < 0) pvm_perror("calling recv");
	int len, tag, tid;
	info = pvm_bufinfo(buf, &len, &tag, &tid);
	if (info < 0) pvm_perror("calling pvm_bufinfo");


	out_s << "Message from " << tid
	      << " tag = " << tag <<"(" << tagname[tag] <<")"
	      << endl <<flush; 

	if( tag == TAG_FINISH ) {
	    break; 
	}    

	if( tag != TAG_SLICENUM ) {
	    ERR_RET("Illegal Tag: aborting");
	}
	int slice_num; 
	info = pvm_upkint(&slice_num,1, 1); 
	if( info < 0 ) perror("calling pvm_unpack"); 

	out_s << " slice_num = " << slice_num << endl; 



	VertexPtr v0 = S->GetVertex(start_vertex); 
	Dir start_dir = SA.get_dir(v0, slice_num); 
	
	smry.clear(); 
	out_s << "Sweeping from " << start_dir.vec 
	      << " for " << v0->total_angle()/n_slices << " depth = " 
	      << depth << " follow_depth = " << follow_depth 
	      << " number of slices = " <<n_slices<< endl<< flush; 

	S->SweepNew(depth, start_dir, v0->total_angle()/n_slices); 

	out_s << "finished computing slice" << slice_num << endl; 
	S->issueFinalReport(smry,out_s,1.0/n_slices);
	out_s << flush; 

	//synchonously send the summary to node 0

	info = pvm_initsend(PvmDataDefault);
	if (info < 0) {
	    pvm_perror("calling pvm_initsend"); pvm_exit(); exit(-1);
	}
	info = pvm_pkint(&my_id, 1, 1);
	if (info < 0) {
	    pvm_perror("calling pvm_pkint"); pvm_exit(); exit(-1);
	}

	info = pvm_pkint(&slice_num, 1, 1);
	if (info < 0) {
	    pvm_perror("calling pvm_pkint"); pvm_exit(); exit(-1);
	}

	smry.pack();
	
	info = pvm_send(myparent, TAG_DATA);
	if (info < 0) {
	    pvm_perror("calling pvm_send"); pvm_exit(); exit(-1);
	}
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

#endif        //ifdef USE_PVM
