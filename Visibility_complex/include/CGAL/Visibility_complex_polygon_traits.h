// Copyright (c) 2001-2004  ENS of Paris (France).
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
// Author(s)     : Pierre Angelier, Michel Pocchiola

#ifndef CGAL_VISIBILITY_COMPLEX_POLYGON_TRAITS_H
#define CGAL_VISIBILITY_COMPLEX_POLYGON_TRAITS_H

#include <CGAL/basic.h>

#include <CGAL/Polygon_2.h>
#include <CGAL/Bitangent_2.h>
#include <CGAL/Arc_2.h>
#include <CGAL/Polygon_2_Polygon_2_intersection.h>
#include <CGAL/Bitangent_2_Bitangent_2_intersection.h>

CGAL_BEGIN_NAMESPACE

// -----------------------------------------------------------------------------

template < class R_ >
class Visibility_complex_polygon_traits
{
public:
    // -------------------------------------------------------------------------
    // Geometric types
    typedef R_                                R;
    typedef typename R::FT                    FT;
    typedef typename R::Point_2               Point_2;
    typedef typename R::Segment_2             Segment_2;
    typedef std::list<Point_2>                Polygon_container;
    typedef Polygon_2<R,Polygon_container>    Disk;
    typedef Arc_2<Disk>            Arc_2;
    typedef Bitangent_2<Disk>             Bitangent_2;
    // -------------------------------------------------------------------------
public:
    // -------------------------------------------------------------------------
    // The chi2 predicate
    struct Orientation_object {
	Orientation operator()(const Bitangent_2& a,const Bitangent_2& b) const{ 
	    return R().orientation_2_object()(a.source() , a.target() ,
					      a.source() + (b.target() 
							    - b.source()));
	}	
    };
    // -------------------------------------------------------------------------
    // The two follwing give the chi2 predicate with a point at infinity
    struct Compare_extreme_yx {
	const Point_2& extreme_point(bool b, const Disk& c) const {
	    return (b) ? *bottom_vertex_2(c.vertices_begin(),
					  c.vertices_end())  :
			 *top_vertex_2   (c.vertices_begin(),
					  c.vertices_end());
	}
	const Point_2& extreme_point(bool b, const Bitangent_2& c) const 
	{ return (b) ? c.source() : c.target(); }
	template < class C , class D >
	Comparison_result operator() (bool sa , const C& a,
				      bool sb , const D& b) const { 
	    const Point_2& ap = extreme_point(sa,a);
	    const Point_2& bp = extreme_point(sb,b);

	    Comparison_result cr = R().compare_y_2_object()(ap,bp);
	    cr = (cr != EQUAL) ? cr : R().compare_x_2_object()(ap,bp);
	    return cr;
	    
	}
    };
    // -------------------------------------------------------------------------
    struct Is_upward_directed {
	bool operator()(const Bitangent_2& b) const {
	  Comparison_result comp = R().compare_y_2_object()(b.source(), 
							    b.target());
	  comp = (comp != EQUAL) ? comp : R().compare_x_2_object()(b.source(), 
								   b.target());
	  return (comp != LARGER);
	}
    };
    // -------------------------------------------------------------------------
    // The chi3 predicate
    struct Orientation_infinite {
	Orientation operator() (const Bitangent_2& a, 
				const Disk& o) const{
	    return R().orientation_2_object()(a.source(),a.target(),
			       *top_vertex_2(o.vertices_begin(),
					     o.vertices_end()));
	} 
	Orientation operator() (const Disk& o, 
				const Bitangent_2& b) const{
	    return R().orientation_2_object()(*bottom_vertex_2(o.vertices_begin(),
					        o.vertices_end()),
			       *top_vertex_2   (o.vertices_begin(),
						o.vertices_end()),
			       b.target());
	} 
	Orientation operator() (const Bitangent_2& a, 
				const Bitangent_2& b) const
	{ return R().orientation_2_object()(a.source(),a.target(),b.target()); } 
    };
    // -------------------------------------------------------------------------
    // Detection of degenerate cases
    struct Equal_as_segments {
	bool operator() (const Bitangent_2& a, const Bitangent_2& b) const {
	    return (a.source() == b.source() && a.target() == b.target());
	}
    };
    struct Is_point {
	bool operator() (const Disk& c) const { return (c.size() == 1); }
    };
    // -------------------------------------------------------------------------
    // Intersection test. Optional
    typedef Tag_true supports_intersection;
    struct Do_intersect {
	bool operator()(const Disk& o1, const Disk& o2) const {
	    return do_intersect(o1,o2);
	}
	bool operator()(const Bitangent_2& o1, const Disk& o2) const {
	    if (o1.source_object() == &o2 || o1.target_object() == &o2)
		return false;
	    for (typename Disk::Edge_const_iterator e  = o2.edges_begin();
					       e != o2.edges_end() ; ++e)
		if (do_intersect(*e,o1)) return true;
	    return false;		
	}
	bool operator()(const Disk& o1, const Bitangent_2& o2) const {
	    if (o2.source_object() == &o1 || o2.target_object() == &o1)
		return false;
	    for (typename Disk::Edge_const_iterator e  = o1.edges_begin();
					       e != o1.edges_end() ; ++e)
		if (do_intersect(*e,o2)) return true;
	    return false;		
	}
	bool operator()(const Bitangent_2& b1, const Bitangent_2& b2) const {
	    return do_intersect(b1,b2);
	}
    };
    // -------------------------------------------------------------------------
};

// ----------------------------------------------------------------------------- 

CGAL_END_NAMESPACE

#endif // VISIBILITY_COMPLEX_TRAITS_H
