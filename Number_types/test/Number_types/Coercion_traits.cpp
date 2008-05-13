// TODO: Add licence
//
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
//
// $URL$
// $Id$
// 
//
// Author(s)     : Michael Hemmer <mhemmer@uni-mainz.de>
//
// ============================================================================


#include <CGAL/basic.h>
#include <cassert>
#include <CGAL/Arithmetic_kernel.h>

#include <CGAL/Coercion_traits.h>
#include <CGAL/Sqrt_extension.h>
#include <CGAL/Quotient.h>
#include <CGAL/Lazy_exact_nt.h>
#include <CGAL/Interval_nt.h>

//#include <NiX/Polynomial.h>
//#include <NiX/Compactified.h>
//#include <NiX/Tendency.h>
//#include <CGAL/Vector_d.h> // TODO: Is this available in CGAL? CGAL/Vector_d?
//#include <NiX/Matrix_d.h>

#include <CGAL/Test/_test_coercion_traits.h>

typedef CGAL::Interval_nt<false> Interval;  

void test_coercion_from_to(CGAL::Null_tag, CGAL::Null_tag){};
template<class A> void test_coercion_from_to(A, CGAL::Null_tag){};
template<class B> void test_coercion_from_to(CGAL::Null_tag, B){};
template<class A, class B> void test_coercion_from_to(A, B){
    CGAL::INTERN_COERCION_TRAITS::direct_coercion_from_to_test<A,B>();
};

template <class AT> 
void check_type_coercion_at(){
    typedef typename AT::Integer Integer; 
    typedef typename AT::Rational Rational; 
    typedef typename AT::Field_with_sqrt Field_with_sqrt; 
    typedef typename AT::Field_with_kth_root Field_with_kth_root; 
    typedef typename AT::Field_with_root_of  Field_with_root_of; 
    typedef typename AT::Bigfloat Bigfloat;
    typedef typename AT::Bigfloat_interval Bigfloat_interval;
    
    
    test_coercion_from_to(int(),Integer());
    test_coercion_from_to(short(),Integer());
    test_coercion_from_to(Integer(),Integer());

    test_coercion_from_to(int(),Rational());
    test_coercion_from_to(short(),Rational());
    test_coercion_from_to(float(),Rational());
    test_coercion_from_to(double(),Rational());
    test_coercion_from_to(Integer(),Rational());
    test_coercion_from_to(Bigfloat(),Rational());
    test_coercion_from_to(Rational(),Rational());

    test_coercion_from_to(int(),Field_with_sqrt());
    test_coercion_from_to(short(),Field_with_sqrt());
    test_coercion_from_to(float(),Field_with_sqrt());
    test_coercion_from_to(double(),Field_with_sqrt());
    test_coercion_from_to(Integer(),Field_with_sqrt());
    test_coercion_from_to(Bigfloat(),Field_with_sqrt());
    test_coercion_from_to(Rational(),Field_with_sqrt());
    test_coercion_from_to(Field_with_sqrt(),Field_with_sqrt());

    test_coercion_from_to(int(),Field_with_kth_root());
    test_coercion_from_to(short(),Field_with_kth_root());
    test_coercion_from_to(float(),Field_with_kth_root());
    test_coercion_from_to(double(),Field_with_kth_root());
    test_coercion_from_to(Integer(),Field_with_kth_root());
    test_coercion_from_to(Bigfloat(),Field_with_kth_root());
    test_coercion_from_to(Rational(),Field_with_kth_root());
    test_coercion_from_to(Field_with_sqrt(),Field_with_kth_root());
    test_coercion_from_to(Field_with_kth_root(),Field_with_kth_root());


    test_coercion_from_to(int(),Field_with_root_of());
    test_coercion_from_to(short(),Field_with_root_of());
    test_coercion_from_to(float(),Field_with_root_of());
    test_coercion_from_to(double(),Field_with_root_of());
    test_coercion_from_to(Integer(),Field_with_root_of());
    test_coercion_from_to(Bigfloat(),Field_with_root_of());
    test_coercion_from_to(Rational(),Field_with_root_of());
    test_coercion_from_to(Field_with_sqrt(),Field_with_root_of());
    test_coercion_from_to(Field_with_kth_root(),Field_with_root_of());
    test_coercion_from_to(Field_with_root_of(),Field_with_root_of());


    test_coercion_from_to(int(),Bigfloat());
    test_coercion_from_to(short(),Bigfloat());
    test_coercion_from_to(float(),Bigfloat());
    test_coercion_from_to(double(),Bigfloat());
    test_coercion_from_to(Integer(),Bigfloat());
    test_coercion_from_to(Bigfloat(),Bigfloat());


    test_coercion_from_to(int(),Bigfloat_interval());
    test_coercion_from_to(short(),Bigfloat_interval());
    test_coercion_from_to(float(),Bigfloat_interval());
    test_coercion_from_to(double(),Bigfloat_interval());
    test_coercion_from_to(Integer(),Bigfloat_interval());
    test_coercion_from_to(Bigfloat(),Bigfloat_interval());
    test_coercion_from_to(Rational(),Bigfloat_interval());
    test_coercion_from_to(Field_with_sqrt(),Bigfloat_interval());
    test_coercion_from_to(Field_with_kth_root(),Bigfloat_interval());
    test_coercion_from_to(Field_with_root_of(),Bigfloat_interval());
    test_coercion_from_to(Bigfloat_interval(),Bigfloat_interval());
     
}


