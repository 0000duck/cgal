// Copyright (c) 1997-2002  Max-Planck-Institute Saarbruecken (Germany).
// All rights reserved.
//
// This file is part of CGAL (www.cgal.org); you may redistribute it under
// the terms of the Q Public License version 1.0.
// See the file LICENSE.QPL distributed with CGAL.
//
// Licensees holding a valid commercial license may use this file in
// accordance with the commercial license agreement provided with the software.
//
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
//
// $Source$
// $Revision$ $Date$
// $Name$
//
// Author(s)     : Michael Seel    <seel@mpi-sb.mpg.de>
//                 Miguel Granados <granados@mpi-sb.mpg.de>
//                 Susan Hert      <hert@mpi-sb.mpg.de>
//                 Lutz Kettner    <kettner@mpi-sb.mpg.de>
#ifndef CGAL_SNC_DECORATOR_H
#define CGAL_SNC_DECORATOR_H

#include <CGAL/basic.h>
#include <CGAL/Nef_3/Normalizing.h>
#include <CGAL/Unique_hash_map.h>
#include <CGAL/Nef_3/SNC_iteration.h>
#include <CGAL/Nef_3/SNC_const_decorator.h>
#include <CGAL/Nef_3/SNC_SM_decorator.h>
#include <CGAL/Nef_3/SNC_SM_point_locator.h>
#include <CGAL/Nef_3/SNC_SM_overlayer.h>
#include <CGAL/Nef_3/SNC_SM_io_parser.h>
#include <CGAL/Nef_3/SNC_constructor.h>
#include <CGAL/Nef_3/SNC_intersection.h>
#include <CGAL/Nef_3/SNC_point_locator.h>
#include <CGAL/Nef_3/binop_intersection_tests.h>

// #include <CGAL/Nef_3/SegRay.h>
#include <CGAL/IO/Verbose_ostream.h>
#ifdef  CGAL_NEF3_SM_VISUALIZOR
#include <CGAL/Nef_3/SNC_SM_visualizor.h>
#endif // CGAL_NEF3_SM_VISUALIZOR
#undef _DEBUG
#define _DEBUG 19
//#define CGAL_NEF3_DUMP_SPHERE_MAPS
#include <CGAL/Nef_3/debug.h>

CGAL_BEGIN_NAMESPACE

template <typename SNC_structure_>
class SNC_decorator : public SNC_const_decorator<SNC_structure_> { 
  typedef SNC_structure_ SNC_structure;
  typedef SNC_decorator<SNC_structure>           Self;
  typedef SNC_const_decorator<SNC_structure>     Base;
  typedef SNC_constructor<SNC_structure>         SNC_constructor;
  typedef SNC_SM_decorator<SNC_structure>        SM_decorator;
  typedef SNC_SM_const_decorator<SNC_structure>  SM_const_decorator;
  typedef SNC_SM_overlayer<SNC_structure>        SM_overlayer;
  typedef SNC_SM_point_locator<SNC_structure>    SM_point_locator;
  typedef SNC_intersection<SNC_structure>        SNC_intersection;
  typedef SNC_point_locator<SNC_structure>       SNC_point_locator;
  SNC_structure* sncp_;
  SNC_point_locator* pl0;

public:
#define USING(t) typedef typename SNC_structure::t t
  USING(Vertex_handle);
  USING(Halfedge_handle);
  USING(Halffacet_handle);
  USING(Volume_handle);
  USING(SVertex_handle);
  USING(SHalfedge_handle);
  USING(SHalfloop_handle);
  USING(SFace_handle);

  USING(Vertex_iterator);
  USING(Halfedge_iterator);
  USING(Halffacet_iterator); 
  USING(Volume_iterator);
  USING(SVertex_iterator);  
  USING(SHalfedge_iterator);
  USING(SHalfloop_iterator);
  USING(SFace_iterator);  

  USING(SHalfedge_around_facet_circulator);
  USING(SFace_cycle_iterator);
  USING(Halffacet_cycle_iterator);
  USING(Shell_entry_iterator);
#undef USING

#define USING(t) typedef typename Base::t t
  //USING(SHalfedge);
  USING(Object_handle);
  USING(SObject_handle);
  USING(Object_iterator);
  
  USING(Vertex);
  USING(Halfedge);
  USING(Halffacet);
  USING(Volume);
  USING(SVertex);
  USING(SHalfedge);
  USING(SHalfloop);
  USING(SFace);

  USING(Vertex_const_handle);
  USING(Halfedge_const_handle);
  USING(Halffacet_const_handle);
  USING(Volume_const_handle);
  USING(SVertex_const_handle);
  USING(SHalfedge_const_handle);
  USING(SHalfloop_const_handle);
  USING(SFace_const_handle);

  USING(Vertex_const_iterator);
  USING(Halfedge_const_iterator);
  USING(Halffacet_const_iterator);
  USING(Volume_const_iterator);
  USING(SVertex_const_iterator);
  USING(SHalfedge_const_iterator);
  USING(SHalfloop_const_iterator);
  USING(SFace_const_iterator);

  USING(SHalfedge_around_facet_const_circulator);
  USING(SFace_cycle_const_iterator);
  USING(Halffacet_cycle_const_iterator);
  USING(Shell_entry_const_iterator);

  USING(Kernel);
  USING(FT);
  USING(RT);

  USING(Point_3);
  USING(Segment_3);
  USING(Ray_3);
  USING(Line_3);
  USING(Plane_3);
  USING(Vector_3);
  USING(Direction_3);

  USING(Sphere_kernel);
  USING(Sphere_point);
  USING(Sphere_segment);
  USING(Sphere_circle);
  USING(Sphere_direction);

  USING(Size_type);
  USING(Mark);
  USING(Infi_box);
  USING(Aff_transformation_3);
#undef USING

#define DECUSING(t) typedef typename SM_decorator::t t
  DECUSING(SHalfedge_around_svertex_const_circulator);
  DECUSING(SHalfedge_around_svertex_circulator);
  DECUSING(SHalfedge_around_sface_circulator);
  DECUSING(SHalfedge_around_sface_const_circulator);
#undef DECUSING

  struct points_lt {
    bool operator()(Point_3& p1, Point_3& p2) const {
      return CGAL::lexicographically_xyz_smaller(p1,p2);
    }
  };

  enum {NO_SNC, WITH_SNC};

 public:
  typedef void* GenPtr;

#define using(f) using Base::f
  using(vertex);
  // using(twin);
  // using(source);
  using(target);
  // using(sface);
  /* SVertex queries */

  // using(vertex);
  // using(twin);
  // using(source);
  // using(target);
  // using(previous);
  // using(next);
  using(facet);
  using(sface);
  // using(ssource);
  using(starget);
  /* SHalfedge queries */

  // using(mark);
  // using(twin);
  // using(facet);
  // using(vertex);
  // using(sface);
  /* SHalfloop queries */

  // using(vertex);
  using(volume);
  /* SHalffacet queries */

  using(twin);
  // using(volume);
  /* Halffacet queries */

  using(point);
  // using(tmp_point);
  // using(calc_point);

  using(segment);
  using(plane);
  using(orthogonal_vector);
  // using(mark);
#undef using

