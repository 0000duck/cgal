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
// file          : leda_rational.h
// package       : Number_types
// revision      : $Revision$
// revision_date : $Date$
// author(s)     : Andreas Fabri
//
// coordinator   : MPI, Saarbruecken
// ======================================================================
 

#ifndef CGAL_LEDA_RATIONAL_H
#define CGAL_LEDA_RATIONAL_H

#include <CGAL/basic.h>
#include <CGAL/Number_type_traits.h>
#include <CGAL/Interval_arithmetic.h>

#include <utility>

#include <CGAL/LEDA_basic.h>
#include <LEDA/rational.h>

CGAL_BEGIN_NAMESPACE

template <> 
struct Number_type_traits<leda_rational> {
  typedef Tag_false Has_gcd;
  typedef Tag_true  Has_division;
  typedef Tag_false Has_sqrt;
};

template <> 
struct Rational_traits<leda_rational> {
  typedef leda_integer RT;
 RT numerator   (const leda_rational & r) const { return r.numerator(); }
 RT denominator (const leda_rational & r) const { return r.denominator(); }
 leda_rational make_rational(const RT & n, const RT & d) const
 { return leda_rational(n, d); } 
};

#ifndef CGAL_NO_NAMESPACE
inline
double
to_double(const leda_rational &r)
{ return r.to_double(); }
#endif // CGAL_NO_NAMESPACE

inline
bool
is_finite(const leda_rational &)
{ return true; }

inline
bool
is_valid(const leda_rational &)
{ return true; }

inline
io_Operator
io_tag(const leda_rational &)
{ return io_Operator(); }

#ifndef CGAL_CFG_NO_NAMESPACE
inline
Sign
sign(const leda_rational& r)
{ return (Sign) CGAL_LEDA_SCOPE::sign(r); }
#endif // CGAL_CFG_NO_NAMESPACE

inline
std::pair<double,double>
to_interval (const leda_rational & z)
{
  // There's no guarantee about the error of to_double(), so I add 3 ulps...
  Protect_FPU_rounding<true> P (CGAL_FE_TONEAREST);
  Interval_nt_advanced approx (z.to_double());
  FPU_set_cw(CGAL_FE_UPWARD);

  approx += Interval_nt<false>::smallest();
  approx += Interval_nt<false>::smallest();
  approx += Interval_nt<false>::smallest();
  return approx.pair();
}

CGAL_END_NAMESPACE

#endif  // CGAL_LEDA_RATIONAL_H
