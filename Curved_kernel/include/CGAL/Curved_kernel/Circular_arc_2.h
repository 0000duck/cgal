// Copyright (c) 2003-2006  INRIA Sophia-Antipolis (France).
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
// Author(s)     : Monique Teillaud, Sylvain Pion

// Partially supported by the IST Programme of the EU as a Shared-cost
// RTD (FET Open) Project under Contract No  IST-2000-26473 
// (ECG - Effective Computational Geometry for Curves and Surfaces) 
// and a STREP (FET Open) Project under Contract No  IST-006413 
// (ACS -- Algorithms for Complex Shapes)

#ifndef CGAL_CURVED_KERNEL_CIRCULAR_ARC_2_H
#define CGAL_CURVED_KERNEL_CIRCULAR_ARC_2_H

#include <CGAL/global_functions_on_circular_arcs_2.h>
#include <CGAL/Curved_kernel/internal_functions_on_circular_arc_2.h> // temporarily
#include <CGAL/intersections.h>
#include <CGAL/Simple_cartesian.h>

namespace CGAL {
namespace CGALi {

  template <class CK >
  class Circular_arc_2
  {
    typedef typename CK::FT                        FT;
    typedef typename CK::RT                        RT;
    typedef typename CK::Point_2                   Point_2;
    typedef typename CK::Line_2                    Line_2;
    typedef typename CK::Circle_2                  Circle_2;
    typedef typename CK::Circular_arc_point_2      Circular_arc_point_2;
    typedef typename CK::Root_of_2                 Root_of_2;
    typedef struct bit_field {
      unsigned short int is_full:2;
      unsigned short int Cache_minmax:2;
      unsigned short int is_x_monotonic:2;
      unsigned short int is_y_monotonic:2;
      unsigned short int two_end_points_on_upper_part:2;
      unsigned short int two_end_points_on_left_part:2;
      unsigned short int is_complementary_x_monotone:1;
      unsigned short int is_complementary_y_monotone:1;
    } bit_field;

  private:

  // set flags to 0
  // when 1 bit -> 0 = false, 1 = true
  // when 2 bits -> 0 = don_know, 1 = false or 's' (Cache_minmax), 
  //                              2 = true or 't' (Cache_minmax)
  void reset_flags() {
    flags.is_full = 0;
    flags.Cache_minmax = 0;
    flags.is_x_monotonic = 0;
    flags.is_y_monotonic = 0;
    flags.two_end_points_on_upper_part = 0;
    flags.two_end_points_on_left_part = 0;
    flags.is_complementary_x_monotone = 0;
    flags.is_complementary_y_monotone = 0;
  }

  public:

    Circular_arc_2() {}

    Circular_arc_2(const Circle_2 &c)
      : _support(c)
    {
      reset_flags();           // example
      flags.is_full = 2;       // is_full = true
      flags.Cache_minmax = 1;  // Cache_minmax = 's'
      _begin = _end  = 
	CircularFunctors::x_extremal_point<CK>(supporting_circle(),true); 
      
    }

    Circular_arc_2(const Circle_2 &support,
                   const Line_2 &l1, bool b1,
                   const Line_2 &l2, bool b2)
    {
      Point_2 center1 (support.center().x() + l1.a()/2,
                       support.center().y() + l1.b()/2);

      FT sqr1 = support.squared_radius() + l1.c()
	        - CGAL::square(support.center().x())
	        - CGAL::square(support.center().y())
	        + CGAL::square(center1.x())
	        + CGAL::square(center1.y());

      Circle_2 c1 (center1, sqr1);
      
      Point_2 center2 (support.center().x() + l2.a()/2,
                       support.center().y() + l2.b()/2);

      FT sqr2 = support.squared_radius() + l2.c()
	        - CGAL::square(support.center().x())
	        - CGAL::square(support.center().y())
	        + CGAL::square(center2.x())
	        + CGAL::square(center2.y());

      Circle_2 c2 (center2, sqr2);
      
      *this = Circular_arc_2(support, c1, b1, c2, b2);

      CGAL_kernel_assertion(do_intersect(support, c1));
      CGAL_kernel_assertion(do_intersect(support, c2));
    }