  SNC_decorator() : Base(), sncp_(), pl0() {}
  SNC_decorator(SNC_structure& W, SNC_point_locator* pl = NULL) 
    : Base(W), sncp_(&W), pl0(pl) {}
  SNC_structure* sncp() const { 
    CGAL_nef3_assertion( sncp_ != NULL);
    return sncp_; 
  }
  SNC_point_locator* pl() const {
    CGAL_nef3_assertion( pl0 != NULL);
    return pl0;
  }
  void initialize(SNC_structure* W, SNC_point_locator* pl = NULL) { 
    sncp_ = W; 
    pl0 = pl;
  }

  static Vertex_handle vertex( Halfedge_handle e)
  { return e->center_vertex_; }
  static Halfedge_handle twin( Halfedge_handle e)
  { return e->twin_; }
  static Vertex_handle source( Halfedge_handle e)
  { return e->center_vertex_; }
  static Vertex_handle target( Halfedge_handle e)
  { return source(twin(e)); }
  static SFace_handle sface( Halfedge_handle e)
  { return e->incident_sface_; }
  /* SVertex queries*/

  static Vertex_handle vertex(SHalfedge_handle e)
  { return vertex(e->source_); }
  static SHalfedge_handle twin(SHalfedge_handle e)
  { return e->twin_; }
  static Vertex_handle source(SHalfedge_handle e)
  { return e->source_->center_vertex_; }
  static Vertex_handle source(SHalfedge e)
  { return e.source_->center_vertex_; }
  static Vertex_handle target(SHalfedge_handle e)
  { return e->twin_->source_->twin_->center_vertex_; }
  static SHalfedge_handle previous(SHalfedge_handle e)
  { return e->prev_; }
  static SHalfedge_handle next(SHalfedge_handle e)
  { return e->next_; }
  static Halffacet_handle facet(SHalfedge_handle e)
  { return e->incident_facet_; }
  static SFace_handle sface(SHalfedge_handle e)
  { return e->incident_sface_; }
  static Halfedge_handle ssource(SHalfedge_handle e)
  { return e->source_; }
  static Halfedge_handle starget(SHalfedge_handle e)
  { return e->twin_->source_; }
  /* SHalfedge queries */

  static const Mark& mark(SHalfedge_handle e)
  /*{\Mop returns the mark associated to |e| as
  a sphere object. This is temporary information!!!}*/
  { return e->tmp_mark(); }

  static const Sphere_circle& tmp_circle(SHalfedge_handle e)
  { return e->tmp_circle(); }
  static const Sphere_circle& tmp_circle(SHalfloop_handle l)
  { return l->tmp_circle(); }

  std::string debug(SHalfedge_handle e) const
  { std::stringstream os; set_pretty_mode(os);
    os << "sedge-use " << point(source(e)) << point(target(e))<<'\0';
    return os.str();
  }

  static SHalfloop_handle twin( SHalfloop_handle l)
  { return l->twin_; }
  static Halffacet_handle facet( SHalfloop_handle l)
  { return l->incident_facet_; }
  static Vertex_handle vertex( SHalfloop_handle l)
  { return l->incident_sface_->center_vertex_; }
  static SFace_handle sface( SHalfloop_handle l)
  { return l->incident_sface_; }
  /* SHalfloop queries */

  static Vertex_handle vertex(SFace_handle f)
  { return f->center_vertex_; }
  static Volume_handle volume(SFace_handle f)
  { return f->incident_volume_; }
  /* SHalffacet queries */

  static Halffacet_handle twin(Halffacet_handle f)
  { return f->twin_; }
  static Volume_handle volume(Halffacet_handle f)
    { return f->volume_; }
  /* Halffacet queries */

  SFace_handle adjacent_sface(Halffacet_handle f) const {
    Halffacet_cycle_iterator fc(f->facet_cycles_begin());
    CGAL_nef3_assertion( fc != f->facet_cycles_end());
    SHalfedge_handle se;
    if ( assign(se, fc) ) { 
      CGAL_nef3_assertion( facet(se) == f);
      CGAL_nef3_assertion( sface(se) != SFace_handle());
      CGAL_nef3_assertion( volume(sface(twin(se))) == volume(f));
      return sface(twin(se));
    } 
    else 
      CGAL_nef3_assertion_msg( 0, "Facet outer cycle entry point"
			     "is not an SHalfedge? ");
    return SFace_handle(); // never reached
  }

  // attributes::
  static Point_3& point(Vertex_handle v)
  { return v->point(); }
  static Point_3 standard_point(Vertex_handle v)  {
    Point_3 ep = v->point();
    return Point_3(ep.hx().eval_at(1000), 
		   ep.hy().eval_at(1000), 
		   ep.hz().eval_at(1000), 
		   ep.hw().eval_at(1000));
  }

  Sphere_point tmp_point(Halfedge_handle e) const
  { return e->tmp_point(); }
  Sphere_point calc_point(Halfedge_handle e) const
  { CGAL_nef3_assertion(twin(e)!=Halfedge_handle());
    Point_3 ps(point(source(e)));
    Point_3 pt(point(target(e)));
    return Sphere_point(pt-ps);
  }

  static Segment_3 segment(Halfedge_handle e)
  { return Segment_3(point(source(e)),
		     point(target(e))); }

  static Plane_3& plane(Halffacet_handle f)
  { return f->plane(); }

  Vector_3 orthogonal_vector(Halffacet_handle f) const
  { return f->plane().orthogonal_vector(); }

  Mark& mark(Vertex_handle v) const
  { return v->mark(); }
  Mark& mark(Halfedge_handle e) const
  { return e->mark(); }
  Mark& mark(Halffacet_handle f) const
  { return f->mark(); }
  Mark& mark(Volume_handle c) const
  { return c->mark(); }

  GenPtr& info(Vertex_handle v) const
  { return v->info(); }

  Mark& tmp_mark(Halfedge_handle e) const
  { return e->tmp_mark(); }

  template <typename H>
  bool is_boundary_object(H h) const
  { return sncp()->is_boundary_object(h); }

  template <typename H>
  void store_boundary_object(H h, Halffacet_handle f) const
  { f->boundary_entry_objects_.push_back(Object_handle(h));
    sncp()->store_boundary_item(h, --(f->facet_cycles_end()));
  }

  template <typename H>
  void store_as_first_boundary_object(H h, Halffacet_handle f) const
  { f->boundary_entry_objects_.push_front(Object_handle(h));
    sncp()->store_boundary_item(h, --(f->facet_cycles_end()));
  }

  template <typename H>
  void undo_boundary_object(H h, Halffacet_handle f) const
  { CGAL_nef3_assertion(sncp()->is_boundary_object(h));
    Halffacet_cycle_iterator it = sncp()->boundary_item(h);
    sncp()->undef_boundary_item(h);
    f->boundary_entry_objects_.erase(it);
  }

  void link_as_facet_cycle(SHalfedge_handle e, Halffacet_handle f) const
  { SHalfedge_around_facet_circulator hfc(e), hend(hfc);
    CGAL_For_all(hfc,hend) hfc->incident_facet_ = f;
    store_boundary_object(e,f);
  } 

  void link_as_interior_loop(SHalfloop_handle l, Halffacet_handle f) const
  { l->incident_facet_ = f;
    store_boundary_object(l,f);
  } 

  template <typename H>
  void make_twins(H h1, H h2) const
  { h1->twin_ = h2; h2->twin_ = h1; }

  void link_as_prev_next_pair(SHalfedge_handle e1, SHalfedge_handle e2) const
  { e1->next_ = e2; e2->prev_ = e1; } 

