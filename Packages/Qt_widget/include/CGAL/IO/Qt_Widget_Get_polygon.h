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
// file          : include/CGAL/IO/Qt_Widget_Get_polygon.h
// package       : QT_window
// author(s)     : Laurent Rineau
// release       : 
// release_date  : 
//
// coordinator   : Laurent Rineau <rineau@clipper.ens.fr>
//
// ============================================================================

#ifndef CGAL_QT_WINDOW_GET_POLYGON_H
#define CGAL_QT_WINDOW_GET_POLYGON_H

#include <CGAL/IO/Qt_Widget.h>
#include <CGAL/IO/Qt_Widget_tool.h>
#include <list>

namespace CGAL {

template <class Polygon>
class Qt_widget_get_polygon : public Qt_widget_tool
{
public:
  typedef typename Polygon::Point_2 Point_2;
  typedef typename Polygon::FT FT;
  
  Qt_widget_get_polygon()
    : active(false), lastx(0), lasty(0), num_points(0), poly(),
      qpoints(0), qpoints_old(0) {};

  void mousePressEvent(QMouseEvent *e)
  {
    if(e->button() == Qt::LeftButton)
    {
      FT
	x=static_cast<FT>(widget->x_real(e->x())),
	y=static_cast<FT>(widget->y_real(e->y()));
      poly.push_back(Point_2(x,y));
      qpoints.putPoints(num_points, 1, e->x(), e->y());
      qpoints_old.putPoints(num_points, 1, e->x(), e->y());
      ++num_points;
      active=true;
      widget->setMouseTracking(TRUE);
      return;
    };
    if(e->button() == Qt::RightButton)
    {
      if (active)
      {
	RasterOp old_rasterop=widget->rasterOp();
	widget->painter().setRasterOp(NotROP);
	widget->painter().drawPolyline(qpoints_old);
	widget->setRasterOp(old_rasterop);
	widget->do_paint();
	FT
	  x=static_cast<FT>(widget->x_real(e->x())),
	  y=static_cast<FT>(widget->y_real(e->y()));
	poly.push_back(Point_2(x, y));
	widget->new_object(make_object(poly));

	// TODO: have we something better to clear a polygon?
	while(!poly.is_empty())
	  poly.erase(--poly.vertices_end());

	qpoints.resize(0);
	qpoints_old.resize(0);
	num_points=0;
	active=false;
	};
    };
  };

  void mouseMoveEvent(QMouseEvent *e)
  {
    if (active)
    {
      int 
	x=(e->pos()).x(),
	y=(e->pos()).y();
	if ((lastx != x) || (lasty != y)) {
	  widget->lock();
	  RasterOp old_rasterop=widget->rasterOp();
	  widget->painter().setRasterOp(NotROP);
	  qpoints.putPoints(num_points,1,x,y);
	  widget->painter().drawPolyline(qpoints);
	  // Erase old polyline
	  widget->painter().drawPolyline(qpoints_old);
	  widget->setRasterOp(old_rasterop);
	  widget->unlock();
	  lastx= x;
	  lasty= y;
	  qpoints_old.putPoints(num_points,1,x,y);
	}
    }
  };
  void attaching()
  {	
    oldcursor = widget->cursor();
    widget->setCursor(crossCursor);
  };
  
  void detaching()
  {
    widget->setCursor(oldcursor);
  };
  
protected:
  bool active;
  int lastx, lasty;
  int num_points;
  Polygon poly;
  QPointArray qpoints;
  QPointArray qpoints_old;
};

} // namespace CGAL

#endif // CGAL_QT_WINDOW_GET_POLYGON_H
