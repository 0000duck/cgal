// Copyright (c) 2005-2006  INRIA Sophia-Antipolis (France).
// All rights reserved.
//
// This file is part of CGAL (www.cgal.org); you may redistribute it under
// the terms of the Q Public License version 1.0.
// See the file LICENSE.QPL distributed with CGAL.
//
// Licensees holding a valid commercial license may use this file in
// accordance with the commercial license agreement provided with the software.
//
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
//
// Partially supported by the IST Programme of the EU as a Shared-cost
// RTD (FET Open) Project under Contract No  IST-2000-26473 
// (ECG - Effective Computational Geometry for Curves and Surfaces) 
// and a STREP (FET Open) Project under Contract No  IST-006413 
// (ACS -- Algorithms for Complex Shapes)
//
// $URL$
// $Id$
//
// Author(s) : Monique Teillaud <Monique.Teillaud@sophia.inria.fr>
//             Sylvain Pion     <Sylvain.Pion@sophia.inria.fr>
//             Pedro Machado    <tashimir@gmail.com>

#ifndef CGAL_SPHERICAL_KERNEL_PREDICATES_ON_SPHERE_3_H
#define CGAL_SPHERICAL_KERNEL_PREDICATES_ON_SPHERE_3_H

namespace CGAL {
  namespace SphericalFunctors {

    template < class SK >
    typename SK::Algebraic_kernel::Polynomials_for_line_3
    get_equation( const typename SK::Line_3 & l)
    {
      typedef typename SK::Algebraic_kernel AK;
      return AK().construct_polynomials_for_line_3_object()
	  (l.to_vector().x(), l.point().x(), 
           l.to_vector().y(), l.point().y(),
           l.to_vector().z(), l.point().z());
    }

    template < class SK >
    typename SK::Polynomial_1_3
    get_equation( const typename SK::Plane_3 & s )
    {
      typedef typename SK::Algebraic_kernel AK;
      return AK().construct_polynomial_1_3_object()
	  ( s.a(), s.b(), s.c(), s.d() );
    }

    template < class SK >
    typename SK::Polynomial_for_spheres_2_3
    get_equation( const typename SK::Sphere_3 & s )
    {
      typedef typename SK::RT RT;
      typedef typename SK::Point_3    Point_3;
      typedef typename SK::Algebraic_kernel   AK;
      Point_3 center = s.center();
      return AK().construct_polynomial_for_spheres_2_3_object()
	  ( center.x(), center.y(), center.z(), s.squared_radius() );
    }

    template < class SK >
    typename SK::Polynomials_for_circle_3
    get_equation( const typename SK::Circle_3 & c )
    {
      typedef typename SK::Algebraic_kernel AK;
      return std::make_pair ( AK().construct_polynomial_for_spheres_2_3_object()
                               (c.center().x(), c.center().y(), 
                                c.center().z(), c.squared_radius()),
                              AK().construct_polynomial_1_3_object()
                               (c.supporting_plane().a(),
                                c.supporting_plane().b(),
                                c.supporting_plane().c(),
                                c.supporting_plane().d()));
    }

    template < class SK >
    typename SK::Sphere_3
    construct_sphere_3(const typename SK::Polynomial_for_spheres_2_3 &eq)
    {
      typedef typename SK::Sphere_3 Sphere_3;
      typedef typename SK::Point_3  Point_3;
      return Sphere_3(Point_3(eq.a(),eq.b(),eq.c()),eq.r_sq());
    }

    template < class SK >
    inline
    typename SK::Linear_kernel::Bounded_side
    bounded_side(const typename SK::Sphere_3 &s,
                 const typename SK::Circular_arc_point_3 &p) {
      typedef typename SK::AK AK;
      typedef typename SK::Polynomial_for_spheres_2_3 Equation;
      Equation equation = get_equation<SK>(s);
      Sign sign = AK().sign_at_object()(equation,p.rep().coordinates());
      if(sign == NEGATIVE) return ON_BOUNDED_SIDE;
      else if(sign == POSITIVE) return ON_UNBOUNDED_SIDE;
      else return ON_BOUNDARY;
    }

