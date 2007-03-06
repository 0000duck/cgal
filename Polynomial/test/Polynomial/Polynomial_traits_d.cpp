#include <iostream>
#include <CGAL/basic.h>

#include <CGAL/Arithmetic_kernel.h>
#include <CGAL/Polynomial.h>
#include <CGAL/Polynomial_traits_d.h>

#include <CGAL/Testsuite/assert.h>

#include <CGAL/_test_basic.h>


#include <CGAL/Random.h>

static CGAL::Random my_rnd(346); // some seed 

#define CGAL_SNAP_POLYNOMIAL_TRAITS_D(T)                        \
    typedef T PT;                                               \
    typedef typename PT::Polynomial_d          Polynomial_d;    \
    typedef typename PT::Coefficient           Coeff;           \
    typedef typename PT::Innermost_coefficient ICoeff;          \
    typedef CGAL::Polynomial_traits_d<Coeff> PTC;               \
    typedef CGAL::Exponent_vector Exponent_vector;              \
    typedef std::pair< CGAL::Exponent_vector , ICoeff > Monom;  \
    typedef std::vector< Monom > Monom_rep;      

template <typename T>
inline
T my_ipower(const T& base, int expn) {
    // compute base^expn using square-and-multiply
    CGAL_precondition(expn >= 0);

    // handle trivial cases efficiently
    if (expn == 0) return T(1);
    if (expn == 1) return base;

    // find the most significant non-zero bit of expn
    int e = expn, msb = 0;
    while (e >>= 1) msb++;

    // computing base^expn by square-and-multiply
    T res = base;
    int b = 1<<msb;
    while (b >>= 1) { // is there another bit right of what we saw so far?
        res *= res;
        if (expn & b) res *= base;
    }
    return res;
}

template <class Polynomial_d_>
Polynomial_d_
generate_sparse_random_polynomial(int max_degree = 10 ){
    typedef CGAL::Polynomial_traits_d<Polynomial_d_> PT;
    CGAL_SNAP_POLYNOMIAL_TRAITS_D(PT);
    typename PT::Construct_polynomial construct; 


    typedef CGAL::Exponent_vector Exponent_vector;
    typedef std::pair< CGAL::Exponent_vector , ICoeff > Monom;
    typedef std::vector< Monom > Monom_rep;

    int range = 20;
    int number_of_variables = PT::d;
    int number_of_coeffs = 
        CGAL::min(number_of_variables * (int)ceil(log(max_degree+1))+1,4);
    
    Polynomial_d result; 
    for(int i = 0; i < number_of_coeffs; i++){
        CGAL::Exponent_vector exps(PT::d);
        for(int j = 0; j < PT::d; j++){
            exps[j]=my_rnd.get_int(0,max_degree);
        }
        ICoeff c = ICoeff(my_rnd.get_int(-range,range));
        Monom_rep monom_rep;
        monom_rep.push_back(Monom(exps,c));
        result += construct(monom_rep.begin(), monom_rep.end());
    }
    
    return result;
}

void  print_monom(std::vector<int> ev){
    if(ev.size() >= 1) 
        if (ev[0] != 0) 
            if (ev[0] ==1 ) 
                std::cout << "x";
            else
                std::cout << "x^" << ev[0];
    
    if(ev.size() >= 2) 
        if (ev[0] != 0) 
            if (ev[0] == 1 ) 
                std::cout << "y";
            else
                std::cout << "y^" << ev[1];
    if(ev.size() >= 3) std::cout << "z^" << ev[2];
    if(ev.size() >= 4) std::cout << "x^" << ev[3];
    
}

template <class Monom_rep>
void  print_monom_rep(Monom_rep monom_rep){
    std::sort(monom_rep.begin(), monom_rep.end());
    for( typename Monom_rep::iterator it = monom_rep.begin();
         it != monom_rep.end(); ++it){
        //if(it->second != 0){
            std::cout << it->second <<"*" ;
            print_monom(it->first);
            std::cout <<" + ";
            //}
    }
    std::cout << std::endl;
}



template <class Polynomial_traits_d>
void test_construct_polynomial(){
    std::cerr << "start test_construct_polynomial "; 
    std::cerr.flush();
    CGAL_SNAP_POLYNOMIAL_TRAITS_D(Polynomial_traits_d);
    
    const int d = PT::d;
    { // Construct_polynomial
        typedef typename PT::Construct_polynomial Constructor;
        BOOST_STATIC_ASSERT(!(boost::is_same< Constructor , CGAL::Null_functor >::value));
        typedef typename Constructor::result_type result_type;
        BOOST_STATIC_ASSERT( (boost::is_same< result_type , Polynomial_d >::value));
        Constructor construct;

        CGAL_test_assert( Polynomial_d() == construct()); 
        CGAL_test_assert( Polynomial_d(3) == construct(3)); 
        CGAL_test_assert( Polynomial_d(Coeff(3)) == construct(Coeff(3))); 
        CGAL_test_assert( Polynomial_d(ICoeff(3)) == construct(ICoeff(3))); 

        CGAL_test_assert( construct(Coeff(2)) != Polynomial_d(0)); 
        CGAL_test_assert( construct(Coeff(2)) == Polynomial_d(2)); 
        CGAL_test_assert( construct(Coeff(0),Coeff(1)) != Polynomial_d(1)); 
        CGAL_test_assert( construct() 
                == construct(Coeff(0)));
        CGAL_test_assert( construct(Coeff(1)) 
                == construct(Coeff(1),Coeff(0)));
        CGAL_test_assert( construct(Coeff(2),Coeff(1)) 
                == construct(Coeff(2),Coeff(1),Coeff(0)));
        CGAL_test_assert( construct(Coeff(3),Coeff(2),Coeff(1)) 
                == construct(Coeff(3),Coeff(2),Coeff(1),Coeff(0)));
        CGAL_test_assert( construct(Coeff(3),Coeff(2),Coeff(1)) 
                != construct(Coeff(3),Coeff(2),Coeff(1),Coeff(1)));
        // construct via iterator range
        std::vector<Coeff> coeffs;
        CGAL_test_assert(construct(coeffs.begin(),coeffs.end())== construct(0));
        for(int i = 0; i<4;i++){coeffs.push_back(Coeff(i));}
        CGAL_test_assert(construct(coeffs.begin(),coeffs.end() )
                == construct(Coeff(0),Coeff(1),Coeff(2),Coeff(3)));
        
        Monom_rep monom_rep;
        CGAL_test_assert(construct(monom_rep.begin(),monom_rep.end())== construct(0));
        CGAL::Random rnd(7);
        for(int j = 0; j < 2; j++){
            CGAL::Exponent_vector exps(d);
            for(int i = 0; i < d; i++){
                exps[i]=j+i*5;
            }
            monom_rep.push_back(Monom(exps,j+1));
        }
        //std::cout<<"\n"<<std::endl;
        //std::cout<<"monom_rep_1: "; print_monom_rep(monom_rep); 
       
        std::random_shuffle(monom_rep. begin(),monom_rep. end());
        Polynomial_d p1 = construct(monom_rep. begin(),
                monom_rep. begin()+((monom_rep. end()- monom_rep. begin())/2));
        Polynomial_d p2 = construct(monom_rep. begin()+
                ((monom_rep. end()- monom_rep. begin())/2), monom_rep. end());
        Polynomial_d p  = construct(monom_rep. begin(), monom_rep. end());
        CGAL_test_assert(p == p1+p2);
    
        CGAL_test_assert(construct(monom_rep. begin(),monom_rep. end()) 
                == construct(monom_rep.rbegin(),monom_rep.rend()));  
    }
    std::cerr << " ok "<< std::endl; 
}

