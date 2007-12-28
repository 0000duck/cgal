// Copyright (c) 1998  INRIA Sophia-Antipolis (France).
// All rights reserved.
//
// This file is part of CGAL (www.cgal.org); you may redistribute it under
// the terms of the Q Public License version 1.0.
// See the file LICENSE.QPL distributed with CGAL.
//
// Licensees holding a valid commercial license may use this file in
// accordance with the commercial license agreement provided with the software.
//
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
//
// $URL$
// $Id$ 
// $Date$
// 
//
// Author(s)     : Mariette Yvinec

#include <CGAL/Regular_triangulation_euclidean_traits_3.h>

#include <CGAL/Testsuite/assert.h>

#include <CGAL/Simple_cartesian.h>
#include <CGAL/Gmpz.h>
#include <CGAL/MP_Float.h>
#include <CGAL/Quotient.h>

#include <CGAL/_test_cls_regular_euclidean_traits_3.h>

//needs an exact number type to test the traits class
#ifdef CGAL_USE_GMP
typedef CGAL::Quotient<CGAL::Gmpz>  NT;
#else
typedef CGAL::Quotient<CGAL::MP_Float>  NT;
#endif

typedef CGAL::Simple_cartesian<NT> K;

// Explicit instantiation of the whole class :
template class CGAL::Regular_triangulation_euclidean_traits_3<K>;



int main()
{
  typedef CGAL::Regular_triangulation_euclidean_traits_3<K> Traits;
  _test_cls_regular_euclidean_traits_3(Traits() );
}
