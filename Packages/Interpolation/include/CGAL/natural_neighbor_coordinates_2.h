// Copyright (c) 2003 INRIA Sophia-Antipolis (France).
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
// Author(s)     : Frank Da, Julia Floetotto
#ifndef CGAL_NATURAL_NEIGHBOR_COORDINATES_2_H
#define CGAL_NATURAL_NEIGHBOR_COORDINATES_2_H

#include <utility>
#include <CGAL/Polygon_2.h>

//-------------------------------------------------------------------
CGAL_BEGIN_NAMESPACE
//-------------------------------------------------------------------
// The following natural_neighbor_coordinate_2 functions fix the 
// traits class to be Dt::Geom_traits. The following signatures could
// be used if one wants to pass a traits class as argument:
// template <class Dt, class OutputIterator, class Traits>
// Triple< OutputIterator, typename Traits::FT, bool > 
// natural_neighbor_coordinates_2(const Dt& dt,
// 			     const typename Traits::Point_2& p, 
// 			     OutputIterator out, const Traits& traits, 
// 			     typename Dt::Face_handle start 
// 			     = typename Dt::Face_handle(NULL))
//
//template <class Dt, class OutputIterator, class Traits>
//Triple< OutputIterator, typename Traits::FT, bool > 
//natural_neighbor_coordinates_2(const Dt& dt, 
//			       typename Dt::Vertex_handle vh, 
//			       OutputIterator out, const Traits& traits)

template <class Dt, class OutputIterator>
Triple< OutputIterator, typename Dt::Geom_traits::FT, bool > 
natural_neighbor_coordinates_2(const Dt& dt, 
			     const typename Dt::Geom_traits::Point_2& p, 
			     OutputIterator out, typename Dt::Face_handle start 
			     = typename Dt::Face_handle(NULL))

{
  typedef typename Dt::Geom_traits       Traits;        
  typedef typename Traits::FT            Coord_type;
  typedef typename Traits::Point_2       Point_2;
  typedef typename Dt::Face_handle       Face_handle;
  typedef typename Dt::Edge              Edge;
  typedef typename Dt::Locate_type       Locate_type;
  
  Locate_type lt;
  int li;
  Face_handle fh = dt.locate(p, lt, li, start);
  
  //the point must lie inside the convex hull: 
  // otherwise return false
  if(lt == Dt::OUTSIDE_AFFINE_HULL 
     || lt == Dt::OUTSIDE_CONVEX_HULL
     || (lt == Dt::EDGE && 
	 (dt.is_infinite(fh) ||
	  dt.is_infinite(fh->neighbor(li)))))
    return(make_triple(out, Coord_type(1), false));
  
  if(lt == Dt::VERTEX){
    *out++= std::pair<Point_2, Coord_type>(fh->vertex(li)->point(),
					   Coord_type(1));
    return( make_triple(out, Coord_type(1), true));
  }
  
  std::list<Edge> hole;
  
  dt.get_boundary_of_conflicts(p,std::back_inserter(hole), fh); 
  return 
    natural_neighbor_coordinates_2
    (dt, p, out, hole.begin(),hole.end());
}

//function call if the conflict zone is known:
template <class Dt, class OutputIterator, class EdgeIterator  >
Triple< OutputIterator, typename Dt::Geom_traits::FT, bool > 
natural_neighbor_coordinates_2(const Dt& dt, 
			       const typename Dt::Geom_traits::Point_2& p, 
			       OutputIterator out, EdgeIterator
			       hole_begin, EdgeIterator hole_end){
  
  CGAL_precondition(dt.dimension()==2);
  //precondition: p must lie inside the hole 
  //             (=^ inside convex hull of neighbors)
  typedef typename Dt::Geom_traits       Traits;
  typedef typename Traits::FT            Coord_type;
  typedef typename Traits::Point_2       Point_2;
  
  typedef typename Dt::Vertex_handle     Vertex_handle;
  typedef typename Dt::Face_circulator   Face_circulator;

  std::vector<Point_2> vor(3); 
  
  Coord_type area_sum(0);
  EdgeIterator hit = hole_end;
  --hit;
  //in the beginning: prev is the "last" vertex of the hole:
  // later: prev is the last vertex processed (previously) 
  Vertex_handle prev = hit->first->vertex(dt.cw(hit->second));
  hit = hole_begin;
  
  while(hit != hole_end)
    { 
      Coord_type area(0);
      Vertex_handle current = hit->first->vertex(dt.cw(hit->second));
      
      vor[0] = dt.geom_traits().construct_circumcenter_2_object()
	  (current->point(),
	   hit->first->vertex(dt.ccw(hit->second))->point(),
	   p);
      
      Face_circulator fc = dt.incident_faces(current, hit->first);
      ++fc;
      vor[1] = dt.dual(fc);
      
     while(!fc->has_vertex(prev))
       {
	 ++fc;
	 vor[2] = dt.dual(fc);
	  
	 area += polygon_area_2(vor.begin(), vor.end(), dt.geom_traits());
	 
	 vor[1] = vor[2];
       };
     vor[2] = 
       dt.geom_traits().construct_circumcenter_2_object()(prev->point(),
						current->point(),p);
     
     area += polygon_area_2(vor.begin(), vor.end(), dt.geom_traits());
     
     *out++= std::pair<Point_2, Coord_type>(current->point(),area);
     area_sum += area;
     
     //update prev and hit:
     prev= current;
     ++hit;
    }
  return(make_triple(out, area_sum, true));
};

/**********************************************************/
//compute the coordinates for a vertex of the triangulation 
// with respect to the other points in the triangulation
template <class Dt, class OutputIterator>
Triple< OutputIterator, typename Dt::Geom_traits::FT, bool > 
natural_neighbor_coordinates_2(const Dt& dt, 
			       typename Dt::Vertex_handle vh,
			       OutputIterator out){
  //this functions creates a small triangulation of the 
  // incident vertices of this vertex and computes the 
  // natural neighbor coordinates of ch->point() wrt. it.
  typedef typename Dt::Vertex_circulator     Vertex_circulator;
  
  Dt t2;
  Vertex_circulator vc = dt.incident_vertices(vh),
    done(vc);
  do{
    assert(!dt.is_infinite(vc));
    t2.insert(vc->point());
  }
  while(++vc!=done);
  
  return natural_neighbor_coordinates_2(t2, vh->point(), out);
};

//class providing a function object:
template <class Dt, class OutputIterator>
class natural_neighbor_coordinates_2_object 
{
public:
  Triple< OutputIterator, typename Dt::Geom_traits::FT, bool > 
  operator()(const Dt& dt, 
	     typename Dt::Vertex_handle vh,
	     OutputIterator out){
    return natural_neighbor_coordinates_2(dt, vh, out);
  }
};

//-------------------------------------------------------------------
CGAL_END_NAMESPACE
//-------------------------------------------------------------------

#endif // CGAL_NATURAL_NEIGHBOR_COORDINATES_2_H
