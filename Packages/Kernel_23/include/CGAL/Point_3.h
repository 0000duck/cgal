// ======================================================================
//
// Copyright (c) 1999 The CGAL Consortium
//
// This software and related documentation is part of an INTERNAL release
// of the Computational Geometry Algorithms Library (CGAL). It is not
// intended for general use.
//
// ----------------------------------------------------------------------
// 
// release       : 
// release_date  : 
// 
// file          : Point_3.h
// package       : _3
// revision      : $Revision$
// revision_date : $Date$
// author(s)     : Andreas Fabri
//                 Stefan Schirra
//
// coordinator   : MPI, Saarbruecken  (<Stefan.Schirra@mpi-sb.mpg.de>)
// ======================================================================
 

#ifndef CGAL_POINT_3_H
#define CGAL_POINT_3_H

#ifndef CGAL_REP_CLASS_DEFINED
#error  no representation class defined
#endif  // CGAL_REP_CLASS_DEFINED

#if defined CGAL_HOMOGENEOUS_H || defined CGAL_SIMPLE_HOMOGENEOUS_H
#include <CGAL/PointH3.h>
#endif

#if defined CGAL_CARTESIAN_H || defined CGAL_SIMPLE_CARTESIAN_H
#include <CGAL/Cartesian/Point_3.h>
#endif

#include <CGAL/point_vector_declarations_3.h>

CGAL_BEGIN_NAMESPACE

template <class R_>
class Point_3 : public R_::Point_3_base
{
public:
  typedef          R_                       R;
  typedef typename R::RT                    RT;
  typedef typename R::FT                    FT;
  typedef typename R::Point_3_base  RPoint_3;
  typedef typename R::Vector_3_base  RVector_3;

friend  CGAL_FRIEND_INLINE
        CGAL::Point_3<R>
        vector_to_point_conversion CGAL_NULL_TMPL_ARGS
                                            (const CGAL::Vector_3<R>& v);

public:
  Point_3()
  {}
  Point_3(const Origin& o) : RPoint_3(o)
  {}
  Point_3(const CGAL::Point_3<R>& p)
      : RPoint_3( static_cast<const RPoint_3&>(p) )
  {}
  Point_3(const RPoint_3&  p) : RPoint_3(p)
  {}
  Point_3(const RT& hx, const RT& hy, const RT& hz)
    : RPoint_3(hx, hy, hz)
  {}
  Point_3(const RT& hx, const RT& hy, const RT& hz, const RT& hw)
    : RPoint_3(hx, hy, hz, hw)
  {}

  bool operator==(const CGAL::Point_3<R>& p) const
  { return RPoint_3::operator==(p); }

  bool operator!=(const CGAL::Point_3<R>& p) const
  { return !(*this == p); }


  RT hx() const
  { return RPoint_3::hx(); }

  RT hy() const
  { return RPoint_3::hy(); }

  RT hz() const
  { return RPoint_3::hz(); }

  RT hw() const
  { return RPoint_3::hw(); }

  FT x() const
  { return RPoint_3::x(); }

  FT y() const
  { return RPoint_3::y(); }

  FT z() const
  { return RPoint_3::z(); }

  RT homogeneous(int i) const
  { return RPoint_3::homogeneous(i); }

  FT cartesian(int i) const
  { return RPoint_3::cartesian(i); }

  FT operator[](int i) const
  { return cartesian(i); }

  int dimension() const
  { return 3; }

  Bbox_3       bbox() const
  { return RPoint_3::bbox(); }

  CGAL::Point_3<R> transform(const CGAL::Aff_transformation_3<R>& t) const
  { return RPoint_3::transform(t); }

private:
  Point_3(const RVector_3&  v) : RPoint_3(v)
  {}
};

CGAL_END_NAMESPACE

#include <CGAL/Vector_3.h>
#include <CGAL/point_vector_definitions_3.C>
#include <CGAL/Aff_transformation_3.h>

CGAL_BEGIN_NAMESPACE

template <class R>
inline
bool
operator==(const Origin& o, const Point_3<R>& p)
{ return p == o; }

template <class R>
inline
bool
operator!=(const Origin& o, const Point_3<R>& p)
{ return p != o; }


#ifndef CGAL_NO_OSTREAM_INSERT_POINT_3

template < class R >
std::ostream&
operator<<(std::ostream& os, const Point_3<R>& p)
{
  typedef typename  R::Point_3_base  RPoint_3;
  return os << static_cast<const RPoint_3&>(p);
}
#endif // CGAL_NO_OSTREAM_INSERT_POINT_3

#ifndef CGAL_NO_ISTREAM_EXTRACT_POINT_3
template < class R >
std::istream& operator>>(std::istream& is, Point_3<R>& p)
{
  typedef typename  R::Point_3_base  RPoint_3;
  return is >> static_cast<RPoint_3&>(p);
}
#endif // CGAL_NO_ISTREAM_EXTRACT_POINT_3

CGAL_END_NAMESPACE

#endif // CGAL_POINT_3_H
