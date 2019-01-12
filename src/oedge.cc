#include "two_complex.h"

OEdge::OEdge() 
{

    from_edge = NULL_OEdgeIter;

};


OEdge::OEdge(UEdgePtr e, int dir) 
{
    ue = e;
    direction = dir; 
    from_edge = NULL_OEdgeIter;
};

bool OEdge::operator==(OEdge &b)
{
    if( this->ue == b.ue && this->direction == b.direction) {
	return(true);
    }
    return(false);
}


int OEdge::id()
{
    return (ue->id());
}

void OEdge::Print(ostream& out)
{
    if( direction == 1) 
	out << " +";
    else
	out << " -";

//    out << (direction == 1) ? '+' : '-'; 
//    fprintf(out_f,"%c", (direction == 1) ? '+' : '-');
    ue->Simplex::Print(out);
}

VertexPtr OEdge::head()
{
    if( direction == 1) {
	return(ue->v0);
    } 
    return(ue->v1);
}

VertexPtr OEdge::tail()
{
    if( direction == 1) {
	return(ue->v1);
    } 
    return(ue->v0);
}

FacePtr OEdge::in_face()
{
    if( direction == 1) {
	return(ue->f0);
    }
    return(ue->f1);
}

FacePtr OEdge::other_face()
{
    if( direction == 1) {
	return(ue->f1);
    }
    return(ue->f0);
}

bool OEdge::deleted()
{
    return(ue->deleted());
}

void OEdge::Delete()
{

    ue->Delete();

}


OEdgeIter OEdge::this_iter()
{
    OEdgeIter i;
    FacePtr f;
    
    f = in_face();

    for( i = f->oedges.begin(); i != f->oedges.end() ; i++ ) {
	if ( (*i) == *this ) {
	    return(i);
	}
    }
    ERR_RET("this_iter: not found");
    return(NULL_OEdgeIter);
}

OEdgeIter OEdge::pair_edge()
{

    OEdgeIter i;
    FacePtr f;
    
    f = other_face();

    for( i = f->oedges.begin(); i != f->oedges.end() ; i++ ) {
	if ( (*i).ue == this->ue  && 
	        (*i).direction == -this->direction ) {
	    
	    return(i);
	}
    }
    ERR_RET("PairEdge: not found");
    return(NULL_OEdgeIter);
}

template <>
BigPointQ OEdge::vec<BigPointQ>()
{

   if( direction == 1 ) {
	return( ue->ue_vecQ );
    } else {
	return( -ue->ue_vecQ );
    }

}

BigPointQ OEdge::vecQ()
{
    return( vec<BigPointQ>() );
}


template <>
BigPointI OEdge::vec<BigPointI>()
{

   if( direction == 1 ) {
	return( ue->ue_vecI );
    } else {
	return( -ue->ue_vecI );
    }

}

template <>
BigPointI& OEdge::_vec<BigPointI>()
{

   if( direction == 1 ) {
	return( ue->ue_vecI );
    } else {
	return( ue->minus_ue_vecI );
    }

}



BigPointI OEdge::vecI()
{
    return( vec<BigPointI>() );
}


template <>
Point OEdge::vec<Point>()
{

   if( direction == 1 ) {
	return( ue->ue_vecQ.cx );
    } else {
	return( -ue->ue_vecQ.cx );
    }

}

template <>
Point& OEdge::_vec<Point>()
{

   if( direction == 1 ) {
	return( ue->ue_vecQ.cx );
    } else {
	return( ue->minus_ue_vecQ.cx );
    }

}


Point OEdge::vec_cx()
{
    return( vec<Point>() );
}



/*	       
BigPointQ OEdge::vecQ()
{
    BigPointQ q;

    if( direction == 1 ) {
	return( ue->ue_vecQ );
    } else {
	return( -ue->ue_vecQ );
    }
}
*/

// Point OEdge::vec_cx()
// {
//     if( direction == 1 ) {
// 	return( ue->ue_vecQ.cx );
//     } else {
// 	return( -ue->ue_vecQ.cx );
//     }
// }




alg_tQ OEdge::vec_algt()
{
    if( direction == 1 ) {
	return( ue->ue_vecQ.algt );
    } else {
	return( -ue->ue_vecQ.algt );
    }
}


void OEdge::set_vec_cx(Point p)
{

    p *= direction;
    ue->ue_vecQ.cx = p; 

}

void OEdge::set_vec_algt(alg_tQ p)
{

    p *= direction;
    ue->ue_vecQ.algt = p; 

}



/* the next function is stupid */
OEdgeIter OEdge::prev_edge()
{
    OEdgeIter i;
    FacePtr f;

    f = in_face();

    i = this_iter();

    if( i == f->oedges.begin() ) {
	i = f->oedges.end();
    }
    --i;
    return(i);
}

