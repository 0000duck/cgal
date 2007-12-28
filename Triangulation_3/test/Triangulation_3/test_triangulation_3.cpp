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

#include <CGAL/Triangulation_3.h>
#include <CGAL/Testsuite/assert.h>

bool del = false;

#include <CGAL/_test_types.h>
#include <CGAL/_test_cls_triangulation_3.h>
#include <CGAL/Triangulation_vertex_base_with_info_3.h>

// Explicit instantiation of the whole class :
template class CGAL::Triangulation_3<K>;

int main()
{
  typedef CGAL::Triangulation_3<K>                               Cls3;

  _test_cls_triangulation_3( Cls3() );

  // Test operator== between triangulations of different Tds types.

  typedef CGAL::Triangulation_3<K, CGAL::Triangulation_data_structure_3<CGAL::Triangulation_vertex_base_with_info_3<int, K> > > Cls3_2;

  CGAL_test_assert(Cls3() == Cls3_2());

  return 0;
}
