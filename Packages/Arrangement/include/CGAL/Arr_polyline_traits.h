// ======================================================================
//
// Copyright (c) 1999 The CGAL Consortium
//
// This software and related documentation is part of an INTERNAL release
// of the Computational Geometry Algorithms Library (CGAL). It is not
// intended for general use.
//
// ----------------------------------------------------------------------
//
// release       : $CGAL_Revision: CGAL-2.3-I-44 $
// release_date  : $CGAL_Date: 2001/03/09 $
//
// file          : include/CGAL/Arr_polyline_traits.h
// package       : Arrangement (1.77)
// maintainer    : Eyal Flato <flato@math.tau.ac.il>
// author(s)     : Iddo Hanniel
// coordinator   : Tel-Aviv University (Dan Halperin <halperin@math.tau.ac.il>)
//
// ======================================================================
#ifndef CGAL_ARR_POLYLINE_TRAITS_H
#define CGAL_ARR_POLYLINE_TRAITS_H

#include <CGAL/basic.h>

#include <CGAL/predicates_on_points_2.h>
#include <CGAL/predicates_on_lines_2.h>
#include <CGAL/Segment_2_Segment_2_intersection.h>

#include <CGAL/Point_2.h>
#include <CGAL/squared_distance_2.h>

#include <CGAL/Pm_segment_traits_2.h>
#include <CGAL/tags.h>

#include <list>
#include <deque>
#include <vector>

#include <algorithm>

CGAL_BEGIN_NAMESPACE

template <class Kernel_,
  class Container  = std::vector<CGAL_TYPENAME_MSVC_NULL Kernel_::Point_2 > >
class Arr_polyline_traits : public Kernel_
{
public:
  typedef Tag_false                     Has_left_category;
    
  typedef Kernel_                       Kernel;
  typedef Arr_polyline_traits<Kernel>   Self;

  typedef typename Kernel::Point_2      Point_2;
  typedef Container                     Curve_2;
  typedef Container                     X_monotone_curve_2;

  typedef typename Kernel::Vector_2     Vector_2;
  typedef typename Kernel::Line_2       Line_2;

  // Obsolete, for backward compatibility
  typedef Point_2                       Point;
  typedef Vector_2                      Vector;
  typedef X_monotone_curve_2            X_curve;
  typedef Curve_2                       Curve;

protected:
  typedef typename Kernel::Segment_2    Segment_2;

protected:
  // Functors:
  typedef typename Kernel::Is_vertical_2        Is_vertical_2;
  typedef typename Kernel::Construct_vertex_2   Construct_vertex_2;
  typedef typename Kernel::Less_x_2             Less_x_2;
  typedef typename Kernel::Equal_2              Equal_2;

public:
  Arr_polyline_traits() { }

  /*! compare_x() compares the x-coordinates of two given points
   * \param p1 the first point
   * \param p2 the second point
   * \return LARGER if x(p1) > x(p2), SMALLER if x(p1) < x(p2), or else EQUAL
   *
   * \todo replace indirect use compare_x() with compare_x_2()
   */
  Comparison_result compare_x(const Point_2 & p1, const Point_2 & p2) const
  {
    return compare_x_2_object()(p1, p2);
  }

  /*! compare_xy() compares the two given points by x, then by y
   * \param p1 the first point
   * \param p2 the second point
   * \return LARGER if x(p1) > x(p2) or if x(p1) = x(p2) and y(p1) > y(p2); 
   *         SMALLER if x(p1) < x(p2) or if x(p1) = x(p2) and y(p1) < y(p2); 
   *         or else EQUAL
   *
   * \todo replace indirect use compare_xy() with compare_xy_2()
   */
  Comparison_result compare_xy(const Point_2 & p1, const Point_2 & p2) const
  {
    return compare_xy_2_object()(p1, p2);;
  }

  /*! curve_is_vertical
   * on X_curve only - not Curve!
   */
  bool curve_is_vertical(const X_monotone_curve_2 & cv) const
  {
    CGAL_assertion(is_x_monotone(cv));
    return compare_x(curve_source(cv), curve_target(cv)) == EQUAL;
  } 

