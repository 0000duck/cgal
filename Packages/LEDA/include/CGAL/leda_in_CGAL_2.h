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
// file          : include/CGAL/leda_in_CGAL_2.h
// package       : LEDA (1.0)
// maintainer    : Susan Hert <hert@mpi-sb.mpg.de>
// revision      : 1.0
// revision_date : 19 March 2002
// author(s)     : Stefan Schirra
//
// coordinator   : MPI, Saarbruecken
// ======================================================================


#ifndef LEDA_IN_CGAL_H
#define LEDA_IN_CGAL_H

#include <CGAL/basic.h>
#include <CGAL/enum.h>
#include <CGAL/LEDA_basic.h>
#include <LEDA/point.h>
#include <LEDA/segment.h>
#include <LEDA/line.h>

CGAL_BEGIN_NAMESPACE
inline
bool
left_turn( const leda_point & p, 
          const leda_point & q, 
          const leda_point & r)
{ return  CGAL_LEDA_SCOPE::left_turn(p,q,r); }

inline
bool
right_turn( const leda_point & p, 
           const leda_point & q, 
           const leda_point & r)
{ return  CGAL_LEDA_SCOPE::right_turn(p,q,r); }

#ifndef CGAL_CFG_NO_NAMESPACE
inline
Orientation
orientation( const leda_point & p, 
             const leda_point & q, 
             const leda_point & r)
{ return (Orientation)CGAL_LEDA_SCOPE::orientation(p,q,r); }
#endif // CGAL_CFG_NO_NAMESPACE

inline
bool
lexicographically_xy_smaller( const leda_point & p, 
                              const leda_point & q)
{ return ( leda_point::cmp_xy(p,q)  <  0 ); }

inline
bool
lexicographically_yx_smaller( const leda_point & p, 
                              const leda_point & q)
{ return ( leda_point::cmp_yx(p,q)  <  0 ); }

inline
bool
lexicographically_xy_larger( const leda_point & p, 
                             const leda_point & q)
{ return ( leda_point::cmp_xy(p,q)  >  0 ); }

inline
bool
lexicographically_yx_larger( const leda_point & p, 
                             const leda_point & q)
{ return ( leda_point::cmp_yx(p,q)  >  0 ); }

inline
bool
collinear_are_ordered_along_line( const leda_point & p, 
                                  const leda_point & q, 
                                  const leda_point & r)
{ 
  return 
  (  (( leda_point::cmp_xy(p,q)<=0 )&&( leda_point::cmp_xy(q,r)<=0 )) 
   ||(( leda_point::cmp_xy(r,q)<=0 )&&( leda_point::cmp_xy(q,p)<=0 )) );
}

inline
Comparison_result 
cmp_signed_dist_to_line( const leda_point & p, const leda_point & q,
                         const leda_point & r, const leda_point & s )
{
#if ( __LEDA__ >= 360 )
  return (Comparison_result)::cmp_signed_dist(p,q,r,s);
#else
  leda_line  l(p,q);
  int  r_or = CGAL_LEDA_SCOPE::orientation( l, r );
  int  s_or = CGAL_LEDA_SCOPE::orientation( l, s );
  if ( r_or != s_or )
  {
      return (Comparison_result)( r_or < s_or );
  }
  else
  {
     return 
      (Comparison_result)(r_or *( CGAL::sign(l.sqr_dist(r) - l.sqr_dist(s) )));
  }
#endif  // __LEDA__ >= 360
}
CGAL_END_NAMESPACE

#endif // LEDA_IN_CGAL_H