    Circular_arc_2(const Circle_2 &c,
		   const Circle_2 &c1, const bool b_1,
		   const Circle_2 &c2, const bool b_2)
      : _support(c)
    {
      reset_flags();
      if (c1 != c2) {
	_begin = CGAL::circle_intersect<CK>(c, c1, b_1);
	_end = CGAL::circle_intersect<CK>(c, c2, b_2);
      }
      else{
	typedef std::vector<CGAL::Object > solutions_container;
	
	solutions_container solutions;
	CGAL::intersect_2<CK>( c, c1, std::back_inserter(solutions) );
	typename solutions_container::iterator it = solutions.begin();

	CGAL_kernel_precondition( it != solutions.end() );
	// the circles intersect
	
	const std::pair<typename CK::Circular_arc_point_2, unsigned> *result;
	result = CGAL::object_cast<
	  std::pair<typename CK::Circular_arc_point_2, unsigned> > (&(*it));
	if ( result->second == 2 ){ // double solution
	  _begin =  result->first;
	  _end = result->first;
	}
	else{
	  if (b_1)
	    _begin = result->first;
	  if (b_2)
	    _end = result->first;
	  if (!(b_1 & b_2)) {
	    ++it;
	    result = CGAL::object_cast<
	      std::pair<typename CK::Circular_arc_point_2, unsigned> >(&(*it));
	    if (!b_1)
	      _begin = result->first;
	    if (!b_2)
	      _end = result->first;
	  }
	}
      }
    }

    // IS THIS CONSTRUCTOR USED ?
    // constructs a circular arc that is the arc included in A
    // having same (b) endpoint as A (true == _begin, false == _end)
    // but whose (!b) endpoint is the intersection of A with ccut given 
    // by b_cut
    Circular_arc_2(const Circular_arc_2 &A, const bool b,
		   const Circle_2 &ccut, const bool b_cut)
      : _support(A.supporting_circle())
    {
      reset_flags();
      CGAL_kernel_precondition(A.is_x_monotone());
      CGAL_kernel_precondition(do_intersect(A.supporting_circle(), ccut));
      
      Circular_arc_point_2 new_p =
	CGAL::circle_intersect<CK>(A.supporting_circle(), ccut, b_cut);
      
      // CGAL_kernel_assertion(point_in_range(A, new_p));
      CGAL_kernel_assertion
	(A.on_upper_part() == (CGAL::compare(new_p.y(), A.center().y()) >= 0));

      if (b) {
	_begin  = A._begin;
	_end    = new_p;
      }
      else {
	_begin  = new_p;
	_end    = A._end;
      }
    }

    // Constructs an arc supported by Circle_2(begin, middle, end),
    // with _begin == begin, _end == end.
    // (middle is not necessarily on the arc)
    Circular_arc_2(const Point_2 &begin,
                   const Point_2 &middle,
                   const Point_2 &end)
      : _begin(begin), _end(end)
    {
      reset_flags();
      CGAL_kernel_precondition(!CGAL::collinear(begin, middle, end));
      _support = Circle_2(begin, middle, end);
      /*
       *  Circle_2 c = Circle_2(begin, middle, end);
       * Line_2   l1 (begin, middle);
      Line_2   l2 (middle, end);
      *this = Circular_arc_2(c, 
			     l1, compare_xy(begin, middle) < 0,
			     l2, compare_xy(end,   middle) < 0);*/
	  //std::cout << source() << std::endl;
	  //std::cout << target() << std::endl;
    }

    Circular_arc_2(const Circle_2 &support,
		   const Circular_arc_point_2 &source,
		   const Circular_arc_point_2 &target)
      : _begin(source), _end(target), _support(support)
    {
      reset_flags();
      // We cannot enable these preconditions for now, since the 
      // Lazy_curved_kernel
      // calls it on the Interval kernel without try/catch protection
      // through the Curved_kernel_converter.
      // CGAL_kernel_exactness_precondition(CK().has_on_2_object()(support, source));
      // CGAL_kernel_exactness_precondition(CK().has_on_2_object()(support, target));
    }
    
    Circular_arc_2(const Point_2 &begin,
                   const Point_2 &end,
		   const FT &bulge)
      : _begin(begin), _end(end)
    {
      reset_flags();
      const FT sqr_bulge = CGAL::square(bulge);
      const FT common = (FT(1) - sqr_bulge) / (FT(4)*bulge);
      const FT x_coord = (begin.x() + end.x())/FT(2)
	                 + common*(begin.y() - end.y());
      const FT y_coord = (begin.y() + end.y())/FT(2)
                          + common*(end.x() - begin.x());
      
      const FT sqr_rad = squared_distance(begin, end) 
	                 * (FT(1)/sqr_bulge + FT(2) + sqr_bulge) / FT(16); 

      _support = Circle_2(Point_2(x_coord, y_coord), sqr_rad);
    }
    
  private:
    // The arc goes from _begin to _end in the positive order
    // If _begin == _end, then it's the full circle
    Circular_arc_point_2  _begin, _end;
    Circle_2 _support;
    mutable bit_field flags;

  public:
    // to remember if the arc was constructed from a full circle
    
