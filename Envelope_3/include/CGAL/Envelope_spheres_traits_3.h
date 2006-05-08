// Copyright (c) 2005  Tel-Aviv University (Israel).
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
// $Source: /CVSROOT/CGAL/Packages/Envelope_3/include/CGAL/Envelope_spheres_traits_3.h,v $
// $Revision$ $Date$
// $Name:  $
//
// Author(s)     : Michal Meyerovitch     <gorgymic@post.tau.ac.il>

#ifndef ENVELOPE_SPHERES_TRAITS_3_H
#define ENVELOPE_SPHERES_TRAITS_3_H

#include <CGAL/Object.h>
#include <CGAL/enum.h>
#include <CGAL/Bbox_3.h>
#include <CGAL/functions_on_signs.h>

//#define CGAL_DEBUG_ENVELOPE_3_SPHERES_TRAITS
//#define CGAL_ENV_SPHERES_TRAITS_CACHE_POINT_ON

CGAL_BEGIN_NAMESPACE

inline
Sign
operator*(const Sign &s1, const Sign &s2)
{
  if ( s1 == ZERO || s2 == ZERO )  return ZERO;
  if ( s1 == s2 )  return POSITIVE;
  return NEGATIVE;
}

template <class Kernel_> class Envelope_sphere_3;
 
template <class ConicTraits_2>
class Envelope_spheres_traits_3 : public ConicTraits_2
{
public:
  typedef ConicTraits_2                             Traits_2;
  typedef Envelope_spheres_traits_3<Traits_2>       Self;
  
  typedef typename Traits_2::Point_2                Point_2;
  typedef typename Traits_2::Curve_2                Curve_2;
  typedef typename Traits_2::X_monotone_curve_2     X_monotone_curve_2;

  typedef typename Traits_2::Rat_kernel             Rat_kernel;
  typedef typename Traits_2::Alg_kernel             Alg_kernel;
  typedef typename Traits_2::Nt_traits              Nt_traits;

  typedef typename Rat_kernel::FT                   Rational;
  typedef typename Rat_kernel::Point_2              Rat_point_2;
  typedef typename Rat_kernel::Segment_2            Rat_segment_2;
  typedef typename Rat_kernel::Line_2               Rat_line_2;
  typedef typename Rat_kernel::Circle_2             Rat_circle_2;
  typedef typename Rat_kernel::Point_3              Rat_point_3;

  typedef typename Alg_kernel::FT                   Algebraic;
  typedef typename Alg_kernel::Point_2              Alg_point_2;
  typedef typename Alg_kernel::Circle_2             Alg_circle_2;
  
//  typedef typename Rat_kernel::Sphere_3             Surface_3;
  typedef Envelope_sphere_3<Rat_kernel>             Surface_3;
  
  // here we refer to the lower part of the sphere only
  typedef Surface_3                                 Xy_monotone_surface_3;
protected:
  typedef std::pair<Curve_2, Intersection_type>     Intersection_curve;
  
  #ifdef CGAL_ENV_SPHERES_TRAITS_CACHE_POINT_ON
    // caching the computation of a surface 3d point from a 2d point
    typedef std::pair<Xy_monotone_surface_3, Point_2> Surface_point_pair;
    struct Less_surface_point_pair
    {
      bool operator() (const Surface_point_pair& sp1,
                       const Surface_point_pair& sp2) const
      {
        // Compare the pairs of IDs lexicographically.
        return (sp1.first < sp2.first ||
                (sp1.first == sp2.first && Alg_kernel().less_xy_2_object()(sp1.second,sp2.second)));
      }
    };
    typedef std::map<Surface_point_pair, Algebraic,
                     Less_surface_point_pair>          Surface_point_cache;
  #endif

public:
  class Construct_envelope_xy_monotone_parts_3

  {
  protected:
    const Self& parent;
  public:
    Construct_envelope_xy_monotone_parts_3(const Self* p)
      : parent(*p)
    {}

    // create xy-monotone surfaces from a general surface
    // return a past-the-end iterator
    template <class OutputIterator>
    OutputIterator operator()(const Surface_3& s, OutputIterator o) const
    {
      // our half sphere is of same type as our full sphere since we always
      // need only the lower/upper part of each sphere
      parent.total_timer.start();
      *o++ = s;
      parent.total_timer.stop();
      return o;
    }
  };

  /*! Get a Construct_envelope_xy_monotone_parts_3 functor object. */
  Construct_envelope_xy_monotone_parts_3
  construct_envelope_xy_monotone_parts_3_object() const
  {
    return Construct_envelope_xy_monotone_parts_3(this);
  }

  class Construct_projected_boundary_curves_2
  {
  protected:
    const Self& parent;

  public:
    Construct_projected_boundary_curves_2(const Self* p)
      : parent(*p)
    {}

    // insert into the OutputIterator all the (2d) curves of the boundary of
    // the vertical projection of the surface on the xy-plane
    // the OutputIterator value type is Curve_2
    template <class OutputIterator>
    OutputIterator
    operator()(const Xy_monotone_surface_3& s, OutputIterator o) const
    {
      parent.total_timer.start();
      // the projected boundary in a circle, with a projected center,
      // and same radius
      Rat_point_2 proj_center = parent.project(s.center());
      Rat_circle_2 circ(proj_center, s.squared_radius());
      *o++ = Curve_2(circ);
      parent.total_timer.stop();
      return o;
    }  
  };  
  
  /*! Get a Construct_projected_boundary_curves_2 functor object. */
  Construct_projected_boundary_curves_2
  construct_projected_boundary_curves_2_object() const
  {
    return Construct_projected_boundary_curves_2(this);
  }

  class Construct_projected_intersections_2
  {
  protected:
    const Self& parent;


  public:
    Construct_projected_intersections_2(const Self* p)
      : parent(*p)
    {}
    
