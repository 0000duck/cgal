// ======================================================================
//
// Copyright (c) The CGAL Consortium
//
// This software and related documentation is part of an INTERNAL release
// of the Computational Geometry Algorithms Library (CGAL). It is not
// intended for general use.
//
// ----------------------------------------------------------------------
//
// release       : $CGAL_Revision: CGAL-2.3-I-24 $
// release_date  : $CGAL_Date: 2000/12/29 $
//
// file          : include/CGAL/Snap_rounding_2.h
// package       : arr (1.73)
// maintainer    : Eli Packer <elip@post.tau.ac.il>
// author(s)     : Eli Packer
// coordinator   : Tel-Aviv University (Dan Halperin <halperin@math.tau.ac.il>)
//
// ======================================================================
#ifndef CGAL_SR_2_H
#define CGAL_SR_2_H

#ifndef CGAL_LEDA_RATIONAL_H
#include <CGAL/leda_rational.h> 
#endif

#include <iostream.h>

#ifndef CGAL_ENUM_H
#include <CGAL/enum.h>
#endif

#ifndef CGAL_PREDICATES_ON_POINTS_2_H
#include <CGAL/predicates_on_points_2.h>
#endif

#ifndef CGAL_RANDOM_H
#include <CGAL/Random.h>
#endif

#ifndef CGAL_SQUARED_DISTANCE_2_H
#include <CGAL/squared_distance_2.h>
#endif

#ifndef CGAL_POINT_GENERATORS_2_H
#include <CGAL/point_generators_2.h>
#endif

#ifndef CGAL_INTERSECTION_2_H
#include <CGAL/intersection_2.h>
#endif

#ifndef CGAL_TIMER_H
#include <CGAL/Timer.h>
#endif

#ifdef ISR_DEBUG

#ifndef CGAL_LEDA_WINDOW_H
#include <CGAL/IO/leda_window.h>
#endif
#endif
#include <list.h>
#include <set.h>


#ifndef CGAL_REAL_H
#include <CGAL/leda_real.h>
#endif

#include <CGAL/Snap_rounding_kd_2.h>

CGAL_BEGIN_NAMESPACE

#ifdef ISR_DEBUG
typedef CGAL::Window_stream Window_stream;
#endif

template<class Rep_>
class Segment_data {

typedef Rep_ Rep;
typedef typename Rep::FT NT;
typedef CGAL::Segment_2<Rep> Segment_2;
typedef CGAL::Point_2<Rep> Point_2;

private:
  NT x1;
  NT y1;
  NT x2;
  NT y2;

public:
  Segment_data();
  Segment_data(NT inp_x1,NT inp_y1,NT inp_x2,NT inp_y2);
  void debug() const;
  NT get_x1();
  NT get_y1();
  NT get_x2();
  NT get_y2();
  inline void set_data(NT inp_x1,NT inp_y1,NT inp_x2,NT inp_y2);
  void determine_direction();
};

template<class Rep_>
class Hot_Pixel {

typedef Rep_ Rep;
typedef typename Rep::FT NT;
typedef CGAL::Segment_2<Rep> Segment_2;
typedef CGAL::Point_2<Rep> Point_2;

private:
  // (x,y) is the center of the hot pixel, prec is its edge length
  NT x;
  NT y;
  NT prec;
  Segment_2 *right_seg;
  Segment_2 *left_seg;
  Segment_2 *top_seg;
  Segment_2 *bot_seg;

public:
  void debug() const;
  template<class Out>
  void draw(Out &o) const;
  Hot_Pixel(NT inp_x,NT inp_y,NT inp_prec);
  ~Hot_Pixel();
  inline NT get_x() const;
  inline NT get_y() const;
  bool intersect_left(Segment_2 &seg) const;
  bool intersect_right(Segment_2 &seg) const;
  bool intersect_bot(Segment_2 &seg) const;
  bool intersect_top(Segment_2 &seg) const;
  bool intersect(Segment_data<Rep> &seg) const;
};

// a function for compare two hot pixels for the set of hot pixels
template<class Rep_>
struct hot_pixel_auclidian_cmp
{
  bool operator()(const Hot_Pixel<Rep_> *h1,const Hot_Pixel<Rep_> *h2) const;
};

// a function for compare two hot pixels for the set of hot pixels a
// certain segment intersect
template<class Rep_>
struct hot_pixel_dir_cmp
{
  bool operator ()(const Hot_Pixel<Rep_> *h1,const Hot_Pixel<Rep_> *h2);
};