template <class AT> 
void AT_coercion_test_for_cgal_types_int(){
    typedef typename AT::Integer Integer;
    CGAL::INTERN_COERCION_TRAITS::direct_coercion_from_to_test<int    ,Integer>();
    CGAL::INTERN_COERCION_TRAITS::direct_coercion_from_to_test<Integer,Integer>();
    CGAL::INTERN_COERCION_TRAITS::direct_coercion_from_to_test<Integer,Interval>();

    // Quotient
    typedef CGAL::Quotient<Integer> Quotient;

    CGAL::INTERN_COERCION_TRAITS::direct_coercion_from_to_test<int     ,Quotient>();
    CGAL::INTERN_COERCION_TRAITS::direct_coercion_from_to_test<double  ,Quotient>();
    CGAL::INTERN_COERCION_TRAITS::direct_coercion_from_to_test<Integer ,Quotient>();
    CGAL::INTERN_COERCION_TRAITS::direct_coercion_from_to_test<Quotient,Quotient>();
    CGAL::INTERN_COERCION_TRAITS::direct_coercion_from_to_test<Quotient,Interval>();

    typedef CGAL::Sqrt_extension<Integer, Integer> Extn_1;
    CGAL::INTERN_COERCION_TRAITS::direct_coercion_from_to_test<int    ,Extn_1>();
    CGAL::INTERN_COERCION_TRAITS::direct_coercion_from_to_test<Integer,Extn_1>();
    CGAL::INTERN_COERCION_TRAITS::direct_coercion_from_to_test<Extn_1 ,Extn_1>();
    CGAL::INTERN_COERCION_TRAITS::direct_coercion_from_to_test<Extn_1,Interval>();

    typedef CGAL::Sqrt_extension<Extn_1,  Extn_1 > Extn_2n;
    CGAL::INTERN_COERCION_TRAITS::direct_coercion_from_to_test<int    ,Extn_2n>();
    CGAL::INTERN_COERCION_TRAITS::direct_coercion_from_to_test<Integer,Extn_2n>();
    CGAL::INTERN_COERCION_TRAITS::direct_coercion_from_to_test<Extn_1 ,Extn_2n>();
    CGAL::INTERN_COERCION_TRAITS::direct_coercion_from_to_test<Extn_2n,Extn_2n>();
    CGAL::INTERN_COERCION_TRAITS::direct_coercion_from_to_test<Extn_2n,Interval>();
    
    typedef CGAL::Sqrt_extension<Extn_1,  Integer> Extn_2d;
    CGAL::INTERN_COERCION_TRAITS::direct_coercion_from_to_test<int    ,Extn_2d>();
    CGAL::INTERN_COERCION_TRAITS::direct_coercion_from_to_test<Integer,Extn_2d>();
    CGAL::INTERN_COERCION_TRAITS::direct_coercion_from_to_test<Extn_1 ,Extn_2d>();
    CGAL::INTERN_COERCION_TRAITS::direct_coercion_from_to_test<Extn_2d,Extn_2d>();
    CGAL::INTERN_COERCION_TRAITS::direct_coercion_from_to_test<Extn_2d,Interval>();
}