    template < class SK >
    inline
    typename SK::Linear_kernel::Bounded_side
    bounded_side(const typename SK::Circle_3 &c,
                 const typename SK::Circular_arc_point_3 &p) {
      typedef typename SK::AK AK;
      typedef typename SK::Polynomial_for_spheres_2_3 Equation;
      CGAL_kernel_assertion(SK().has_on_3_object()(c.supporting_plane(),p));
      Equation equation = get_equation<SK>(c.diametral_sphere());
      Sign sign = AK().sign_at_object()(equation,p.rep().coordinates());
      if(sign == NEGATIVE) return ON_BOUNDED_SIDE;
      else if(sign == POSITIVE) return ON_UNBOUNDED_SIDE;
      else return ON_BOUNDARY;
    }

    template < class SK >
    inline
    bool
    non_oriented_equal(const typename SK::Sphere_3 & s1, 
                       const typename SK::Sphere_3 & s2) {
      // Should we compare anyway even if they are degenerated?
      CGAL_kernel_assertion(!(s1.is_degenerate() || s2.is_degenerate()));
      return s1.center() == s2.center() &&
             s1.squared_radius() == s2.squared_radius();
    }

    template < class SK >
    inline
    bool
    non_oriented_equal(const typename SK::Plane_3 & p1, 
                       const typename SK::Plane_3 & p2) {
      // Should we compare anyway even if they are degenerated?
      CGAL_kernel_assertion(!(p1.is_degenerate() || p2.is_degenerate()));
      if(is_zero(p1.a())) {
        if(!is_zero(p2.a())) return false;
        if(is_zero(p1.b())) {
          if(!is_zero(p2.b())) return false;
          return p1.c() * p2.d() == p1.d() * p2.c();
        }
        return (p2.c() * p1.b() == p1.c() * p2.b()) &&
               (p2.d() * p1.b() == p1.d() * p2.b());
      }
      return (p2.b() * p1.a() == p1.b() * p2.a()) &&
             (p2.c() * p1.a() == p1.c() * p2.a()) &&
             (p2.d() * p1.a() == p1.d() * p2.a());
    }

    template < class SK >
    inline
    bool
    non_oriented_equal(const typename SK::Circle_3 & c1, 
                       const typename SK::Circle_3 & c2) {
      // We see degeneracies on the other non_oriented_equal functions
      if(!non_oriented_equal<SK>(c1.diametral_sphere(),
                                 c2.diametral_sphere())) return false;
      if(!non_oriented_equal<SK>(c1.supporting_plane(),
                                 c2.supporting_plane())) return false;
      return true;
    }

    template< class SK>
    bool
    non_oriented_equal( const typename SK::Line_3 &l1,
                        const typename SK::Line_3 &l2)
    {
      typedef typename SK::Vector_3 Vector_3;
      if(!SK().has_on_3_object()(l1, l2.point())) return false;

      const Vector_3& v1 = l1.to_vector();
      const Vector_3& v2 = l2.to_vector();

      if(v1.x() * v2.y() != v1.y() * v2.x()) return false;
      if(v1.x() * v2.z() != v1.z() * v2.x()) return false;
      if(v1.y() * v2.z() != v1.z() * v2.y()) return false;

      return true;
    }

    template< class SK>
    bool
    non_oriented_equal( const typename SK::Line_arc_3 &l1,
                        const typename SK::Line_arc_3 &l2)
    {
      if(!non_oriented_equal<SK>(l1.supporting_line(),
                                 l2.supporting_line())) return false;
      return (l1.lower_xyz_extremity() == l2.lower_xyz_extremity()) &&
             (l1.higher_xyz_extremity() == l2.higher_xyz_extremity());
    }