template< class Polynomial_traits_d >
void test_get_coefficient() {
    std::cerr << "start test_get_coefficient ";
    std::cerr.flush();
    CGAL_SNAP_POLYNOMIAL_TRAITS_D( Polynomial_traits_d );
    
    typename PT::Construct_polynomial construct;
    typename PT::Get_coefficient get_coeff;
    
    Polynomial_d p = construct( Coeff(1), Coeff(2), Coeff(3) );
    CGAL_test_assert( get_coeff( p, 0 ) == Coeff(1) );
    CGAL_test_assert( get_coeff( p, 1 ) == Coeff(2) );
    CGAL_test_assert( get_coeff( p, 2 ) == Coeff(3) );
    CGAL_test_assert( get_coeff( p, 3 ) == Coeff(0) );
    
    std::cerr << " ok" << std::endl;
}

template< class Polynomial_traits_d >
void test_get_innermost_coefficient() {
    std::cerr << "start test_get_innermost_coefficient ";
    std::cerr.flush();
    CGAL_SNAP_POLYNOMIAL_TRAITS_D( Polynomial_traits_d );
    
    typename PT::Construct_polynomial construct;
    typename PT::Get_innermost_coefficient get_innermost_coeff;
    
    Polynomial_d p = construct( Coeff(1), Coeff(2), Coeff(3) );
    
    Exponent_vector ev;
    
    for( int i = 0; i < PT::d-1; ++i ) {
        ev.push_back( 0 );
    }
    
    ev.push_back( 0 );    
    CGAL_test_assert( get_innermost_coeff( p, ev ) == ICoeff(1) );
    
    ev.pop_back();
    ev.push_back( 1 );
    CGAL_test_assert( get_innermost_coeff( p, ev ) == ICoeff(2) );

    ev.pop_back();
    ev.push_back( 2 );
    CGAL_test_assert( get_innermost_coeff( p, ev ) == ICoeff(3) );

    ev.pop_back();
    ev.push_back( 3 );
    CGAL_test_assert( get_innermost_coeff( p, ev ) == ICoeff(0) );
    
    std::cerr << " ok" << std::endl;
}

template <class Polynomial_traits_d>
void test_get_monom_representation(){
    std::cerr << "start test_get_monom_representation "; 
    std::cerr.flush();
    CGAL_SNAP_POLYNOMIAL_TRAITS_D(Polynomial_traits_d);
    
    typename PT::Construct_polynomial construct;
    typename PT::Get_monom_representation gmr;
        
    for (int i = 0; i < 5 ; i++){
        Polynomial_d p,q;
        Monom_rep monom_rep;
        p = generate_sparse_random_polynomial<Polynomial_d>();
        gmr(p,std::back_inserter(monom_rep));
        q = construct(monom_rep.begin(), monom_rep.end());
        CGAL_test_assert(q == p);
        std::random_shuffle(monom_rep.begin(), monom_rep.end());
        q = construct(monom_rep.begin(), monom_rep.end());
        CGAL_test_assert(q == p);
    }
    std::cerr << " ok "<< std::endl; 
}




template <class Polynomial_traits_d>
void test_swap(){
    std::cerr << "start test_swap "; std::cerr.flush();
    CGAL_SNAP_POLYNOMIAL_TRAITS_D(Polynomial_traits_d);
    int d = PT::d;
    typename Polynomial_traits_d::Swap swap;
    
    //std::cout << "start_test ----------- "<< d << std::endl; 
    for(int i = 0; i < 5; i++){
        int i1 = my_rnd.get_int(0,d);
        int i2 = my_rnd.get_int(0,d);
        int i3 = my_rnd.get_int(0,d);
       
        Polynomial_d p,q;
        p = generate_sparse_random_polynomial<Polynomial_d>();
        q = swap(p,i1,i2);
        CGAL_test_assert(p!=q || i1 == i2 );
        q = swap(q,i1,i2);
        CGAL_test_assert(p==q);
        
        if(i1 != i2 && i1 != i3 && i2 != i3 ){
            q = swap(q,i1,i2);
            CGAL_test_assert(p!=q || i1 == i2 );
            q = swap(q,i1,i3);
            q = swap(q,i1,i2);
            q = swap(q,i2,i3);
            CGAL_test_assert(p==q);
        }
    }
    for(int i = 0; i < 5; i++){
        int n = my_rnd.get_int(0,d);
        int m = my_rnd.get_int(0,d);
        Polynomial_d p,q;
        p = generate_sparse_random_polynomial<Polynomial_d>();
        q = generate_sparse_random_polynomial<Polynomial_d>();
        Polynomial_d pq_1, pq_2;
        pq_1= p*q;
        p = swap(p,n,m);
        q = swap(q,n,m);
        pq_2 = swap(p*q,n,m);
        CGAL_test_assert(pq_1==pq_2); 
    }
    std::cerr << " ok "<< std::endl; 
    
}


template <class Polynomial_traits_d>
void test_move(){
    std::cerr << "start test_move "; std::cerr.flush();
    CGAL_SNAP_POLYNOMIAL_TRAITS_D(Polynomial_traits_d);
    typename Polynomial_traits_d::Move move;
    typename Polynomial_traits_d::Swap swap;
    
    //std::cout << "start_test ----------- "<< d << std::endl; 
    for(int i = 0; i < 5; i++){
        int n = my_rnd.get_int(0,PT::d-1);
        Polynomial_d p = generate_sparse_random_polynomial<Polynomial_d>();
        if(n <= PT::d-2){           
            CGAL_test_assert(move(p,n,n+1) == swap(p,n,n+1));
        }
        if(n <= PT::d-3){
            CGAL_test_assert(move(p,n,n+2) == swap(swap(p,n,n+1),n+1,n+2));
        }
        if(n >= 1){
            CGAL_test_assert(move(p,n-1,n) == swap(p,n-1,n));
        }
        if(n >= 2){
            CGAL_test_assert(move(p,n-2,n) == swap(swap(p,n-1,n),n-1,n-2));
        }
    }
    
    std::cerr << " ok "<< std::endl; 
    
}

template <class Polynomial_traits_d>
void test_degree(){
    std::cerr << "start test_degree "; std::cerr.flush();
    CGAL_SNAP_POLYNOMIAL_TRAITS_D(Polynomial_traits_d);

    typename PT::Construct_polynomial construct;
    typename PT::Degree degree;

    CGAL::Test_functor_arity< typename PT::Degree >()(1);
    
    Polynomial_d p; 
    p= construct(Coeff(0));
    CGAL_test_assert(degree(p)==0);
    p= construct(Coeff(1));
    CGAL_test_assert(degree(p)==0);
    p= construct(Coeff(1),Coeff(2));
    CGAL_test_assert(degree(p)==1);

    
    p= construct(Coeff(0));
    CGAL_test_assert(degree(p,(PT::d-1))==0);
    p= construct(Coeff(1));
    CGAL_test_assert(degree(p,(PT::d-1))==0);
    p= construct(Coeff(1),Coeff(2));
    CGAL_test_assert(degree(p,(PT::d-1))==1);
    std::cerr << " ok "<< std::endl; 
}


//       Total_degree;
template <class Polynomial_traits_d>
void test_total_degree(){

    std::cerr << "start test_total_degree "; std::cerr.flush();
    CGAL_SNAP_POLYNOMIAL_TRAITS_D(Polynomial_traits_d);
    typename PT::Total_degree total_degree;
    for(int i = 0; i < 5; i++){
        Polynomial_d p =generate_sparse_random_polynomial<Polynomial_d>();
        Polynomial_d q =generate_sparse_random_polynomial<Polynomial_d>();
        int tdp = total_degree(p);
        int tdq = total_degree(q);
        CGAL_test_assert(total_degree(p*q) == tdp+tdq);    
    }
    std::cerr << " ok "<< std::endl; 
}
// //       Leading_coefficient;
template <class Polynomial_traits_d>
void test_leading_coefficient(){
    
    std::cerr << "start test_leading_coefficient "; std::cerr.flush();
    CGAL_SNAP_POLYNOMIAL_TRAITS_D(Polynomial_traits_d);
    typename PT::Leading_coefficient lcoeff;
    for(int i = 0; i < 5; i++){
        Polynomial_d p =generate_sparse_random_polynomial<Polynomial_d>();
        Polynomial_d q =generate_sparse_random_polynomial<Polynomial_d>();
        Coeff lcoeffp = lcoeff(p);
        Coeff lcoeffq = lcoeff(q); 
        CGAL_test_assert(lcoeff(p*q) == lcoeffp*lcoeffq);    
    }
    std::cerr << " ok "<< std::endl; 
}

