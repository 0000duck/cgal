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
// $Source$
// $Revision$ $Date$
// $Name$
//
// Author(s)     : Pierre Angelier, Michel Pocchiola

#ifndef VISIBILITY_COMPLEX_POINT_TRAITS_H
#define VISIBILITY_COMPLEX_POINT_TRAITS_H

#ifndef CGAL_BASIC_H
#include <CGAL/basic.h>
#endif

#ifndef CGAL_BITANGENT_2_H
#include <CEP/Visibility_complex/Bitangent_2.h>
#endif

#ifndef CGAL_CONVEX_ARC_2
#include <CEP/Visibility_complex/Arc_2.h>
#endif

CGAL_BEGIN_NAMESPACE

// -----------------------------------------------------------------------------

template <class R_>
class Visibility_complex_point_traits
{
public:
    // -------------------------------------------------------------------------
    typedef R_                            R;
    typedef typename R::FT                FT;
    typedef typename R::Point_2           Point_2;
    typedef typename R::Segment_2         Segment_2;
    typedef Point_2                       Disk;
    typedef Arc_2<Disk>                   Arc_2;
    typedef Bitangent_2<Disk>             Bitangent_2;
    // -------------------------------------------------------------------------
    // The chi2 predicate
    struct Orientation_object {
	Orientation operator()(const Bitangent_2& a,const Bitangent_2& b) const{ 
	    return orientation(a.source() , a.target() ,
			       a.source() + (b.target() - b.source()));
	}	
    };
    // -------------------------------------------------------------------------
    // The two follwing give the chi2 predicate with a point at infinity
    struct Compare_extreme_yx {
      Point_2 extreme_point(bool /*b*/, const Disk& c) const { return c; }
	Point_2 extreme_point(bool b, const Bitangent_2& c) const 
	{ return (b) ? c.source() : c.target(); }
	template < class C , class D >
	Comparison_result operator() (bool sa , const C& a,
				      bool sb , const D& b) const { 
	    Point_2 ap = extreme_point(sa,a);
	    Point_2 bp = extreme_point(sb,b);
	    return compare_lexicographically_xyC2(ap.y(),ap.x(),bp.y(),bp.x());
	}
    };
    // -------------------------------------------------------------------------
    struct Is_upward_directed {
	bool operator()(const Bitangent_2& b) const {
	    Comparison_result comp = 
		compare_lexicographically_xyC2(b.source().y(),b.source().x(),
					       b.target().y(),b.target().x());
	    return (comp != LARGER);
	}
    };
    // -------------------------------------------------------------------------
    // The chi3 predicate
    struct Orientation_infinite {
	Orientation operator() (const Bitangent_2& a, 
				const Disk& o) const
	{ return orientation(a.source(),a.target(),o); } 
      Orientation operator() (const Disk& /*o*/, 
			      const Bitangent_2& /*b*/) const{ return COLLINEAR; }
	Orientation operator() (const Bitangent_2& a, 
				const Bitangent_2& b) const
	{ return orientation(a.source(),a.target(),b.target()); } 
    };
    // -------------------------------------------------------------------------
    // Detection of degenerate cases
    struct Equal_as_segments {
	bool operator() (const Bitangent_2& a, const Bitangent_2& b) const {
	    return (a.source() == b.source() && a.target() == b.target());
	}
    };
    struct Is_point {
	bool operator() (const Disk& c) const { return true; }
    };
    // -------------------------------------------------------------------------
    struct Do_intersect {
	bool operator()(const Disk& o1, const Disk& o2) {
	    return (o1 == o2);
	}
	bool operator()(const Bitangent_2& o1, const Disk& o2) {
	    return false;		
	}
	bool operator()(const Disk& o1, const Bitangent_2& o2) {
	    return false;
	}
	bool operator()(const Bitangent_2& b1, const Bitangent_2& b2) {
	    return false;
	}
    };
    // -------------------------------------------------------------------------
};
// ----------------------------------------------------------------------------- 

CGAL_END_NAMESPACE

#endif // VISIBILITY_COMPLEX_POINT_TRAITS_H
