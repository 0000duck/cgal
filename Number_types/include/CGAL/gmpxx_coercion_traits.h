// TODO: Add licence
//
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
//
// $URL: $
// $Id: $
// 
//
// Author(s)     : Michael Hemmer  <mhemmer@uni-mainz.de>
//
// ============================================================================

/*! \file NiX/Gmp/Coercion_traits.h
 *  \brief Provides specializations of Coercion_traits for the Gmp types.
 */

#ifndef CGAL_GMPXX_COERCION_TRAITS_H
#define CGAL_GMPXX_COERCION_TRAITS_H 1

#include <CGAL/number_type_basic.h>
#include <CGAL/Coercion_traits.h>

#ifdef CGAL_USE_GMP

#include <gmpxx.h>
#include <mpfr.h>

CGAL_BEGIN_NAMESPACE

//mpz_class internal coercions: 
//self for mpz_class / mpq_class 
template <class T , class U>                                              
struct Coercion_traits<
  ::__gmp_expr< T , U>,::__gmp_expr< T , U>  >{                                
    typedef Tag_true  Are_explicit_interoperable;     
    typedef Tag_true  Are_implicit_interoperable;     
    typedef ::__gmp_expr<T , T> Type;                                          
    struct Cast{                                            
        typedef Type result_type;  
        template <class U3>
        Type operator()(const ::__gmp_expr< T , U3>& x) const { 
            return x;
        }       
    };                                                      
}; 

template <class T, class U1, class U2>                                              
struct Coercion_traits<
  ::__gmp_expr< T , U1>,::__gmp_expr< T , U2>  >{                                
    typedef Tag_true  Are_explicit_interoperable;     
    typedef Tag_true  Are_implicit_interoperable;     
    typedef ::__gmp_expr< T , T > Type;                                          
    struct Cast{                                            
        typedef Type result_type;  
        template <class U3>
        Type operator()(const ::__gmp_expr< T , U3>& x) const { 
            return x;
        }       
    };                                                      
};    


template <class T1 , class T2, class U1, class U2>  
struct Coercion_traits< ::__gmp_expr< T1 , U1>,::__gmp_expr< T2 , U2>  >{   
    typedef Tag_true  Are_explicit_interoperable;     
    typedef Tag_true  Are_implicit_interoperable;     
    typedef mpq_class Type;                                          
    struct Cast{                                            
        typedef Type result_type;  
        template <class T , class U>
        Type operator()(const ::__gmp_expr< T , U>& x) const { 
            return Type(x);
        }       
    };                                                      
};    
   
   
// gmpzq_class implicit interoperable with int  
template <class T, class U>                                              
struct Coercion_traits<
  ::__gmp_expr< T , U >, int >{                                
    typedef Tag_true  Are_explicit_interoperable;     
    typedef Tag_true  Are_implicit_interoperable;     
    typedef ::__gmp_expr< T , T > Type;                                          
    struct Cast{                                            
        typedef Type result_type;  
        template <class U3>
        Type operator()(const ::__gmp_expr< T , U3>& x) const { 
            return x;
        }      
        Type operator()(int x) const { return Type(x); }       
    };                                                      
};    
// gmpz_class implicit interoperable with int 
template <class U, class T>                                              
struct Coercion_traits< int , ::__gmp_expr< T , U> >
    :public Coercion_traits< ::__gmp_expr< T , U>, int >{}; 

CGAL_END_NAMESPACE

#endif // CGAL_USE_GMP
#endif //CGAL_GMPXX_COERCION_TRAITS_H 1
//EOF
