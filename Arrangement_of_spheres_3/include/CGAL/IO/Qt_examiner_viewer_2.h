#ifndef QT_EXAMINER_VIEWER_2_H
#define QT_EXAMINER_VIEWER_2_H

#include <CGAL/basic.h>
#include <limits>
#include <qmutex.h>
#include <qthread.h>
#include <string>
#include <vector>
#include <CGAL/IO/Color.h>
#include <CGAL/IO/Qt_widget.h>
#include <CGAL/IO/Qt_widget_layer.h>
#include <CGAL/Simple_cartesian.h>
#include <CGAL/Algebraic_kernel_2_2.h>
#include <CGAL/Circular_kernel.h>
#include <CGAL/Tools/Label.h>
#include <CGAL/Circular_arc_point_2.h>
#include <CGAL/Line_arc_2.h>
#include "Qt_examiner_viewer_window_2.h"


class Qt_examiner_viewer_2 : public Qt_examiner_viewer_window_2 {
  typedef Qt_examiner_viewer_window_2 P;
  struct QTEV_layer;
  typedef CGAL::Simple_cartesian<double> K;
  //typedef K::FT                                          NT;
  typedef CGAL::Algebraic_kernel_for_circles_2_2<double>     Algebraic_k;
  typedef CGAL::Circular_kernel_2<K, Algebraic_k>        Circular_k;

  
public:
  typedef K Geom_traits;
  typedef K::FT NT;
  typedef K::Circle_2 Circle;
  typedef K::Point_2 Point;
  typedef K::Vector_2 Vector;
  typedef K::Line_2 Line;
  typedef K::Segment_2 Segment;
  typedef Circular_k::Circular_arc_2 Circular_arc;

  Qt_examiner_viewer_2(NT scale=1): P(600,600),
				  scale_(scale)
  {
    *P::widget() << CGAL::BackgroundColor(CGAL::WHITE);
    set_layer(0);
  };

  ~Qt_examiner_viewer_2();


  virtual void click(double x, double y){}

  
  void clear() ;
  //void clear_layer();
  // void set_is_editing_layer(bool tf);
  void show_everything() ;
  void show() ;
  void redraw();
  template <class K>
  void new_circle(const CGAL::Circle_2<K> &ci) {
    Circle c(Point(CGAL::to_double(ci.center().x()),
		   CGAL::to_double(ci.center().y())),
	     CGAL::to_double(ci.squared_radius()));
    layers_[cur_layer_]->new_circle(c);
  }
  template <class Pt>
  void new_point(const Pt &ci) {
    Point c(CGAL::to_double(ci.x()),
	    CGAL::to_double(ci.y()));
    layers_[cur_layer_]->new_point(c);
  }

  template <class K>
  void new_circular_arc(const CGAL::Circular_arc_2<K> &ca) {
    Point center(CGAL::to_double(ca.supporting_circle().center().x()),
		 CGAL::to_double(ca.supporting_circle().center().y()));
    Circle c(center, CGAL::to_double(ca.supporting_circle().squared_radius()));
    Point sp(CGAL::to_double(ca.source().x()),
	     CGAL::to_double(ca.source().y()));
    Point tp(CGAL::to_double(ca.target().x()),
	     CGAL::to_double(ca.target().y()));
    layers_[cur_layer_]->new_circular_arc(c, sp, tp);
  }
  template <class C, class P>
  void new_circular_arc(const C &ca, const P &s, const P &t) {
    Point center(CGAL::to_double(ca.center().x()),
		 CGAL::to_double(ca.center().y()));
    Circle c(center, CGAL::to_double(ca.squared_radius()));
    Point sp(CGAL::to_double(s.x()),
	     CGAL::to_double(s.y()));
    Point tp(CGAL::to_double(t.x()),
	     CGAL::to_double(t.y()));
    layers_[cur_layer_]->new_circular_arc(c, sp, tp);
  }

  /*template <class K>
  void new_circular_arc(const Circle &c,
			const Point &s,
			const Point &t) {
    layers_[cur_layer_]->new_circular_arc(c, s, t);
    }*/
  
  template <class S>
  void new_segment(const S &ci) {
    Point a(CGAL::to_double(ci.source().x()),
	    CGAL::to_double(ci.source().y()));
    Point b(CGAL::to_double(ci.target().x()),
	    CGAL::to_double(ci.target().y()));
    layers_[cur_layer_]->new_segment(Segment(a,b));
  }
  template <class K>
  void new_line(const CGAL::Line_2<K> &ci) {
    Point a(CGAL::to_double(ci.point().x()),
	    CGAL::to_double(ci.point().y()));
    Vector b(CGAL::to_double(ci.to_vector().x()),
	     CGAL::to_double(ci.to_vector().y()));
    layers_[cur_layer_]->new_line(Line(a,b));
  }

  void new_label(const std::string s) {
    layers_[cur_layer_]->new_label(s);
  }
    
  void set_line_color(CGAL::Color c){
    layers_[cur_layer_]->set_line_color(c);
  }
  void set_point_style(CGAL::PointStyle p){
    layers_[cur_layer_]->set_point_style(p);
  }
  
  bool updating_box() const {
    return layers_[cur_layer_]->updating_box();
  }
  void set_updating_box(bool v) {
    layers_[cur_layer_]->set_updating_box(v);
  }

