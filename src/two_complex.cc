/**********************************************************************
 *  This file is part of Polygon.
 * 
 *        Copyright (C) 2018 Alex Eskin
 *        Copyright (C) 2019 Vincent Delecroix
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

TwoComplex:: TwoComplex() 
{
    cur_vertex_id = 0;
    cur_face_id = 0;
    cur_uedge_id = 0;
    VISIT_ID = 0;
    area = -1.0;
    scale_factor = 1.0;

};



int TwoComplex::nedges() {
    return(uedges.size());
};

int TwoComplex::nfaces() {
    return(faces.size());
};

int TwoComplex::nvertices() {
    return(vertices.size());
};

void TwoComplex::PrintAll(ostream& out) {

    if( field_arithmetic ) {
	NumberField<bigrat>::F->print(out);
	Params::print(out);
    }    

    VertexPtrIter i;

    out << "# Vertices:";
    for( i = vertices.begin(); i != vertices.end(); i++ ) {
	out << " ";
	(*i)->Print(out);
    }
    out << endl; 

    UEdgePtrIter j;
    
    out << "# UEdges: " << endl;
    for( j = uedges.begin(); j != uedges.end(); j++ ) {
	(*j)->Print(out);
	out << endl;
    }

    FacePtrIter k;
    out << "# Faces: " << endl;
    for( k = faces.begin(); k != faces.end(); k++) {
	if ( (*k)->deleted() ) {
	    out << "#"; 
	}
	(*k)->Print(out);
	out << endl;
    }

    if ( field_arithmetic ) {
	out << "#UEdges Algebraic " << endl;
	for( j = uedges.begin(); j != uedges.end(); j++ ) {
	    out << "#E" << (*j)->id() << ": " << (*j)->ue_vecQ.algt;
	    out << endl;
    	}
    }

};


void TwoComplex::CheckAllFaces() 
{
    FacePtrIter i;

    for( i = faces.begin(); i != faces.end(); i++ ) {
	(*i)->Check();
    }
    
}

void TwoComplex::CheckAllVertices() 
{
    VertexPtrIter i;

    for( i = vertices.begin(); i != vertices.end(); i++ ) {
	(*i)->Check();
    }

}




VertexPtr TwoComplex::AddVertex(int id) {

    VertexPtr v = new Vertex;
    if( id >= 0 ) {
	v->ID = id;
	if ( id >= cur_vertex_id ) {
	    cur_vertex_id = id+1;
	}
    }
    else { 
	v->ID = cur_vertex_id++;
    }
    vertices.insert(vertices.end(), v);
    return(v);
};

UEdgePtr TwoComplex::AddUEdge(int id, VertexPtr v0, VertexPtr v1, Point vec) {

    UEdgePtr ue = new UEdge(v0,v1,vec);
    if( id < 0 ) {
	ue->ID = cur_uedge_id++;
    } else {
	ue->ID = id;
	if( id >= cur_uedge_id ) {
	    cur_uedge_id = id+1;
	}
    }
    ue->v0->order++;
    ue->v0->e = ue;

    ue->v1->order++;
    ue->v1->e = ue;

    uedges.insert(uedges.end(), ue);
    return(ue);
};

UEdgePtr TwoComplex::AddUEdge(int id, VertexPtr v0, VertexPtr v1, BigPointQ vec) {

  UEdgePtr ue = new UEdge(v0,v1,vec);
    if( id < 0 ) {
	ue->ID = cur_uedge_id++;
    } else {
	ue->ID = id;
	if( id >= cur_uedge_id ) {
	    cur_uedge_id = id+1;
	}
    }
    ue->v0->order++;
    ue->v0->e = ue;

    ue->v1->order++;
    ue->v1->e = ue;

    uedges.insert(uedges.end(), ue);
    return(ue);
};

FacePtr TwoComplex::AddFace(int id, list<OEdge> L)
{
    FacePtr f = new Face(L);
    if ( id >= 0 ) {
	f->ID = id;
	if( id >= cur_face_id ) {
	    cur_face_id = id+1;
	}
    } else {
	f->ID = cur_face_id++;
    }
    faces.insert(faces.end(),f);
    return(f);

};

void TwoComplex::StatPrint(ostream& out)
{

    out << "# File = " << filename 
	<< " perturb = " << perturb_magnitude 
	<< " rescale = " << !norescale << endl; 


    out << "# vertices: " << nvertices() << endl;
    out << "## ";
    for(VertexPtrIter i = vertices.begin(); i!= vertices.end(); i++ ) {
	(*i)->Print(out);
//	fprintf(out_f,"(%d) (%g PI) ",(*i)->order,(*i)->total_angle()/MY_PI);
	out << "(" << (*i)->order << ") (" <<(*i)->total_angle()/MY_PI 
	    << " PI) ";
    }
    out << endl;

    out << "# edges: " << nedges() << endl;
    out << "## ";
    for(UEdgePtrIter i = uedges.begin(); i!= uedges.end() ; i++ ) {
	(*i)->Simplex::Print(out);
	if( (*i)->deleted())
	    out << "(0) ";
	else if ((*i)->boundary())
	    out << "(1) ";
	else 
	    out << "(2) ";
    }
    out << endl;
  
    out << "# faces: " << nfaces() << endl;
    out << "# genus: " <<  (2-nvertices()+nedges()-nfaces())/2 << endl;
    out << "# area: " << get_area() << endl;
}

void TwoComplex::AddPFace(FacePtr f, Point q, int pcomplex)
{

    Point r,t;
    PFace* sp;
    PVertex *vp;
    PUEdge *ep;

    sp = new PFace(f, q, pcomplex);
    dl.insert(dl.end(),sp);

    
    r = sp->s->GetOffsetV0();
    r = r + q;
    
    for (OEdgeIter i = f->oedges.begin(); i != f->oedges.end(); i++ ) {
	vp = new PVertex((*i).head(), r, pcomplex);
	dl.insert(dl.end(),vp);

	ep = new PUEdge((*i), r,pcomplex);
	dl.insert(dl.end(),ep);

	t = (*i).vec_cx();
	r = r + t;
    }


}

void TwoComplex::MakeDrawListInternal(FacePtr f, Point q)
{
   
    OEdgeIter i,j;
    FacePtr candidate;
    VertexPtr v;
    Point t;



    if( f->deleted() )
	return;

    AddPFace(f, q, 0);
    f->visit_id = VISIT_ID;

    for( i = f->oedges.begin(); i!= f->oedges.end() ; i++ ) {
	if ((*i).ue->boundary())
	    continue;

	

	
	candidate = (*i).other_face();
	if (candidate->visit_id == VISIT_ID)
	    continue;

	
	j = (*i).pair_edge();

	if ( j == NULL_OEdgeIter ) 
	    ERR_RET("draw_list_internal: no pair edge");

	v = (*i).head();


	if( v != (*j).tail()) {
	    ERR_RET("drawlist_internal:edges don't match");
	}
	// q + offset(v) = q' + offset(v')
	// hence q' = q + offset(v) - offset(v')
	
	t = q + (*i).headOffset<Point>() - 
	    (*i).pair_edge()->tailOffset<Point>(); 


	MakeDrawListInternal(candidate, t);
    }
    return;
}

void TwoComplex::ClearSegmentsToDraw()
{

    FacePtrIter i;
    
    for ( i = faces.begin(); i!= faces.end(); i++ ) {
	(*i)->ClearSegmentsToDraw();
    }
}


void TwoComplex::MakeDrawList()
{
    // fix later 

    Point q(0.0,0.0);


    VISIT_ID++;

    // fix later
    FacePtrIter i;
    
    for ( i = faces.begin(); i!= faces.end(); i++ ) {
	if (! (*i)->deleted() )
	    break;
    }
    dl.clear();

    MakeDrawListInternal((*i), q);

    return;

}

/*
void TwoComplex::Draw(ostream& output_stream)
{
    COORD max_x, min_x, max_y, min_y, scale_factor;
    Point mean;
    Point tmp;

    DrawListIter i;
    
    if (dl.empty()) {
	ERR_RET("twocompex::draw: empty draw list");
    }
    max_x = min_x = dl.front()->p.real();
    max_y = min_y = dl.front()->p.imag();


    for( i = dl.begin(); i != dl.end(); i++ ) {
	if( (*i)->p.real() > max_x ) 
	    max_x = (*i)->p.real();
	if( (*i)->p.real() < min_x ) 
	    min_x = (*i)->p.real();
	if( (*i)->p.imag() > max_y ) 
	    max_y = (*i)->p.imag();
	if( (*i)->p.imag() < min_y ) 
	    min_y = (*i)->p.imag();
    }

    scale_factor = 20.0;

    if ( max_x - min_x > max_y - min_y ) 
	scale_factor = scale_factor/(max_x - min_x);
    else
	scale_factor = scale_factor/(max_y - min_y);
    
    mean = Point((max_x + min_x)/2, (max_y + min_y)/2);

    fprintf(out_f,"min_x = %lf max_x = %lf min_y = %lf max_y = %lf scale_factor = %lf \n",min_x, max_x, min_y, max_y, scale_factor);

    for( i = dl.begin(); i != dl.end(); i++ ) {
	tmp = (*i)->p -mean;
	(*i)->p = tmp*Point(scale_factor,0);
    }

    for( i = dl.begin(); i != dl.end() ; i++ ) {
	(*i)->Draw(output_stream, scale_factor);
    }

}
*/


