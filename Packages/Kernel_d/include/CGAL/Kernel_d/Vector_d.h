// ======================================================================
//
// Copyright (c) 2000,2001 The CGAL Consortium
//
// This software and related documentation is part of an INTERNAL release
// of the Computational Geometry Algorithms Library (CGAL). It is not
// intended for general use.
//
// ----------------------------------------------------------------------
//
// release       : $CGAL_Revision: CGAL-2.4-I-64 $
// release_date  : $CGAL_Date: 2002/03/18 $
//
// file          : include/CGAL/Kernel_d/Vector_d.h
// package       : Kernel_d (0.9.54)
// maintainer    : Michael Hoffmann <hoffmann@inf.ethz.ch>
// revision      : $Revision$
// revision_date : $Date$
// author(s)     : Michael Seel
// coordinator   : ETH Zurich 
//
// ======================================================================
#ifndef CGAL_VECTOR_D_H
#define CGAL_VECTOR_D_H

CGAL_BEGIN_NAMESPACE
 
template <class pR>
class Vector_d : public pR::Vector_d_base
{ public:
  typedef typename pR::Vector_d_base Base;
  typedef Vector_d<pR>               Self;
  typedef pR R;
  typedef typename R::RT RT;
  typedef typename R::FT FT;
  typedef typename R::LA LA;
  typedef typename Base::Base_vector Base_vector;

  Vector_d(int d=0) : Base(d) {}
  Vector_d(int d, Null_vector v) : Base(d,v) {}
  Vector_d(int a, int b, int c = 1) : Base(a,b,c) {}
  Vector_d(const RT& a, const RT& b, const RT& c = 1) :
    Base(a,b,c) {}
  Vector_d(int a, int b, int c, int d) : Base(a,b,c,d) {}
  Vector_d(const RT& a, const RT& b, const RT& c, const RT& d) :
    Base(a,b,c,d) {}
  Vector_d(int d, Base_vector, int i) :
    Base(d,Base_vector(),i) {}

  template <class InputIterator>
  Vector_d (int d, InputIterator first, InputIterator last)
    : Base (d, first, last) {}
  template <class InputIterator>
  Vector_d (int d, InputIterator first, InputIterator last, const RT& D)
    : Base (d, first, last, D) {}

  Vector_d(const Self& v) : Base(v) {}
  Vector_d(const Base& v) : Base(v) {}

  Direction_d<R> direction() const { return Base::direction(); }

  FT operator* (const Self& w) const
  { return Base::operator*(w); }
  Self operator+(const Self& w) const
  { return Base::operator+(w); }
  Self operator-(const Self& w) const
  { return Base::operator-(w); }
  Self operator-() const 
  { return Base::operator-(); }

  template <class NT> 
  Self operator/(const NT& n) const { return Base::operator/(n); }

  Self& operator+=(const Self& w) 
  { return static_cast<Self&>(Base::operator+=(w)); }
  Self& operator-=(const Self& w) 
  { return static_cast<Self&>(Base::operator-=(w)); }
  template <class NT> 
  Self& operator*=(const NT& n) 
  { return static_cast<Self&>(Base::operator*=(n)); }
  template <class NT> 
  Self& operator/=(const NT& n) 
  { return static_cast<Self&>(Base::operator/=(n)); }

  bool operator==(const Self& w) const
  { return Base::operator==(w); }
  bool operator!=(const Self& w) const
  { return Base::operator!=(w); }
  
};

template <class R> Point_d<R> 
operator+ (const Origin& o, const Vector_d<R>& v)
{ return Point_d<R>( o + static_cast<typename Vector_d<R>::Base>(v) ); }

template <class NT, class R>
Vector_d<R> operator*(const NT& n, const Vector_d<R>& v) 
{ return Vector_d<R>( n * static_cast<typename Vector_d<R>::Base>(v) ); }

CGAL_END_NAMESPACE
#endif //CGAL_VECTOR_D_H