template< class Polynomial_traits_d >
void test_innermost_leading_coefficient() {
    std::cerr << "start test_innermost_leading_coefficient "; std::cerr.flush();
    CGAL_SNAP_POLYNOMIAL_TRAITS_D(Polynomial_traits_d);
    
    typename PT::Innermost_leading_coefficient ilcoeff;
    Polynomial_d p( Coeff(1), Coeff(2), Coeff(3) );
    
    CGAL_test_assert( ilcoeff(p) == ICoeff(3) );
    
    std::cerr << " ok" << std::endl;
}

// //       Univariate_content;
template <class Polynomial_traits_d>
void test_univariate_content(){
    
    std::cerr << "start test_univariate_content "; std::cerr.flush();
    CGAL_SNAP_POLYNOMIAL_TRAITS_D(Polynomial_traits_d);
    typename PT::Univariate_content univariate_content;
    for(int i = 0; i < 5; i++){
        Polynomial_d p =generate_sparse_random_polynomial<Polynomial_d>();
        Polynomial_d q =generate_sparse_random_polynomial<Polynomial_d>();
        Coeff ucontentp = univariate_content(p);
        Coeff ucontentq = univariate_content(q);
        CGAL_test_assert(univariate_content(p*q) == ucontentp*ucontentq);    
    }
    for(int i = 0; i < 5; i++){
        Polynomial_d p =generate_sparse_random_polynomial<Polynomial_d>();
        Polynomial_d q =generate_sparse_random_polynomial<Polynomial_d>();
        Coeff ucontentp = univariate_content(p,0);
        Coeff ucontentq = univariate_content(q,0);
        CGAL_test_assert(univariate_content(p*q,0) == ucontentp*ucontentq);    
    }
    std::cerr << " ok "<< std::endl; 
}
// //       Multivariate_content;
template <class Polynomial_traits_d>
void test_multivariate_content(){
    std::cerr << "start test_multivariate_conten ";
    std::cerr.flush();
    CGAL_SNAP_POLYNOMIAL_TRAITS_D(Polynomial_traits_d);
    typename PT::Multivariate_content mcontent;
    
    CGAL_test_assert(mcontent(Polynomial_d( 0))==ICoeff( 0));
    CGAL_test_assert(mcontent(Polynomial_d( 1))==ICoeff( 1));
    CGAL_test_assert(mcontent(Polynomial_d( 2))==ICoeff( 2));
    CGAL_test_assert(mcontent(Polynomial_d(-2))==ICoeff( 2));
    
    for(int i = 0; i < 5; i++){
        Polynomial_d p =generate_sparse_random_polynomial<Polynomial_d>();
        Polynomial_d q =generate_sparse_random_polynomial<Polynomial_d>();
        
        ICoeff content_p = mcontent(p);
        ICoeff content_q = mcontent(q);
           
        CGAL_test_assert(mcontent(p*q) == content_p*content_q); 
        CGAL_test_assert(mcontent(p*Polynomial_d(2)) == content_p*ICoeff(2));     
        p = CGAL::integral_division(p,content_p);
        CGAL_test_assert(mcontent(p) == ICoeff( 1));
        
    }
    
    std::cerr << " ok "<< std::endl; 
}

// //       Shift;
template <class Polynomial_traits_d>
void test_shift(){
    
    std::cerr << "start test_shift "; std::cerr.flush();
    CGAL_SNAP_POLYNOMIAL_TRAITS_D(Polynomial_traits_d);
    typename PT::Shift shift; 
    typename PT::Construct_polynomial construct;
    for(int i = 0; i < 5; i++){ 
        Polynomial_d p = generate_sparse_random_polynomial<Polynomial_d>();
        Polynomial_d q = p*my_ipower(construct(Coeff(0),Coeff(1)),5);
        CGAL_test_assert(shift(p,5) == q);   
    }
    std::cerr << " ok "<< std::endl; 
}
// //       Negate;
template <class Polynomial_traits_d>
void test_negate(){
    std::cerr << "start test_negate "; std::cerr.flush();
    CGAL_SNAP_POLYNOMIAL_TRAITS_D(Polynomial_traits_d);
    typename PT::Construct_polynomial construct;
    typename PT::Negate negate;
    typename PT::Swap swap;
    
    CGAL_test_assert(negate(Polynomial_d(0))==Polynomial_d(0));
    CGAL_test_assert(negate(Polynomial_d(2))==Polynomial_d(2));

    Polynomial_d p = construct(Coeff(1),Coeff(2),Coeff(3));
    CGAL_test_assert(negate(p)==construct(Coeff(1),Coeff(-2),Coeff(3)));;
    
    for( int i = 0 ; i < 5 ; i++){
        Polynomial_d p = generate_sparse_random_polynomial<Polynomial_d>();
        Polynomial_d q = p;
        int n = my_rnd.get_int(0,PT::d-1);
        int m = my_rnd.get_int(0,PT::d-1);
        CGAL_test_assert(negate(swap(p,n,m),n) ==  swap(negate(p,m),n,m));
    }
    std::cerr << " ok "<< std::endl;
}
//      Invert;
template <class Polynomial_traits_d>
void test_invert(){
    std::cerr << "start test_invert "; std::cerr.flush();
    CGAL_SNAP_POLYNOMIAL_TRAITS_D(Polynomial_traits_d);
    typename PT::Invert invert; 
    typename PT::Swap swap; 
    for( int i = 0 ; i < 5 ; i++){
        Polynomial_d p = generate_sparse_random_polynomial<Polynomial_d>();       
        std::vector<Coeff> coeffs (p.begin(),p.end());
        p = invert(p);
        std::vector<Coeff> rcoeffs (p.begin(),p.end());
        CGAL_test_assert(coeffs.size() >= rcoeffs.size());
        for (unsigned int i = 0; i < rcoeffs.size(); i++){
            CGAL_test_assert(rcoeffs[i] == coeffs[coeffs.size()-i-1]);
        }   
        int n = my_rnd.get_int(0,PT::d-1);
        
        CGAL_test_assert( invert(p,n) == swap(invert(swap(p,n,PT::d-1)),n,PT::d-1));
    }
    std::cerr << " ok "<< std::endl;
}
// //       Translate;
template <class Polynomial_traits_d>
void test_translate(){
    std::cerr << "start test_translate "; std::cerr.flush();
    CGAL_SNAP_POLYNOMIAL_TRAITS_D(Polynomial_traits_d);
    typename PT::Translate translate;
    typename PT::Evaluate evaluate;
    typename PT::Move move;
    for( int i = 0 ; i < 5 ; i++){
        Polynomial_d p = generate_sparse_random_polynomial<Polynomial_d>();       
        CGAL_test_assert(evaluate(translate(p,ICoeff(5)),ICoeff(3)) 
                == evaluate(p,ICoeff(8)));
        CGAL_test_assert(evaluate(translate(p,ICoeff(5),0),ICoeff(3),0) 
                == evaluate(move(p,0,PT::d-1),ICoeff(8)));
    }
    std::cerr << " ok "<< std::endl;
}