  template <typename H>
  void undo_boundary_object(H h, Volume_handle c) const
  { CGAL_nef3_assertion(sncp()->is_boundary_object(h));
    Shell_entry_iterator it = sncp()->boundary_item(h);
    sncp()->undef_boundary_item(h);
    c->shell_entry_objects_.erase(it);
  }

  template <typename H>
    void store_boundary_object(H h, Volume_handle c, bool at_front = false) const
  { 
    if(at_front) {
      c->shell_entry_objects_.push_front(Object_handle(h));
      sncp()->store_boundary_item(h, c->shells_begin());
    }
    else {
      c->shell_entry_objects_.push_back(Object_handle(h));
      sncp()->store_boundary_item(h, --(c->shells_end()));
    }
  }

  struct Shell_volume_setter {
    const SNC_decorator D;
    Volume_handle c;
    typedef Unique_hash_map< SFace_handle, bool> SFace_map;
    SFace_map linked;
    Shell_volume_setter(const SNC_decorator& Di)
      : D(Di), linked(false) {}
    void visit(SFace_handle h) { 
      TRACEN(D.point(D.vertex(h))); 
      D.set_volume(h, c); 
      linked[h] = true;
    }
    void visit(Vertex_handle h) { /* empty */ }
    void visit(Halfedge_handle h) { /* empty */ }
    void visit(Halffacet_handle h ) { D.set_volume(h, c); }
    void set_volume(Volume_handle ci) { c = ci; }
    bool is_linked(SFace_handle(h)) {return linked[h];}
  };

  void link_as_outer_shell( SFace_handle f, Volume_handle c ) const {
    //    CGAL_nef3_assertion(c->shell_entry_objects_.size() == 0);
    Shell_volume_setter Setter(*this);
    Setter.set_volume(c);
    visit_shell_objects( f, Setter );
    TRACEN("Volume "<<&*c<<", outer shell "<<&*f);
    store_boundary_object( f, c );
  }

  void link_as_inner_shell( SFace_handle f, Volume_handle c ) const {
    // CGAL_nef3_assertion(c->shell_entry_objects_.size() > 0);
    Shell_volume_setter Setter(*this);
    Setter.set_volume(c);
    visit_shell_objects( f, Setter );
    TRACEN("Volume "<<&*c<<", inner shell "<<&*f);
    store_boundary_object( f, c);
  }

  template <class H> void set_facet(H h, Halffacet_handle f) const 
    { h->incident_facet_ = f; }
  void set_volume(Halffacet_handle h, Volume_handle c) const
    { h->volume_ = c; }
  void set_volume(SFace_handle h, Volume_handle c) const 
    { h->incident_volume_ = c; }

  void add_sloop_to_facet(SHalfloop_handle l, Halffacet_handle f) const {
    SM_decorator SD(vertex(l));
    Sphere_circle facet_plane(plane(f));
    if( facet_plane == SD.circle(l)) {
      l->incident_facet_ = f;
      SD.twin(l)->incident_facet_ = twin(f);
    } else {
      CGAL_nef3_assertion( facet_plane.opposite() == SD.circle(l));
      l->incident_facet_ = twin(f);
      SD.twin(l)->incident_facet_ = f;
    }
  }

  // returns true if |v| is part of the infinimaximal box. ###################
  // Needs to be fixed for true infbox! LK! ##################################
  static bool is_infbox_vertex( Vertex_handle v) {
      return !Infi_box::is_standard(v->point());
  }

  /* returns true if |f| is part of the infinimaximal box.*/
  static bool is_infbox_facet(Halffacet_handle f) {
    return !Infi_box::is_standard(f->plane());
  }

  /* returns true when |v| has outdegree two.*/
  bool has_outdeg_two(SVertex_handle v) const {
    SM_decorator SD;
    if( SD.is_isolated(v))
      return false;
    SHalfedge_handle e1 = SD.first_out_edge(v);
    SHalfedge_handle e2 = SD.cyclic_adj_succ(e1);
    return( e1!=e2 && SD.cyclic_adj_succ(e2)==e1);
  }

  Halffacet_handle get_visible_facet( const Vertex_handle v, 
				      const Ray_3& ray) const
    /*{\Mop when one shoot a ray |ray| in order to find the facet below to
      an object, and vertex |v| is hit, we need to choose one of the facets
      in the adjacency list of |v| such that it could be 'seen' from the
      piercing point of the |ray| on the sphere map on |v|.  We make it just
      locating the sphere facet |sf| pierced by |ray| and taking the adjacent 
      facet to one of the sphere segments on the boundary of |sf|.
      \precondition |ray| target is on |v| and the intersection between
      |ray| and the 2-skeleton incident to v is empty. }*/ {

    Halffacet_handle f_visible;
    CGAL_nef3_assertion( ray.source() != point(v));
    CGAL_nef3_assertion( ray.has_on(point(v)));
    Sphere_point sp(ray.source() - point(v));
    TRACEN( "Locating "<<sp <<" in "<<point(v));
    CGAL_assertion(Infi_box::degree(sp.hx()) < 2 && 
		   Infi_box::degree(sp.hy()) < 2 && 
		   Infi_box::degree(sp.hz()) < 2 && 
		   Infi_box::degree(sp.hw()) == 0);
    sp = Infi_box::simplify(sp);
    TRACEN( "Locating "<<sp <<" in "<<point(v));
    SM_point_locator L(v);
    SObject_handle o = L.locate(sp);

    SFace_const_handle sf;
    //    CGAL_nef3_assertion(assign(sf,o));
    //    assign(sf,o);
    if(!assign( sf, o)) {
      CGAL_nef3_assertion_msg( 0, "it is not possible to decide which one is a visible facet (if any)");
      return Halffacet_handle();
    }
    /*    SM_decorator SD;
    if(sncp()->halfedges_begin() == sncp()->halfedges_end() || 
       SD.is_isolated(sncp()->halfedges_begin())) 
      return Halffacet_handle();  
    */

    SFace_cycle_const_iterator fc = sf->sface_cycles_begin(),
      fce = sf->sface_cycles_end();
    if( is_empty_range( fc, fce)) {
	TRACEN( "no adjacent facet found.");
	f_visible =  Halffacet_handle();
    }
    else {
      SHalfedge_handle se; 
      SHalfloop_handle sl;
      SVertex_handle sv;
      if ( assign( se, fc)) {
	TRACEN( "adjacent facet found (SEdges cycle).");
	TRACEN("se"<<PH(se));
	SM_decorator SD;
	TRACEN(plane(facet(se))<<"/"<<plane(facet(SD.next(se)))<<"/"<<plane(facet(SD.next(SD.next(se)))));
	f_visible = facet(twin(se));
	TRACEN("f_visible"<<plane(f_visible));
      }
      else if ( assign( sl, fc)) {
	SM_decorator SD;
	TRACEN( "adjacent facet found (SHalfloop cycle)."<<SD.circle(sl) 
		<< " with facet "<<plane(facet(sl)));
	f_visible = facet(twin(sl));
	TRACEN("f_visible"<<plane(f_visible));
      }
      else if( assign( sv, fc)) {
#ifdef _DEBUG
	// TODO: is there any warranty that the outter facet cycle enty point is always at first
	// in the cycles list?
	++fc; while( fc != fce)  { CGAL_nef3_assertion( assign( sv, fc)); ++fc; }
#endif
	TRACEN( "no adjacent facets were found (but incident edge(s)).");
	f_visible = Halffacet_handle();
      }
      else
	CGAL_nef3_assertion_msg(0,"Damn wrong handle");
    }
    return f_visible;
  }