  /*! point_in_x_range
   */
  bool point_in_x_range(const X_monotone_curve_2 & cv, const Point_2 & p) const
  {
    CGAL_assertion(is_x_monotone(cv));

    Construct_vertex_2 construct_vertex = construct_vertex_2_object();
    const Point_2 & source = curve_source(cv);
    const Point_2 & target = curve_target(cv);
    Less_x_2 less_x = less_x_2_object();
    return !((less_x(source, p) && less_x(target, p)) ||
             (less_x(p, source) && less_x(p, target)));
  }

  Comparison_result curves_compare_y_at_x(const X_monotone_curve_2 & cv1, 
				       const X_monotone_curve_2 & cv2, 
				       const Point_2 & p) const
  {
    CGAL_assertion(is_x_monotone(cv1));
    CGAL_assertion(is_x_monotone(cv2));

    CGAL_precondition(point_in_x_range(cv1,p));
    CGAL_precondition(point_in_x_range(cv2,p));

    typename X_monotone_curve_2::const_iterator pit_1   = cv1.begin(),
      pit_2   = cv2.begin();
    typename X_monotone_curve_2::const_iterator after_1 = pit_1,
      after_2 = pit_2;
    ++after_1; ++after_2;

    // read: as long as both *pit and *after are on the same
    //       side of p 
    for( ; (compare_x(*pit_1,p) * compare_x(*after_1,p)) /*<=*/ >0 ; 
	 ++pit_1,++after_1) {}
    for( ; (compare_x(*pit_2,p) * compare_x(*after_2,p)) > 0 ; 
	 ++pit_2,++after_2) {}

    // the R here is the template parameter (see class def. above)
    Pm_segment_traits_2<Kernel> segment_traits;
    
    const typename Pm_segment_traits_2<Kernel>::X_monotone_curve_2
      seg1(*pit_1, *after_1),
      seg2(*pit_2, *after_2);

    // bug fix, shai 19/03/2000:
    // the polylines may contain vertical segments
    return (segment_traits.curves_compare_y_at_x(seg1, seg2, p));
  }

  // precondition - x-monotone
  Comparison_result curves_compare_y_at_x_left(const X_monotone_curve_2 & cv1, 
                                               const X_monotone_curve_2 & cv2,
                                               const Point_2 & p) const
  {
    CGAL_assertion(is_x_monotone(cv1));
    CGAL_assertion(is_x_monotone(cv2));

    // Check preconditions.
    CGAL_precondition(point_in_x_range(cv1,p));
    CGAL_precondition(point_in_x_range(cv2,p));
    CGAL_precondition(! curve_is_vertical(cv1));
    CGAL_precondition(! curve_is_vertical(cv2));
    
    CGAL_precondition_code(      
        Point_2 leftmost1 = 
	  (compare_x(curve_source(cv1),curve_target(cv1))==LARGER) ? 
	  curve_target(cv1) : curve_source(cv1);
	Point_2 leftmost2 = 
	  (compare_x(curve_source(cv2),curve_target(cv2))==LARGER) ? 
	  curve_target(cv2) : curve_source(cv2);
	);
    CGAL_precondition(compare_x(leftmost1,p) == SMALLER);
    CGAL_precondition(compare_x(leftmost2,p) == SMALLER);

    // Compare.
    typename X_monotone_curve_2::const_iterator pit = cv1.begin();
    typename X_monotone_curve_2::const_iterator after = pit; ++after;
    
    for( ; (compare_x(*pit,p) * compare_x(*after,p)) > 0 ; ++pit,++after) {}
    
    Line_2 l1(*pit,*after);
    
    pit=cv2.begin();
    after=pit; ++after;
    for( ; (compare_x(*pit,p) * compare_x(*after,p)) > 0 ; ++pit,++after) {}
    
    Line_2 l2(*pit,*after);
    
    CGAL_precondition (CGAL::compare_y_at_x(p,l1,l2) == EQUAL);
    
    // check if they are right endpoints (and compare to the one from 
    // the left) otherwise -
    // 
    //! \todo This is incorrect! we should probably return EQUAL.
    return (CGAL::compare_y_at_x(point_to_left(p),l1,l2)); 
  } 

