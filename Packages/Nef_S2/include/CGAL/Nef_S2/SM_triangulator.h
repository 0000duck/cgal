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
// release       : $CGAL_Revision$
// release_date  : $CGAL_Date$
//
// file          : include/CGAL/Nef_S2/SM_triagulator.h
// package       : Nef_S2 
// chapter       : Nef Polyhedra
//
// revision      : $Revision$
// revision_date : $Date$
//
// author(s)     : Michael Seel <seel@mpi-sb.mpg.de>
// maintainer    : Michael Seel <seel@mpi-sb.mpg.de>
// coordinator   : Michael Seel <seel@mpi-sb.mpg.de>
//
// implementation: Triangulation module for sphere maps
// ============================================================================

#ifndef CGAL_SM_TRIANGULATOR_H
#define CGAL_SM_TRIANGULATOR_H

#include <CGAL/basic.h>
#include <CGAL/Partition.h>
#include <CGAL/Unique_hash_map.h>
#include <CGAL/Nef_2/Segment_overlay_traits.h>
#include <CGAL/Nef_2/geninfo.h>
#include <CGAL/Nef_S2/SM_decorator.h>
#include <CGAL/Nef_S2/SM_io_parser.h>
#include <CGAL/Nef_S2/SM_constrained_triang_traits.h>

#undef _DEBUG
#define _DEBUG 137
#include <CGAL/Nef_S2/debug.h>

#define USING(t) typedef typename Base::t t
#ifndef CGAL_USE_LEDA
#define LEDA_MEMORY(t) 
#endif
CGAL_BEGIN_NAMESPACE

template <typename Decorator_, typename IT, typename INFO>
struct SM_subdivision {
  typedef Decorator_ Triangulator;
  typedef typename Decorator_::Explorer Explorer;
  typedef typename Decorator_::Vertex_handle Vertex_handle;
  typedef typename Decorator_::Halfedge_handle   Halfedge_handle;
  typedef typename Decorator_::Sphere_point   Point;
  typedef typename Decorator_::Sphere_segment Segment;
  Triangulator T;
  Explorer E;
  CGAL::Unique_hash_map<IT,INFO>& M;
  /* M stores the object that supports the segment that
     is input object of the sweep */

  SM_subdivision(Triangulator Ti, Explorer Ei,
                 CGAL::Unique_hash_map<IT,INFO>& Mi) : 
    T(Ti), E(Ei), M(Mi) {}

Vertex_handle new_vertex(const Point& p) const
{ Vertex_handle v = T.new_vertex(p); T.assoc_info(v);
  return v;
}

void link_as_target_and_append(Vertex_handle v, Halfedge_handle e) const
{ T.link_as_target_and_append(v,e); }

Halfedge_handle new_halfedge_pair_at_source(Vertex_handle v) const
{ Halfedge_handle e = 
  T.new_edge_pair_at_source(v,Decorator_::BEFORE); 
  T.assoc_info(e);
  return e;
}

void halfedge_below(Vertex_handle v, Halfedge_handle e) const 
{ T.halfedge_below(v) = e; }

/* the following operation associates segment support with
   halfedges, we only update if non-NULL; this prevents 
   artificial sphere subdivision segments that have NULL 
   support to overwrite non-NULL support */

void supporting_segment(Halfedge_handle e, IT it) const
{ T.is_forward(e) = true; 
  if ( M[it] != NULL ) T.support(e) = M[it]; }

/* the following operation associate segment support with
   vertices, we only update if non-NULL; this prevents 
   artificial segments that have NULL support to overwrite
   non-NULL support */

void trivial_segment(Vertex_handle v, IT it) const
{ if ( M[it] != NULL ) T.support(v) = M[it]; }

void starting_segment(Vertex_handle v, IT it) const
{ if ( M[it] != NULL ) T.support(v) = M[it]; }

void ending_segment(Vertex_handle v, IT it) const
{ if ( M[it] != NULL ) T.support(v) = M[it]; }

void passing_segment(Vertex_handle v, IT it) const
{ if ( M[it] != NULL ) T.support(v) = M[it]; }


}; // SM_subdivision



