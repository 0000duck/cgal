// ======================================================================
//
// Copyright (c) 2000 The CGAL Consortium
//
// This software and related documentation is part of an INTERNAL release
// of the Computational Geometry Algorithms Library (CGAL). It is not
// intended for general use.
//
// ----------------------------------------------------------------------
//
// release       : 
// release_date  : 
//
// file          : include/CGAL/IO/Triangulation_geomview_ostream_2.h
// revision      : $Revision$
// revision_date : $Date$
// author(s)     : Sylvain Pion
//
// coordinator   : INRIA Sophia-Antipolis (<Mariette.Yvinec@sophia.inria.fr>)
//
// ======================================================================

#ifndef CGAL_IO_TRIANGULATION_GEOMVIEW_OSTREAM_2_H
#define CGAL_IO_TRIANGULATION_GEOMVIEW_OSTREAM_2_H

#include <CGAL/IO/Geomview_stream.h>
#include <CGAL/Triangulation_2.h>

CGAL_BEGIN_NAMESPACE

// There are 2 drawing functions for triangulations : depending on the wired
// mode of the Geomview_stream, we draw either the edges or the faces.

// TODO :
// - Check the correctness when dimension < 2.
// - Use the current stream color instead of built-in constant.

template < class GT, class TDS >
void
show_triangulation_edges(Geomview_stream &gv, const Triangulation_2<GT,TDS> &T)
{
    // Header.
    gv.set_ascii_mode();
    gv << "(geometry triangulationwired {appearance {}{ SKEL \n";
    gv << T.number_of_vertices();
    gv << T.number_of_vertices() + T.number_of_faces()-1 << "\n";

    // Finite vertices coordinates.
    std::map<Triangulation_2<GT, TDS>::Vertex_handle, int> V;
    int inum = 0;
    for( Triangulation_2<GT, TDS>::Vertex_iterator
	  vit = T.vertices_begin(); vit != T.vertices_end(); ++vit) {
        V[vit] = inum++;
        gv << vit->point() << "\n";
    }
  
    // Finite edges indices.
    for( Triangulation_2<GT, TDS>::Edge_iterator
	  eit = T.edges_begin(); eit != T.edges_end(); ++eit) {
        gv << 2
           << V[(*eit).first->vertex(T.ccw((*eit).second))]
           << V[(*eit).first->vertex(T. cw((*eit).second))]
           << "\n"; // without color.
        // << 4 << drand48() << drand48() << drand48() << 1.0; // random color
    }
}

template < class GT, class TDS >
void
show_triangulation_faces(Geomview_stream &gv, const Triangulation_2<GT,TDS> &T)
{
    // Header.
    gv.set_binary_mode();
    gv << "(geometry triangulation {appearance {}{ OFF BINARY\n";
    gv << T.number_of_vertices() << T.number_of_faces() << 0;

    // Finite vertices coordinates.
    std::map<Triangulation_2<GT, TDS>::Vertex_handle, int> V;
    int inum = 0;
    for( Triangulation_2<GT, TDS>::Vertex_iterator
	  vit = T.vertices_begin(); vit != T.vertices_end(); ++vit) {
        V[vit] = inum++;
        gv << vit->point();
    }
  
    // Finite faces indices.
    for( Triangulation_2<GT, TDS>::Face_iterator
	  fit = T.faces_begin(); fit != T.faces_end(); ++fit) {
        gv << 3;
        for (int i=0; i<3; i++)
            gv << V[fit->vertex(i)];
        gv << 0; // without color.
      // gv << 4 << drand48() << drand48() << drand48() << 1.0; // random color.
    }
}

template < class GT, class TDS >
Geomview_stream&
operator<<( Geomview_stream &gv, const Triangulation_2<GT,TDS> &T)
{
    bool ascii_bak = gv.get_ascii_mode();
    bool raw_bak = gv.set_raw(true);

    if (gv.get_wired())
	show_triangulation_edges(gv, T);
    else
	show_triangulation_faces(gv, T);

    // Footer.
    gv << "}})";

    gv.set_raw(raw_bak);
    gv.set_ascii_mode(ascii_bak);
    return gv;
}

CGAL_END_NAMESPACE

#endif // CGAL_IO_TRIANGULATION_GEOMVIEW_OSTREAM_2_H