  Comparison_result curves_compare_y_at_x_right(const X_monotone_curve_2& cv1, 
                                                const X_monotone_curve_2& cv2,
                                                const Point_2& p) const
  {
    CGAL_assertion(is_x_monotone(cv1));
    CGAL_assertion(is_x_monotone(cv2));

    // Check preconditions.
    CGAL_precondition(point_in_x_range(cv1,p));
    CGAL_precondition(point_in_x_range(cv2,p));
    CGAL_precondition(! curve_is_vertical(cv1));
    CGAL_precondition(! curve_is_vertical(cv2));
    
    CGAL_precondition_code(      
        Point_2 rightmost1 = 
	  (compare_x(curve_source(cv1),curve_target(cv1))==SMALLER) ?
	  curve_target(cv1) : curve_source(cv1);
	Point_2 rightmost2 = 
	  (compare_x(curve_source(cv2),curve_target(cv2))==SMALLER) ?
	  curve_target(cv2) : curve_source(cv2);
 	);
    CGAL_precondition(compare_x(rightmost1,p) == LARGER);
    CGAL_precondition(compare_x(rightmost2,p) == LARGER);

    // Compare.
    typename X_monotone_curve_2::const_iterator pit=cv1.begin();
    typename X_monotone_curve_2::const_iterator after=pit; ++after;
    
    for( ; (compare_x(*pit,p) * compare_x(*after,p)) > 0 ; ++pit,++after) {}
    
    Line_2 l1(*pit,*after);
    
    pit=cv2.begin();
    after=pit; ++after;
    for( ; (compare_x(*pit,p) * compare_x(*after,p)) > 0 ; ++pit,++after) {}

    Line_2 l2(*pit,*after);
    
    CGAL_precondition (CGAL::compare_y_at_x(p,l1,l2) == EQUAL);

    // check if they are left endpoints (and compare to the one from the 
    // right) otherwise -
    //! \todo This is incorrect! we should probably return EQUAL.
    return (CGAL::compare_y_at_x(point_to_right(p),l1,l2)); 
  }

  Comparison_result curve_compare_y_at_x (const Point_2& p,
                                          const X_monotone_curve_2 &cv) const
  {
    CGAL_assertion(is_x_monotone(cv));
    CGAL_precondition(point_in_x_range(cv, p));

    if (curve_is_vertical(cv))
    {
      if (CGAL::compare_y(p, curve_source(cv)) *
	  CGAL::compare_y(p, curve_target(cv)) <= 0)
        return EQUAL;
      else
	return (CGAL::compare_y(p, curve_source(cv)));
    }

    typename X_monotone_curve_2::const_iterator pit = cv.begin(),
      after = pit; ++after;
    while ( (compare_x(*pit,p) * compare_x(*after,p)) > 0 ) {
      ++pit; ++after;
    }
 
    Line_2 l(*pit,*after);
    return CGAL::compare_y_at_x(p, l);
  }
    
  /*! \todo replace indirect use point_equal() with equal_2()
   */
  bool point_equal(const Point_2 & p1, const Point_2 & p2) const
  { return equal_2_object()(p1, p2); }
  
  /*! \todo replace indirect use curve_equal() with equal_2()
   */
  bool curve_equal(const X_monotone_curve_2 & cv1,
                   const X_monotone_curve_2 & cv2) const
  {
    CGAL_assertion(is_x_monotone(cv1));
    CGAL_assertion(is_x_monotone(cv2));

    if (cv1.size() != cv2.size()) return false;

    typename X_monotone_curve_2::const_iterator it1 = cv1.begin();
    typename X_monotone_curve_2::const_iterator it2 = cv2.begin();
    Equal_2 equal = equal_2_object();
    bool is_equal = true;

    while (it1 != cv1.end()) {
      if (!equal(*it1++, *it2++)) {
        is_equal = false;
        break;
      }
    }
    if (is_equal) return true;

    // Reverse order:
    it1 = cv1.begin();
    it2 = cv2.end();
    while (it1 != cv1.end()) {
      if (!equal(*it1++, *--it2)) return false;
    }
    return true;
  }

  /*! 
   */
  Point_2 curve_source(const X_monotone_curve_2 & cv) const
  { return *(cv.begin()); }