template<class Rep_>
class Snap_rounding_2 {

typedef Rep_ Rep;
typedef typename Rep::FT NT;
typedef CGAL::Segment_2<Rep> Segment_2;
typedef CGAL::Point_2<Rep> Point_2;

private:
  // statistics data
  bool do_stat;
  int stat_current_number_of_vertices;
  int stat_max_number_of_vertices;
  int stat_sum_number_of_vertices;
  CGAL::Timer timer;
  NT stat_max_distance;
  NT stat_average_distance;
  NT stat_sum_distance;

  std::set<Hot_Pixel<Rep> *,hot_pixel_auclidian_cmp<Rep> > hp_set;
  NT prec,min_x,max_x,min_y,max_y;
  std::list<Segment_2> seg_2_list;
  std::list<Segment_data<Rep> > seg_list;
  std::list<std::list<Point_2> > segments_output_list;
  int number_of_segments,number_of_kd_trees;
  Multiple_kd_tree<NT,Hot_Pixel<Rep> *> *mul_kd_tree;
  bool do_isr;

  void find_hot_pixels();
  void find_intersected_hot_pixels(Segment_data<Rep> &seg,
                         std::set<Hot_Pixel<Rep> *,
                         hot_pixel_dir_cmp<Rep> > &hot_pixels_intersected_set,
                         int &number_of_intersections);

  void debug(std::set<Hot_Pixel<Rep>,hot_pixel_dir_cmp<Rep> > &s);
  void reroute_sr(std::set<Hot_Pixel<Rep> *,hot_pixel_dir_cmp<Rep> >
                  &inp_hot_pixels_intersected_set,std::list<Point_2>
                  &seg_output);
  void reroute_isr(std::set<Hot_Pixel<Rep> *,hot_pixel_dir_cmp<Rep> >
                   &inp_hot_pixels_intersected_set,std::list<Point_2>
                   &seg_output,int number_of_intersections,bool first_time);
  void iterate();

public:
  enum Direction {UP_RIGHT,UP_LEFT,DOWN_RIGHT,DOWN_LEFT,UP,DOWN,LEFT,
                  RIGHT,POINT_SEG};
  static Direction seg_dir;
  static bool erase_hp;
  static inline Direction get_direction() {return(seg_dir);}
  static inline void set_direction(Direction dir) {seg_dir = dir;}
  static inline bool get_erase_hp() {return(erase_hp);}
  Snap_rounding_2(typename std::list<Segment_2>::const_iterator begin,
                  typename std::list<Segment_2>::const_iterator end,
                  NT inp_prec,bool inp_do_isr = true,
                  int inp_number_of_kd_trees = 5);
#ifdef ISR_DEBUG
  template<class Out>
  void output_distances(Out &o);
#endif

  typedef typename std::list<std::list<CGAL::Point_2<Rep> > >
             Polylines_container;
  typedef typename Polylines_container::const_iterator Polyline_const_iterator;
  typedef typename Polylines_container::iterator Polyline_iterator;
  typedef typename std::list<CGAL::Point_2<Rep> > Points_container;
  typedef typename Points_container::const_iterator Point_const_iterator;
  inline Polyline_const_iterator polylines_begin() const {
             return(segments_output_list.begin());}
  inline Polyline_const_iterator polylines_end() const {
             return(segments_output_list.end());}
  typedef typename std::list<Segment_2> Segments_container;
  typedef Segments_container::const_iterator Segment_const_iterator;
  typedef Segments_container::iterator Segment_iterator;
  inline Segment_const_iterator segments_begin() const {
             return(seg_2_list.begin());}
  inline Segment_const_iterator segments_end() const {return(seg_2_list.end());}
  inline Segment_iterator segments_begin() {return(seg_2_list.begin());}
  inline Segment_iterator segments_end() {return(seg_2_list.end());}

  // statistics functions
  inline double average_number_of_vertices() const {return(double(stat_sum_number_of_vertices) / number_of_segments);}
  inline int max_number_of_vertices() const {return(stat_max_number_of_vertices);}
  inline NT max_distance() const {return(stat_max_distance);}
  inline NT average_distance() const {return(stat_average_distance);}
  inline double time() const {return(timer.time());}
  inline int number_of_input_segments() const {return(number_of_segments);}

  template<class Out>
  void output(Out &o);
#ifdef ISR_DEBUG
  void window_output(Window_stream &w,bool wait_for_click);
#endif
};

#ifdef ISR_TIMER
#include <CGAL/Timer.h>
static double time_for_is = 0;
#endif

#ifdef ISR_DEBUG
#include <CGAL/squared_distance_2.h>

#endif

