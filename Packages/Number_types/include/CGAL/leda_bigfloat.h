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
// coordinator   : MPI, Saarbruecken  (<Stefan.Schirra@mpi-sb.mpg.de>)
// ======================================================================
 

#ifndef CGAL_BIGFLOAT_H
#define CGAL_BIGFLOAT_H

#include <CGAL/basic.h>
#include <CGAL/LEDA_basic.h>

// #ifndef IO_IO_TAGS_H
// #include <CGAL/IO/io_tags.h>
// #endif // IO_IO_TAGS_H
// #ifndef CGAL_NUMBER_TYPE_TAGS_H
// #include <CGAL/number_type_tags.h>
// #endif // CGAL_NUMBER_TYPE_TAGS_H

#ifndef CGAL_PROTECT_LEDA_BIGFLOAT_H
#include <LEDA/bigfloat.h>
#define CGAL_PROTECT_LEDA_BIGFLOAT_H
#endif // CGAL_PROTECT_LEDA_BIGFLOAT_H

CGAL_BEGIN_NAMESPACE

template <> struct Number_type_traits<leda_bigfloat> {
  typedef Tag_false Has_gcd_tag;
  typedef Tag_true  Has_division_tag;
  typedef Tag_false Has_sqrt_tag;
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
Interval_base
to_interval (const leda_bigfloat & z)
{
  // assuming leda_bigfloat guarantee 1 bit error max
  Protect_FPU_rounding<true> P (CGAL_FE_TONEAREST);
  Interval_nt_advanced approx (CGAL_LEDA_SCOPE::to_double(z));
  FPU_set_cw(CGAL_FE_UPWARD);
  return approx + Interval_base::Smallest;
}


CGAL_END_NAMESPACE



#endif // CGAL_BIGFLOAT_H