  /*! 
   */
  Point_2 curve_target(const X_monotone_curve_2 & cv) const
  {
    typename X_monotone_curve_2::const_iterator it = cv.end();
    return *--it;
  }

  ///////////////////////////////////////////////////////
  //         ARRANGEMENT FUNCS


  X_monotone_curve_2 curve_opposite(const X_monotone_curve_2 & cv) const
  {
    X_monotone_curve_2 cv1(cv);
    std::reverse(cv1.begin(),cv1.end()); 
    return cv1; 
  }

  bool is_x_monotone(const Curve_2& cv) const {
    CGAL_assertion(cv.size()>=2); //one point is not a curve

    if (cv.size()==2) return true; //segment

    typename X_monotone_curve_2::const_iterator p0=cv.begin();
    typename X_monotone_curve_2::const_iterator p1=p0; ++p1;
    typename X_monotone_curve_2::const_iterator p2=p1; ++p2;

    for(; p2!=cv.end(); ++p0,++p1,++p2) {
      if ( compare_x(*p0,*p1) * compare_x(*p1,*p2) <=0 )
        return false;
    }

    // <= a matter of decision - only one vertical segment is considered 
    // x-monotone
    return true;

  }

  /*! Cut the given curve into x-monotone subcurves and insert them to the
   * given output iterator.
   * \param cv the curve.
   * \param o the output iterator
   * \return the past-the-end iterator
   */
  template<class OutputIterator>
  OutputIterator curve_make_x_monotone(const Curve_2 & cv,
                                       OutputIterator o)
  {
    CGAL_assertion(cv.size()>=2); //one point is not a curve

    if (cv.size()==2) { //segment
      *o++ = cv;
      return o;
    }

    typename X_monotone_curve_2::const_iterator p0=cv.begin();
    typename X_monotone_curve_2::const_iterator p1=p0; ++p1;
    typename X_monotone_curve_2::const_iterator p2=p1; ++p2;

    typename X_monotone_curve_2::const_iterator last_cut=p0;

    for(; p2!=cv.end(); ++p0,++p1,++p2) {
      //in future use constants instead of compare_x
      if (compare_x(*p0,*p1)==EQUAL) { //vertical segment - (p0,p1) 
        if (p0!=last_cut) { //needed for the case:
          //    | p1
          //    o p0=lastcut (was p1 before)
          //    |

          *o++ = X_monotone_curve_2(last_cut, p1); 
          //push_back the curve (last_cut...p0)
        }
        *o++ = X_monotone_curve_2(p0, p2); //push_back the segment (p0,p1)
        last_cut=p1;
      }
      else
        if ( compare_x(*p0,*p1) * compare_x(*p1,*p2) <= 0 ) {
          *o++ = X_monotone_curve_2(last_cut, p2);
          last_cut=p1;
        }
    }

    //push the residue (last cut to end)
    *o++ = X_monotone_curve_2(last_cut, p2);

    CGAL_assertion(p2==cv.end());
    return o;
  }


  void curve_split(const X_monotone_curve_2 & cv,
                   X_monotone_curve_2 & c1, X_monotone_curve_2 & c2, 
		   const Point_2 & split_pt)
  {
    //split curve at split point into c1 and c2
    CGAL_precondition(curve_compare_y_at_x(split_pt, cv) == EQUAL);
    CGAL_precondition(CGAL::compare_lexicographically_xy(curve_source(cv),
							 split_pt) != EQUAL);
    CGAL_precondition(CGAL::compare_lexicographically_xy(curve_target(cv),
							 split_pt) != EQUAL);

    typename X_monotone_curve_2::const_iterator p0=cv.begin();
    typename X_monotone_curve_2::const_iterator p1=p0; ++p1;
 
    bool split_at_vertex=false;

    for (; p1 != cv.end(); ++p0,++p1) {
      if (point_equal(*p1,split_pt)) {
	split_at_vertex=true;
	break;
      }
 
      if (compare_x(*p0,split_pt) * compare_x(split_pt,*p1) >= 0) {
	//in x range 
        break;
      }
    } 
    
    c1.clear(); c2.clear();

    typename X_monotone_curve_2::const_iterator ci=cv.begin();
    while (ci!=p1) {
      c1.push_back(*ci);
      ++ci;
    }

    if (!split_at_vertex) {
      c1.push_back(split_pt);
      c2.push_back(split_pt);
    }
    else {
      c1.push_back(*p1);
    }

    while (ci!=cv.end()) {
      c2.push_back(*ci);
      ++ci;
    }

    //moved this up to enable use of vector as container
    /*
      if (!split_at_vertex) {
      c1.push_back(split_pt);
      c2.push_front(split_pt);
      }
      else {
      c1.push_back(*p1);
      }
    */


  }

