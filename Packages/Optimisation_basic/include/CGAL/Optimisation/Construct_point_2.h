// ============================================================================
//
// Copyright (c) 1997-2000 The CGAL Consortium
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
// file          : include/CGAL/Optimisation/Construct_point_2.h
// package       : $CGAL_Package: Optimisation_basic WIP $
//
// revision      : $Revision$
// revision_date : $Date$
//
// author(s)     : Sven Sch�nherr
// maintainer    : Sven Sch�nherr <sven@inf.ethz.ch>
// coordinator   : ETH Z�rich (Bernd G�rtner <gaertner@inf.ethz.ch>)
//
// implementation: dD construction `point'
// ============================================================================

#ifndef CGAL_OPTIMISATION_CONSTRUCT_POINT_2_H
#define CGAL_OPTIMISATION_CONSTRUCT_POINT_2_H

// includes
#ifndef CGAL_POINT_2_H
#  include <CGAL/Point_2.h>
#endif
#ifndef CGAL_PROTECT_VECTOR
#  include <vector>
#  define CGAL_PROTECT_VECTOR
#endif
#ifndef CGAL_PROTECT_FUNCTIONAL
#  include <functional>
#  define CGAL_PROTECT_FUNCTIONAL
#endif
#ifndef CGAL_PROTECT_ITERATOR
#  include <iterator>
#  define CGAL_PROTECT_ITERATOR
#endif

CGAL_BEGIN_NAMESPACE

// Class declaration
// =================
template < class R >
class Construct_point_2;

// Class interface
// ===============
template < class R_ >
class Construct_point_2 {
  public:
    // self
    typedef  R_                         R;
    typedef  Construct_point_2<R>       Self;

    // types
    typedef  typename R::Point_2        Point;

    // creation
    Construct_point_2( ) { }

    // operations
    template < class InputIterator >
    Point
    operator() ( int d, InputIterator first, InputIterator last) const
    {
	std::vector<typename R::RT>  coords;
	std::copy( first, last, std::back_inserter( coords));
	if ( coords.size() < 3) {
	    return Point( coords[ 0], coords[ 1]);
	} else {
	    return Point( coords[ 0], coords[ 1], coords[ 2]);
	}
    }
};

CGAL_END_NAMESPACE

#endif // CGAL_OPTIMISATION_CONSTRUCT_POINT_2_H

// ===== EOF ==================================================================
