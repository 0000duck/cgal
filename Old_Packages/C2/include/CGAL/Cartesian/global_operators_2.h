// ============================================================================
//
// Copyright (c) 1999 The CGAL Consortium
//
// This software and related documentation is part of an INTERNAL release
// of the Computational Geometry Algorithms Library (CGAL). It is not
// intended for general use.
//
// ----------------------------------------------------------------------------
//
// release       :
// release_date  :
//
// file          : include/CGAL/Cartesian/global_operators_2.h
// source        : include/CGAL/Cartesian/global_operators_2.h
// package       : C2
// revision      : $Revision$
// revision_date : $Date$
// author(s)     : Herve Bronnimann
//
// coordinator   : INRIA Sophia-Antipolis (Herve.Bronnimann@sophia.inria.fr)
//
// ============================================================================

#ifndef CGAL_CARTESIAN_GLOBAL_OPERATORS_2_C
#define CGAL_CARTESIAN_GLOBAL_OPERATORS_2_C

#include <CGAL/Cartesian/redefine_names_2.h>

CGAL_BEGIN_NAMESPACE

template < class R >
inline
PointC2<R CGAL_CTAG>
operator+(const PointC2<R CGAL_CTAG> &p, const VectorC2<R CGAL_CTAG> &v)
{
  return PointC2<R CGAL_CTAG>(p.x() + v.x(), p.y() + v.y()) ;
}

template < class R >
inline
PointC2<R CGAL_CTAG>
operator-(const PointC2<R CGAL_CTAG> &p, const VectorC2<R CGAL_CTAG> &v)
{
  return PointC2<R CGAL_CTAG>(p.x() - v.x(), p.y() - v.y()) ;
}

template < class R >
inline
PointC2<R CGAL_CTAG>
operator+(const Origin &, const VectorC2<R CGAL_CTAG> &v)
{
  return PointC2<R CGAL_CTAG>(v);
}

template < class R >
inline
PointC2<R CGAL_CTAG>
operator-(const Origin &, const VectorC2<R CGAL_CTAG> &v)
{
  return PointC2<R CGAL_CTAG>(-v);
}

template < class R >
inline
VectorC2<R CGAL_CTAG>
operator-(const PointC2<R CGAL_CTAG> &p, const PointC2<R CGAL_CTAG> &q)
{
  return VectorC2<R CGAL_CTAG>(p.x() - q.x(), p.y() - q.y()) ;
}

template < class R >
inline
VectorC2<R CGAL_CTAG>
operator-(const PointC2<R CGAL_CTAG> &p, const Origin &)
{
  return VectorC2<R CGAL_CTAG>(p) ;
}

template < class R >
inline
VectorC2<R CGAL_CTAG>
operator-(const Origin &, const PointC2<R CGAL_CTAG> &p)
{
  return VectorC2<R CGAL_CTAG>(-p.x(), -p.y()) ;
}

template < class R >
CGAL_KERNEL_INLINE
VectorC2<R CGAL_CTAG>
operator*(const typename R::FT &c, const VectorC2<R CGAL_CTAG> &w)
{
   return VectorC2<R CGAL_CTAG>( c* w.x(), c * w.y()) ;
}

template < class R >
CGAL_KERNEL_INLINE
VectorC2<R CGAL_CTAG>
operator*(const VectorC2<R CGAL_CTAG> &w, const typename R::FT &c)
{
   return VectorC2<R CGAL_CTAG>( c* w.x(), c * w.y()) ;
}

CGAL_END_NAMESPACE

#endif // CGAL_CARTESIAN_GLOBAL_OPERATORS_2_C