    template< class SK>
    bool
    non_oriented_equal( const typename SK::Circular_arc_3 &c1,
                        const typename SK::Circular_arc_3 &c2)
    {
      if(!non_oriented_equal<SK>(c1.supporting_circle(),
                                 c2.supporting_circle())) return false;
      if(c1.rep().is_full() && c2.rep().is_full())
        return true;
      return (c1.source() == c2.source()) &&
             (c1.target() == c2.target());
    }

    template < class SK>
    inline
    typename SK::Plane_3
    radical_plane(const typename SK::Sphere_3 & s1, 
                  const typename SK::Sphere_3 & s2)
    {
      typedef typename SK::Plane_3 Plane_3;
      typedef typename SK::Point_3 Point_3;
      typedef typename SK::FT FT;
      // Concentric Spheres don't have radical plane
      CGAL_kernel_precondition (s1.center() != s2.center());
      const FT a = 2*(s2.center().x() - s1.center().x());
      const FT b = 2*(s2.center().y() - s1.center().y());
      const FT c = 2*(s2.center().z() - s1.center().z());
      const FT d = CGAL::square(s1.center().x()) + 
        CGAL::square(s1.center().y()) +
        CGAL::square(s1.center().z()) - s1.squared_radius() - 
        CGAL::square(s2.center().x()) - 
        CGAL::square(s2.center().y()) -
        CGAL::square(s2.center().z()) + s2.squared_radius();
      return Plane_3(a, b, c, d);
    }

    template < class SK, class OutputIterator >
    OutputIterator
    intersect_3(const typename SK::Plane_3 & p, 
               const typename SK::Sphere_3 & s, 
	       OutputIterator res) 
    { 
      typedef typename SK::AK AK;
      typedef typename SK::Sphere_3 Sphere_3;
      typedef typename SK::Polynomial_for_spheres_2_3 Polynomial_for_spheres_2_3;
      typedef typename SK::Polynomial_1_3 Polynomial_1_3;
      typedef typename SK::Circle_3 Circle_3;
      typedef typename SK::Plane_3 Plane_3;
      typedef typename SK::Point_3 Point_3;
      typedef typename SK::Circular_arc_point_3 Circular_arc_point_3;
      typedef typename SK::Root_for_spheres_2_3 Root_for_spheres_2_3;
      typedef typename SK::FT FT;

      CGAL_kernel_precondition(!p.rep().is_degenerate());
      CGAL_kernel_precondition(!s.rep().is_degenerate());

      const FT d2 = CGAL::square(p.a()*s.center().x() + 
                                 p.b()*s.center().y() + 
                                 p.c()*s.center().z() + p.d()) /
       (CGAL::square(p.a()) + CGAL::square(p.b()) + CGAL::square(p.c()));

      // do not intersect
      if(d2 >  s.squared_radius()) return res;
      // tangent
      if(d2 == s.squared_radius()) {
        Polynomial_for_spheres_2_3 e1 = get_equation<SK>(s);
        Polynomial_1_3 e2 = get_equation<SK>(p);
        typedef std::vector< std::pair < Root_for_spheres_2_3, unsigned > > 
        solutions_container;
        solutions_container solutions;
        AK().solve_object()(e1, e1, e2, std::back_inserter(solutions)); 
        // only 1 solution
        typename solutions_container::iterator it = solutions.begin(); 
	*res++ = make_object(std::make_pair(Circular_arc_point_3(it->first),
					    it->second ));
        return res;
      }
      // d2 < s.squared_radius()
      Point_3 center = p.projection(s.center());
      *res++ = make_object(Circle_3(center,s.squared_radius() - d2,p));
      return res;
    }

