// ============================================================================
//
// Copyright (c) 1997 The CGAL Consortium
//
// This software and related documentation is part of an INTERNAL release
// of the Computational Geometry Algorithms Library (CGAL). It is not
// intended for general use.
//
// ----------------------------------------------------------------------------
//
// release       : $CGAL_Revision: $
// release_date  : $CGAL_Date: $
//
// file          : HalfedgeDS_face_min_base.h
// chapter       : $CGAL_Chapter: Halfedge Data Structures $
// package       : $CGAL_Package: HalfedgeDS 3.3 (27 Sep 2000) $
// source        : hds_bases.fw
// revision      : $Revision$
// revision_date : $Date$
// author(s)     : Lutz Kettner  <kettner@inf.ethz.ch>
//
// coordinator   : MPI Saarbruecken (Stefan Schirra <stschirr@mpi-sb.mpg.de>)
//
// Halfedge Data Structure Minimal Base Class for Vertices.
// ============================================================================

#ifndef CGAL_HALFEDGEDS_FACE_MIN_BASE_H
#define CGAL_HALFEDGEDS_FACE_MIN_BASE_H 1

#ifndef CGAL_BASIC_H
#include <CGAL/basic.h>
#endif

CGAL_BEGIN_NAMESPACE

template < class Refs>
class HalfedgeDS_face_min_base {
public:
    typedef Refs                                 HalfedgeDS;
    typedef HalfedgeDS_face_min_base< Refs>      Base;
    typedef Tag_false                            Supports_face_halfedge;
    typedef typename Refs::Vertex_handle         Vertex_handle;
    typedef typename Refs::Vertex_const_handle   Vertex_const_handle;
    typedef typename Refs::Halfedge_handle       Halfedge_handle;
    typedef typename Refs::Halfedge_const_handle Halfedge_const_handle;
    typedef typename Refs::Face_handle           Face_handle;
    typedef typename Refs::Face_const_handle     Face_const_handle;
    typedef typename Refs::Vertex                Vertex;
    typedef typename Refs::Halfedge              Halfedge;
    // Additional tags required by Polyhedron.
    typedef Tag_false                            Supports_face_plane;
    typedef Tag_false                            Supports_face_normal;
};

CGAL_END_NAMESPACE

#endif // CGAL_HALFEDGEDS_FACE_MIN_BASE_H //
// EOF //