  //NOTE: when there is an overlap we will always return a SEGMENT (i.e.,
  //      p1 and p2 will be on a segment) even if the overlap is a polyline
  //      , this is still sufficient for the arrangement. might be
  //      changed in the future.
  bool nearest_intersection_to_right(const X_monotone_curve_2& cv1,
                                     const X_monotone_curve_2& cv2,
                                     const Point_2& pt,
                                     Point_2& p1,
                                     Point_2& p2) const 
  {      
    CGAL_assertion(is_x_monotone(cv1));
    CGAL_assertion(is_x_monotone(cv2));

    bool found( false);

    // curves do not necessarily intersect
    if ( ! _do_curves_intersect_to_right(cv1,cv2,pt)) 
      return false;

    X_monotone_curve_2 c1(cv1),c2(cv2);
    if ( ! lexicographically_xy_smaller (curve_source(c1),curve_target(c1)))
      c1=curve_opposite(cv1);
    if ( ! lexicographically_xy_smaller (curve_source(c2),curve_target(c2)))
      c2=curve_opposite(cv2);
      
    // check if both first points are left of pt, if they are reach the 
    // points directly left of pt, and check if their segments intersect  
    //to the right of pt, if not continue with a normal sweep until 
    //we find an intersection point or we reach the end.
      
    typename X_monotone_curve_2::const_iterator i1s=c1.begin(),i1e=c1.end();
    typename X_monotone_curve_2::const_iterator i1t=i1s; ++i1t;
      
    typename X_monotone_curve_2::const_iterator i2s=c2.begin(),i2e=c2.end();
    typename X_monotone_curve_2::const_iterator i2t=i2s; ++i2t;
      
    int number_to_left=0; //increment this variable if curve starts left of pt
      
    if (!lexicographically_xy_larger (*i1s,pt)) {
      //increment to nearest from the left of pt
      ++number_to_left;
      for (; i1t!=i1e; ++i1s,++i1t) {
	if (lexicographically_xy_larger (*i1t,pt)) break;
      }
      if (i1t==i1e) return false;
    }
      
    //now i1s holds the source vertex and i1t holds the target
      
    if (!lexicographically_xy_larger (*i2s,pt)) {
      //increment 
      ++number_to_left;
      for (; i2t!=i2e; ++i2s,++i2t) {
	if (lexicographically_xy_larger (*i2t,pt)) break;
      }
      if (i2t==i2e) return false ; //c2 ends to the left of pt
    }
      
    if (number_to_left==2) {
      //check if intersection exists and is lex larger
      Object result =
        intersection(Segment_2(*i1s, *i1t), Segment_2(*i2s, *i2t));
      Point_2 i_pt;
      Segment_2 i_seg;
      if (assign(i_pt,result)) {
	//check if intersection point to the right of pt
	if (lexicographically_xy_larger (i_pt,pt)) {
	  //debug
#ifndef ARR_USES_LEDA_RATIONAL  //normalize if we are with rational numbers
	  p1=p2=i_pt;
	  found =  true;
#else
	  p1=p2=Point_2(i_pt.x().normalize(),i_pt.y().normalize());
	  found = true;
#endif
	}
      }
       
      if ( ! found && assign(i_seg,result)) {
	//check if intersection seg to the right of pt
	if (lexicographically_xy_larger (i_seg.source(),i_seg.target()))
	  i_seg=i_seg.opposite();

	if (lexicographically_xy_larger (i_seg.target(),pt)) {
	  p2=i_seg.target();
	  if (lexicographically_xy_larger (i_seg.source(),pt)) {
	    p1=i_seg.source();}
	  else {
	    // p1=pt;
            // Modified by Eug
            // Performing vertical ray shooting from pt.
	    // Finding the intersection point. We know by now
	    // that there is exactly ONE point. Assinging this
	    // point to p1.
            
            // Bug: What happens if the curves are vertical. The vertical ray
            // shoot will return a segment instead of point.
            Point_2 ap1( pt.x(), i_seg.source().y() );
            Point_2 ap2( pt.x(), i_seg.target().y() );
            Segment_2 vertical_pt_x_base( ap1, ap2 );
            Object i_obj = intersection( vertical_pt_x_base, i_seg );
            //assign( p1, i_obj ); // Causes a bug if the result is a segment.
            
            // Bug fix: Eti.
            Point_2 tmp_p1;
            Segment_2 i_vertical;
            if ( assign( tmp_p1, i_obj ))
              p1=tmp_p1;
            else if ( assign(i_vertical, i_obj))
              // the intersection vertical segment starts at pt.
              p1=i_vertical.source();  
          }         
	  found = true;
	}
      }
        
      if ( ! found) {
	//advance to the nearer point
	if (lexicographically_xy_larger (*i2t,*i1t)) {
	  ++i1s; ++i1t;
	  if (i1t==i1e) return false;
	}
	else {
	  ++i2s; ++i2t;
	  if (i2t==i2e) return false;
	}
      }
    }
      
    //NOW we can start sweeping the chains
      
    while ( ! found) {
      //check intersection of the segments
        
      Object result;
        
      result = intersection(Segment_2(*i1s, *i1t), Segment_2(*i2s, *i2t));
        
      Point_2 i_pt;
      Segment_2 i_seg;
      if (assign(i_pt,result)) {
          
#ifndef ARR_USES_LEDA_RATIONAL  //normalize if we are with rational numbers
	p1=p2=i_pt;
#else
	p1=p2=Point_2(i_pt.x().normalize(),i_pt.y().normalize());
#endif
	found = true;
      }
        
       
      if (!found && assign(i_seg,result)) {
	//check if intersection seg to the right of pt
	if (lexicographically_xy_larger (i_seg.source(),i_seg.target()))
          i_seg=i_seg.opposite();

	if (lexicographically_xy_larger (i_seg.target(),pt)) {
	  p2=i_seg.target();
	  if (lexicographically_xy_larger (i_seg.source(),pt)) {
	    p1=i_seg.source();}
	  else {
	    // p1=pt;
            // Modified by Eug
            // Performing vertical ray shooting from pt.
	    // Finding the intersection point. We know by now
	    // that there is exactly ONE point. Assinging this
	    // point to p1.
            Point_2 ap1( pt.x(), i_seg.source().y() );
            Point_2 ap2( pt.x(), i_seg.target().y() );
            Segment_2 vertical_pt_x_base( ap1, ap2 );
            Object i_obj = intersection( vertical_pt_x_base, i_seg );
            //assign( p1, i_obj ); // Causes a bug if the result is a segment.

            // Bug fix: Eti.
            Point_2 tmp_p1;
            Segment_2 i_vertical;
            if ( assign( tmp_p1, i_obj ))
              p1=tmp_p1;
            else if ( assign(i_vertical, i_obj))
              // the intersection vertical segment starts at pt.
              p1=i_vertical.source();  

	  }    
	  found = true;
	}
      }
        

      if ( ! found) {
	//advance to the nearer point
	if (lexicographically_xy_larger (*i2t,*i1t)) {
	  ++i1s; ++i1t;
	  if (i1t==i1e) return false;
	}
	else {
	  ++i2s; ++i2t;
	  if (i2t==i2e) return false;
	}
      }
        
    } // end while ( ! found)
    
    // if the x point is at the end of a segment, then there might be 
    // an overlap in the continious of the polyline
    if ( found && point_equal(p1, p2) ) {
      typename X_monotone_curve_2::const_iterator s1 = i1s, t1 = i1t,
        s2 = i2s, t2 = i2t;
      s1++; t1++; s2++; t2++;
      if (t1 != i1e && t2 != i2e) {

	// check for overlap after x point
	Object result;
	Segment_2 i_seg;

	if ( point_equal(p1, *i1t) && point_equal(p1, *i2t)) {
	  result = intersection(Segment_2(*s1, *t1), Segment_2(*s2, *t2));
	}
	else if ( point_equal(p1, *i1t)) {
	  result = intersection(Segment_2(*s1, *t1), Segment_2(*i2s, *i2t));
	}
	else if ( point_equal(p1, *i2t)) {
	  result = intersection(Segment_2(*i1s, *i1t),
                                Segment_2(*s2, *t2));
	}
	if (assign(i_seg,result)) {
	  // no need to check whether intersection seg to the right of pt, 
	  // because we have already found an x point to the right of pt.
	  if (lexicographically_xy_larger (i_seg.source(),i_seg.target()))
	    i_seg=i_seg.opposite();
	  p1 = i_seg.source();
	  p2 = i_seg.target();
	}
      }  
    }

    return found;
  }