/*{\Manpage {SM_triangulator}{Decorator_}{Overlay in the sphere}{O}}*/

template <typename Decorator_>
class SM_triangulator : public Decorator_ {
public:
  /*{\Mdefinition An instance |\Mvar| of data type |\Mname| is a
  decorator object offering sphere map triangulation calculation.}*/

  typedef Decorator_                    Base;
  typedef typename Decorator_::Base     Explorer;
  typedef Decorator_                    Decorator;
  typedef SM_triangulator<Decorator_>   Self;
  USING(Vertex_handle);
  USING(Halfedge_handle);
  USING(Halfloop_handle);
  USING(Face_handle);
  USING(Vertex_iterator);
  USING(Halfedge_iterator);
  USING(Face_iterator);
  USING(Vertex_const_handle);
  USING(Halfedge_const_handle);
  USING(Halfloop_const_handle);
  USING(Face_const_handle);
  USING(Vertex_const_iterator);
  USING(Halfedge_const_iterator);
  USING(Face_const_iterator);
  USING(Object_handle);
  USING(Halfedge_around_vertex_circulator);
  USING(Halfedge_around_face_circulator);
  USING(Sphere_map);
  typedef std::pair<Halfedge_handle,Halfedge_handle> Halfedge_pair;

  /*{\Mtypes 3}*/

  typedef typename Base::Kernel Kernel;

  typedef typename Kernel::Sphere_point   Sphere_point;
  /*{\Mtypemember the point type of the sphere geometry.}*/
  typedef typename Kernel::Sphere_segment Sphere_segment;
  /*{\Mtypemember the segment type of the sphere geometry.}*/
  typedef typename Kernel::Sphere_circle  Sphere_circle;
  /*{\Mtypemember the circle type of the sphere geometry.}*/
  typedef typename Kernel::Sphere_triangle Sphere_triangle;
  /*{\Mtypemember the triangle type of the sphere geometry.}*/

  typedef typename Decorator::Mark        Mark;
  /*{\Mtypemember the mark of sphere map objects.}*/

  /*{\Mgeneralization Decorator_}*/

protected:
  Explorer   E_;
  const Kernel& K;

public:

  typedef std::list<Sphere_segment>            Seg_list;
  typedef typename Seg_list::iterator          Seg_iterator;
  typedef std::pair<Seg_iterator,Seg_iterator> Seg_it_pair;
  typedef std::pair<Sphere_segment,Sphere_segment> Seg_pair;
  typedef CGAL::Unique_hash_map<Seg_iterator,Object_handle> Seg_map;

  // vertex_info stores the origin of vertices
  struct vertex_info {
    Object_handle         o_;
    Halfedge_handle       e_;
    vertex_info() : o_(),e_() {}
    LEDA_MEMORY(vertex_info)
  };

  void assoc_info(Vertex_handle v) const
  { geninfo<vertex_info>::create(info(v)); }

  void discard_info(Vertex_handle v) const
  { geninfo<vertex_info>::clear(info(v)); }

  vertex_info& ginfo(Vertex_handle v) const
  { return geninfo<vertex_info>::access(info(v)); }

  Object_handle& support(Vertex_handle v) const
  { return ginfo(v).o_; }

  Halfedge_handle& halfedge_below(Vertex_handle v) const
  { return ginfo(v).e_; }

  // edge_info stores the origin of edges
  struct edge_info {
    Mark m_left_; Object_handle o_; bool forw_;

    edge_info() { m_left_=Mark(); o_=Object_handle(); forw_=false; }
    LEDA_MEMORY(edge_info)
  };

  void assoc_info(Halfedge_handle e)  const
  { geninfo<edge_info>::create(info(e)); 
    geninfo<edge_info>::create(info(twin(e))); }

  void discard_info(Halfedge_handle e)  const
  { geninfo<edge_info>::clear(info(e)); 
    geninfo<edge_info>::clear(info(twin(e))); }