// //       Translate_homogeneous;
template <class Polynomial_traits_d>
void test_translate_homongenous(){
    std::cerr << "start test_translate_homongenous "; std::cerr.flush();
    CGAL_SNAP_POLYNOMIAL_TRAITS_D(Polynomial_traits_d);
    typename PT::Translate_homogeneous transh;
    typename PT::Canonicalize canonicalize;
    typename PT::Evaluate_homogeneous evh;
    //typename PT::Move move;
    for( int i = 0 ; i < 5 ; i++){
        Polynomial_d p,q1,q2;
        p = generate_sparse_random_polynomial<Polynomial_d>(); 
        q1 = transh(transh(p,ICoeff(5),ICoeff(3)),ICoeff(3),ICoeff(2)) ;
        q2 = transh(p,ICoeff(19),ICoeff(6)) ;
        CGAL_test_assert( canonicalize(q1) != canonicalize(p)) ;
        CGAL_test_assert( canonicalize(q2) != canonicalize(p) ) ;
        CGAL_test_assert( canonicalize(q1) == canonicalize(q2));
        
        CGAL_test_assert(evh(p,ICoeff(19),ICoeff(6)) == evh(q2,ICoeff(0),ICoeff(1)));
        
        q1 = transh(transh(p,ICoeff(5),ICoeff(3),0),ICoeff(3),ICoeff(2),0) ;  
        q2 = transh(p,ICoeff(19),ICoeff(6),0) ;
        
        CGAL_test_assert( canonicalize(q1) != canonicalize(p)) ;
        CGAL_test_assert( canonicalize(q2) != canonicalize(p) ) ;
        CGAL_test_assert( canonicalize(q1) == canonicalize(q2)); 
    }
    std::cerr << " ok "<< std::endl;
}

template< class Polynomial_traits_d>
void test_scale() {
    (std::cerr << "start test_scale ").flush();
    
    CGAL_SNAP_POLYNOMIAL_TRAITS_D(Polynomial_traits_d);
    
    typename PT::Scale scale;
    Polynomial_d p( Coeff(1), Coeff(2), Coeff(3) );
    
    CGAL_test_assert( scale( p, ICoeff(2) ) == Polynomial_d( Coeff(1), Coeff(4), Coeff(12) ) );
    
    std::cerr << " ok" << std::endl;
}

// //       Scale_homogeneous;
template <class Polynomial_traits_d>
void test_scale_homogeneous(){
    std::cerr << "start test_scale_homogeneous "; std::cerr.flush();
    CGAL_SNAP_POLYNOMIAL_TRAITS_D(Polynomial_traits_d);
    typename PT::Scale_homogeneous scaleh;
    typename PT::Canonicalize canonicalize;
    //typename PT::Move move;
    for( int i = 0 ; i < 5 ; i++){
        Polynomial_d p,q1,q2;
        p = generate_sparse_random_polynomial<Polynomial_d>(); 
        q1 = scaleh(scaleh(p,ICoeff(5),ICoeff(3)),ICoeff(3),ICoeff(2)) ;
        q2 = scaleh(p,ICoeff(15),ICoeff(6)) ;
        CGAL_test_assert( canonicalize(q1) != canonicalize(p)) ;
        CGAL_test_assert( canonicalize(q2) != canonicalize(p) ) ;
        CGAL_test_assert( canonicalize(q1) == canonicalize(q2));
        
        q1 = scaleh(scaleh(p,ICoeff(5),ICoeff(3),1),ICoeff(3),ICoeff(2),0) ;  
        q2 = scaleh(p,ICoeff(15),ICoeff(6),0) ;
        
        CGAL_test_assert( canonicalize(q1) != canonicalize(p)) ;
        CGAL_test_assert( canonicalize(q2) != canonicalize(p) ) ;
        CGAL_test_assert( canonicalize(q1) == canonicalize(q2)); 
    }
    std::cerr << " ok "<< std::endl;
}

// //       Differentiate;
template <class Polynomial_traits_d>
void test_differentiate(){
    std::cerr << "start test_differentiate "; std::cerr.flush();
    CGAL_SNAP_POLYNOMIAL_TRAITS_D(Polynomial_traits_d);
    
    typename PT::Differentiate diff;
    typename PT::Swap swap;
    
    CGAL_test_assert(diff(Polynomial_d(0)) == Polynomial_d(0));
    CGAL_test_assert(diff(Polynomial_d(1)) == Polynomial_d(0));
    CGAL_test_assert(diff(Polynomial_d(Coeff(1), Coeff(2))) == Polynomial_d(2));
    
    for( int i = 0 ; i < 5 ; i++){
        int n = my_rnd.get_int(0,PT::d-1);
        Polynomial_d p,pd;
        p = generate_sparse_random_polynomial<Polynomial_d>(); 
        pd = diff(p,n);
        CGAL_test_assert(pd == swap(diff(swap(p,n,PT::d-1)),n,PT::d-1));
    }
    std::cerr << " ok "<< std::endl;
}

// //       Make_square_free;
template <class Polynomial_traits_d>
void test_make_square_free(){
    std::cerr << "start test_make_square_free "; std::cerr.flush();
    CGAL_SNAP_POLYNOMIAL_TRAITS_D(Polynomial_traits_d);
    typename PT::Make_square_free make_square_free;
    typename PT::Leading_coefficient lcoeff;
    typename PT::Univariate_content_up_to_constant_factor ucontent_utcf;
    typename PT::Integral_division_up_to_constant_factor idiv_utcf;
    
    CGAL_test_assert( Polynomial_d(0) == make_square_free(Polynomial_d(0)));
    CGAL_test_assert( Polynomial_d(1) == make_square_free(Polynomial_d(1)));
    CGAL_test_assert( Polynomial_d(1) == make_square_free(Polynomial_d(2)));
    
    //typename PT::Canonicalize canonicalize; 
    for( int i = 0 ; i < 5 ; i++){
        Polynomial_d p;
        p = generate_sparse_random_polynomial<Polynomial_d>(3); 
        
        p = idiv_utcf(p, ucontent_utcf(p));
        p = make_square_free(p);
        Coeff f_intern = lcoeff(p);
        
        f_intern = typename PTC::Make_square_free()(f_intern);
       
        //std::cout <<"f: " << f_intern << std::endl;
        
        CGAL_test_assert( p * f_intern == make_square_free(p*p*f_intern));
        CGAL_test_assert( p * f_intern == make_square_free(p*f_intern*f_intern));
    }
    std::cerr << " ok "<< std::endl;
}


// //       Square_free_factorization;
template <class Polynomial_traits_d>
void test_square_free_factorization(){
    std::cerr << "start test_square_free_factorization "; std::cerr.flush(); 
    CGAL_SNAP_POLYNOMIAL_TRAITS_D(Polynomial_traits_d);
    typedef CGAL::Algebraic_structure_traits<Polynomial_d> AST;
    typename AST::Integral_division idiv;
    typename PT::Square_free_factorization sqff;
    for(int i = 0; i < 10; i++){
        Polynomial_d f1 = generate_sparse_random_polynomial<Polynomial_d>(2);
        Polynomial_d f2 = generate_sparse_random_polynomial<Polynomial_d>(2);    
        Polynomial_d p = f1*f1*f2;
        std::vector<Polynomial_d> factors;
        std::vector<int> mults;
        int n = sqff(p, std::back_inserter(factors), std::back_inserter(mults));
        //std::cout << "n: " << n <<std::endl;
        CGAL_test_assert(n == (int) factors.size());
        CGAL_test_assert(n == (int) mults.size());
        for (int j = 0; j < n; j++){
            for (int k = 0; k < mults[j]; k++){
                p = idiv(p,factors[j]);
            }
        }
        CGAL_test_assert(typename PT::Total_degree()(p) == 0 );
        //if(typename PT::Total_degree()(p) != 0){
        //    std::cout<< p << std::endl; 
        //}
    }

    std::cerr << "ok"<< std::endl; 
    
}
// //       Pseudo_division;
template <class Polynomial_traits_d>
void test_pseudo_division(){
    std::cerr << "start test_pseudo_division "; std::cerr.flush();
    CGAL_SNAP_POLYNOMIAL_TRAITS_D(Polynomial_traits_d);
    typename PT::Pseudo_division pdiv;
    for(int i = 0; i < 10; i++){
        Polynomial_d f = generate_sparse_random_polynomial<Polynomial_d>(3);
        Polynomial_d g = generate_sparse_random_polynomial<Polynomial_d>(2);    
        Coeff D;
        Polynomial_d q,r;
        pdiv(f,g,q,r,D);
        CGAL_test_assert(f*Polynomial_d(D)==g*q+r);
    }
    std::cerr << " ok "<< std::endl;
}