    // insert into OutputIterator all the (2d) projections on the xy plane of
    // the intersection objects between the 2 surfaces
    // the data type of OutputIterator is Object
    template <class OutputIterator>
    OutputIterator
    operator()(const Xy_monotone_surface_3& s1,
               const Xy_monotone_surface_3& s2,
               OutputIterator o) const
    {
      parent.total_timer.start();
      parent.intersection_timer.start();
      
      Rat_point_3 p1 = s1.center();
      Rat_point_3 p2 = s2.center();
      const Rational a1 = p1.x(), b1 = p1.y(), c1 = p1.z(),
                 a2 = p2.x(), b2 = p2.y(), c2 = p2.z();
      const Rational sqr_r1 = s1.squared_radius(), 
                     sqr_r2 = s2.squared_radius();

//      // the spheres intersect iff d(p1, p2) <= (r1+r2)
//      // squaring this twice, we get the condition
//      // sqr_d^2 + (1-2*sqr_d)(sqr_r1 + sqr_r2) - 2*sqr_r1*sqr_r2 <= 0
//      // with only rational numbers involved.
//      // todo: check if it helps
//      Rat_kernel ratk;
//      Rational sqr_d = ratk.compute_squared_distance_3_object()(p1, p2);
//      Sign do_inter = CGAL_NTS sign(sqr_d*sqr_d + (1-2*sqr_d)*(sqr_r1+sqr_r2)-2*sqr_r1*sqr_r2);
//      if (do_inter == POSITIVE)
//        return o;
        
      Nt_traits nt_traits;

      // we check if the centers of the 2 spheres have same z coordinate -
      // in this case the potential projected intersection is a segment
      // (or point)
      if (CGAL_NTS compare(c1, c2) == EQUAL)
      {
        if (CGAL_NTS compare(b1, b2) == EQUAL)
        {
          if (CGAL_NTS compare(a1, a2) == EQUAL)
            // the same center, we have no intersection
            // (we don't return overlappings as intersections)
          {
            parent.total_timer.stop();
            parent.intersection_timer.stop();  
            return o;
          }

          // here we have c1 == c2, b1 == b2
          
          // the intersection lies on the plane
          //                m
          //  (1)    x = --------
          //             2(a1-a2)
          // where m = (a1^2 - a2^2 + sqr_r2 - sqr_r1)

          // which is orthogonal to the xy-plane
          // we look at the intersection of this plane with the plane z = c1
          // to get the projected segment of the spheres intersection

          // we get the quadratic equation:
          // A*y^2 + B*y + C = 0, where:
          //    A = 4(a1-a2)^2
          //    B = -8b1(a1-a2)^2
          //    C = m^2 - 4ma1(a1-a2) + 4(a1^2 + b1^2 - sqr_r1)(a1-a2)^2
          // (we multiplied the equation by 4(a1-a2)^2 to get integer
          //  coefficients)
          
          Rational a_diff = a1 - a2;
          Rational sqr_a_diff = a_diff * a_diff;
          Rational sqr_a1 = a1*a1, sqr_a2 = a2*a2;
          Rational sqr_b1 = b1*b1;
          Rational m = sqr_a1 - sqr_a2 + sqr_r2 - sqr_r1;
         
          Rational A = 4*sqr_a_diff;
          Rational B = -8*b1*sqr_a_diff;
          Rational C = 4*sqr_a_diff*(sqr_a1+sqr_b1-sqr_r1) + m*m - 4*m*a1*a_diff;

          
          Algebraic  ys[2];
          Algebraic *ys_end;
          int   n_ys;

          ys_end = nt_traits.solve_quadratic_equation(A, B, C, ys);
          n_ys = ys_end - ys;

          if (n_ys == 0)
          {
            parent.total_timer.stop();
            parent.intersection_timer.stop();          

            return o; // no intersection
          }

          // the x coordinate of the solution points
          Algebraic xs = m / (2*a_diff);

          if (n_ys == 1)
          {
            // intersection is a point
            Point_2 inter_point(xs , ys[0]);
            *o++ = make_object(inter_point);
            parent.total_timer.stop();
            parent.intersection_timer.stop();
            return o;
          }

          CGAL_assertion(n_ys == 2);
          // intersection is a segment, with non-rational endpoints
          // so we construct a COLLINEAR conic (with equation as in (1))
          // with 2 endpoints

          Alg_point_2 end1(xs, ys[0]);
          Alg_point_2 end2(xs, ys[1]);

          // equation (1) is:
          // 2(a1-a2)x - m = 0

          Curve_2 res(0,0,0, 2*a_diff, 0, -m, COLLINEAR, end1, end2);
          *o++ = make_object(Intersection_curve(res, TRANSVERSAL));
        }
        else
        {
          // here we have c1 == c2, b1 != b2
          #ifdef CGAL_DEBUG_ENVELOPE_3_SPHERES_TRAITS
            std::cout << "intersections: the case c1 == c2, b1 != b2"
                      << std::endl;
          #endif

          // the intersection lies on the plane
          //             -2(a1-a2)x + m
          //  (1)    y = ----------------
          //                2(b1-b2)
          // where m = (a1^2 + b1^2 - a2^2 - b2^2 + sqr_r2 - sqr_r1)
          
          // which is orthogonal to the xy-plane
          // we look at the intersection of this plane with the plane z = c1
          // to get the projected segment of the spheres intersection

          // we get the quadratic equation:
          // A*x^2 + B*x + C = 0
          // where
          //         (a1-a2)^2                      m(a1-a2)    2b1(a1-a2)
          // A = 1 + ---------          B = -2a1 - ---------- + ----------
          //         (b1-b2)^2                     (b1-b2)^2     (b1-b2)
          // and                                          m^2       b1*m
          //               C = a1^2 + b1^2 - sqr_r1 + ---------- - -------
          //                                          4(b1-b2)^2   (b1-b2)
          
          // since we can solve only equations with integer coefficients we 
          // multiply everything by 4(b1 - b2)^2, and get:
          // D*x^2 + E*x + F = 0 where
          //         D = 4(b1-b2)^2 + 4(a1-a2)^2
          //         E = -8a1(b1-b2)^2 - 4m(a1-a2) + 8b1(a1-a2)(b1-b2)
          //         F = 4(a1^2 + b1^2 - sqr_r1)(b1-b2)^2 + m^2 - 4mb1(b1-b2)
          // TODO: in the new version coefficients can be rationals
          Rational a_diff = a1 - a2;
          Rational b_diff = b1 - b2;
          Rational sqr_a_diff = a_diff * a_diff;
          Rational sqr_b_diff = b_diff * b_diff;
          Rational sqr_a1 = a1*a1, sqr_a2 = a2*a2;
          Rational sqr_b1 = b1*b1, sqr_b2 = b2*b2;
          Rational m = sqr_a1 + sqr_b1 - sqr_a2 - sqr_b2 + sqr_r2 - sqr_r1;

          Rational D = 4*sqr_a_diff + 4*sqr_b_diff;
          Rational E = -8*a1*sqr_b_diff - 4*m*a_diff + 8*b1*a_diff*b_diff;
          Rational F = 4*sqr_b_diff*(sqr_a1+sqr_b1-sqr_r1) + m*m - 4*m*b1*b_diff;

          #ifdef CGAL_DEBUG_ENVELOPE_3_SPHERES_TRAITS
            std::cout << "D = " << D << std::endl << "E = " << E << std::endl
                      << "F = " << F << std::endl;
          #endif
          
          Algebraic  xs[2];
          Algebraic *xs_end;
          int   n_xs;

          xs_end = nt_traits.solve_quadratic_equation(D, E, F, xs);
          n_xs = xs_end - xs;

          if (n_xs == 0)
          {
            parent.total_timer.stop();
            parent.intersection_timer.stop();
            return o; // no intersection
          }
          if (n_xs == 1)
          {
            // intersection is a point
            Point_2 inter_point(xs[0], (-2*a_diff*xs[0] + m)/(2*b_diff) );
            *o++ = make_object(inter_point);
            parent.total_timer.stop();
            parent.intersection_timer.stop();
            return o;
          }

          CGAL_assertion(n_xs == 2);
          // intersection is a segment, with non-rational endpoints
          // so we construct a COLLINEAR conic (with equation as in (1))
          // with 2 endpoints
          Algebraic ys[2];
          ys[0] = (-2*a_diff*xs[0] + m)/(2*b_diff);
          ys[1] = (-2*a_diff*xs[1] + m)/(2*b_diff);                

          Alg_point_2 end1(xs[0], ys[0]);
          Alg_point_2 end2(xs[1], ys[1]);

          #ifdef CGAL_DEBUG_ENVELOPE_3_SPHERES_TRAITS
            std::cout << "the projected segment endpoints are: " << std::endl 
                      << end1 << std::endl << end2 << std::endl;
          #endif
                              
          // equation (1) is:
          // 2(a1-a2)x + 2(b1-b2)y - m = 0
          Curve_2 res(0,0,0, 2*a_diff, 2*b_diff, -m, COLLINEAR, end1, end2);
          *o++ = make_object(Intersection_curve(res, TRANSVERSAL));
        }
        
      }
      // now the potential intersection is (a part of) a circle,
      // and the projection is (a part of) an ellipse
      else
      {
        // here we have c1 != c2
        #ifdef CGAL_DEBUG_ENVELOPE_3_SPHERES_TRAITS
          std::cout << "intersections: the case c1 != c2" << std::endl;
        #endif

        // the intersection lies on the plane:
        //           -2(a1-a2)x -2(b1-b2)y + m
        // (*)   z = --------------------------
        //                 2(c1-c2)
        // where m = a1^2 + b1^2 + c1^2 - a2^2 - b2^2 - c2^2 + sqr_r2 - sqr_r1
        //
        // (**) since we deal with only half sphere we are interested 
        //      in the part below min(c1, c2) in the case of lower envelope
        //  and in the part above max(c1, c2) in the case of upper envelope
        //
        // substituting z in the sphere equation we get the ellipse equation:
        // r*x^2 + s*y^2 + t*x*y + u*x + v*y + w = 0 where:
        //
        //         (a1-a2)^2            (b1-b2)^2       2(a1-a2)(b1-b2)
        // r = 1 + ---------    s = 1 + ---------   t = ---------------
        //         (c1-c2)^2            (c1-c2)^2          (c1-c2)^2
        //
        //            2c1(a1-a2)    m(a1-a2)
        // u = -2a1 + ---------- - ----------
        //             (c1-c2)     (c1-c2)^2
        //
        //            2c1(b1-b2)    m(b1-b2)
        // v = -2b1 + ---------- - ----------          // here we have c1 != c2
        //             (c1-c2)     (c1-c2)^2
        //
        //                                    m*c1        m^2
        // w = a1^2 + b1^2 + c1^2 - sqr_r1 - ------- + ----------
        //                                   (c1-c2)   4(c1-c2)^2

        // since we can solve only equations with integer coefficients we
        // multiply everything by 4(c1-c2)^2, and get:
        // R*x^2 + S*y^2 + T*x*y + U*x + V*y + W = 0 where:
        //    R = 4(c1-c2)^2 + 4(a1-a2)^2
        //    S = 4(c1-c2)^2 + 4(b1-b2)^2
        //    T = 8(a1-a2)(b1-b2)
        //    U = -8a1(c1-c2)^2 + 8c1(a1-a2)(c1-c2) - 4m(a1-a2)
        //    V = -8b1(c1-c2)^2 + 8c1(b1-b2)(c1-c2) - 4m(b1-b2)
        //    W = 4(a1^2 + b1^2 + c1^2 - sqr_r1)(c1-c2)^2 - 4mc1(c1-c2) + m^2
        // TODO: in the new version coefficients can be rationals
        Rational a_diff = a1 - a2;
        Rational b_diff = b1 - b2;
        Rational c_diff = c1 - c2;
        
        Rational sqr_a_diff = a_diff * a_diff;
        Rational sqr_b_diff = b_diff * b_diff;
        Rational sqr_c_diff = c_diff * c_diff;

        Rational sqr_a1 = a1*a1, sqr_a2 = a2*a2;
        Rational sqr_b1 = b1*b1, sqr_b2 = b2*b2;
        Rational sqr_c1 = c1*c1, sqr_c2 = c2*c2;
        
        Rational m = sqr_a1 + sqr_b1 + sqr_c1 -
                 sqr_a2 - sqr_b2 - sqr_c2 + sqr_r2 - sqr_r1;

        Rational R = 4*sqr_c_diff + 4*sqr_a_diff;
        Rational S = 4*sqr_c_diff + 4*sqr_b_diff;
        Rational T = 8*a_diff*b_diff;
        Rational U = -8*a1*sqr_c_diff + 8*c1*c_diff*a_diff - 4*m*a_diff;
        Rational V = -8*b1*sqr_c_diff + 8*c1*c_diff*b_diff - 4*m*b_diff;
        Rational W = 4*sqr_c_diff*(sqr_a1+sqr_b1+sqr_c1-sqr_r1) -
                 4*m*c1*c_diff + m*m;

        #ifdef CGAL_DEBUG_ENVELOPE_3_SPHERES_TRAITS
          std::cout << "full intersection ellipse: " << R << "*x^2 + " << S <<
            "*y^2 + " << T << "*xy + " << U << "*x + " << V << "*y + " << W <<
            " = 0" << std::endl;
        #endif

        // if the full spheres do not intersect, the equation we get has no
        // real solution, so we should check it:
        bool ellipse_is_point = false;  
        if (!parent.is_valid_conic_equation(R, S, T, U, V, W, 
                                            ellipse_is_point))
        {
          parent.total_timer.stop();
          parent.intersection_timer.stop();
          return o;
        }

        #ifdef CGAL_DEBUG_ENVELOPE_3_SPHERES_TRAITS
          std::cout << "ellipse is valid " 
                    << (ellipse_is_point) ? "- ellipse is a point" : ""
                    << std::endl;
        #endif
        
        // we need only a part of the ellipse (as stated in (**)) so we
        // construct the cutting line, which is:
        //    equation (*) <= min(c1,c2)    -- for lower envelope
        //    equation (*) >= max(c1,c2)    -- for upper envelope
        Rational z_plane;
        if (parent.get_envelope_type() == LOWER)
          z_plane = ((c1 < c2) ? c1 : c2);
        else
          z_plane = ((c1 > c2) ? c1 : c2);


        // we get (for lower envelope)
        //           -2(a1-a2)x -2(b1-b2)y + m
        // (*)   z = -------------------------- <= z_plane
        //                 2(c1-c2)
        // and since we need integer coefficients, and also need to be in the
        // positive side of the line (i.e., our halfplane equation should be of
        // type ax+by+c >= 0), we have:
        // sign_c_diff* [2(a1-a2)x + 2(b1-b2)y - m + 2(c1-c2)*z_plane] >= 0

        // for upper envelope, we should multiply the line equation by -1
        int envelope_coef = 1;
        if (parent.get_envelope_type() == UPPER)
          envelope_coef = -1;
        
        Sign sign_c_diff = CGAL_NTS sign(c_diff);
        Rational la = envelope_coef*2*a_diff*sign_c_diff;
        Rational lb = envelope_coef*2*b_diff*sign_c_diff;
        Rational lc = envelope_coef*sign_c_diff*(2*c_diff*z_plane - m);

      	if (ellipse_is_point)
      	{
      	  // as specified in the is_valid_conic_equation method, the
          // intersection point is:
          //
      	  Rational px = S*(4*U - T*V)/(T*T - 4*S*R);
      	  Rational py = -(T*px + V)/(2*S);
          // should check if the point is in the non-negative side of the
          // line
      	  if (CGAL_NTS sign(la*px + lb*py +lc) != NEGATIVE)
      	    *o++ = make_object(Point_2(px, py));
          parent.total_timer.stop();
          parent.intersection_timer.stop();
          return o;
      	}

        // if (a1==a2) and (b1==b2) (*) is a plane parallel to the xy-plane
        // and either all ellipse (which should be a circle) is the
        // intersection - in which case lc >= 0
        // or there is no intersection at all between the 2 half spheres -
        // in which case lc < 0
        if (CGAL_NTS compare(a_diff, 0) == EQUAL &&
            CGAL_NTS compare(b_diff, 0) == EQUAL)
        {
          Sign sign_lc = CGAL_NTS sign(lc);
          if (sign_lc != NEGATIVE)
          {
            Curve_2 res(R, S, T, U, V, W);
            *o++ = make_object(Intersection_curve(res, TRANSVERSAL));
          }
          parent.total_timer.stop();
          parent.intersection_timer.stop();
          return o;
        }


        #ifdef CGAL_DEBUG_ENVELOPE_3_SPHERES_TRAITS
          std::cout << "the cutting line: " << la << "*x + " << lb <<
                       "*y + " << lc << " = 0" << std::endl;
        #endif

        // find the intersection of the line
        //    la * x + lb * y + lc = 0
        // with the conic
        //    R*x^2 + S*y^2 + T*xy + U*x + V*y + W = 0
        Alg_point_2 source, target, pmid;
        int n_inter_points;
        if (CGAL_NTS compare(lb, 0) != EQUAL)
        {
          // Find the x-coordinates of the intersection points of the conic 
          // curve and the line y = -(la*x + lc) / lb:
          // we get a quadratic equation Ax^2 + Bx + C = 0
          // where A = lb*lb*R + la*(la*S - lb*T)
          //       B = 2*la*lc*S - lb*(lc*T + la*V - lb*U)
          //       C = S*lc*lc + lb*(lb*W - lc*V)
          Rational A = lb*lb*R + la*(la*S - lb*T),
                   B = 2*la*lc*S - lb*(lc*T + la*V - lb*U),
                   C = S*lc*lc + lb*(lb*W - lc*V);
                   
          Algebraic      inter_xs[2];
          Algebraic     *inter_xs_end;

          inter_xs_end = nt_traits.solve_quadratic_equation(A, B, C, inter_xs);
          n_inter_points = inter_xs_end - inter_xs;
                  
          #ifdef CGAL_DEBUG_ENVELOPE_3_SPHERES_TRAITS
            std::cout << "intersection of conic with line: " << n_inter_points
                      << " points" << std::endl;
          #endif
          
          if (n_inter_points > 0)
            source = Alg_point_2(inter_xs[0],
                             -(la*inter_xs[0] + lc) / Algebraic(lb));
          if (n_inter_points == 2)
          {
            target = Alg_point_2(inter_xs[1],
                             -(la*inter_xs[1] + lc) / Algebraic(lb));

            // Get the conic points whose x-coordinate are in the middle of the
            // two endpoints.
            // since inter_xs[0] and inter_xs[1] are the roots of a quadratic
            // equation Ax^2 + Bx + C = 0, their sum is -B/A which is rational
            Algebraic x_mid = Algebraic(Rational(-B/(2*A)));
            //Algebraic x_mid = (inter_xs[0] + inter_xs[1]) / 2;
            Alg_point_2 x_mid_point(x_mid, 0);
            
            int  x_mid_n_y_points;
            Alg_point_2 x_mid_y_points[2];

            Curve_2 inter_cv(R, S, T, U, V, W);

            x_mid_n_y_points = inter_cv.get_points_at_x(x_mid_point,
                                                        x_mid_y_points);
            
            CGAL_precondition(x_mid_n_y_points > 0);

            Algebraic y1 = x_mid_y_points[0].y(), y2 = x_mid_y_points[1].y();
            if (CGAL_NTS compare (
                    Algebraic(la)*x_mid + Algebraic(lb)*y1 + Algebraic(lc),
                    0) == LARGER)
            {
              pmid = Alg_point_2(x_mid, y1);
            }
            else
            {
              CGAL_assertion(CGAL_NTS compare 
                     (Algebraic(la)*x_mid + Algebraic(lb)*y2 + Algebraic(lc),
                      0) == LARGER);
              pmid = Alg_point_2(x_mid, y2);
            }
          }
        }
        else
        { // lb == 0
          CGAL_assertion(CGAL_NTS compare(la, 0) != EQUAL);

          // Find the intersection of the vertical line x = -lc / la:
          Rational inter_x = -lc/la;
          // we should solve the quadratic equation A*y^2 + B*y + C = 0
          // where A = S
          //       B = T*inter_x + V
          //       C = R*inter_x^2 + U*inter_x + W
          Rational A = S,
                   B = T*inter_x + V,
                   C = R*inter_x*inter_x + U*inter_x + W;
                   
          Algebraic  inter_points[2];
          Algebraic *inter_points_end;

          inter_points_end =
                nt_traits.solve_quadratic_equation(A, B, C, inter_points);
          n_inter_points = inter_points_end - inter_points;

          if (n_inter_points > 0)
            source = Alg_point_2(Algebraic(inter_x), inter_points[0]);
          if (n_inter_points == 2)
          {
            target = Alg_point_2(Algebraic(inter_x), inter_points[1]);
            
            // Get the conic points whose y-coordinate are in the middle of the
            // two endpoints.
            // since inter_points[0] & inter_points[1] are roots of quadratic
            // equation, their sum is -B/A, and mid_y is -B/2A
            Algebraic y_mid = Algebraic(Rational(-B/(2*A)));

            Alg_point_2 y_mid_point(0, y_mid);
            Alg_point_2 y_mid_x_points[2];
            int  y_mid_n_x_points;

            Curve_2 inter_cv(R, S, T, U, V, W);
            y_mid_n_x_points = inter_cv.get_points_at_y(y_mid_point,
                                                        y_mid_x_points);

            CGAL_precondition(y_mid_n_x_points > 0);

            Algebraic x1 = y_mid_x_points[0].x(), x2 = y_mid_x_points[1].x();
            if (CGAL_NTS compare(
                        Algebraic(la)*x1 + Algebraic(lb)*y_mid + Algebraic(lc),
                        0) == LARGER)
            {
              pmid = Alg_point_2(x1, y_mid);
            }
            else
            {
              CGAL_assertion(CGAL_NTS compare (
                        Algebraic(la)*x2 + Algebraic(lb)*y_mid + Algebraic(lc),
                        0) == LARGER);
              pmid = Alg_point_2(x2, y_mid);
            }
          }
        }

        if (n_inter_points < 2)
        {
          #ifdef CGAL_DEBUG_ENVELOPE_3_SPHERES_TRAITS
            std::cout << "projected intersections, n_inter_points = "
                      << n_inter_points << std::endl;
            std::cout << s1 << std::endl << s2 << std::endl;
          #endif
          
          // we should check whether the ellipse is in the positive side of the
          // line - in which case we return the full ellipse
          // or not - in which case there is no intersection if
          // n_inter_points = 0, and a point intersection (equal to source)
          // if n_inter_points = 1

          // for this, we find a point inside the ellipse and substitute
          // its coordinates in the line equation

          Curve_2 inter_cv(R, S, T, U, V, W);
          Alg_point_2 vtan_ps[2];
          int         n_vtan_ps;

          n_vtan_ps = inter_cv.vertical_tangency_points(vtan_ps);
          CGAL_assertion(n_vtan_ps == 2);
          Algebraic lval = Algebraic(la)*vtan_ps[0].x() +
                           Algebraic(lb)*vtan_ps[0].y() + Algebraic(lc);
          Sign lval_sign = CGAL_NTS sign(lval);
          if (lval_sign == POSITIVE)
          {
            // the full ellipse is in the positive side
            *o++ = make_object(Intersection_curve(inter_cv, TRANSVERSAL));
            parent.total_timer.stop();
            parent.intersection_timer.stop();
            return o;
          }
          else if (lval_sign == NEGATIVE)
          {
            // the full ellipse is in the negative side, except maybe the point
            // source in the case n_inter_points = 1 (which lies on the line)
            if (n_inter_points == 1)
              *o++ = make_object(Point_2(source));
            parent.total_timer.stop();
            parent.intersection_timer.stop();
            return o;
          }

          CGAL_assertion(lval_sign == ZERO);
          // in this case lval_sign lies on the line, so it must be that
          // n_inter_points == 1 and source = vtan_ps[0]
          CGAL_assertion(n_inter_points == 1 && source == vtan_ps[0]);
          // so we try the other vertical tangency point
          lval = Algebraic(la)*vtan_ps[1].x() +
                           Algebraic(lb)*vtan_ps[1].y() + Algebraic(lc);
          lval_sign = CGAL_NTS sign(lval);
          CGAL_assertion(lval_sign != ZERO);
          
          if (lval_sign == POSITIVE)
            *o++ = make_object(Intersection_curve(inter_cv, TRANSVERSAL));
          else
            *o++ = make_object(Point_2(source));

          parent.total_timer.stop();
          parent.intersection_timer.stop();
          return o;          
        }

        CGAL_assertion(n_inter_points == 2);

        // find the correct orientation of the conic between the 2 endpoints
        // it should lie on the positive side of line
        // If the mid-point forms a left-turn with the source and the target
        // points, the orientation is positive (going counterclockwise).
        // Otherwise, it is negative (going clockwise).
        static Alg_kernel k;
        typename Alg_kernel::Orientation_2 orient_f = k.orientation_2_object();
        Orientation orient;
        if (orient_f(source, pmid, target) == LEFT_TURN)
          orient = CGAL::COUNTERCLOCKWISE;
        else
          orient = CGAL::CLOCKWISE;

        Curve_2 res(R, S, T, U, V, W, orient, source, target);
        CGAL_assertion(res.is_valid());
        *o++ = make_object(Intersection_curve(res, TRANSVERSAL));  
      }
      
      parent.total_timer.stop();
      parent.intersection_timer.stop();
      return o;
    }  
  };  

