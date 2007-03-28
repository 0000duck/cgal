#include <CGAL/basic.h>
#include <CGAL/Polynomial.h>
#include <CGAL/Modular_traits.h>

//fwd: try to apply modular filtering 
template< typename Polynomial >
Polynomial modular_filtered_gcd(const Polynomial& p1, const Polynomial& p2);

// fwd: function if Polynomial is Modularizable
template< typename Polynomial >
bool may_have_common_factor(
        const Polynomial& p1, const Polynomial& p2, CGAL::Tag_true);

// fwd: function if Polynomial is not Modularizable
template< typename Polynomial >
bool may_have_common_factor(
        const Polynomial& p1, const Polynomial& p2, CGAL::Tag_false);



template< typename Polynomial >
Polynomial modular_filtered_gcd(const Polynomial& p1, const Polynomial& p2){
    typedef CGAL::Modular_traits<Polynomial> MT;
    typedef typename MT::Is_modularizable Is_modularizable;
    
    // Try to avoid actual gcd computation 
    if (may_have_common_factor(p1,p2, Is_modularizable())){
        // Compute gcd, since the filter indicates a common factor
        return CGAL::gcd(p1,p2);
    }else{
        return Polynomial(1); // return trivial gcd 
    }
}

// Function in case  Polynomial is Modularizable
template< typename Polynomial >
bool may_have_common_factor(
        const Polynomial& p1, const Polynomial& p2, CGAL::Tag_true){
    
    // Use Modular_traits to convert to polynomials with modular coefficients
    typedef CGAL::Modular_traits<Polynomial> MT;
    typedef typename MT::Modular_type modular_polynomial;
    typedef typename MT::Modular_image Modular_image;
    modular_polynomial mp1 = Modular_image()(p1);
    modular_polynomial mp2 = Modular_image()(p2);
    // compute gcd for modular images 
    modular_polynomial mg  = CGAL::gcd(mp1,mp2);
    
    typename CGAL::Polynomial_traits_d<Polynomial>::Degree degree;
    // if the modular gcd is not trivial: return true 
    return (degree(mg) > 0);
}

// This function returns true, since the filter is not applicable 
template< typename Polynomial >
bool may_have_common_factor(
        const Polynomial& p1, const Polynomial& p2, CGAL::Tag_false){
    return true; 
}

#ifdef CGAL_USE_GMP
#include <CGAL/Gmpz.h>
int main(){
    CGAL::set_pretty_mode(std::cout);
    
    typedef CGAL::Gmpz NT; 
    typedef CGAL::Polynomial<NT> Poly; 

    Poly  f1(NT(2), NT(7), NT(1)); 
    Poly  f2(NT(3), NT(1), NT(4));
    Poly  f3(NT(3), NT(4));
    
    std::cout << "f1        : " << f1 << std::endl;
    std::cout << "f2        : " << f2 << std::endl;
    std::cout << "f3        : " << f3 << std::endl;

    Poly p1 = f1*f3;
    Poly p2 = f2*f3;
    
    std::cout << "p1=f1*f3  : " << p1 << std::endl;
    std::cout << "p2=f2*f3  : " << p2 << std::endl;

    std::cout << "modular filtered gcd: " << std::endl;
    Poly g = modular_filtered_gcd(p1,p2);
    std::cout << "gcd(p1,p2): " << g << std::endl;

}
#else
int main(){ std::cout << "This examples needs GMP" << std::endl; }
#endif