  namespace CGALi {
    template < class SK >
    inline
    bool
    do_intersect(const typename SK::Sphere_3 & s1, 
                const typename SK::Sphere_3 & s2)
    { 
      typedef typename SK::Root_of_2 Root_of_2;
      typedef typename SK::FT FT;
      const FT dx = s2.center().x() - s1.center().x();
      const FT dy = s2.center().y() - s1.center().y();
      const FT dz = s2.center().z() - s1.center().z();
      const FT d2 = CGAL::square(dx) +
                  CGAL::square(dy) +
                  CGAL::square(dz);
      const FT sq_r1r2 = s1.squared_radius()*s2.squared_radius();
      const FT sq_r1_p_sq_r2 = s1.squared_radius() + s2.squared_radius();
      Root_of_2 left_1 = make_root_of_2(d2,FT(-2),sq_r1r2);
      if(left_1 > sq_r1_p_sq_r2) return false;
      Root_of_2 left_2 = make_root_of_2(d2,FT(2),sq_r1r2);
      if(left_2 < sq_r1_p_sq_r2) return false;
      return true;
    }  
  }

    template < class SK, class OutputIterator >
    OutputIterator
    intersect_3(const typename SK::Sphere_3 & s1, 
                const typename SK::Sphere_3 & s2, 
	        OutputIterator res)
    { 
      typedef typename SK::Plane_3 Plane_3;
      typedef typename SK::Sphere_3 Sphere_3;
      CGAL_kernel_precondition(!s1.rep().is_degenerate());
      CGAL_kernel_precondition(!s2.rep().is_degenerate());
      if(non_oriented_equal<SK>(s1,s2)) {
        *res++ = make_object(s1);
        return res;
      }
      if(!CGALi::do_intersect<SK>(s1,s2)) return res;
      Plane_3 p = radical_plane<SK>(s1,s2);
      return intersect_3<SK>(p,s1,res);
    }

    template < class SK, class OutputIterator >
    OutputIterator
    intersect_3(const typename SK::Sphere_3 & s, 
                const typename SK::Line_3 & l, 
	        OutputIterator res)
    {
      typedef typename SK::AK                          AK;
      typedef typename SK::Polynomial_for_spheres_2_3  Equation_sphere; 
      typedef typename SK::Polynomials_for_line_3      Equation_line; 
      typedef typename SK::Root_for_spheres_2_3        Root_for_spheres_2_3;
      typedef typename SK::Circular_arc_point_3        Circular_arc_point_3;
      CGAL_kernel_precondition(!s.rep().is_degenerate());
      CGAL_kernel_precondition(!l.rep().is_degenerate());
      Equation_sphere e1 = get_equation<SK>(s);
      Equation_line e2 = get_equation<SK>(l);
      typedef std::vector< std::pair < Root_for_spheres_2_3, unsigned > > 
        solutions_container;
      solutions_container solutions;
      AK().solve_object()(e1, e2, std::back_inserter(solutions)); 
      for ( typename solutions_container::iterator it = solutions.begin(); 
	  it != solutions.end(); ++it ) {
        *res++ = make_object(std::make_pair(Circular_arc_point_3(it->first),
					    it->second ));
      }
      return res;
    }