  /*! Get a Construct_projected_intersections_2 functor object. */
  Construct_projected_intersections_2
  construct_projected_intersections_2_object() const
  {                                                                
    return Construct_projected_intersections_2(this);
  }
   
  class Compare_distance_to_envelope_3
  {
  protected:
    const Self& parent;
  public:
    Compare_distance_to_envelope_3(const Self* p)
      : parent(*p)
    {}

    // check which of the surfaces is closer to the envelope at the xy
    // coordinates of point (i.e. lower if computing the lower envelope, or
    // upper if computing the upper envelope)
    // precondition: the surfaces are defined in point
    Comparison_result operator()(const Point_2& p,
                                 const Xy_monotone_surface_3& s1,
                                 const Xy_monotone_surface_3& s2) const
    {
      bool use_timer = true;
      if (parent.total_timer.is_running())
        use_timer = false;

      if (use_timer)
      {
        parent.total_timer.start();
        parent.compare_on_point_timer.start();
      }

      #ifdef CGAL_DEBUG_ENVELOPE_3_SPHERES_TRAITS
        std::cout << "compare_distance_to_envelope in point: " << p <<
                     std::endl <<
                     "surfaces: "  << s1 << std::endl << s2 << std::endl;
      #endif

      parent.compare_in_point2_timer.start();
      Comparison_result c2 = compare_in_point_second_method(p, s1, s2);
      parent.compare_in_point2_timer.stop();

      CGAL_expensive_assertion_code(

        parent.compare_in_point1_timer.start();
        //CGAL_assertion_code(Comparison_result c1 = )
        Comparison_result c1 = compare_in_point_first_method(p, s1, s2);
        parent.compare_in_point1_timer.stop();


        #ifdef CGAL_DEBUG_ENVELOPE_3_SPHERES_TRAITS
          std::cout << "first method returned " << c1 << std::endl <<
                       "second method returned " << c2 << std::endl;
        #endif
      );
      CGAL_expensive_assertion(c1 == c2);
//      CGAL_assertion(c1 == c2);
      
      if (use_timer)
      {
        parent.total_timer.stop();
        parent.compare_on_point_timer.stop();
      }
      return c2;
    }