template <class AT> 
void AT_coercion_test_for_cgal_types_rat(){
    
    AT_coercion_test_for_cgal_types_int<AT>();

    typedef typename AT::Integer Integer;
    typedef typename AT::Rational Rational;

    CGAL::INTERN_COERCION_TRAITS::direct_coercion_from_to_test<int     ,Rational>();
    CGAL::INTERN_COERCION_TRAITS::direct_coercion_from_to_test<double  ,Rational>();
    CGAL::INTERN_COERCION_TRAITS::direct_coercion_from_to_test<Integer ,Rational>();
    CGAL::INTERN_COERCION_TRAITS::direct_coercion_from_to_test<Rational,Rational>();
    CGAL::INTERN_COERCION_TRAITS::direct_coercion_from_to_test<Rational,Interval>();

    
    typedef CGAL::Sqrt_extension<Integer  , Integer> Extn_1;  
    typedef CGAL::Sqrt_extension<Extn_1   , Extn_1 > Extn_2n;
    typedef CGAL::Sqrt_extension<Extn_1   , Integer> Extn_2d;

    typedef CGAL::Sqrt_extension<Rational  , Integer> Extn_rat_int; 
    CGAL::INTERN_COERCION_TRAITS::direct_coercion_from_to_test<int         ,Extn_rat_int>();
//  CGAL::INTERN_COERCION_TRAITS::direct_coercion_from_to_test<double      ,Extn_rat_int>();
    CGAL::INTERN_COERCION_TRAITS::direct_coercion_from_to_test<Integer     ,Extn_rat_int>();
    CGAL::INTERN_COERCION_TRAITS::direct_coercion_from_to_test<Rational    ,Extn_rat_int>();
    CGAL::INTERN_COERCION_TRAITS::direct_coercion_from_to_test<Extn_1      ,Extn_rat_int>();
    CGAL::INTERN_COERCION_TRAITS::direct_coercion_from_to_test<Extn_rat_int,Extn_rat_int>();
        
    typedef CGAL::Sqrt_extension<Rational  , Rational> Extn_rat_1;
    CGAL::INTERN_COERCION_TRAITS::direct_coercion_from_to_test<int         ,Extn_rat_1>();
//  CGAL::INTERN_COERCION_TRAITS::direct_coercion_from_to_test<double      ,Extn_rat_1>();
    CGAL::INTERN_COERCION_TRAITS::direct_coercion_from_to_test<Integer     ,Extn_rat_1>();
    CGAL::INTERN_COERCION_TRAITS::direct_coercion_from_to_test<Rational    ,Extn_rat_1>();
//  CGAL::INTERN_COERCION_TRAITS::direct_coercion_from_to_test<Extn_1      ,Extn_rat_1>();
    CGAL::INTERN_COERCION_TRAITS::direct_coercion_from_to_test<Extn_rat_1,Extn_rat_1>();

    typedef CGAL::Sqrt_extension<Extn_rat_1, Extn_rat_1 > Extn_rat_2n;
    CGAL::INTERN_COERCION_TRAITS::direct_coercion_from_to_test<int         ,Extn_rat_2n>();
//  CGAL::INTERN_COERCION_TRAITS::direct_coercion_from_to_test<double      ,Extn_rat_2n>();
    CGAL::INTERN_COERCION_TRAITS::direct_coercion_from_to_test<Integer     ,Extn_rat_2n>();
    CGAL::INTERN_COERCION_TRAITS::direct_coercion_from_to_test<Rational    ,Extn_rat_2n>();
    CGAL::INTERN_COERCION_TRAITS::direct_coercion_from_to_test<Extn_rat_1  ,Extn_rat_2n>();
    CGAL::INTERN_COERCION_TRAITS::direct_coercion_from_to_test<Extn_rat_2n ,Extn_rat_2n>();

    typedef CGAL::Sqrt_extension<Extn_rat_1, Rational> Extn_rat_2d;
    CGAL::INTERN_COERCION_TRAITS::direct_coercion_from_to_test<int         ,Extn_rat_2d>();
//  CGAL::INTERN_COERCION_TRAITS::direct_coercion_from_to_test<double      ,Extn_rat_2d>();
    CGAL::INTERN_COERCION_TRAITS::direct_coercion_from_to_test<Integer     ,Extn_rat_2d>();
    CGAL::INTERN_COERCION_TRAITS::direct_coercion_from_to_test<Rational    ,Extn_rat_2d>();
    CGAL::INTERN_COERCION_TRAITS::direct_coercion_from_to_test<Extn_rat_1  ,Extn_rat_2d>();
    CGAL::INTERN_COERCION_TRAITS::direct_coercion_from_to_test<Extn_rat_2d ,Extn_rat_2d>();    
}


