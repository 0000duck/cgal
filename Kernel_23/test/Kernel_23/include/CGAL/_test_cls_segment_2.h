// Copyright (c) 1999  Utrecht University (The Netherlands),
// ETH Zurich (Switzerland), Freie Universitaet Berlin (Germany),
// INRIA Sophia-Antipolis (France), Martin-Luther-University Halle-Wittenberg
// (Germany), Max-Planck-Institute Saarbruecken (Germany), RISC Linz (Austria),
// and Tel-Aviv University (Israel).  All rights reserved.
//
// This file is part of CGAL (www.cgal.org); you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License as
// published by the Free Software Foundation; version 2.1 of the License.
// See the file LICENSE.LGPL distributed with CGAL.
//
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
//
// $URL$
// $Id$
// 
//
// Author(s)     : Stefan Schirra
 

#ifndef CGAL__TEST_CLS_SEGMENT_2_H
#define CGAL__TEST_CLS_SEGMENT_2_H

#include <CGAL/Bbox_2.h>
#include <CGAL/Testsuite/assert.h>

template <class R>
bool
_test_cls_segment_2(const R& )
{
 std::cout << "Testing class Segment_2";

 typedef typename  R::RT    RT;
 typedef typename  R::FT    FT;

 typename R::Segment_2  is;
 CGAL::Segment_2<R> s0;

 RT  n2 = 2;
 RT  n3 = 3;
 RT  n4 = 4;
 RT  n5 = 5;
 RT  n8 = 8;
 RT  n9 = 9;
 RT n10 =10;

 CGAL::Point_2<R> p1( n2, n8, n2);
 CGAL::Point_2<R> p2( n10, n4, n2);
 CGAL::Point_2<R> p3( n9, n9, n3);
 CGAL::Point_2<R> p4( n10, n8, n2);

 CGAL::Segment_2<R> s1 ( p1, p2 );
 CGAL::Segment_2<R> s2 ( p2, p1 );
 CGAL::Segment_2<R> s3 ( p2, p3 );
 CGAL::Segment_2<R> s4 ( p2, p4 );
 CGAL::Segment_2<R> s5 ( p4, p1 );
 CGAL::Segment_2<R> s6 ( s3 );
 s0 = s2;

 CGAL_test_assert(   CGAL::parallel(s0, s2) );
 CGAL_test_assert(   CGAL::parallel(s1, s2) );
 CGAL_test_assert(   CGAL::parallel(s1, s3) );
 CGAL_test_assert( ! CGAL::parallel(s1, s5) );

 CGAL::Vector_2<R> v1 = s1.to_vector();
 CGAL_test_assert( v1 == (p2-p1) );

 CGAL_test_assert( s5 == s5 );
 CGAL_test_assert( s6 == s3 );
 CGAL_test_assert( s2 == s0 );
 CGAL_test_assert( s0 == s2 );
 CGAL_test_assert( s1 != s5 );
 CGAL_test_assert( s1 != s2 );
 CGAL_test_assert( s3 != s2 );

 std::cout << '.';

 CGAL_test_assert( s1.source() == p1 );
 CGAL_test_assert( s2.source() == p2 );
 CGAL_test_assert( s6.source() == p2 );
 CGAL_test_assert( s4.target() == p4 );
 CGAL_test_assert( s5.target() == p1 );

 CGAL_test_assert( (s1.min)() == p1 );
 CGAL_test_assert( (s3.min)() == p3 );
 CGAL_test_assert( (s4.min)() == p2 );
 CGAL_test_assert( (s4.max)() == p4 );
 CGAL_test_assert( (s5.max)() == p4 );

 CGAL_test_assert( s3.vertex(0) == p2 );
 CGAL_test_assert( s3.vertex(1) == p3 );
 CGAL_test_assert( s3.vertex(2) == p2 );
 CGAL_test_assert( s4.point(8) == s4.vertex(8) );
 CGAL_test_assert( s1.point(3) == s1.vertex(3) );
 CGAL_test_assert( s5[0] == s5.vertex(0) );
 CGAL_test_assert( s6[1] == s6.vertex(1) );

 std::cout << '.';

 CGAL_test_assert( s1.squared_length() == FT( 20 ) );
 CGAL_test_assert( s5.squared_length() == FT( 16 ) );
 CGAL_test_assert( s4.direction() == CGAL::Direction_2<R>( p4 - p2 ) );
 CGAL_test_assert( s2.opposite() == s1 );
 CGAL_test_assert( s1.opposite() == s2 );
 CGAL_test_assert( s1.supporting_line() == CGAL::Line_2<R>( p1, p2 ) );
 CGAL_test_assert( s3.supporting_line() == CGAL::Line_2<R>( p2, p3 ) );
 CGAL_test_assert( ! s1.is_horizontal() );
 CGAL_test_assert( ! s1.is_vertical() );
 CGAL_test_assert( s5.is_horizontal() );
 CGAL_test_assert( s4.is_vertical() );

 std::cout << '.';

 CGAL_test_assert( s1.has_on( p1 ) );
 CGAL_test_assert( s1.has_on( p2 ) );
 CGAL_test_assert( s1.has_on( p3 ) );
 CGAL_test_assert( s2.has_on( p3 ) );
 CGAL_test_assert( ! s4.has_on( p3 ) );
 CGAL_test_assert( s1.collinear_has_on( p3 ) );
 CGAL_test_assert( s2.collinear_has_on( p1 ) );
 CGAL_test_assert( ! s3.collinear_has_on( p1 ) );
 CGAL_test_assert( s3.collinear_has_on( CGAL::Point_2<R>( n8, n5, n2)) );

 CGAL_test_assert( CGAL::Segment_2<R>( p3, p3).is_degenerate() );

 std::cout << '.';

 CGAL::Bbox_2 bb = s1.bbox();
 CGAL_test_assert(bb.xmin() <= 1.0);
 CGAL_test_assert(bb.xmax() >= 5.0);
 CGAL_test_assert(bb.ymin() <= 2.0);
 CGAL_test_assert(bb.ymax() >= 4.0);

 std::cout << "done" << std::endl;
 return true;
}

#endif // CGAL__TEST_CLS_SEGMENT_2_H