// //       Pseudo_division_remainder;
template <class Polynomial_traits_d>
void test_pseudo_division_remainder(){
    std::cerr << "start test_pseudo_division_remainder "; std::cerr.flush();
    CGAL_SNAP_POLYNOMIAL_TRAITS_D(Polynomial_traits_d);
    typename PT::Pseudo_division pdiv;
    typename PT::Pseudo_division_remainder pdiv_r;
    for(int i = 0; i < 10; i++){
        Polynomial_d f = generate_sparse_random_polynomial<Polynomial_d>(3);
        Polynomial_d g = generate_sparse_random_polynomial<Polynomial_d>(2);    
        Coeff D;
        Polynomial_d q,r;
        pdiv(f,g,q,r,D);
        CGAL_test_assert(r == pdiv_r(f,g));
    }
    std::cerr << " ok "<< std::endl;
}

// //       Pseudo_division_quotient;
template <class Polynomial_traits_d>
void test_pseudo_division_quotient(){
    std::cerr << "start test_pseudo_division_quotient "; std::cerr.flush();
    CGAL_SNAP_POLYNOMIAL_TRAITS_D(Polynomial_traits_d);
    typename PT::Pseudo_division pdiv;
    typename PT::Pseudo_division_quotient pdiv_q;
    for(int i = 0; i < 10; i++){
        Polynomial_d f = generate_sparse_random_polynomial<Polynomial_d>(3);
        Polynomial_d g = generate_sparse_random_polynomial<Polynomial_d>(2);    
        Coeff D;
        Polynomial_d q,r;
        pdiv(f,g,q,r,D);
        CGAL_test_assert(q == pdiv_q(f,g));
    }
    std::cerr << " ok "<< std::endl;
}

// //       Gcd_up_to_constant_factor;
template <class Polynomial_traits_d>
void test_gcd_up_to_constant_factor(){
    std::cerr << "start test_gcd_up_to_constant_factor "; std::cerr.flush();
    CGAL_SNAP_POLYNOMIAL_TRAITS_D(Polynomial_traits_d);
    typename PT::Gcd_up_to_constant_factor gcd_utcf;
    
    CGAL_test_assert(Polynomial_d(0) == gcd_utcf(Polynomial_d(0),Polynomial_d(0)));
    CGAL_test_assert(Polynomial_d(1) == gcd_utcf(Polynomial_d(1),Polynomial_d(0)));
    CGAL_test_assert(Polynomial_d(1) == gcd_utcf(Polynomial_d(0),Polynomial_d(1)));
    CGAL_test_assert(Polynomial_d(1) == gcd_utcf(Polynomial_d(1),Polynomial_d(1)));
    CGAL_test_assert(Polynomial_d(1) == gcd_utcf(Polynomial_d(-1),Polynomial_d(-1)));
    CGAL_test_assert(Polynomial_d(1) == gcd_utcf(Polynomial_d(2),Polynomial_d(2)));
    
    std::cerr << " ok "<< std::endl;
}

// //       Integral_division_up_to_constant_factor;
template <class Polynomial_traits_d>
void test_integral_division_up_to_constant_factor(){
    std::cerr << "start test_integral_division_up_to_constant_factor "; std::cerr.flush();
    CGAL_SNAP_POLYNOMIAL_TRAITS_D(Polynomial_traits_d);
    typename PT::Integral_division_up_to_constant_factor idiv_utcf;
    typename PT::Canonicalize canonicalize;
    CGAL_test_assert(Polynomial_d(0) == idiv_utcf(Polynomial_d(0),Polynomial_d(1)));
    CGAL_test_assert(Polynomial_d(1) == idiv_utcf(Polynomial_d(1),Polynomial_d(1)));
    CGAL_test_assert(Polynomial_d(1) == idiv_utcf(Polynomial_d(2),Polynomial_d(1)));
    
    for(int i = 0; i < 5; i++){
        Polynomial_d p,q;
        p = generate_sparse_random_polynomial<Polynomial_d>(3);
        q = generate_sparse_random_polynomial<Polynomial_d>(2);
        CGAL_test_assert(canonicalize(p) == idiv_utcf(p*q,q));
    }
    std::cerr << " ok "<< std::endl;
}

// //       Content_up_to_constant_factor;
template <class Polynomial_traits_d>
void test_univariate_content_up_to_constant_factor(){
    std::cerr << "start test_univariate_content_up_to_constant_factor "; std::cerr.flush();
    CGAL_SNAP_POLYNOMIAL_TRAITS_D(Polynomial_traits_d);
    CGAL_SNAP_POLYNOMIAL_TRAITS_D(Polynomial_traits_d);
    typename PT::Univariate_content_up_to_constant_factor ucontent_utcf;
    typename PT::Integral_division_up_to_constant_factor idiv_utcf;
    typename PT::Leading_coefficient lcoeff;
    
    typename PT::Canonicalize canonicalize;
    CGAL_test_assert(Coeff(0) == ucontent_utcf(Polynomial_d(0)));
    CGAL_test_assert(Coeff(1) == ucontent_utcf(Polynomial_d(1)));
    CGAL_test_assert(Coeff(1) == ucontent_utcf(Polynomial_d(2)));
    CGAL_test_assert(Coeff(1) == ucontent_utcf(Polynomial_d(-2)));
    
    for(int i = 0; i < 5; i++){
        Polynomial_d p,q;
        p = generate_sparse_random_polynomial<Polynomial_d>(3);
        Coeff content = ucontent_utcf(p);
        p = idiv_utcf(p,Polynomial_d(content));
        CGAL_test_assert(Coeff(1) == ucontent_utcf(p));
        Coeff lc = lcoeff(p);
        p = p*lc*lc;
        CGAL_test_assert(canonicalize(Polynomial_d(lc*lc)) == ucontent_utcf(p));
        
    }
    
    std::cerr << " ok "<< std::endl;
}


// //       Square_free_factorization_up_to_constant_factor;
template <class Polynomial_traits_d>
void test_square_free_factorization_up_to_constant_factor(){
    std::cerr << "start test_square_free_factorization_up_to_constant_factor "; 
    std::cerr.flush(); 
    CGAL_SNAP_POLYNOMIAL_TRAITS_D(Polynomial_traits_d);
    typename PT::Integral_division_up_to_constant_factor idiv_utcf;
    typename PT::Square_free_factorization_up_to_constant_factor sqff_utcf;
    typename PT::Canonicalize canonicalize;
    for(int i = 0; i < 5; i++){
        Polynomial_d f1 = generate_sparse_random_polynomial<Polynomial_d>(2);
        Polynomial_d f2 = generate_sparse_random_polynomial<Polynomial_d>(2);    
        Polynomial_d p = f1*f1*f2;
        std::vector<Polynomial_d> factors;
        std::vector<int> mults;
        int n = sqff_utcf(p, std::back_inserter(factors), std::back_inserter(mults));
        //std::cout << "n: " << n <<std::endl;
        CGAL_test_assert(n == (int) factors.size());
        CGAL_test_assert(n == (int) mults.size());
        for (int j = 0; j < n; j++){
            for (int k = 0; k < mults[j]; k++){
                p = idiv_utcf(p,factors[j]);
            }
        }
        CGAL_test_assert( CGAL::is_one(canonicalize(p)) );
    }
    std::cerr << "ok"<< std::endl; 
}





