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
// release       : $CGAL_Revision: CGAL-2.5-I-11 $
// release_date  : $CGAL_Date: 2002/08/04 $
//
// file          : include/CGAL/Map_overlay.h
// package       : Map_overlay (1.12)
// maintainer    : Efi Fogel <efif@math.tau.ac.il>
// source        : 
// revision      : 
// revision_date : 
// author(s)     : Eti Ezra          <estere@post.tau.ac.il>
//
// coordinator   : Tel-Aviv University (Dan Halperin <halperin@math.tau.ac.il>)
//
// Chapter       : 
// ======================================================================
#ifndef CGAL_MAP_OVERLAY_H
#define CGAL_MAP_OVERLAY_H

#include <vector>
#include <list>
//#include <time.h>

#ifndef CGAL_PM_POINT_LOCATION_BASE_H
#include <CGAL/Pm_point_location_base.h>
#endif

#ifndef CGAL_MAP_OVERLAY_BASE_H
#include <CGAL/Map_overlay_base.h>
#endif

#ifndef CGAL_MAP_OVERLAY_SWEEP_H
#include <CGAL/Map_overlay_sweep.h>
#endif

#ifndef CGAL_MAP_OVERLAY_DEFAULT_NOTIFIER_H
#include <CGAL/Map_overlay_default_notifier.h>
#endif

#ifndef CGAL_HANDLE_FOR_H
#include <CGAL/Handle.h>
#endif

CGAL_BEGIN_NAMESPACE

template  <class Subdivision_, class Change_notification_>
class Map_overlay_2_rep;

template  <class Subdivision_, class Change_notification_>
class Map_overlay_2;

template  <class Subdivision_, 
           class Change_notification_ = 
                 Map_overlay_default_notifier<Subdivision_> > 
class Map_overlay_2_rep : public Rep{ 
public:
  typedef Subdivision_                                Subdivision;
  typedef Change_notification_                        Change_notification;
  typedef typename Subdivision::Planar_map            Planar_map;
  typedef typename Subdivision::Traits                Traits;
  typedef typename Traits::X_monotone_curve_2                  X_monotone_curve_2;  
  typedef typename Traits::Point_2                    Point_2;

  typedef Map_overlay_base<Subdivision, Change_notification>  Map_overlay_base;
  typedef Map_overlay_sweep<Subdivision, Change_notification> 
                                                             Map_overlay_sweep;
  
  typedef Pm_point_location_base<Planar_map> Point_location_base;
  
private:
  typedef Map_overlay_2_rep<Subdivision, Change_notification>       Self;
  
public:
  
  Map_overlay_2_rep () : 
    arr_(new Subdivision()),
    sub_division1(0), sub_division2(0), 
    ovl_change_notf(new Change_notification),  
    ovl_alg(new Map_overlay_sweep), 
    use_delete_notf(true), 
    use_delete_ovl(true) {}
  
  Map_overlay_2_rep (Point_location_base *pl_ptr) : 
    arr_(new Subdivision(pl_ptr)), 
    sub_division1(0), sub_division2(0), 
    ovl_change_notf(new Change_notification),  
    ovl_alg(new Map_overlay_sweep), 
    use_delete_notf(true), 
    use_delete_ovl(true) {}
  
  Map_overlay_2_rep (const Subdivision &arr) : 
    arr_(new Subdivision(arr)), 
    sub_division1(0), sub_division2(0),  
    ovl_change_notf(new Change_notification),  
    ovl_alg(new Map_overlay_sweep), 
    use_delete_notf(true), 
    use_delete_ovl(true) {}
  
  Map_overlay_2_rep (const Subdivision &arr, 
                     Change_notification* pmwx_change_notf) : 
    arr_(new Subdivision(arr)), 
    sub_division1(0), sub_division2(0), 
    ovl_change_notf(pmwx_change_notf), 
    ovl_alg(new Map_overlay_sweep), 
    use_delete_notf(false), use_delete_ovl(true) {}
  
