#ifndef PSIMPLEX_H
#define PSIMPLEX_H

#include "libpolygon.h"

class my_ostream {

public:

    my_ostream(string filename);
    ofstream& tri();
    ofstream& tex();
    void close();
    
private:

    ofstream tri_stream;
    ofstream tex_stream;
    
}; 


// included from two_complex.h
class PSimplex {

public:
    PSimplex();
    PSimplex(Point, int);

    Point p;
    int in_pcomplex;

    virtual SimplexPtr sp();
    virtual void Draw(my_ostream&, COORD);
    virtual ~PSimplex();

private:
    PSimplex (PSimplex&);

};



class PVertex:  public PSimplex {

public: 
    PVertex(VertexPtr, Point, int);

    virtual void Draw(my_ostream&, COORD d);

    virtual SimplexPtr sp();
    VertexPtr s;

};

class PUEdge:  public PSimplex {

public:
    PUEdge(UEdgePtr,Point,int);
    PUEdge(OEdge&, Point,int);

    virtual void Draw(my_ostream& ,COORD);
    
    UEdgePtr s;
    virtual SimplexPtr sp();

};

class PFace: public PSimplex {

public:
    PFace(FacePtr, Point, int);


    virtual void Draw(my_ostream& ,COORD);

    virtual SimplexPtr sp();
    FacePtr s;
    void DrawCylinders(my_ostream&, COORD);

};

typedef PVertex *PVertexPtr;
typedef PUEdge *PUEdgePtr;
typedef PFace *PFacePtr;

#endif // PSIMPLEX_H
