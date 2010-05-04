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
// 
//
// Author(s)     : Francois Rebufat
//                 Manuel Caroli

#include <iostream>
#include <cassert>

#include <CGAL/Periodic_3_Delaunay_triangulation_3.h>
#include <CGAL/Periodic_3_triangulation_traits_3.h>

#include <CGAL/internal/Exact_type_selector.h>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
typedef CGAL::Exact_predicates_inexact_constructions_kernel K1;
typedef CGAL::Periodic_3_triangulation_traits_3<K1> PTT1;
// Explicit instantiation of the whole class:
template class CGAL::Periodic_3_triangulation_3<PTT1>;

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
typedef CGAL::Exact_predicates_exact_constructions_kernel K2;
typedef CGAL::Periodic_3_triangulation_traits_3<K2> PTT2;
// Explicit instantiation of the whole class:
template class CGAL::Periodic_3_triangulation_3<PTT2>;

#include <CGAL/MP_Float.h>
#include <CGAL/Simple_homogeneous.h>
typedef CGAL::Simple_homogeneous<CGAL::MP_Float> K3;
typedef CGAL::Periodic_3_triangulation_traits_3<K3> PTT3;
// Explicit instantiation of the whole class:
template class CGAL::Periodic_3_triangulation_3<PTT3>;

#include <CGAL/_test_cls_periodic_3_triangulation_3.h>

// We use Periodic_3_Delaunay_triangulation_3 because
// Periodic_3_triangulation_3 does not have an insert function and
// thus we cannot construct non-trivial triangulations without using
// the insert from the periodic Delaunay triangulation.

int main()
{
  // temporary test to create minimal example for the problems with CentOS
  K1::FT ft1(-0.1);
  K1::FT ft2(0.2);
  K1::Iso_cuboid_3 domain(ft1,ft1,ft1,ft2,ft2,ft2);

  typedef CGAL::internal::Exact_type_selector<K1::FT>::Type EFT;
  assert((EFT(domain.xmax())-EFT(domain.xmin()))
      == (EFT(domain.ymax())-EFT(domain.ymin())));
  assert((EFT(domain.xmax())-EFT(domain.xmin()))
      == (EFT(domain.zmax())-EFT(domain.zmin())));
  assert((EFT(domain.ymax())-EFT(domain.ymin()))
      == (EFT(domain.zmax())-EFT(domain.zmin())));

  typedef CGAL::Periodic_3_triangulation_3<PTT1>            P3T3_1;
  _test_periodic_3_triangulation_3_constructors( P3T3_1() );
  typedef CGAL::Periodic_3_Delaunay_triangulation_3<PTT1>   P3DT3_1;
  _test_cls_periodic_3_triangulation_3( P3DT3_1() );

  typedef CGAL::Periodic_3_triangulation_3<PTT2>            P3T3_2;
  _test_periodic_3_triangulation_3_constructors( P3T3_2() );
  typedef CGAL::Periodic_3_Delaunay_triangulation_3<PTT2>   P3DT3_2;
  _test_cls_periodic_3_triangulation_3( P3DT3_2(), true );

  typedef CGAL::Periodic_3_triangulation_3<PTT3>            P3T3_3;
  _test_periodic_3_triangulation_3_constructors( P3T3_3() );
  typedef CGAL::Periodic_3_Delaunay_triangulation_3<PTT3>   P3DT3_3;
  //_test_cls_periodic_3_triangulation_3( P3DT3_3(), false, true );

  return 0;
}