#ifdef ISR_DEBUG
int max_rec = 1,cur_rec = -1,cur_max,needed_hp = 0,unneeded_hp = 0;
#elif defined ISR_TIMER
int needed_hp = 0,unneeded_hp = 0;
#endif

template<class Rep_>
Segment_data<Rep_>::Segment_data() {}
template<class Rep_>
Segment_data<Rep_>::Segment_data(NT inp_x1,NT inp_y1,NT inp_x2,NT inp_y2) :
                    x1(inp_x1),y1(inp_y1),x2(inp_x2),y2(inp_y2) {}

template<class Rep_>
void Segment_data<Rep_>::debug() const {std::cerr << "Segment (" << x1 << ","
     << y1 << "):(" << x2 << ":" << y2 << ")\n";}

template<class Rep_>
typename Rep_::FT Segment_data<Rep_>::get_x1() {return(x1);}

template<class Rep_>
typename Rep_::FT Segment_data<Rep_>::get_y1() {return(y1);}

template<class Rep_>
typename Rep_::FT Segment_data<Rep_>::get_x2() {return(x2);}

template<class Rep_>
typename Rep_::FT Segment_data<Rep_>::get_y2() {return(y2);}

template<class Rep_>
inline void Segment_data<Rep_>::set_data(NT inp_x1,NT inp_y1,NT inp_x2,
            NT inp_y2)
  {
     x1 = inp_x1;
     y1 = inp_y1;
     x2 = inp_x2;
     y2 = inp_y2;
  }

template<class Rep_>
void Segment_data<Rep_>::determine_direction()
  {
    if(x1 < x2) {
      if(y1 < y2)
        Snap_rounding_2<Rep_>::set_direction(Snap_rounding_2<Rep_>::UP_RIGHT);
      else if(y1 == y2)
        Snap_rounding_2<Rep_>::set_direction(Snap_rounding_2<Rep_>::RIGHT);
      else
        Snap_rounding_2<Rep_>::set_direction(Snap_rounding_2<Rep_>::DOWN_RIGHT);
    } else if(x1 == x2) {
      if(y1 < y2)
        Snap_rounding_2<Rep_>::set_direction(Snap_rounding_2<Rep_>::UP);
      else if(y1 == y2)
        Snap_rounding_2<Rep_>::set_direction(Snap_rounding_2<Rep_>::POINT_SEG);
      else
        Snap_rounding_2<Rep_>::set_direction(Snap_rounding_2<Rep_>::DOWN);
    } else {
      if(y1 < y2)
        Snap_rounding_2<Rep_>::set_direction(Snap_rounding_2<Rep_>::UP_LEFT);
      else if(y1 == y2)
        Snap_rounding_2<Rep_>::set_direction(Snap_rounding_2<Rep_>::LEFT);
      else
        Snap_rounding_2<Rep_>::set_direction(Snap_rounding_2<Rep_>::DOWN_LEFT);
    }
  }


template<class Rep_>
void Hot_Pixel<Rep_>::debug() const {std::cerr << "Hot Pixel (" << x << ":"
                      << y << ")\n";}

template<class Rep_>
template<class Out> void Hot_Pixel<Rep_>::draw(Out &o) const
  {
    o << *right_seg;
    o << *left_seg;
    o << *top_seg;
    o << *bot_seg;
  }

  // intersection pixel
template<class Rep_>
Hot_Pixel<Rep_>::Hot_Pixel(NT inp_x,NT inp_y,NT inp_prec) : prec(inp_prec)
  {
    x = NT(floor((inp_x / prec).to_double())) * prec + prec / 2.0;

    y = NT(floor((inp_y / prec).to_double())) * prec + prec / 2.0;

    right_seg = new Segment_2(Point_2(x + prec / 2.0,y - prec / 2.0),
                              Point_2(x + prec / 2.0,y + prec / 2.0));
    left_seg = new Segment_2(Point_2(x - prec / 2.0,y - prec / 2.0),
                             Point_2(x - prec / 2.0,y + prec / 2.0));
    top_seg = new Segment_2(Point_2(x - prec / 2.0,y + prec / 2.0),
                            Point_2(x + prec / 2.0,y + prec / 2.0));
    bot_seg = new Segment_2(Point_2(x - prec / 2.0,y - prec / 2.0),
                            Point_2(x + prec / 2.0,y - prec / 2.0));
  }

template<class Rep_>
Hot_Pixel<Rep_>::~Hot_Pixel()
  {
    if(Snap_rounding_2<Rep_>::get_erase_hp()) {
      delete(right_seg);
      delete(left_seg);
      delete(top_seg);
      delete(bot_seg);
    }
  }