  Map_overlay_2_rep (const Subdivision &arr, 
                     Map_overlay_base *ovl_ptr) : 
    arr_(new Subdivision(arr)),  
    sub_division1(0), sub_division2(0), 
    ovl_change_notf(new Change_notification), 
    ovl_alg(ovl_ptr), 
    use_delete_notf(true), use_delete_ovl(false) {}

  Map_overlay_2_rep (const Subdivision &arr,  
                     Change_notification* pmwx_change_notf, 
               Map_overlay_base *ovl_ptr) : 
    arr_(new Subdivision(arr)), 
    sub_division1(0), sub_division2(0), 
    ovl_change_notf(pmwx_change_notf), 
    ovl_alg(ovl_ptr),
    use_delete_notf(false), 
    use_delete_ovl(false) {}

  Map_overlay_2_rep (const Self &ovl1, const Self &ovl2) : 
    arr_(new Subdivision),
    sub_division1(&ovl1), 
    sub_division2(&ovl2),
    ovl_change_notf(new Change_notification(ovl1.arr_,ovl2.arr_ )), 
    ovl_alg(new Map_overlay_sweep), 
    use_delete_notf(true), 
    use_delete_ovl(true)
  {
    ovl_alg->map_overlay(*(ovl1.arr_), 
                         *(ovl2.arr_), 
                         ovl_change_notf, *arr_);
  }
  
  Map_overlay_2_rep (const Self &ovl1, const Self &ovl2, 
                 Point_location_base *pl_ptr) : 
    arr_(new Subdivision(pl_ptr)), 
    sub_division1(&ovl1), 
    sub_division2(&ovl2),
    ovl_change_notf(new Change_notification(ovl1.arr_,ovl2.arr_ )), 
    ovl_alg(new Map_overlay_sweep),
    use_delete_notf(true), 
    use_delete_ovl(true)
  {    
    ovl_alg->map_overlay(*(ovl1.arr_), 
                         *(ovl2.arr_), 
                         ovl_change_notf, 
                         *arr_);
  }

  Map_overlay_2_rep (const Self &ovl1, 
                     const Self &ovl2, 
                     Change_notification* pmwx_change_notf) : 
    arr_(new Subdivision()),
    sub_division1(&ovl1), 
    sub_division2(&ovl2),
    ovl_change_notf(pmwx_change_notf), 
    ovl_alg(new Map_overlay_sweep),
    use_delete_notf(false), 
    use_delete_ovl(true)
  {
    ovl_alg->map_overlay(*(ovl1.arr_), 
                         *(ovl2.arr_), 
                         ovl_change_notf, 
                         *arr_);
  }

  Map_overlay_2_rep (const Self &ovl1, 
                     const Self &ovl2, 
                     Point_location_base* pl_ptr,
                     Change_notification* pmwx_change_notf) :  
    arr_(new Subdivision(pl_ptr)), 
    sub_division1(&ovl1), 
    sub_division2(&ovl2),
    ovl_change_notf(pmwx_change_notf), 
    ovl_alg(new Map_overlay_sweep),
    use_delete_notf(false), 
    use_delete_ovl(true)
  {
    ovl_alg->map_overlay(*(ovl1.arr_), 
                         *(ovl2.arr_), 
                         ovl_change_notf, 
                         *arr_);
  }
  
  
  Map_overlay_2_rep (const Self &ovl1, 
                     const Self &ovl2, 
                     Map_overlay_base *ovl_ptr) :
    arr_(new Subdivision()),
    sub_division1(&ovl1), sub_division2(&ovl2),
    ovl_change_notf(new Change_notification(ovl1.arr_,ovl2.arr_ )),
    ovl_alg(ovl_ptr), 
    use_delete_notf(true), use_delete_ovl(false)
  {
    ovl_alg->map_overlay(*(ovl1.arr_), 
                         *(ovl2.arr_), 
                         ovl_change_notf, 
                         *arr_);
  }
  