  Halffacet_handle get_visible_facet( const Halfedge_handle e,
                                      const Ray_3& ray) const {
   //{\Mop when one shoot a ray |ray| in order to find the facet below to
   //  an object, and an edge |e| is hit, we need to choose one of the two 
   //  facets in the adjacency list of |e| that could be 'seen'  from the
   //  piercing point of the |ray| on the local (virtual) view  of |e|
   //  \precondition |ray| target belongs to |e|. } 

    SM_decorator SD(source(e));
    if( SD.is_isolated(e))
      return Halffacet_handle();
    
    // We search for the plane in the adjacency list of e, which is closest
    // to the ray. The cross product of the direction of e and the orthogonal
    // vector of a plane gives us a vector vec0/vec1 on the plane of the facet 
    // and orthogonal to e, pointing inside of the facet.

    Vector_3 ev(segment(e).direction()), rv(ray.direction());
    SHalfedge_around_svertex_circulator sh(SD.first_out_edge(e));
    Halffacet_handle res = facet(sh); 
    Vector_3 vec0(cross_product(ev,plane(res).orthogonal_vector()));
    CGAL_nef3_assertion_code
      (Sphere_segment _ess( SD.point(SD.source(sh)), 
			    SD.point(SD.source(next(sh))),
			    SD.circle(sh)));
    CGAL_nef3_assertion( _ess.has_on(vec0));
    SHalfedge_around_svertex_circulator send(sh);
    TRACEN("initial face candidate "<< plane(res)<<" with vector  "<<vec0);

    // We compare the vectors vec0/vec1 of the facets. The one that is nearest 
    // to pointing in the opposite direction of the ray, is chosen. The 
    // respective facet is the nearest to the ray.

    sh++;
    CGAL_For_all(sh,send) {
      Vector_3 vec1(cross_product(ev,plane(facet(sh)).orthogonal_vector()));
      TRACEN("test face candidate "<< plane(facet(sh))<<" with vector  "<<vec1);
      FT sk0(rv*vec0),  sk1(rv*vec1);
      if(sk0<FT(0) && sk1>FT(0))
        continue;
      if(sk0>FT(0) && sk1<FT(0)) {
        res = facet(sh); 
	vec0 = vec1;
	continue;
      }

      // We have to comapare the two skalar products sk0 and sk1. Therefore 
      // we have to normalize the input vectors vec0 and vec1, which means
      // that we have to divide them by their lengths len0 and len1. 
      // To cicumvent irrational numbers, we sqaure the whole inequality.

      FT len0 = vec0.x()*vec0.x()+vec0.y()*vec0.y()+vec0.z()*vec0.z();
      FT len1 = vec1.x()*vec1.x()+vec1.y()*vec1.y()+vec1.z()*vec1.z();
      FT diff = len0*sk1*sk1 - len1*sk0*sk0;

      if((sk0>FT(0) && diff<FT(0)) || (sk0<FT(0) && diff>FT(0))) {
        res = facet(sh);
	vec0 = vec1;
      }
    }
 
    // We have to check which of the two halffacet is visible from
    // the ray. 

    if(rv*plane(res).orthogonal_vector() > FT(0))
      res = twin(res);

    TRACEN("return "<<plane(res));
    return res; // never reached
  }

  Halffacet_handle get_visible_facet( const Halffacet_handle f,
				      const Ray_3& ray) const 
    /*{\Mop when one shoot a ray |ray| in order to find the facet below to
      an object, and a facet |f| is hit, we need to choose the right facet
      from the halffacet pair |f| that  could be 'seen'  from the
      piercing point of the |ray| on the local (virtual) view  of |f|.
      \precondition |ray| target belongs to |f| and the intersection between
      |ray| and is not coplanar with |f|. }*/ {

    Halffacet_handle f_visible = f;
    // CGAL_nef3_assertion( !plane(f_visible).has_on(ray.source()));
    if( plane(f_visible).has_on_negative_side(ray.source()))
      f_visible = twin(f);
    CGAL_nef3_assertion( plane(f_visible).has_on_positive_side(ray.source()));
    return f_visible;
  }

  Halffacet_handle get_visible_facet( const Vertex_handle v, 
				      const Segment_3& ray) const 
    /*{\Mop when one shoots a ray |ray| in order to find the facet below to
      an object, and vertex |v| is hit, we need to choose one of the facets
      in the adjacency list of |v| such that it could be 'seen' from the
      piercing point of the |ray| on the sphere map on |v|.  We make it just
      locating the sphere facet |sf| pierced by |ray| and taking the adjacent 
      facet to one of the sphere segments on the boundary of |sf|.
      \precondition |ray| target is on |v| and the intersection between
      |ray| and the 2-skeleton incident to v is empty. }*/ {

    Halffacet_handle f_visible;
    CGAL_nef3_assertion( ray.source() != point(v));
    CGAL_nef3_assertion( ray.has_on(point(v)));
    Sphere_point sp(ray.source() - point(v));
    TRACEN( "Locating "<<sp <<" in "<<point(v));
    CGAL_assertion(Infi_box::degree(sp.hx()) < 2 && 
		   Infi_box::degree(sp.hy()) < 2 && 
		   Infi_box::degree(sp.hz()) < 2 && 
		   Infi_box::degree(sp.hw()) == 0);
    sp = Infi_box::simplify(sp);
    TRACEN( "Locating "<<sp <<" in "<<point(v));
    SM_point_locator L(v);
    SObject_handle o = L.locate(sp);

    SFace_const_handle sf;
    CGAL_nef3_assertion(assign(sf,o));
    assign( sf, o);

    SFace_cycle_const_iterator fc = sf->sface_cycles_begin(),
      fce = sf->sface_cycles_end();
    if( is_empty_range( fc, fce)) {
	TRACEN( "no adjacent facets were found.");
	f_visible =  Halffacet_handle();
    }
    else {
      SHalfedge_handle se; 
      SHalfloop_handle sl;
      SVertex_handle sv;
      if ( assign( se, fc)) {
	TRACEN( "adjacent facet found (SEdges cycle).");
	TRACEN("se"<<PH(se));
	f_visible = facet(twin(se));
	TRACEN("f_visible"<<&f_visible);
      }
      else if ( assign( sl, fc)) {
	TRACEN( "adjacent facet found (SHalfloop cycle).");
	f_visible = facet(twin(sl));
      }
      else if( assign( sv, fc)) {
	TRACEN( "no adjacent facets were found (but incident edge(s)).");
	f_visible = Halffacet_handle();
      }
      else
	CGAL_nef3_assertion_msg(0,"Damn wrong handle");
    }
    return f_visible;
  }