// //       Evaluate;
template <class Polynomial_traits_d>
void test_evaluate(){
    std::cerr << "start test_evaluate "; std::cerr.flush();
    CGAL_SNAP_POLYNOMIAL_TRAITS_D(Polynomial_traits_d);
    typename PT::Evaluate evaluate;
    typename PT::Move move;
    CGAL_test_assert(evaluate(Polynomial_d(0),ICoeff(0)) == Coeff(0));
    CGAL_test_assert(evaluate(Polynomial_d(1),ICoeff(0)) == Coeff(1));
    CGAL_test_assert(evaluate(Polynomial_d(2),ICoeff(5)) == Coeff(2));

    CGAL_test_assert(evaluate(Polynomial_d(Coeff(3),Coeff(2)),ICoeff(0))==Coeff(3));
    CGAL_test_assert(evaluate(Polynomial_d(Coeff(3),Coeff(2)),ICoeff(1))==Coeff(5));
    CGAL_test_assert(evaluate(Polynomial_d(Coeff(3),Coeff(2)),ICoeff(2))==Coeff(7));
    
    for(int i = 0; i < 5; i++){
        int n = my_rnd.get_int(0,PT::d-1);
        Polynomial_d p,q;
        p = generate_sparse_random_polynomial<Polynomial_d>();
        CGAL_test_assert(
                evaluate(p,ICoeff(3),n) == evaluate(move(p,n,PT::d-1),ICoeff(3)));
    }
    
    Polynomial_d p( Coeff(1), Coeff(2), Coeff(3) );
    std::vector< ICoeff > ev;
    for( int i = 0; i < PT::d-1; ++i ) {
        ev.push_back( ICoeff(0) );
    }
    ev.push_back( ICoeff(1) );
    CGAL_test_assert( evaluate( p, ev.begin(), ev.end() ) == ICoeff(1+2+3) );
    ev.pop_back();
    ev.push_back( ICoeff(2) );  
    CGAL_test_assert( evaluate( p, ev.begin(), ev.end() ) == ICoeff(1+4+12) );
    
    
    std::cerr << " ok "<< std::endl;
}

// //       Evaluate_homogeneous;
template <class Polynomial_traits_d>
void test_evaluate_homogeneous(){
    std::cerr << "start test_evaluate_homogeneous "; std::cerr.flush();
    CGAL_SNAP_POLYNOMIAL_TRAITS_D(Polynomial_traits_d);
    typename PT::Evaluate_homogeneous evh;
  
    CGAL_test_assert(evh(Polynomial_d(0),ICoeff(0),ICoeff(1)) == Coeff(0));
    CGAL_test_assert(evh(Polynomial_d(1),ICoeff(0),ICoeff(2)) == Coeff(1));
    CGAL_test_assert(evh(Polynomial_d(2),ICoeff(5),ICoeff(3)) == Coeff(2));

    CGAL_test_assert(evh(Polynomial_d(Coeff(3),Coeff(2)),ICoeff(0),ICoeff(1))==Coeff(3));
    CGAL_test_assert(evh(Polynomial_d(Coeff(3),Coeff(2)),ICoeff(1),ICoeff(1))==Coeff(5));
    CGAL_test_assert(evh(Polynomial_d(Coeff(3),Coeff(2)),ICoeff(2),ICoeff(3))==Coeff(9+4));
    
    // hdegree parameter no longer available
    /*CGAL_test_assert(evh(Polynomial_d(Coeff(5),Coeff(7)),ICoeff(2),ICoeff(3),2)
            ==Coeff(5*3*3+7*3*2));*/
    
    Polynomial_d p( Coeff(1), Coeff(2), Coeff(3) );
    std::vector< ICoeff > ev;
    for( int i = 0; i < PT::d-1; ++i ) {
        ev.push_back( ICoeff(0) );
    }
    ev.push_back( ICoeff(1) );
    ev.push_back( ICoeff(1) );
    CGAL_test_assert( evh( p, ev.begin(), ev.end() ) == ICoeff(1+2+3) );
    ev.pop_back();
    ev.push_back( ICoeff(2) );  
    CGAL_test_assert( evh( p, ev.begin(), ev.end() ) == ICoeff(4+4+3) );
    ev.pop_back();
    ev.pop_back();
    ev.push_back( ICoeff(2) );  
    ev.push_back( ICoeff(2) );  
    CGAL_test_assert( evh( p, ev.begin(), ev.end() ) == ICoeff(4+8+12) );
        
    std::cerr << " ok "<< std::endl;
}

template< class Polynomial_traits_d >
void test_is_zero_at() {
    std::cerr << "start test_is_zero_at "; 
    std::cerr.flush();
    
    CGAL_SNAP_POLYNOMIAL_TRAITS_D( Polynomial_traits_d );
    
    typename PT::Is_zero_at is_zero_at;
    
    Polynomial_d p( Coeff(-1), Coeff(0), Coeff(1) );
    
    std::vector< ICoeff > cv;
    for( int i = 0; i < PT::d-1; ++i )
        cv.push_back( ICoeff(0) );
        
    cv.push_back( ICoeff(0) );
    CGAL_test_assert( !is_zero_at( p, cv.begin(), cv.end() ) );
    
    cv.pop_back();
    cv.push_back( ICoeff(1) );
    CGAL_test_assert( is_zero_at( p, cv.begin(), cv.end() ) );
    
    cv.pop_back();
    cv.push_back( ICoeff(-1) );
    CGAL_test_assert( is_zero_at( p, cv.begin(), cv.end() ) );
    
    std::cerr << " ok" << std::endl;
}

template< class Polynomial_traits_d >
void test_is_zero_at_homogeneous() {
    std::cerr << "start test_is_zero_at_homogeneous "; 
    std::cerr.flush();
    
    CGAL_SNAP_POLYNOMIAL_TRAITS_D( Polynomial_traits_d );
    
    typename PT::Is_zero_at_homogeneous is_zero_at_homogeneous;
    
    Polynomial_d p( Coeff(-1), Coeff(0), Coeff(1) );
    
    std::vector< ICoeff > cv;
    for( int i = 0; i < PT::d-1; ++i )
        cv.push_back( ICoeff(0) );
        
    for( int v = 1; v < 5; ++v ) {
        cv.push_back( ICoeff(0) );
        cv.push_back( ICoeff(v) );
        CGAL_test_assert( !is_zero_at_homogeneous( p, cv.begin(), cv.end() ) );
        
        cv.pop_back();
        cv.pop_back();
        cv.push_back( ICoeff(v) );
        cv.push_back( ICoeff(v) );
        CGAL_test_assert( is_zero_at_homogeneous( p, cv.begin(), cv.end() ) );
        
        cv.pop_back();
        cv.pop_back();
        cv.push_back( ICoeff(-v) );
        cv.push_back( ICoeff(v) );
        CGAL_test_assert( is_zero_at_homogeneous( p, cv.begin(), cv.end() ) );
        
        cv.pop_back();
        cv.pop_back();
    }
    
    std::cerr << " ok" << std::endl;
}

template< class Polynomial_traits_d >
void test_sign_at() {
    std::cerr << "start test_sign_at "; 
    std::cerr.flush();
    
    CGAL_SNAP_POLYNOMIAL_TRAITS_D( Polynomial_traits_d );
    
    typename PT::Sign_at sign_at;
    
    Polynomial_d p( Coeff(-1), Coeff(0), Coeff(1) );
    
    std::vector< ICoeff > cv;
    for( int i = 0; i < PT::d-1; ++i )
        cv.push_back( ICoeff(0) );
        
    cv.push_back( ICoeff(0) );
    CGAL_test_assert( sign_at( p, cv.begin(), cv.end() ) == CGAL::NEGATIVE );
    
    cv.pop_back();
    cv.push_back( ICoeff(1) );
    CGAL_test_assert( sign_at( p, cv.begin(), cv.end() ) == CGAL::ZERO );
    
    cv.pop_back();
    cv.push_back( ICoeff(-1) );
    CGAL_test_assert( sign_at( p, cv.begin(), cv.end() ) == CGAL::ZERO );
    
    cv.pop_back();
    cv.push_back( ICoeff(2) );
    CGAL_test_assert( sign_at( p, cv.begin(), cv.end() ) == CGAL::POSITIVE );

    std::cerr << " ok" << std::endl;
}