    // check which of the surfaces is closer to the envelope at the xy
    // coordinates of cv (i.e. lower if computing the lower envelope, or upper
    // if computing the upper envelope)
    // precondition: the surfaces are defined in all points of cv, and the 
    //               answer is the same for each of these points
    Comparison_result operator()(const X_monotone_curve_2& cv,
                                 const Xy_monotone_surface_3& s1,
                                 const Xy_monotone_surface_3& s2) const
    {
      parent.total_timer.start();
      parent.compare_on_cv_timer.start();
      // we compute a middle point on cv and use the previous function
      Point_2 mid = parent.construct_middle_point(cv);
      Comparison_result res = 
                   parent.compare_distance_to_envelope_3_object()(mid, s1, s2);
      parent.total_timer.stop();
      parent.compare_on_cv_timer.stop();
      return res;
    }

  protected:
    // first method of compare in point, calculates the z value of both
    // surfaces, and compares them
    Comparison_result
    compare_in_point_first_method(const Point_2& p,
                                  const Xy_monotone_surface_3& s1,
                                  const Xy_monotone_surface_3& s2) const
    {
      // find the z coordinates of surface 1 over p
      Algebraic z1 = parent.compute_envelope_z_in_point(p, s1);
      // find the z coordinates of surface 2 over p
      Algebraic z2 = parent.compute_envelope_z_in_point(p, s2);
      #ifdef CGAL_DEBUG_ENVELOPE_3_SPHERES_TRAITS
        std::cout << "z1= " << z1 << std::endl <<
                     "z2= " << z2 << std::endl;
      #endif

      Sign res = CGAL_NTS sign(z1 - z2);
      if (parent.get_envelope_type() == LOWER)
        return Comparison_result(res);
      else
        return Comparison_result(-res);
    }