template<class Rep_>
inline typename Rep_::FT Hot_Pixel<Rep_>::get_x() const {return(x);}

template<class Rep_>
inline typename Rep_::FT Hot_Pixel<Rep_>::get_y() const {return(y);}

template<class Rep_>
bool Hot_Pixel<Rep_>::intersect_left(Segment_2 &seg) const
  {
    CGAL::Object result;    
    Point_2 p;
    Segment_2 s;

    result = CGAL::intersection(seg,*left_seg);

    if(CGAL::assign(p,result)) {
      NT tmp = y + prec / 2.0;
      return(p.y() != tmp || Snap_rounding_2<Rep_>::get_direction() ==
             Snap_rounding_2<Rep_>::UP_LEFT && seg.source().y() != tmp ||
             Snap_rounding_2<Rep_>::get_direction() ==
             Snap_rounding_2<Rep_>::DOWN_RIGHT && seg.target().y() != tmp);
    } else if(CGAL::assign(s,result))
      return(true);
    else
      return(false);
  }


template<class Rep_>
bool Hot_Pixel<Rep_>::intersect_right(Segment_2 &seg) const
  {
    CGAL::Object result;    
    Point_2 p;
    Segment_2 s;
    
    result = CGAL::intersection(seg,*right_seg);

    if(CGAL::assign(p,result)) {
      // bottom right point was checked in intersect_bot

      NT tmp = y + prec / 2.0;
      if(p.y() == tmp)
        return(Snap_rounding_2<Rep_>::get_direction() ==
               Snap_rounding_2<Rep_>::UP_RIGHT && seg.source().y() != tmp ||
               Snap_rounding_2<Rep_>::get_direction() ==
               Snap_rounding_2<Rep_>::DOWN_LEFT && seg.target().y() != tmp);
      else if(p.y() == y - prec / 2.0)
        return(false);// was checked
      else
        return((Snap_rounding_2<Rep_>::get_direction() ==
                Snap_rounding_2<Rep_>::LEFT ||
                Snap_rounding_2<Rep_>::get_direction() ==
                Snap_rounding_2<Rep_>::DOWN_LEFT ||
                Snap_rounding_2<Rep_>::get_direction() ==
                Snap_rounding_2<Rep_>::UP_LEFT) && seg.target().x() !=
                x + prec / 2.0 ||
                (Snap_rounding_2<Rep_>::get_direction() ==
                Snap_rounding_2<Rep_>::RIGHT ||
                Snap_rounding_2<Rep_>::get_direction() ==
                Snap_rounding_2<Rep_>::DOWN_RIGHT ||
                Snap_rounding_2<Rep_>::get_direction() ==
                Snap_rounding_2<Rep_>::UP_RIGHT) && seg.source().x() !=
                x + prec / 2.0);
    } else
      return(false);
  }

template<class Rep_>
bool Hot_Pixel<Rep_>::intersect_bot(Segment_2 &seg) const
  {
    CGAL::Object result;
    Point_2 p;
    Segment_2 s;

    result = CGAL::intersection(seg,*bot_seg);

    if(CGAL::assign(p,result)) {
      NT tmp = x + prec / 2.0;
      return(p.x() != tmp || Snap_rounding_2<Rep_>::get_direction() ==
             Snap_rounding_2<Rep_>::UP_LEFT && seg.target().x() != tmp ||
             Snap_rounding_2<Rep_>::get_direction() ==
             Snap_rounding_2<Rep_>::DOWN_RIGHT && seg.source().x() != tmp);
    } else if(CGAL::assign(s,result))
      return(true);
    else
      return(false);
  }

template<class Rep_>
bool Hot_Pixel<Rep_>::intersect_top(Segment_2 &seg) const
  {
    CGAL::Object result;
    Point_2 p;
    Segment_2 s;
    
    result = CGAL::intersection(seg,*top_seg);

    if(CGAL::assign(p,result)) {
      // corner points was checked in intersect_bot
      NT tar_y = seg.target().y(),sou_y = seg.source().y();

      if(p.x() == x - prec / 2.0 || p.x() == x + prec / 2.0)
        return(false);// were checked
      else
        return((Snap_rounding_2<Rep_>::get_direction() ==
               Snap_rounding_2<Rep_>::DOWN ||
               Snap_rounding_2<Rep_>::get_direction() ==
               Snap_rounding_2<Rep_>::DOWN_LEFT ||
               Snap_rounding_2<Rep_>::get_direction() ==
               Snap_rounding_2<Rep_>::DOWN_RIGHT) && tar_y != y + prec / 2.0 ||
               (Snap_rounding_2<Rep_>::get_direction() ==
               Snap_rounding_2<Rep_>::UP ||
               Snap_rounding_2<Rep_>::get_direction() ==
               Snap_rounding_2<Rep_>::UP_LEFT ||
               Snap_rounding_2<Rep_>::get_direction() ==
               Snap_rounding_2<Rep_>::UP_RIGHT) && sou_y != y + prec / 2.0);
    } else
    return(false);
  }