  Halffacet_handle get_visible_facet( const Halfedge_handle e,
                                      const Segment_3& ray) const {
    //{\Mop when one shoot a ray |ray| in order to find the facet below to
    //  an object, and an edge |e| is hit, we need to choose one of the two 
   //   facets in the adjacency list of |e| that could be 'seen'  from the
   //   piercing point of the |ray| on the local (virtual) view  of |e|
   //   \precondition |ray| target belongs to |e|. } 

    CGAL_nef3_assertion(false);

    SM_decorator SD;
    if( SD.is_isolated(e))
      return Halffacet_handle();

    Halffacet_handle res = facet(sh);    

    Direction_3 ed(segment(e).direction());
    Vector_3 ev(segment(e).direction()), rv(ray.direction());
    SHalfedge_around_svertex_circulator sh(SD.first_out_edge(e)), send(sh);
    Vector_3 vec0(cross_product(ev,plane(res).orthogonal_vector()));
    TRACEN("initial face candidate "<< plane(res));
   
    sh++;
    CGAL_For_all(sh,send) {
    Vector_3 vec1(cross_product(ev,plane(sh).orthogonal_vector()));
      RT sk0(rv*vec0),  sk1(rv*vec1);
      if(sk0<0 && sk1>0)
        continue;
      if(sk0>0 && sk1<0) {
        res = facet(sh);
	continue;
      }

      RT len0 = vec0.x()*vec0.x()+vec0.y()*vec0.y()+vec0.z()*vec0.z();
      RT len1 = vec1.x()*vec1.x()+vec1.y()*vec1.y()+vec1.z()*vec1.z();
      RT sq0 = sk0 * sk0;
      RT sq1 = sk1 * sk1;     
      RT diff = len0*sq1 - len1*sq0;

      if((sk0 > 0 && diff<0) || (sk0 < 0 && diff>0))
        res = facet(sh);
    }

    return Halffacet_handle(); // never reached
  }

  Halffacet_handle get_visible_facet( const Halffacet_handle f,
				      const Segment_3& ray) const 
    //{\Mop when one shoot a ray |ray| in order to find the facet below to
    //  an object, and a facet |f| is hit, we need to choose the right facet
    //  from the halffacet pair |f| that  could be 'seen'  from the
    //  piercing point of the |ray| on the local (virtual) view  of |f|.
    //  \precondition |ray| target belongs to |f| and the intersection between
    //  |ray| and is not coplanar with |f|. } 
    {
      Halffacet_handle f_visible = f;
      CGAL_nef3_assertion( !plane(f_visible).has_on(ray.source()));
      if( plane(f_visible).has_on_negative_side(ray.source()))
	f_visible = twin(f);
      CGAL_nef3_assertion( plane(f_visible).has_on_positive_side(ray.source()));
      return f_visible;
    }

  template <typename Selection>
    Vertex_handle binop_local_views( Vertex_handle v0, Vertex_handle v1,
				     const Selection& BOP, SNC_structure& rsnc)
    /*{\opOverlays two spheres maps.}*/ {
  
#ifdef CGAL_NEF3_DUMP_SPHERE_MAPS
    typedef SNC_SM_io_parser<SNC_structure> SNC_SM_io_parser;
    SNC_SM_io_parser IO0( std::cerr, v0);
    SNC_SM_io_parser IO1( std::cerr, v1);
    TRACEN(" sphere maps before local binary operation");
    TRACEN(v0->debug());
    TRACEN(v1->debug());
    IO0.debug();    IO1.debug();
#endif // CGAL_NEF3_DUMP_SPHERE_MAPS
    CGAL_assertion( point(v0) == point(v1));
    Vertex_handle v01 = rsnc.new_vertex( point(v0), BOP( mark(v0),mark(v1)));
    //    cerr<<"BOP Vertex "<<mark(v0)<<" "<<mark(v1)<<std::endl;
    TRACEN("  binop result on vertex "<<&*v01<<" on "<<&*(v01->sncp()));
    SM_overlayer O(v01);
    O.subdivide( v0, v1);
    O.select( BOP);
    O.simplify();
    O.check_integrity_and_topological_planarity();

#ifdef CGAL_NEF3_DUMP_SPHERE_MAPS
    TRACEN(" result sphere map:");
    SNC_SM_io_parser IO01( std::cerr, v01);
    TRACEN(v01->debug());
    IO01.print();
    TRACEN(" sphere maps after local binary operation");
    IO0.debug();
    IO1.debug();
#endif //CGAL_NEF3_DUMP_SPHERE_MAPS

#ifdef CGAL_NEF3_SM_VISUALIZOR
    typedef SNC_SM_visualizor<SNC_structure> SMV;
    CGAL::OGL::add_sphere();
    SMV V0(v0, CGAL::OGL::spheres_.back());
    V0.draw_map();
    SMV V1(v1, CGAL::OGL::spheres_.back());
    V1.draw_map();
    SMV V01(v01, CGAL::OGL::spheres_.back());
    V01.draw_map();
    CGAL::OGL::start_viewer();
    TRACEN("any key to continue...");
    char c;
    std::cin >> c;
#endif // CGAL_NEF3_VISUALIZOR
    return v01;
  }

  Vertex_handle create_local_view_on( const Point_3& p, Halfedge_handle e) {
    SNC_constructor C(*sncp());
    return C.create_from_edge( e, p);
  }
  
  Vertex_handle create_local_view_on( const Point_3& p, Halffacet_handle f) {
    SNC_constructor C(*sncp());
    return C.create_from_facet( f, p);
  }

  Vertex_handle create_local_view_on( const Point_3& p, Volume_handle c) {
    Vertex_handle v = sncp()->new_vertex( p, mark(c));
    SM_decorator SD(v);
    SFace_handle f = SD.new_face();
    SD.mark(f) = mark(c);
    TRACEN("volume "<<&*c<<" marked as "<<mark(c)); 
    SM_point_locator PL(v);
    PL.init_marks_of_halfspheres(); // necessary to init default marks
    return v;
  }

  Vertex_handle 
    qualify_with_respect( const Point_3 p, 
			  const SNC_point_locator* pl1,
			  SNC_structure& result) {
    Vertex_handle v;
    Halfedge_handle e;
    Halffacet_handle f;
    Volume_handle c;
    SNC_decorator D(result);
    Object_handle o = pl1->locate(p);
    if( assign( v, o)) {
      TRACEN("<-> vertex local view on "<<point(v));
      return v;
    }
    else if( assign( e, o)) {
      TRACEN("<-> edge local view of "<<p<<" on "<<&*e);
      return D.create_local_view_on( p, e);
    }
    else if( assign( f, o)) {
      TRACEN("<-> facet local view of "<<p<<" on "<<&*f);
      return D.create_local_view_on( p, f);
    }
    else if( assign( c, o)) {
      TRACEN("<-> volume local view of "<<p<<" on "<<&*c);
      return D.create_local_view_on( p, c);
    }
    else CGAL_nef3_assertion_msg(0, "Where is the point then?");
    return Vertex_handle(); // never reached
  }

  Vertex_handle 
    qualify_with_respect( const Point_3& p, 
			  Object_handle o,
			  SNC_structure& snc) {
    Halfedge_handle e;
    Halffacet_handle f;
    if( assign( e, o))
      return create_local_view_on( p, e);
    else if( assign( f, o))
      return create_local_view_on( p, f);
    else 
      CGAL_nef3_assertion_msg( 0, "wrong handle");
    return Vertex_handle(); // never reached
  }