    template < class SK, class OutputIterator >
    OutputIterator
    intersect_3(const typename SK::Sphere_3 & s1, 
                const typename SK::Sphere_3 & s2, 
	        const typename SK::Sphere_3 & s3, 
                OutputIterator res)
    { 
       typedef typename SK::Polynomial_for_spheres_2_3  Equation_sphere;
       typedef typename SK::Root_for_spheres_2_3  Root_for_spheres_2_3;
       typedef typename SK::Circular_arc_point_3  Circular_arc_point_3;
       typedef typename SK::Circle_3  Circle_3;
       typedef typename SK::AK  AK;
       typedef std::vector< Object > solutions_container;
       CGAL_kernel_precondition(!s1.rep().is_degenerate());
       CGAL_kernel_precondition(!s2.rep().is_degenerate());
       CGAL_kernel_precondition(!s3.rep().is_degenerate());
       if(non_oriented_equal<SK>(s1,s2) && non_oriented_equal<SK>(s2,s3)) {
         *res++ = make_object(s1);
         return res;
       }
       if(non_oriented_equal<SK>(s1,s2)) {
         return intersect_3<SK>(s1,s3,res);
       }
       if((non_oriented_equal<SK>(s1,s3)) || 
          (non_oriented_equal<SK>(s2,s3))) {
         return intersect_3<SK>(s1,s2,res);
       }
       if(SK().collinear_3_object()(s1.center(),s2.center(),s3.center())) {
         solutions_container solutions;
         intersect_3<SK>(s1, s2, std::back_inserter(solutions));
         if(solutions.size() == 0) return res;
         std::pair<Circular_arc_point_3, unsigned> pair;
         if(assign(pair,solutions[0])) {
           if(SK().has_on_3_object()(s3,pair.first)) {
             *res++ = solutions[0];
             return res;
           } else return res;
         }
         // must be a circle
         Circle_3 c;
         assign(c,solutions[0]);
         if(SK().has_on_3_object()(s3,c)) {
           *res++ = solutions[0];
           return res;
         }
         return res;
       }
       Equation_sphere e1 = get_equation<SK>(s1);
       Equation_sphere e2 = get_equation<SK>(s2);
       Equation_sphere e3 = get_equation<SK>(s3);
       typedef std::vector< std::pair < Root_for_spheres_2_3, unsigned > > 
         algebraic_solutions_container;
       algebraic_solutions_container solutions;
       AK().solve_object()(e1, e2, e3, std::back_inserter(solutions)); 
       for ( typename algebraic_solutions_container::iterator it = 
             solutions.begin(); it != solutions.end(); ++it ) {
        *res++ = make_object(std::make_pair(Circular_arc_point_3(it->first),
					    it->second ));
       }
       return res;
    }

    template < class SK, class OutputIterator >
    OutputIterator
    intersect_3(const typename SK::Plane_3 & p, 
                const typename SK::Sphere_3 & s1, 
	        const typename SK::Sphere_3 & s2, 
                OutputIterator res)
    { 
      typedef typename SK::Root_for_spheres_2_3  Root_for_spheres_2_3;
      typedef typename SK::Circular_arc_point_3  Circular_arc_point_3;
      typedef typename SK::Polynomial_for_spheres_2_3  Equation_sphere;
      typedef typename SK::Polynomial_1_3  Equation_plane;
      typedef typename SK::Plane_3  Plane_3;
      typedef typename SK::AK  AK;
      CGAL_kernel_precondition(!p.rep().is_degenerate());
      CGAL_kernel_precondition(!s1.rep().is_degenerate());
      CGAL_kernel_precondition(!s2.rep().is_degenerate());
      if(non_oriented_equal<SK>(s1,s2)) {
         return intersect_3<SK>(p,s1,res);
      }
      Plane_3 radical_p = radical_plane<SK>(s1,s2);
      if(non_oriented_equal<SK>(p,radical_p)) {
         return intersect_3<SK>(p,s1,res);
      }
      Equation_sphere e1 = get_equation<SK>(s1);
      Equation_sphere e2 = get_equation<SK>(s2);
      Equation_plane e3 = get_equation<SK>(p);
      typedef std::vector< std::pair < Root_for_spheres_2_3, unsigned > > 
        algebraic_solutions_container;
      algebraic_solutions_container solutions;
      AK().solve_object()(e1, e2, e3, std::back_inserter(solutions)); 
      for ( typename algebraic_solutions_container::iterator it = 
            solutions.begin(); it != solutions.end(); ++it ) {
       *res++ = make_object(std::make_pair(Circular_arc_point_3(it->first),
				    it->second ));
      }
      return res;
    }