    // second method of compare in point
    //    p = (x1, y1)
    //    s1: (x-a1)^2 + (y-b1)^2 + (z-c1)^2 = r1^2
    //    s2: (x-a2)^2 + (y-b2)^2 + (z-c2)^2 = r2^2
    // (both lower parts or upper parts)
    // then in point p we get:
    //    s1(p): (z-c1)^2 = r1^2 - (x1-a1)^2 - (y1-b1)^2 = A1
    //    s2(p): (z-c2)^2 = r2^2 - (x1-a2)^2 - (y1-b2)^2 = A2
    // so we get  z - ci = +- sqrt(Ai) where -sqrt(Ai) is for the lower part
    // and +sqrt(Ai) is for the upper part
    // we now need to compute the sign of:
    //    c1 - sqrt(A1) - (c2 - sqrt(A2))  - for lower envelope
    //    c1 + sqrt(A1) - (c2 + sqrt(A2))  - for upper envelope    
    Comparison_result
    compare_in_point_second_method(const Point_2& p,
                                   const Xy_monotone_surface_3& s1,
                                   const Xy_monotone_surface_3& s2) const
    {
      Rat_point_3 p1 = s1.center();
      Rat_point_3 p2 = s2.center();
      const Rational a1 = p1.x(), b1 = p1.y(), c1 = p1.z(),
                 a2 = p2.x(), b2 = p2.y(), c2 = p2.z();
      const Rational sqr_r1 = s1.squared_radius(), 
                     sqr_r2 = s2.squared_radius();
      const Algebraic x1 = p.x(), y1 = p.y();

      Rational c_diff = c1 - c2;
      Algebraic x_diff1 = x1 - a1, y_diff1 = y1 - b1;
      Algebraic x_diff2 = x1 - a2, y_diff2 = y1 - b2;
      
      Algebraic A1 = sqr_r1 - x_diff1*x_diff1 - y_diff1*y_diff1;
      Algebraic A2 = sqr_r2 - x_diff2*x_diff2 - y_diff2*y_diff2;

      if (CGAL_NTS sign(A1) == NEGATIVE)
        std::cout << "A1 = " << A1 << std::endl;
      if (CGAL_NTS sign(A2) == NEGATIVE)
        std::cout << "A2 = " << A2 << std::endl;
        
      Sign res;
      // sign_a_plus_b_x_sqrt_e_plus_c_x_sqrt_f is a CGAL method which
      // computes the sign of quantity: a + b * sqrt(e) + c * sqrt(f)
      if (parent.get_envelope_type() == LOWER)
        res = CGAL::sign_a_plus_b_x_sqrt_e_plus_c_x_sqrt_f(Algebraic(c_diff),
                                                           Algebraic(-1),
                                                           Algebraic(1),
                                                           A1,
                                                           A2);
      else
        res = CGAL::sign_a_plus_b_x_sqrt_e_plus_c_x_sqrt_f(Algebraic(c_diff),
                                                           Algebraic(1),
                                                           Algebraic(-1),
                                                           A1,
                                                           A2);
      
//      if (parent.get_envelope_type() == LOWER)
//        res = CGAL_NTS sign(c1 - CGAL::sqrt(A1) - c2 + CGAL::sqrt(A2));
//      else
//        res = CGAL_NTS sign(c1 + CGAL::sqrt(A1) - c2 - CGAL::sqrt(A2));
      return Comparison_result(res);    
    }  
  };
   
  /*! Get a Compare_distance_to_envelope_3 functor object. */
  Compare_distance_to_envelope_3
  compare_distance_to_envelope_3_object() const
  {
    return Compare_distance_to_envelope_3(this);
  }

  class Compare_distance_to_envelope_above_3
  {
  protected:
    const Self& parent;
  public:
    Compare_distance_to_envelope_above_3(const Self* p)
      : parent(*p)
    {}

