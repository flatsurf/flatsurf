#include "two_complex.h"
#include "getopt.h"
#include <unistd.h>

#ifdef USE_MPI
#include "slice.h"
#endif

#ifdef USE_PVM
#include "slice-pvm.h"
#endif

int start_vertex = UNDEFINED;
int end_vertex = UNDEFINED;
bool perturb_euclidean = true;
bool perturb_all = false;
COORD perturb_magnitude = 0.0;
bool perturb_conjugate = false; 
COORD depth = 0.0;
int perturb_vertex = UNDEFINED;
Point offset = Point(UNDEFINED,UNDEFINED);
int verbose = 0;
COORD DELTA = 1E-9;
COORD EPSILON = 1E-13;
bool randomize = false;
char filename[1000];
bool file_arg  = false;
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
//bool draw_cylinders = false; 

TwoComplex *S; 
int random_seed;
FILE *out_f;

ostdiostream out_stream; 
ostdiostream out_s; 


/* more global vars to get around compiler changes */
list<OEdge>      stupid_OEdge_list;
list<OEdgeIter>  stupid_OEdgePtr_list;
list<VertexPtr>  stupid_VertexPtr_list;
list<UEdgePtr>   stupid_UEdgePtr_list;
list<FacePtr>    stupid_FacePtr_list;





OEdgeIter NULL_OEdgeIter = stupid_OEdge_list.end();
OEdgePtrIter NULL_OEdgePtrIter = stupid_OEdgePtr_list.end();
VertexPtrIter NULL_VertexPtrIter = stupid_VertexPtr_list.end();
UEdgePtrIter NULL_UEdgePtrIter = stupid_UEdgePtr_list.end();
FacePtrIter NULL_FacePtrIter = stupid_FacePtr_list.end();



#ifdef USE_PARALLEL
int numprocs;
int my_id; 
#endif

char hostname[MAX_HOSTNAME]; 


int main(int argc, char **argv)
{
    S = new TwoComplex();

// stream using a UNIX file descriptor
//  std::ofstream os;
//  __gnu_cxx::stdio_filebuf<char> fdbuf(1, std::ios::out);
//  os.std::ios::rdbuf(&fdbuf);




    __gnu_cxx::stdio_sync_filebuf<char> fdbuf(stdout);

    out_s.std::ios::rdbuf(&fdbuf);


    


    COORD t1, t2;


    gethostname(hostname, MAX_HOSTNAME-1); 


#ifdef USE_MPI
    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD,&my_id);

//    MPI_Errhandler_set(MPI_COMM_WORLD, MPI_ERRORS_RETURN);
    my_mpi_init();
#endif

#ifdef USE_PVM
    NA.spawn(argc,argv); 
#endif

#ifdef USE_PARALLEL
    // defaults
    n_slices = 50*(numprocs -1); 
    nice_inc = 15;
    mc_group = 3000; 

    //output log files
    char mpi_filename[100]; 
    sprintf(mpi_filename,"output_log.%d",my_id); 

    out_f = fopen(mpi_filename, "w"); 
    out_stream = new ostdiostream(out_f);
#else
#define MAX_SLICES 1
    out_f = stdout; 
    n_slices = 1; 
#endif

    out_s << "hostname = " << hostname << endl; 

    for(int i = 0; i < argc; i++ ) {
	out_s << argv[i] << " ";
    }
    out_s << endl; 






    int c;