  edge_info& ginfo(Halfedge_handle e)  const
  { return geninfo<edge_info>::access(info(e)); }

  Object_handle& support(Halfedge_handle e) const
  // uedge information we store in the smaller one 
  { if (&*e < &*(twin(e))) return ginfo(e).o_; 
    else                   return ginfo(twin(e)).o_; }

  Mark& incident_mark(Halfedge_handle e)  const
  // biedge information we store in the edge
  { return ginfo(e).m_left_; }

  const edge_info& ginfo(Halfedge_const_handle e)  const
  { return geninfo<edge_info>::const_access(info(e)); }
  const Mark& incident_mark(Halfedge_const_handle e)  const
  { return ginfo(e).m_left_; }

  bool& is_forward(Halfedge_handle e) const
  // biedge information we store in the edge
  { return ginfo(e).forw_; }

  void assert_equal_marks(Vertex_handle v1, Vertex_handle v2) const 
  { CGAL_assertion(mark(v1)==mark(v2)); }

  void assert_equal_marks(Halfedge_handle e1, Halfedge_handle e2) const
  { CGAL_assertion(mark(e1)==mark(e2)); }

  Sphere_segment segment(Explorer N, 
                         Halfedge_const_handle e) const
  { return Sphere_segment(
      N.point(N.source(e)),N.point(N.target(e)),N.circle(e)); }

  Sphere_segment trivial_segment(Explorer N, 
                                 Vertex_const_handle v) const
  { Sphere_point p = N.point(v); 
    return Sphere_segment(p,p); }

  Seg_pair two_segments(Explorer N, 
                        Halfedge_const_handle e) const
  // we know that source(e)==target(e)
  { return N.circle(e).split_at(N.point(N.source(e))); }

  Seg_pair two_segments(Explorer N, 
                        Halfloop_const_handle l) const
  { return N.circle(l).split_at_xy_plane(); }


  Mark& mark(Vertex_handle h) const
  { return Base::mark(h); }
  Mark& mark(Halfedge_handle h) const
  { return Base::mark(h); }
  Mark& mark(Halfloop_handle h) const
  { return Base::mark(h); }
  Mark& mark(Face_handle h) const
  { return Base::mark(h); }
  const Mark& mark(Vertex_const_handle h) const
  { return Base::mark(h); }
  const Mark& mark(Halfedge_const_handle h) const
  { return Base::mark(h); }
  const Mark& mark(Halfloop_const_handle h) const
  { return Base::mark(h); }
  const Mark& mark(Face_const_handle h) const
  { return Base::mark(h); }

  /*{\Mcreation 6}*/
  SM_triangulator(const Sphere_map& M, Sphere_map& MT,
		  const Kernel& Kr = Kernel()) : 
    Base(MT), E_(const_cast<Sphere_map&>(M)), K(Kr) {}
  /*{\Mcreate |\Mvar| is a triangulator object for the map |M|,
     stores the triangulation in |MT|.}*/

  /*{\Moperations 1.1 1}*/

  void triangulate();
  /*{\Mop produces a triangulated sphere map.}*/

  void triangulate_per_hemisphere(Vertex_iterator start, Vertex_iterator end);

  template <typename Iterator, typename T>
  void partition_to_halfsphere(Iterator start, Iterator end,
    Seg_list& L, CGAL::Unique_hash_map<Iterator,T>& M, int pos) const;

  void merge_halfsphere_maps(Vertex_handle v1, Vertex_handle v2) const;
  void merge_nodes(Halfedge_handle e1, Halfedge_handle e2) const;
  void complete_support(Vertex_iterator v_start, Vertex_iterator v_end, 
			int pos) const;

  void correct_triangle_at(Vertex_handle v)
  { TRACEN("correct_triangle_at "<<PH(v));
    if ( !has_outdeg_two(v) ) return;
    Halfedge_handle e = first_out_edge(v);
    CGAL_assertion(next(next(next(e)))==e);
    flip_diagonal(next(e));
  }

