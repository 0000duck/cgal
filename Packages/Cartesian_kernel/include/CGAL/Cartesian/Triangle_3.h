// ======================================================================
//
// Copyright (c) 2000 The CGAL Consortium
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
// file          : include/CGAL/Cartesian/Triangle_3.h
// revision      : $Revision$
// revision_date : $Date$
// author(s)     : Andreas Fabri
// coordinator   : INRIA Sophia-Antipolis (Mariette.Yvinec@sophia.inria.fr)
//
// ======================================================================

#ifndef CGAL_CARTESIAN_TRIANGLE_3_H
#define CGAL_CARTESIAN_TRIANGLE_3_H

#include <CGAL/Cartesian/redefine_names_3.h>
#include <CGAL/Threetuple.h>

CGAL_BEGIN_NAMESPACE

template <class R_>
class TriangleC3 CGAL_ADVANCED_KERNEL_PARTIAL_SPEC
  : public Handle_for< Threetuple< typename R_::Point_3 > >
{
public:
  typedef R_                               R;
  typedef typename R::FT                   FT;
  typedef typename R::RT                   RT;
#ifndef CGAL_CFG_NO_ADVANCED_KERNEL
  typedef TriangleC3<R CGAL_CTAG>          Self;
  typedef typename R::Point_3              Point_3;
  typedef typename R::Vector_3             Vector_3;
  typedef typename R::Plane_3              Plane_3;
  typedef typename R::Aff_transformation_3 Aff_transformation_3;
#else
  typedef TriangleC3<R>                         Self;
  typedef typename R::Point_3_base              Point_3;
  typedef typename R::Vector_3_base             Vector_3;
  typedef typename R::Plane_3_base              Plane_3;
  typedef typename R::Aff_transformation_3_base Aff_transformation_3;
#endif

  TriangleC3();
  TriangleC3(const Self &t);
  TriangleC3(const Point_3 &p, const Point_3 &q, const Point_3 &r);
  ~TriangleC3() {}

  bool       operator==(const Self &t) const;
  bool       operator!=(const Self &t) const;

  Plane_3    supporting_plane() const;

  Self       transform(const Aff_transformation_3 &t) const;

  bool       has_on(const Point_3 &p) const;
  bool       is_degenerate() const;

  Point_3    vertex(int i) const;
  Point_3    operator[](int i) const;

  Bbox_3     bbox() const;

};

template < class R >
CGAL_KERNEL_CTOR_INLINE
TriangleC3<R CGAL_CTAG>::TriangleC3()
{
  new ( static_cast< void*>(ptr)) Threetuple<Point_3>();
}

template < class R >
CGAL_KERNEL_CTOR_INLINE
TriangleC3<R CGAL_CTAG>::
TriangleC3(const TriangleC3<R CGAL_CTAG> &t)
  : Handle_for<Threetuple< typename R::Point_3> >(t)
{}

template < class R >
CGAL_KERNEL_CTOR_INLINE
TriangleC3<R CGAL_CTAG>::
TriangleC3(const typename TriangleC3<R CGAL_CTAG>::Point_3 &p,
           const typename TriangleC3<R CGAL_CTAG>::Point_3 &q,
           const typename TriangleC3<R CGAL_CTAG>::Point_3 &r)
{
  new ( static_cast< void*>(ptr)) Threetuple<Point_3>(p, q, r);
}


template < class R >
bool
TriangleC3<R CGAL_CTAG>::operator==(const TriangleC3<R CGAL_CTAG> &t) const
{
  if ( identical(t) ) return true;
  int i;
  for(i=0; i<3; i++)
    if ( vertex(0) == t.vertex(i) )
       break;

  return (i<3) && vertex(1) == t.vertex(i+1) && vertex(2) == t.vertex(i+2);
}

template < class R >
inline
bool
TriangleC3<R CGAL_CTAG>::operator!=(const TriangleC3<R CGAL_CTAG> &t) const
{
  return !(*this == t);
}

template < class R >
typename TriangleC3<R CGAL_CTAG>::Point_3
TriangleC3<R CGAL_CTAG>::vertex(int i) const
{
  if (i<0) i=(i%3)+3;
  else if (i>3) i=i%3;
  return (i==0) ? ptr->e0 :
         (i==1) ? ptr->e1 :
                  ptr->e2;
}

template < class R >
inline
typename TriangleC3<R CGAL_CTAG>::Point_3
TriangleC3<R CGAL_CTAG>::operator[](int i) const
{
  return vertex(i);
}

template < class R >
inline
typename TriangleC3<R CGAL_CTAG>::Plane_3
TriangleC3<R CGAL_CTAG>::supporting_plane() const
{
  return Plane_3(vertex(0), vertex(1), vertex(2));
}

template < class R >
Bbox_3
TriangleC3<R CGAL_CTAG>::bbox() const
{
  return vertex(0).bbox() + vertex(1).bbox() + vertex(2).bbox();
}

template < class R >
inline
TriangleC3<R CGAL_CTAG>
TriangleC3<R CGAL_CTAG>::
transform
  (const typename TriangleC3<R CGAL_CTAG>::Aff_transformation_3 &t) const
{
  return TriangleC3<R CGAL_CTAG>(t.transform(vertex(0)),
                        t.transform(vertex(1)),
                        t.transform(vertex(2)));
}

template < class R >
bool
TriangleC3<R CGAL_CTAG>::
has_on(const typename TriangleC3<R CGAL_CTAG>::Point_3 &p) const
{
  Point_3  o  = vertex(0) + supporting_plane().orthogonal_vector();
  Vector_3 v0 = vertex(0)-o,
           v1 = vertex(1)-o,
           v2 = vertex(2)-o;

  FT alpha, beta, gamma;
  solve(v0, v1, v2, p-o, alpha, beta, gamma);
  return (alpha >= FT(0)) && (beta >= FT(0)) && (gamma >= FT(0))
      && ((alpha+beta+gamma == FT(1)));
}

template < class R >
bool
TriangleC3<R CGAL_CTAG>::is_degenerate() const
{
  return collinear(vertex(0),vertex(1),vertex(2));
}

#ifndef CGAL_NO_OSTREAM_INSERT_TRIANGLEC3
template < class R >
std::ostream &
operator<<(std::ostream &os, const TriangleC3<R CGAL_CTAG> &t)
{
    switch(os.iword(IO::mode)) {
    case IO::ASCII :
        return os << t[0] << ' ' << t[1] << ' ' << t[2];
    case IO::BINARY :
        return os << t[0]  << t[1]  << t[2];
    default:
        os << "TriangleC3(" << t[0] <<  ", " << t[1] <<   ", " << t[2] <<")";
        return os;
    }
}
#endif // CGAL_NO_OSTREAM_INSERT_TRIANGLEC3

#ifndef CGAL_NO_ISTREAM_EXTRACT_TRIANGLEC3
template < class R >
std::istream &
operator>>(std::istream &is, TriangleC3<R CGAL_CTAG> &t)
{
    typename TriangleC3<R CGAL_CTAG>::Point_3 p, q, r;

    is >> p >> q >> r;

    t = TriangleC3<R CGAL_CTAG>(p, q, r);
    return is;
}
#endif // CGAL_NO_ISTREAM_EXTRACT_TRIANGLEC3

CGAL_END_NAMESPACE

#endif // CGAL_CARTESIAN_TRIANGLE_3_H