void TwoComplex::Expunge()
{

    
    bool did_something;


    do { 
	did_something = false;
	for (VertexPtrIter i= vertices.begin(); i!= vertices.end(); i++ ) {
	    if ( (*i)->deleted() ) {
		vertices.erase(i);
		did_something = true;
		break;
	    }
	}
    } while ( did_something );

    do { 
	did_something = false;
	for (UEdgePtrIter i= uedges.begin(); i!= uedges.end(); i++ ) {
	    if ( (*i)->deleted() ) {
		uedges.erase(i);
		did_something = true;
		break;
	    }
	}
    } while ( did_something );

    do { 
	did_something = false;
	for (FacePtrIter i= faces.begin(); i!= faces.end(); i++ ) {
	    if ( (*i)->deleted() ) {
		faces.erase(i);
		did_something = true;
		break;
	    }
	}
    } while ( did_something );


}


void TwoComplex::BuildNeighborLists()
{


    OEdgeIter ep,ep_first;


    for( VertexPtrIter i = vertices.begin(); i != vertices.end(); i++ ) {

	if ( (*i)->deleted() ) {
	    continue;
	}

	(*i)->out_edges.clear();

	ep = (*i)->e->this_edge((*i)->e->f0);


	if( ep->tail() == (*i)) 
	    ep = ep->pair_edge();
   
	if( ep->head() != (*i))
	    ERR_RET("neighborlist: e pointer not adjacent to vertex");

	if( ep->deleted() ) {
	    ERR_RET("BuildNeighborLists: deleted ->e\n");
	}

	ep_first = ep;
   

	do {

	    (*i)->out_edges.insert((*i)->out_edges.begin(), ep);
	    ep = ep->pair_edge()->next_edge();

	} while (ep != ep_first);

	(*i)->int_angle = (*i)->total_angle_over_pi();
	
	if ( (*i)->int_angle == 2 ) {
	   (*i)->euclidean = true;
	} else {
	    (*i)->euclidean = false;
	}
    }



    return;


};


