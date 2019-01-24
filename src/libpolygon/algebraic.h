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

#ifndef LIBPOLYGON_ALGEBRAIC_H
#define LIBPOLYGON_ALGEBRAIC_H

#include "libpolygon.h"

#include "defs.h"

#include<gmpxx.h>
#include<iostream>
#include<vector>
#include<complex>
#include<cmath>

using namespace std;

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>





typedef mpq_class bigrat;

template <typename T>
class algebraic;

template <typename T>
class poly;

template <typename T>
class alg_t;

template <typename T>
void divide(poly<T>& quotient, poly<T>& remainder, const poly<T> &f, const poly<T> &g);  //f divided by g.
template <typename T>
poly<T> operator*(poly<T>, const poly<T>&);
template <typename T>
ostream& operator <<(ostream& outputStream, const poly<T>& poly);
template <typename T>
bool operator==(const poly<T>&, const poly<T>&);
template <typename T>
bool operator!=(const poly<T>&, const poly<T>&);




template <typename T>
class poly
{
public:
    poly();  //zero 
    explicit poly(vector<T> a);  //a_0 + a_1 x + ... + a_n x^n
    poly(T coefficients[], int degree);
    int degree() const;  
    T leading_coefficient() const;

    friend void divide<>(poly<T>& quotient, poly<T>& remainder, const poly<T> &f, const poly<T> &g);  //f divided by g
  
  void print();  //for debugging: prints coefficients

  poly<T>& operator +=(const poly<T>&);
  poly<T>& operator -=(const poly<T>&);
  poly<T>& operator*=(T);
  friend poly<T> operator*<>(poly<T>, const poly<T>&);
  algebraic<T> operator()(const algebraic<T> &);

  friend ostream& operator << <>(ostream& outputStream, const poly<T>& poly);
  friend bool operator== <>(const poly<T>&, const poly<T>&);
  friend bool operator!= <>(const poly<T>&, const poly<T>&);
  


  vector<T> coefficients;
  void reduce();  //remove trailing zero coefficients
  void fill_back(int n);  //add trailing zero coefficients until there are at least n coefficients
};

template<typename T>
void gcd_extended(poly<T>& u, poly<T>& v, const poly<T> &p, const poly<T> &q); //finds u and v such that up+vq = gcd(p, q)
template<typename T>
poly<T> operator+(poly<T>, const poly<T>&);
template<typename T>
poly<T> operator-(poly<T>, const poly<T>&);
template<typename T>
poly<T> operator*(T, poly<T>);
template<typename T>
poly<T> operator%(poly<T>, const poly<T>&);

template<class T>
bool is_unit(const poly<T>);


int max(int, int);

template<typename T>
class NumberField;

template<typename T>
algebraic<T> operator *(algebraic<T>, T);
template<typename T>
algebraic<T> operator *(T, algebraic<T>);
template<typename T>
algebraic<T> operator -(const algebraic<T> &r);
    //friend  algebraic<T> operator ^(algebraic<T>, int);
template<typename T>
ostream& operator <<(ostream& outputStream, const algebraic<T>& num);
template<typename T>
bool operator==(const algebraic<T>&, const algebraic<T>&);
template<typename T>
bool operator!=(const algebraic<T>&, const algebraic<T>&);

template<class T>
algebraic<T> cross_product(const algebraic<T> &u, const algebraic<T> &v);



template<typename T>
class algebraic
{
public:
    algebraic();
    explicit algebraic(NumberField<T>*);  //zero element of NumberField

    algebraic(int n, NumberField<T>*); //standard basis element e_n in NumberField
    algebraic(vector<T> coords, NumberField<T>*);  //vector with coeffients coords in NumberField
    algebraic(T coords[], NumberField<T>*); 
    complex<COORD> tocomplex() const;
    algebraic<T> conjugate() const; // the complex conjugate
    algebraic<T> norm() const; // just the number times its complex conjugate, not the real norm

    algebraic<T> pow(int n) const; //nth power
   
    algebraic<T>& operator +=(const algebraic<T>&);
    algebraic<T>& operator *=(const algebraic<T>&);
    algebraic<T>& operator -=(const algebraic<T>&);
    algebraic<T>& operator /=(const algebraic<T>&);
    algebraic<T>& operator *=(T);
    algebraic<T>& operator /=(T);


    algebraic<T> reciprocal() const;


    friend  algebraic<T> operator *<>(algebraic<T>, T);
    friend  algebraic<T> operator *<>(T, algebraic<T>);
    friend algebraic<T> operator -<>(const algebraic<T> &r);
    //friend  algebraic<T> operator ^(algebraic<T>, int);
    friend ostream& operator << <>(ostream& outputStream, const algebraic<T>& num);
    friend bool operator== <>(const algebraic<T>&, const algebraic<T>&);
    friend bool operator!= <>(const algebraic<T>&, const algebraic<T>&);

    friend algebraic<bigrat>to_rational(const algebraic<int64_t> &p);
    friend algebraic<T>  cross_product<>(const algebraic<T> &u, const algebraic<T> &v);


    friend class NumberField<T>;
    NumberField<T>* field() const;