  bool curves_overlap(const X_monotone_curve_2& ca,
                      const X_monotone_curve_2& cb) const
  {
    CGAL_assertion(is_x_monotone(ca));
    CGAL_assertion(is_x_monotone(cb));

    //do a flip so they are left to right 
    X_monotone_curve_2 c1(ca),c2(cb);
    if (lexicographically_xy_larger(curve_source(ca),curve_target(ca)))
      c1=curve_opposite(ca);
    if (lexicographically_xy_larger(curve_source(cb),curve_target(cb)))
      c2=curve_opposite(cb);

    typename X_monotone_curve_2::const_iterator i1s=c1.begin(),i1e=c1.end();
    typename X_monotone_curve_2::const_iterator i1t=i1s; ++i1t;

    typename X_monotone_curve_2::const_iterator i2s=c2.begin(),i2e=c2.end();
    typename X_monotone_curve_2::const_iterator i2t=i2s; ++i2t;

    //now i1s holds the source vertex and i1t holds the target
    Point_2 i_pt;
    Segment_2 i_seg;
    Segment_2 s1(*i1s,*i1t),s2(*i2s,*i2t);
    Object res=intersection(s1,s2);
    
    if (assign(i_seg,res)) {
      if (!point_equal(i_seg.source(),i_seg.target()))
	return true;
    }
    //advance to the nearer point
    if (lexicographically_xy_larger(*i2t,*i1t)) {
      ++i1s; ++i1t;
      if (i1t==i1e) return false;
    }
    else {
      ++i2s; ++i2t;
      if (i2t==i2e) return false;
    }

    //NOW we can start sweeping the chains

    while (1) {
      Segment_2 i_seg;
      Segment_2 s1(*i1s,*i1t),s2(*i2s,*i2t);
      res=intersection(s1,s2);
      if (assign(i_seg,res)) {
	if (!point_equal(i_seg.source(),i_seg.target()))
	  return true;
      }

      if (lexicographically_xy_larger(*i2t,*i1t)) {
        ++i1s; ++i1t;
        if (i1t==i1e) return false;
      }
      else {
        ++i2s; ++i2t;
        if (i2t==i2e) return false;
      }
    }    
  }