    const Circular_arc_point_2 & left() const
    {
      CGAL_kernel_precondition(is_x_monotone());
      CGAL_kernel_precondition(on_upper_part() ? compare_xy(_end,_begin)<0
			       : compare_xy(_begin,_end)<0);
      if (flags.Cache_minmax == 1)
	return _end;
      if (flags.Cache_minmax == 2)
	return _begin;
      if (on_upper_part()) {
        flags.Cache_minmax = 1;
	return _end;
      }
      flags.Cache_minmax = 2;
      return  _begin;
    }

    const Circular_arc_point_2 & right() const
    {
      CGAL_kernel_precondition(is_x_monotone());
      CGAL_kernel_precondition(on_upper_part() ? compare_xy(_end,_begin)<0
			       : compare_xy(_begin,_end)<0);
      if (flags.Cache_minmax == 1)
	return _begin;
      if (flags.Cache_minmax == 2)
	return _end;
      if (on_upper_part()) {
	flags.Cache_minmax = 1;
	return _begin;
      }
      flags.Cache_minmax = 2;
      return  _end;
    }

    const Circular_arc_point_2 & source() const
    {
      return _begin;
    }
    
    const Circular_arc_point_2 & target() const
    {
      return _end;
    }
    
    inline const bool is_full() const {
      return flags.is_full == 2;
    }

private:

    bool _is_x_monotone() const {
      if (is_full()) return false;
      
      int cmp_begin = CGAL::compare(_begin.y(), center().y());
      int cmp_end   = CGAL::compare(_end.y(),   center().y());

      // XXX : be careful, this may be surprising if the return value
      // is not -1/1 but some random int...

      if (cmp_begin == opposite(cmp_end) && cmp_begin != 0)
        return false;

      // Maybe the complementar is x_monotone
      // but we have to go further to know
      // see is_x_monotone()
      flags.is_complementary_x_monotone = 1;

      int cmp_x = compare_x(_begin, _end);
      
      // Is the arc on the upper part ?
      if (cmp_begin > 0 || cmp_end > 0)
        return cmp_x > 0;

      // Is the arc on the lower part ?
      if (cmp_begin < 0 || cmp_end < 0)
        return cmp_x < 0;

      // There remains the case :
      CGAL_kernel_assertion(cmp_begin == 0 && cmp_end == 0);

      return cmp_x != 0; // full circle or half circle.
    }

    bool _is_y_monotone() const {
      if (is_full()) return false;

      int cmp_begin = CGAL::compare(_begin.x(), center().x());
      int cmp_end   = CGAL::compare(_end.x(),   center().x());
      
      // XXX : be careful, this may be surprising if the return value
      // is not -1/1 but some random int...
      if (cmp_begin == opposite(cmp_end) && cmp_begin != 0)
        return false;

      // Maybe the complementar is y_monotone
      // but we have to go further to know
      // see is_y_monotone()
      flags.is_complementary_y_monotone = 1;

      int cmp_y = compare_y(_begin, _end);
      
      // Is the arc on the right part ?
      if (cmp_begin > 0 || cmp_end > 0)
        return cmp_y < 0;

      // Is the arc on the left part ?
      if (cmp_begin < 0 || cmp_end < 0)
        return cmp_y > 0;
      
      // There remains the case :
      assert(cmp_begin == 0 && cmp_end == 0);

      return cmp_y != 0; // full circle or half circle.
    }

    bool _two_end_points_on_upper_part() const {
      int c1y = CGAL::compare(_begin.y(),
        supporting_circle().center().y());
      if(c1y > 0) return true;
      if(c1y < 0) return false;
      int c2y = CGAL::compare(_end.y(),
        supporting_circle().center().y());
      if(c2y > 0) return true;
      if(c2y < 0) return false;
      return compare_x(_begin, _end) > 0;
    }

    bool _two_end_points_on_left_part() const
    { 
      int c1x = CGAL::compare(_begin.x(), 
        supporting_circle().center().x());
      if(c1x < 0) return true;
      if(c1x > 0) return false;
      int c2x = CGAL::compare(_end.x(), 
        supporting_circle().center().x());
      if(c2x < 0) return true;
      if(c2x > 0) return false;
      return compare_y(_begin, _end) > 0;
    }

public:

    bool is_x_monotone() const {
      if(flags.is_x_monotonic == 0) {
        bool b = _is_x_monotone();
        if(b) { 
          flags.is_x_monotonic = 2;
          flags.is_complementary_x_monotone = 0;
        } else flags.is_x_monotonic = 1;
        return b;
      } else {
        return (flags.is_x_monotonic == 1) ? false : true;
      }
    } 

    // Returns true if the complementary arc is x_monotone()
    bool is_complementary_x_monotone() const {
      // is_x_monotone calculates also if 
      // the complementary is x-monotone if needed
      is_x_monotone();
      return (flags.is_complementary_x_monotone == 0) ? false : true;
    } 

