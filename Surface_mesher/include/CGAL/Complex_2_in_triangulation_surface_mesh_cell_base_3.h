// Copyright (c) 2003-2005  INRIA Sophia-Antipolis (France).
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
// $Source: 
// $Revision$ $Date$
// $Name$
//
// Author(s)     : Steve Oudot, David Rey, Mariette Yvinec, Laurent Rineau, Andreas Fabri





#ifndef CGAL_COMPLEX_2_IN_TRIANGULATION_SURFACE_MESH_CELL_BASE_3_H
#define CGAL_COMPLEX_2_IN_TRIANGULATION_SURFACE_MESH_CELL_BASE_3_H

#include <CGAL/Complex_2_in_triangulation_cell_base_3.h>

namespace CGAL {
  
  template < class GT, class Cb=Complex_2_in_triangulation_cell_base_3<GT> > 
  class Complex_2_in_triangulation_surface_mesh_cell_base_3 : 
  public Cb {    
    
  public:
    typedef Complex_2_in_triangulation_surface_mesh_cell_base_3 <GT, Cb> Self;

    template < class TDS3 >
    struct Rebind_TDS {
      typedef typename Cb::template Rebind_TDS<TDS3>::Other  Cb3;
      typedef Complex_2_in_triangulation_surface_mesh_cell_base_3 <GT, Cb3> Other;
    };
    
    
    typedef typename GT::Point_3 Point;
    
    typedef typename Cb::Triangulation_data_structure Tds;
    typedef typename Tds::Vertex_handle Vertex_handle;
    typedef typename Tds::Cell_handle Cell_handle;
    
    
  private:
    
    // Champs ajoutes a la classe
    
    // Facets
    Point tab_surface_center_facets [4];
    bool facet_visited [4];
    bool visits [4];
    
    
  public:
    
    // Constructors

    Complex_2_in_triangulation_surface_mesh_cell_base_3() : Cb() {
      
      facet_visited[0] = facet_visited[1] = facet_visited[2] = 
	facet_visited[3] = false;
      visits[0] = visits[1] = visits[2] = visits[3] = false;
    }
    
    Complex_2_in_triangulation_surface_mesh_cell_base_3 (Vertex_handle v0,
							 Vertex_handle v1,
							 Vertex_handle v2,
							 Vertex_handle v3) : 
      Cb (v0, v1, v2, v3) {
      
      facet_visited[0] = facet_visited[1] = facet_visited[2] = 
	facet_visited[3] = false;
      visits[0] = visits[1] = visits[2] = visits[3] = false;
    }
    
    Complex_2_in_triangulation_surface_mesh_cell_base_3 (Vertex_handle v0,
							 Vertex_handle v1,
							 Vertex_handle v2,
							 Vertex_handle v3,
							 Cell_handle n0,
							 Cell_handle n1,
							 Cell_handle n2,
							 Cell_handle n3) : 
      Cb (v0, v1, v2, v3, n0, n1, n2, n3) {
      
      facet_visited[0] = facet_visited[1] = facet_visited[2] = 
	facet_visited[3] = false;
      visits[0] = visits[1] = visits[2] = visits[3] = false;
    }
    
    
    
    // Access functions
    
    // Facets
    
    bool is_facet_visited (const int facet) const {
      CGAL_assertion (facet>=0 && facet <4);
      return facet_visited[facet];
    }

    bool visited (const int facet) const {
      CGAL_assertion (facet>=0 && facet <4);
      return visits[facet];
    }
    
    const Point& get_facet_surface_center(const int facet) const {
      CGAL_assertion (facet>=0 && facet <4);
      return(tab_surface_center_facets[facet]);
  }
    
    // Setting functions
    
    // Facets
    
    void set_facet_visited (const int facet) {
      CGAL_assertion (facet>=0 && facet <4);
      facet_visited[facet] = true;
    }
    
    void set_visited (const int facet) {
      CGAL_assertion (facet>=0 && facet <4);
      visits[facet] = true;
    }
    
    void reset_visited (const int facet) {
      CGAL_assertion (facet>=0 && facet <4);
      visits[facet] = false;
      facet_visited [facet] = false;
    }
    
    void set_facet_surface_center(const int facet, 
			  const Point& p) {
      CGAL_assertion (facet>=0 && facet <4);
      tab_surface_center_facets[facet]=p;
    }

  };  // end Complex_2_in_triangulation_surface_mesh_cell_base_3

template < class GT, class Cb >
std::istream&
operator>>(std::istream &is,
           Complex_2_in_triangulation_surface_mesh_cell_base_3<GT, Cb> &v)
{
  return is >> static_cast<Cb&>(v);
}

template < class GT, class Cb >
std::ostream&
operator<<(std::ostream &os,
           const Complex_2_in_triangulation_surface_mesh_cell_base_3<GT, Cb> &v)
{
  return os << static_cast<const Cb&>(v);
}

}  // namespace CGAL


#endif  // CGAL_COMPLEX_2_IN_TRIANGULATION_SURFACE_MESH_CELL_BASE_3_H