void TwoComplex::StoreVertexOffsets()
{

    FacePtr f;

    for( FacePtrIter i = faces.begin(); i != faces.end(); i++ ) {
	f = (*i);
	if( f->deleted() )
	    continue;
	
	for( OEdgeIter j = f->oedges.begin(); j!= f->oedges.end(); j++ ) {
	    (*j).next_e = (*j).next_edge();
	    (*j).pair_e = (*j).pair_edge();

	    (*j).head_offst = (*j).headOffset<Point>();
	    (*j).head_offstI = (*j).headOffset<BigPointI>();

	    (*j).tail_offst = (*j).tailOffset<Point>();
	    (*j).tail_offstI = (*j).tailOffset<BigPointI>();

	}
    }
    for (UEdgePtrIter i = uedges.begin(); i != uedges.end(); i++ ) {
	(*i)->minus_ue_vecQ = -(*i)->ue_vecQ;
	(*i)->minus_ue_vecI = -(*i)->ue_vecI;
    }
    return;
}


void TwoComplex::set_area()
{

    COORD s =0.0;
    for(FacePtrIter i = faces.begin(); i!= faces.end(); i++ ) {
	s = s + (*i)->volume();
    }
    area = s; 
}


COORD TwoComplex::get_area()
{

    if ( area > 0 ) {
	return(area);
    } else {
	ERR_RET("TwoComplex::get_area called before area was set");
	return(0.0);
    }
}

COORD TwoComplex::get_scale_factor()
{

    return scale_factor;
}

void TwoComplex::set_scale_factor(COORD scale)
{

    scale_factor = scale;

}

/*
void TwoComplex::Rescale(COORD scale)
{
    

    for(UEdgePtrIter i = uedges.begin(); i!= uedges.end(); i++ ) {
	(*i)->ue_vecQ *= scale;
    }

}
*/