template <class AT> 
void AT_coercion_test_for_cgal_types_fws(){
    AT_coercion_test_for_cgal_types_rat<AT>();
    
    typedef typename AT::Integer Integer;
    typedef typename AT::Rational Rational;
    typedef typename AT::Field_with_sqrt Real;

    typedef CGAL::Sqrt_extension<Integer  , Integer> Extn_1;  
    typedef CGAL::Sqrt_extension<Extn_1   , Extn_1 > Extn_2n;
    typedef CGAL::Sqrt_extension<Extn_1   , Integer> Extn_2d;

    typedef CGAL::Sqrt_extension<Rational  , Integer>     Extn_rat_int; 
    typedef CGAL::Sqrt_extension<Rational  , Rational>    Extn_rat_1;
    typedef CGAL::Sqrt_extension<Extn_rat_1, Extn_rat_1 > Extn_rat_2n;
    typedef CGAL::Sqrt_extension<Extn_rat_1, Rational>    Extn_rat_2d; 
    
    CGAL::INTERN_COERCION_TRAITS::direct_coercion_from_to_test<int         ,Real>();
    CGAL::INTERN_COERCION_TRAITS::direct_coercion_from_to_test<double      ,Real>();
    CGAL::INTERN_COERCION_TRAITS::direct_coercion_from_to_test<Integer     ,Real>();
    CGAL::INTERN_COERCION_TRAITS::direct_coercion_from_to_test<Rational    ,Real>();
    CGAL::INTERN_COERCION_TRAITS::direct_coercion_from_to_test<Extn_1      ,Real>();
    CGAL::INTERN_COERCION_TRAITS::direct_coercion_from_to_test<Extn_2n     ,Real>();
    CGAL::INTERN_COERCION_TRAITS::direct_coercion_from_to_test<Extn_2d     ,Real>();
    CGAL::INTERN_COERCION_TRAITS::direct_coercion_from_to_test<Extn_rat_int,Real>();
    CGAL::INTERN_COERCION_TRAITS::direct_coercion_from_to_test<Extn_rat_1  ,Real>();
    CGAL::INTERN_COERCION_TRAITS::direct_coercion_from_to_test<Extn_rat_2n ,Real>();
    CGAL::INTERN_COERCION_TRAITS::direct_coercion_from_to_test<Extn_rat_2d ,Real>();

  // direct casts 
  // Integer;
    CGAL::INTERN_COERCION_TRAITS::direct_coercion_from_to_test<Integer,Interval>();
  
  // Rational
    CGAL::INTERN_COERCION_TRAITS::direct_coercion_from_to_test<Rational,Interval>();
  
  // Interval
    CGAL::INTERN_COERCION_TRAITS::direct_coercion_from_to_test<Interval,Interval>();
}

