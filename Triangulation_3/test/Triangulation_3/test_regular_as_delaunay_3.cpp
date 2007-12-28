// Copyright (c) 2004  INRIA Sophia-Antipolis (France).
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
// Author(s)     : Sylvain Pion

#include <CGAL/Regular_triangulation_3.h>
#include <CGAL/Regular_triangulation_euclidean_traits_3.h>
#include <CGAL/Regular_triangulation_filtered_traits_3.h>

bool del=true;

#include <CGAL/_test_types.h>
#include <CGAL/_test_cls_delaunay_3.h>

typedef CGAL::Regular_triangulation_filtered_traits_3<K>        Traits;

int main()
{
  typedef CGAL::Regular_triangulation_3<Traits>    Cls;

  _test_cls_delaunay_3( Cls() );

  return 0;
}

// MipsPro prefers this after the other instantiations...
// Explicit instantiation of the whole class :
template class CGAL::Regular_triangulation_3<Traits>;
