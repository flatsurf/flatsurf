#include "libpolygon/two_complex.h"
#include <assert.h>

void TwoComplex::TriangulateAll() 
{
    
    bool did_something; 

    do {
    
	did_something = false;

	for(FacePtrIter i = faces.begin(); i != faces.end(); i++ ) {

	    if ( (!(*i)->deleted()) && (*i)->order() != 3 ) {
		TriangulateFace(*i);
		did_something = true;
		break;
	    }
	    
	}
	
    } while ( did_something );

}

int nbr = 0;
my_ostream *movie_stream, *dbg_stream;
char buf[1000];


void TwoComplex::TriangulateFace( FacePtr f )
{


    OEdgeIter Candidate1 = NULL_OEdgeIter;
    OEdgeIter Candidate2 = NULL_OEdgeIter;

    COORD best_angle = 0;
    COORD a;

    if ( f->order() == 3 ) {
	return;
    }


//    if ( f->order > MAX_FACE_ORDER -1 ) {
//	ERR_RET("TriangulateFace: order of face too large");
//    }
    if ( f->deleted() ) {
	ERR_RET( "TriangulateFace: face already deleted");
    }
    
    for(OEdgeIter i = f->oedges.begin(); i!=f->oedges.end() ; i++ ) {
	Point bs = (*i).vec_cx();
	for (OEdgeIter j = i; j != f->oedges.end(); j++ ) {
	    if ( i == j ) {
		continue;
	    }

//	    a = 0; 
	    a = min(true_angle((*i).vec_cx(), bs),
		    true_angle(bs, -(*i).prev_edge()->vec_cx()));

	    a = min(a, true_angle(-bs, -(*j).prev_edge()->vec_cx()));
	    a = min(a, true_angle((*j).vec_cx(),-bs));

	    if( a < 0 ) {
		ERR_RET("triangluate face: a < 0 \n");
	    }
	    
    // 	    // a = true_angle((*i).vec_cx(), -(*i).prev_edge()->vec_cx()) + 
    // 	    // 	true_angle((*j).vec_cx(), -(*j).prev_edge()->vec_cx());
	    

    	    printf("F%d V%d(E%d) V%d(E%d) a=%g, can_bisect=%d\n", f->id(),
    		   (*i).head()->id(), (*i).id(),
    		   (*j).head()->id(), (*j).id(),
    		   a, f->can_bisect(i,j));

    	    if ( a > best_angle && f->can_bisect(i,j) ) {
    		Candidate1 = i;
    		Candidate2 = j;
    		best_angle = a;
    	    }
	    bs += (*j).vec_cx();

    	}
    }

    printf("F%d best_angle = %g\n",f->id(), best_angle);


//	sprintf(buf,"dbg_bisect");
//	dbg_stream = new my_ostream(buf);
//	make_pface(f);
//	NewDraw(*dbg_stream);
//	dbg_stream->close();
//	delete dbg_stream;

    
    
    if( best_angle == 0 || (! f->can_bisect(Candidate1, Candidate2 ))) {
	printf("F%d: Parent Face: F%d best_angle=%g",f->id(),
	       f->parent_face_ID,best_angle);
	if (Candidate1!=NULL_OEdgeIter && Candidate2!=NULL_OEdgeIter) {
	    printf(" Candidate1=E%d Candidate2=E%d",  Candidate1->id(), Candidate2->id());
	}
	printf("\n");
	f->Print(cout);
	printf("\n");
	for(OEdgeIter i1=f->oedges.begin(); i1!=f->oedges.end(); i1++) {
	    (*i1).ue->Print(cout);
	    printf("\n");
	}
	
	sprintf(buf,"dbg");
	dbg_stream = new my_ostream(buf);
	make_pface(f);
	NewDraw(*dbg_stream);
	dbg_stream->close();
	delete dbg_stream;

	ERR_RET("TriangulateFace: cannot find candidate");
    }
    
    OEdgeIter new_edge = BisectFace(f, Candidate1, Candidate2);
    new_edge->ue->from_triang = true;


    if ( verbose >= 1) {
	nbr++;
	sprintf(buf,"movie_triang%d",nbr);
	movie_stream = new my_ostream(buf);
	make_pcomplexes();
	NewDraw(*movie_stream);
	movie_stream->close();
	delete movie_stream;
    }

//    PrintAll(std::cout);
//    CheckAllFaces();
//    CheckAllVertices();
    
    TriangulateFace(new_edge->pair_edge()->in_face());
    TriangulateFace(new_edge->in_face());

}