template< class Polynomial_traits_d >
void test_sign_at_homogeneous() {
    std::cerr << "start test_sign_at_homogeneous "; 
    std::cerr.flush();
    
    CGAL_SNAP_POLYNOMIAL_TRAITS_D( Polynomial_traits_d );
    
    typename PT::Sign_at_homogeneous sign_at_homogeneous;
    
    Polynomial_d p( Coeff(-1), Coeff(0), Coeff(1) );
    
    std::vector< ICoeff > cv;
    for( int i = 0; i < PT::d-1; ++i )
        cv.push_back( ICoeff(0) );
        
    for( int v = 1; v < 5; ++v ) {
        cv.push_back( ICoeff(0) );
        cv.push_back( ICoeff(v) );
        CGAL_test_assert( sign_at_homogeneous( p, cv.begin(), cv.end() ) == CGAL::NEGATIVE );
        
        cv.pop_back();
        cv.pop_back();
        cv.push_back( ICoeff(v) );
        cv.push_back( ICoeff(v) );
        CGAL_test_assert( sign_at_homogeneous( p, cv.begin(), cv.end() ) == CGAL::ZERO );
        
        cv.pop_back();
        cv.pop_back();
        cv.push_back( ICoeff(-v) );
        cv.push_back( ICoeff(v) );
        CGAL_test_assert( sign_at_homogeneous( p, cv.begin(), cv.end() ) == CGAL::ZERO );

        cv.pop_back();
        cv.pop_back();
        cv.push_back( ICoeff(v+1) );
        cv.push_back( ICoeff(v) );
        CGAL_test_assert( sign_at_homogeneous( p, cv.begin(), cv.end() ) == CGAL::POSITIVE );
        
        cv.pop_back();
        cv.pop_back();
    }
    
    std::cerr << " ok" << std::endl;
}

template< class Polynomial_traits_d>
void test_compare() {
    (std::cerr << "start test compare ").flush();
    
    CGAL_SNAP_POLYNOMIAL_TRAITS_D(Polynomial_traits_d);
    
    typename PT::Compare compare;
    
    Polynomial_d p0( Coeff(0) );
    Polynomial_d pp2( Coeff(2) );
    Polynomial_d pm2( Coeff(-2) );
    Polynomial_d pp1p2( Coeff(1), Coeff(2) );
    Polynomial_d pm1m2( Coeff(-1), Coeff(-2) );
    
    CGAL_test_assert( compare( p0, p0 ) == CGAL::EQUAL );
    CGAL_test_assert( compare( pp2, pp2 ) == CGAL::EQUAL );
    CGAL_test_assert( compare( pm2, pm2 ) == CGAL::EQUAL );
    CGAL_test_assert( compare( pp1p2, pp1p2 ) == CGAL::EQUAL );
    CGAL_test_assert( compare( pm1m2, pm1m2 ) == CGAL::EQUAL );

    CGAL_test_assert( compare( p0, pp2 ) == CGAL::SMALLER );
    CGAL_test_assert( compare( p0, pm2 ) == CGAL::LARGER );
    CGAL_test_assert( compare( pp2, pm2 ) == CGAL::LARGER );
    CGAL_test_assert( compare( pm1m2, pp1p2 ) == CGAL::SMALLER );
    
    CGAL_test_assert( compare( pp1p2, pp2 ) == CGAL::LARGER );
    CGAL_test_assert( compare( pm1m2, pp2 ) == CGAL::SMALLER );
    CGAL_test_assert( compare( pp1p2, pm2 ) == CGAL::LARGER );
    CGAL_test_assert( compare( pm1m2, pm2 ) == CGAL::SMALLER );
    
    std::cerr << " ok" << std::endl;
}


// //       Resultant;
template <class Polynomial_traits_d>
void test_resultant(){
    std::cerr << "start test_resultant "; std::cerr.flush();
    CGAL_SNAP_POLYNOMIAL_TRAITS_D(Polynomial_traits_d);
    typename  PT::Resultant resultant;
    typename  PT::Move move;
    {
        Polynomial_d A(0);
        Polynomial_d B(0);
        CGAL_test_assert(resultant(A,B)==Coeff(0));
    }{
        Polynomial_d A(4);
        Polynomial_d B(8);
        CGAL_test_assert(resultant(A,B)==Coeff(1));
    }{
        Polynomial_d f(Coeff(2), Coeff(7), Coeff(1), Coeff(8), Coeff(1), Coeff(8));
        Polynomial_d g(Coeff(3), Coeff(1), Coeff(4), Coeff(1), Coeff(5), Coeff(9));       
        CGAL_test_assert( resultant(f, g) == Coeff(230664271L)); // Maple
            
        Polynomial_d h(Coeff(3), Coeff(4), Coeff(7), Coeff(7));
        Polynomial_d fh(f*h);
        Polynomial_d gh(g*h);
        CGAL_test_assert( resultant(fh, gh) == Coeff(0) );
    } 
    for(int i = 0 ; i < 5 ; i++){
        int n = my_rnd.get_int(0,PT::d-1);
        Polynomial_d p,q;
        p = generate_sparse_random_polynomial<Polynomial_d>(3);
        q = generate_sparse_random_polynomial<Polynomial_d>(3);
        CGAL_test_assert(resultant(p,q,n) == resultant(move(p,n),move(q,n)));
    }
    
    std::cerr << " ok "<< std::endl;    
}

// //       Canonicalize;
template <class Polynomial_traits_d>
void test_canonicalize(){
    std::cerr << "start test_canonicalize "; std::cerr.flush();
    CGAL_SNAP_POLYNOMIAL_TRAITS_D(Polynomial_traits_d);
    typename PT::Canonicalize canonicalize;

    CGAL_test_assert(Polynomial_d(0) == canonicalize(Polynomial_d(0)));
    CGAL_test_assert(Polynomial_d(1) == canonicalize(Polynomial_d(1)));
    CGAL_test_assert(Polynomial_d(1) == canonicalize(Polynomial_d(2)));
    CGAL_test_assert(Polynomial_d(1) == canonicalize(Polynomial_d(-2)));

     for( int i = 0 ; i < 5 ; i++){
        Polynomial_d p = generate_sparse_random_polynomial<Polynomial_d>(3); 
        Polynomial_d q = generate_sparse_random_polynomial<Polynomial_d>(3); 
        CGAL_test_assert(canonicalize(p)*canonicalize(q) == canonicalize(p*q) );
    }
    std::cerr << " ok "<< std::endl;
}


