// ======================================================================
//
// Copyright (c) 1997 The CGAL Consortium
//
// This software and related documentation is part of an INTERNAL release
// of the Computational Geometry Algorithms Library (CGAL). It is not
// intended for general use.
//
// ----------------------------------------------------------------------
//
// release       : $CGAL_Revision: CGAL-2.3-I-44 $
// release_date  : $CGAL_Date: 2001/03/09 $
//
// file          : include/CGAL/Sweep_curves_to_subcurves.h
// package       : arr (1.87)
// maintainer    : Eyal Flato <flato@math.tau.ac.il>
// source        : 
// revision      : 
// revision_date : 
// author(s)     : Eti Ezra <estere@post.tau.ac.il>
//
//
// coordinator   : Tel-Aviv University (Dan Halperin <halperin@math.tau.ac.il>)
//
// Chapter       : 
// ======================================================================

#ifndef CGAL_SWEEP_CURVES_TO_SUBCURVES_H
#define CGAL_SWEEP_CURVES_TO_SUBCURVES_H

#include <vector>
#include <list>

#ifndef CGAL_IN_PLACE_LIST_H
#include <CGAL/In_place_list.h>
#endif

#ifndef CGAL_MEMORY_H
#include <CGAL/memory.h>
#endif

//#ifndef CGAL_HANDLE_H
//#include <CGAL/Handle.h>
//#endif

#ifndef CGAL_HANDLE_FOR_H
#include <CGAL/Handle_for.h>
#endif

#ifndef CGAL_ASSERTIONS_H
#include <CGAL/assertions.h>
#endif

#ifndef CGAL_SWEEP_CURVES_BASE_H
#include <CGAL/Sweep_line_2/Sweep_curves_base.h>
#endif

#include <CGAL/Timer.h>

//#include <CGAL/IO/leda_window.h>  //used for visualization -

CGAL_BEGIN_NAMESPACE

struct Sweep_curves_to_subcurves_utils { 
  
  // X_curve_plus_id:
  // holds a curve and its id number. 
  // The addition of id number to a curve was made due to overlapping 
  // (in which some binary predicates return EQUAL, 
  // while we are interseted in sharp order relation. 
  template <class Traits_> 
  class X_curve_plus_id : public Traits_::X_curve
  {
  public:
    typedef Traits_                       Traits;
    typedef typename Traits::X_curve      curve; 
    typedef typename Traits::Point        Point;
    
    X_curve_plus_id() : curve() {};
    
    X_curve_plus_id(const curve &cv, unsigned int i) : curve(cv) , _id(i) {}
    
    X_curve_plus_id(const X_curve_plus_id &cv) : curve(cv) , _id(cv.id()) {}
    
    ~X_curve_plus_id(){}
    
    X_curve_plus_id& operator=(const X_curve_plus_id &cv)
    {
      curve::operator=(cv);
      _id = cv.id();
      return *this;
    }
    
    bool operator==(const X_curve_plus_id &cv) const
    {
      Traits traits;
      
      return (_id == cv.id() && traits.curve_is_same(*this, cv));
      
      //return curve::operator==(cv);
    }
    
    void  set_id(unsigned int i) { _id = i; }
    
    unsigned int id() const { return _id; }
    
  protected:
    unsigned int _id;
  };
};

//template <class Traits_>
//class Point_rep;

template <class Traits_>
class Point_handle;

// Point_rep:
// Point_rep holds only a Point. This class holds the representation, 
// and the next will hold the Handle to Point.
template <class Traits_>
class Point_rep : public Ref_counted  {
public:
  typedef Traits_                       Traits;
  typedef typename Traits::Point        Point;
  
    
  Point_rep() {}
  
  Point_rep(const Point& p) : p_(p) {}
  
  ~Point_rep() {}
  
protected:
  
  friend class Point_handle<Traits_>;    
  
  Point p_;
  };

// Point_handle:
// Here I used an optimization of using handle_for instead of handle: the advatage here that hande_for uses CGAL_allocator instead 
// of the usual new and delete operators.
template <class Traits_>
class Point_handle : public Handle_for<Point_rep<Traits_> > {
  /*typedef Alloc_                                   Allocator;
    typedef typename Allocator::value_type          value_type;
    typedef typename Allocator::pointer             pointer;
    typedef typename Allocator::const_pointer       const_pointer;
    typedef typename Allocator::reference           reference;
    typedef typename Allocator::const_reference     const_reference;
    typedef typename Allocator::size_type           size_type;
    typedef typename Allocator::difference_type     difference_type;*/
  typedef Handle_for<Point_rep<Traits_> >           Handle_for_Point_rep;
  
public:
  typedef Traits_                          Traits;
  typedef typename Traits::Point           Point;
  typedef Point_rep<Traits>                Point_rep_traits;
    
  Point_handle() : Handle_for_Point_rep() {
#ifdef CGAL_MAKE_PROFILING
    std::cout<<"allocating handle for DEFAULT Point_rep_traits"<<endl;
#endif
  }
  
  Point_handle(const Point& p) : Handle_for_Point_rep(Point_rep_traits(p)) {  
#ifdef CGAL_MAKE_PROFILING
    std::cout<<"allocating handle for Point_rep_traits(p)" << p << endl;
#endif
    //PTR = allocator.allocate(1);
    //allocator.construct(ptr(), Point_rep_traits(p));
    
    //  PTR = new Point_rep_traits(p); 
  }
  
  Point_handle(const Point_handle& p) : Handle_for_Point_rep(p) {}
    
  ~Point_handle() {  // cout<<" ~Point_handle() "<<endl; 
  }
  
