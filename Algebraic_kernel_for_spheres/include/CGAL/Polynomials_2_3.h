// Copyright (c) 2005  INRIA Sophia-Antipolis (France) 
// All rights reserved.
//
// Authors : Monique Teillaud <Monique.Teillaud@sophia.inria.fr>
//           Sylvain Pion     <Sylvain.Pion@sophia.inria.fr>
//           Julien Hazebrouck
//           Damien Leroy
// 
// Partially supported by the IST Programme of the EU as a Shared-cost
// RTD (FET Open) Project under Contract No  IST-2000-26473 
// (ECG - Effective Computational Geometry for Curves and Surfaces) 
// and a STREP (FET Open) Project under Contract No  IST-006413 
// (ACS -- Algorithms for Complex Shapes)

#ifndef CGAL_ALGEBRAIC_KERNEL_POLYNOMIALS_2_3_H
#define CGAL_ALGEBRAIC_KERNEL_POLYNOMIALS_2_3_H

#include <CGAL/enum.h>

CGAL_BEGIN_NAMESPACE

// polynomials of the form (X-a)^2 + (Y-b)^2 + (Z-c)^2 - R^2
template < typename FT_ >
class Polynomial_for_spheres_2_3
{
  FT_ rep[4]; // stores a, b, c, R^2
  
public:
  
  typedef FT_ FT;
  
  Polynomial_for_spheres_2_3(){}
  
  Polynomial_for_spheres_2_3(const FT & a, const FT & b, const FT & c, const FT & rsq)
  { 
    rep[0]=a;
    rep[1]=b;
    rep[2]=c;
    rep[3]=rsq;
  }

  const FT & a() const
  { return rep[0]; }

  const FT & b() const
  { return rep[1]; }
  
  const FT & c() const
  { return rep[2]; }
  
  const FT & r_sq() const
  { return rep[3]; }

  const bool empty_space() const {
    return is_negative(r_sq());
  }

  const bool isolated_point() const {
    return is_zero(r_sq());
  }

};

template < typename FT >
inline
bool 
operator == ( const Polynomial_for_spheres_2_3<FT> & p1,
	      const Polynomial_for_spheres_2_3<FT> & p2 )
{
  return( (p1.a() == p2.a()) && 
	  (p1.b() == p2.b()) &&
	  (p1.c() == p2.c()) &&
	  (p1.r_sq() == p2.r_sq()) );
}

CGAL_END_NAMESPACE

#endif //CGAL_ALGEBRAIC_KERNEL_POLYNOMIALS_2_3_H