OEdgeIter TwoComplex::BisectFace(FacePtr f, OEdgeIter e1_iter,OEdgeIter e2_iter)
{

    /* head of e1 to head of e2 */
    /* return new oedge in face 1 (containing e1 ) */

    list<OEdge> new_face1_oedge_list; 
    list<OEdge> new_face2_oedge_list; 


    BigPointQ bp;
    BigPointQ bs;



    VertexPtr v1 = (*e1_iter).head();
    VertexPtr v2 = (*e2_iter).head();

    if ( f->order() <= 3 ) {
	ERR_RET("Bisect Face: order of face at most 3");
    }
    if ( e1_iter == e2_iter ) {
	ERR_RET("Bisect Face: edges the same");
    }
    

    /* assume face is on the left of edges */


    OEdgeIter i = e1_iter;
    

    while ( i != e2_iter ) {
	new_face1_oedge_list.insert(new_face1_oedge_list.end(),*i);
	bs += (*i).vecQ();

	/* clean up */
	i++;
	if ( i == f->oedges.end() ) {
	    i = f->oedges.begin();
	}
	if ( i == e1_iter ) {
	    ERR_RET("Bisect Face: e2 not found");
	}

    }
    
    bp = bs; /* p from v1 to v2 */
    bp.Check();

    if ( i != e2_iter ) {
	ERR_RET("Bisect Face: this is impossible");
    }
    
    bs = BigPointQ();   //set to 0;

    while ( i != e1_iter ) {
	new_face2_oedge_list.insert(new_face2_oedge_list.end(),*i);
	bs += (*i).vecQ();

	/* clean up */
	i++;
	if ( i == f->oedges.end() ) {
	    i = f->oedges.begin();
	}
	if ( i == e2_iter ) {
	    ERR_RET("Bisect Face: e1 not found");
	}

    }
    bs.Check();

    if ( abs( bp.cx + bs.cx ) > EPSILON ) {
	ERR_RET("Bisect Face: total vec not zero");
    }


    BigPointQ bq = bp+bs;

    if ( field_arithmetic && !(bq.algt == alg_tQ()) ) { //bq is not zero
	std::cout << bq.algt; 
	ERR_RET("Bisect Face: total vec_algt not zero");
    }


    UEdgePtr new_uedge = AddUEdge(UNDEFINED, v1, v2, bp);
    OEdgePtr new_oedge1 = new OEdge(new_uedge, -1);
    OEdgePtr new_oedge2 = new OEdge(new_uedge, 1);

    new_face1_oedge_list.insert(new_face1_oedge_list.end(),*new_oedge1);
    new_face2_oedge_list.insert(new_face2_oedge_list.end(),*new_oedge2);

    if ( new_face1_oedge_list.size() < 3 ) {
	ERR_RET("BisectFace: new_face1 has less then 3 edges");
    }

    if ( new_face2_oedge_list.size() < 3 ) {
	ERR_RET("BisectFace: new_face2 has less then 3 edges");
    }


    AddFace(UNDEFINED, new_face1_oedge_list);
    AddFace(UNDEFINED, new_face2_oedge_list);

    if ( new_uedge->boundary() ) {
	ERR_RET("BisectFace: bad hookup");
    }

    /* do not know what this does, but seems needed */
    new_uedge->internal = true;
    new_oedge1->from_edge = new_oedge1->pair_edge();
    new_oedge2->from_edge = new_oedge2->pair_edge();
    new_oedge1->in_face()->parent_face_ID = f->id();
    new_oedge2->in_face()->parent_face_ID = f->id();

    

    f->Delete();        /* this deletes the old face f */
                       /* it will be expunged later */

    return( new_oedge1->this_iter() );
}

