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
// file          : leda_bigfloat.h
// package       : Number_types
// revision      : $Revision$
// revision_date : $Date$
// author(s)     : Stefan Schirra
//
//
// coordinator   : MPI, Saarbruecken
// ======================================================================
 

#ifndef CGAL_LEDA_BIGFLOAT_H
#define CGAL_LEDA_BIGFLOAT_H

#include <CGAL/basic.h>
#include <CGAL/LEDA_basic.h>
#include <LEDA/bigfloat.h>

CGAL_BEGIN_NAMESPACE

template <> struct Number_type_traits<leda_bigfloat> {
  typedef Tag_false Has_gcd;
  typedef Tag_true  Has_division;
  typedef Tag_false Has_sqrt;
};

#ifndef CGAL_CFG_NO_NAMESPACE
inline
double
to_double(const leda_bigfloat & b)
{ return CGAL_LEDA_SCOPE::to_double(b); }
#endif // CGAL_CFG_NO_NAMESPACE

inline
bool
is_finite(const leda_bigfloat & b)
{ return !( CGAL_LEDA_SCOPE::isInf(b) || CGAL_LEDA_SCOPE::isNaN(b) ); }

inline
bool
is_valid(const leda_bigfloat & b)
{ return !( CGAL_LEDA_SCOPE::isNaN(b) ); }

inline
io_Operator
io_tag(const leda_bigfloat &)
{ return io_Operator(); }

inline
std::pair<double,double>
to_interval (const leda_bigfloat & z)
{
  // assuming leda_bigfloat guarantee 1 bit error max
  Protect_FPU_rounding<true> P (CGAL_FE_TONEAREST);
  Interval_nt_advanced approx (CGAL_LEDA_SCOPE::to_double(z));
  FPU_set_cw(CGAL_FE_UPWARD);
  approx += Interval_nt<false>::smallest();
  return approx.pair();
}

CGAL_END_NAMESPACE

#endif // CGAL_LEDA_BIGFLOAT_H
