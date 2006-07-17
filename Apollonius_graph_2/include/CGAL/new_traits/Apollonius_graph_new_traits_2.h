// Copyright (c) 2006 INRIA Sophia-Antipolis (France).
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
// 
//
// Author(s)     : Menelaos Karavelas <mkaravel@tem.uoc.gr>
//                 Christophe Delage <Christophe.Delage@sophia.inria.fr>
//                 David Millman <dlm336@cs.nyu.edu>

#ifndef CGAL_APOLLONIUS_GRAPH_NEW_TRAITS_2_H
#define CGAL_APOLLONIUS_GRAPH_NEW_TRAITS_2_H


// FIXME: We include the old traits class file for now to get the functors.
#include <CGAL/Apollonius_graph_traits_2.h>

#include <CGAL/new_traits/Vertex_conflict_2.h>
#include <CGAL/new_traits/Finite_edge_conflict_2.h>
#include <CGAL/new_traits/Infinite_edge_conflict_2.h>
#include <CGAL/new_traits/Orientation_2.h>

CGAL_BEGIN_NAMESPACE

// Would be *much* more convenient
// static inline
// Sign operator- (const Sign &s)
// {
//     return opposite (s);
// }
// 
// static inline
// Sign operator* (const Sign &s1, const Sign &s2)
// {
//     return static_cast<Sign> (static_cast<int> (s1) * static_cast<int> (s2));
// }



//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
// the Traits class
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
template < class Rep, class MTag = Ring_tag >
class Apollonius_graph_new_traits_2
{
public:
  //-----------------------------------------------------------------------
  //                  TYPE DEFINITIONS
  //-----------------------------------------------------------------------

  // BASIC TYPES
  //------------
private:  
  typedef Apollonius_graph_new_traits_2<Rep,MTag>       Self;
  typedef Apollonius_graph_kernel_wrapper_2<Rep>        Kernel;

public:
  typedef Rep                                           R;
  typedef MTag                                          Method_tag;
  typedef typename Kernel::Point_2                      Point_2;
  typedef typename Kernel::Site_2                       Site_2;

  typedef typename Kernel::Line_2                       Line_2;
  typedef typename Kernel::Ray_2                        Ray_2;
  typedef typename Rep::Segment_2                       Segment_2;

  typedef typename Kernel::Object_2                     Object_2;
  typedef typename Kernel::FT                           FT;
  typedef typename Kernel::RT                           RT;


public:
  // OBJECT CONSTRUCTION & ASSIGNMENT
  //---------------------------------
  typedef typename Kernel::Construct_object_2     Construct_object_2;
  typedef typename Kernel::Assign_2               Assign_2;

  // CONSTRUCTIONS
  //--------------
  // vertex and dual site
  typedef CGAL::Construct_Apollonius_vertex_2<Kernel>
  /*                                      */ Construct_Apollonius_vertex_2;
  typedef CGAL::Construct_Apollonius_site_2<Kernel>
  /*                                        */ Construct_Apollonius_site_2;


  // PREDICATES
  //-----------
  typedef CGAL::Ag2_compare_x_2<Kernel>                 Compare_x_2;
  typedef CGAL::Ag2_compare_y_2<Kernel>                 Compare_y_2;
  typedef CGAL::Ag2_compare_weight_2<Kernel>            Compare_weight_2;
  typedef CGAL::AG2_Orientation_test_new_2<Kernel,MTag> Orientation_2;
  typedef CGAL::Is_hidden_2<Kernel,MTag>                Is_hidden_2;
  typedef CGAL::Oriented_side_of_bisector_2<Kernel,MTag> 
  /*                                          */ Oriented_side_of_bisector_2;
  typedef CGAL::Vertex_conflict_new_2<Kernel,MTag>          Vertex_conflict_2;
  typedef CGAL::Finite_edge_interior_conflict_new_2<Kernel,MTag>
  /*                                      */ Finite_edge_interior_conflict_2;
  typedef CGAL::Infinite_edge_interior_conflict_new_2<Kernel,MTag>
  /*                                    */ Infinite_edge_interior_conflict_2;
  typedef CGAL::Is_degenerate_edge_2<Kernel,MTag>       Is_degenerate_edge_2;


public:
  //-----------------------------------------------------------------------
  //                  ACCESS TO OBJECTS
  //-----------------------------------------------------------------------

  // OBJECT CONSTRUCTION & ASSIGNMENT
  Assign_2
  assign_2_object() const {
    return Assign_2();
  }

  Construct_object_2
  construct_object_2_object() const { 
    return Construct_object_2();
  }


  // CONSTRUCTIONS
  //--------------
  Construct_Apollonius_vertex_2
  construct_Apollonius_vertex_2_object() const { 
    return Construct_Apollonius_vertex_2();
  }

  Construct_Apollonius_site_2
  construct_Apollonius_site_2_object() const {
    return Construct_Apollonius_site_2();
  }


  // PREDICATES
  //-----------
  Compare_x_2
  compare_x_2_object() const {
    return Compare_x_2();
  }

  Compare_y_2
  compare_y_2_object() const {
    return Compare_y_2();
  }

  Compare_weight_2
  compare_weight_2_object() const {
    return Compare_weight_2();
  }

  Orientation_2
  orientation_2_object() const {
    return Orientation_2();
  }

  Is_hidden_2
  is_hidden_2_object() const {
    return Is_hidden_2();
  }

  Oriented_side_of_bisector_2
  oriented_side_of_bisector_2_object() const {
    return Oriented_side_of_bisector_2();
  }

  Vertex_conflict_2
  vertex_conflict_2_object() const {
    return Vertex_conflict_2();
  }

  Finite_edge_interior_conflict_2
  finite_edge_interior_conflict_2_object() const {
    return Finite_edge_interior_conflict_2();
  }

  Infinite_edge_interior_conflict_2
  infinite_edge_interior_conflict_2_object() const {
    return Infinite_edge_interior_conflict_2();
  }

  Is_degenerate_edge_2
  is_degenerate_edge_2_object() const {
    return Is_degenerate_edge_2();
  }

};

CGAL_END_NAMESPACE

#endif // CGAL_APOLLONIUS_GRAPH_NEW_TRAITS_2_H
