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
// file          : include/CGAL/IO/Qt_widget_handtool.h
// package       : Qt_widget
// author(s)     : Radu Ursu
// release       : 
// release_date  : 
//
// coordinator   : Laurent Rineau <rineau@clipper.ens.fr>
//
// ============================================================================

#ifndef CGAL_QT_WIDGET_HANDTOOL_H
#define CGAL_QT_WIDGET_HANDTOOL_H


#include <cstdio>
#include <CGAL/IO/pixmaps/hand.xpm>
#include <CGAL/IO/pixmaps/holddown.xpm>
#include <CGAL/IO/Qt_widget.h>
#include <CGAL/IO/Qt_widget_tool.h>
#include <qrect.h>


#ifndef CGAL_QT_WINDOW_GET_POINT_BUTTON
#define CGAL_QT_WINDOW_GET_POINT_BUTTON Qt::LeftButton
#endif


namespace CGAL {

template <class R>
class Qt_widget_handtool : public Qt_widget_tool
{
public:
  typedef Point_2<R>		Point;
  typedef Segment_2<R>		Segment;
  typedef typename R::FT	FT;

  Qt_widget_handtool() : wasrepainted(TRUE), on_first(FALSE){};

private:
  void widget_repainted(){
    wasrepainted = TRUE;
  };

  void mousePressEvent(QMouseEvent *e)
  {
    if(e->button() == CGAL_QT_WINDOW_GET_POINT_BUTTON)
    {
      widget->setCursor(QCursor( QPixmap( (const char**)holddown_xpm)));
      if (!on_first){
	first_x = e->x();
	first_y = e->y();
	on_first = TRUE;
      }	
    }
  };

  void mouseReleaseEvent(QMouseEvent *e)
  {
    if(e->button() == CGAL_QT_WINDOW_GET_POINT_BUTTON)		
    {
      widget->setCursor(QCursor( QPixmap( (const char**)hand_xpm)));
      FT
	x=static_cast<FT>(widget->x_real(e->x())),
	y=static_cast<FT>(widget->y_real(e->y())),
	xfirst2 = static_cast<FT>(widget->x_real(first_x)),
	yfirst2 = static_cast<FT>(widget->y_real(first_y));
			
	double	xmin, xmax, ymin, ymax, distx, disty;
	if(x < xfirst2) {xmin = x; xmax = xfirst2;}
	else {xmin = xfirst2; xmax = x;};
	if(y < yfirst2) {ymin = y; ymax = yfirst2;}
	else {ymin = yfirst2; ymax = y;};
	distx = xfirst2 - x;
	disty = yfirst2 - y;
	widget->move_center(distx, disty);
	widget->redraw();
	emit(redraw());	
	on_first = FALSE;
    }
  }
  void mouseMoveEvent(QMouseEvent *e)
  {
    char tempc1[130], tempc2[40];
    if(on_first)
    {
      double
	x = e->x(),
	y = e->y();
	RasterOp old = widget->rasterOp();	//save the initial raster mode
	widget->setRasterOp(XorROP);
	widget->lock();
	*widget << CGAL::GRAY;
	if(!wasrepainted) {
	  CGAL_CLIB_STD::sprintf(tempc1, " dx=%20.6f", widget->x_real(x2 - first_x));
	  CGAL_CLIB_STD::sprintf(tempc2, ", dy=%20.6f", widget->x_real(y2 - first_y));
	  strcat(tempc1, tempc2);
	  widget->painter().drawLine(first_x, first_y, x2, y2);
	  *widget << CGAL::GREEN;
	  widget->painter().drawText(x2, y2, tempc1, 49);
	  *widget << CGAL::GRAY;
	}
	CGAL_CLIB_STD::sprintf(tempc1, " dx=%20.6f", widget->x_real(x - first_x));
	CGAL_CLIB_STD::sprintf(tempc2, ", dy=%20.6f", widget->x_real(y - first_y));
	strcat(tempc1, tempc2);
	widget->painter().drawLine(first_x, first_y, x, y);
	*widget << CGAL::GREEN;
	widget->painter().drawText(x, y, tempc1, 49);
	widget->unlock();
	widget->setRasterOp(old);

	//save the last coordinates to redraw the screen
	x2 = x;
	y2 = y;
	wasrepainted = FALSE;
    }
   
  };
	

  void attaching()
  {
    oldcursor = widget->cursor();
    widget->setCursor(QCursor( QPixmap( (const char**)hand_xpm)));
    wasrepainted = TRUE;
  };

  void detaching()
  {
    widget->setCursor(oldcursor);
  };

  double  first_x, first_y;
  double  x2, y2;
  bool	  wasrepainted;
  bool	  on_first;
};//end class 

} // namespace CGAL

#endif // CGAL_QT_WIDGET_HANDTOOL_H