bool Face::can_bisect(OEdgeIter i, OEdgeIter j)
{


    if ( j == i || j == prev_edge(i) || j == next_edge(i) ) {
	return(false);
    }




    Point v1_offset, v2_offset;

    bool i_found = false;
    bool j_found = false;

    Point s = Point(0,0);
    COORD diameter = 0; 


    for (OEdgeIter k = oedges.begin(); k != oedges.end(); k++ ) {
	if ( k == i ) {
	    v1_offset = s;
	    i_found = true;
	}
	if ( k == j ) {
	    v2_offset = s;
	    j_found = true;
	}
	s += (*k).vec_cx();
	diameter += abs(s);
    }

    if ( ( !i_found ) || ( !j_found ) ) {
	ERR_RET("can_bisect:i or j not found"); 
    }

    if ( abs(s) > EPSILON ) {
	ERR_RET("can_bisect: sum of face not zero");
    }
    
    
    Point intersection; 

    s = Point(0,0);

/* Check for intersecting edges */
    for(OEdgeIter k = oedges.begin(); k!= oedges.end(); k++ ) {
	
	if( k != i && k != j && k != prev_edge(i) && k != prev_edge(j) &&
	    intersect_segment_interior(v1_offset, v2_offset-v1_offset, 
			      s, (*k).vec_cx(), intersection)) {
//	    VertexPtr v1 = (*i).head();
//	    VertexPtr v2 = (*j).head();
// 	    fprintf(out_f,"V%d V%d: blocking E%d\n", v1->id(), v2->id(),
// 		   (*k).ue->id());
// 	    fprintf(out_f,"(%g %g)--(%g %g), (%g %g)--(%g %g)\n",
// 		   v1_offset.real(), v1_offset.imag(),
// 		   v2_offset.real(), v2_offset.imag(),
// 		   s.real(), s.imag(), 
// 		   (s+(*k).vecQ()).real(), 
// 		   (s+(*k).vecQ()).imag()); 

	    return(false);
	}
	s += (*k).vec_cx();	
    }



/* check that the new edge is inside the face */



    Point midpoint = Point(0.5)*(v1_offset + v2_offset);


    
    if ( contains(midpoint+GetOffsetV0()) ) {
      return(true);
    }
    return(false);
}


