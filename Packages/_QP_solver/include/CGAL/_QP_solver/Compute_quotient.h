// ============================================================================
//
// Copyright (c) 1997-2001 The CGAL Consortium
//
// This software and related documentation is part of an INTERNAL release
// of the Computational Geometry Algorithms Library (CGAL). It is not
// intended for general use.
//
// ----------------------------------------------------------------------------
//
// release       : $CGAL_Revision: CGAL-wip $
// release_date  : $CGAL_Date$
//
// file          : include/CGAL/_QP_solver/Compute_quotient.h
// package       : _QP_solver 0.9.1 (21 Mar 2001)
//
// revision      : 0.1
// revision_date : 2000/08/09 00:00:00
//
// author(s)     : Sven Sch�nherr
// maintainer    : Sven Sch�nherr <sven@inf.ethz.ch>
// coordinator   : ETH Z�rich (Bernd G�rtner <gaertner@inf.ethz.ch>)
//
// implementation: compute quotient function class
// ============================================================================

#ifndef CGAL_COMPUTE_QUOTIENT_H
#define CGAL_COMPUTE_QUOTIENT_H

#include <CGAL/Quotient.h>
#include <functional>

CGAL_BEGIN_NAMESPACE

template < class NT >
struct Compute_quotient {
    typedef  NT                         argument1_type;
    typedef  NT                         argument2_type;
    typedef  CGAL::Quotient<NT>         result_type;

    result_type  operator () ( const NT& numer, const NT& denom) const
        { return result_type( numer, denom); }
};

CGAL_END_NAMESPACE
  
#endif

// ===== EOF ==================================================================