// TODO: We have no NT compactified, Matrix, Vector, etc. yet
/*void test_compactified(){
    typedef NiX::Compactified<int>  Compactified_int;
    typedef NiX::Compactified<long> Compactified_long;
    
    Compactified_int zero(0);
    Compactified_int one(1);
    Compactified_int p_infty(NiX::PLUS_INFTY);
    Compactified_int m_infty(NiX::MINUS_INFTY);

    {
        typedef NiX::Coercion_traits<Compactified_int,Compactified_int> CT;
        CT::Cast cast;
        assert(cast(one) == Compactified_int(1));
        assert(cast(p_infty) == Compactified_int(NiX::PLUS_INFTY));
        assert(cast(m_infty) == Compactified_int(NiX::MINUS_INFTY));
        assert(cast(zero) == Compactified_int(0));
    }{
        typedef NiX::Coercion_traits<Compactified_int,Compactified_long> CT;
        CT::Cast cast;
        assert(cast(Compactified_long(1)) == Compactified_long(1));
        assert(cast(one) == Compactified_long(1));
        assert(cast(p_infty) == Compactified_long(NiX::PLUS_INFTY));
        assert(cast(m_infty) == Compactified_long(NiX::MINUS_INFTY));
        assert(cast(zero) == Compactified_long(0));
    }{
        typedef NiX::Coercion_traits<Compactified_long,Compactified_int> CT;
        CT::Cast cast;
        assert(cast(Compactified_long(1)) == Compactified_long(1));
        assert(cast(one) == Compactified_long(1));
        assert(cast(p_infty) == Compactified_long(NiX::PLUS_INFTY));
        assert(cast(m_infty) == Compactified_long(NiX::MINUS_INFTY));
        assert(cast(zero) == Compactified_long(0));
    }
}

void test_tendency(){
    typedef NiX::Tendency<int>  Tendency_int;
    typedef NiX::Tendency<long> Tendency_long;
    
    Tendency_int zero(0);
 
    Tendency_int one_me (1,Tendency_int::MINUS_EPSILON);
    Tendency_int one_mee(1,Tendency_int::MINUS_SQUARED_EPSILON);
    Tendency_int one    (1,Tendency_int::NONE);  
    Tendency_int one_pee(1,Tendency_int::PLUS_SQUARED_EPSILON);
    Tendency_int one_pe (1,Tendency_int::PLUS_EPSILON);

    {
        typedef NiX::Coercion_traits<Tendency_int,Tendency_int> CT; 
        CT::Cast cast;
        assert(cast(zero) == Tendency_int(0));
        assert(cast(one) == Tendency_int(1));
        assert(cast(one_me)  == Tendency_int(1,Tendency_int::MINUS_EPSILON));
        assert(cast(one_mee) ==Tendency_int(1,Tendency_int::MINUS_SQUARED_EPSILON));
        assert(cast(one)     ==Tendency_int(1,Tendency_int::NONE));
        assert(cast(one_pee) ==Tendency_int(1,Tendency_int::PLUS_SQUARED_EPSILON));
        assert(cast(one_pe)  ==Tendency_int(1,Tendency_int::PLUS_EPSILON));
       
    }{
        typedef NiX::Coercion_traits<Tendency_int,Tendency_long> CT;        
        CT::Cast cast;
        assert(cast(Tendency_long(0))    == Tendency_long(0));
        assert(cast(zero) == Tendency_long(0));
        assert(cast(one) == Tendency_long(1));
        assert(cast(one_me)  == Tendency_long(1,Tendency_long::MINUS_EPSILON));
        assert(cast(one_mee) ==Tendency_long(1,Tendency_long::MINUS_SQUARED_EPSILON));
        assert(cast(one)     ==Tendency_long(1,Tendency_long::NONE));
        assert(cast(one_pee) ==Tendency_long(1,Tendency_long::PLUS_SQUARED_EPSILON));
        assert(cast(one_pe)  ==Tendency_long(1,Tendency_long::PLUS_EPSILON));
       
    }{
        typedef NiX::Coercion_traits<Tendency_long,Tendency_int> CT;        
        CT::Cast cast;
        assert(cast(Tendency_long(0))    == Tendency_long(0));
        assert(cast(zero)    == Tendency_long(0));
        assert(cast(one)     == Tendency_long(1));
        assert(cast(one_me)  ==Tendency_long(1,Tendency_long::MINUS_EPSILON));
        assert(cast(one_mee) ==Tendency_long(1,Tendency_long::MINUS_SQUARED_EPSILON));
        assert(cast(one)     ==Tendency_long(1,Tendency_long::NONE));
        assert(cast(one_pee) ==Tendency_long(1,Tendency_long::PLUS_SQUARED_EPSILON));
        assert(cast(one_pe)  ==Tendency_long(1,Tendency_long::PLUS_EPSILON));
    }
}

void test_matrix_d(){
    typedef NiX::Matrix_d<int > M_int;
    typedef NiX::Matrix_d<long> M_long;
    
    std::vector<int> input;
    for(int i= 0; i< 4 ;i++) input.push_back(i);
    M_int::Element_range erange_int ;
    M_long::Element_range erange_long ;
    
    M_int  m_int (erange_int ,2,2,input.begin(),input.end());   
    M_long m_long(erange_long,2,2,input.begin(),input.end());
    
    {
        typedef NiX::Coercion_traits<M_int,M_int> CT; 
        CT::Cast cast;
        assert(cast(m_int) == m_int);
    }{
        typedef NiX::Coercion_traits<M_long,M_int> CT; 
        CT::Cast cast;
        assert(cast(m_int) == m_long);
        assert(cast(m_long) == m_long);
    }{
        typedef NiX::Coercion_traits<M_int,M_long> CT; 
        CT::Cast cast;
        assert(cast(m_int) == m_long);
        assert(cast(m_long) == m_long);
    }    
}

void test_vector_d(){
    typedef NiX::Vector_d<int > V_int;
    typedef NiX::Vector_d<long> V_long;

    
    V_int  v_int (2); v_int [0] = 0;v_int [1] = 3;
    V_long v_long(2); v_long[0] = 0;v_long[1] = 3;
  
    
    {
        typedef NiX::Coercion_traits<V_int,V_int> CT; 
        CT::Cast cast;
        assert(cast(v_int) == v_int);
    }{
        typedef NiX::Coercion_traits<V_long,V_int> CT; 
        CT::Cast cast;
        assert(cast(v_int) == v_long);
        assert(cast(v_long) == v_long);
    }{
        typedef NiX::Coercion_traits<V_int,V_long> CT; 
        CT::Cast cast;
        assert(cast(v_int) == v_long);
        assert(cast(v_long) == v_long);
    }    
}*/