  Map_overlay_2_rep (const Self &ovl1, 
                     const Self &ovl2, 
                     Point_location_base* pl_ptr, 
                     Map_overlay_base *ovl_ptr) :
    arr_(new Subdivision(pl_ptr)),
    sub_division1(&ovl1), sub_division2(&ovl2),
    ovl_change_notf(new Change_notification(ovl1.arr_,ovl2.arr_ )),
    ovl_alg(ovl_ptr), 
    use_delete_notf(true), use_delete_ovl(false)
  {
    ovl_alg->map_overlay(*(ovl1.arr_), 
                         *(ovl2.arr_), 
                         ovl_change_notf, 
                         *arr_);
  }
  
  
  Map_overlay_2_rep (const Self &ovl1, 
                     const Self &ovl2, 
                     Change_notification* pmwx_change_notf, 
                     Map_overlay_base *ovl_ptr) :
    arr_(new Subdivision()),
    sub_division1(&ovl1), sub_division2(&ovl2),
    ovl_change_notf(pmwx_change_notf), ovl_alg(ovl_ptr),
    use_delete_notf(false), use_delete_ovl(false)
  {
    ovl_alg->map_overlay(*(ovl1.arr_), 
                         *(ovl2.arr_), 
                         ovl_change_notf, 
                         *arr_);
  }

  Map_overlay_2_rep (const Self &ovl1, 
                     const Self &ovl2, 
                     Point_location_base* pl_ptr,
                     Change_notification* pmwx_change_notf, 
                     Map_overlay_base *ovl_ptr) :
    arr_(new Subdivision(pl_ptr)),
    sub_division1(&ovl1), sub_division2(&ovl2),
    ovl_change_notf(pmwx_change_notf), ovl_alg(ovl_ptr),
    use_delete_notf(false), use_delete_ovl(false)
  {
    ovl_alg->map_overlay(*(ovl1.arr_), 
                         *(ovl2.arr_), 
                         ovl_change_notf, 
                         *arr_);
  }
  
  // ------------------- Copy contructor -----------------------------
  Map_overlay_2_rep (const Self &ovl) : 
    arr_(new Subdivision(*(ovl.arr_))), 
    sub_division1(ovl.sub_division1), 
    sub_division2(ovl.sub_division2), 
    ovl_change_notf(ovl.ovl_change_notf),
    ovl_alg(ovl.ovl_alg),  // needs a copy constructor for Point location.
    use_delete_notf(false), use_delete_ovl(false) {}
  
  virtual ~Map_overlay_2_rep() 
  {
    delete  arr_;
    if (use_delete_notf)
      delete ovl_change_notf;
    if (use_delete_ovl)
      delete ovl_alg;
  }
  
  // -------------------- Assignement operator --------------------------
  const Self& operator=(const Self& ovl)
  {
    *arr_ = *(ovl.arr_); 
    sub_division1 = ovl.sub_division1; 
    sub_division2 = ovl.sub_division2; 

    // The notifier and ovl algorithm remain their initial values obtained 
    // in the defualt constructor (or other constructor taken for *this).
    //ovl_change_notf(new Change_notification(ovl.ovl_change_notf));
    //ovl_alg = ovl.ovl_alg;
    //use_delete_notf=true; use_delete_ovl=false;

    return *this;
  }
  
  void	 delete_subtree() {
    *arr_ = Map_overlay_2_rep(*arr_);
    
    for (typename Subdivision::Vertex_iterator 
           v_iter = arr_->vertices_begin();
         v_iter !=  arr_->vertices_end(); v_iter++)
      v_iter->reset();
    
    for (typename Subdivision::Halfedge_iterator 
           h_iter = arr_->halfedges_begin(); 
         h_iter !=  arr_->halfedges_end(); h_iter++)
      h_iter->reset();
    
    for (typename Subdivision::Face_iterator 
           f_iter = arr_->faces_begin(); 
         f_iter != arr_->faces_end(); f_iter++)
      f_iter->reset();
  }

