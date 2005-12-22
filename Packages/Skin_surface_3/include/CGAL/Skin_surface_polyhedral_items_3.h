// Copyright (c) 2005 RuG (Netherlands)
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
// Author(s)     : Nico Kruithof <Nico@cs.rug.nl>

#ifndef SKIN_SURFACE_POLYHEDRAL_ITEMS_3_H
#define SKIN_SURFACE_POLYHEDRAL_ITEMS_3_H

#include <CGAL/HalfedgeDS_face_base.h>
#include <set>

CGAL_BEGIN_NAMESPACE

template <class Refs,  class  T, class PolyT>
struct Skin_surface_polyhedral_vertex
  : public HalfedgeDS_vertex_base<Refs, T,
					typename PolyT::Point_3> {
	
  typedef typename PolyT::Point_3               Point;
  typedef typename PolyT::RT                    RT;
  typedef HalfedgeDS_vertex_base<Refs, T, Point> Base;
	
  Skin_surface_polyhedral_vertex() : Base() {}
  Skin_surface_polyhedral_vertex(const Skin_surface_polyhedral_vertex &v)
    : Base(v) {}
  Skin_surface_polyhedral_vertex(const Point &p) : Base(p) {}
};

// Faces have references to the tetrahedra that contain them:
template <class Refs, class TetraComplex>
struct Skin_surface_polyhedral_face : public HalfedgeDS_face_base<Refs> {
  Skin_surface_polyhedral_face() : HalfedgeDS_face_base<Refs>(), tetras() {}
  Skin_surface_polyhedral_face(const Skin_surface_polyhedral_face &f)
    : HalfedgeDS_face_base<Refs>(f), tetras(f.tetras) {}

  std::set<typename TetraComplex::Cell_handle> tetras;
};

// An items type using my face.
template <class TetraComplex>
struct Skin_surface_polyhedral_items_3 : public Polyhedron_3_items_3 {
  template <class Refs, class Traits>
  struct Vertex_wrapper {
    typedef typename Traits::Point_3 Point;
    typedef Skin_surface_polyhedral_vertex<Refs, Tag_true, Traits> Vertex;
  };
  template <class Refs, class Traits>
  struct Face_wrapper {
    typedef Skin_surface_polyhedral_face<Refs, TetraComplex> Face;
  };
};

CGAL_END_NAMESPACE

#endif // SKIN_SURFACE_POLYHEDRAL_ITEMS_3_H
