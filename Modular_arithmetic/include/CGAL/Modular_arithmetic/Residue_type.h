// Copyright (c) 2007  INRIA Sophia-Antipolis (France), Max-Planck-Institute
// Saarbruecken (Germany).
// All rights reserved.
//
// This file is part of CGAL (www.cgal.org); you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License as
// published by the Free Software Foundation; version 2.1 of the License.
// See the file LICENSE.LGPL distributed with CGAL.
//
// Licensees holding a valid commercial license may use this file in
// accordance with the commercial license agreement provided with the software.
//
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
//
// $URL:$
// $Id:$
//
// Author(s)     : Sylvain Pion, Michael Hemmer

#ifndef CGAL_RESIDUE_TYPE_H
#define CGAL_RESIDUE_TYPE_H

#include <CGAL/basic.h>

#include <cfloat>
#include <boost/operators.hpp>

CGAL_BEGIN_NAMESPACE

// fwd
void force_ieee_double_precision(); 

namespace CGALi{

struct Modular_arithmetic_needs_ieee_double_precision{
    Modular_arithmetic_needs_ieee_double_precision(){
        CGAL::force_ieee_double_precision();
    }
};
}

class Residue;
    
Residue operator + (const Residue&);
Residue operator - (const Residue&);

std::ostream& operator << (std::ostream& os, const Residue& p);
std::istream& operator >> (std::istream& is, Residue& p);

/*! \ingroup CGAL_Modular_traits
 * \brief This class represents the Field Z mod p. 
 *  
 * This class uses the type double for representation. 
 * Therefore the value of p is restricted to primes less than 2^26.
 * By default p is set to 67111067.
 *
 * It provides the standard operators +,-,*,/ as well as in&output.
 * 
 * \see Modular_traits
 */