  //const Subdivision& subdivision() const 
  //{ 
  //  return *arr_; 
  // }
  
protected:
  template  <class Subdivision, class Change_notification>
  friend class Map_overlay_2;

  void add_reference() { ++count; }
  void remove_reference() { --count; }
  
  // Saving the subdivision as a pointer is crucial.  Since the
  // overlay has levels of creators, we make sure this way all levels
  // are valid.  For example, it is possible to create a map overlay
  // of two subdivisions (rather than two map overlays) using the
  // converter and no harm is done: the converter saves its
  // corresponding map overlay as the creator. If the usage is not by
  // pointers, a temporary value of map overlay may be constructed by
  // the converter and may cause memory fligh.
  Subdivision                       *arr_;
  const Self                        *sub_division1, *sub_division2;
  Change_notification               *ovl_change_notf;
  Map_overlay_base                  *ovl_alg;
  bool use_delete_notf;
  bool use_delete_ovl;
};

template  <class Subdivision_, 
  class Change_notification_ = Map_overlay_default_notifier<Subdivision_> > 
class  Map_overlay_2 : public Handle
{
  typedef Map_overlay_2_rep<Subdivision_, Change_notification_>  Ovl_rep;
  //typedef Handle_for<ovl_rep>                         Handle_for_ovl_rep;
  
  Ovl_rep* ptr()
  { 
    return (Ovl_rep*) PTR; 
  }
  
  const Ovl_rep* ptr() const 
  { 
    return (Ovl_rep*) PTR; 
  }
  
public:  
  
  typedef Subdivision_                                Subdivision;
  typedef Change_notification_                        Change_notification;
  typedef typename Subdivision::Planar_map            Planar_map;
  //typedef typename Subdivision::Pmwx                Pmwx;
  typedef typename Subdivision::Vertex                Vertex;
  typedef typename Subdivision::Face                  Face;
  typedef typename Subdivision::Halfedge              Halfedge;
  typedef typename Subdivision::Vertex_handle         Vertex_handle;
  typedef typename Subdivision::Halfedge_handle       Halfedge_handle;
  typedef typename Subdivision::Face_handle           Face_handle;
  typedef typename Subdivision::Vertex_const_handle   Vertex_const_handle;
  typedef typename Subdivision::Halfedge_const_handle   Halfedge_const_handle;
  typedef typename Subdivision::Face_const_handle       Face_const_handle;
  typedef typename Subdivision::Vertex_iterator         Vertex_iterator;
  typedef typename Subdivision::Halfedge_iterator       Halfedge_iterator;
  typedef typename Subdivision::Face_iterator           Face_iterator;
  typedef typename Subdivision::Vertex_const_iterator   Vertex_const_iterator;
  typedef typename Subdivision::Halfedge_const_iterator  
                                                      Halfedge_const_iterator;
  typedef typename Subdivision::Face_const_iterator     Face_const_iterator;
  typedef typename Subdivision::Ccb_halfedge_circulator 
                                                     Ccb_halfedge_circulator;
  typedef typename Subdivision::Ccb_halfedge_const_circulator   
                                               Ccb_halfedge_const_circulator;
  typedef typename Subdivision::Holes_iterator          Holes_iterator;
  typedef typename Subdivision::Holes_const_iterator    Holes_const_iterator;
  typedef typename Subdivision::Locate_type             Locate_type;
  //typedef typename Subdivision::Point_location_base    Point_location_base;

  typedef typename Subdivision::Traits                  Traits;
  typedef typename Traits::X_monotone_curve_2                    X_monotone_curve_2;  
  typedef typename Traits::Point_2                      Point_2;

