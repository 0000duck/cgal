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
// file          : include/CGAL/IO/Min_circle_2_Window_stream.h
// package       : Min_circle_2 3.10.4 (19 Mar 2001)
// chapter       : Geometric Optimisation
//
// source        : web/Min_circle_2.aw
// revision      : 5.30
// revision_date : 2001/03/19 18:06:10
//
// author(s)     : Sven Sch�nherr, Bernd G�rtner
// maintainer    : Sven Sch�nherr <sven@inf.ethz.ch>
// coordinator   : ETH Z�rich (Bernd G�rtner <gaertner@inf.ethz.ch>)
//
// implementation: graphical output to `leda_window' for Min_circle_2 algorith.
// ============================================================================

// Each of the following operators is individually
// protected against multiple inclusion.

// Window_stream I/O operators
// ===========================

// Optimisation_circle_2
// ---------------------
#ifdef CGAL_OPTIMISATION_CIRCLE_2_H
#ifndef CGAL_IO_WINDOW_STREAM_OPTIMISATION_CIRCLE_2
#define CGAL_IO_WINDOW_STREAM_OPTIMISATION_CIRCLE_2

template< class R >
CGAL::Window_stream&
operator << ( CGAL::Window_stream &ws,
              const CGAL::Optimisation_circle_2<R>& oc)
{
    double  cx( CGAL::to_double( oc.center().x()));
    double  cy( CGAL::to_double( oc.center().y()));
    double  sr( CGAL::to_double( oc.squared_radius()));

    if ( ! CGAL_NTS is_negative( sr))
        ws.draw_circle( cx, cy, CGAL::sqrt( sr));
    return( ws);
}

#endif // CGAL_IO_WINDOW_STREAM_OPTIMISATION_CIRCLE_2
#endif // CGAL_OPTIMISATION_CIRCLE_2_H

// Min_circle_2
// ------------
#ifdef CGAL_MIN_CIRCLE_2_H
#ifndef CGAL_IO_WINDOW_STREAM_MIN_CIRCLE_2
#define CGAL_IO_WINDOW_STREAM_MIN_CIRCLE_2

template< class R >
CGAL::Window_stream&
operator << ( CGAL::Window_stream &ws,
              const CGAL::Min_circle_2<R>& min_circle)
{
    typedef  CGAL::Min_circle_2<R>::Point_iterator  Point_iterator;

    Point_iterator  first( min_circle.points_begin());
    Point_iterator  last ( min_circle.points_end());
    for ( ; first != last; ++first)
        ws << *first;
    return( ws << min_circle.circle());
}

#endif // CGAL_IO_WINDOW_STREAM_MIN_CIRCLE_2
#endif // CGAL_MIN_CIRCLE_2_H

// ===== EOF ==================================================================