  /*! point_reflect_in_x_and_y() reflects the given point about the origin
   */
  Point_2 point_reflect_in_x_and_y(const Point_2 & pt) const
  {
    Point_2 org = construct_point_2_object()(ORIGIN);      
    Vector_2 v = construct_vector_2_object()(pt, org);
    Point_2 reflected_pt(v);
    return reflected_pt;
  }

  /*!
   */
  X_monotone_curve_2
  curve_reflect_in_x_and_y(const X_monotone_curve_2 & cv) const
  {
    X_monotone_curve_2 reflected_cv;
    typename Curve_2::const_iterator it = cv.begin();
    while (it != cv.end())
      reflected_cv.push_back(point_reflect_in_x_and_y(*it++)); 
    return reflected_cv;
  }

  ////////////////////////////////////////////////////////////////////
  // PRIVATE
private:

  //! \todo Remove the point_to_left() and point_to_right() functions.
  Point_2 point_to_left(const Point_2& p) const
  { return p+Vector_2(-1,0); }

  Point_2 point_to_right(const Point_2& p) const
  { return p+Vector_2(1,0); }

  //returns true iff the intersectionis lexicographically strictly right of pt
  bool _do_curves_intersect_to_right (const X_monotone_curve_2& ca, 
				      const X_monotone_curve_2& cb,
				      const Point_2& pt) const 
  {
    CGAL_assertion(is_x_monotone(ca));
    CGAL_assertion(is_x_monotone(cb));


    // check if both first points are left of pt, if they are reach the 
    // points directly left of pt, and check if their segments intersect  
    //to the right of pt, if not continue with a normal sweep until 
    //we find an intersection point or we reach the end.
    
    //do a flip or can we assume they are left to right ??
    X_monotone_curve_2 c1(ca),c2(cb);
    if (lexicographically_xy_larger(curve_source(ca), curve_target(ca)) ==
	LARGER )
      c1=curve_opposite(ca);
    if (lexicographically_xy_larger(curve_source(cb),curve_target(cb)) == 
	LARGER )
      c2=curve_opposite(cb);

    typename X_monotone_curve_2::const_iterator i1s=c1.begin(),i1e=c1.end();
    typename X_monotone_curve_2::const_iterator i1t=i1s; ++i1t;

    typename X_monotone_curve_2::const_iterator i2s=c2.begin(),i2e=c2.end();
    typename X_monotone_curve_2::const_iterator i2t=i2s; ++i2t;

    int number_to_left=0; //increment this variable if curve starts left of pt

    if (!lexicographically_xy_larger (*i1s,pt)) {

      //increment to nearest from the left of pt
      ++number_to_left;
      for (; i1t!=i1e; ++i1s,++i1t) {
	if (lexicographically_xy_larger (*i1t,pt)) break;
      }
      if (i1t==i1e) return false; //c1 ends to the left of pt
    }
    //now i1s holds the source vertex and i1t holds the target

    if (!lexicographically_xy_larger (*i2s,pt)) {
      //increment 
      ++number_to_left;
      for (; i2t!=i2e; ++i2s,++i2t) {
	if (lexicographically_xy_larger (*i2t,pt)) break;
      }
      if (i2t==i2e) return false; //c2 ends to the left of pt
    }

    if (number_to_left==2) {
      //check if intersection exists and is lex larger
      Object result;
      Point_2 i_pt;
      Segment_2 i_seg;
      
      result = intersection(Segment_2(*i1s, *i1t), Segment_2(*i2s, *i2t));
      if (assign(i_pt,result)) {
        //check if intersection point to the right of pt
        if (lexicographically_xy_larger (i_pt,pt)) 
	  {
	    return true;
	  }
      }
      else

	if (assign(i_seg,result)) {
	  //check if intersection seg to the right of pt
	  if (lexicographically_xy_larger (i_seg.source(),pt) ||
	      lexicographically_xy_larger (i_seg.target(),pt))
	    {
	      return true;
	    }
	}
      //debug
	else {
	  //cerr << "segments don't intersect ??" << endl;
	}
      //advance to the nearer point
      if (lexicographically_xy_larger (*i2t,*i1t)) {
	++i1s; ++i1t;
        if (i1t==i1e) return false;
      }
      else {
	++i2s; ++i2t;
        if (i2t==i2e) return false;
      }

    }
    //NOW we can start sweeping the chains

    while (1) {
      //check for intersection of the segments
      if (do_intersect(Segment_2(*i1s, *i1t), Segment_2(*i2s, *i2t)))
	{
	  return true;
	}
      
      //advance to the nearer point
      if (lexicographically_xy_larger (*i2t,*i1t)) {
	++i1s; ++i1t;
        if (i1t==i1e) return false;
      }
      else {
	++i2s; ++i2t;
        if (i2t==i2e) return false;
      }
    }

  }

public:
  void display(const X_monotone_curve_2& cv) const
  {
    typename X_monotone_curve_2::const_iterator it=cv.begin(),eit=cv.end();
    while(it!=eit) { std::cerr << *it++;}
  }
  
  //the same for window stream
};

CGAL_END_NAMESPACE

#endif