template<class Rep_>
bool Hot_Pixel<Rep_>::intersect(Segment_data<Rep_> &seg) const
  {
    Segment_2 s(Point_2(seg.get_x1(),seg.get_y1()),Point_2(seg.get_x2(),
                seg.get_y2()));

    return(intersect_bot(s) || intersect_left(s) || intersect_right(s) ||
           intersect_top(s));
  }

// a function for compare two hot pixels for the set of hot pixels
template<class Rep_>
bool hot_pixel_auclidian_cmp<Rep_>::operator()(const Hot_Pixel<Rep_> *h1,
     const Hot_Pixel<Rep_> *h2) const
  {
    return(h1->get_x() < h2->get_x() ||
         h1->get_x() == h2->get_x() && h1->get_y() < h2->get_y());
  }

// a function for compare two hot pixels for the set of hot pixels a certain
// segment intersect
template<class Rep_>
bool hot_pixel_dir_cmp<Rep_>::operator ()(const Hot_Pixel<Rep_> *h1,const Hot_Pixel<Rep_> *h2) 
  {
    return(
	   // Point segment intersects only one pixel, thus ignored
        Snap_rounding_2<Rep_>::get_direction() ==
           Snap_rounding_2<Rep_>::UP_RIGHT &&
           (h1->get_x() < h2->get_x() || 
           h1->get_x() == h2->get_x() && h1->get_y() < h2->get_y()) ||
        Snap_rounding_2<Rep_>::get_direction() ==
           Snap_rounding_2<Rep_>::UP_LEFT &&
           (h1->get_x() > h2->get_x() || 
           h1->get_x() == h2->get_x() && h1->get_y() < h2->get_y()) ||
        Snap_rounding_2<Rep_>::get_direction() ==
           Snap_rounding_2<Rep_>::DOWN_RIGHT &&
           (h1->get_x() < h2->get_x() || 
           h1->get_x() == h2->get_x() && h1->get_y() > h2->get_y()) ||
        Snap_rounding_2<Rep_>::get_direction() ==
           Snap_rounding_2<Rep_>::DOWN_LEFT &&
           (h1->get_x() > h2->get_x() || 
           h1->get_x() == h2->get_x() && h1->get_y() > h2->get_y()) ||
        Snap_rounding_2<Rep_>::get_direction() == Snap_rounding_2<Rep_>::UP &&
           h1->get_y() < h2->get_y() ||
        Snap_rounding_2<Rep_>::get_direction() == Snap_rounding_2<Rep_>::DOWN &&
           h1->get_y() > h2->get_y() ||
        Snap_rounding_2<Rep_>::get_direction() == Snap_rounding_2<Rep_>::LEFT &&
           h1->get_x() > h2->get_x() ||
        Snap_rounding_2<Rep_>::get_direction() ==
           Snap_rounding_2<Rep_>::RIGHT &&
	   h1->get_x() < h2->get_x());
  }