    vector<T> get_coords() { return coords; }; 
private:
    vector<T> coords;
    NumberField<T> *in_field;
};

template<typename T>
algebraic<T> operator +(algebraic<T>, const algebraic<T>&);
template<typename T>
algebraic<T> operator *(algebraic<T>, const algebraic<T>&);
template<typename T>
algebraic<T> operator -(algebraic<T>, const algebraic<T>&); 
template<typename T>
algebraic<T> operator /(algebraic<T>, const algebraic<T>&);

// what are the next 3 for?
algebraic<bigrat> operator /(const algebraic<bigrat>&, int);
algebraic<bigrat> operator *(const algebraic<bigrat>&, int);
algebraic<bigrat> operator *(int, const algebraic<bigrat>&);

template<typename T>
algebraic<T> operator *(algebraic<T>, const algebraic<T>&);



template<typename T>
class NumberField
{
public:
    NumberField<T>();
    NumberField<T>(T minimal_poly[], int degree);
    NumberField<T>(T minimal_poly[], int degree, complex<COORD> embedding);

    void store_conjugate(algebraic<T>);  

/*
    inline algebraic<T> zero() { return algebraic<T>(this); } //0 in F
    inline algebraic<T> one() { return algebraic<T>(0,this); } //1 in F
*/
    friend class algebraic<T>;
    void print_mt();  //for debugging: prints multiplication table
    void print(ostream &);
  
    //return here for more const stuff
    friend  algebraic<T> operator * <>(algebraic<T>, const algebraic<T>&);
    friend algebraic<T>  cross_product<>(const algebraic<T> &u, const algebraic<T> &v);

    poly<T> min_poly();
    static NumberField<T> *F; //everybody is in this field.
    static algebraic<T> F_zero; // 0 in F
    static algebraic<T> F_one; // 1 in F

    friend algebraic<bigrat> to_rational(const algebraic<int64_t> &p); //only works with given F


private: 
    algebraic<T> conjugate;         //a conjugate of generator of field.  Used by algebraic<T>::conjugate().
  complex<COORD> embedding;   //numerical value for generator of field.  Used by algebraic<T>::to_complex().
    void build_multiplication_table();
    void build_cross_product_table();
    poly<T> minimal_poly;       //minimal polynomial of field.  {a_0, ..., a_n} represents a_0 + ... + a_n x^n
    vector<vector<T> > multiplication_table;
    int degree;
    vector<algebraic<T> >conjugate_table;
    boost::numeric::ublas::matrix<algebraic<T> >cross_product_table;
//    vector<vector<algebraic<T> > >cross_product_table;

};

template<typename T>
NumberField<T>* NumberField<T>::F;

template<typename T>
algebraic<T> NumberField<T>::F_zero; // 0 in F
template<typename T>
algebraic<T> NumberField<T>::F_one; // 1 in F



//NumberField<int>* NumberField<int>::F;
//NumberField<bigrat>* NumberField<bigrat>::F;



complex<COORD> exp(complex<COORD>, int);

/*
template<typename T>
vector<T>& operator+=(vector<T>& v, vector<T> w);
template<typename T>
vector<T> operator+(vector<T> v, vector<T> w);
template<typename T>
vector<T> operator*(T v, vector<T> w);
*/

template<typename T>
NumberField<T>* InitCyclotomic(int n);
template<typename T>
poly<T> cyclotomic_poly(int n);
template<class T>
poly<T> operator%(const poly<T> &p, const poly<T> &q);

template<class T>
algebraic<T> operator /(algebraic<T> a, const algebraic<T> &b);

template<class T> 
algebraic<T> operator /(algebraic<T> a, T b);

template<class T>
vector<T>& operator+=(vector<T>& v, vector<T> w);

template<class T>
vector<T> operator+(vector<T> v, vector<T> w);

template<class T>
algebraic<T> operator *(algebraic<T> a, const algebraic<T> &b);

template<class T>
algebraic<T> operator *(T a, algebraic<T> b);

template<class T>
algebraic<T> operator *(algebraic<T> b, T a);

template<class T> 
algebraic<T> operator /(algebraic<T> a, T b);

template<class T>
vector<T> operator*(T v, vector<T> w);

template<class T>
void gcd_extended(poly<T>& x, poly<T>& y, const poly<T> &a, const poly<T> &b);

inline algebraic<bigrat> to_rational(const algebraic<int64_t> &p) //only works with given F
{

    assert( p.in_field == NumberField<int64_t>::F );

    vector<bigrat> new_coords;
    for(int i=0; i < p.in_field->degree; i++) {
        static_assert(std::is_convertible<int64_t, long>::value, "mpz does not implement conversion from long long, so we can only initialize like this if long and int64_t have the same size; note that they might not the same type as long == long long and only one of the two can be aliased to int64_t");
	    bigrat r = bigrat((long)p.coords[i], 1);
	    new_coords.push_back(r);
    }


    algebraic<bigrat> q = algebraic<bigrat>(new_coords, 
					     NumberField<bigrat>::F);

    return q;
    
};








#endif  // LIBPOLYGON_ALGEBRAIC_H
