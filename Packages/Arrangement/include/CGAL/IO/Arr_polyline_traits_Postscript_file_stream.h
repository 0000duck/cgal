// ======================================================================
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
// file          : include/CGAL/IO/Arr_polyline_traits_Postscript_file_stream.h
// package       : Arrangement (1.81)
// maintainer    : Eyal Flato <flato@math.tau.ac.il>
// source        : 
// revision      : 
// revision_date : 
// author(s)     : Eti Ezra <estere@post.tau.ac.il>
//
//
// coordinator   : Tel-Aviv University (Dan Halperin <halperin@math.tau.ac.il>)
//
// Chapter       : 
// ======================================================================

#ifdef CGAL_ARR_POLYLINE_TRAITS_H
#ifndef CGAL_ARR_POLYLINE_TRAITS_POSTSCRIPT_FILE_STREAM_H   
#define CGAL_ARR_POLYLINE_TRAITS_POSTSCRIPT_FILE_STREAM_H  

#include <CGAL/Segment_2.h> 

#ifndef CGAL_POSTSCRIPT_FILE_STREAM_H
#include <CGAL/IO/Postscript_file_stream.h>
#endif

CGAL_BEGIN_NAMESPACE

template <class Curve>
Postscript_file_stream& operator<<(Postscript_file_stream& ps, const Curve& cv)        
{
  typedef typename Curve::value_type           Point;
  typedef typename Curve::iterator             Curve_iter;
  typedef typename Point::R                    R;
  typedef CGAL::Segment_2<R>                   Segment;
  typedef typename Curve::const_iterator       Points_iterator;
  
  ps << *(cv.begin());
  
  Points_iterator points_iter;
  for (points_iter = cv.begin(); points_iter != cv.end(); ) {
    //for (unsigned int i = 0; i < cv.size() - 1; i++){
    Points_iterator source_point = points_iter;
    
    Points_iterator target_point =  (++points_iter);
    
    if (target_point == cv.end())
      break;
    
    ps << Segment(*source_point, *target_point);
  }

  ps << *(--points_iter);

  return ps;
 
  /*typedef typename Curve::value_type           Point;
    typedef typename Point::R                    R;
    typedef CGAL::Segment_2<R>          Segment;
    
    ps << cv[0];
    
    for (unsigned int i = 0; i < cv.size() - 1; i++){
    ps << Segment(cv[i], cv[i+1]);
    }
    
    ps << cv[cv.size() - 1];
    
    return ps;*/
}

CGAL_END_NAMESPACE

#endif
#endif 









