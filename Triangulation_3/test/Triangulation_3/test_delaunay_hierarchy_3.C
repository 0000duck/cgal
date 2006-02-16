// Copyright (c) 1998,2001  INRIA Sophia-Antipolis (France).
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
// Author(s)     : Mariette Yvinec, Sylvain Pion

#include <CGAL/Delaunay_triangulation_3.h>
#include <CGAL/Triangulation_hierarchy_3.h>

bool del=true;

#include <CGAL/_test_types.h>
#include <CGAL/_test_cls_delaunay_3.C>

int main()
{
  typedef CGAL::Triangulation_vertex_base_3<K>             Vbb;
  typedef CGAL::Triangulation_hierarchy_vertex_base_3<Vbb> Vb;
  typedef CGAL::Triangulation_data_structure_3<Vb>         Tds;
  typedef CGAL::Delaunay_triangulation_3<K,Tds>            Dt;
  typedef CGAL::Triangulation_hierarchy_3<Dt>              Dh;

  _test_cls_delaunay_3( Dh() );

  return 0;
}