/* for debugging */
void TwoComplex::AddSteinerPoint(FacePtr f)
{
    if ( f->order() != 3 ) {
	ERR_RET("AddSteinerPoint: not triangle");
    }

    OEdgeIter i = f->oedges.begin();
    OEdgePtr edge01 = &(*i++);
    OEdgePtr edge12 = &(*i++);
    OEdgePtr edge20 = &(*i++);
    if ( i != f->oedges.end() ) {
	ERR_RET("AddSteinerPoint: error");
    }

    Point v0_pos = Point(0,0);
    Point v1_pos = edge01->vec_cx();
    Point v2_pos = edge01->vec_cx() + edge12->vec_cx();

    Point p_steiner = Point(1.0/3)*(v0_pos+v1_pos+v2_pos);
//  Point p_steiner = 0.1*v0_pos+0.45*v1_pos+0.45*v2_pos;


    std::cout << v0_pos << v1_pos << v2_pos << p_steiner <<"\n";

  
    VertexPtr v0 = edge01->head();
    VertexPtr v1 = edge12->head();
    VertexPtr v2 = edge20->head();
  
    VertexPtr v_steiner = AddVertex(UNDEFINED);
    UEdgePtr new_uedge0 = AddUEdge(UNDEFINED, v0, v_steiner, p_steiner);
    UEdgePtr new_uedge1 = AddUEdge(UNDEFINED, v1, v_steiner, 
				   p_steiner - edge01->vec_cx());
    UEdgePtr new_uedge2 = AddUEdge(UNDEFINED, v2, v_steiner, 
				   p_steiner - edge01->vec_cx() - edge12->vec_cx());
  

    list<OEdge> tmp_oedge_list; 
    OEdgePtr e0_in  = new OEdge(new_uedge0, 1);
    OEdgePtr e0_out = new OEdge(new_uedge0, -1);
    OEdgePtr e1_in  = new OEdge(new_uedge1, 1);
    OEdgePtr e1_out = new OEdge(new_uedge1, -1);
    OEdgePtr e2_in  = new OEdge(new_uedge2, 1);
    OEdgePtr e2_out = new OEdge(new_uedge2, -1);


    tmp_oedge_list.clear();
    tmp_oedge_list.insert(tmp_oedge_list.end(),*edge01);
    tmp_oedge_list.insert(tmp_oedge_list.end(),*e1_in);
    tmp_oedge_list.insert(tmp_oedge_list.end(),*e0_out);
    AddFace(UNDEFINED, tmp_oedge_list);

    tmp_oedge_list.clear();
    tmp_oedge_list.insert(tmp_oedge_list.end(),*edge12);
    tmp_oedge_list.insert(tmp_oedge_list.end(),*e2_in);
    tmp_oedge_list.insert(tmp_oedge_list.end(),*e1_out);
    AddFace(UNDEFINED, tmp_oedge_list);

    tmp_oedge_list.clear();
    tmp_oedge_list.insert(tmp_oedge_list.end(),*edge20);
    tmp_oedge_list.insert(tmp_oedge_list.end(),*e0_in);
    tmp_oedge_list.insert(tmp_oedge_list.end(),*e2_out);
    AddFace(UNDEFINED, tmp_oedge_list);


    /* do not know what this does, but seems needed */
    new_uedge0->internal = true;
    new_uedge1->internal = true;
    new_uedge2->internal = true;

    e0_in->from_edge = e0_in->pair_edge();
    e1_in->from_edge = e1_in->pair_edge();
    e2_in->from_edge = e2_in->pair_edge();

    e0_out->from_edge = e0_out->pair_edge();
    e1_out->from_edge = e1_out->pair_edge();
    e2_out->from_edge = e2_out->pair_edge();

    f->Delete();

}

bool Vertex::CanRemove()
{

    if( ! euclidean ) {
	return(false);
    }

    for( OEdgePtrIter i = out_edges.begin(); i!= out_edges.end(); i++ ) {
	if( (*i)->tail()->id() == id() ) {
	    return(false);
	}

    }
    return(true);

}


FacePtr TwoComplex::RemoveVertex(VertexPtr v0)
{

    if( !v0->euclidean ) {
	ERR_RET("Remove Vertex: not euclidean");
    }
    list<OEdge> tmp_oedge_list;

    for(OEdgePtrIter i = v0->out_edges.begin(); i!= v0->out_edges.end(); i++){
	tmp_oedge_list.insert(tmp_oedge_list.end(), *((*i)->next_edge()));
	(*i)->tail()->e = (*i)->next_edge()->ue;
    }
    FacePtr f = AddFace(UNDEFINED, tmp_oedge_list);

    for(OEdgePtrIter i = v0->out_edges.begin(); i!= v0->out_edges.end(); i++){
	(*i)->tail()->order = (*i)->tail()->order - 1;
	if( (*i)->tail()->e == (*i)->ue ) {
	    ERR_RET("RemoveVertex: ->e illegal");
	}


	FacePtr tmp = (*i)->in_face();
	(*i)->Delete();
	tmp->Delete();

    }
    v0->Delete();
    return(f);
    
}
    
