// ======================================================================
//
// Copyright (c) 1999 The CGAL Consortium
//
// This software and related documentation is part of an INTERNAL release
// of the Computational Geometry Algorithms Library (CGAL). It is not
// intended for general use.
//
// ----------------------------------------------------------------------
// release       : 
// release_date  : 
//
// file          : include/CGAL/predicate_objects_on_points_2.h
// package       : Kernel_basic
// revision      : $Revision$
// revision_date : $Date$ 
// author(s)     : Stefan Schirra
//
// coordinator   : MPI, Saarbruecken
// ======================================================================

#ifndef CGAL_PREDICATE_OBJECTS_ON_POINTS_2_H
#define CGAL_PREDICATE_OBJECTS_ON_POINTS_2_H

CGAL_BEGIN_NAMESPACE

template <class Point>
struct p_Less_xy
{
  typedef bool    result_type;
  typedef  Arity_tag< 2 >   Arity;

  bool operator()( const Point& p1, const Point& p2) const
       { return lexicographically_xy_smaller( p1, p2); }
};

template <class Point>
struct p_Less_yx
{
  typedef bool    result_type;
  typedef  Arity_tag< 2 >   Arity;

  bool operator()( const Point& p1, const Point& p2) const
       { return lexicographically_yx_smaller( p1, p2); }
};

template <class Point>
class p_Less_dist_to_line_2p
{
public:
  typedef bool    result_type;
  typedef  Arity_tag< 2 >   Arity;

        p_Less_dist_to_line_2p(const Point& a, const Point& b)
         : p_a(a), p_b(b)
        {}

  bool  operator()(const Point& c, const Point& d) const
        {
          Comparison_result 
            res = compare_signed_distance_to_line( p_a, p_b, c, d);
          if ( res == LARGER )
          {
              return false;
          }
          else if ( res == SMALLER )
          {
              return true;
          }
          else
          {
              return lexicographically_xy_smaller( c, d );
          }
        }

private:
  Point           p_a;
  Point           p_b;
};

template <class Point>
class p_Less_dist_to_line_2
{
public:
  typedef bool    result_type;
  typedef  Arity_tag< 4 >   Arity;

  bool  operator()(const Point&a, const Point& b,
                   const Point& c, const Point& d) const
        {
          Comparison_result 
            res = compare_signed_distance_to_line( a, b, c, d);
          if ( res == LARGER )
          {
              return false;
          }
          else if ( res == SMALLER )
          {
              return true;
          }
          else
          {
              return lexicographically_xy_smaller( c, d );
          }
        }

};

template <class Point>
class p_Less_rotate_ccw
{
public:
  typedef bool    result_type;
  typedef  Arity_tag< 3 >   Arity;

  bool  operator()(const Point& r, const Point& p, const Point& q) const
        {
          Orientation ori = orientation(r, p, q);
          if ( ori == LEFTTURN )
          {
              return true;
          }
          else if ( ori == RIGHTTURN )
          {
              return false;
          }
          else
          {
              if (p == r) return false;
              if (q == r) return true;
              if (p == q)         return false;
              return  collinear_are_ordered_along_line( r, q, p);
          }
        }
};

template <class R>
class r_Less_dist_to_line
{
public:
  typedef bool    result_type;
  typedef  Arity_tag< 4 >   Arity;

  typedef typename R::Point_2  Point;
  typedef typename R::Line_2   Line;

        r_Less_dist_to_line() : line_constructed( false )
        { }
       
  bool  operator()(const Point& a, const Point& b,
                   const Point& c, const Point& d) const
        {
          if (!line_constructed)
          {
             line_constructed = true;
             l_ab = Line(a,b);
          }
          Comparison_result res = compare_signed_distance_to_line(l_ab, c, d);
          if ( res == SMALLER )
          {
              return true;
          }
          else if ( res == EQUAL )
          {
              return lexicographically_xy_smaller( c, d );
          }
          else
          {
              return false;
          }
        }

private:
  mutable bool line_constructed;
  mutable Line l_ab;
};

template <class Point>
struct p_Left_turn
{
  typedef bool    result_type;
  typedef  Arity_tag< 3 >   Arity;

  bool  operator()(const Point& p, const Point& q, const Point& r) const
        { return left_turn(p,q,r); }
};

CGAL_END_NAMESPACE

#endif // CGAL_PREDICATE_OBJECTS_ON_POINTS_2_H