template <class Polynomial_traits_d>
void test_polynomial_traits_d(){
    std::cout << "\n start test for dimension: "
              << Polynomial_traits_d::d << std::endl; 

//       Construct_polynomial;
    test_construct_polynomial<Polynomial_traits_d>();
//       Get_coefficient;
    test_get_coefficient<Polynomial_traits_d>();
//       Get_innermost_coefficient;
    test_get_innermost_coefficient<Polynomial_traits_d>();
//       Get_monom_representation;
    test_get_monom_representation<Polynomial_traits_d>();
//       Swap;
    test_swap<Polynomial_traits_d>();
//       Move;
    test_move<Polynomial_traits_d>();
//       Degree;
    test_degree<Polynomial_traits_d>();
//       Total_degree;
    test_total_degree<Polynomial_traits_d>();
//       Leading_coefficient;
    test_leading_coefficient<Polynomial_traits_d>();
//       Innermost_leading_coefficient;
    test_innermost_leading_coefficient<Polynomial_traits_d>();
//       Is_zero_at;
    test_is_zero_at<Polynomial_traits_d>();
//       Is_zero_at_homogeneous;
    test_is_zero_at_homogeneous<Polynomial_traits_d>();
//       Sign_at;
    test_sign_at<Polynomial_traits_d>();
//       Sign_at_homogeneous;
    test_sign_at_homogeneous<Polynomial_traits_d>();
//       Compare;
    test_compare<Polynomial_traits_d>();
//       Univariate_content;
    test_univariate_content<Polynomial_traits_d>();
//       Multivariate_content;
    test_multivariate_content<Polynomial_traits_d>();
//       Shift;
    test_shift<Polynomial_traits_d>();
//       Negate;
    test_negate<Polynomial_traits_d>();
//       Invert;
    test_invert<Polynomial_traits_d>();
//       Translate;
    test_translate<Polynomial_traits_d>();
//       Translate_homogeneous;
    test_translate_homongenous<Polynomial_traits_d>();
//       Scale;
    test_scale<Polynomial_traits_d>();
//       Scale_homogeneous;
    test_scale_homogeneous<Polynomial_traits_d>();
//       Differentiate;
    test_differentiate<Polynomial_traits_d>();
//       Make_square_free;
    test_make_square_free<Polynomial_traits_d>();
//       Square_free_factorization;
    test_square_free_factorization<Polynomial_traits_d>();
//       Pseudo_division;
    test_pseudo_division<Polynomial_traits_d>();
//       Pseudo_division_remainder;
    test_pseudo_division_remainder<Polynomial_traits_d>();
//       Pseudo_division_quotient;
    test_pseudo_division_quotient<Polynomial_traits_d>();
//       Gcd_up_to_constant_factor;
    test_gcd_up_to_constant_factor<Polynomial_traits_d>();
//       Integral_division_up_to_constant_factor;
    test_integral_division_up_to_constant_factor<Polynomial_traits_d>();
//       Content_up_to_constant_factor;
    test_univariate_content_up_to_constant_factor<Polynomial_traits_d>();
//       Square_free_factorization_up_to_constant_factor;
    test_square_free_factorization_up_to_constant_factor<Polynomial_traits_d>();
//       Evaluate;
    test_evaluate<Polynomial_traits_d>();
//       Evaluate_homogeneous;
    test_evaluate_homogeneous<Polynomial_traits_d>();
//       Resultant;
    test_resultant<Polynomial_traits_d>();
//       Canonicalize;
    test_canonicalize<Polynomial_traits_d>();
    
//   private:
//       Innermost_leading_coefficient;
}


int main(){
    CGAL::set_pretty_mode(std::cout);
    
    /*CGAL::Polynomial< CGAL::Polynomial< leda_integer > > poly2( 
        CGAL::Polynomial< leda_integer >(1,2,3,4),
        CGAL::Polynomial< leda_integer >(5,6,7,8),
        CGAL::Polynomial< leda_integer >(-9,10,11,12) );
    
    CGAL::Polynomial_traits_d< CGAL::Polynomial< CGAL::Polynomial< leda_integer > > >::Evaluate_homogeneous eval_hom;*/
    
    /*CGAL::Polynomial< leda_integer > poly1( -1,0,1);
    CGAL::Polynomial_traits_d< CGAL::Polynomial< leda_integer > >::Evaluate_homogeneous eval_hom;*/
    
    /*std::vector< leda_integer > eval_vec;
    eval_vec.push_back( 1 );
    eval_vec.push_back( 1 );
    eval_vec.push_back( 2 );
    
    std::cerr << eval_hom( poly2, eval_vec.begin(), eval_vec.end()  ) << std::endl;*/
    /*CGAL::Exponent_vector ev;
    ev.push_back( 0 );
    ev.push_back( 3 );*/
    //std::cerr << gic( poly2, ev ) << std::endl;
    
    
    {
        typedef CGAL::Polynomial<leda_integer> Polynomial_1;
       
        const int dimension                    = 1;
        typedef Polynomial_1                   Polynomial_d;
        typedef leda_integer                   Coefficient;
        typedef leda_integer                   Innermost_coefficient;
        
        typedef CGAL::Polynomial_traits_d<Polynomial_d> PT;
        
        BOOST_STATIC_ASSERT(
                (boost::is_same< PT::Polynomial_d,Polynomial_d>::value));
        BOOST_STATIC_ASSERT(
                (boost::is_same< PT::Coefficient, Coefficient>::value));
        BOOST_STATIC_ASSERT((boost::is_same< PT::Innermost_coefficient, 
                        Innermost_coefficient>::value));
        BOOST_STATIC_ASSERT((PT::d == dimension));
        test_polynomial_traits_d<PT>();
    }
    {
        typedef CGAL::Polynomial<leda_integer> Polynomial_1;
        typedef CGAL::Polynomial<Polynomial_1> Polynomial_2;

        const int dimension                  = 2;
        typedef Polynomial_2                   Polynomial_d;
        typedef Polynomial_1                   Coefficient;
        typedef leda_integer                   Innermost_coefficient;
        
        typedef CGAL::Polynomial_traits_d<Polynomial_d> PT;
        
        BOOST_STATIC_ASSERT(
                (boost::is_same< PT::Polynomial_d,Polynomial_d>::value));
        BOOST_STATIC_ASSERT(
                (boost::is_same< PT::Coefficient, Coefficient>::value));
        BOOST_STATIC_ASSERT((boost::is_same< PT::Innermost_coefficient, 
                        Innermost_coefficient>::value));
        BOOST_STATIC_ASSERT((PT::d == dimension));
        test_polynomial_traits_d<PT>();
    }{
        typedef CGAL::Polynomial<leda_integer> Polynomial_1;
        typedef CGAL::Polynomial<Polynomial_1> Polynomial_2;
        typedef CGAL::Polynomial<Polynomial_2> Polynomial_3;

        const int dimension                  = 3;
        typedef Polynomial_3                   Polynomial_d;
        typedef Polynomial_2                   Coefficient;
        typedef leda_integer                   Innermost_coefficient;
        
        typedef CGAL::Polynomial_traits_d<Polynomial_d> PT;
        
        BOOST_STATIC_ASSERT(
                (boost::is_same< PT::Polynomial_d, Polynomial_d>::value));
        BOOST_STATIC_ASSERT(
                (boost::is_same< PT::Coefficient, Coefficient>::value));
        BOOST_STATIC_ASSERT((boost::is_same< PT::Innermost_coefficient, 
                        Innermost_coefficient>::value));
        BOOST_STATIC_ASSERT((PT::d == dimension));
        test_polynomial_traits_d<PT>();
        } 

#ifdef WITH_D10    
{
        typedef CGAL::Polynomial<leda_integer> Polynomial_1;
        typedef CGAL::Polynomial<Polynomial_1> Polynomial_2;
        typedef CGAL::Polynomial<Polynomial_2> Polynomial_3;
        typedef CGAL::Polynomial<Polynomial_3> Polynomial_4;
        typedef CGAL::Polynomial<Polynomial_4> Polynomial_5;
        typedef CGAL::Polynomial<Polynomial_5> Polynomial_6;
        typedef CGAL::Polynomial<Polynomial_6> Polynomial_7;
        typedef CGAL::Polynomial<Polynomial_7> Polynomial_8;
        typedef CGAL::Polynomial<Polynomial_8> Polynomial_9;
        typedef CGAL::Polynomial<Polynomial_9> Polynomial_10;

        const int dimension                  = 10;
        typedef Polynomial_10                   Polynomial_d;
        typedef Polynomial_9                   Coefficient;
        typedef leda_integer                   Innermost_coefficient;
        
        typedef CGAL::Polynomial_traits_d<Polynomial_d> PT;
        
        BOOST_STATIC_ASSERT(
                (boost::is_same< PT::Polynomial_d, Polynomial_d>::value));
        BOOST_STATIC_ASSERT(
                (boost::is_same< PT::Coefficient, Coefficient>::value));
        BOOST_STATIC_ASSERT((boost::is_same< PT::Innermost_coefficient, 
                        Innermost_coefficient>::value));
        BOOST_STATIC_ASSERT((PT::d == dimension));
        test_polynomial_traits_d<PT>();
        } 
#endif
    



 


    return 0;
}