  typedef Map_overlay_base<Subdivision, Change_notification>  Map_overlay_base;
  typedef Map_overlay_sweep<Subdivision, Change_notification> 
                                                             Map_overlay_sweep;
  
  typedef Pm_point_location_base<Planar_map> Point_location_base;
  
private:
  typedef Map_overlay_2<Subdivision, Change_notification>       Self;
  
public:

  Map_overlay_2 () : Handle(),
  first_creator_(0), second_creator_(0) {}

  Map_overlay_2 (Point_location_base *pl_ptr) :
    first_creator_(0), second_creator_(0) 
  {
    PTR = new Ovl_rep(pl_ptr);
  }
  
  Map_overlay_2 (const Subdivision &arr) :
    first_creator_(0), second_creator_(0) 
  {
    PTR = new Ovl_rep(arr);
  }
  
  Map_overlay_2 (const Subdivision &arr, 
                 Change_notification* pmwx_change_notf) :
    first_creator_(0), second_creator_(0) 
  {
    PTR = new Ovl_rep(arr,pmwx_change_notf);
  }
  
  Map_overlay_2 (const Subdivision &arr, 
                 Map_overlay_base *ovl_ptr) :
    first_creator_(0), second_creator_(0) 
  {
    PTR = new Ovl_rep(arr,ovl_ptr);
  }

  Map_overlay_2 (const Subdivision &arr,  
                 Change_notification* pmwx_change_notf, 
                 Map_overlay_base *ovl_ptr) : 
    first_creator_(0), second_creator_(0) 
  {
    PTR = new Ovl_rep(arr,pmwx_change_notf,ovl_ptr);
  }

  Map_overlay_2 (const Self &ovl1, const Self &ovl2)
  {
    PTR = new Ovl_rep(*(ovl1.ptr()),*(ovl2.ptr()));
    
    ((Ovl_rep*) ovl1.PTR)->add_reference();
    ((Ovl_rep*) ovl2.PTR)->add_reference();
    first_creator_ = &ovl1;
    second_creator_ = &ovl2;
  }
  
  Map_overlay_2 (const Self &ovl1, const Self &ovl2, 
                 Point_location_base *pl_ptr)
  {
    PTR = new Ovl_rep(*(ovl1.ptr()),
                      *(ovl2.ptr()),
                      pl_ptr);

    ((Ovl_rep*) ovl1.PTR)->add_reference();
    ((Ovl_rep*) ovl2.PTR)->add_reference();
    
    first_creator_ = &ovl1;
    second_creator_ = &ovl2;
  }
  
  Map_overlay_2 (const Self &ovl1, 
                 const Self &ovl2, 
                 Change_notification* pmwx_change_notf)
  {
    PTR = new Ovl_rep(*(ovl1.ptr()),
                      *(ovl2.ptr()),
                      pmwx_change_notf);
    
    ((Ovl_rep*) ovl1.PTR)->add_reference();
    ((Ovl_rep*) ovl2.PTR)->add_reference();
    first_creator_ = &ovl1;
    second_creator_ = &ovl2;
  }
  
  Map_overlay_2 (const Self &ovl1, 
                 const Self &ovl2, 
                 Point_location_base* pl_ptr,
                 Change_notification* pmwx_change_notf)
  {
    PTR = new Ovl_rep(*(ovl1.ptr()),
                      *(ovl2.ptr()),
                      pl_ptr,pmwx_change_notf);
    
    ((Ovl_rep*) ovl1.PTR)->add_reference();
    ((Ovl_rep*) ovl2.PTR)->add_reference();
    first_creator_ = &ovl1;
    second_creator_ = &ovl2;
  }
  
  Map_overlay_2 (const Self &ovl1, 
                 const Self &ovl2, 
                 Map_overlay_base *ovl_ptr)
  {
    PTR = new Ovl_rep(*(ovl1.ptr()),
                      *(ovl2.ptr()),
                      ovl_ptr);
 
    ((Ovl_rep*) ovl1.PTR)->add_reference();
    ((Ovl_rep*) ovl2.PTR)->add_reference();
    first_creator_ = &ovl1;
    second_creator_ = &ovl2;
  }
  
