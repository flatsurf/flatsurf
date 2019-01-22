#ifndef ALG_T_H
#define ALG_T_H

#include "libpolygon.h"
#include "defs.h"
#include "algebraic.h"



template<typename T>
class NumberField;

class Params
{

public:
    static void AddParams(int n, COORD parameters[]);
    static void ClearParams();
    static int nbr_params();
    static void print(ostream &);

    static int n_params;
    static vector<COORD> params; 
};

/* forward declarations for alg_t */
template<typename T>
class alg_t;

template<typename T>
class BigPoint;

template<typename T>
ostream& operator <<(ostream& outputStream, const alg_t<T>& num);
template<typename T>
bool operator==(const alg_t<T>&, const alg_t<T>&);
template<typename T>
bool operator!=(const alg_t<T>&, const alg_t<T>&);
template<typename T>
bool colinear(const alg_t<T> &u, const alg_t<T> &v);



/* alg_t class */

template<typename T>
class alg_t
{
 public:
 
    alg_t();   //initialize to 0
    alg_t(vector<algebraic<T> > coeffs);
//    alg_t(algebraic<T> coeffs[]);

    void pad_coeffs(int n);  // need to call this when increasing nbr_params
    algebraic<T> get_coeff(int n);
    void set_coeff(int n, const algebraic<T> &a);

    alg_t<T> operator -() const;
    alg_t<T>& operator +=(const alg_t<T>&);
    alg_t<T>& operator -=(const alg_t<T>&);
    alg_t<T>& operator*=(const algebraic<T>&);
    alg_t<T>& operator*=(int);
    alg_t<T>& operator/=(const algebraic<T>&);
    alg_t<T>& operator/=(int);
//    friend alg_t<T> operator+(alg_t<T>, alg_t<T>);

    friend ostream& operator << <>(ostream& outputStream, const alg_t<T>& num);
    friend bool operator== <>(const alg_t<T>&, const alg_t<T>&);
    friend bool operator!= <>(const alg_t<T>&, const alg_t<T>&);

    Point tocomplex() const;
    alg_t<T> conjugate() const;

    friend bool colinear <>(const alg_t<T> &u, const alg_t<T> &v);
    friend bool  is_alg_quotient(const alg_t<int64_t> &p1, 
			     const alg_t<int64_t> &p2, 
			     algebraic<bigrat> &ratio);  
                        //p1 divided by p2. Only works if it algebraic


    bool get_direction(algebraic<T> &direction);  //is direction in C independent of parameters? If yes, return true, and return the direction in direction. If no, return false

private:
    
    vector<algebraic<T> > coeffs;

};

template<typename T>
alg_t<T> operator +(alg_t<T>, const alg_t<T>&);
template<typename T>
alg_t<T> operator -(alg_t<T>, const alg_t<T>&); 
template<typename T>
alg_t<T> operator /(alg_t<T>&, const algebraic<T>&);
template<typename T>
alg_t<T> operator /(alg_t<T>&, int);
template<typename T>
alg_t<T> operator *(alg_t<T>&, int);
template<typename T>
alg_t<T> operator *(int, alg_t<T>&);
template<typename T>
alg_t<T> operator *(alg_t<T>, const algebraic<T>&);
template<typename T>
alg_t<T> operator *(const algebraic<T>, alg_t<T>&);


/* Big point class */

template <typename T>
class BigPoint;

template <typename T>
BigPoint<T> operator +(BigPoint<T>, const BigPoint<T>&);
template <typename T>
BigPoint<T> operator -(BigPoint<T>, const BigPoint<T>&); 
template <typename T>
BigPoint<T> operator /(BigPoint<T>, const algebraic<T>&);
template <typename T>
BigPoint<T> operator /(BigPoint<T>, int);
template <typename T>
BigPoint<T> operator *(BigPoint<T>, int);
template <typename T>
BigPoint<T> operator *(int, BigPoint<T>);
template <typename T>
BigPoint<T> operator *(BigPoint<T>, const algebraic<T>&);
template <typename T>
BigPoint<T> operator *(const algebraic<T>&, BigPoint<T>);
template <typename T>
ostream& operator <<(ostream& outputStream, const BigPoint<T>& num);
template <typename T>
bool operator==(const BigPoint<T>&, const BigPoint<T>&);
template <typename T>
bool operator!=(const BigPoint<T>&, const BigPoint<T>&);


template<typename T>
class BigPoint
{

 public:
 
    BigPoint();   //initialize to 0
    BigPoint(int, int); //only works if int's are 0; initialized to 0.
    BigPoint(alg_t<T> p_alg_t);

    BigPoint<T> operator -();
    BigPoint<T>& operator +=(const BigPoint<T>&);
    BigPoint<T>& operator -=(const BigPoint<T>&);
    BigPoint<T>& operator *=(const algebraic<T>&);
    BigPoint<T>& operator *=(int);
    BigPoint<T>& operator /=(const algebraic<T>&);
    BigPoint<T>& operator /=(int);


    friend BigPoint<T> operator+ <>(BigPoint<T>, const BigPoint<T>&);


    friend ostream& operator << <>(ostream& outputStream, const BigPoint<T>& num);
    friend bool operator== <>(const BigPoint<T>&, const BigPoint<T>&);
    friend bool operator!= <>(const BigPoint<T>&, const BigPoint<T>&);

    BigPoint<T> conjugate();

    friend bool colinear <>(const alg_t<T> &u, const alg_t<T> &v);

    void SetCxFromAlgebraic();
    void Check();

    Point cx;
    alg_t<T> algt;

};


//#include "alg_t.cc"

template<typename T>
inline Point to_cx( BigPoint<T> bp)
{
    return bp.cx;
}

//template <>
inline Point to_cx( Point p)
{
    return p;

}




typedef alg_t<bigrat> alg_tQ;
typedef alg_t<int64_t> alg_tI;
typedef algebraic<bigrat> algebraicQ;
typedef algebraic<int64_t> algebraicI;
typedef BigPoint<bigrat> BigPointQ;
typedef BigPoint<int64_t> BigPointI;

extern alg_tI convert_to_algtI(alg_tQ p);
extern bool is_alg_quotient(const alg_tI &p1, const alg_tI &p2,algebraicQ &r);  



#endif //ALG_T_H