int main(){
    
    CGAL::INTERN_COERCION_TRAITS::direct_coercion_from_to_test<short,int>();
    CGAL::INTERN_COERCION_TRAITS::direct_coercion_from_to_test<short,long>();
    CGAL::INTERN_COERCION_TRAITS::direct_coercion_from_to_test<short,long long>();
    CGAL::INTERN_COERCION_TRAITS::direct_coercion_from_to_test<short,float>();
    CGAL::INTERN_COERCION_TRAITS::direct_coercion_from_to_test<short,double>();
    CGAL::INTERN_COERCION_TRAITS::direct_coercion_from_to_test<short,long double>();
    
    CGAL::INTERN_COERCION_TRAITS::direct_coercion_from_to_test<int,long>();
    CGAL::INTERN_COERCION_TRAITS::direct_coercion_from_to_test<int,long long>();
    CGAL::INTERN_COERCION_TRAITS::direct_coercion_from_to_test<int,float>();
    CGAL::INTERN_COERCION_TRAITS::direct_coercion_from_to_test<int,double>();
    CGAL::INTERN_COERCION_TRAITS::direct_coercion_from_to_test<int,long double>();
    
   
    CGAL::INTERN_COERCION_TRAITS::direct_coercion_from_to_test<long,long long>();
    CGAL::INTERN_COERCION_TRAITS::direct_coercion_from_to_test<long,float>();
    CGAL::INTERN_COERCION_TRAITS::direct_coercion_from_to_test<long,double>();
    CGAL::INTERN_COERCION_TRAITS::direct_coercion_from_to_test<long,long double>();
    
  
    CGAL::INTERN_COERCION_TRAITS::direct_coercion_from_to_test<long long,float>();
    CGAL::INTERN_COERCION_TRAITS::direct_coercion_from_to_test<long long,double>();
    CGAL::INTERN_COERCION_TRAITS::direct_coercion_from_to_test<long long,long double>();

    
    CGAL::INTERN_COERCION_TRAITS::direct_coercion_from_to_test<float,double>();
    CGAL::INTERN_COERCION_TRAITS::direct_coercion_from_to_test<float,long double>();
    
   
    CGAL::INTERN_COERCION_TRAITS::direct_coercion_from_to_test<double,long double>();
    
    CGAL::INTERN_COERCION_TRAITS::direct_coercion_from_to_test<short,short>();
    CGAL::INTERN_COERCION_TRAITS::direct_coercion_from_to_test<int,int>();   
    CGAL::INTERN_COERCION_TRAITS::direct_coercion_from_to_test<long,long>();
    CGAL::INTERN_COERCION_TRAITS::direct_coercion_from_to_test<long long,long long>();
    CGAL::INTERN_COERCION_TRAITS::direct_coercion_from_to_test<float,float>();
    CGAL::INTERN_COERCION_TRAITS::direct_coercion_from_to_test<double,double>();
    CGAL::INTERN_COERCION_TRAITS::direct_coercion_from_to_test<long double,long double>();

//    test_compactified();
//    test_tendency();
//    test_matrix_d();
//    test_vector_d();
   

#ifdef CGAL_HAVE_DEFAULT_ARITHMETIC_KERNEL
    AT_coercion_test_for_cgal_types_fws<CGAL::Arithmetic_kernel>();
#endif // CGAL_HAVE_DEFAULT_ARITHMETIC_KERNEL

#ifdef CGAL_USE_LEDA
  check_type_coercion_at<CGAL::LEDA_arithmetic_kernel>();
  AT_coercion_test_for_cgal_types_fws<CGAL::LEDA_arithmetic_kernel>();
#endif // CGAL_USE_LEDA

#ifdef CGAL_USE_CORE
  check_type_coercion_at<CGAL::CORE_arithmetic_kernel>();
  AT_coercion_test_for_cgal_types_fws<CGAL::CORE_arithmetic_kernel>();
#endif // CGAL_USE_CORE

#ifdef CGAL_USE_GMP
  check_type_coercion_at<CGAL::GMP_arithmetic_kernel>();
  AT_coercion_test_for_cgal_types_rat<CGAL::CORE_arithmetic_kernel>();
#endif // CGAL_USE_GMP

  return EXIT_SUCCESS;
}
