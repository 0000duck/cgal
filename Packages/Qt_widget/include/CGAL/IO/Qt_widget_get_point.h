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
// file          : include/CGAL/IO/Qt_Window_Get_point.h
// package       : QT_window
// author(s)     : Laurent Rineau
// release       : 
// release_date  : 
//
// coordinator   : Laurent Rineau <rineau@clipper.ens.fr>
//
// ============================================================================

#ifndef CGAL_QT_WIDGET_GET_POINT_H
#define CGAL_QT_WIDGET_GET_POINT_H

#include <CGAL/IO/Qt_widget.h>
#include <CGAL/IO/Qt_widget_tool.h>

#ifndef CGAL_QT_WIDGET_GET_POINT_BUTTON
#define CGAL_QT_WIDGET_GET_POINT_BUTTON Qt::LeftButton
#endif

namespace CGAL {

template <class R>
class Qt_widget_get_point : public Qt_widget_tool
{
public:
  typedef typename R::Point_2	Point;
  typedef typename R::FT	FT;
  
  Qt_widget_get_point(const QCursor c=QCursor(Qt::crossCursor)) :
    cursor(c) {};
  
private:
  void mousePressEvent(QMouseEvent *e)
  {
    if(e->button() == CGAL_QT_WIDGET_GET_POINT_BUTTON)
    {
      FT
	x=static_cast<FT>(widget->x_real(e->x())),
	y=static_cast<FT>(widget->y_real(e->y()));
      widget->new_object(make_object(Point(x, y)));
    }
  };
  void attaching()
  {
    oldcursor = widget->cursor();
    widget->setCursor(cursor);
  };
  
  void detaching()
  {
    widget->setCursor(oldcursor);
  };

  QCursor cursor;
};

} // namespace CGAL

#endif // CGAL_QT_WINDOW_GET_POINT_H