void TwoComplex::RetriangulateSurface()
{

    VertexPtrIter i;
    UEdgePtrIter j;
    bool did_something;


    fprintf(out_f,"In RetriangulateSurface\n");

    do {
	did_something = false;	
	
	for( j = uedges.begin(); j!= uedges.end(); j++ ) {
	    if( (*j)->deleted() ) {
		continue;
	    }
	    if( (*j)->v0->id() == (*j)->v1->id() && (*j)->v0->euclidean ) {
		/* FIXME: should also check if others are different */
	        fprintf(out_f,"Flipping E%d\n", (*j)->id());
		FlipEdge(*j);

		BuildNeighborLists();
		CheckAllFaces();
		CheckAllVertices();
		//		PrintAll(std::cout);

		if ( verbose >= 1 ) {
		    nbr++;
		    sprintf(buf,"movie_flip%d.tri",nbr);
		    movie_stream = new my_ostream(buf);
		    make_pcomplexes();
		    NewDraw(*movie_stream);
		    movie_stream->close();
		    delete movie_stream;
		}





		did_something = true;
		break;
	    }
	}

	for( i = vertices.begin(); i!= vertices.end(); i++ ) {
	    if( (*i)->deleted() ) {
		continue;
	    }
	    if( (*i)->CanRemove() ) {
		break;
	    }
	}
	if( i == vertices.end() ) { /* no euclidean vertex found */
	    continue;
	}
	fprintf(out_f,"Removing V%d\n",(*i)->id());
	FacePtr f = RemoveVertex(*i);
	did_something = true;

	if ( verbose >= 1 ) {
	    nbr++;
	    sprintf(buf,"movie_triang%d",nbr);
	    movie_stream = new my_ostream(buf);
	    make_pcomplexes();
	    NewDraw(*movie_stream);
	    movie_stream->close();
	    delete movie_stream;
	}


//	BuildNeighborLists();
//	CheckAllVertices();
	CheckAllFaces();

	TriangulateFace(f);
	BuildNeighborLists();
	CheckAllVertices();
	CheckAllFaces();
	//	PrintAll(std::cout);

	if ( verbose >= 1 ) {
	    nbr++;
	    sprintf(buf,"movie_triang%d",nbr);
	    movie_stream = new my_ostream(buf);
	    make_pcomplexes();
	    NewDraw(*movie_stream);
	    movie_stream->close();
	    delete movie_stream;
	}


    } while( did_something );

    BuildNeighborLists();
    StoreVertexOffsets();

}


UEdgePtr TwoComplex::FlipEdge(UEdgePtr u)
{

/*           vt
            / |\
           / u| \ 
          /   |  \
       w0  f0 |   w1
          \   |f1/
           \  | /
            \ |/
             vh

*/



    FacePtr f0 = u->f0;
    FacePtr f1 = u->f1;

    OEdgeIter oe = u->this_edge(f0);

    VertexPtr vh = oe->head();
    VertexPtr vt = oe->tail();

    OEdgeIter vt_w0 = oe->next_edge();
    OEdgeIter w0_vh = oe->prev_edge();

    OEdgeIter vh_w1 = oe->pair_edge()->next_edge();
    OEdgeIter w1_vt = oe->pair_edge()->prev_edge();

    VertexPtr w0 = vt_w0->tail();
    VertexPtr w1 = vh_w1->tail();

    BigPointQ w0_w1_p = w0_vh->vecQ() + vh_w1->vecQ();
    if ( field_arithmetic ) {
	w0_w1_p.SetCxFromAlgebraic();
    }
    UEdgePtr new_u = AddUEdge(UNDEFINED, w0, w1, w0_w1_p);
    u->internal = true; 

    OEdgePtr w0_w1 = new OEdge(new_u,1);
    OEdgePtr w1_w0 = new OEdge(new_u,-1);

    list<OEdge> tmp_oedge_list;

    tmp_oedge_list.clear();
    tmp_oedge_list.insert(tmp_oedge_list.end(), *w0_w1);
    tmp_oedge_list.insert(tmp_oedge_list.end(), *w1_vt);
    tmp_oedge_list.insert(tmp_oedge_list.end(), *vt_w0);
 
    AddFace(UNDEFINED, tmp_oedge_list);

    tmp_oedge_list.clear();
    tmp_oedge_list.insert(tmp_oedge_list.end(), *w1_w0);
    tmp_oedge_list.insert(tmp_oedge_list.end(), *w0_vh);
    tmp_oedge_list.insert(tmp_oedge_list.end(), *vh_w1);
 
    AddFace(UNDEFINED, tmp_oedge_list);

    vh->e = vh_w1->ue;
    vt->e = vt_w0->ue;

    /* really out to be part of Delete */
    vh->order--;
    vt->order--;

    u->Delete();
    f0->Delete();
    f1->Delete();

    return(new_u);

}

