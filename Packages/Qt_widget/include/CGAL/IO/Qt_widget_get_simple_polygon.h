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
// file          : include/CGAL/IO/Qt_widget_get_simple_polygon.h
// package       : Qt_widget
// author(s)     : Laurent Rineau && Radu Ursu
// release       : 
// release_date  : 
//
// coordinator   : Laurent Rineau <rineau@clipper.ens.fr>
//
// ============================================================================

#ifndef CGAL_QT_WIDGET_GET_SIMPLE_POLYGON_H
#define CGAL_QT_WIDGET_GET_SIMPLE_POLYGON_H


#include <CGAL/IO/Qt_widget_tool.h>
#include <CGAL/Segment_2_Segment_2_intersection.h>  
#include <list>


namespace CGAL {
template <class Polygon>
class Qt_widget_get_simple_polygon : public Qt_widget_tool
{
public:
  typedef typename Polygon::Point_2   Point_2;
  typedef typename Polygon::Segment_2 Segment_2;
  typedef typename Polygon::FT	      FT;
  
  Qt_widget_get_simple_polygon()
    : active(false), first_time(true) {};

  void widget_repainted()
  {
    if(poly.size() > 1)
    {
      Polygon::Edge_const_iterator  it;
      widget->lock();
	RasterOp old_rasterop=widget->rasterOp();
	widget->painter().setRasterOp(XorROP);
	*widget << CGAL::GREEN;
	for(it = poly.edges_begin(); it != --poly.edges_end(); it++)
	  *widget << *it;
	widget->setRasterOp(old_rasterop);
      widget->unlock();
    }
  };

  void mousePressEvent(QMouseEvent *e)
  {
    if(e->button() == Qt::LeftButton)
    {
      FT
	x=static_cast<FT>(widget->x_real(e->x())),
	y=static_cast<FT>(widget->y_real(e->y()));
      if(!active)
      {
	active=true;
	widget->setMouseTracking(TRUE);
	last_of_poly = Point_2(x, y);
	poly.push_back(Point_2(x, y));	
      } else{
	if (last_of_poly == Point_2(x,y))
	  return;
	rubber_old = Point_2(x, y);
	if(is_simple()){
	  poly.push_back(Point_2(x,y));	
	  //show the last rubber as edge of the polygon
	  widget->lock();
	    RasterOp old_rasterop=widget->rasterOp();
	    widget->painter().setRasterOp(XorROP);
	    *widget << CGAL::WHITE;
	    *widget << Segment_2(rubber, last_of_poly);
	    *widget << CGAL::GREEN;
	    *widget << Segment_2(rubber, last_of_poly);
	    widget->setRasterOp(old_rasterop);
	  widget->unlock();
	  last_of_poly = Point_2(x, y);
	}
      }
      return;
    };
    if(e->button() == Qt::RightButton)
    {
      if (active)
      {
	if(!poly.is_simple())
	  return;
	if(poly.is_clockwise_oriented())
	  poly.reverse_orientation ();
	assert( ! poly.is_clockwise_oriented());
	  
	widget->new_object(make_object(poly));
	active = false;
	first_time = true;
	poly.erase(poly.vertices_begin(), poly.vertices_end());
	widget->redraw();
      }
    };
  };

  void keyPressEvent(QKeyEvent *e)
  {
    switch ( e->key() ) {
	case 0x1000:				// key_escape
	  if(poly.size() > 1){
	    widget->lock();
	      RasterOp old_rasterop=widget->rasterOp();
	      widget->painter().setRasterOp(XorROP);
	      *widget << CGAL::GREEN;
	      *widget << Segment_2(*(----poly.vertices_end()), last_of_poly);
	      *widget << CGAL::WHITE;
	      *widget << Segment_2(rubber, last_of_poly);
	      *widget << Segment_2(rubber, *(----poly.vertices_end()));
	      widget->setRasterOp(old_rasterop);
	    widget->unlock();
	    poly.erase(--poly.vertices_end());
	    last_of_poly = *(--poly.vertices_end());
	  }
	  break;
    }//endswitch
  }

  void mouseMoveEvent(QMouseEvent *e)
  {
    if (active)
    {
      FT
	x=static_cast<FT>(widget->x_real(e->x())),
	y=static_cast<FT>(widget->y_real(e->y()));

      rubber = Point_2(x, y);
      widget->lock();
	RasterOp old_rasterop=widget->rasterOp();
	widget->painter().setRasterOp(XorROP);
	*widget << CGAL::WHITE;      	
	if(!first_time)
	  *widget << Segment_2(rubber_old, last_of_poly);
	*widget << Segment_2(rubber, last_of_poly);
	first_time = false;
	rubber_old = rubber;
	widget->setRasterOp(old_rasterop);
      widget->unlock();
    }
  };
  void attaching()
  {	
    oldcursor = widget->cursor();
    widget->setCursor(crossCursor);
    oldpolicy = widget->focusPolicy();
    widget->setFocusPolicy(QWidget::StrongFocus);
  };
  
  void detaching()
  {
    poly.erase(poly.vertices_begin(), poly.vertices_end());
    active = false;
    first_time = true;
    widget->setCursor(oldcursor);
    widget->setFocusPolicy(oldpolicy);
    widget->redraw();
  };
  void leaveEvent(QEvent *e)
  {
    if (active)
    {
      widget->lock();
	RasterOp old_rasterop=widget->rasterOp();
	widget->painter().setRasterOp(XorROP);
	*widget << CGAL::WHITE;
	*widget << Segment_2(rubber_old, last_of_poly);
	widget->setRasterOp(old_rasterop);
      widget->unlock();
      first_time = true;
    }
  }
private:
  bool is_simple()
  {
    Segment_2 rubber_segment(rubber, last_of_poly);
    if(poly.size() > 1)
    {
      Polygon::Edge_const_iterator  it;
      for(it = poly.edges_begin(); it != ----poly.edges_end(); it++)
      {
	if(do_intersect(*it, rubber_segment))
	  return false;
      }
      //if I'm out of this means that all the edges, 
      //didn't intersect the last one
      ++it;
      Object o = intersection(*it, rubber_segment);
      Point_2 p;
      if(assign(p, o))
	return true;
      else
	return false;
    }
    return true;
  }
  
protected:
  bool	  active,     //true if the first point was inserted
	  first_time; //true if it is the first time when 
		      //draw the rubber band
  Point_2 rubber,     //the new point of the rubber band
	  last_of_poly,	//the last point of the polygon
	  rubber_old; //the old point of the rubber band
  Polygon poly;	      //the polygon
  QWidget::FocusPolicy	oldpolicy;
};

} // namespace CGAL

#endif // CGAL_QT_WIDGET_GET_SIMPLE_POLYGON_H