  void dump(std::ostream& os = std::cerr) const
  { SM_io_parser<Explorer>::dump(E_,os);
    SM_io_parser<Base>::dump(*this,os); }

  Sphere_triangle incident_triangle(Halfedge_handle e) const
  { Halfedge_handle en(next(e)), enn(next(en));
    CGAL_assertion(next(enn)==e);
    return Sphere_triangle(
      point(source(e)),point(source(en)),point(source(enn)),
      circle(e),circle(en),circle(enn));
  }

  Sphere_triangle incident_triangle(Halfedge_const_handle e) const
  { Halfedge_const_handle en(next(e)), enn(next(en));
    CGAL_assertion(next(enn)==e);
    return Sphere_triangle(
      point(source(e)),point(source(en)),point(source(enn)),
      circle(e),circle(en),circle(enn));
  }

  void discard_info()
  {
    Vertex_iterator v;
    Halfedge_iterator e;
    CGAL_forall_vertices(v,*this) discard_info(v);
    CGAL_forall_halfedges(e,*this) discard_info(e);
  }


}; // SM_triangulator<Decorator_>


template <typename Decorator_>
void SM_triangulator<Decorator_>::triangulate()
{ TRACEN("triangulate");
  // first create sphere segments from isoverts, edges, loops
  Seg_list L;
  Seg_map From;
  CGAL::Unique_hash_map<Halfedge_const_iterator,bool> Done(false);
  Vertex_const_iterator v;
  CGAL_forall_vertices(v,E_) {
    if ( !E_.is_isolated(v) ) continue;
    L.push_back(trivial_segment(E_,v));
    From[--L.end()] = make_object(v);
  }
  Halfedge_const_iterator e;
  CGAL_forall_halfedges(e,E_) {
    if ( Done[e] ) continue;
    if ( source(e) == target(e) ) {
      Seg_pair p = two_segments(E_,e);
      L.push_back(p.first); L.push_back(p.second);
      From[--L.end()] = From[--(--L.end())] = make_object(e);
    } else {
      L.push_back(segment(E_,e));
      From[--L.end()] = make_object(e);
    }
    Done[e] = Done[twin(e)] = true;
      
  }
  if ( E_.has_loop() ) {
    Seg_pair p = two_segments(E_,E_.halfloop());
    L.push_back(p.first); L.push_back(p.second);
    From[--L.end()] = From[--(--L.end())] = make_object(E_.halfloop());
  }

  // partition segments from L to positive and negative hemisphere
  Seg_list L_pos,L_neg;
  partition_to_halfsphere(L.begin(), L.end(), L_pos, From, +1);
  partition_to_halfsphere(L.begin(), L.end(), L_neg, From, -1);

  // sweep the hemispheres to create two half sphere maps
  typedef SM_subdivision<Self,Seg_iterator,Object_handle> SM_output;
  typedef typename Kernel::Positive_halfsphere_geometry PH_geometry;
  typedef CGAL::Segment_overlay_traits< 
            Seg_iterator, SM_output, PH_geometry>  PHS_traits;
  typedef CGAL::generic_sweep<PHS_traits> Positive_halfsphere_sweep;

  typedef typename Kernel::Negative_halfsphere_geometry NH_geometry;
  typedef CGAL::Segment_overlay_traits< 
            Seg_iterator, SM_output, NH_geometry> NHS_traits;
  typedef CGAL::generic_sweep<NHS_traits> Negative_halfsphere_sweep;

  Vertex_handle v_sep;
  Halfedge_handle e_sep;
  SM_output O(*this,E_,From); 

  typedef typename PHS_traits::INPUT Input_range;
  Positive_halfsphere_sweep SP(
    Input_range(L_pos.begin(),L_pos.end()),O,
    K.get_positive_halfsphere_geometry());
  SP.sweep();
  v_sep=--vertices_end(); e_sep=--halfedges_end();

  Negative_halfsphere_sweep SM(
    Input_range(L_neg.begin(),L_neg.end()),O,
    K.get_negative_halfsphere_geometry());
  SM.sweep();
  ++v_sep; ++e_sep;
  // now two CCs of sphere graph are calculated
  // v_sep = first vertex of CC in negative x-sphere
  // e_sep = first edge of CC in negative x-sphere
   
  // enrich the edges by circle information
  CGAL::Unique_hash_map<Halfedge_iterator,bool> Done2(false);
  for(Halfedge_iterator e = halfedges_begin(); e != halfedges_end(); ++e)
  { if ( Done2[e] ) continue;
    Sphere_segment s(point(source(e)),point(target(e)));
    circle(e) = s.sphere_circle(); 
    circle(twin(e)) = s.sphere_circle().opposite();
    Done2[e] = Done2[twin(e)] = true;
  }

  // complete the support of per hemisphere
  complete_support(vertices_begin(), v_sep, +1);
  complete_support(v_sep, vertices_end(), -1);

  // triangulate per hemisphere
  typedef SM_constrained_triang_traits<Self,PH_geometry>  PCT_traits;
  typedef CGAL::generic_sweep<PCT_traits> Positive_halfsphere_ct_sweep;
  typedef SM_constrained_triang_traits<Self,NH_geometry>  NCT_traits;
  typedef CGAL::generic_sweep<NCT_traits> Negative_halfsphere_ct_sweep;
  typedef std::pair<Vertex_iterator,Vertex_iterator> Vertex_pair;

  Vertex_pair vpp(vertices_begin(),v_sep);
  Positive_halfsphere_ct_sweep PCTS(vpp, *this,
    K.get_positive_halfsphere_geometry());
  PCTS.sweep();
  Vertex_pair vpn(v_sep,vertices_end());
  Negative_halfsphere_ct_sweep NCTS(vpn, *this,
    K.get_negative_halfsphere_geometry());
  NCTS.sweep();

  /* Note the we divide the world along the xy equator and 
     split the equator at y- and y+. We treat the halfcircle
     at x+ as if perturbed slightly up. This makes triangles
     that have y- or y+ as a vertex degenerate. if such triangles
     appear we repair it by flipping the edge opposite to the
     vertex y-(y+).
  */
  correct_triangle_at(vertices_begin());
  correct_triangle_at(--Vertex_iterator(v_sep));
  correct_triangle_at(v_sep);
  correct_triangle_at(--vertices_end());

  // enrigh triangulation edges by circle information
  for(Halfedge_iterator e = halfedges_begin(); e != halfedges_end(); ++e)
  { if ( Done2[e] ) continue;
    Sphere_segment s(point(source(e)),point(target(e)));
    circle(e) = s.sphere_circle();
    circle(twin(e)) = s.sphere_circle().opposite();
    Done2[e] = Done2[twin(e)] = true;
  }

  // merge the hemisphere maps into one sphere map
  merge_halfsphere_maps(vertices_begin(),v_sep);
  check_integrity_and_topological_planarity(false);
}