    // check which of the surfaces is closer to the envelope on the points above
    // the curve cv (i.e. lower if computing the lower envelope, or upper if
    // computing the upper envelope)
    // precondition: the surfaces are defined above cv
    //               the choise between s1 and s2 for the envelope is the same
    //               for every point in the infinitesimal region above cv
    //               the surfaces are EQUAL over the curve cv
    Comparison_result
    operator()(const X_monotone_curve_2& cv,
               const Xy_monotone_surface_3& s1,
               const Xy_monotone_surface_3& s2) const
    {
      parent.total_timer.start();
      parent.compare_side_timer.start();
      #ifdef CGAL_DEBUG_ENVELOPE_3_SPHERES_TRAITS
        std::cout << "Compare_distance_to_envelope_above_3: " << std::endl <<
                  cv << std::endl << s1 << std::endl << s2 << std::endl;
      #endif
      Comparison_result res = parent.compare_on_side(cv, s1, s2, false);
      parent.total_timer.stop();
      parent.compare_side_timer.stop();
      return res;
    }  
  };

  /*! Get a Compare_distance_to_envelope_above_3 functor object. */
  Compare_distance_to_envelope_above_3
  compare_distance_to_envelope_above_3_object() const
  {
    return Compare_distance_to_envelope_above_3(this);
  }

  class Compare_distance_to_envelope_below_3
  {
  protected:
    const Self& parent;
  public:
    Compare_distance_to_envelope_below_3(const Self* p)
      : parent(*p)
    {}

    Comparison_result
    operator()(const X_monotone_curve_2& cv,
               const Xy_monotone_surface_3& s1,
               const Xy_monotone_surface_3& s2) const
    {
      parent.total_timer.start();
      parent.compare_side_timer.start();
      #ifdef CGAL_DEBUG_ENVELOPE_3_SPHERES_TRAITS
        std::cout << "Compare_distance_to_envelope_below_3: " << std::endl <<
                  cv << std::endl << s1 << std::endl << s2 << std::endl;
      #endif
      Comparison_result res = parent.compare_on_side(cv, s1, s2, true);
      parent.total_timer.stop();
      parent.compare_side_timer.stop();
      return res;      
    }  
  };

  /*! Get a Compare_distance_to_envelope_below_3 functor object. */
  Compare_distance_to_envelope_below_3
  compare_distance_to_envelope_below_3_object() const
  {
    return Compare_distance_to_envelope_below_3(this);
  }

  /***************************************************************************/

  // public method needed for testing

  // checks if point is in the xy-range of surf
  class Is_defined_over
  {
  protected:
    const Self& parent;
  public:
    Is_defined_over(const Self* p)
      : parent(*p)
    {}
    
    // checks if point is in the xy-range of surf
    bool operator()(const Point_2& p, const Xy_monotone_surface_3& s) const
    {
      // project the surface on the plane
      Rat_point_2 proj_center = parent.project(s.center());
      Rat_circle_2 boundary(proj_center, s.squared_radius());

      Nt_traits nt_traits;
      Alg_kernel k;
      Alg_point_2 aproj_center(proj_center.x(), proj_center.y());
      Alg_circle_2 aboundary(aproj_center, nt_traits.convert(s.squared_radius()));

      // check if the projected point is inside the projected boundary
      return (!k.has_on_unbounded_side_2_object()(aboundary, p));
    }
  };

  /*! Get a Is_defined_over functor object. */
  Is_defined_over is_defined_over_object() const
  {
    return Is_defined_over(this);
  }

  /***************************************************************************/

  // helper methods
    
  // compare the surfaces over the side (as specified in the compare_on_right
  // parameter) of the curve, assuming they are defined there
  Comparison_result compare_on_side(const X_monotone_curve_2& cv,
                                    const Xy_monotone_surface_3& s1,
                                    const Xy_monotone_surface_3& s2,
                                    bool compare_on_right) const
  {
    #ifdef CGAL_DEBUG_ENVELOPE_3_SPHERES_TRAITS
      std::cout << "compare_on_side: " << std::endl <<
                cv << std::endl << s1 << std::endl << s2 << std::endl;
    #endif

    // cv(x,y) : r*x^2 + s*y^2 + t*xy + u*x + v*y + w = 0
    // let p be the leftmost endpoint of cv, p=(x0, y0)
    // the tangence of cv at p is a line. on the infinitesimal region
    // near p the relation between the surfaces to the right of cv is
    // the same as the relation between the tangences of the surfaces
    // in p to the right of this line (unless the tangence of the surface
    // is vertical)

    // we take a point in the internal of the curve, not an endpoint
    // since we assume that such point represents better what is going
    // on all internal curve points
    Point_2 cv_point = construct_middle_point(cv);
    Algebraic x0 = cv_point.x(), y0 = cv_point.y();

    // d(cv)/dx : 2r*x + 2s*y*dy/dx + t*y + t*x*dy/dx +u + v*dy/dx = 0
    // in point p=(x0,y0) we get
    // dy         m
    // --  = y' = -    where m = -2rx0 -ty0 - u
    // dx         n          n = 2sy0 + tx0 + v
    // if n != 0 (if n = 0 we have a vertical line).
    //
    // So the tangence equation (in p) is:
    //    n =  0:    x = x0
    //    n != 0:    y - y0 = y'(x-x0) ==> -y'x + y + (y'x0 - y0) = 0
    // and in general we have:
    //    -m*x + n*y + (m*x0 -n*y0) = 0 (with integer coordinates)
    const Rational r = cv.r(), s = cv.s(), t = cv.t(),
               u = cv.u(), v = cv.v(), w = cv.w();
    Algebraic m = -1 * (2*r*x0 + t*y0 + u);
    Algebraic n = 2*s*y0 + t*x0 + v;
    // line coefficients: A3, B3, C3
    Algebraic A3 = -1*m, B3 = n, C3 = m*x0 - n*y0;

    // the tangences of the spheres (in point (x0,y0,z0)):
    Algebraic z0 = compute_envelope_z_in_point(cv_point, s1);
    #ifdef CGAL_DEBUG_ENVELOPE_3_SPHERES_TRAITS
      std::cout << "z0 =  " << z0 << std::endl;
    #endif

    // we assume the surfaces are equal over cv:
    CGAL_expensive_precondition_code(
      Algebraic z0_2 = compute_envelope_z_in_point(cv_point, s2);
      #ifdef CGAL_DEBUG_ENVELOPE_3_SPHERES_TRAITS
        std::cout << "z0_2 =  " << z0_2 << std::endl;
      #endif
    )
    // this test can be very time consuming ...
    CGAL_expensive_precondition(CGAL_NTS compare(z0, z0_2) == EQUAL);

    // the sphere i : fi(x,y,z) = (x-ai)^2 + (y-bi)^2 + (z-ci)^2 - ri^2 = 0
    // dfi / dx = 2(x-ai) + 2(z-ci)*dz/dx = 0
    // dfi / dy = 2(y-bi) + 2(z-ci)*dz/dy = 0
    // if z = ci the tangent plane is vertical - if only one of the tangent
    // planes is vertical, them its sphere wins (i.e. is on envelope).
    // we assume not both are tangent, since this means that they are the
    // same sphere
    // if z != ci the tangent plane is:

    //      z-z0 = dz/dx (x-x0) + dz/dy (y-y0)
    // ==>
    //      (x0-ai)(x-x0) + (y0-bi)(y-y0) + (z0-ci)(z-z0) = 0
    //      Ai*x + Bi*y + Ci*z + Di = 0
    // where  Ai = (x0-ai)
    //        Bi = (y0-bi)
    //        Ci = (z0-ci)
    //        Di = -(x0-ai)x0 - (y0-bi)y0 - (z0-ci)z0
    //
    // and we solve the problem as for triangles
    Rat_point_3 p1 = s1.center();
    Rat_point_3 p2 = s2.center();
    const Rational a1 = p1.x(), b1 = p1.y(), c1 = p1.z(),
               a2 = p2.x(), b2 = p2.y(), c2 = p2.z();
    Algebraic A1 = x0 - a1, B1 = y0 - b1, C1 = z0 - c1;
    Algebraic A2 = x0 - a2, B2 = y0 - b2, C2 = z0 - c2;
    #ifdef CGAL_DEBUG_ENVELOPE_3_SPHERES_TRAITS
      std::cout << "plane1: " << A1 << "*x + " << B1 << "*y + " << C1
                << "*z + D1 = 0" << std::endl;
      std::cout << "plane2: " << A2 << "*x + " << B2 << "*y + " << C2
                << "*z + D2 = 0" << std::endl;
    #endif
    if (C1 != 0 && C2 != 0)
    {
      Sign sign1 = CGAL_NTS sign((A2*A3+B2*B3)/C2-(A1*A3+B1*B3)/C1);
      // to make sure the direction is correct, we take a second point on the
      // line: for vertical line we take (x0, y0+1)
      //       otherwise we take (x0+1, y0+ m/n)
      Algebraic x1, y1;
      if (n == 0)
      {
        x1 = x0;
        y1 = y0+1;
      }
      else
      {
        x1 = x0+1;
        y1 = y0 + (m/n);
      }
      Sign sign2 = CGAL_NTS sign(-B3*x1+A3*y1-(-B3*x0+A3*y0));

      // the answer negates according to the side of the line we ask of
      Sign sign3 = (compare_on_right ? (CGAL_NTS sign(1)) :
                                       (CGAL_NTS sign(-1)));

      return Comparison_result(sign1 * sign2 * sign3);

    }
    else if (C1 != 0 && C2 == 0)
    {
      // sphere 2 is on the envelope (both lower & upper)
      #ifdef CGAL_DEBUG_ENVELOPE_3_SPHERES_TRAITS
        std::cout << "compare_on_side: LARGER" << std::endl;
      #endif
      return LARGER;
    }
    else if (C1 == 0 && C2 != 0)

    {
      // sphere 1 is on the envelope (both lower & upper)
      #ifdef CGAL_DEBUG_ENVELOPE_3_SPHERES_TRAITS
        std::cout << "compare_on_side: SMALLER" << std::endl;
      #endif
      return SMALLER;
    }
    else
      CGAL_assertion(false);

    return EQUAL;
  }

