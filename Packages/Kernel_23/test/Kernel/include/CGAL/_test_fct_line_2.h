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
// $Source$
// $Revision$ $Date$
// $Name$
//
// Author(s)     : Susan Hert
 

#ifndef CGAL__TEST_FCT_LINE_2_H
#define CGAL__TEST_FCT_LINE_2_H


template <class R>
bool
_test_fct_line_2(const R& )
{
 std::cout << "Testing functions Line_2" ;

 typedef typename  R::RT       RT;
 typedef typename  R::FT       FT;
 typedef typename  R::Point_2  Point_2;
 typedef typename  R::Line_2   Line_2;

 Point_2 p1 ( RT(18), RT(12), RT(3) );  // ( 6, 4)
 Point_2 p2 ( RT(18), RT(15), RT(3) );  // ( 6, 5)
 Point_2 p3 ( RT(18), RT( 9), RT(3) );  // ( 6, 3)
 Point_2 p4 ( RT(28), RT(40), RT(4) );  // ( 7,10)
 Point_2 p5 ( RT(12), RT(40), RT(4) );  // ( 3,10)
 Point_2 p6 ( RT(28), RT(12), RT(4) );  // ( 7, 3)
 Point_2 p7 ( RT(18), RT( 6), RT(3) );  // ( 6, 2)
 Point_2 p8 ( RT(24), RT( 9), RT(3) );  // ( 8, 3)
 Point_2 p9 ( RT( 6), RT(10), RT(1) );  // ( 6,10)
 Point_2 p10( RT( 8), RT( 5), RT(1) );  // ( 8, 5)
 Point_2 p11( RT( 7), RT( 5), RT(1) );  // ( 7, 5)

 // vertical lines
 Line_2 l1(p1, p2);
 Line_2 l2(p3, p2);
 Line_2 l3(p4, p6);

 assert( CGAL::compare_slopes(l1,l2) == CGAL::EQUAL );
 assert( CGAL::compare_slopes(l1,l3) == CGAL::EQUAL );
 assert( CGAL::compare_slopes(l3,l1) == CGAL::EQUAL );

 std::cout <<'.';

 // horizontal lines
 Line_2 l4(p3, p8);
 Line_2 l5(p4, p9);
 assert( CGAL::compare_slopes(l4, l5) == CGAL::EQUAL );
 assert( CGAL::compare_slopes(l3, l4) == CGAL::LARGER );
 assert( CGAL::compare_slopes(l4, l3) == CGAL::SMALLER );

 std::cout <<'.';

 // parallel lines
 Line_2 l5a(p6, p7);
 Line_2 l5b(p11, p1);
 assert ( CGAL::compare_slopes(l5a, l5b) == CGAL::EQUAL );

 // two positive slopes
 Line_2 l6(p2, p4);
 Line_2 l7(p2, p6);
 Line_2 l8(p7, p10);
 assert( CGAL::compare_slopes(l6, l6) == CGAL::EQUAL );
 assert( CGAL::compare_slopes(l6, l7) == CGAL::LARGER );
 assert( CGAL::compare_slopes(l7, l6) == CGAL::SMALLER );
 assert( CGAL::compare_slopes(l6, l8) == CGAL::LARGER );
 assert( CGAL::compare_slopes(l8, l6) == CGAL::SMALLER );

 // vertical and positive slope
 assert( CGAL::compare_slopes(l1, l6) == CGAL::LARGER );
 assert( CGAL::compare_slopes(l6, l1) == CGAL::SMALLER );

 // horizontal and positive slope
 assert( CGAL::compare_slopes(l5, l6) == CGAL::SMALLER );
 assert( CGAL::compare_slopes(l6, l5) == CGAL::LARGER );

 std::cout <<'.';

 // two negative slopes
 Line_2 l9 (p4, p8);
 Line_2 l10(p9, p8);
 Line_2 l11(p5, p3);

 assert( CGAL::compare_slopes(l9, l10) == CGAL::SMALLER );
 assert( CGAL::compare_slopes(l10, l9) == CGAL::LARGER );
 assert( CGAL::compare_slopes(l11, l10) == CGAL::LARGER );

 // vertical and negative slope
 assert( CGAL::compare_slopes(l2, l9) == CGAL::LARGER );
 assert( CGAL::compare_slopes(l9, l2) == CGAL::SMALLER );

 // horizontal and negative slope
 assert( CGAL::compare_slopes(l5, l9) == CGAL::LARGER );
 assert( CGAL::compare_slopes(l9, l5) == CGAL::SMALLER );

 std::cout <<'.';

 // positive and negative slope
 assert( CGAL::compare_slopes(l6, l9) == CGAL::LARGER );
 assert( CGAL::compare_slopes(l9, l7) == CGAL::SMALLER );

 std::cout << "done" << std::endl;
 return true;
}

#endif // CGAL__TEST_FCT_LINE_2_H