template <typename Decorator_>
template <typename Iterator, typename T>
void SM_triangulator<Decorator_>::
partition_to_halfsphere(Iterator start, Iterator beyond, Seg_list& L, 
  CGAL::Unique_hash_map<Iterator,T>& M, int pos) const
{ TRACEN("partition_to_halfsphere ");
  CGAL_assertion(pos!=0);
  Sphere_segment s1,s2;
  Sphere_circle xycircle(0,0,pos);
  while ( start != beyond ) { 
    int i = start->intersection(xycircle,s1,s2);
    if (i>1) { L.push_back(s2); M[--L.end()] = M[start]; }
    if (i>0) { L.push_back(s1); M[--L.end()] = M[start]; }
    ++start;
  }
  // now all segments are split into halfspheres
  // we still have to:
  // - split segments containing our special poles y^-, y^+
  // - split halfcircles
  // - add four equator segments 
  Sphere_point S(0,-1,0),N(0,1,0);
  Sphere_circle yzcircle(1,0,0);
  typename Seg_list::iterator it, itl;

  bool part_in_hemisphere(false);
  CGAL_forall_iterators(it,L) { TRACEN("  "<<*it);
    if ( equal_as_sets(it->sphere_circle(),xycircle) ) {
      TRACEN("  splitting xy seg "<<*it);
      int n1 =  it->intersection(yzcircle,s1,s2);
      if (n1 > 1 && !s2.is_degenerate()) 
      { M[ L.insert(it,s2) ] = M[it]; }
      if (n1 > 0 && !s1.is_degenerate()) 
      { M[ L.insert(it,s1) ] = M[it]; }
      int n2 =  it->intersection(yzcircle.opposite(),s1,s2);
      if (n2 > 1 && !s2.is_degenerate()) 
      { M[ L.insert(it,s2) ] = M[it]; }
      if (n2 > 0 && !s1.is_degenerate()) 
      { M[ L.insert(it,s1) ] = M[it]; }
      itl = it; --it; L.erase(itl); M[itl] = T();
      // at least one item was appended
    } else {
      part_in_hemisphere = true;
    }
  }
  CGAL_forall_iterators(it,L) {
    if ( it->is_halfcircle() ) {
      TRACEN("  splitting halfcircle "<<*it);
      Sphere_segment s1,s2;
      it->split_halfcircle(s1,s2);
      *it = s2; 
      M[ L.insert(it,s1) ] = M[it];
    }
  }
  // append 4 xy-equator segments:
  Sphere_segment sp(S,N,xycircle);
  Sphere_segment sm(S,N,xycircle.opposite());
  Sphere_segment s[4];
  sp.split_halfcircle(s[0],s[1]);
  sm.split_halfcircle(s[2],s[3]);
  L.insert(L.end(),s,s+4);
  /* if no segment is covering the interior of the hemisphere
     we have to add a trivial segment to allow for a correct
     triangulation */
  if ( !part_in_hemisphere ) {
    Sphere_point p(0,0,pos);
    Sphere_circle c(1,0,0);
    L.push_back(Sphere_segment(p,p,c));
  }
}