template<class Rep_>
void Snap_rounding_2<Rep_>::find_hot_pixels()
  {
    Hot_Pixel<Rep_> *hp;
    typename std::list<Segment_data<Rep_> >::iterator iter1,iter2;
    CGAL::Object result;
    Point_2 p;
    std::list<std::pair<std::pair<NT,NT>,Hot_Pixel<Rep_> *> > hot_pixels_list;
    for(iter1 = seg_list.begin();iter1 != seg_list.end();++iter1) {
      // add hot pixels for end-points
      hp = new Hot_Pixel<Rep_>(iter1->get_x1(),iter1->get_y1(),prec);
      if(hp_set.insert(hp).second) // check that the hot pixel does not exist
        hot_pixels_list.push_back(
        std::pair<std::pair<NT,NT>,Hot_Pixel<Rep_> *>(std::pair<NT,NT>(
        hp->get_x(),hp->get_y()),hp));

      hp = new Hot_Pixel<Rep_>(iter1->get_x2(),iter1->get_y2(),prec);
      if(hp_set.insert(hp).second)
        hot_pixels_list.push_back(std::pair<std::pair<NT,NT>,Hot_Pixel<Rep_> *>
        (std::pair<NT,NT>(hp->get_x(),hp->get_y()),hp));

      // add hot pixels for intersections(overlappings do not count for
      // intersections)
      iter2 = iter1;
      for(++iter2;iter2 != seg_list.end();++iter2) {
        result = CGAL::intersection(Segment_2(Point_2(iter1->get_x1(),
                 iter1->get_y1()),Point_2(iter1->get_x2(),iter1->get_y2())),
                 Segment_2(Point_2(iter2->get_x1(),iter2->get_y1()),
                 Point_2(iter2->get_x2(),iter2->get_y2())));
        if(CGAL::assign(p,result)) {
          hp = new Hot_Pixel<Rep_>(p.x(),p.y(),prec);
          if(hp_set.insert(hp).second)
            hot_pixels_list.push_back(std::pair<std::pair<NT,NT>,
            Hot_Pixel<Rep_> *>(std::pair<NT,NT>(hp->get_x(),hp->get_y()),hp));
        }
      }
    }

    // create kd multiple tree
    // create simple_list from seg_list
    std::list<std::pair<std::pair<NT,NT>,std::pair<NT,NT> > > simple_seg_list;
    for(typename std::list<Segment_data<Rep_> >::iterator iter =
        seg_list.begin();iter != seg_list.end();++iter) {
      std::pair<NT,NT> first(iter->get_x1(),iter->get_y1()),
   	          second(iter->get_x2(),iter->get_y2());
      simple_seg_list.push_back(std::pair<std::pair<NT,NT>,std::pair<NT,NT> >(
                                first,second));
    }

    mul_kd_tree = new Multiple_kd_tree<NT,Hot_Pixel<Rep_> *>(hot_pixels_list,
                  number_of_kd_trees,simple_seg_list);
  }

template<class Rep_>
void Snap_rounding_2<Rep_>::find_intersected_hot_pixels(Segment_data<Rep_> &seg,
                    std::set<Hot_Pixel<Rep_> *,
                    hot_pixel_dir_cmp<Rep_> > &hot_pixels_intersected_set,
                    int &number_of_intersections)
  {
    typename std::list<Hot_Pixel<Rep_> *>::iterator iter;

    hot_pixels_intersected_set.clear();
    seg.determine_direction();
    number_of_intersections = 0;

    std::list<Hot_Pixel<Rep_> *> hot_pixels_list;
    mul_kd_tree->get_intersecting_points(hot_pixels_list,
	   Segment_2(Point_2(seg.get_x1(),seg.get_y1()),
           Point_2(seg.get_x2(),seg.get_y2())),prec);

    for(iter = hot_pixels_list.begin();iter != hot_pixels_list.end();++iter) {
      if((*iter)->intersect(seg)) {

#if defined ISR_DEBUG || defined ISR_TIMER
        ++needed_hp;
#endif

        ++number_of_intersections;
        hot_pixels_intersected_set.insert(*iter);
      }
#if defined ISR_DEBUG || defined ISR_TIMER
        else
          ++unneeded_hp;
#endif
    }
  }

template<class Rep_>
void debug(std::set<Hot_Pixel<Rep_>,hot_pixel_dir_cmp<Rep_> > &s)
  {
    std::cerr << "  Debugging inp_hot_pixels_intersected_set\n";
    for(typename std::set<Hot_Pixel<Rep_>,hot_pixel_dir_cmp<Rep_> >::
        iterator iter = s.begin();iter != s.end();++iter) {
      std::cerr << "    ";
      iter->debug();
    }
    std::cerr << "  Finish Debugging inp_hot_pixels_intersected_set\n";
  }

template<class Rep_>
void Snap_rounding_2<Rep_>::reroute_sr(std::set<Hot_Pixel<Rep_> *,
     hot_pixel_dir_cmp<Rep_> > &inp_hot_pixels_intersected_set,
     std::list<Point_2> &seg_output)
  {
    typename std::set<Hot_Pixel<Rep_> *,
    hot_pixel_dir_cmp<Rep_> >::iterator hot_pixel_iter =
    inp_hot_pixels_intersected_set.begin();
    ++hot_pixel_iter;

    while(hot_pixel_iter != inp_hot_pixels_intersected_set.end()) {
      seg_output.push_back(Point_2((*hot_pixel_iter)->get_x(),
                           (*hot_pixel_iter)->get_y()));
      ++hot_pixel_iter;
      ++stat_current_number_of_vertices;
    }

  }

