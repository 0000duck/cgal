
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
// file          : _test_valid_finite_float.h
// revision      : $Revision$
// revision_date : $Date$
// author(s)     : Stefan Schirra
//
//
// coordinator   : MPI, Saarbruecken  (<Stefan.Schirra@mpi-sb.mpg.de>)
// ============================================================================
 

#include <cmath>

bool
_test_valid_finite( const float& )
{
  float d0 = 0.0;
  float d1 = 1.0;
  float d2;
  
  //  d0 = 0.0;
  //  d1 = 1.0;
  
  assert( CGAL::is_valid( d0) );
  assert( CGAL::is_valid( d1) );
  
  assert( CGAL::is_finite( d0) );
  assert( CGAL::is_finite( d1) );
  
  if ( CGAL::is_valid( d1/d0 - d1/d0 ))
  { d2 = d1/d0 - d1/d0; show( reinterpret_cast<IEEE_754_float*>(&d2)); }
  if ( CGAL::is_finite( d1/d0 ))
  { d2 = d1/d0; show( reinterpret_cast<IEEE_754_float*>(&d2)); }
  
  assert( CGAL::is_valid( d1/d0 ));
  assert( !CGAL::is_valid( d1/d0 - d1/d0 ));
  assert( !CGAL::is_finite( d1/d0 ));
  
  

  return true;
}