template <typename Decorator_>
void SM_triangulator<Decorator_>::
merge_nodes(Halfedge_handle e1, Halfedge_handle e2) const
{
  Vertex_handle v1 = source(e1), v2 = target(e2);
  TRACEN("merge_nodes "<<PH(v1)<<PH(v2));
  CGAL_assertion(point(v1)==point(v2));
  Halfedge_handle ep1 = previous(e1), en2 = next(e2);
  Halfedge_around_vertex_circulator eav(out_edges(v2)),ee(eav);
  CGAL_For_all(eav,ee) { set_source(eav,v1); }
  link_as_prev_next_pair(e2,e1);  
  link_as_prev_next_pair(ep1,en2); 
  assert_equal_marks(v1,v2);
  discard_info(v2);
  delete_vertex_only(v2);
}

template <typename Decorator_>
void SM_triangulator<Decorator_>::
merge_halfsphere_maps(Vertex_handle v1, Vertex_handle v2) const
{ TRACEN("merging halfspheres "<<PH(v1)<<PH(v2));
  CGAL_assertion(point(v1)==point(v2));
  std::list<Halfedge_pair> L_equator;
  Halfedge_around_face_circulator 
    ep(last_out_edge(v1)), en(twin(first_out_edge(v2)));
  do { 
   L_equator.push_back(Halfedge_pair(ep,en));
   merge_nodes(ep,en); ++ep; --en; 
  } while ( source(ep) != v1 );
  
  typename std::list<Halfedge_pair>::iterator it;
  CGAL_forall_iterators(it,L_equator) { 
    Halfedge_handle e1 = it->first, e2 = it->second;
    Halfedge_handle e1t = twin(e1), e2t = twin(e2);
    TRACEV(PH(e1));TRACEV(PH(e2));
    assert_equal_marks(e1,e2);
    unify_marks(e1); unify_marks(e2);
    make_twins(e1,e2); make_twins(e1t,e2t);
    if ( e1 == first_out_edge(source(e1)) )
      set_first_out_edge(source(e1),e2t);
    if ( e2 == first_out_edge(source(e2)) )
      set_first_out_edge(source(e2),e1t);
    discard_info(e1);
    delete_edge_pair_only(e1);
  }
}

