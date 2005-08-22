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
// release       : $CGAL_Revision: CGAL-I $
// release_date  : $CGAL_Date$
//
// file          : test/Min_sphere_d_new/test_Min_sphere_d.h
// package       : $CGAL_Package: Min_sphere_d_new $
// chapter       : Geometric Optimisation
//
// source        : web/Min_sphere_d.aw
// revision      : $Revision$
// revision_date : $Date$
//
// author(s)     : Bernd G�rtner, Sven Sch�nherr <sven@inf.ethz.ch>
// coordinator   : ETH Z�rich (Bernd G�rtner <gaertner@inf.ethz.ch>)
//
// implementation: test function for smallest enclosing sphere
// ============================================================================

#ifndef CGAL_TEST_MIN_SPHERE_D_H
#define CGAL_TEST_MIN_SPHERE_D_H

// includes
#ifndef CGAL_IO_VERBOSE_OSTREAM_H
#  include <CGAL/IO/Verbose_ostream.h>
#endif
#include <cassert>

CGAL_BEGIN_NAMESPACE

#define COVER(text,code) \
            verr0.out().width( 26); verr0 << text << "..." << flush; \
            verrX.out().width(  0); verrX << "==> " << text << endl \
              << "----------------------------------------" << endl; \
            { code } verr0 << "ok."; verr << endl;

template < class ForwardIterator, class Traits >
void
test_Min_sphere_d( ForwardIterator first, ForwardIterator last,
                   const Traits& traits, int verbose)
{
    using namespace std;

    typedef  CGAL::Min_sphere_d< Traits >  Min_sphere;
    typedef  typename Traits::Point_d      Point;

    CGAL::Verbose_ostream  verr ( verbose >= 0);
    CGAL::Verbose_ostream  verr0( verbose == 0);
    CGAL::Verbose_ostream  verrX( verbose >  0);
    CGAL::set_pretty_mode( verr.out());

    bool  is_valid_verbose = ( verbose > 0);

    // constructors
    COVER( "default constructor",
        Min_sphere  ms( traits);
        assert( ms.is_valid( is_valid_verbose));
        assert( ms.is_empty());
    )

    COVER( "point set constructor",
        Min_sphere  ms( first, last, traits);
        verrX << endl;
        assert( ms.is_valid( is_valid_verbose));
    )

    Min_sphere  min_sphere( first, last);
    COVER( "ambient dimension",
        Min_sphere  ms;
        assert( ms.ambient_dimension() == -1);
        verrX << min_sphere.ambient_dimension() << endl;
    )

    COVER( "(number of) points",
        verrX << min_sphere.number_of_points() << endl;
        typename Min_sphere::Point_iterator
            point_it = min_sphere.points_begin();
        for ( ; point_it != min_sphere.points_end(); ++point_it) {
            verrX << *point_it << endl;
        }
        assert( ( min_sphere.points_end() - min_sphere.points_begin())
                == min_sphere.number_of_points());
    )

    COVER( "(number of) support points",
        verrX << min_sphere.number_of_support_points() << endl;
        typename Min_sphere::Support_point_iterator
            point_it = min_sphere.support_points_begin();
        for ( ; point_it != min_sphere.support_points_end(); ++point_it) {
            verrX << *point_it << endl;
        }
        assert( ( min_sphere.support_points_end()
                  - min_sphere.support_points_begin())
                == min_sphere.number_of_support_points());
    )

    COVER( "center and squared radius",
        verrX << "center (as range):";
        typename Min_sphere::Coordinate_iterator  coord_it;
        for ( coord_it  = min_sphere.center_coordinates_begin();
              coord_it != min_sphere.center_coordinates_end();
              ++coord_it) {
            verrX << ' ' << *coord_it;
        }
        verrX << endl;
        verrX << "squared radius numerator  : "
              << min_sphere.squared_radius_numerator()   << endl;
        verrX << "squared radius denominator: "
              << min_sphere.squared_radius_denominator() << endl;
    )

    COVER( "predicates",
        CGAL::Bounded_side  bounded_side;
        bool                has_on_bounded_side;
        bool                has_on_boundary;
        bool                has_on_unbounded_side;
        Point               p;
        typename Min_sphere::Point_iterator
            point_it = min_sphere.points_begin();
        for ( ; point_it != min_sphere.points_end(); ++point_it) {
            p = *point_it;
            bounded_side          = min_sphere.bounded_side( p);
            has_on_bounded_side   = min_sphere.has_on_bounded_side( p);
            has_on_boundary       = min_sphere.has_on_boundary( p);
            has_on_unbounded_side = min_sphere.has_on_unbounded_side( p);
            verrX.out().width( 2);
            verrX << bounded_side          << "  "
                  << has_on_bounded_side   << ' '
                  << has_on_boundary       << ' '
                  << has_on_unbounded_side << endl;
            assert( bounded_side != CGAL::ON_UNBOUNDED_SIDE);
            assert( has_on_bounded_side || has_on_boundary);
            assert( ! has_on_unbounded_side);
        }
    )

    COVER( "clear",
        min_sphere.clear();
        verrX << "min_sphere is" << ( min_sphere.is_empty() ? "" : " not")
              << " empty." << endl;
        assert( min_sphere.is_empty());
    )

    COVER( "insert (single point)",
        min_sphere.insert( *first);
        assert( min_sphere.is_valid( is_valid_verbose));
        assert( min_sphere.is_degenerate());
    )

    COVER( "insert (point set)",
        min_sphere.insert( first, last);
        assert( min_sphere.is_valid( is_valid_verbose));
    )

    COVER( "traits class access",
        min_sphere.traits();
    )

    COVER( "I/O",
        verrX << min_sphere;
    )
}

CGAL_END_NAMESPACE

#endif // CGAL_TEST_MIN_SPHERE_D_H

// ===== EOF ==================================================================