//    int digit_optind = 0;

    while (1) {
//	int this_option_optind = optind ? optind : 1;
	int option_index = 0;
	static struct option long_options[] =
	{
	    {"delta", required_argument, NULL, 'a'},
	    {"retriangulate", no_argument, NULL, 'b'},
	    {"move_vertex", required_argument, NULL, 'c'},
	    {"depth", required_argument, NULL, 'd'},
	    {"end_vertex", required_argument, NULL, 'e'},
	    {"file", required_argument, NULL, 'f'},
	    {"group",required_argument, NULL, 'g'},

	    {"lock", no_argument, NULL, 'l'},
	    {"max_priority",required_argument, NULL, 'm'},
	    {"number",required_argument, NULL, 'n'},
	    {"offset",required_argument,NULL,'o'},
	    {"perturb", optional_argument, NULL, 'p'},
	    {"quiet", no_argument, NULL, 'q'},
	    {"randomize", no_argument, NULL, 'r'},
	    {"start_vertex", required_argument, NULL, 's'},
	    {"show_moduli", no_argument, NULL, 't'},
	    {"cyls_only", no_argument, NULL, 'u'},
	    {"verbose", optional_argument, NULL, 'v'},
	    {"new_sweep",no_argument, NULL, 'w'},
	    {"perturb_conjugate",optional_argument, NULL, 'x'},

	    {"follow_depth", required_argument, NULL, 'z'},
	    {"no_field_arithmetic", no_argument,NULL,'A'},
	    {"no_retriang", no_argument, NULL, 'B'},
	    {"closure", no_argument, NULL, 'C'},
	    {"allow_long_cyls", no_argument, NULL, 'D'},
	    {"epsilon", required_argument, NULL, 'E'},
	    {"show_length_list", no_argument, NULL, 'F'},
	    {"draw_saddles", required_argument, NULL, 'G'},
	    {"draw_cylinders", required_argument, NULL, 'H'},

	    
	    {"individual", no_argument, NULL, 'I'},

	    {"tag", required_argument, NULL, 'J'},
	    {"billiard_mode", no_argument, NULL, 'K'},
	    {"show_lengths", no_argument, NULL, 'L'},


	    {"nice", required_argument, NULL, 'N'},
	    {"tikz_output", no_argument,NULL,'P'},

	    {"norescale", no_argument, NULL, 'R'},
	    {"slices", required_argument, NULL, 'S'},

	    {"show_cyls", no_argument, NULL, 'Y'},
	    {"heights_and_twists", no_argument, NULL, 'Z'},
	    {0, 0, 0, 0}
	};

	c = getopt_long (argc, argv, "f:s:e:d:v::lp::o:ti",
			 long_options, &option_index);
	if (c == -1)
            break;

	switch (c)
	{
	case 's':
	    if( optarg ) {
		start_vertex = atoi(optarg);
	    } else
		ERR_RET("start_vertex: no arg");
	    break;
	    
	case 'e':
	    if (optarg ) {
		end_vertex = atoi(optarg);
	    } else
		ERR_RET("end_vertex: no arg");
	    break;

	case 'd':
	    if (optarg ) {
		depth = atof(optarg);
	    } else
		ERR_RET("depth: no arg");
	    break;

	case 'z':
	    if (optarg ) {
		follow_depth = atof(optarg);
	    } else
		ERR_RET("follow_depth: no arg");
	    break;


	case 'v':
	    verbose = 1;
	    if( optarg ) { 
		verbose = atoi(optarg);
	    }
	    break;


	case 'l':
	    perturb_euclidean = false;
	    break;

	case 'r':
	    randomize = true;
	    if ( optarg ) {
		random_seed = atoi(optarg); 
	    } else {
		random_seed = getpid(); 
	    }
	    break;

	case 'q':
	    quiet = true;
	    break;

	case 'B':
	    retriangulate = false;
	    break;


	case 'I':
	    individual = true;
	    break;


	case 'R':
	    norescale = true;
	    break;


	case 'C':
	    closure = true;
	    break;


	case 'A':
	    no_field_arithmetic = true;
	    break;

	case 'K':
	    billiard_mode = true;
	    retriangulate = false;
	    perturb_euclidean = false; 
	    break;
	    
	case 'P':
	    tikz_output = true;
	    break;

	case 'p':
	    perturb_all = true;
	    if( optarg ) { 
		perturb_magnitude = atof(optarg);
	    }  
	    break;

	case 'x':
	    perturb_conjugate = true;
	    if( optarg ) { 
		perturb_magnitude = atof(optarg);
	    }  
	    break;

	case 'c':
	    if ( optarg ) {
		perturb_vertex = atoi(optarg);
		perturb_all = false;
	    } else
		ERR_RET("move_vertex: no arg");
	    break;

	case 'o':
	    if( optarg ) { 
#ifdef USE_LONG_DOUBLE
		if( sscanf(optarg,"%Lf,%Lf",&t1,&t2) == 2) {
#else
		if( sscanf(optarg,"%lf,%lf",&t1,&t2) == 2) {
#endif
		    offset = Point(t1,t2);
		} else
		    ERR_RET("offset: bad args");
	    } else 
		ERR_RET ("offset: no arg");

	    break;

	   
	case 'a':
	    if( optarg ) {
		DELTA = atof(optarg);
	    } else 
		ERR_RET("delta: bad arg");

	    break;

	case 'E':
	    if( optarg ) {
		EPSILON = atof(optarg);
		cout << "epsilon= " << EPSILON << endl;
	    } else 
		ERR_RET("epsilon: bad arg");

	    break;

	case 'G':
	    if( optarg ) {
		draw_saddle_length = atof(optarg);
		draw_saddles = true;
	    } else 
		ERR_RET("draw_saddles: bad arg");

	    break;
	    
	case 'H':
	    if( optarg ) {
		draw_saddle_length = atof(optarg);
		draw_cylinders = true;
		show_cyls = true;
		show_moduli = true; 
	    } else 
		ERR_RET("draw_cylinders: bad arg");

	    break;

	case 'J':
	    if( optarg ) {
		draw_tag = atoi(optarg);
	    } else 
		ERR_RET("tag: bad arg");

	    break;


	case 'f':
	    if( optarg ) {
		strncpy(filename,optarg,999);
		filename[999] = '\0';
		file_arg = true;
	    } else {
		ERR_RET("file: bad arg");
	    }



	    break;

	case 'n':
	    if (optarg ) {
		mc_number = atoi(optarg);
	    } else
		ERR_RET("mc_number: no arg");
	    break;

	case 'g':
	    if (optarg ) {
		mc_group = atoi(optarg);
	    } else
		ERR_RET("mc_group: no arg");
	    break;

	case 'm':
	    if (optarg ) {
		max_priority = atoi(optarg);
	    } else
		ERR_RET("max_priority: no arg");
	    break;

	case 'w':
	    new_sweep = true;
	    break;

	case 'N':
	    if (optarg ) {
		nice_inc = atoi(optarg);
	    } else
		ERR_RET("nice: no arg");
	    break;

	case 'S':
	    if (optarg ) {
		n_slices = atoi(optarg);
	    } else
		ERR_RET("slices: no arg");
	    break;

	case 'L':
	    show_lengths = true;
	    break;

	case 'Y':
	    show_cyls = true;
	    break;

	case 'F':
	    show_length_list = true;
	    break;



	case 'u':
	    cyls_only = true;
	    show_cyls = true;
	    break;

	case 't':
	    show_moduli = true;
	    show_cyls = true; 
	    break;


	case 'D':
	    allow_long_cyls = true;
	    break;


	case 'Z':
	    heights_and_twists = true;
	    show_cyls = true;
	    break;


	case '?':
	    ERR_RET("bad option");
	    break;
	    
	default:
	    printf ("?? getopt returned character code 0%o ??\n", c);
	    ERR_RET("Bad Option");
	}
    }

	if( n_slices > MAX_SLICES ) {
	    printf("Using %d slices\n", MAX_SLICES);
	    n_slices = MAX_SLICES; 
	}
	

    if( nice_inc ) {
	nice( nice_inc); 
    }



    if( mc_number == 0 && new_sweep == 0 ) {
	ERR_RET("must specify either number or new_sweep");
    }

    if ( follow_depth < 0.0 ) {
	follow_depth = depth+5; /* FIX ME */
    }

    if ( randomize ) {
#ifdef USE_MPI
	MPI_Bcast(&random_seed, 1, MPI_INT, 0, MPI_COMM_WORLD);
#endif 
	srandom(random_seed);
    }
    
    if(closure)
      {
	retriangulate = false;
	perturb_euclidean = false;
	norescale = true;
	field_arithmetic = true;
	heights_and_twists = true;
      }

    if( file_arg == false ) {
	if (optind != argc -3 && optind != argc - 4)
	{
	    ERR_RET("triangle: bad # of args");

	}
	else if(optind == argc -3 ) {
	    int a1, a2, a3;
	    a1 = atoi(argv[optind++]); 
	    a2 = atoi(argv[optind++]);
	    a3 = atoi(argv[optind++]);

	    if( ! no_field_arithmetic ) {
		field_arithmetic = true;
	    }
	    S->BuildTriangle(a1,a2,a3); 

	}  else { /* optind == argc - 4 */
	  int a1, a2, a3, a4;
	    a1 = atoi(argv[optind++]); 
	    a2 = atoi(argv[optind++]);
	    a3 = atoi(argv[optind++]);
	    a4 = atoi(argv[optind++]);
	    
	    if( ! no_field_arithmetic ) {
		field_arithmetic = true;
	    }
	    S->BuildQuad(a1,a2,a3,a4); 
	}
    } else {
	S->ReadComplex(filename);
    }
    poly<bigrat> minimal_polynomial = NumberField<bigrat>::F->min_poly();
    out_s << "Number field: min poly " << minimal_polynomial << endl;


    S->PrintAll(out_s);
    S->CheckAllFaces();
//    S->CheckAllVertices();

    S->make_pcomplexes();
    my_ostream debug_input_stream ("input");
    S->NewDraw(debug_input_stream);
    debug_input_stream.close();

    char buf[1000];

    for ( FacePtrIter f = S->faces.begin(); f!=S->faces.end(); f++ ) { 
	sprintf(buf,"face%d",(*f)->id());
	my_ostream* face_stream = new my_ostream(buf);
	S->make_pface(*f);
	S->NewDraw(*face_stream);
	face_stream->close();
	delete face_stream;
    }


    S->TriangulateAll();


    S->make_pcomplexes();
    my_ostream debug_stream ("debug");
    S->NewDraw(debug_stream);
    debug_stream.close();

/*
    S->make_pcomplexes();
    my_ostream test_stream("test");
    S->NewDraw(test_stream);
    test_stream.close();
*/
    S->Expunge();
    S->CheckAllFaces();
    
    S->CompleteSurface();
    S->BuildNeighborLists();
    S->set_area();
  
    S->CheckAllFaces();
    S->CheckAllVertices();
    S->StoreVertexOffsets();

    S->make_pcomplexes();
    my_ostream surface_stream("surface");
    S->NewDraw(surface_stream);
    surface_stream.close();

    if( retriangulate ) {
	S->RetriangulateSurface();
	S->Expunge();
	S->CheckAllFaces();
	S->CheckAllVertices();
	S->StoreVertexOffsets();


	S->doExtraFlips(); 
	S->Expunge(); 
	S->CheckAllFaces();
	S->CheckAllVertices();
	S->StoreVertexOffsets();

    }

    if ( retriangulate ) {
	for(UEdgePtrIter i = S->uedges.begin(); i != S->uedges.end(); i++ ) {
	    (*i)->internal = false;
	    (*i)->from_triang = false;
	}
    }

    
    S->make_pcomplexes();
    my_ostream retriang_stream("retriang");
    S->NewDraw(retriang_stream);
    retriang_stream.close();






    out_s << "Longest Edge: " << S->MaxEdge()->len() << endl;
    Dir<Point> shortest_dir;
    out_s << "Looking for shortest saddle...";
    COORD shortest = S->MinSaddle(shortest_dir);
    out_s  << shortest << endl;
    COORD mv = min(0.1, shortest/2);




    Point tmp_offset;
    VertexPtrIter i;

    if ( perturb_euclidean ) { 
	for ( i = S->vertices.begin(); i != S->vertices.end(); i++) {
	    if ( (*i)->euclidean ) {
		tmp_offset = Point(mv*my_random()/RANDOM_MAX, 
				     mv*my_random()/RANDOM_MAX);
		out_s << "Moving: "; (*i)->Print(out_s);
		out_s << " offset " << tmp_offset << "\n";
		(*i)->MoveVertex(tmp_offset);
	    } 
	}
    }


    if( perturb_vertex != UNDEFINED ) {
	if ( offset == Point(UNDEFINED,UNDEFINED)) {
	    offset = Point(mv*my_random()/RANDOM_MAX, mv*my_random()/RANDOM_MAX);
	}
	for ( i = S->vertices.begin(); i != S->vertices.end(); i++ ) {
	    if ( (*i)->id() == perturb_vertex ) {
		out_s << "Moving: "; (*i)->Print(out_s);
		out_s << " offset " << offset << "\n";
		(*i)->MoveVertex(offset);
		
	    } 
	}
    }

    if ( perturb_conjugate ) {
	fprintf(out_f, "Perturbing conjugates.....");
	if (  perturb_magnitude < EPSILON ) {
	    perturb_magnitude = mv;
	}


	S->PerturbConjugates(perturb_magnitude);
	fprintf(out_f,"done\n");
    }
	
    if( perturb_all ) {
	fprintf(out_f,"Perturbing surface...");
	if (  perturb_magnitude < EPSILON ) {
	    perturb_magnitude = 0.5;
	}
	S->PerturbAll(perturb_magnitude);
	fprintf(out_f,"done\n");
    }
//    sleep(3);


    S->make_pcomplexes();
    my_ostream perturbed_stream("perturbed");
    S->NewDraw(perturbed_stream);
    perturbed_stream.close();



//    S->PrintAll(out_s);
    S->CheckAllFaces();
    S->CheckAllVertices();
    if(field_arithmetic)
	S->check_algebraicQ();


    if ( field_arithmetic ) {
	S->ClearDenominators(); // also rescales
	S->CheckAllFaces();
	S->CheckAllVertices();
	S->check_algebraicQ();

	int_field_arithmetic = true; // use algebraic integers
	S->check_algebraicI();
    }
    S->PrintAll(out_s);

    

    S->StoreVertexOffsets();

    S->set_area();


    

//    S->PrintAll(out_s);
    S->StatPrint(out_s);    


    ofstream fout("the_surface.dat");
    S->StatPrint(fout);
    S->PrintAll(fout); 
    fout.close(); 

/*
    S->make_pcomplexes();
    //    S->MakeDrawList();
    my_ostream output_stream ("data");
    S->NewDraw(output_stream);
    output_stream.close();
*/

    if( !norescale ) {
	S->set_scale_factor(S->get_scale_factor()/sqrt(S->get_area()));
    }
    
    depth /= S->get_scale_factor();
    follow_depth /= S->get_scale_factor();
    
    // not so sure about these 
    //EPSILON /= S->get_scale_factor();
    //DELTA /= S->get_scale_factor();
    


    for ( i = S->vertices.begin(); i != S->vertices.end() ; i++ ) {
	if( start_vertex != UNDEFINED && (*i)->id()==start_vertex ) {
	    break;
	}
	if( start_vertex == UNDEFINED && !((*i)->euclidean) ) {
	    start_vertex = (*i)->id(); /* need to set end_vertex */
	    break;
	}

    }
    if( i == S->vertices.end() ) {
	ERR_RET("sweep: genus 1 and no vertex specified");
    }
    start_vertex = (*i)->id();



    if( end_vertex == UNDEFINED ) {
	end_vertex = start_vertex;
    }


    if ( new_sweep ) {
#ifdef USE_PARALLEL
	if( my_id == 0 ) {
	    NA.init0();
	    NA.main_loop0();
 
	    S->issueFinalReport(fsm, out_s); 
	    ofstream results_stream("final_results");
	    S->issueFinalReport(fsm,results_stream); //check
	    results_stream.close();

	} else {
	    NA.init1(); 
	    NA.main_loop1();
	}

#else
	out_s << "sweeping start V" << (*i)->id()<< " depth = " <<
	    depth << endl;
	
	
	COORD GoalTotalAngle = (*i)->total_angle(); 

	if( ! int_field_arithmetic ) {
	    Dir<Point> start_dir = Dir<Point>(*i, Point(1,0.0000123));
	    S->SweepNew<Point>(depth,start_dir,GoalTotalAngle);
	} else {
	    OEdgeIter first_edge = *((*i)->out_edges.begin());
	    Dir<BigPointI> start_dir = Dir<BigPointI>(first_edge);
	    S->SweepNew<BigPointI>(depth,start_dir,GoalTotalAngle);
	}

	S->issueFinalReport(smry,out_s);

	ofstream results_stream("final_results");
	S->issueFinalReport(smry,results_stream); //check
	results_stream.close();

#endif


    }

    if ( mc_number ) { 
	fprintf(out_f,"Using groups of size %d\n", mc_group);
	S->RandomShoot((*i), depth, mc_number );
//	fprintf(out_f,"Final Results:\n");
//	s1 = (*i)->total_angle()*mc_number/ta;
//	fprintf(out_f,"%g %g (", s1*S->volume()*MY_PI/(6*depth*depth), 
//	       s1*S->volume()/(MY_PI*depth*depth));

//	fprintf(out_f,") raw = %g\n", s1);
    }

/* old code (before slice)
#ifdef USE_MPI
    MPI_Finalize(); 
#endif
*/
    exit(0);
};