  template <typename Selection>
  class Intersection_call_back : 
    public SNC_point_locator::Intersection_call_back 
  {
  public:
    Intersection_call_back( SNC_structure& s0, SNC_structure& s1,
			    const Selection& _bop, SNC_structure& r, 
			    bool invert_order = false) :
      snc0(s0), snc1(s1), bop(_bop), result(r),
      inverse_order(invert_order) {}

    void operator()( Halfedge_handle e0, Object_handle o1, const Point_3& p)
      const {
      TRACEN("Intersection_call_back: intersection reported on " << p << " (normalized: " << normalized(p) << " )");
#ifdef _DEBUG
      SNC_decorator D0(snc0);
      TRACEN("edge 0 has source " << point(source(e0)) << " and direction " << D0.tmp_point(e0));
      Halfedge_handle e;
      Halffacet_handle f;
      SNC_decorator D1(snc1);
      if( assign( e, o1)) {
	TRACEN("edge 1 has source " << point(source(e)) << " and direction " << D1.tmp_point(e));
      }
      else if( assign( f, o1)) {
	TRACEN("face 1 has plane equation " << plane(f));
      }
      else 
      	CGAL_nef3_assertion_msg( 0, "wrong handle");
#endif      
      SNC_decorator D(result);
      Vertex_handle v0, v1;
      v0 = D.qualify_with_respect( p, Object_handle(e0), snc0);
      v1 = D.qualify_with_respect( p, o1, snc1);
      if( inverse_order)
	std::swap( v0, v1);
      D.binop_local_views( v0, v1, bop, result);
      result.delete_vertex(v0);
      result.delete_vertex(v1);
    }
  private:
    SNC_structure& snc0;
    SNC_structure& snc1;
    const Selection& bop;
    SNC_structure& result;
    bool inverse_order;
  };

  template <typename Selection>
    void binary_operation( SNC_structure& snc1i, // TODO: should it be const?
			   const SNC_point_locator* pl1,
			   const Selection& BOP,
			   SNC_structure& result,
			   SNC_point_locator* plresult)
    /*{\opPerforms a binary operation defined on |BOP| between two
      SNC structures.  The input structures are not modified and the
      result of the operation is stored in |result|.
      \precondition: the structure |result| is empty.}*/ {
    CGAL_nef3_assertion( result.is_empty());
    CGAL_nef3_assertion( pl1 != NULL);

    //    Progress_indicator_cout v_qualifying
    //      (sncp()->number_of_vertices()+snc1i.number_of_vertices(),
    //       "binary_operator: qualifying vertices...");

    Unique_hash_map<Vertex_handle, bool> ignore(false);
    Vertex_iterator v0, v1;
    SNC_decorator D(result);

    //    SETDTHREAD(19*131*43);
    
    TRACEN("=> binary operation");
    /*
    TRACEN("=> first operand:");
    SNC_io_parser<SNC_structure> O0(std::cout, *sncp());
    O0.print();
    TRACEN("=> second operand:");
    SNC_io_parser<SNC_structure> O1(std::cout, snc1i);
    O1.print();
    */
    TRACEN("\nnumber of vertices (so far...) = "<<result.number_of_vertices());

    TRACEN("=> for all v0 in snc0, qualify v0 with respect snc1");
    CGAL_nef3_forall_vertices( v0, *sncp()) {
      //      v_qualifying++;
      CGAL_nef3_assertion(!ignore[v0]);
      Point_3 p0(point(v0));
      Vertex_handle v;
      Halfedge_handle e;
      Halffacet_handle f;
      Volume_handle c;
      TRACEN("Locating point " << p0);
      Object_handle o = pl1->locate(p0);
      if( assign( v, o)) {
	v1 = v;
	binop_local_views( v0, v1, BOP, result);
	ignore[v1] = true;
	TRACEN("p0 found on vertex");
      }
      else if( assign( e, o)) {
	v1 = D.create_local_view_on( p0, e);
	binop_local_views( v0, v1, BOP, result);
	result.delete_vertex(v1);
	TRACEN("p0 found on edge");
      }
      else if( assign( f, o)) {
	v1 = D.create_local_view_on( p0, f);
	binop_local_views( v0, v1, BOP, result);
	result.delete_vertex(v1);
	TRACEN("p0 found on facet");
      }
      else if( assign( c, o)) {
	//	if( BOP( true, mark(c)) != BOP( false, mark(c))) {
	  // TODO: to copy the SM on v0, instead of create a volume local view
	  // and operate it with the vertex?
	  v1 = D.create_local_view_on( p0, c);
	  binop_local_views( v0, v1, BOP, result);
	  result.delete_vertex(v1);
	  TRACEN("p0 found on volume");
	  //	}
      }
      else CGAL_nef3_assertion_msg( 0, "wrong handle");
    }
    TRACEN("\nnumber of vertices (so far...) = "<<result.number_of_vertices());

    TRACEN("=> for all v1 in snc1, qualify v1 with respect snc0");
    CGAL_nef3_forall_vertices( v1, snc1i) {
      //      v_qualifying++;
      if(ignore[v1]) continue;
      Point_3 p1(point(v1));
      Vertex_handle v;
      Halfedge_handle e;
      Halffacet_handle f;
      Volume_handle c;
      Object_handle o = pl()->locate(p1);
      CGAL_nef3_assertion( !assign( v, o));
      if( assign( e, o)) {
	v0 = D.create_local_view_on( p1, e);
	binop_local_views( v0, v1, BOP, result);
	result.delete_vertex(v0);
	TRACEN("p1 found on edge");
      } 
      else if( assign( f, o)) {
	v0 = D.create_local_view_on( p1, f);
	binop_local_views( v0, v1, BOP, result);
	result.delete_vertex(v0);
	TRACEN("p1 found on facet");
      } 
      else if( assign( c, o)) {
	//	if( BOP( mark(c), true) != BOP( mark(c), false)) {
	  // TODO: to copy the SM on v0, instead of create a volume local view
	  // and operate it with the vertex?
	  v0 = D.create_local_view_on( p1, c);
	  binop_local_views( v0, v1, BOP, result);
	  result.delete_vertex(v0);
	  TRACEN("p1 found on volume");
	  //	}
      }
      else CGAL_nef3_assertion_msg( 0, "wrong handle");
    }
    TRACEN("\nnumber of vertices (so far...) = "<<result.number_of_vertices());

    // Each time the intersect method of the point locator for the 
    // SNC structure finds an intersection between the segment defined
    // by an edge on the other SNC structure, the call back method is
    // called with the intersecting objects and the intersection point.
    // The responsability of the call back functor is to construct the
    // local view on the intersection point on both SNC structures,
    // overlay them and add the resulting sphere map to the result.

    //    SETDTHREAD(19*509*43);

    Halfedge_iterator e0, e1;

    //    Progress_indicator_cout ee_intersections
    //      (sncp()->number_of_edges(),
    //       "binary_operator: finding edge-edge intersections...");

    Intersection_call_back<Selection> call_back0
      ( *sncp(), snc1i, BOP, result);
    Intersection_call_back<Selection> call_back1 
      ( snc1i, *sncp(), BOP, result, true);

    TRACEN("=> finding edge-edge intersections...");
    CGAL_nef3_forall_edges( e0, *sncp()) {
      //      ee_intersections++;
      pl1->intersect_with_edges( e0, call_back0);
    }
    TRACEN("\nnumber of vertices (so far...) = "<<result.number_of_vertices());

    //    Progress_indicator_cout ef_intersections
    //      (sncp()->number_of_edges()+snc1i.number_of_edges(),
    //       "binary_operator: finding edge-facet intersections...");

    TRACEN("number of vertices (so far...) = "<<result.number_of_vertices());
    TRACEN("=> finding edge0-facet1 intersections...");
    CGAL_nef3_forall_edges( e0, *sncp()) {
      //      ef_intersections++;
      pl1->intersect_with_facets( e0, call_back0);
    }
    TRACEN("\nnumber of vertices (so far...) = "<<result.number_of_vertices());

    TRACEN("=> finding edge1-facet0 intersections...");
    CGAL_nef3_forall_edges( e1, snc1i) {
      //      ef_intersections++;
      pl()->intersect_with_facets( e1, call_back1);
    }
    TRACEN("\nnumber of vertices (so far...) = "<<result.number_of_vertices());

    TRACEN("=> resultant vertices (before simplification): ");
    CGAL_nef3_forall_vertices( v0, result) TRACEN(&*v0<<" "<<point(v0));

    result.vertex_simplification(NO_SNC);

    TRACEN("=> resultant vertices (after simplification): ");
    CGAL_nef3_forall_vertices( v0, result) TRACEN(&*v0<<" "<<point(v0));
#ifdef CGAL_NEF3_DUMP_SNC_OPERATORS
    TRACEN("=> pre-construction result");
    SNC_io_parser<SNC_structure> O(std::cerr, result);
    O.print();
#endif
    
    CGAL_assertion( !result.is_empty());
    SNC_constructor C(result, plresult);
    C.build_external_structure();

#ifdef CGAL_NEF3_DUMP_SNC_OPERATORS
    TRACEN("=> construction completed, result: ");
    SNC_io_parser<SNC_structure> Op(std::cout, result);
    Op.print();
#endif

    CGAL_nef3_assertion(!result.simplify());
    TRACEN("=> end binary operation. ");
  }