  Rat_point_2 project(const Rat_point_3& p) const
  {
    return Rat_point_2(p.x(), p.y());
  }

  // compute the z coordinate of the surface s in point p on the envelope
  // (i.e. take lower point if lower envelope, upper otherwise)
  // precondition: s is defined at p
  Algebraic compute_envelope_z_in_point(const Point_2& p,
                                   const Xy_monotone_surface_3& s) const
  {
    #ifdef CGAL_DEBUG_ENVELOPE_3_SPHERES_TRAITS
      std::cout << "compute z point of " << s << " in point " << p << std::endl;
    #endif

    Algebraic res;
    #ifdef CGAL_ENV_SPHERES_TRAITS_CACHE_POINT_ON
      // first try the cache:
      typename Surface_point_cache::iterator  cache_iter;
      Surface_point_pair spair(const_cast<Xy_monotone_surface_3>(s), p);

      std::cout << "before find" << std::endl;
      cache_iter = point_on_cache.find(spair);
      std::cout << "after find" << std::endl;
      if (cache_iter != point_on_cache.end())
      {
        res = (*cache_iter).second;
        cache_hits++;
        return res;
      }
    #endif
    
    // the point coordinates
    const Algebraic x1 = p.x(), y1 = p.y();

    // the surface equations
    Rat_point_3 center = s.center();
    const Rational a = center.x(), b = center.y(), c = center.z();
    const Rational sqr_r = s.squared_radius();
    
    // we substitute x1 and y1 in the equation of s
    // (x-a)^2 + (y-b)^2 + (z-c)^2 = r^2
    // and get a quadratic equation of z:
    // z^2 - 2cz + [(x1-a)^2 + (y1-b)^2 + c^2 - r^2] = 0
    Algebraic x_diff = x1 - a, y_diff = y1 - b;
    // the coefficients are:
    Algebraic A = 1,
              B = -2*c,
              C = x_diff*x_diff + y_diff*y_diff + c*c - sqr_r;

    #ifdef CGAL_DEBUG_ENVELOPE_3_SPHERES_TRAITS
      std::cout << "solve_quadratic_equation with A = " << A << " B = " << B
                << " C = " << C << std::endl;
    #endif
    
    Algebraic  zs[2];
    Algebraic *zs_end;
    int   n_zs;

    Nt_traits nt_traits;
    zs_end = nt_traits.solve_quadratic_equation(A, B, C, zs);
    n_zs = zs_end - zs;

    CGAL_precondition(n_zs > 0);

    if (n_zs == 1)
      // only one point is defined at p, this is the result
      return zs[0];
    CGAL_assertion(n_zs == 2);

    Comparison_result comp = CGAL_NTS compare(zs[0], zs[1]);
    if (get_envelope_type() == LOWER)
      res = ((comp == SMALLER) ? zs[0] : zs[1]);
    else
      res = ((comp == LARGER) ? zs[0] : zs[1]);

    #ifdef CGAL_ENV_SPHERES_TRAITS_CACHE_POINT_ON
      // update the cache
      point_on_cache[spair] = res;
    #endif

    return res;
  }

  // construct the point in the middle of cv
  Point_2 construct_middle_point(const X_monotone_curve_2& cv) const
  {
    // get the x-value of the middle point
    Alg_kernel k;
    Alg_point_2 mid_x = k.construct_midpoint_2_object()(cv.source(), 
                                                        cv.target());

// TODO_NEW_DESIGN - this is not implemented in X_monotone_curve_2, but maybe we want it there?
//    if (cv.is_segment())
//      return mid_x;
    if (cv.is_vertical())
      return Point_2(mid_x);

    return Point_2(cv.get_point_at_x(mid_x));
  }


  // for the test
  Point_2 construct_middle_point(const Point_2& p1, const Point_2& p2) const
  {
    Alg_kernel k;
    return Point_2(k.construct_midpoint_2_object()(p1, p2));
  }

  // check if the equation
  // r*x^2 + s*y^2 + t*xy + u*x + v*y + w = 0
  // has real solutions
  // is_point is set to true if the equation represents just one point
  template <class NT> 
  bool is_valid_conic_equation(const NT& r, const NT& s, const NT& t,
                               const NT& u, const NT& v, const NT& w,
                               bool &is_point) const
  {
    // initialize is_point to false, and will change it when we detect
    // that the equation represents a point
    is_point = false; 
    // (*)  r*x^2 + s*y^2 + t*xy + u*x + v*y + w = 0
    // we fix x, and get a 1-variable quadratic equation:
    // (**) s*y^2 + (tx + v)*y + (rx^2 + ux + w) = 0
    // (*) has real solution (x,y) iff there exists x such that (**) has a
    // solution y, i.e. discriminant(**) >= 0
    // discriminant(**) = f(x) = (tx + v)^2 -4s(rx^2 + ux + w) 
    //                         = (t^2 - 4sr)*x^2 + (stv - 4su)*x + (v^2 - 4sw)
    //                         = A*x^2 + B*x + C >= 0
    // where A = t^2 - 4sr
    //       B = stv - 4su
    //       C = v^2 - 4sw
    // so we should check if there exists x such that f(x) >= 0
    // if A > 0 we have a smiling parabula, and a positive answer
    // (the conic equation in this case represents hyperbola or 2 
    // intersecting lines)
    Sign sign_A = CGAL_NTS sign(t*t - 4*s*r);
    if (sign_A == POSITIVE)
      return true;
    // if A < 0 we have a sad parabula, so we should check if it crosses the
    //          x-axis, i.e. if the equation f(x) = 0 has a real solution x.
    //          this means that discriminant(f(x)) >= 0
    //          discriminant(f(x)) = B^2 - 4AC 
    //                             = (2tv-4su)^2 - 4(t^2-4sr)(v^2-4sw)
    //                             = s(-tvu + su^2 + wt^2 + rv^2 - 4srw)
    if (sign_A == NEGATIVE)
    {
      // (in this case the conic equation represents ellipse, circle, point 
      // or no curve)
      Sign sign_s = CGAL_NTS sign(s);
      Sign sign_eq = CGAL_NTS sign(-t*v*u + s*u*u + w*t*t + r*v*v - 4*s*r*w);
      // if sign_eq = 0 then discriminant(f(x))=0, and so we have only one x
      // solution for f(x), say x0. since we get f(x0)=0 and f(x)<0 forall 
      // x!=x0, we have only one solution for (**). So the equation represents
      // a point with coordinates x0=-B/2A, y0=-(tx0 + v)/2s
      if (sign_eq == ZERO)
	is_point = true;

      Sign sign_disc = CGAL_NTS sign(sign_s * sign_eq);
      return (sign_disc != NEGATIVE);
    }
    // if A = 0 we get (***) f(x) = (stv - 4su)*x + (v^2 - 4sw) = B*x + C
    //          if B != 0 we get a line equation, which always has x
    //                    such that f(x) >= 0
    //          if B = 0  then f(x) = v^2 - 4sw = C and should check its sign
    CGAL_assertion(sign_A == ZERO);
    // (in this case the conic equation represents parabola, 2 parallel lines,
    // 1 line or no curve)
    Sign sign_B = CGAL_NTS sign(s*(t*v - 4*u));
    if (sign_B != ZERO)
      return true;

    Sign sign_C = CGAL_NTS sign(v*v - 4*s*w);

    return (sign_C != NEGATIVE);
  }

