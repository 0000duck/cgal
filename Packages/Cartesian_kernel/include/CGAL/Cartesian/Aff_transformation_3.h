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
// file          : include/CGAL/Cartesian/Aff_transformation_3.h
// revision      : $Revision$
// revision_date : $Date$
// author(s)     : Andreas Fabri
// coordinator   : INRIA Sophia-Antipolis (Mariette.Yvinec@sophia.inria.fr)
//
// ======================================================================

#ifndef CGAL_CARTESIAN_AFF_TRANSFORMATION_3_H
#define CGAL_CARTESIAN_AFF_TRANSFORMATION_3_H

#include <cmath>

CGAL_BEGIN_NAMESPACE

class Identity_transformation;
template <class R> class Aff_transformation_rep_baseC3;
template <class R> class Aff_transformation_repC3;
template <class R> class Translation_repC3;
template <class R> class Scaling_repC3;

CGAL_END_NAMESPACE

#include <CGAL/Cartesian/Aff_transformation_rep_3.h>
#include <CGAL/Cartesian/Translation_rep_3.h>
#include <CGAL/Cartesian/Scaling_rep_3.h>

CGAL_BEGIN_NAMESPACE

template < class R_ >
class Aff_transformationC3 CGAL_ADVANCED_KERNEL_PARTIAL_SPEC
  : public R_::Aff_transformation_handle_3
{
#ifdef CGAL_CFG_NO_ADVANCED_KERNEL
  friend class PlaneC3<R_ CGAL_CTAG>; // FIXME: why ?
#endif

public:
  typedef R_                               R;
  typedef typename R::FT                   FT;
  typedef Aff_transformation_rep_baseC3<R> Aff_t_base;

  typedef typename R::Kernel_base::Point_3              Point_3;
  typedef typename R::Kernel_base::Vector_3             Vector_3;
  typedef typename R::Kernel_base::Direction_3          Direction_3;
  typedef typename R::Kernel_base::Plane_3              Plane_3;

  Aff_transformationC3()
  {
    FT ft1(1), ft0(0);
    initialize_with(Aff_transformation_repC3<R>(ft1, ft0, ft0,
                                          ft0, ft1, ft0,
                                          ft0, ft0, ft1));
  }

  Aff_transformationC3(const Identity_transformation)
  {
    FT ft1(1), ft0(0);
    initialize_with(Aff_transformation_repC3<R>(ft1, ft0, ft0,
                                          ft0, ft1, ft0,
                                          ft0, ft0, ft1));
  }

  Aff_transformationC3(const Translation, const Vector_3 &v)
  {
    initialize_with(Translation_repC3<R>(v));
  }

  Aff_transformationC3(const Scaling, const FT &s, const FT &w = FT(1))
  {
    if (w != FT(1))
      initialize_with(Scaling_repC3<R>(s/w));
    else
      initialize_with(Scaling_repC3<R>(s));
  }

  // General form: without translation
  Aff_transformationC3(const FT& m11, const FT& m12, const FT& m13,
                       const FT& m21, const FT& m22, const FT& m23,
                       const FT& m31, const FT& m32, const FT& m33,
                       const FT& w = FT(1))
  {
    if (w != FT(1))
      initialize_with(Aff_transformation_repC3<R>(m11/w, m12/w, m13/w,
                                            m21/w, m22/w, m23/w,
                                            m31/w, m32/w, m33/w));
    else
      initialize_with(Aff_transformation_repC3<R>(m11, m12, m13,
                                            m21, m22, m23,
                                            m31, m32, m33));
  }

  // General form: with translation
  Aff_transformationC3(
              const FT& m11, const FT& m12, const FT& m13, const FT& m14,
              const FT& m21, const FT& m22, const FT& m23, const FT& m24,
              const FT& m31, const FT& m32, const FT& m33, const FT& m34,
              const FT& w = FT(1))
  {
    if (w != FT(1))
      initialize_with(Aff_transformation_repC3<R>(m11/w, m12/w, m13/w, m14/w,
                                            m21/w, m22/w, m23/w, m24/w,
                                            m31/w, m32/w, m33/w, m34/w));
    else
      initialize_with(Aff_transformation_repC3<R>(m11, m12, m13, m14,
                                            m21, m22, m23, m24,
                                            m31, m32, m33, m34));
  }

  Point_3
  transform(const Point_3 &p) const
  { return Ptr()->transform(p); }

  Point_3
  operator()(const Point_3 &p) const
  { return transform(p); }

  Vector_3
  transform(const Vector_3 &v) const
  { return Ptr()->transform(v); }

  Vector_3
  operator()(const Vector_3 &v) const
  { return transform(v); }

  Direction_3
  transform(const Direction_3 &d) const
  { return Ptr()->transform(d); }

  Direction_3
  operator()(const Direction_3 &d) const
  { return transform(d); }

  Plane_3
  transform(const Plane_3& p) const
  { return p.transform(*this); }

  Plane_3
  operator()(const Plane_3& p) const
  { return transform(p); } // FIXME : not compiled by the test-suite !

  Aff_transformationC3 inverse() const { return Ptr()->inverse(); }
  
  bool is_even() const { return Ptr()->is_even(); }
  bool is_odd() const { return  ! (Ptr()->is_even()); }
  
  FT cartesian(int i, int j) const { return Ptr()->cartesian(i,j); }
  FT homogeneous(int i, int j) const { return cartesian(i,j); }
  FT m(int i, int j) const { return cartesian(i,j); }
  FT hm(int i, int j) const { return cartesian(i,j); }

  Aff_transformationC3 operator*(const Aff_transformationC3 &t) const
  { return (*Ptr()) * (*t.Ptr()); }

protected:
  Aff_transformationC3        transpose() const { return Ptr()->transpose(); }
};


#ifndef CGAL_NO_OSTREAM_INSERT_AFF_TRANSFORMATIONC3
template < class R >
std::ostream &operator<<(std::ostream &os,
                         const Aff_transformationC3<R CGAL_CTAG> &t)
{
    t.print(os);
    return os;
}
#endif // CGAL_NO_OSTREAM_INSERT_AFF_TRANSFORMATIONC3

CGAL_END_NAMESPACE

#endif // CGAL_CARTESIAN_AFF_TRANSFORMATION_3_H