template<class Rep_>
void Snap_rounding_2<Rep_>::reroute_isr(std::set<Hot_Pixel<Rep_> *,
     hot_pixel_dir_cmp<Rep_> > &inp_hot_pixels_intersected_set,
     std::list<Point_2> &seg_output,int number_of_intersections,bool first_time)
  {
    typename std::set<Hot_Pixel<Rep_> *,hot_pixel_dir_cmp<Rep_> >::
      iterator hot_pixel_iter,next_hot_pixel_iter,before_last_hot_pixel_iter;
    Segment_data<Rep_> seg;
    std::set<Hot_Pixel<Rep_> *,hot_pixel_dir_cmp<Rep_> >
      hot_pixels_intersected_set;

    if(number_of_intersections > 2 || first_time) {
      before_last_hot_pixel_iter = inp_hot_pixels_intersected_set.end();
      --before_last_hot_pixel_iter;

      for(hot_pixel_iter = inp_hot_pixels_intersected_set.begin();
          hot_pixel_iter != before_last_hot_pixel_iter;++hot_pixel_iter) {
        next_hot_pixel_iter = hot_pixel_iter;
        ++next_hot_pixel_iter;
        seg.set_data((*hot_pixel_iter)->get_x(),(*hot_pixel_iter)->get_y(),
            (*next_hot_pixel_iter)->get_x(),(*next_hot_pixel_iter)->get_y());
        seg.determine_direction();
        find_intersected_hot_pixels(seg,hot_pixels_intersected_set,
            number_of_intersections);
        reroute_isr(hot_pixels_intersected_set,seg_output,
            number_of_intersections,false);
      }
    } else {      
      // insert second hot pixel
      hot_pixel_iter = inp_hot_pixels_intersected_set.begin();
      ++hot_pixel_iter;
      seg_output.push_back(Point_2((*hot_pixel_iter)->get_x(),
          (*hot_pixel_iter)->get_y()));
      ++stat_current_number_of_vertices;
    }
  }


template<class Rep_>
void Snap_rounding_2<Rep_>::iterate()
  {
    std::list<Point_2> seg_output;
    std::set<Hot_Pixel<Rep_> *,hot_pixel_dir_cmp<Rep_> >
      hot_pixels_intersected_set;
    typename std::set<Hot_Pixel<Rep_> *,hot_pixel_dir_cmp<Rep_> >::
      iterator hot_pixel_iter;
    int number_of_intersections;
    Hot_Pixel<Rep_> *hp;

    for(typename std::list<Segment_data<Rep_> >::iterator iter =
        seg_list.begin();iter != seg_list.end();++iter) {
      stat_current_number_of_vertices = 1;
      seg_output.clear();
      iter->determine_direction();
      find_intersected_hot_pixels(*iter,hot_pixels_intersected_set,
        number_of_intersections);

      // hot_pixels_intersected_set must have at least two hot pixels when the
      // segment is not in entirely inside a hot pixel enter first hot pixel
      hot_pixel_iter = hot_pixels_intersected_set.begin();
      if(hot_pixel_iter == hot_pixels_intersected_set.end()) {
        // segment entirely inside a pixel
        hp = new Hot_Pixel<Rep_>(iter->get_x1(),iter->get_y1(),prec);
        seg_output.push_back(Point_2(hp->get_x(),hp->get_y()));
        erase_hp = true;
        delete(hp);
        erase_hp = false;
      } else
        seg_output.push_back(Point_2((*hot_pixel_iter)->get_x(),
                             (*hot_pixel_iter)->get_y()));
        if(number_of_intersections > 1) {
          // segments that have at most one intersecting hot pixel are
          // done(it was inserted)
          if(do_isr)
            reroute_isr(hot_pixels_intersected_set,seg_output,
                        number_of_intersections,true);
          else
            reroute_sr(hot_pixels_intersected_set,seg_output);
      }

      if(stat_current_number_of_vertices > stat_max_number_of_vertices)
        stat_max_number_of_vertices = stat_current_number_of_vertices;
      stat_sum_number_of_vertices += stat_current_number_of_vertices;

      segments_output_list.push_back(seg_output);
    }
  }

template<class Rep_>
Snap_rounding_2<Rep_>::Snap_rounding_2(typename std::list<Segment_2>::
  const_iterator begin,typename std::list<Segment_2>::const_iterator end,
  NT inp_prec,bool inp_do_isr = true,int inp_number_of_kd_trees = 5)
  {
    timer.start();

    stat_max_number_of_vertices = 0;
    stat_sum_number_of_vertices = 0;
    stat_max_distance = 0;
    stat_sum_distance = 0;

    erase_hp = false;
    do_isr = inp_do_isr;
    prec = inp_prec;
    number_of_segments = 0;
    number_of_kd_trees = inp_number_of_kd_trees;

    // copy segments list
    while(begin != end) {
      seg_list.push_back(Segment_data<Rep_>(begin->source().x(),
                         begin->source().y(),begin->target().x(),
                         begin->target().y()));
      seg_2_list.push_back(*begin);
      ++number_of_segments;
      ++begin;
    }

    find_hot_pixels();
    iterate();

    timer.stop();
  }

