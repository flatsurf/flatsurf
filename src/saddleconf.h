#ifndef SADDLECONF_H
#define SADDLECONF_H


//this file is included from two_complex.h


typedef int saddle_t;
typedef int vp_index;

class Cylinder;

class VertPattern {

    friend class SaddleConf;

public:

    VertPattern();

    void add(Dir<Point>& start_or_end, saddle_t id);
// - id is incoming, + id outgoing 

    void clear();

    void print(ostream& output_stream, SaddleConf &sc);

    int len(); /* number of saddles attached */

    bool is_empty(); /* i.e. base does not exist */
    saddle_t get_id(int i); /* returns the id of the 
			    saddle connection at base+MY_PI*i */

    COORD get_length(int i); /* returns the length of the 
			    saddle connection at base+MY_PI*i */

    int shortest_saddle_id(); /*id of shortest saddle */
                                  
    void divide_lengths(COORD factor); /* divide all lengths by factor */


    VertexPtr get_v();  /* returns the vertex this is a pattern for */

    int nbr_same(); /* number saddles returning to same vertex */
    int nbr_different(); /* number of saddles going to different vertices */
    int nbr_at_pi(); /* number of saddles returning at \pm \pi */
    void simple_print(ostream &output_steam); 

    void normalize(); 
    void pack(); 
    void unpack(); 

private:

    Dir<Point> base;
//	base.v is null if empty

//    Dir<BigPointI> baseI;            //finish later

    int v_id;  
// this is supposed to be the id of base.v
// it may get out of sync when structure is passed
// the normalize() function will make sure that it is back in sync....




    saddle_t at[MAX_SADDLES];

    COORD length[MAX_SADDLES]; //used only for --show_lengths
    Point vec[MAX_SADDLES]; //used only for --closure
//    BigPointI vecI[MAX_SADDLES];

};


class Cylinder {

public:

/*
    COORD cyl_lengths[MAX_SADDLES];
    Point cyl_vecs[MAX_SADDLES];
    Dir<Point> cyl_cross_saddles[MAX_SADDLES];
    COORD cyl_moduli[MAX_SADDLES];
*/  

    COORD length;
    Point vec;
    Dir<Point> cross_saddle;
    COORD modulus;
    int random_saddle_on_left; //for drawing cylinders

};
    





class SaddleConf {

    friend class VertPattern;

public:

     /* WARNING: check renumber_saddles before adding fields to this 
        class */

    SaddleConf();
    void clear();
    saddle_t add_saddle(Dir<Point> start, Dir<Point> end, alg_tI &algt);
    bool isom(SaddleConf& sc, int *s_matched);
    void print(ostream& output_stream);
    int n_saddles();
    int find_vert(VertexPtr v); /* the index of the vert */

    VertPattern vp[MAX_VERTICES]; /* add to check */
    int n_vp; /* nbr of vertex patterns this saddleconf */

    int count; /* number of occurences of this conf */
    int group_count; /* number of occurences in of conf in this group */

    void normalize();
    void pack();
    void unpack(); 


    void renorm_lengths(); //for --show_lengths
    COORD get_orig_min_saddle_length();
    void renumber_saddles(int *s_matched); //for --closure


    Cylinder cyl[MAX_SADDLES];

    //for calculating cylinders
    int n_cyl;
     void calculate_cylinders();
    void renorm_cylinders();
    COORD get_orig_min_cyl_length();


    int shortest_saddle_id(); /*id of shortest saddle */
    void get_saddle_by_id(int id, int& in_vp_index, int& in_at,  
			  int &out_vp_index, int& out_at);

    COORD get_length_by_id(int id);
//    int follow_right(int i);
    int follow_left(int i);
    void output_cylinders(FILE *fp);

    void check_cross_saddle(COORD cyl_len, Dir<Point> cross_saddle_Dir);
    void CheckCrossSaddles();


    
    // for long and short
    COORD shortest_occurence;
    COORD shortest_cyl_occurence;
    COORD longest_occurence;

    COORD smallest_total_area;
    COORD largest_total_area; 

    //for heights_and_twists
    COORD total_area_of_cyls;    
    Dir<Point> get_Dir_by_id(int id);

    //for closure
    FILE *output_f;

    // for sorting
    bool operator < (const SaddleConf& scf) const
    {
        return (count > scf.count);
    };

    //for visualization
    int tag; 
    void DrawSaddles();
    void DrawCylinders();

    set<COORD> lengths_set;
    

private:
    COORD original_min_cylinder_length;
    COORD original_shortest_length; /*original length of shortest saddle, 
					before dividing */
    alg_tI original_shortest_algt; /* the same as above, but algebraically */

    saddle_t next_id; /* id of next saddle connection */
    bool isomInternal(SaddleConf&sc, int n_matched, 
		     int* v_match, saddle_t* s_match);
    Dir<Point> start_Dir[MAX_SADDLES];
    alg_tI start_algt[MAX_SADDLES];
};


typedef SaddleConf* SaddleConfPtr;

class Summary {

public:
    Summary();
    void print(ostream& output_stream, COORD part_total, 
		    COORD part_group, COORD volume, COORD depth);

    int add_new_conf(SaddleConf& sc); 
    int add_one_conf(SaddleConf& sc); 

    void clear_group();
    int bad_angle_count;
    int weird_count;
    int close_count;
    int reject_count;

    void normalize(); 
    void define_mpi_type(); 

    void merge(Summary &s2); 
    void clear();

    void pack(); 
    void unpack(); 

    bool scf_compare(int i, int j);

    int tag_count;

//private:

    vector<SaddleConf> scf;
};

typedef list<SaddleConf>::iterator SaddleConfIter;





#endif // SADDLECONF_H
