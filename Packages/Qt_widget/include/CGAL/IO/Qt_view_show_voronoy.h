// ============================================================================
//
// Copyright (c) 1997-2000 The CGAL Consortium
//
// This software and related documentation is part of an INTERNAL release
// of the Computational Geometry Algorithms Library (CGAL). It is not
// intended for general use.
//
// ----------------------------------------------------------------------------
//
// file          : include/CGAL/IO/Qt_view_show_voronoy.h
// package       : Qt_widget
// author(s)     : Radu Ursu
// release       : 
// release_date  : 
//
// coordinator   : Laurent Rineau <rineau@clipper.ens.fr>
//
// ============================================================================

#ifndef CGAL_QT_VIEW_SHOW_VORONOI_H
#define CGAL_QT_VIEW_SHOW_VORONOI_H


#include <qobject.h>
#include <CGAL/IO/Qt_widget_view.h>

namespace CGAL {

template <class T>
class Qt_view_show_voronoi : public Qt_widget_view
{
public:
  Qt_view_show_voronoi(T &t1) : tr(t1){};

  void draw(Qt_widget &widget)
  {
    widget << CGAL::RED ;
    tr.draw_dual(widget);
  };
	
private:
  T	&tr;
};//end class 

} // namespace CGAL

#endif // CGAL_QT_VIEW_SHOW_VORONOI_H