  void compute_all_marks_of_halfspheres() {
    
    Vertex_handle v;
    CGAL_nef3_forall_vertices(v,*this) {
      SM_point_locator L(v);
      L.init_marks_of_halfspheres();
    }    
  }

  bool is_valid( bool verb = false, int level = 0) {
    
    Verbose_ostream verr(verb);
    verr << "begin CGAL::SNC_decorator<...>::is_valid( verb=true, "
      "level = " << level << "):" << std::endl;
    
    int max = number_of_vertices() 
      + number_of_halfedges() 
      + number_of_halffacets() 
      + number_of_volumes()
      + 2 * sncp()->number_of_shalfedges()
      + sncp()->number_of_shalfloops()
      + sncp()->number_of_sfaces();
    int count = 0;

    bool valid = true;

    Vertex_iterator vi;
    std::list<Point_3> Points(false);   // durch hashmap ersetzen    
    CGAL_nef3_forall_vertices(vi,*this) {
      if(!valid) break;

      valid = valid && (vi->sncp()==sncp());
      valid = valid && vi->is_valid(verb, level);

      SM_decorator SD(vi);
      Unique_hash_map<SVertex_handle, bool> SVvisited(false);
      Unique_hash_map<SHalfedge_handle, bool> SEvisited(false);
      Unique_hash_map<SFace_handle, bool> SFvisited(false);

      valid = valid && SD.is_valid(SVvisited,SEvisited,SFvisited, verb, level);

      Points.push_back(point(vi));

      Mark m1 = SD.mark_of_halfsphere(-1);  
      Mark m2 = SD.mark_of_halfsphere(+1);
      SM_point_locator PL(vi);
      PL.init_marks_of_halfspheres();
      valid = valid && (SD.mark_of_halfsphere(-1) == m1);
      valid = valid && (SD.mark_of_halfsphere(+1) == m2);
     
      TRACEN(m1 << "," << m2 << " should have been " << 
	     SD.mark_of_halfsphere(-1) << "," << SD.mark_of_halfsphere(+1));

      valid = valid && (++count <= max);
    }

 

    Points.sort(points_lt());
    typename std::list<Point_3>::const_iterator li1, li2;
    li2 = li1 = Points.begin();
    if(Points.empty())
      valid = false;
    else {
      li2++;
      while(valid && li2 != Points.end()) {
	valid = valid && (*li1++ != *li2++);
      }
    }

    Halfedge_iterator he;
    CGAL_nef3_forall_halfedges(he,*this) {
      valid = valid && he->is_valid(verb, level);
      valid = valid && (twin(he)!=he);
      valid = valid && (twin(twin(he))==he);

      if(he->is_twin()) {
	SM_decorator S1(source(he));
	SM_decorator S2(target(he));
	SHalfedge_handle se1(S1.first_out_edge(he));
	SHalfedge_handle se2(S2.first_out_edge(twin(he)));
	if(se1 != NULL && se2 != NULL) {
	  SHalfedge_handle start1(se1);
	  SHalfedge_handle start2(S2.next(twin(se2)));
	  while(facet(se1) != twin(facet(se2)) && se2 != start2)
	    se2 = twin(S2.previous(se2));
	  valid = valid && (facet(se1) == twin(facet(se2)));
	  start2 = se2;
	  do {
	    se1 = S1.next(twin(se1));
	    se2 = twin(S2.previous(se2));
	    valid = valid && (facet(se1) == twin(facet(se2)));
	  } while(se1 != start1);
	  valid = valid && (se2 == start2);
	}

	//	Line_3 supporting_line(point(source(he)), tmp_point(he));
	//	valid = valid && supporting_line.has_on(point(source(twin(he))));
      }
      valid = valid && (++count <= max);
    }

    Unique_hash_map<SHalfedge_handle, bool> SEinUniqueFC(false);
    Halffacet_iterator hfi;
    CGAL_nef3_forall_halffacets(hfi,*this) {
      valid = valid && hfi->is_valid(verb, level);
      valid = valid && (twin(hfi)!=hfi);
      valid = valid && (twin(twin(hfi))==hfi);
      valid = valid && (plane(hfi).opposite() == plane(twin(hfi))); 

      Halffacet_cycle_iterator hfci;
      CGAL_nef3_forall_facet_cycles_of(hfci,hfi) { 
	if(hfci.is_shalfedge()) {
	  SHalfedge_handle sheh = SHalfedge_handle(hfci);
	  valid = valid && (sheh != SHalfedge_handle());
	  SHalfedge_around_facet_circulator shec1(sheh), shec2(shec1);
	  CGAL_For_all(shec1, shec2) {
	    SM_decorator SD(vertex(shec1));
	    if(SEinUniqueFC[shec1])
	      TRACEN("redundant facet " << point(vertex(shec1)) << " | " << SD.circle(shec1));
	    CGAL_nef3_assertion(!SEinUniqueFC[shec1]);
	    SEinUniqueFC[shec1] = true;

	    Plane_3 p_ref(point(vertex(shec1)),SD.circle(shec1).opposite().orthogonal_vector());
	    valid = valid && (normalized(plane(hfi)) == normalized(p_ref));
	  }
	}
	else if(hfci.is_shalfloop())
	  valid = valid && (SHalfloop_handle(hfci) != SHalfloop_handle()); 
	else
	  valid = false;
	valid = valid && (++count <= max);	
      }
      
      valid = valid && (++count <= max);
    }

    Volume_iterator voli;
    CGAL_nef3_forall_volumes(voli,*this) {
      
      valid = valid && (voli->is_valid(verb, level));

      Shell_entry_iterator si;
      CGAL_nef3_forall_shells_of(si,voli) {
	valid = valid && (si != NULL && 
			  SFace_handle(si) != SFace_handle() && 
			  SFace_handle(si) != NULL);
	valid = valid && (++count <= max);
      }
      valid = valid && (++count <= max);
    }

    SHalfedge_iterator she;
    CGAL_nef3_forall_shalfedges(she,*sncp()) {
      valid = valid && (next(she) != she);
      valid = valid && (previous(she) != she);
      valid = valid && (previous(next(she)) == she);
      valid = valid && (next(previous(she)) == she);
      valid = valid && (next(twin(next(she))) == twin(she));
      valid = valid && (previous(twin(previous(she))) == twin(she)); 
      valid = valid && (twin(facet(she)) == facet(twin(she))); 
      valid = valid && (mark(facet(she)) == mark(she));
      valid = valid && (++count <= max);
    }

    SHalfloop_iterator shl;
    CGAL_nef3_forall_shalfloops(shl,*sncp()){
      SM_decorator SD;
      valid = valid && (mark(facet(shl)) == SD.mark(shl));
      //      valid = valid && (plane(facet(twin(shl))) == SD.circle(shl)); 
      //      valid = valid && (volume(facet(twin(shl))) == volume(sface(shl)));
    }

    SFace_iterator sf;
    CGAL_nef3_forall_sfaces(sf,*sncp()) {
      SM_decorator SD;
      valid = valid && (mark(volume(sf)) == SD.mark(sf));
    }

    verr << "end of CGAL::SNC_decorator<...>::is_valid(): structure is "
	 << ( valid ? "valid." : "NOT VALID.") << std::endl;

    return valid;
  }