  // for the test:
  Point_2 vertical_ray_shoot_2(const Point_2& pt,
                               const X_monotone_curve_2& cv) const
  {
    if (cv.is_vertical())
    {
      Alg_kernel k;
      if (!k.less_y_2_object()(cv.left(), pt))
        return cv.left();
      else
      {
        CGAL_assertion(k.less_y_2_object()(cv.right(), pt));
        return cv.right();
      }
    }
    else
      return cv.get_point_at_x(pt);
  }

  void print_times()
  {
    std::cout << total_timer.intervals()
              << " number of calls to traits interface functions" << std::endl;
    std::cout << "total time: " << total_timer.time() << " seconds"
              << std::endl << std::endl;

    std::cout << "projected intersections #: "
              << intersection_timer.intervals()
              << " total time: " << intersection_timer.time()
              << " average time: " 
              << intersection_timer.time()/intersection_timer.intervals()
              << std::endl;
    std::cout << "compare_distance on point #: "
              << compare_on_point_timer.intervals()
              << " total time: " << compare_on_point_timer.time()
              << " average time: " 
              << compare_on_point_timer.time()/compare_on_point_timer.intervals()
              << std::endl;
    std::cout << "compare_distance on curve #: "
              << compare_on_cv_timer.intervals()
              << " total time: " << compare_on_cv_timer.time()
              << " average time: " 
              << compare_on_cv_timer.time()/compare_on_cv_timer.intervals()
              << std::endl;
    std::cout << "compare_distance on side #: "
              << compare_side_timer.intervals()
              << " total time: " << compare_side_timer.time()
              << " average time: " 
              << compare_side_timer.time()/compare_side_timer.intervals()
              << std::endl;
    std::cout << std::endl;

    std::cout << "compare on point first method: " 
              << compare_in_point1_timer.time() << std::endl;
    std::cout << "compare on point second method: " 
              << compare_in_point2_timer.time() << std::endl;
    #ifdef CGAL_ENV_SPHERES_TRAITS_CACHE_POINT_ON
      std::cout << "point on cache hits: " << cache_hits << std::endl;
    #endif

  }

  Envelope_spheres_traits_3() : type(LOWER)
  {}
  Envelope_spheres_traits_3(Envelope_type t) : type(t)
  {}

  virtual ~Envelope_spheres_traits_3() {}

  Envelope_type get_envelope_type() const
  {
    return type;
  }

  void reset()
  {
    total_timer.reset();
    intersection_timer.reset();
    compare_on_point_timer.reset();
    compare_on_cv_timer.reset();
    compare_side_timer.reset();
    compare_in_point1_timer.reset();
    compare_in_point2_timer.reset();

    #ifdef CGAL_ENV_SPHERES_TRAITS_CACHE_POINT_ON
      cache_hits = 0;
    #endif
  }
protected:
  Envelope_type type;

public:
  // measure times:
  // measure the total time for all interface methods
  mutable Timer total_timer;
  // measure the total time for the intersection method
  mutable Timer intersection_timer;

  // measure the total time for the compare_distance_to_envelope method
  mutable Timer compare_on_point_timer;
  mutable Timer compare_on_cv_timer;

  // measure the total time for the compare_distance_to_envelope_side method
  mutable Timer compare_side_timer;

  // for testings
  mutable Timer compare_in_point1_timer;
  mutable Timer compare_in_point2_timer;

  #ifdef CGAL_ENV_SPHERES_TRAITS_CACHE_POINT_ON
    mutable Surface_point_cache point_on_cache;

    mutable int     cache_hits;
  #endif

};

/*!
 * \class A representation of a sphere, as used by the
 * Envelope_spheres_traits_3 traits-class.
 */
template <class Kernel_>
class Envelope_sphere_3 :
    public Handle_for<typename Kernel_::Sphere_3>
{
  typedef Kernel_                                                  Kernel;
  typedef typename Kernel::Sphere_3                                Sphere_3;
  typedef typename Kernel::Point_3                                 Point_3;
  typedef typename Kernel::FT                                      NT;
  
  typedef typename Kernel::Sphere_3                                Rep_type;

public:

  /*!
   * Default constructor.
   */
  Envelope_sphere_3() :
    Handle_for<Rep_type>(Rep_type())
  {}

  /*!
   * Constructor from a "kernel" triangle.
   * \param seg The segment.
   */
  Envelope_sphere_3(const Sphere_3& s) :
    Handle_for<Rep_type>(s)
  {}

  /*!
   * Construct a triangle from 3 end-points.
   * \param p1 The first point.
   * \param p2 The second point.
   * \param p3 The third point.
   */
  Envelope_sphere_3(const Point_3 &c, const NT &r_sqr) :
      Handle_for<Rep_type>(Rep_type(c, r_sqr))
  {}

    /*!
   * Cast to a triangle.
   */
  operator Sphere_3() const
  {
    return (Sphere_3(ptr()->center(), ptr()->squared_radius()));
  }

  Point_3 center() const
  {
    return ptr()->center();
  }

  NT squared_radius() const
  {
    return ptr()->squared_radius();
  }
  Bbox_3 bbox() const
  {
    return (ptr()->bbox());
  }

};

template <class Kernel>
bool
operator<(const Envelope_sphere_3<Kernel> &a,
          const Envelope_sphere_3<Kernel> &b)
{
  return (a.id() < b.id());
}
template <class Kernel>
bool
operator==(const Envelope_sphere_3<Kernel> &a,
           const Envelope_sphere_3<Kernel> &b)
{
  return (a.id() == b.id());
}
/*!
 * Exporter for the triangle class used by the traits-class.
 */
template <class Kernel, class OutputStream>
OutputStream& operator<< (OutputStream& os, const Envelope_sphere_3<Kernel>& s)
{
  os << static_cast<typename Kernel::Sphere_3>(s);
  return (os);
}

/*!
 * Importer for the triangle class used by the traits-class.
 */
template <class Kernel, class InputStream>
InputStream& operator>> (InputStream& is, Envelope_sphere_3<Kernel>& s)
{
  typename Kernel::Sphere_3   kernel_s;
  is >> kernel_s;
  s = kernel_s;
  return (is);
}

//template <class Kernel>
//bool
//operator<(const CGAL::Sphere_3<Kernel> &a,
//          const CGAL::Sphere_3<Kernel> &b)
//{
//  return (a.center() < b.center() ||
//          (a.center() == b.center() && a.squared_radius() < b.squared_radius()));
//}

CGAL_END_NAMESPACE

#endif // ENVELOPE_SPHERES_TRAITS_3_H
