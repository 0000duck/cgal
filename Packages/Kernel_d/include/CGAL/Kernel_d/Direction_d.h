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
// file          : include/CGAL/Kernel_d/Direction_d.h
// package       : Kernel_d (0.9.54)
// maintainer    : Michael Hoffmann <hoffmann@inf.ethz.ch>
// revision      : $Revision$
// revision_date : $Date$
// author(s)     : Michael Seel
// coordinator   : ETH Zurich
//
// ======================================================================
#ifndef CGAL_DIRECTION_D_H
#define CGAL_DIRECTION_D_H

CGAL_BEGIN_NAMESPACE

template <class pR>
class Direction_d : public pR::Direction_d_base
{ public:
  typedef typename pR::Direction_d_base Base;
  typedef Direction_d<pR>               Self;
  typedef pR R;
  typedef typename R::RT RT;
  typedef typename R::FT FT;
  typedef typename R::LA LA;
  typedef typename Base::Base_direction Base_direction;

  Direction_d(int d=0) : Base(d) {}
  Direction_d(int a, int b) : Base(a,b) {}
  Direction_d(const RT& a, const RT& b) : Base(a,b) {}
  Direction_d(int a, int b, int c) : Base(a,b,c) {}
  Direction_d(const RT& a, const RT& b, const RT& c) : Base(a,b,c) {}

  template <class InputIterator>
  Direction_d (int d, InputIterator first, InputIterator last)
    : Base(d, first, last) {}

  Direction_d(const Direction_d<R> &d) : Base(d) {}
  Direction_d(const Vector_d<R> &v) : Base(v) {}
  Direction_d(int d, Base_direction, int i) : 
    Base(d,Base_direction(),i) {}
  Direction_d(const Base& p) : Base(p) {}

  Self operator-() const { return Base::operator-(); }
  Vector_d<R> vector() const { return Base::vector(); }

  bool operator==(const Self& w) const
  { return Base::operator==(w); }
  bool operator!=(const Self& w) const
  { return Base::operator!=(w); }
};

CGAL_END_NAMESPACE
#endif //CGAL_DIRECTION_D_H
