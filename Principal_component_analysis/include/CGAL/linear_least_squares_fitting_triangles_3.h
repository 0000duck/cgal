// Copyright (c) 2005  INRIA Sophia-Antipolis (France).
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
// $URL: svn+ssh://gankit@scm.gforge.inria.fr/svn/cgal/trunk/Principal_component_analysis/include/CGAL/linear_least_squares_fitting_triangles_3.h $
// $Id: linear_least_squares_fitting_2.h 37882 2007-04-03 15:15:30Z spion $
//
// Author(s) : Pierre Alliez and Sylvain Pion and Ankit Gupta

#ifndef CGAL_LINEAR_LEAST_SQUARES_FITTING_TRIANGLES_3_H
#define CGAL_LINEAR_LEAST_SQUARES_FITTING_TRIANGLES_3_H

#include <CGAL/basic.h>
#include <CGAL/Object.h>
#include <CGAL/centroid.h>
#include <CGAL/eigen.h>
#include <CGAL/util.h>

#include <iterator>

CGAL_BEGIN_NAMESPACE

namespace CGALi {

// fits a plane to a 3D triangle set
template < typename InputIterator, 
           typename K >
typename K::FT
linear_least_squares_fitting_3(InputIterator first,
                               InputIterator beyond, 
                               typename K::Plane_3& plane,   // best fit plane
                               typename K::Point_3& c,       // centroid
                               const K& k,                   // kernel
                               const typename K::Triangle_3*,  // used for indirection
			       const CGAL::PCA_dimension_2_tag& tag)
{
  typedef typename K::FT          FT;
  typedef typename K::Triangle_3  Triangle;

  // precondition: at least one element in the container.
  CGAL_precondition(first != beyond);
  
  // compute centroid
  c = centroid(first,beyond,K(),tag);

  // assemble covariance matrix
  FT covariance[6] = {0.0,0.0,0.0,0.0,0.0,0.0};
  assemble_covariance_matrix_3(first,beyond,covariance,c,k,(Triangle*) NULL,tag);

  // to remove later
  std::cout<<covariance[0]<<" "<<covariance[1]<<" "<<covariance[2]<<" "<<covariance[3]<<" "<<covariance[4]<<" "<<covariance[5]<<std::endl;
  
  // compute fitting plane
  return fitting_plane_3(covariance,c,plane,k);

} // end linear_least_squares_fitting_triangles_3

// fits a plane to a 3D triangle set
template < typename InputIterator, 
           typename K >
typename K::FT
linear_least_squares_fitting_3(InputIterator first,
                               InputIterator beyond, 
                               typename K::Plane_3& plane,   // best fit plane
                               typename K::Segment_3& c,       // centroid
                               const K& k,                   // kernel
                               const typename K::Triangle_3*,  // used for indirection
			       const CGAL::PCA_dimension_1_tag& tag)
{
  typedef typename K::FT          FT;
  typedef typename K::Triangle_3  Triangle;
  typedef typename K::Segment_3  Segment;

  // precondition: at least one element in the container.
  CGAL_precondition(first != beyond);
  
  std::list<Segment> segments;
  for(InputIterator it = first;
      it != beyond;
      it++)
  {
    const Triangle& t = *it;
    segments.push_back(t[0],t[1]);
    segments.push_back(t[1],t[2]);
    segments.push_back(t[2],t[0]);    
  }

  // compute fitting plane
  return linear_least_squares_fitting_3(segments.begin(),segments.end(),plane,c,k,(Segment*)NULL,tag);

} // end linear_least_squares_fitting_triangles_3

// fits a plane to a 3D triangle set
template < typename InputIterator, 
           typename K >
typename K::FT
linear_least_squares_fitting_3(InputIterator first,
                               InputIterator beyond, 
                               typename K::Plane_3& plane,   // best fit plane
                               typename K::Point_3& c,       // centroid
                               const K& k,                   // kernel
                               const typename K::Triangle_3*,  // used for indirection
			       const CGAL::PCA_dimension_0_tag& tag)
{
  typedef typename K::FT          FT;
  typedef typename K::Triangle_3  Triangle;
  typedef typename K::Point_3  Point;

  // precondition: at least one element in the container.
  CGAL_precondition(first != beyond);
  
  std::list<Point> points;
  for(InputIterator it = first;
      it != beyond;
      it++)
  {
    const Triangle& t = *it;
    points.push_back(t[0]);
    points.push_back(t[1]);
    points.push_back(t[2]);    
  }

  // compute fitting plane
  return linear_least_squares_fitting_3(points.begin(),points.end(),plane,c,k,(Point*)NULL,tag);

} // end linear_least_squares_fitting_triangles_3

// fits a line to a 3D triangle set
template < typename InputIterator, 
           typename K >
typename K::FT
linear_least_squares_fitting_3(InputIterator first,
                               InputIterator beyond, 
                               typename K::Line_3& line,     // best fit line
                               typename K::Point_3& c,       // centroid
                               const K& k,                   // kernel
                               const typename K::Triangle_3*,  // used for indirection
			       const CGAL::PCA_dimension_2_tag& tag)
{
  typedef typename K::FT          FT;
  typedef typename K::Triangle_3  Triangle;

  // precondition: at least one element in the container.
  CGAL_precondition(first != beyond);
  
  // compute centroid
  c = centroid(first,beyond,K(),tag);

  // assemble covariance matrix
  FT covariance[6] = {0.0,0.0,0.0,0.0,0.0,0.0};
  assemble_covariance_matrix_3(first,beyond,covariance,c,k,(Triangle*) NULL,tag);

  // to remove later
  std::cout<<covariance[0]<<" "<<covariance[1]<<" "<<covariance[2]<<" "<<covariance[3]<<" "<<covariance[4]<<" "<<covariance[5]<<std::endl;

  // compute fitting line
  return fitting_line_3(covariance,c,line,k);
  
} // end linear_least_squares_fitting_triangles_3

// fits a line to a 3D triangle set
template < typename InputIterator, 
           typename K >
typename K::FT
linear_least_squares_fitting_3(InputIterator first,
                               InputIterator beyond, 
                               typename K::Line_3& line,   // best fit line
                               typename K::Segment_3& c,       // centroid
                               const K& k,                   // kernel
                               const typename K::Triangle_3*,  // used for indirection
			       const CGAL::PCA_dimension_1_tag& tag)
{
  typedef typename K::FT          FT;
  typedef typename K::Triangle_3  Triangle;
  typedef typename K::Segment_3  Segment;

  // precondition: at least one element in the container.
  CGAL_precondition(first != beyond);
  
  std::list<Segment> segments;
  for(InputIterator it = first;
      it != beyond;
      it++)
  {
    const Triangle& t = *it;
    segments.push_back(t[0],t[1]);
    segments.push_back(t[1],t[2]);
    segments.push_back(t[2],t[0]);    
  }

  // compute fitting line
  return linear_least_squares_fitting_3(segments.begin(),segments.end(),line,c,k,(Segment*)NULL,tag);

} // end linear_least_squares_fitting_triangles_3

// fits a line to a 3D triangle set
template < typename InputIterator, 
           typename K >
typename K::FT
linear_least_squares_fitting_3(InputIterator first,
                               InputIterator beyond, 
                               typename K::Line_3& line,   // best fit line
                               typename K::Point_3& c,       // centroid
                               const K& k,                   // kernel
                               const typename K::Triangle_3*,  // used for indirection
			       const CGAL::PCA_dimension_0_tag& tag)
{
  typedef typename K::FT          FT;
  typedef typename K::Triangle_3  Triangle;
  typedef typename K::Point_3  Point;

  // precondition: at least one element in the container.
  CGAL_precondition(first != beyond);
  
  std::list<Point> points;
  for(InputIterator it = first;
      it != beyond;
      it++)
  {
    const Triangle& t = *it;
    points.push_back(t[0]);
    points.push_back(t[1]);
    points.push_back(t[2]);    
  }

  // compute fitting line
  return linear_least_squares_fitting_3(points.begin(),points.end(),line,c,k,(Point*)NULL,tag);

} // end linear_least_squares_fitting_triangles_3

} // end namespace CGALi

CGAL_END_NAMESPACE

#endif // CGAL_LINEAR_LEAST_SQUARES_FITTING_TRIANGLES_3_H