  Map_overlay_2 (const Self &ovl1, 
                 const Self &ovl2, 
                 Point_location_base* pl_ptr, 
                 Map_overlay_base *ovl_ptr)
  {
    PTR = new Ovl_rep(*(ovl1.ptr()),
                      *(ovl2.ptr()),
                      pl_ptr, ovl_ptr);
    
    ((Ovl_rep*) ovl1.PTR)->add_reference();
    ((Ovl_rep*) ovl2.PTR)->add_reference();
    first_creator_ = &ovl1;
    second_creator_ = &ovl2;
  }
  
  Map_overlay_2 (const Self &ovl1, 
                 const Self &ovl2, 
                 Change_notification* pmwx_change_notf, 
                 Map_overlay_base *ovl_ptr)
  {
    PTR = new Ovl_rep(*(ovl1.ptr()),
                      *(ovl2.ptr()),
                      pmwx_change_notf,ovl_ptr);
    
    ((Ovl_rep*) ovl1.PTR)->add_reference();
    ((Ovl_rep*) ovl2.PTR)->add_reference();
    first_creator_ = &ovl1;
    second_creator_ = &ovl2;
  }

  Map_overlay_2 (const Self &ovl1, 
                 const Self &ovl2, 
                 Point_location_base* pl_ptr,
                 Change_notification* pmwx_change_notf, 
                 Map_overlay_base *ovl_ptr)
  {
    PTR = new Ovl_rep(*(ovl1.ptr()),
                      *(ovl2.ptr()),
                      pl_ptr,
                      pmwx_change_notf,ovl_ptr);
    
    ((Ovl_rep*) ovl1.PTR)->add_reference();
    ((Ovl_rep*) ovl2.PTR)->add_reference();
    first_creator_ = &ovl1;
    second_creator_ = &ovl2;
  }
  
  // ------------------- Copy contructor -----------------------------
  Map_overlay_2 (const Self &ovl) : Handle(ovl) 
  {
    // check this again.
    ((Ovl_rep*) ovl.first_creator_->PTR)->add_reference();
    ((Ovl_rep*) ovl.second_creator_->PTR)->add_reference();
    
    first_creator_ = ovl.first_creator_;
    second_creator_ = ovl.second_creator_;
  }
 
  // conversion from rep to handle.
  //Map_overlay_2(const Ovl_rep* rep) 
  // {
  //  (Ovl_rep*) PTR = rep;
  //  ((Ovl_rep*) PTR)->add_reference();
  // }
  
  ~Map_overlay_2() 
  {
    if (first_creator_)
      ((Ovl_rep*) first_creator_->PTR)->remove_reference();
    if (second_creator_)
      ((Ovl_rep*) second_creator_->PTR)->remove_reference();
  }

  void	 delete_subtree() 
  {
    ptr()->delete_subtree();
  }
  
  const Subdivision& subdivision() const 
  { 
    return *(ptr()->arr_); 
  }
 
  //const Self&  first_creator () const
  //{ 
  //  return first_creator_;
  // }
  
  //const Self&  second_creator () const
  //{ 
  //  return second_creator_;
  // }

  const Self&  first_creator () const
  { 
    //const Self creator(ptr()->sub_division1);
    //return creator;
    
    return *first_creator_;
  }
  
  const Self&  second_creator () const
  { 
    //const Self creator(ptr()->sub_division2);
    // return creator;
    
    return *second_creator_;
  }
  
  const Change_notification* change_notification() const 
  { 
    return ptr()->ovl_change_notf; 
  }
private:
  const Self *first_creator_, *second_creator_;
};

CGAL_END_NAMESPACE

#endif