  template <typename Visitor>
  void visit_shell_objects(SFace_handle f, Visitor& V) const;

  Vertex_iterator   vertices_begin()   { return sncp()->vertices_begin(); }
  Vertex_iterator   vertices_end()     { return sncp()->vertices_end(); }
  Halfedge_iterator halfedges_begin()  { return sncp()->halfedges_begin(); }
  Halfedge_iterator halfedges_end()    { return sncp()->halfedges_end(); }
  Halffacet_iterator halffacets_begin(){ return sncp()->halffacets_begin(); }
  Halffacet_iterator halffacets_end()  { return sncp()->halffacets_end(); }
  Volume_iterator   volumes_begin()    { return sncp()->volumes_begin(); }
  Volume_iterator   volumes_end()      { return sncp()->volumes_end(); }

  Shell_entry_iterator shells_begin(Volume_handle c) {
    return c->shells_begin();
  }
  Shell_entry_iterator shells_end(Volume_handle c) {
    return c->shells_end();
  }

  Size_type number_of_vertices() const  
  { return sncp()->number_of_vertices(); }
  Size_type number_of_halfedges() const 
  { return sncp()->number_of_halfedges(); }
  Size_type number_of_edges() const     
  { return sncp()->number_of_edges(); }
  Size_type number_of_halffacets() const    
  { return sncp()->number_of_halffacets();}
  Size_type number_of_facets() const    
  { return sncp()->number_of_facets();}
  Size_type number_of_volumes() const   
  { return sncp()->number_of_volumes();}

};




/* visiting shell objects:

Objects are marked as done, when placed in the output list.  We have
to maintain a stack of sface candidates (the spherical rubber sectors
that provide connectivity at the local graphs of vertices) and facet
candiates (the plane pieces in three space also providing
connectivity). Note that we have to take care about the orientation of
sobjects and facets. We have to take care that (1) the search along
the shell extends along the whole shell structure (2) does not visit
any object twice, and (3) all 3-space objects have to be reported and
this also just once.

The facets and sfaces are marked |done| when they are put into their
corresponding queues thus each such object is visited exactly once
when taken out of the queue. 

When an sface |sf| is taken out of the queue |SFaceCandiates| its
boundary structure is examined and all 2-skeleton objects (vertices
and edges of 3-space) that are incident to the volume represented by
|sf| are reported. Facets are reported when they are taken out of
|FacetCandiates|.

*/

template <typename EW>
template <typename Visitor>
void SNC_decorator<EW>::
visit_shell_objects(SFace_handle f, Visitor& V) const
{ 
  typedef typename SM_decorator::SHalfedge_around_sface_circulator 
    SHalfedge_around_sface_circulator;
  std::list<SFace_handle> SFaceCandidates;
  std::list<Halffacet_handle> FacetCandidates;
  CGAL::Generic_handle_map<bool> Done(false);
  SFaceCandidates.push_back(f);  Done[f] = true;
  while ( true ) {
    if ( SFaceCandidates.empty() && FacetCandidates.empty() ) break;
    if ( !FacetCandidates.empty() ) {
      Halffacet_handle f = *FacetCandidates.begin();
      FacetCandidates.pop_front();
      V.visit(f); // report facet
      Halffacet_cycle_iterator fc;
      CGAL_nef3_forall_facet_cycles_of(fc,f) {
        SHalfedge_handle e; SHalfloop_handle l;
        if ( assign(e,fc) ) {
          SHalfedge_around_facet_circulator ec(e),ee(e);
          CGAL_For_all(ec,ee) { e = twin(ec);
            if ( Done[sface(e)] ) continue;
            SFaceCandidates.push_back(sface(e));
            Done[sface(e)] = true;
          }
        } else if ( assign(l,fc) ) { 
	  l = twin(l);
          if ( Done[sface(l)] ) continue;
          SFaceCandidates.push_back(sface(l));
          Done[sface(l)] = true;
        } else CGAL_nef3_assertion_msg(0,"Damn wrong handle.");
      }
    }
    if ( !SFaceCandidates.empty() ) {
      SFace_handle sf = *SFaceCandidates.begin();
      SFaceCandidates.pop_front();
      V.visit(sf);
      if ( !Done[vertex(sf)] )
        V.visit(vertex(sf)); // report vertex
      Done[vertex(sf)] = true;
      //      SVertex_handle sv;
      SM_decorator SD(vertex(sf));
      /*      
      CGAL_nef3_forall_svertices(sv,SD){
	if(SD.is_isolated(sv) && !Done[sv])
	  V.visit(sv);
      }
      */
      SFace_cycle_iterator fc;
      CGAL_nef3_forall_sface_cycles_of(fc,sf) {
        SVertex_handle v; SHalfedge_handle e; SHalfloop_handle l;
        if ( assign(e,fc) ) {
	  SHalfedge_around_sface_circulator ec(e),ee(e);
          CGAL_For_all(ec,ee) { 
            v = starget(ec);
            if ( !SD.is_isolated(v) && !Done[v] ) {
              V.visit(v); // report edge
              Done[v] = Done[twin(v)] = true;
            }
            Halffacet_handle f = facet(twin(ec));
            if ( Done[f] ) continue;
            FacetCandidates.push_back(f); Done[f] = true;
          }
        } else if ( assign(v,fc) ) {
          if ( Done[v] ) continue; 
          V.visit(v); // report edge
	  V.visit(twin(v));
          Done[v] = Done[twin(v)] = true;
	  CGAL_assertion(SD.is_isolated(v));
	  SFaceCandidates.push_back(sface(twin(v)));
	  Done[sface(twin(v))]=true;
          // note that v is isolated, thus twin(v) is isolated too
	  //	  SM_decorator SD;
	  //	  SFace_handle fo;
	  //	  fo = sface(twin(v));
	  /*
	  if(SD.is_isolated(v)) 
	    fo = source(v)->sfaces_begin();
	  else
	    fo = sface(twin(v));
	  */
	  //	  if ( Done[fo] ) continue;
	  //	  SFaceCandidates.push_back(fo); Done[fo] = true;
        } else if ( assign(l,fc) ) {
          Halffacet_handle f = facet(twin(l));
          if ( Done[f] ) continue;
          FacetCandidates.push_back(f);  Done[f] = true;
        } else CGAL_nef3_assertion_msg(0,"Damn wrong handle.");
      }
    }
  }
}


CGAL_END_NAMESPACE
#endif //CGAL_SNC_DECORATOR_H

