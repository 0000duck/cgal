// ======================================================================
//
// Copyright (c) 2001 The CGAL Consortium
//
// This software and related documentation is part of an INTERNAL release
// of the Computational Geometry Algorithms Library (CGAL). It is not
// intended for general use.
//
// ----------------------------------------------------------------------
// release       :
// release_date  : 
//
// file          : include/CGAL/Convex_hull_projective_xz_traits_2.h
// package       : Convex_hull_2 
// revision      : $Revision$
// revision_date : $Date#
// author(s)     : Stefan Schirra
//
// coordinator   : MPI, Saarbruecken
// ======================================================================

#ifndef CONVEX_HULL_PROJECTIVE_XZ_TRAITS_2_H
#define CONVEX_HULL_PROJECTIVE_XZ_TRAITS_2_H

#include <CGAL/predicates/kernel_ftC2.h>
#include <CGAL/predicates_on_points_2.h>
#include <CGAL/function_objects.h>

namespace CGAL {

template <class Point_3>
class Less_xy_plane_xz_2 
{
public:
   bool 
   operator()(const Point_3& p, const Point_3& q) const
   { 
   return compare_lexicographically_xyC2(p.x(), p.z(), q.x(), q.z()) == SMALLER;
   }
};

template <class Point_3>
class Less_yx_plane_xz_2 
{
public:
   bool 
   operator()(const Point_3& p, const Point_3& q) const
   { 
   return compare_lexicographically_xyC2(p.z(), p.x(), q.z(), q.x()) == SMALLER;
   }
};

template <class Point_3>
class Leftturn_plane_xz_2 
{
public:
   bool 
   operator()(const Point_3& p, const Point_3& q, const Point_3& r) const
   { 
    return orientationC2(p.x(), p.z(), q.x(), q.z(), r.x(), r.z()) == LEFTTURN;
   }
};

template <class Point_3>
class Left_of_line_plane_xz_2
{
public:
   Left_of_line_plane_xz_2(const Point_3& a, const Point_3& b):
      p_a(a), p_b(b) 
   { }

   bool 
   operator()(const Point_3& c) const
   {
      return orientationC2(p_a.x(), p_a.z(), p_b.x(), p_b.z(), c.x(), c.z()) ==
             LEFTTURN; 
   }
private:
   Point_3 p_a;
   Point_3 p_b;
};

template <class Point_3>
class Less_dist_to_line_plane_xz_2
{
public:
   Less_dist_to_line_plane_xz_2(const Point_3& a, const Point_3& b):
      p(a), q(b)
   { }

   bool
   operator()(const Point_3& r, const Point_3& s) const
   {
      Comparison_result
         res = cmp_signed_dist_to_lineC2(p.x(), p.z(), q.x(), q.z(),
                                         r.x(), r.z(), s.x(), s.z());
      if ( res == LARGER )
         return false;
      else if ( res == SMALLER )
         return true;
      else
         return compare_lexicographically_xyC2(r.x(), r.z(), s.x(), s.z())
             == SMALLER;
   }
private:
   Point_3 p;
   Point_3 q;
};


template <class Point_3>
class Less_rotate_ccw_plane_xz_2
{
public:
   Less_rotate_ccw_plane_xz_2(const Point_3& p):
      rot_point(p)
   { }

   bool
   operator()(const Point_3& p, const Point_3& q) const
   {
      Orientation orient =
               orientationC2(rot_point.x(), rot_point.z(), p.x(), p.z(),
                             q.x(), q.z());
      if ( orient ==  LEFTTURN )
         return true;
      else if ( orient == RIGHTTURN )
         return false;
      else
      {
         if (p.x() == rot_point.x() && p.z() == rot_point.z()) return false;
         if (q.x() == rot_point.x() && q.z() == rot_point.z()) return true;
         if (p.x() == q.x() && p.z() == q.z()) return false;
         return
            collinear_are_ordered_along_lineC2(rot_point.x(), rot_point.z(),
                                               q.x(), q.z(), p.x(), p.z());
      }
   }

private:
   Point_3 rot_point;
};



template <class Point_3>
class Convex_hull_projective_xz_traits_2 
{
public:
    typedef Point_3                             Point_2;
    typedef Less_xy_plane_xz_2<Point_3>         Less_xy_2;
    typedef Less_yx_plane_xz_2<Point_3>         Less_yx_2;
    typedef Leftturn_plane_xz_2<Point_3>        Leftturn_2;
    typedef Left_of_line_plane_xz_2<Point_3>    Left_of_line_2;
    typedef Less_rotate_ccw_plane_xz_2<Point_3> Less_rotate_ccw_2;
    typedef Less_dist_to_line_plane_xz_2<Point_3> 
                                                Less_signed_distance_to_line_2;

    Left_of_line_2
    left_of_line_2_object(Point_2 p, Point_2 q) const
    {  return Left_of_line_2(p, q); }

    Less_xy_2
    less_xy_2_object() const
    {  return Less_xy_2(); }

    Less_yx_2
    less_yx_2_object() const
    {  return Less_yx_2(); }

    Leftturn_2
    leftturn_2_object() const
    {  return Leftturn_2(); }

    Less_rotate_ccw_2
    less_rotate_ccw_2_object() const
    {  return Less_rotate_ccw_2(); }

    Less_signed_distance_to_line_2
    less_signed_distance_to_line_2_object() const
    {  return Less_signed_distance_to_line_2(); }

};

}
#endif // CONVEX_HULL_PROJECTIVE_XZ_TRAITS_2_H
