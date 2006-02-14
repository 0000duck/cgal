// Copyright (c) 2002  Max Planck Institut fuer Informatik (Germany).
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
// $Id$ $Date$
// 
//
// Author(s)     : Radu Ursu

#include <CGAL/Straight_skeleton_builder_2.h>
#include <CGAL/Polygon_offset_builder_2.h>

typedef CGAL::Straight_skeleton_2<K>                            Sls;
typedef CGAL::Straight_skeleton_builder_traits_2<K>             SlsBuilderTraits;
typedef CGAL::Straight_skeleton_builder_2<SlsBuilderTraits,Sls> SlsBuilder;

typedef CGAL::Polygon_offset_builder_traits_2<K>                        OffsetBuilderTraits;
typedef CGAL::Polygon_offset_builder_2<Sls,OffsetBuilderTraits,Polygon> OffsetBuilder;

typedef Sls::Halfedge_iterator     Halfedge_iterator;
typedef Sls::Vertex_handle         Vertex_handle;
typedef Sls::Face_const_iterator   Face_const_iterator;
typedef Sls::Halfedge_const_handle Halfedge_const_handle ;
typedef Sls::Vertex_const_handle   Vertex_const_handle ;