UEdgePtr TwoComplex::MaxEdge()
{

    UEdgePtr longest_edge = NULL;
    COORD longest_length = 0;

    

    for(UEdgePtrIter i = uedges.begin(); i != uedges.end(); i++ ) {
	if ( (*i)->len() > longest_length ) {
	    longest_edge = (*i);
	    longest_length =  (*i)->len();
	}
    }
    return(longest_edge);
}

UEdgePtr TwoComplex::MinEdge()
{

    UEdgePtr shortest_edge = NULL;
    COORD shortest_length = 0;

    for( UEdgePtrIter i = uedges.begin(); i != uedges.end(); i++ ) {
	if ( (*i)->len() > shortest_length ) {
	    shortest_edge = (*i);
	    shortest_length =  (*i)->len();
	}
    }
    return(shortest_edge);
}

COORD TwoComplex::MinSaddle(Dir<Point>& the_shortest)
{	
	

    VertexPtr v0;
    Dir<Point> old_dir, new_dir, tmp_dir;
    COORD TotalAngle;

    COORD depth = MaxEdge()->len() + 0.1;
    COORD shortest = depth;

    for ( VertexPtrIter i = vertices.begin(); i!= vertices.end(); i++ ) {


	v0 = (*i);
	old_dir = Dir<Point>(v0,Point(1.12, 1.3));
	TotalAngle = 0;

	while ( TotalAngle < v0->total_angle() ) {

	    SweepNextLeft(old_dir, new_dir, depth*depth,10);
	    TotalAngle += true_angle(old_dir.vec, -new_dir.vec);

	    old_dir = old_dir.RotateCCwToVec(-new_dir.vec);

	    /* check for non-euclideanness ? */

	    if (abs(old_dir.vec) < shortest ) {
		shortest = abs(old_dir.vec);
		the_shortest = old_dir;


	    }
	}
    }
    
    return(shortest);
}

void TwoComplex::issueFinalReport(Summary& fsm, ostream& out, 
				  double part_done, 
				  double part_group) {

	    out << "File = " << filename << " depth = " << depth
			   << " follow_depth = " << follow_depth
			   << " perturb = " << perturb_magnitude << endl; 
	

	    out << "Final Report:\n";
	    fsm.print(out, part_done, part_group, get_area(), depth);
	    out << flush;
}





void TwoComplex::check_algebraicQ()
{
  for(UEdgePtrIter i = uedges.begin(); i!= uedges.end(); i++)
    {
	if( abs((**i).ue_vecQ.cx - (**i).ue_vecQ.algt.tocomplex()) > EPSILON)
	ERR_RET("ue_vecQ_alg doesn't match ue_vecQ");
    }  
  
 
}


void TwoComplex::check_algebraicI()
{
  for(UEdgePtrIter i = uedges.begin(); i!= uedges.end(); i++)
    {
	if( abs((**i).ue_vecI.cx - (**i).ue_vecI.algt.tocomplex()) > EPSILON)
	ERR_RET("ue_vecI_alg doesn't match ue_vecI");
    }  
  
 
}



int64_t TwoComplex::get_global_denom()
{

    mpz_class d(1);

    for(UEdgePtrIter i = uedges.begin(); i!= uedges.end(); i++) {
	for(int j=0; j<=Params::nbr_params(); j++ ) {
	    algebraic<bigrat> a = (*i)->ue_vecQ.algt.get_coeff(j);
	    vector<bigrat> c = a.get_coords();
	    for (unsigned int k = 0; k < c.size(); k++ ) {
		mpz_class t = c[k].get_den();
		if ( ! mpz_divisible_p(d.get_mpz_t(), t.get_mpz_t() ) ) {
		    d *= t; //d should really be lcm(d,t)
		}
	    }
	}
    }
    std::cout << "Clearing denominators: multiplying by " << d << endl; 
    
    if (! d.fits_sint_p() ) {
	ERR_RET("Denominator too large");
    }
    return (int)(d.get_si());


}


void TwoComplex::ClearDenominators()   
{

    int64_t d = get_global_denom();


    for(UEdgePtrIter i = uedges.begin(); i!= uedges.end(); i++) {
	(*i)->ue_vecQ *= d; 
	(*i)->ue_vecI.cx = (*i)->ue_vecQ.cx;

	(*i)->ue_vecI.algt = convert_to_algtI((*i)->ue_vecQ.algt);
    }
    set_scale_factor( d*get_scale_factor()); 

}