/* this is also stupid */
OEdgeIter OEdge::next_edge()
{

    FacePtr f;

    f = in_face();

    OEdgeIter i = this_iter();
    i++;

    if( i == f->oedges.end() ) {
	i = f->oedges.begin();
    }
    return(i);
}


OEdgePtrIter OEdge::this_vert_iter()
{
    
    VertexPtr v;
    
    v = head();

    for(OEdgePtrIter i = v->out_edges.begin(); i!= v->out_edges.end(); i++ ) {
	if ( *(*i) == *this ) {
	    return(i);
	}
    }
    return(NULL_OEdgePtrIter);
}

template <>
Point OEdge::headOffset<Point>()
{
    Point q,t;


    FacePtr f = in_face();

//    q = f->GetOffsetV0();
    q = Point(0.0);
    
    for(OEdgeIter i = f->oedges.begin(); i!= f->oedges.end(); i++ ) {
      if ( (*i) == *this ) {
	return(q);
      }
      t = (*i).vec_cx();
      q = q + t;
    };
    ERR_RET("headOffset: oedge not found");
    // unreachable
    return(q);
}

template <>
BigPointI OEdge::headOffset<BigPointI>()
{
    BigPointI q,t;


    FacePtr f = in_face();

//    q = f->GetOffsetV0();
    q = BigPointI();
    q.Check();
    
    for(OEdgeIter i = f->oedges.begin(); i!= f->oedges.end(); i++ ) {
      if ( (*i) == *this ) {
	return(q);
      }
      t = (*i).vec<BigPointI>();
      q = q + t;
    };
    ERR_RET("headOffset: oedge not found");
    // unreachable
    return(q);
}



template <>
Point OEdge::tailOffset<Point>()
{
    Point q,t;

    FacePtr f = in_face();
//    q = f->GetOffsetV0();
    q = Point(0.0);

  
    for(OEdgeIter i = f->oedges.begin(); i!=f->oedges.end(); i++ ) {
	if ( (*i) == *this ) {
	    return(q + (*i).vec_cx());
	}
	t = (*i).vec_cx();
	q = q + t;
    };
    ERR_RET("tailOffset: oedge not found");
    // unreachable
    return(q);
}

template <>
BigPointI OEdge::tailOffset<BigPointI>()
{
    BigPointI q,t;

    FacePtr f = in_face();
//    q = f->GetOffsetV0();
    q = BigPointI();
    q.Check();
  
    for(OEdgeIter i = f->oedges.begin(); i!=f->oedges.end(); i++ ) {
	if ( (*i) == *this ) {
	    return(q + (*i).vec<BigPointI>());
	}
	t = (*i).vec<BigPointI>();
	q = q + t;
    };
    ERR_RET("tailOffset: oedge not found");
    // unreachable
    return(q);
}



/*
algebraic OEdge::headOffset_alg()
{
    algebraic q,t;


    FacePtr f = in_face();

    q = f->GetOffsetV0_alg();
    
   
    for(OEdgeIter i = f->oedges.begin(); i!= f->oedges.end(); i++ ) {
	if ( (*i) == *this ) {
	  return(q);
	}
	t = (*i).vec_alg();
	q = q + t;
    };
    ERR_RET("headOffset: oedge not found");
    // unreachable
    return(q);
}
*/

/*
algebraic OEdge::tailOffset_alg()
{
    algebraic q,t;

    FacePtr f = in_face();
    q = f->GetOffsetV0_alg();

    for(OEdgeIter i = f->oedges.begin(); i!=f->oedges.end(); i++ ) {
	if ( (*i) == *this ) {
	    return(q + (*i).vec_alg());
	}
	t = (*i).vec_alg();
	q = q + t;
    };
    ERR_RET("tailOffset: oedge not found");
    // unreachable
    return(q);
}
*/


template <>
Point& OEdge::_head_offset<Point>()
{
    return head_offst;
}

template <>
BigPointI& OEdge::_head_offset<BigPointI>()
{
    return head_offstI;
}



template <>
Point& OEdge::_tail_offset<Point>()
{
    return tail_offst;
}

template <>
BigPointI& OEdge::_tail_offset<BigPointI>()
{
    return tail_offstI;
}




template Point OEdge::headOffset<Point>();
template Point OEdge::tailOffset<Point>();

template BigPointI OEdge::headOffset<BigPointI>();
template BigPointI OEdge::tailOffset<BigPointI>();

template Point& OEdge::_head_offset<Point>();
template Point& OEdge::_tail_offset<Point>();

template BigPointI& OEdge::_head_offset<BigPointI>();
template BigPointI& OEdge::_tail_offset<BigPointI>();