class Residue:
    boost::ordered_field_operators1< Residue,
    boost::ordered_field_operators2< Residue, int > >{
    
public:
    typedef Residue Self;
    typedef Residue NT;

private:
    static const double  CST_CUT; 
    static const CGALi::Modular_arithmetic_needs_ieee_double_precision 
    modular_arithmetic_needs_ieee_double_precision;
private:
   
    static int prime_int;
    static double prime;
    static double prime_inv;
    
    
    /* Quick integer rounding, valid if a<2^51. for double */ 
    static inline 
    double RES_round (double a){
#ifdef CGAL_USE_LEDA 
        return ( (a + CST_CUT)  - CST_CUT); 
#else
     // TODO: 
     // In order to get rid of the volatile double
     // one should call: 
     // CGAL/FPU.h : inline void force_ieee_double_precision()
     // the problem is where and when it should be called ? 
     // and whether on should restore the old behaviour 
     // since it changes the global behaviour of doubles. 
     // Note that this code works if LEDA is present, since leda automatically 
     // changes this behaviour in the desired way. 
        volatile double b = (a + CST_CUT);
        return b - CST_CUT;
#endif
    }


    /* Big modular reduction (e.g. after multiplication) */
    static inline 
    double RES_reduce (double a){
        return a - prime * RES_round(a * prime_inv);
    }


    /* Little modular reduction (e.g. after a simple addition). */
    static inline 
    double RES_soft_reduce (double a){
        double b = 2*a;
        return (b>prime) ? a-prime :
            ((b<-prime) ? a+prime : a);
    }
    
    /* -a */
    static inline 
    double RES_negate(double a){
        return RES_soft_reduce(-a);
    }


    /* a*b */
    static inline 
    double RES_mul (double a, double b){
        double c = a*b;
        return RES_reduce(c);
    }


    /* a+b */
    static inline 
    double RES_add (double a, double b){
        double c = a+b;
        return RES_soft_reduce(c);
    }

    
    /* a^-1, using Bezout (extended Euclidian algorithm). */
    static inline 
    double RES_inv (double ri1){
        double bi = 0.0;
        double bi1 = 1.0;
        double ri = prime;
        double p, tmp, tmp2;
    
        Real_embeddable_traits<double>::Abs double_abs;
        while (double_abs(ri1) != 1.0)
        {
            p = RES_round(ri/ri1);
            tmp = bi - p * bi1;
            tmp2 = ri - p * ri1;
            bi = bi1;
            ri = ri1;
            bi1 = tmp;
            ri1 = tmp2;
        };

        return ri1 * RES_soft_reduce(bi1);	/* Quicker !!!! */
    }
    
    /* a/b */
    static inline 
    double RES_div (double a, double b){
        return RES_mul(a, RES_inv(b));
    }    

public:
    /*! \brief sets the current prime. 
     *  
     *  Note that you are going to change a static member!
     *  \pre p is prime, but we abstained from such a test.
     *  \pre 0 < p < 2^26
     *  
     */
    static int 
    set_current_prime(int p){       
        int old_prime = prime_int; 
        prime_int = p;
        prime = (double)p;
        prime_inv = (double)1/prime;
        return old_prime; 
    }
     /*! \brief return the current prime.  */
    static int get_current_prime(){
        return prime_int;
    }
    int  get_value() const{
        return int(x_);
    }
    
private:
    double x_;

public: 

    //! constructor of Residue, from int 
    Residue(int n = 0){
        x_= RES_reduce(n);
    }

    //! constructor of Residue, from long 
    Residue(long n){
        x_= RES_reduce(n);
    }
   
    //! Access operator for x, \c const 
    const double& x() const { return x_; }
    //! Access operator for x
    double&       x()       { return x_; }                     

    Self& operator += (const Self& p2) { 
        x() = RES_add(x(),p2.x()); 
        return (*this); 
    }
    Self& operator -= (const Self& p2){ 
        x() = RES_add(x(),RES_negate(p2.x())); 
        return (*this); 
    }
    Self& operator *= (const Self& p2){ 
        x() = RES_mul(x(),p2.x()); 
        return (*this); 
    }
    Self& operator /= (const Self& p2) { 
        x() = RES_div(x(),p2.x()); 
        return (*this); 
    }
    // 
    Self& operator += (int p2) { 
        x() = RES_add(x(),Residue(p2).x()); 
        return (*this); 
    }
    Self& operator -= (int p2){ 
        x() = RES_add(x(),Residue(-p2).x()); 
        return (*this); 
    }

    Self& operator *= (int p2){ 
        x() = RES_mul(x(),Residue(p2).x()); 
        return (*this); 
    }

    Self& operator /= (int p2) { 
        x() = RES_div(x(),Residue(p2).x()); 
        return (*this); 
    }
  
    friend Self operator + (const Self&);
    friend Self operator - (const Self&);                
};

inline Residue operator + (const Residue& p1)
{ return p1; }

inline Residue operator - (const Residue& p1){ 
    typedef Residue RES;
    Residue r; 
    r.x() = RES::RES_negate(p1.x());
    return r; 
}

inline bool operator == (const Residue& p1, const Residue& p2)
{ return ( p1.x()==p2.x() ); }   
inline bool operator == (const Residue& p1, int p2)
{ return ( p1 == Residue(p2) ); }   


inline bool operator < (const Residue& p1, const Residue& p2)
{ return ( p1.x() < p2.x() ); }   
inline bool operator < (const Residue& p1, int p2)
{ return ( p1.x() < Residue(p2).x() ); }   


// I/O 
inline std::ostream& operator << (std::ostream& os, const Residue& p) {   
    typedef Residue RES;
    os <<"("<< int(p.x())<<"%"<<RES::get_current_prime()<<")";
    return os;
}

inline std::istream& operator >> (std::istream& is, Residue& p) {
    typedef Residue RES;
    char ch;
    int prime;

    is >> p.x();
    is >> ch;    // read the %
    is >> prime; // read the prime
    CGAL_precondition(prime==RES::get_current_prime());
    return is;
}

CGAL_END_NAMESPACE

#endif // CGAL_RESIDUE_TYPE_H