Point TwoComplex::OpposingDiagonal(UEdgePtr u)
{

/*           vt
            / |\
           / u| \ 
          /   |  \
       w0  f0 |   w1
          \   |f1/
           \  | /
            \ |/
             vh

*/



    FacePtr f0 = u->f0;
//    FacePtr f1 = u->f1;

    OEdgeIter oe = u->this_edge(f0);

//    VertexPtr vh = oe->head();
//    VertexPtr vt = oe->tail();

//    OEdgeIter vt_w0 = oe->next_edge();
    OEdgeIter w0_vh = oe->prev_edge();

    OEdgeIter vh_w1 = oe->pair_edge()->next_edge();
//    OEdgeIter w1_vt = oe->pair_edge()->prev_edge();

//    VertexPtr w0 = vt_w0->tail();
//    VertexPtr w1 = vh_w1->tail();
    
    return(w0_vh->vec_cx()+vh_w1->vec_cx()); 

}

bool TwoComplex::shouldFlip(UEdgePtr u)
{

/*           vt
            / |\
           / u| \ 
          /   |  \
       w0  f0 |   w1
          \   |f1/
           \  | /
            \ |/
             vh

*/

    if( u->deleted() ) {
	return(false); 
    }



    FacePtr f0 = u->f0;
//    FacePtr f1 = u->f1;

    OEdgeIter oe = u->this_edge(f0);

//    VertexPtr vh = oe->head();
//    VertexPtr vt = oe->tail();

    OEdgeIter vt_w0 = oe->next_edge();
    OEdgeIter w0_vh = oe->prev_edge();

    OEdgeIter vh_w1 = oe->pair_edge()->next_edge();
    OEdgeIter w1_vt = oe->pair_edge()->prev_edge();

//    VertexPtr w0 = vt_w0->tail();
//    VertexPtr w1 = vh_w1->tail();

    //the delaunay condition. Eventually will replace by in_circle() (in math.cc)
    
    if( angle(-vt_w0->vec_cx(),w0_vh->vec_cx()) + 
        angle(-vh_w1->vec_cx(),w1_vt->vec_cx()) < MY_PI+100*EPSILON) {
	return(false);
    }


    // if( norm( OpposingDiagonal(u) ) >= norm(oe->vec_cx()) ) {
    // 	return(false); 
    // }


    if( abs(angle(-w0_vh->vec_cx(),oe->vec_cx()))+
	abs(angle(oe->vec_cx(),vh_w1->vec_cx())) >= MY_PI ) {
	
	return(false); 
    }

    if( abs(angle(-w1_vt->vec_cx(),-oe->vec_cx()))+
	abs(angle(-oe->vec_cx(),vt_w0->vec_cx())) >= MY_PI ) {

	return(false); 
    }

    return(true); 

}

void TwoComplex::doExtraFlips() 
{

    bool did_something; 

    do { 
	did_something = false;
	for (UEdgePtrIter i= uedges.begin(); i!= uedges.end(); i++ ) {
	    if ( shouldFlip((*i)) ) {

//		PrintAll(std::cout);

		fprintf(out_f,"Extra: Flipping E%d\n", (*i)->id());
		FlipEdge(*i);

		BuildNeighborLists();

//		PrintAll(std::cout);

		CheckAllFaces();
		CheckAllVertices();



		did_something = true;
		break;
	    }
	}
    } while ( did_something );

}