  Point_handle& operator=(const Point_handle &p) {
    Handle_for_Point_rep::operator=(p);
    //ptr()->p_ = p.point();
    return *this;
  }
  
  bool operator==(const Point_handle &p) const
  { return ptr()->p_ == p.point(); }
  
  bool operator!=(const Point_handle &p) const
  { return  !(operator==(p));  //ptr()->p_ != p.point(); 
  }
  
  void set_point(const Point& p) { ptr()->p_ = p; }
  
  const Point& point() const { return ptr()->p_; }
    
private:
};

template <class Curve_iterator_,  class Traits_>
class Sweep_curves_to_subcurves : 
  public Sweep_curves_base<Curve_iterator_, Traits_, Point_handle<Traits_>, 
                           Sweep_curves_to_subcurves_utils::X_curve_plus_id<Traits_> > 
{
public:
  typedef Curve_iterator_                                               Curve_iterator;
  typedef Traits_                                                       Traits;
  typedef Point_handle<Traits>                                          Point_handle_traits;
  typedef Sweep_curves_to_subcurves_utils::X_curve_plus_id<Traits>      X_curve_plus;
  
  typedef typename  Traits::X_curve                        X_curve;
  typedef typename  Traits::Point                          Point;

  typedef Sweep_curves_base<Curve_iterator, Traits, Point_handle_traits, X_curve_plus>  Base;
  
  typedef typename Base::Curve_node                  Curve_node;
  typedef typename Base::Intersection_point_node     Intersection_point_node;
  typedef typename Base::Points_iterator             Points_iterator;  
  typedef typename Base::Points_const_iterator       Points_const_iterator;
  typedef typename Base::Curve_node_iterator         Curve_node_iterator;
  typedef typename Base::Curve_node_const_iterator   Curve_node_const_iterator;
  typedef typename Base::X_curve_list                X_curve_list;
  typedef typename Base::X_curve_list_iterator       X_curve_list_iterator;
  typedef typename Base::Vertices_points_plus        Vertices_points_plus;
  typedef typename Base::Event_queue                 Event_queue;
  typedef typename Base::Status_line                 Status_line;
 
  typedef typename Event_queue::value_type           Event_queue_value_type;
  typedef typename Status_line::value_type           Status_line_value_type;
  

  template <class Container>
  void  sweep_curves_to_subcurves(Curve_iterator curves_begin, 
                                  Curve_iterator curves_end, 
                                  Container &subcurves, 
                                  bool overlapping = false)
  { 
    Traits                traits;
    Event_queue           event_queue;
    Status_line           status;
    
    //int c_sweep_t;
    //c_sweep_t = clock();
    
#ifdef  CGAL_SWEEP_LINE_DEBUG
    unsigned int n = 0;
    for (Curve_iterator cv_iter = curves_begin; 
         cv_iter !=  curves_end; cv_iter++, n++);
    cout<<"number of edges on input "<< n <<std::endl;
#endif

    CGAL::Timer pre_preprocess_t;
    pre_preprocess_t.start();   
    
    // splitting all curves to x-monotone curves.
    X_curve_list  x_monotone_curves;
    for(Curve_iterator cv_iter = curves_begin; 
        cv_iter != curves_end; ++cv_iter){
      if (!traits.is_x_monotone(*cv_iter)) {
        X_curve_list x_monotone_subcurves;
        traits.make_x_monotone(*cv_iter, x_monotone_subcurves);

#ifdef  CGAL_SWEEP_LINE_DEBUG
        std::cout<<"printing x-monotone parts"<<std::endl;
#endif
        for(X_curve_list_iterator iter = x_monotone_subcurves.begin(); 
            iter != x_monotone_subcurves.end(); ++iter){
#ifdef  CGAL_SWEEP_LINE_DEBUG
          std::cout<<*iter<<endl;
#endif
          x_monotone_curves.push_back(*iter);  
        }
      }
      else
        x_monotone_curves.push_back(*cv_iter);
    }
    pre_preprocess_t.stop();
    std::cout<<"The time required to pre precossing the input : "<< pre_preprocess_t.time()<<std::endl;
    

    CGAL::Timer prepare_DS_t;
    prepare_DS_t.start();   
    // now creating the Curve_node handles and the event queue.
    unsigned int id = 0;
    for(X_curve_list_iterator cv_iter = x_monotone_curves.begin(); 
        cv_iter != x_monotone_curves.end(); ++cv_iter, ++id){
      
      X_curve cv(*cv_iter);
      if (is_right(traits.curve_source(*cv_iter), 
                   traits.curve_target(*cv_iter)) )
        cv = traits.curve_flip(*cv_iter);
      
#ifdef  CGAL_SWEEP_LINE_DEBUG
      cout<<cv<<std::endl;
#endif
      
      typename Event_queue::iterator  edge_point = 
        event_queue.find( traits.curve_source(cv) );
      // defining one cv_node for both source and target event points. 
      Curve_node  cv_node = Curve_node(X_curve_plus(cv, id), 
                                       traits.curve_source(cv)); 
      
      Intersection_point_node  source_point_node = 
        Intersection_point_node(cv_node, traits.curve_source(cv) );
       
      if (edge_point == event_queue.end() || 
          edge_point->second.get_point() != source_point_node.get_point())
        event_queue.insert(Event_queue_value_type(traits.curve_source(cv), 
						  source_point_node));
      else
        edge_point->second.merge(source_point_node);
      
      
      edge_point = event_queue.find( traits.curve_target(cv) );

      Intersection_point_node  target_point_node = 
        Intersection_point_node(cv_node, traits.curve_target(cv) );

      if (edge_point == event_queue.end() || 
          edge_point->second.get_point() != target_point_node.get_point())
        event_queue.insert(Event_queue_value_type(traits.curve_target(cv), 
						  target_point_node));
      else
        edge_point->second.merge(target_point_node);
    }
    prepare_DS_t.stop();
    std::cout<<"The time required to prepare the Data Structure : "<< prepare_DS_t.time()<<std::endl;
    

    CGAL::Timer sweeping_t;
    sweeping_t.start();   
     // now starting the sweeping.
    unsigned int queue_size = 0;
    bool         event_terminated = true;
    bool         event_overlap_terminated = true;
    while ( !(event_queue.empty()) ){
      ++queue_size;
      // fetch the next event.
      typename Event_queue::iterator  event = event_queue.begin();

      const Point&              event_point = event->first;
      Intersection_point_node&  point_node = event->second;
      //bool                     event_terminated = true;
      
#ifdef  CGAL_SWEEP_LINE_DEBUG     
      cout<<"* * * event point is "<<event_point<<
        " and point node is "<<point_node.get_point().point()<<std::endl;
      CGAL_assertion(event_point == point_node.get_point().point());
#endif
      
      event_terminated = true; // reinitializing event_terminated 
      // to true only after the updating of the subdivision.
     
      // now continue with the sweep line.
      event_terminated = handle_one_event (event_queue, 
                                           status, 
                                           event_point, 
                                           point_node);
      
      // handling overlapping curves. 
      // On each overlapping group, we remove iteratively each 
      // curve and check for new events after the removing.
      // when finish, we reinsert to the status all the overlappting 
      // removed curves.
      for (Curve_node_iterator cv_iter = point_node.curves_begin(); 
           cv_iter != point_node.curves_end(); ++cv_iter){
        //typename Status_line::iterator curr_cv_node = status.find(*cv_iter);
        typename Status_line::iterator curr_cv_node = std::find(status.begin(), status.end(), *cv_iter);
        if (curr_cv_node != status.end()){
          if (curr_cv_node != status.begin()){
            std::list<Curve_node>  overlapping_curves;
            
            typename Status_line::iterator lower =  --curr_cv_node;
            ++curr_cv_node;
            for ( ;curr_cv_node != status.begin() && 
                    traits.curves_overlap(lower->get_curve(), 
                                          curr_cv_node->get_curve()); --lower){
              
              Point p1, p2;
              traits.nearest_intersection_to_right (
                                                    curr_cv_node->get_curve(), 
                                                    lower->get_curve(), 
                                                    event_point, p1, p2);
              
              if (is_left(event_point, p1) || is_right(event_point, p2)) 
                // means that the overlapping does not intersects the 
                // status line.
                break;
              
              overlapping_curves.push_back(*curr_cv_node);
              status.erase(curr_cv_node);
              
              curr_cv_node = lower;
              Curve_node  cv_node = *curr_cv_node;
              
              // now taking care of the events created by the overlapping 
              // curve.
              
              Point xp;
              if (check_status_neighbors_intersections(event_queue, 
                                                       status, 
                                                       curr_cv_node, 
                                                       event_point))
                // Edge case of tangency in the event point, if it is this 
                // event will be taked cared again.
                event_overlap_terminated = false;
              
              if (curr_cv_node != status.begin()){
                --curr_cv_node;
                if (check_status_neighbors_intersections(event_queue, 
                                                         status, 
                                                         curr_cv_node, 
                                                         event_point))
                  // Edge case of tangency in the event point, if it is - this event will be taked cared again.
                  event_overlap_terminated = false;
                ++curr_cv_node;
              }
              
              if (curr_cv_node != status.end() && 
                  CGAL::compare_lexicographically_xy (event_point, 
                                                      traits.curve_target(curr_cv_node->get_curve()) ) 
                  == CGAL::EQUAL){
                
                typename Status_line::iterator prev_cv_node;
                bool first = true;
                // hold the (lower) neighbor element of the current.
                if (curr_cv_node != status.begin()){
                  prev_cv_node = --curr_cv_node;
                  ++curr_cv_node;
                  first = false;
                }
#ifdef  CGAL_SWEEP_LINE_DEBUG  
                cout<<"the event point is the right point of the curve - the curve leaves the status"<<std::endl;
#endif
                status.erase(curr_cv_node);
          
                CGAL_expensive_postcondition_code(is_valid(status));
                
                if (!first){
                  //cout<<"checking neighbors after deletion\n";
                  if (check_status_neighbors_intersections(event_queue, 
                                                           status, 
                                                           prev_cv_node, 
                                                           event_point))
                    // Edge case of tangency in the event point, if it is 
                    // this event will be taked cared again.
                    event_overlap_terminated = false;
                }
              } 
            }
            // reinsert to the status line all the overlapping removed curves.
            for (typename std::list<Curve_node>::iterator  
                   ovlp_iter = overlapping_curves.begin(); 
                 ovlp_iter != overlapping_curves.end();  ++ovlp_iter)
              status.insert(Status_line_value_type(*ovlp_iter));
          }
        }
        
        //curr_cv_node = status.find(*cv_iter);
        curr_cv_node = std::find(status.begin(), status.end(), *cv_iter);
        if (curr_cv_node != status.end()){
          std::list<Curve_node>  overlapping_curves;
          
          typename Status_line::iterator upper =  ++curr_cv_node;
          --curr_cv_node;
        
          
          for ( ;upper != status.end() && curr_cv_node != status.end() && 
                  traits.curves_overlap(curr_cv_node->get_curve(), 
                                        upper->get_curve()); ++upper){
            
            Point p1, p2;
            traits.nearest_intersection_to_right (
                                                  curr_cv_node->get_curve(), 
                                                  upper->get_curve(), 
                                                  event_point , p1, p2);
            
            // means that the overlapping does not intersects the status line
            if (is_left(event_point, p1) || is_right(event_point, p2))
              break;
            
            overlapping_curves.push_back(*curr_cv_node);
            status.erase(curr_cv_node);
            
            curr_cv_node = upper;
            Curve_node  cv_node = *curr_cv_node;
            
            // now taking care of the events created by the overlapping curve.
            Point xp;
            if (check_status_neighbors_intersections(event_queue, status, 
                                                     curr_cv_node, 
                                                     event_point))
              // Edge case of tangency in the event point, if it is this 
              // event will be taked cared again.
              event_overlap_terminated = false;
            
            if (curr_cv_node != status.begin()){
              --curr_cv_node;
              if (check_status_neighbors_intersections(event_queue, 
                                                       status, 
                                                       curr_cv_node, 
                                                       event_point))
                // Edge case of tangency in the event point, if it is - 
                // this event will be taked cared again.
                event_overlap_terminated = false;
              
              ++curr_cv_node;
            }
            
            if (curr_cv_node != status.end() && 
                CGAL::compare_lexicographically_xy (event_point, 
                       traits.curve_target(curr_cv_node->get_curve()) ) 
                == CGAL::EQUAL){
              
              typename Status_line::iterator prev_cv_node;
              bool first = true;
              // hold the (lower) neighbor element of the current.
              if (curr_cv_node != status.begin()){
                prev_cv_node = --curr_cv_node;
                ++curr_cv_node;
                first = false;
              }
#ifdef  CGAL_SWEEP_LINE_DEBUG  
              cout<<"the event point is the right point of the curve - the curve leaves the status"<<std::endl;
#endif
              status.erase(curr_cv_node);
              
              CGAL_expensive_postcondition_code(is_valid(status));
              if (!first){
                //cout<<"checking neighbors after deletion\n";
                if (check_status_neighbors_intersections(event_queue, 
                                                         status, 
                                                         prev_cv_node, 
                                                         event_point))
                  // Edge case of tangency in the event point, if it is this event will be taked cared again.
                  event_overlap_terminated = false;
              }
            } 
          }
          // reinsert to the status line all the overlapping removed curves.
          for (typename std::list<Curve_node>::iterator  ovlp_iter = 
                 overlapping_curves.begin(); 
               ovlp_iter != overlapping_curves.end();  ++ovlp_iter)
            status.insert(Status_line_value_type(*ovlp_iter));
        }
      }
      
      if (!event_terminated){
        handle_one_event (event_queue, status, event_point, point_node);
      }
      
#ifdef  CGAL_SWEEP_LINE_DEBUG  
      cout<<"Printing status line "<<std::endl;
      print_status(status);   
#endif
      
      for (Curve_node_iterator cv_iter = point_node.curves_begin(); 
           cv_iter != point_node.curves_end(); ++cv_iter){
        if (event_point != traits.curve_source(cv_iter->get_curve()) && 
            event_point == cv_iter->get_rightmost_point().point())
          cv_iter->erase_rightmost_point();
      }

      // now, updating the planar map (or arrangement) according the 
      // curves enemating from the currnet event point.
      update_subcurves(point_node, subcurves, overlapping);

      // updating all the new intersection nodes of the curves 
      // participating within the event.
      for (Curve_node_iterator cv_iter = point_node.curves_begin(); 
           cv_iter != point_node.curves_end(); ++cv_iter){
        if (event_point != cv_iter->get_rightmost_point().point())
          cv_iter->push_event_point(point_node.get_point());
      }

      //if (event_terminated)
      event_queue.erase(event);
    }
    sweeping_t.stop();
    std::cout<<"The time required to sweeping: "<< sweeping_t.time()<<std::endl;
    
#ifdef  CGAL_SWEEP_LINE_DEBUG  
    std::cout<<"the number of events was "<<queue_size<<std::endl;
#endif

    CGAL_expensive_postcondition_code(is_valid(status)); 
    
    //c_sweep_t = clock() - c_sweep_t;
    //std::cout<<"The time required by sweep proccess: "
    // << (double) c_sweep_t / (double) CLOCKS_PER_SEC<<std::endl;
  }
  
  template <class Container>
  void  sweep_curves_to_points(Curve_iterator curves_begin, 
                               Curve_iterator curves_end, 
                               Container &points, 
                               bool overlapping = false)
  { 
    Traits                traits;
    Event_queue           event_queue;
    Status_line           status;
    
    //int c_sweep_t;
    //c_sweep_t = clock();
    
#ifdef  CGAL_SWEEP_LINE_DEBUG
    unsigned int n = 0;
    for (Curve_iterator cv_iter = curves_begin; 
         cv_iter !=  curves_end; cv_iter++, n++);
    cout<<"number of edges on input "<< n <<std::endl;
#endif

    CGAL::Timer pre_preprocess_t;
    pre_preprocess_t.start();
    // splitting all curves to x-monotone curves.
    X_curve_list  x_monotone_curves;
    for(Curve_iterator cv_iter = curves_begin; 
        cv_iter != curves_end; cv_iter++){
      if (!traits.is_x_monotone(*cv_iter)) {
        X_curve_list x_monotone_subcurves;
        traits.make_x_monotone(*cv_iter, x_monotone_subcurves);

#ifdef  CGAL_SWEEP_LINE_DEBUG
        std::cout<<"printing x-monotone parts"<<std::endl;
#endif
        for(X_curve_list_iterator iter = x_monotone_subcurves.begin(); 
            iter != x_monotone_subcurves.end(); iter++){
#ifdef  CGAL_SWEEP_LINE_DEBUG
          std::cout<<*iter<<endl;
#endif
          x_monotone_curves.push_back(*iter);  
        }
      }
      else
        x_monotone_curves.push_back(*cv_iter);
    }
    pre_preprocess_t.stop();
    std::cout<<"The time required to pre precossing the input : "<< pre_preprocess_t.time()<<std::endl;

    CGAL::Timer prepare_DS_t;
    prepare_DS_t.start(); 
    // now creating the Curve_node handles and the event queue.
    unsigned int id = 0;
    for(X_curve_list_iterator cv_iter = x_monotone_curves.begin(); 
        cv_iter != x_monotone_curves.end(); cv_iter++, id++){
      
      X_curve cv(*cv_iter);
      if (is_right(traits.curve_source(*cv_iter), 
                   traits.curve_target(*cv_iter)) )
        cv = traits.curve_flip(*cv_iter);
      
#ifdef  CGAL_SWEEP_LINE_DEBUG
      cout<<cv<<std::endl;
#endif
      
      typename Event_queue::iterator  edge_point = 
        event_queue.find( traits.curve_source(cv) );
      // defining one cv_node for both source and target event points. 
      Curve_node  cv_node = Curve_node(X_curve_plus(cv, id), 
                                       Point_plus(traits.curve_source(cv)) ); 
      
      Intersection_point_node  source_point_node = 
        Intersection_point_node(cv_node, traits.curve_source(cv) );
       
      if (edge_point == event_queue.end() || 
          edge_point->second.get_point() != source_point_node.get_point())
        event_queue.insert(Event_queue_value_type(traits.curve_source(cv), 
						  source_point_node));
      else
        edge_point->second.merge(source_point_node);
      
      
      edge_point = event_queue.find( traits.curve_target(cv) );

      Intersection_point_node  target_point_node = 
        Intersection_point_node(cv_node, traits.curve_target(cv) );

      if (edge_point == event_queue.end() || 
          edge_point->second.get_point() != target_point_node.get_point())
        event_queue.insert(Event_queue_value_type(traits.curve_target(cv), 
						  target_point_node));
      else
        edge_point->second.merge(target_point_node);
    }
    prepare_DS_t.stop();
    std::cout<<"The time required to prepare the Data Structure : "<< prepare_DS_t.time()<<std::endl;
    

    CGAL::Timer sweeping_t;
    sweeping_t.start();   
     // now starting the sweeping.
    unsigned int queue_size = 0;
    bool         event_terminated = true;
    bool         event_overlap_terminated = true;
    while ( !(event_queue.empty()) ){
      queue_size++;
      // fetch the next event.
      typename Event_queue::iterator  event = event_queue.begin();

      const Point&              event_point = event->first;
      Intersection_point_node&  point_node = event->second;
      //bool                     event_terminated = true;
      
#ifdef  CGAL_SWEEP_LINE_DEBUG     
      cout<<"* * * event point is "<<event_point<<
        " and point node is "<<point_node.get_point().point()<<std::endl;
      CGAL_assertion(event_point == point_node.get_point().point());
#endif
      
      event_terminated = true; // reinitializing event_terminated 
      // to true only after the updating of the subdivision.
      
      //int handle_one_event_t = clock();
      // now continue with the sweep line.
      event_terminated = handle_one_event (event_queue, 
                                           status, 
                                           event_point, 
                                           point_node);
      
      //handle_one_event_t = clock() - handle_one_event_t;
      //std::cout<<"The time (in ticks) required to handle one event : "<< handle_one_event_t <<std::endl;
      
      // handling overlapping curves. 
      // On each overlapping group, we remove iteratively each 
      // curve and check for new events after the removing.
      // when finish, we reinsert to the status all the overlappting 
      // removed curves.
      for (Curve_node_iterator cv_iter = point_node.curves_begin(); 
           cv_iter != point_node.curves_end(); ++cv_iter){
        //typename Status_line::iterator curr_cv_node = status.find(*cv_iter);
        typename Status_line::iterator curr_cv_node = std::find(status.begin(), status.end(), *cv_iter);
        if (curr_cv_node != status.end()) {
          if (curr_cv_node != status.begin()){
            std::list<Curve_node>  overlapping_curves;
            
            typename Status_line::iterator lower =  --curr_cv_node;
            ++curr_cv_node;
            for ( ;curr_cv_node != status.begin() && 
                    traits.curves_overlap(lower->get_curve(), 
                                          curr_cv_node->get_curve()); --lower){
              
              Point p1, p2;
              traits.nearest_intersection_to_right (
                                                    curr_cv_node->get_curve(), 
                                                    lower->get_curve(), 
                                                    event_point, p1, p2);
              
              if (is_left(event_point, p1) || is_right(event_point, p2)) 
                // means that the overlapping does not intersects the 
                // status line.
                break;
              
              overlapping_curves.push_back(*curr_cv_node);
              status.erase(curr_cv_node);
              
              curr_cv_node = lower;
              Curve_node  cv_node = *curr_cv_node;
              
              // now taking care of the events created by the overlapping 
              // curve.
              
              Point xp;
              if (check_status_neighbors_intersections(event_queue, 
                                                       status, 
                                                       curr_cv_node, 
                                                       event_point))
                // Edge case of tangency in the event point, if it is this 
                // event will be taked cared again.
                event_overlap_terminated = false;
              
              if (curr_cv_node != status.begin()){
                --curr_cv_node;
                if (check_status_neighbors_intersections(event_queue, 
                                                         status, 
                                                         curr_cv_node, 
                                                         event_point))
                  // Edge case of tangency in the event point, if it is - this event will be taked cared again.
                  event_overlap_terminated = false;
                ++curr_cv_node;
              }
              
              if (curr_cv_node != status.end() && 
                  CGAL::compare_lexicographically_xy (event_point, 
                                                      traits.curve_target(curr_cv_node->get_curve()) ) 
                  == CGAL::EQUAL){
                
                typename Status_line::iterator prev_cv_node;
                bool first = true;
                // hold the (lower) neighbor element of the current.
                if (curr_cv_node != status.begin()){
                  prev_cv_node = --curr_cv_node;
                  ++curr_cv_node;
                  first = false;
                }
#ifdef  CGAL_SWEEP_LINE_DEBUG  
                cout<<"the event point is the right point of the curve - the curve leaves the status"<<std::endl;
#endif
                status.erase(curr_cv_node);
          
                CGAL_expensive_postcondition_code(is_valid(status));
                
                if (!first){
                  //cout<<"checking neighbors after deletion\n";
                  if (check_status_neighbors_intersections(event_queue, 
                                                           status, 
                                                           prev_cv_node, 
                                                           event_point))
                    // Edge case of tangency in the event point, if it is 
                    // this event will be taked cared again.
                    event_overlap_terminated = false;
                }
              } 
            }
            // reinsert to the status line all the overlapping removed curves.
            for (typename std::list<Curve_node>::iterator  
                   ovlp_iter = overlapping_curves.begin(); 
                 ovlp_iter != overlapping_curves.end();  ++ovlp_iter)
              status.insert(Status_line_value_type(*ovlp_iter));
          }
        }
        
        //curr_cv_node = status.find(*cv_iter);
        curr_cv_node = std::find(status.begin(), status.end(), *cv_iter);
        if (curr_cv_node != status.end()){
          std::list<Curve_node>  overlapping_curves;
          
          typename Status_line::iterator upper =  ++curr_cv_node;
          --curr_cv_node;
        
          
          for ( ;upper != status.end() && curr_cv_node != status.end() && 
                  traits.curves_overlap(curr_cv_node->get_curve(), 
                                        upper->get_curve()); ++upper){
            
            Point p1, p2;
            traits.nearest_intersection_to_right (
                                                  curr_cv_node->get_curve(), 
                                                  upper->get_curve(), 
                                                  event_point , p1, p2);
            
            // means that the overlapping does not intersects the status line
            if (is_left(event_point, p1) || is_right(event_point, p2))
              break;
            
            overlapping_curves.push_back(*curr_cv_node);
            status.erase(curr_cv_node);
            
            curr_cv_node = upper;
            Curve_node  cv_node = *curr_cv_node;
            
            // now taking care of the events created by the overlapping curve.
            Point xp;
            if (check_status_neighbors_intersections(event_queue, status, 
                                                     curr_cv_node, 
                                                     event_point))
              // Edge case of tangency in the event point, if it is this 
              // event will be taked cared again.
              event_overlap_terminated = false;
            
            if (curr_cv_node != status.begin()){
              --curr_cv_node;
              if (check_status_neighbors_intersections(event_queue, 
                                                       status, 
                                                       curr_cv_node, 
                                                       event_point))
                // Edge case of tangency in the event point, if it is - 
                // this event will be taked cared again.
                event_overlap_terminated = false;
              
              ++curr_cv_node;
            }
            
            if (curr_cv_node != status.end() && 
                CGAL::compare_lexicographically_xy (event_point, 
                                                    traits.curve_target(curr_cv_node->get_curve()) ) 
                == CGAL::EQUAL){
              
              typename Status_line::iterator prev_cv_node;
              bool first = true;
              // hold the (lower) neighbor element of the current.
              if (curr_cv_node != status.begin()){
                prev_cv_node = --curr_cv_node;
                ++curr_cv_node;
                first = false;
              }
#ifdef  CGAL_SWEEP_LINE_DEBUG  
              cout<<"the event point is the right point of the curve - the curve leaves the status"<<std::endl;
#endif
              status.erase(curr_cv_node);
              
              CGAL_expensive_postcondition_code(is_valid(status));
              if (!first){
                //cout<<"checking neighbors after deletion\n";
                if (check_status_neighbors_intersections(event_queue, 
                                                         status, 
                                                         prev_cv_node, 
                                                         event_point))
                  // Edge case of tangency in the event point, if it is this event will be taked cared again.
                  event_overlap_terminated = false;
              }
            } 
          }
          // reinsert to the status line all the overlapping removed curves.
          for (typename std::list<Curve_node>::iterator  ovlp_iter = 
                 overlapping_curves.begin(); 
               ovlp_iter != overlapping_curves.end();  ++ovlp_iter)
            status.insert(Status_line_value_type(*ovlp_iter));
        }
      }
      
      if (!event_terminated){
        handle_one_event (event_queue, status, event_point, point_node);
      }
      
#ifdef  CGAL_SWEEP_LINE_DEBUG  
      cout<<"Printing status line "<<std::endl;
      print_status(status);   
#endif
      
      //int update_t = clock();
      for (Curve_node_iterator cv_iter = point_node.curves_begin(); 
           cv_iter != point_node.curves_end(); cv_iter++){
        if (event_point != traits.curve_source(cv_iter->get_curve()) && 
            event_point == cv_iter->get_rightmost_point().point())
          cv_iter->erase_rightmost_point();
      }

      // now, updating the planar map (or arrangement) according the 
      // curves enemating from the currnet event point.
      //update_subcurves(point_node, subcurves, overlapping);
      points.push_back(point_node.get_point().point());
      
      // updating all the new intersection nodes of the curves 
      // participating within the event.
      for (Curve_node_iterator cv_iter = point_node.curves_begin(); 
           cv_iter != point_node.curves_end(); cv_iter++){
        if (event_point != cv_iter->get_rightmost_point().point())
          cv_iter->push_event_point(point_node.get_point());
      }

      //update_t = clock() - update_t;
      //std::cout<<"The time (in ticks) required to update container : "<< update_t<<std::endl;
      
      //if (event_terminated)
      event_queue.erase(event);
    }
    sweeping_t.stop();
    std::cout<<"The time required to sweeping: "<< sweeping_t.time()<<std::endl;

#ifdef  CGAL_SWEEP_LINE_DEBUG  
    std::cout<<"the number of events was "<<queue_size<<std::endl;
#endif

    CGAL_expensive_postcondition_code(is_valid(status)); 
    
    //c_sweep_t = clock() - c_sweep_t;
    //std::cout<<"The time required by sweep proccess: "
    // << (double) c_sweep_t / (double) CLOCKS_PER_SEC<<std::endl;
  }
  
private:
  template <class Container>
  void  update_subcurves(Intersection_point_node& point_node, 
                         Container &subcurves, 
                         bool overlapping) 
  {
    if (overlapping)
      update_subcurves_with_overlappings(point_node, subcurves);
    else
      update_subcurves_without_overlappings(point_node, subcurves);
    
        /*#ifdef  CGAL_SWEEP_LINE_DEBUG
    cout<<"--------- updating map with point node"<<point_node.get_point().point() <<std::endl;
    for (Curve_node_iterator cv_iter1= point_node.curves_begin(); cv_iter1 != point_node.curves_end(); cv_iter1++){
      cv_iter1++;
      Curve_node_iterator cv_iter2 = cv_iter1;
      cv_iter1--;
      for ( ; cv_iter2 != point_node.curves_end(); cv_iter2++){   
        if (traits.curves_overlap(cv_iter1->get_curve(), cv_iter2->get_curve()))
          cout<<"update_subdivision "<<cv_iter1->get_curve()<<" and "<< cv_iter2->get_curve() <<" are overlapping"<<endl;
      }
    }
#endif    

    X_curve prev_sub_cv;
    for (Curve_node_iterator cv_iter = point_node.curves_begin(); cv_iter != point_node.curves_end(); cv_iter++){
#ifdef  CGAL_SWEEP_LINE_DEBUG
      cout<<"now handling "<<cv_iter->get_curve()<<endl;
#endif
      
      if (is_left(cv_iter->get_rightmost_point().point(), point_node.get_point().point())) { // means we have a new sub curve to insert.
        
        // first splitting the curve in the points cv_iter->get_rightmost_point().point() and point_node.get_point().point().
        X_curve cv = cv_iter->get_curve(), sub_cv = cv_iter->get_curve(), right_cv = cv;

        //cout<<"cv is "<<cv<<endl;
        if (traits.curve_source(cv) != cv_iter->get_rightmost_point().point() &&
            traits.curve_target(cv) != cv_iter->get_rightmost_point().point() ) {
          traits.curve_split(cv, sub_cv, right_cv, cv_iter->get_rightmost_point().point());
          
          cv = right_cv;
        }
        
        //cout<<"after first splitting cv is "<<cv<<endl;
        //cout<<"source and target of cv are"<<traits.curve_source(cv)<<" "<<traits.curve_target(cv)<<endl;
        if (traits.curve_source(cv) != point_node.get_point().point() &&
            traits.curve_target(cv) != point_node.get_point().point())
          traits.curve_split(cv, sub_cv, right_cv, point_node.get_point().point());
        else
          sub_cv = right_cv;
        
#ifdef  CGAL_SWEEP_LINE_DEBUG
        cout<<"inserting "<<sub_cv<<endl;
#endif

        //prev_sub_cv = sub_cv;

        subcurves.push_back(sub_cv);
        
        // else - no new sub curve is inserted to the subdivision.
      }
      }*/
  }
  
  template <class Container>
  void  update_subcurves_with_overlappings(Intersection_point_node& point_node, 
                                           Container &subcurves) 
  {
#ifdef  CGAL_SWEEP_LINE_DEBUG
    cout<<"--------- updating map with point node"<<
      point_node.get_point().point() <<std::endl;
    for (Curve_node_iterator cv_iter1= point_node.curves_begin(); 
         cv_iter1 != point_node.curves_end(); cv_iter1++){
      cv_iter1++;
      Curve_node_iterator cv_iter2 = cv_iter1;
      cv_iter1--;
      for ( ; cv_iter2 != point_node.curves_end(); cv_iter2++){   
        if (traits.curves_overlap(cv_iter1->get_curve(), 
                                  cv_iter2->get_curve()))
          cout<<"update_subdivision "<<cv_iter1->get_curve()<<
            " and "<< cv_iter2->get_curve() <<" are overlapping"<<endl;
      }
    }
#endif    

    X_curve prev_sub_cv;
    for (Curve_node_iterator cv_iter = point_node.curves_begin(); 
         cv_iter != point_node.curves_end(); ++cv_iter){
#ifdef  CGAL_SWEEP_LINE_DEBUG
      cout<<"now handling "<<cv_iter->get_curve()<<endl;
#endif
      
      if (is_left(cv_iter->get_rightmost_point().point(), 
                  point_node.get_point().point())) { 
        // means we have a new sub curve to insert.
        
        // first splitting the curve in the points 
        // cv_iter->get_rightmost_point().point() 
        // and point_node.get_point().point().
        X_curve cv = cv_iter->get_curve(), sub_cv = cv_iter->get_curve(), 
          right_cv = cv;

        //cout<<"cv is "<<cv<<endl;
        if (traits.curve_source(cv) != cv_iter->get_rightmost_point().point() 
            &&
            traits.curve_target(cv) != cv_iter->get_rightmost_point().point()) 
          {
            traits.curve_split(cv, sub_cv, right_cv, 
                               cv_iter->get_rightmost_point().point());
          
            cv = right_cv;
          }
        
        //cout<<"after first splitting cv is "<<cv<<endl;
        //cout<<"source and target of cv are"<<traits.curve_source(cv)
        // <<" "<<traits.curve_target(cv)<<endl;
        
        if (traits.curve_source(cv) != point_node.get_point().point() &&
            traits.curve_target(cv) != point_node.get_point().point())
          traits.curve_split(cv, sub_cv, 
                             right_cv, point_node.get_point().point());
        else
          sub_cv = right_cv;

        /*  if (cv_iter != point_node.curves_begin()){
            if (traits.curves_overlap(sub_cv, prev_sub_cv)){
            //cout<<sub_cv<<" and "<< prev_sub_cv<<" are overlapping"<<endl;
            continue;
            }
            }*/
        
#ifdef  CGAL_SWEEP_LINE_DEBUG
        cout<<"inserting "<<sub_cv<<endl;
#endif

        //prev_sub_cv = sub_cv;

        subcurves.push_back(sub_cv);
      }
      // else - no new sub curve is inserted to the subdivision.
    }
  }
  
  template <class Container> 
  void  update_subcurves_without_overlappings(
                          Intersection_point_node& point_node, 
                          Container &subcurves)
  {
#ifdef  CGAL_SWEEP_LINE_DEBUG
    cout<<"--------- updating map with point node"<<
      point_node.get_point().point() <<std::endl;
    for (Curve_node_iterator cv_iter1= point_node.curves_begin(); 
         cv_iter1 != point_node.curves_end(); ++cv_iter1){
      ++cv_iter1;
      Curve_node_iterator cv_iter2 = cv_iter1;
      --cv_iter1;
      for ( ; cv_iter2 != point_node.curves_end(); ++cv_iter2){   
        if (traits.curves_overlap(cv_iter1->get_curve(), 
                                  cv_iter2->get_curve()))
          cout<<"update_subdivision "<<cv_iter1->get_curve()<<
            " and "<< cv_iter2->get_curve() <<" are overlapping"<<endl;
      }
    }
#endif    

    X_curve prev_sub_cv;
    for (Curve_node_iterator cv_iter = point_node.curves_begin(); 
         cv_iter != point_node.curves_end(); ++cv_iter){
#ifdef  CGAL_SWEEP_LINE_DEBUG
      cout<<"now handling "<<cv_iter->get_curve()<<endl;
#endif

      if (is_left(cv_iter->get_rightmost_point().point(), 
                  point_node.get_point().point())) { 
        // means we have a new sub curve to insert.
        
        // first splitting the curve in the points 
        // cv_iter->get_rightmost_point().point() and 
        // point_node.get_point().point().
        X_curve cv = cv_iter->get_curve(), sub_cv = cv_iter->get_curve(), 
          right_cv = cv;
        
        //cout<<"cv is "<<cv<<endl;
        if (traits.curve_source(cv) != cv_iter->get_rightmost_point().point() 
            &&
            traits.curve_target(cv) != cv_iter->get_rightmost_point().point() ) 
          {
            traits.curve_split(cv, sub_cv, right_cv, 
                               cv_iter->get_rightmost_point().point());
          
            cv = right_cv;
          }
        
        //cout<<"after first splitting cv is "<<cv<<endl;
        //cout<<"source and target of cv are"<<traits.curve_source(cv)<<" "<<traits.curve_target(cv)<<endl;
        if (traits.curve_source(cv) != point_node.get_point().point() &&
            traits.curve_target(cv) != point_node.get_point().point())
          traits.curve_split(cv, sub_cv, right_cv, 
                             point_node.get_point().point());
        else
          sub_cv = right_cv;
        
        // for debugging!
        //assert(sub_cv == X_curve(cv_iter->get_rightmost_point().point(), point_node.get_point().point()));
        //cout<<"sub curve calculates is "<<endl;
        //cout<<sub_cv<<endl;
        //cout<<"And the right curve is supposed to be"<<endl;
        //cout<<X_curve( cv_iter->get_rightmost_point().point(), point_node.get_point().point())<<endl;

        if (cv_iter != point_node.curves_begin()){
          if (traits.curves_overlap(sub_cv, prev_sub_cv)){
            //cout<<sub_cv<<" and "<< prev_sub_cv<<" are overlapping"<<endl;
            continue;
          }
        }

#ifdef  CGAL_SWEEP_LINE_DEBUG
        cout<<"inserting "<<sub_cv<<endl;
#endif

        prev_sub_cv = sub_cv;

        subcurves.push_back(sub_cv);
      }
      // else - no new sub curve is inserted to the subdivision.
    }
  }
};

CGAL_END_NAMESPACE

#endif