#ifdef ISR_DEBUG
template<class Rep_>
template<class Out> void Snap_rounding_2<Rep_>::output_distances(Out &o)
  {
    double max = 0,max_seg_dis,evarage_dis,cur_dis;
    std::list<Segment_data<Rep_> >::iterator orig_iter = seg_list.begin();

    for(std::list<std::list<std::pair<NT,NT> > >::iterator iter1 =
        segments_output_list.begin();iter1 != segments_output_list.end();
        ++iter1) {
      max_seg_dis = 0;
      for(std::list<std::pair<NT,NT> >::iterator iter2 = iter1->begin();
          iter2 != iter1->end();++iter2) {
        cur_dis = sqrt(CGAL::squared_distance(Point_2(iter2->first,
          iter2->second),Segment_2(Point_2(orig_iter->get_x1(),
          orig_iter->get_y1()),Point_2(orig_iter->get_x2(),
          orig_iter->get_y2()))).to_double());
        if(cur_dis > max_seg_dis)
          max_seg_dis = cur_dis;
      }

      evarage_dis += max_seg_dis;
      if(max_seg_dis > max)
        max = max_seg_dis;

      ++orig_iter;
    }
    evarage_dis /= seg_list.size();

    std::cerr << "max distance between output and original is " << max << endl;
    std::cerr << "evarage distance between output and original is " <<
                 evarage_dis << endl;
  }
#endif

template<class Rep_>
template<class Out> void Snap_rounding_2<Rep_>::output(Out &o)
  {
    o << number_of_segments << endl;

    for(typename std::list<std::list<Point_2> >::iterator iter1 =
        segments_output_list.begin();iter1 != segments_output_list.end();
        ++iter1) {
      for(typename std::list<Point_2>::iterator iter2 = iter1->begin();
          iter2 != iter1->end();++iter2)
        o << iter2->x().to_double() << " " << iter2->y().to_double() << " ";

      o << endl;
    }
  }

#ifdef ISR_DEBUG
template<class Rep_>
void Snap_rounding_2<Rep_>::window_output(Window_stream &w,bool wait_for_click)
  {
    double x,y;
    bool seg_painted;

    w << CGAL::GREEN;

    //std::cerr << "hp_set size is " << hp_set.size() << endl;

    for(typename std::set<Hot_Pixel<Rep_> *,hot_pixel_auclidian_cmp<Rep_> >::
        iterator iter = hp_set.begin();
        iter != hp_set.end();++iter)
      (*iter)->draw(w);

    // draw original segments
    w << CGAL::BLACK;
    for(typename std::list<Segment_data<Rep_> >::iterator iter =
        seg_list.begin();iter != seg_list.end();++iter) {
      if(iter->get_x1() == iter->get_x2() && iter->get_y1() == iter->get_y2())
        w << Point_2(iter->get_x1(),iter->get_y1());
      else
        w << Segment_2(Point_2(iter->get_x1(),iter->get_y1()),
                       Point_2(iter->get_x2(),iter->get_y2()));
    }

    // draw isr polylines
    w << CGAL::RED;
    typename std::list<Point_2>::iterator iter2,iter3;

    for(typename std::list<std::list<Point_2> >::iterator iter1 =
        segments_output_list.begin();iter1 != segments_output_list.end();
        ++iter1) {
      if(wait_for_click)
        w.read_mouse(x,y);
      iter2 = iter3 = iter1->begin();
      seg_painted = false;
      for(++iter2;iter2 != iter1->end();++iter2) {
        seg_painted = true;
        w << Segment_2(*iter2,*iter3);
        ++iter3;
      }

      if(!seg_painted) { // segment entirely inside hot pixel
        --iter2;
        w << *iter2;
      }
    }

    int mouse_input;
    while(true) {
      mouse_input = w.read_mouse(x,y);
      if(mouse_input == 1)
        return;
    }
  }
#endif

template<class Rep>
Snap_rounding_2<Rep>::Direction Snap_rounding_2<Rep>::seg_dir;

template<class Rep>
bool Snap_rounding_2<Rep>::erase_hp = false;

CGAL_END_NAMESPACE

#endif // CGAL_ISR_2_H
