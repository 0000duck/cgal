// Copyright (c) 2005, 2006 Fernando Luis Cacciola Carballal. All rights reserved.
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
// $URL: svn+ssh://fcacciola@scm.gforge.inria.fr/svn/cgal/trunk/Straight_skeleton_2/demo/Straight_skeleton_2/cgal_types.h $
// $Id: cgal_types.h 28552 2006-02-15 18:52:27Z fcacciola $
//
// Author(s)     : Fernando Cacciola <fernando_cacciola@ciudad.com.ar>
//
#ifndef CGAL_TEST_SLS_BUILDER_TYPES_H
#define CGAL_TEST_SLS_BUILDER_TYPES_H

#include <CGAL/basic.h>
#include <CGAL/Simple_cartesian.h>
#include <CGAL/Polygon_2.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

#include <CGAL/Straight_skeleton_builder_2.h>
#include <CGAL/Polygon_offset_builder_2.h>
#include <CGAL/compute_outer_frame_margin.h>
#include <CGAL/HalfedgeDS_const_decorator.h>

#include <boost/shared_ptr.hpp>
#include <vector>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;

typedef K::Point_2                    Point;
typedef CGAL::Aff_transformation_2<K> Transformation;
typedef CGAL::Polygon_2<K>            Polygon;
typedef boost::shared_ptr<Polygon>    PolygonPtr;
typedef CGAL::Segment_2<K>            Segment;
typedef std::vector<PolygonPtr>       Region ;
typedef boost::shared_ptr<Region>     RegionPtr ;
typedef std::vector<RegionPtr>        Regions ;
typedef std::vector<double>           Doubles ;

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

typedef CGAL::HalfedgeDS_const_decorator<Sls> Sls_const_decorator ;

typedef boost::shared_ptr<Sls> SlsPtr ;

#endif