template <typename Decorator_>
void SM_triangulator<Decorator_>::
complete_support(Vertex_iterator v_start, Vertex_iterator v_end, int pos) const
{ 
  TRACEN("complete_support");
  for (Vertex_iterator v = v_start; v != v_end; ++v) { 
    TRACEN(" vertex = "<<PH(v));
    Mark m_buffer;
    Halfedge_handle e_below = halfedge_below(v);
    if ( v == v_start ) {
      m_buffer = E_.mark_of_halfsphere(-pos);
    } else if ( e_below != Halfedge_handle() ) {
      m_buffer = incident_mark(e_below); 
    } else { // e_below does not exist
      /* this is only the case for a vertex v on the final equatorial
	 halfcircle; there we take the mark from an inedge edge into v */
      CGAL_assertion( point(v).z() == 0 && 
        ( pos > 0 ? (point(v).x() >= 0) : (point(v).x()<=0)) );
      m_buffer = incident_mark(previous(first_out_edge(v)));
    } 
    TRACEN(" face mark below "<<m_buffer);

    Object_handle o = support(v);
    Vertex_const_handle vs;
    Halfedge_const_handle es;
    Halfloop_const_handle ls;
    if ( o == NULL ) { mark(v) = m_buffer; }
    else if ( assign(vs,o) ) { mark(v) = E_.mark(vs); }
    else if ( assign(es,support(v)) ) {
      if ( point(source(es)) == point(v) ) 
      { mark(v) = E_.mark(E_.source(es)); }
      else if ( point(target(es)) == point(v) ) 
      { mark(v) = E_.mark(E_.target(es)); }
      else { mark(v) = E_.mark(es); }
    }
    else if ( assign(ls,o) ) { mark(v) = E_.mark(ls); }
    else CGAL_assertion_msg(0,"damn wrong support.");
    TRACEN(" face mark at "<<mark(v));

    if ( is_isolated(v) ) continue;

    Halfedge_around_vertex_circulator e(first_out_edge(v)), hend(e);
    CGAL_For_all(e,hend) {
      TRACEN("  edge "<<PH(e));
      if ( !is_forward(e) ) break;
      if ( support(e) != NULL ) {
        Halfedge_const_handle ei;
        if ( assign(ei,support(e)) ) { 
          if ( E_.circle(ei) != circle(e) ) { ei = E_.twin(ei); }
          CGAL_assertion( E_.circle(ei) == circle(e) ); 
          TRACEN("  supporting edge "<<PH(ei));
          incident_mark(twin(e)) = E_.mark(E_.face(E_.twin(ei)));
          mark(e) = E_.mark(ei);
          incident_mark(e) = m_buffer = E_.mark(E_.face(ei)); 
        }
        Halfloop_const_handle li;
        if ( assign(li,support(e)) ) { 
          if ( E_.circle(li) != circle(e) ) { li = E_.twin(li); }
          CGAL_assertion( E_.circle(li) == circle(e) ); 
          TRACEN("  supporting loop "<<PH(li));
          incident_mark(twin(e)) = E_.mark(E_.face(E_.twin(li)));
          mark(e) = E_.mark(li);
          incident_mark(e) = m_buffer = E_.mark(E_.face(li));
        }
      } else { TRACEN("  support from face below ");
        incident_mark(twin(e)) = mark(e) = 
        incident_mark(e) = m_buffer;
      }
      TRACEN("  new face mark "<<m_buffer);

    } // CGAL_For_all(e,hend)

    TRACEN(" mark of "<<PH(v));
  }

}




CGAL_END_NAMESPACE
#undef USING
#endif //CGAL_SM_TRIANGULATOR_H