    bool is_y_monotone() const {
      if(flags.is_y_monotonic == 0) {
        bool b = _is_y_monotone();
        if(b) {
          flags.is_y_monotonic = 2;
          flags.is_complementary_y_monotone = 0;
        } else flags.is_y_monotonic = 1;
        return b;
      } else {
        return (flags.is_y_monotonic == 1) ? false : true;
      }
    }

    // Returns true if the complementary arc is y_monotone()
    bool is_complementary_y_monotone() const {
      // is_y_monotone calculates also if 
      // the complementary is y-monotone if needed
      is_y_monotone();
      return (flags.is_complementary_y_monotone == 0) ? false : true;
    }

    // check whether 2 endpoints are at upper or not from the center 
    bool two_end_points_on_upper_part() const {
      if(flags.two_end_points_on_upper_part == 0) {
        bool b = _two_end_points_on_upper_part();
        if(b) flags.two_end_points_on_upper_part = 2;
        else flags.two_end_points_on_upper_part = 1;
        return b;
      } else {
        return (flags.two_end_points_on_upper_part == 1) ? false : true;
      }
    }

    // check whether the arc is at upper or not from the center 
    bool on_upper_part() const {
      CGAL_kernel_precondition(is_x_monotone());
      return two_end_points_on_upper_part();
    }

    // Returns true if the complementary arc is on_upper_part()
    bool complementary_on_upper_part() const {
      if(is_x_monotone()) return false;
      return two_end_points_on_upper_part();
    }

    // check whether the 2 endpoints are at left or right from the center
    bool two_end_points_on_left_part() const {
      if(flags.two_end_points_on_left_part == 0) {
        bool b = _two_end_points_on_left_part();
        if(b) flags.two_end_points_on_left_part = 2;
        else flags.two_end_points_on_left_part = 1;
        return b;
      } else {
        return (flags.two_end_points_on_left_part == 1) ? false : true;
      }
    }

    // check whether the arc is at left or right from the center 
    bool on_left_part() const {      
      CGAL_kernel_precondition(is_y_monotone());
      return two_end_points_on_left_part();
    }

    // Returns true if the complementary arc is on_left_part()
    bool complementary_on_left_part() const {
      if(is_y_monotone()) return false;
      return two_end_points_on_left_part();
    }

    const Circle_2 & supporting_circle() const
    {
       return _support;
    }

    const Point_2 & center() const           
    {
       return supporting_circle().center();
    }

    const FT & squared_radius() const           
    {
       return supporting_circle().squared_radius();
    }

    Bbox_2 bbox() const
    {
      return CGAL::CircularFunctors::circular_arc_bbox<CK>(*this);
    }    
    
  };

  template < typename CK >
  std::ostream &
  operator<<(std::ostream & os, const Circular_arc_2<CK> &a)
  {
    // The output format is :
    // - supporting circle
    // - circle c1
    // - bool b1
    // - circle c2
    // - bool b2
    return os << a.supporting_circle() << " "
	      << a.source() << " "
	      << a.target() << " ";
  }

  template < typename CK >
  std::istream &
  operator>>(std::istream & is, Circular_arc_2<CK> &a)
  {
    typename CK::Circle_2 s;
    typename CK::Circular_arc_point_2 p1;
    typename CK::Circular_arc_point_2 p2;
    is >> s >> p1 >> p2 ;
    if (is)
      a = Circular_arc_2<CK>(s, p1, p2);
    return is;
  }

  template < typename CK >
  std::ostream &
  print(std::ostream & os, const Circular_arc_2<CK> &a)
  {
    if(a.is_x_monotone()) {
      return os << "Circular_arc_2( " << std::endl
                << "left : " << a.left() << " , " << std::endl
                << "right : " << a.right() << " , " << std::endl
                << "upper part : " << a.on_upper_part() << std::endl
                << "  [[ approximate circle is (x,y,r) : "
                << CGAL::to_double(a.supporting_circle().center().x()) << ""
                << CGAL::to_double(a.supporting_circle().center().y()) << ""
                << std::sqrt(CGAL::to_double(a.supporting_circle().squared_radius()))
                << " ]])" << std::endl;
    } else {
      return os << "Circular_arc_2( " << std::endl
                << "  [[ approximate circle is (x,y,r) : "
                << CGAL::to_double(a.supporting_circle().center().x()) << ""
                << CGAL::to_double(a.supporting_circle().center().y()) << ""
                << std::sqrt(CGAL::to_double(a.supporting_circle().squared_radius()))
                << " ]])" << std::endl;
    }
  }     

} // namespace CGALi
} // namespace CGAL

#endif // CGAL_CURVED_KERNEL_CIRCULAR_ARC_2_H