  void set_layer(unsigned int li);

private: // private data member

  /*void redraw() {
  //std::cout << "Redraw " << std::endl;
  for (unsigned int i=0; i< layers_.size(); ++i){
  layers_[i]->draw();
  }
  }*/
  class QTEV_layer: public CGAL::Qt_widget_layer {
    typedef CGAL::Qt_widget_layer P;
    std::vector<Circle> circles_;
    std::vector<CGAL::Color> circle_colors_;
    std::vector<Point> points_;
    std::vector<CGAL::Color> point_colors_;
    std::vector<CGAL::PointStyle> point_styles_;
    std::vector<Segment> segments_;
    std::vector<CGAL::Color> segment_colors_;
    std::vector<Line> lines_;
    std::vector<CGAL::Color> line_colors_;
    std::vector<Circular_k::Circular_arc_2> arcs_;
    std::vector<CGAL::Color> arc_colors_;
    std::vector<std::pair<int, std::string> > labels_;
    std::vector<CGAL::Color> label_colors_;
    CGAL::Bbox_2 bbox_;
    CGAL::Color cur_color_;
    CGAL::PointStyle cur_point_style_;
    bool ubb_;
    NT scale_;
    QMutex mutex_;

    //struct Conditional_mutex;

    template <class V, class VC>
    CGAL::Color draw(CGAL::Color cc, const V &v, const VC &vc);

    template <class V, class VC, class VS>
    CGAL::Color draw(CGAL::Color cc, const V &v, const VC &vc, const VS &vs);
  public:
    // I can't remember why I even have this
    QTEV_layer(NT scale=1);

    CGAL::Bbox_2 bounding_box();
  
    void draw();

    void new_circle(const Circle &c);
    void new_point(const Point &c);
    void new_circular_arc(const Circle &c, const Point &s, const Point &t);
    void new_segment(const Segment &c);
    
    void new_line(const Line &c);

    void new_label(const std::string str);
    
    void set_line_color(CGAL::Color c);
    void set_point_style(CGAL::PointStyle p);
  
    bool updating_box() const;
    void set_updating_box(bool v);

    void clear();

    //bool set_is_editing(bool tf);

    Point rescale(const Point &p) const;

  };
 

  std::vector<QTEV_layer* > layers_;
  int cur_layer_;
  NT scale_;
  QMutex mutex_;
};

struct Qt_ev_layer_tag{};

typedef CGAL::Label<Qt_ev_layer_tag> Layer;

inline Qt_examiner_viewer_2 &operator<<(Qt_examiner_viewer_2& e,
					const Layer &l) {
  e.set_layer(l.to_index());
  return e;
}

template <class K>
inline Qt_examiner_viewer_2 &operator<<(Qt_examiner_viewer_2& e,
					const typename CGAL::Circle_2<K> &c) {
  e.new_circle(c);
  return e;
}


template <class K>
inline Qt_examiner_viewer_2 &operator<<(Qt_examiner_viewer_2& e,
					const typename CGAL::Point_2<K> &c) {
  e.new_point(c);
  return e;
}

template <class K>
inline Qt_examiner_viewer_2 &operator<<(Qt_examiner_viewer_2& e,
					const typename CGAL::Circular_arc_point_2<K> &c) {
  e.new_point(c);
  return e;
}

template <class K>
inline Qt_examiner_viewer_2 &operator<<(Qt_examiner_viewer_2& e,
					const typename CGAL::Segment_2<K> &c) {
  e.new_segment(c);
  return e;
}

template <class K>
inline Qt_examiner_viewer_2 &operator<<(Qt_examiner_viewer_2& e,
					const typename CGAL::Line_arc_2<K> &c) {
  e.new_segment(c);
  return e;
}

template <class K>
inline Qt_examiner_viewer_2 &operator<<(Qt_examiner_viewer_2& e,
					const typename CGAL::Line_2<K> &c) {
  e.new_line(c);
  return e;
}

inline Qt_examiner_viewer_2 &operator<<(Qt_examiner_viewer_2& e,
					CGAL::Color c) {
  e.set_line_color(c);
  return e;
}


inline Qt_examiner_viewer_2 &operator<<(Qt_examiner_viewer_2& e,
					const std::string c) {
  e.new_label(c);
  return e;
}

template <class K>
inline Qt_examiner_viewer_2 &operator<<(Qt_examiner_viewer_2& e,
					const char * c) {
  e.new_label(std::string(c));
  return e;
}

/*Qt_examiner_viewer_2 &operator<<(Qt_examiner_viewer_2<K>& e,
  CGAL::Object o) {
  typename Qt_examiner_viewer_2::Point_2 p;
  typename K::Line_2 l;
  typename K::Segment_2 s;
  typename K::Circle_2 c;
  if (CGAL::assign(p, o)) {
  e << p;
  } else if (CGAL::assign(s, o)) {
  e << s;
  } else if (CGAL::assign(l, o)) {
  e << l;
  } else if (CGAL::assign(c, o)) {
  e << c;
  }
  return e;
  }*/

inline Qt_examiner_viewer_2 &operator<<(Qt_examiner_viewer_2& e,
					CGAL::PointStyle o) {
  e.set_point_style(o);
  return e;
}

#endif