    template < class SK, class OutputIterator >
    OutputIterator
    intersect_3(const typename SK::Plane_3 & p1, 
                const typename SK::Plane_3 & p2, 
	        const typename SK::Sphere_3 & s, 
                OutputIterator res)
    { 
      typedef typename SK::Root_for_spheres_2_3  Root_for_spheres_2_3;
      typedef typename SK::Circular_arc_point_3  Circular_arc_point_3;
      typedef typename SK::Polynomial_for_spheres_2_3  Equation_sphere;
      typedef typename SK::Polynomial_1_3  Equation_plane;
      typedef typename SK::AK  AK;
      CGAL_kernel_precondition(!p1.rep().is_degenerate());
      CGAL_kernel_precondition(!p2.rep().is_degenerate());
      CGAL_kernel_precondition(!s.rep().is_degenerate());
      if(non_oriented_equal<SK>(p1,p2)) {
         return intersect_3<SK>(p1,s,res);
      }
      Equation_plane e1 = get_equation<SK>(p1);
      Equation_plane e2 = get_equation<SK>(p2);
      Equation_sphere e3 = get_equation<SK>(s);
      typedef std::vector< std::pair < Root_for_spheres_2_3, unsigned > > 
        algebraic_solutions_container;
      algebraic_solutions_container solutions;
      AK().solve_object()(e1, e2, e3, std::back_inserter(solutions)); 
      for ( typename algebraic_solutions_container::iterator it = 
            solutions.begin(); it != solutions.end(); ++it ) {
       *res++ = make_object(std::make_pair(Circular_arc_point_3(it->first),
				    it->second ));
      }
      return res;
    }

    template < class SK, class OutputIterator >
    OutputIterator
    intersect_3(const typename SK::Circle_3 & c, 
                const typename SK::Plane_3 & p,
                OutputIterator res)
    { 
      return intersect_3<SK>(p,c.supporting_plane(),c.diametral_sphere(),res);
    }

    template < class SK, class OutputIterator >
    OutputIterator
    intersect_3(const typename SK::Circle_3 & c, 
                const typename SK::Sphere_3 & s,
                OutputIterator res)
    { 
      return intersect_3<SK>(c.supporting_plane(),s,c.diametral_sphere(),res);
    }

    template < class SK, class OutputIterator >
    OutputIterator
    intersect_3(const typename SK::Circle_3 & c1, 
                const typename SK::Circle_3 & c2,
                OutputIterator res)
    { 
      typedef typename SK::Root_for_spheres_2_3  Root_for_spheres_2_3;
      typedef typename SK::Circular_arc_point_3  Circular_arc_point_3;
      typedef typename SK::Polynomials_for_circle_3  Equation_circle;
      typedef typename SK::Circle_3  Circle_3;
      typedef typename SK::AK  AK;
      if(non_oriented_equal<SK>(c1,c2)) {
         *res++ = make_object(c1);
         return res;
      }
      Equation_circle e1 = get_equation<SK>(c1);
      Equation_circle e2 = get_equation<SK>(c2);
      typedef std::vector< std::pair < Root_for_spheres_2_3, unsigned > > 
        algebraic_solutions_container;
      algebraic_solutions_container solutions;
      AK().solve_object()(e1, e2, std::back_inserter(solutions)); 
      for ( typename algebraic_solutions_container::iterator it = 
            solutions.begin(); it != solutions.end(); ++it ) {
        *res++ = make_object(std::make_pair(Circular_arc_point_3(it->first),
				    it->second ));
      }
      return res;
    }

    template < class SK, class OutputIterator >
    OutputIterator
    intersect_3(const typename SK::Circle_3 & c, 
                const typename SK::Line_3 & l,
                OutputIterator res)
    { 
      typedef typename SK::Root_for_spheres_2_3  Root_for_spheres_2_3;
      typedef typename SK::Circular_arc_point_3  Circular_arc_point_3;
      typedef typename SK::Polynomials_for_circle_3  Equation_circle;
      typedef typename SK::Polynomials_for_line_3    Equation_line;
      typedef typename SK::Circle_3  Circle_3;
      typedef typename SK::AK  AK;
      CGAL_kernel_precondition(!l.rep().is_degenerate());
      Equation_circle e1 = get_equation<SK>(c);
      Equation_line e2 = get_equation<SK>(l);
      typedef std::vector< std::pair < Root_for_spheres_2_3, unsigned > > 
        algebraic_solutions_container;
      algebraic_solutions_container solutions;
      AK().solve_object()(e1, e2, std::back_inserter(solutions)); 
      for ( typename algebraic_solutions_container::iterator it = 
            solutions.begin(); it != solutions.end(); ++it ) {
        *res++ = make_object(std::make_pair(Circular_arc_point_3(it->first),
				    it->second ));
      }
      return res;
    }

    // At the moment we dont need those functions
    // But in the future maybe (some make_x_monotone? etc..)
    template <class SK>
    typename SK::Circular_arc_point_3
    x_extremal_point(const typename SK::Sphere_3 & c, bool i)
    {
      typedef typename SK::Algebraic_kernel   AK;
      return AK().x_critical_points_object()(typename SK::Get_equation()(c),i);
    }

    template <class SK,class OutputIterator>
    OutputIterator
    x_extremal_points(const typename SK::Sphere_3 & c, OutputIterator res)
    {
      typedef typename SK::Algebraic_kernel   AK;
      return AK().x_critical_points_object()(typename SK::Get_equation()(c),res);
    }

    template <class SK>
    typename SK::Circular_arc_point_3
    y_extremal_point(const typename SK::Sphere_3 & c, bool i)
    {
      typedef typename SK::Algebraic_kernel   AK;
      return AK().y_critical_points_object()(typename SK::Get_equation()(c),i);
    }

    template <class SK,class OutputIterator>
    OutputIterator
    y_extremal_points(const typename SK::Sphere_3 & c, OutputIterator res)
    {
      typedef typename SK::Algebraic_kernel   AK;
      return AK().y_critical_points_object()(typename SK::Get_equation()(c),res);
    }

    template <class SK>
    typename SK::Circular_arc_point_3
    z_extremal_point(const typename SK::Sphere_3 & c, bool i)
    {
      typedef typename SK::Algebraic_kernel   AK;
      return AK().z_critical_points_object()(typename SK::Get_equation()(c),i);
    }

    template <class SK,class OutputIterator>
    OutputIterator
    z_extremal_points(const typename SK::Sphere_3 & c, OutputIterator res)
    {
      typedef typename SK::Algebraic_kernel   AK;
      return AK().z_critical_points_object()(typename SK::Get_equation()(c),res);
    }

    template <class SK>
    typename SK::Circular_arc_point_3
    x_extremal_point(const typename SK::Circle_3 & c, bool i)
    {
      typedef typename SK::Algebraic_kernel   AK;
      return AK().x_critical_points_object()(typename SK::Get_equation()(c),i);
    }

    template <class SK,class OutputIterator>
    OutputIterator
    x_extremal_points(const typename SK::Circle_3 & c, OutputIterator res)
    {
      typedef typename SK::Algebraic_kernel   AK;
      return AK().x_critical_points_object()(typename SK::Get_equation()(c),res);
    }

    template <class SK>
    typename SK::Circular_arc_point_3
    y_extremal_point(const typename SK::Circle_3 & c, bool i)
    {
      typedef typename SK::Algebraic_kernel   AK;
      return AK().y_critical_points_object()(typename SK::Get_equation()(c),i);
    }

    template <class SK,class OutputIterator>
    OutputIterator
    y_extremal_points(const typename SK::Circle_3 & c, OutputIterator res)
    {
      typedef typename SK::Algebraic_kernel   AK;
      return AK().y_critical_points_object()(typename SK::Get_equation()(c),res);
    }

    template <class SK>
    typename SK::Circular_arc_point_3
    z_extremal_point(const typename SK::Circle_3 & c, bool i)
    {
      typedef typename SK::Algebraic_kernel   AK;
      return AK().z_critical_points_object()(typename SK::Get_equation()(c),i);
    }

    template <class SK,class OutputIterator>
    OutputIterator
    z_extremal_points(const typename SK::Circle_3 & c, OutputIterator res)
    {
      typedef typename SK::Algebraic_kernel   AK;
      return AK().z_critical_points_object()(typename SK::Get_equation()(c),res);
    }

  }//SphericalFunctors
}//CGAL

#endif //CGAL_SPHERICAL_KERNEL_PREDICATES_ON_SPHERE_3_H
