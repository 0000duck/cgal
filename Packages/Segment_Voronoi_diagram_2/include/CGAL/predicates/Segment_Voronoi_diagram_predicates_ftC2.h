// ======================================================================
//
// Copyright (c) 2003 The CGAL Consortium
//
// This software and related documentation is part of an INTERNAL release
// of the Computational Geometry Algorithms Library (CGAL). It is not
// intended for general use.
//
// ----------------------------------------------------------------------
//
// release       : 
// release_date  : 
//
// file          :
//      include/CGAL/predicates/Segment_Voronoi_diagram_predicates_ftC2.h
// package       : Segment_Voronoi_diagram_2
// source        : $RCSfile$
// revision      : $Revision$
// revision_date : $Date$
// author(s)     : Menelaos Karavelas <mkaravel@cse.nd.edu>
//
// coordinator   :
//
// ======================================================================



#ifndef CGAL_SEGMENT_VORONOI_DIAGRAM_PREDICATES_FTC2_H
#define CGAL_SEGMENT_VORONOI_DIAGRAM_PREDICATES_FTC2_H

#include <CGAL/determinant.h>
#include <CGAL/predicates/Segment_Voronoi_diagram_predicates_C2.h>
#include <CGAL/predicates/check_filter.h>
#include <CGAL/Segment_Voronoi_diagram_kernel_wrapper_2.h>


CGAL_BEGIN_NAMESPACE

//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------

template<class K>
void svd_predicate_push_back_C2(const typename K::Site_2& t,
				std::vector<typename K::FT>& v,
				std::vector<char>& site_types)
{
  if ( t.is_point() ) {
    site_types.push_back('p');
    if ( t.is_exact() ) {
      site_types.push_back('e');
      v.push_back( t.point().x() );
      v.push_back( t.point().y() );
    } else {
      site_types.push_back('i');
      typename K::Segment_2 s1 = t.supporting_segment(0);
      typename K::Segment_2 s2 = t.supporting_segment(1);
      v.push_back( s1.source().x() );
      v.push_back( s1.source().y() );
      v.push_back( s1.target().x() );
      v.push_back( s1.target().y() );
      v.push_back( s2.source().x() );
      v.push_back( s2.source().y() );
      v.push_back( s2.target().x() );
      v.push_back( s2.target().y() );
    }
  } else {
    site_types.push_back('s');
    if ( t.is_exact() ) {
      site_types.push_back('e');
      v.push_back( t.source().x() );
      v.push_back( t.source().y() );
      v.push_back( t.target().x() );
      v.push_back( t.target().y() );
    } else {
      typename K::Segment_2 supp = t.supporting_segment();
      v.push_back( supp.source().x() );
      v.push_back( supp.source().y() );
      v.push_back( supp.target().x() );
      v.push_back( supp.target().y() );

      typename K::Segment_2 cs, cs2;
      char stype;
      if ( t.is_exact(0) ) {
	stype = '0';
	cs = t.crossing_segment(1);
      } else if ( t.is_exact(1) ) {
	stype = '1';
	cs = t.crossing_segment(0);
      } else {
	stype = 'i';
	cs = t.crossing_segment(0);
	cs2 = t.crossing_segment(1);
      }

      site_types.push_back( stype );
      v.push_back( cs.source().x() );
      v.push_back( cs.source().y() );
      v.push_back( cs.target().x() );
      v.push_back( cs.target().y() );

      if ( stype == 'i' ) {
	v.push_back( cs2.source().x() );
	v.push_back( cs2.source().y() );
	v.push_back( cs2.target().x() );
	v.push_back( cs2.target().y() );
      }
    }
  }
}


template<class K>
typename K::Site_2
get_site(const std::vector<typename K::FT>& v, unsigned int& k,
	 const std::vector<char>& site_types, unsigned int& j)
{
  typedef typename K::Point_2             Point_2;
  typedef typename K::Segment_2           Segment_2;
  typedef typename K::Site_2              Site_2;

  Site_2 t;

  unsigned int step(0);

  if ( site_types[j] == 'p' ) {
    if ( site_types[j+1] == 'e' ) {
      Point_2 p(v[k], v[k+1]);
      t.set_point(p);
      step = 2;
    } else {
      Point_2 p1(v[k], v[k+1]), p2(v[k+2], v[k+3]);
      Point_2 p3(v[k+4], v[k+5]), p4(v[k+6], v[k+7]);
      Segment_2 s1(p1, p2), s2(p3, p4);
      t.set_point(s1, s2);
      step = 8;
    }
  } else {
    if ( site_types[j+1] == 'e' ) {
      Point_2 p1(v[k], v[k+1]), p2(v[k+2], v[k+3]);
      Segment_2 s(p1, p2);
      t.set_segment(s);
      step = 4;
    } else {
      if ( site_types[j+1] != 'i' ) {
	Point_2 p1(v[k], v[k+1]), p2(v[k+2], v[k+3]);
	Point_2 p3(v[k+4], v[k+5]), p4(v[k+6], v[k+7]);
	Segment_2 supp(p1, p2);
	Segment_2 cs(p3, p4);
	t.set_segment(supp, cs, (site_types[j+1] == '0'));
	step = 8;
      } else {
	Point_2 p1(v[k], v[k+1]), p2(v[k+2], v[k+3]);
	Point_2 p3(v[k+4], v[k+5]), p4(v[k+6], v[k+7]);
	Point_2 p5(v[k+8], v[k+9]), p6(v[k+10], v[k+11]);
	Segment_2 supp(p1, p2);
	Segment_2 s1(p3, p4), s2(p5, p6);
	t.set_segment(supp, s1, s2);
	step = 12;
      }
    }
  }

  j += 2;
  k += step;

  return t;
}

//--------------------------------------------------------------------------

template<typename Result_t, class Predicate, unsigned int Arity>
struct Svd_predicate_caller;

template<typename Result_t, class Predicate>
struct Svd_predicate_caller<Result_t, Predicate, 2>
{
  template<class S>
  Result_t operator()(const S t[]) const
    {
      return Predicate()(t[0], t[1]);
    }
};

template<typename Result_t, class Predicate>
struct Svd_predicate_caller<Result_t, Predicate, 3>
{
  template<class S>
  Result_t operator()(const S t[]) const
    {
      return Predicate()(t[0], t[1], t[2]);
    }

  template<class S, typename Data>
  Result_t operator()(const S t[], Data data) const
    {
      return Predicate()(t[0], t[1], t[2], data);
    }
};

template<typename Result_t, class Predicate>
struct Svd_predicate_caller<Result_t, Predicate, 4>
{
  template<class S>
  Result_t operator()(const S t[]) const
    {
      return Predicate()(t[0], t[1], t[2], t[3]);
    }

  template<class S, typename Data>
  Result_t operator()(const S t[], Data data) const
    {
      return Predicate()(t[0], t[1], t[2], t[3], data);
    }
};

template<typename Result_t, class Predicate>
struct Svd_predicate_caller<Result_t, Predicate, 5>
{
  template<class S>
  Result_t operator()(const S t[]) const
    {
      return Predicate()(t[0], t[1], t[2], t[3], t[4]);
    }

  template<class S, typename Data>
  Result_t operator()(const S t[], Data data) const
    {
      return Predicate()(t[0], t[1], t[2], t[3], t[4], data);
    }
};


//--------------------------------------------------------------------------

template<template<class Kernel> class Predicate_t,
	 typename Return_t, class FT,
	 unsigned int Num_sites>
Return_t
svd_predicate_ftC2(const std::vector<FT>& v,
		   const std::vector<char>& site_types)
{
   typedef Simple_cartesian<FT>                 Rep;
   typedef CGAL::Segment_Voronoi_diagram_kernel_wrapper_2<Rep>  Kernel;

   typedef typename Kernel::Site_2                   Site_2;
   typedef Predicate_t<Kernel>                       Predicate;

   typedef Svd_predicate_caller<Return_t, Predicate, Num_sites> Caller;

   must_be_filtered(FT());


   Site_2 t[Num_sites];

   for (unsigned int i = 0, k = 0, j = 0; i < Num_sites; i++) {
     t[i] = get_site<Kernel>(v, k, site_types, j);
   }

   Return_t result = Caller()(t);

   return result;
}

template<template<class Kernel, class MTag> class Predicate_t,
	 typename Return_t, class FT,
	 class Method_tag, unsigned int Num_sites>
Return_t
svd_predicate_ftC2(const std::vector<FT>& v,
		   const std::vector<char>& site_types)
{
   typedef Simple_cartesian<FT>                 Rep;
   typedef CGAL::Segment_Voronoi_diagram_kernel_wrapper_2<Rep>  Kernel;

   typedef typename Kernel::Site_2                   Site_2;
   typedef Predicate_t<Kernel,Method_tag>            Predicate;

   typedef Svd_predicate_caller<Return_t, Predicate, Num_sites> Caller;

   must_be_filtered(FT());


   Site_2 t[Num_sites];

   for (unsigned int i = 0, k = 0, j = 0; i < Num_sites; i++) {
     t[i] = get_site<Kernel>(v, k, site_types, j);
   }

   Return_t result = Caller()(t);

   return result;
}

template<template<class Kernel, class MTag> class Predicate_t,
	 typename Return_t, class FT,
	 class Method_tag, typename Data, unsigned int Num_sites>
Return_t
svd_predicate_ftC2(const std::vector<FT>& v,
		   const std::vector<char>& site_types, Data data)
{
   typedef Simple_cartesian<FT>                 Rep;
   typedef CGAL::Segment_Voronoi_diagram_kernel_wrapper_2<Rep>  Kernel;

   typedef typename Kernel::Site_2                   Site_2;
   typedef Predicate_t<Kernel,Method_tag>            Predicate;

   typedef Svd_predicate_caller<Return_t, Predicate, Num_sites> Caller;

   must_be_filtered(FT());


   Site_2 t[Num_sites];

   for (unsigned int i = 0, k = 0, j = 0; i < Num_sites; i++) {
     t[i] = get_site<Kernel>(v, k, site_types, j);
   }

   Return_t result = Caller()(t, data);

   return result;
}

//--------------------------------------------------------------------------

template<template<class Kernel> class Predicate,
	 typename Return_t, class K,
	 unsigned int Num_sites>
Return_t
svd_predicate_C2(const typename K::Site_2 t[])
{
  typedef typename K::FT   FT;

  std::vector<FT> v;
  std::vector<char> site_types;

  for (unsigned int i = 0; i < Num_sites; i++) {
    svd_predicate_push_back_C2<K>(t[i], v, site_types);
  }

  return
    svd_predicate_ftC2<Predicate,Return_t,FT,Num_sites>(v, site_types);
}

template<template<class Kernel, class MTag> class Predicate,
	 typename Return_t, class K,
	 class Method_tag, unsigned int Num_sites>
Return_t
svd_predicate_C2(const typename K::Site_2 t[])
{
  typedef typename K::FT   FT;

  std::vector<FT> v;
  std::vector<char> site_types;

  for (unsigned int i = 0; i < Num_sites; i++) {
    svd_predicate_push_back_C2<K>(t[i], v, site_types);
  }

  return svd_predicate_ftC2<Predicate,Return_t,FT,
    Method_tag,Num_sites>(v, site_types);
}

template<template<class Kernel, class MTag> class Predicate,
	 typename Return_t, class K,
	 class Method_tag, typename Data, unsigned int Num_sites>
Return_t
svd_predicate_C2(const typename K::Site_2 t[], Data data)
{
  typedef typename K::FT   FT;

  std::vector<FT> v;
  std::vector<char> site_types;

  for (unsigned int i = 0; i < Num_sites; i++) {
    svd_predicate_push_back_C2<K>(t[i], v, site_types);
  }

  return svd_predicate_ftC2<Predicate,Return_t,FT,
    Method_tag, Data, Num_sites>(v, site_types, data);
}

//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------


template<class K>
inline
bool svd_are_same_points_C2(const typename K::Site_2& p,
			    const typename K::Site_2& q)
{
  typename K::Site_2 site_vec[] = {p, q};
  return svd_predicate_C2<Svd_are_same_points_C2,bool,K,2>(site_vec);
}

//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------

template<class K, class Method_tag>
inline
Oriented_side
svd_oriented_side_of_bisector_ftC2(const typename K::Site_2& p,
				   const typename K::Site_2& q,
				   const typename K::Site_2& t,
				   Method_tag mtag)
{
  typename K::Site_2 site_vec[] = {p, q, t};
#if 0
  std::cout << "-----------------------------------------"
	    << std::endl;
  std::cout << "inside svd_oriented_side_of_bisector_ftC2"
	    << std::endl;
  std::cout << "p: " << p << std::endl;
  std::cout << "q: " << q << std::endl;
  std::cout << "t: " << t << std::endl;
#endif
  return svd_predicate_C2<Svd_oriented_side_of_bisector_C2,
    Oriented_side,K,Method_tag,3>(site_vec);
}


//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------

template<class K, class Method_tag>
inline
Sign
svd_vertex_conflict_ftC2(const typename K::Site_2& p,
			 const typename K::Site_2& q,
			 const typename K::Site_2& t,
			 Method_tag mtag)
{
  typename K::Site_2 site_vec[] = {p, q, t};
  return
    svd_predicate_C2<Svd_incircle_2,Sign,K,Method_tag,3>(site_vec);
}

//--------------------------------------------------------------------------

template<class K, class Method_tag>
inline
Sign
svd_vertex_conflict_ftC2(const typename K::Site_2& p,
			 const typename K::Site_2& q,
			 const typename K::Site_2& r,
			 const typename K::Site_2& t,
			 Method_tag mtag)
{
  typename K::Site_2 site_vec[] = {p, q, r, t};
  return
    svd_predicate_C2<Svd_incircle_2,Sign,K,Method_tag,4>(site_vec);
}

//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------

template<class K, class Method_tag>
inline
bool
svd_finite_edge_conflict_ftC2(const typename K::Site_2& p,
			      const typename K::Site_2& q,
			      const typename K::Site_2& t,
			      Sign sgn, Method_tag mtag)
{
  typename K::Site_2 site_vec[] = {p, q, t};
  return svd_predicate_C2<Svd_finite_edge_interior_2,bool,K,
    Method_tag,Sign,3>(site_vec, sgn);
}

//--------------------------------------------------------------------------

template<class K, class Method_tag>
inline
bool
svd_finite_edge_conflict_ftC2(const typename K::Site_2& p,
			      const typename K::Site_2& q,
			      const typename K::Site_2& r,
			      const typename K::Site_2& t,
			      Sign sgn, Method_tag mtag)
{
  typename K::Site_2 site_vec[] = {p, q, r, t};
  return svd_predicate_C2<Svd_finite_edge_interior_2,bool,K,
    Method_tag,Sign,4>(site_vec, sgn);
}


//--------------------------------------------------------------------------

template<class K, class Method_tag>
inline
bool
svd_finite_edge_conflict_ftC2(const typename K::Site_2& p,
			      const typename K::Site_2& q,
			      const typename K::Site_2& r,
			      const typename K::Site_2& s,
			      const typename K::Site_2& t,
			      Sign sgn, Method_tag mtag)
{
  typename K::Site_2 site_vec[] = {p, q, r, s, t};
  return svd_predicate_C2<Svd_finite_edge_interior_2,bool,K,
    Method_tag,Sign,5>(site_vec, sgn);
}

//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------

template<class K, class Method_tag>
inline
bool
svd_infinite_edge_conflict_ftC2(const typename K::Site_2& q,
				const typename K::Site_2& r,
				const typename K::Site_2& s,
				const typename K::Site_2& t,
				Sign sgn, Method_tag mtag)
{
  typename K::Site_2 site_vec[] = {q, r, s, t};
  return svd_predicate_C2<Svd_infinite_edge_interior_2,bool,K,
    Method_tag,Sign,4>(site_vec, sgn);
}

//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------

template<class K, class Method_tag>
inline
bool
svd_is_degenerate_edge_ftC2(const typename K::Site_2& p,
			    const typename K::Site_2& q,
			    const typename K::Site_2& r,
			    const typename K::Site_2& t,
			    Method_tag mtag)
{
  typename K::Site_2 site_vec[] = {p, q, r, t};
  return
    svd_predicate_C2<Svd_is_degenerate_edge_2,bool,K,Method_tag,4>(site_vec);
}

//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------

template<class K, class Method_tag>
inline
std::pair<int,int>
svd_do_intersect_C2(const typename K::Site_2& p,
		    const typename K::Site_2& q,
		    Method_tag mtag)
{
  typename K::Site_2 site_vec[2] = {p, q};
  return
    svd_predicate_C2<Svd_do_intersect_C2,std::pair<int,int>,K,2>(site_vec);
}

//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------

template<class K>
inline
bool
svd_are_parallel_C2(const typename K::Site_2& p,
		    const typename K::Site_2& q)
{
  typedef typename K::Segment_2  Segment_2;

  CGAL_precondition( p.is_segment() && q.is_segment() );

  Segment_2 s1 = p.segment();
  Segment_2 s2 = q.segment();

  return svd_are_parallel_ftC2(s1.source().x(),	s1.source().y(),
			       s1.target().x(),	s1.target().y(),
			       s2.source().x(),	s2.source().y(),
			       s2.target().x(),	s2.target().y());
}


template<class FT>
inline
bool
svd_are_parallel_ftC2(const FT& x1, const FT& y1,
		      const FT& x2, const FT& y2,
		      const FT& x3, const FT& y3,
		      const FT& x4, const FT& y4)
{
  must_be_filtered(FT());

  FT det = det2x2_by_formula(x2 - x1, x4 - x3,
			     y2 - y1, y4 - y3);

  return ( CGAL::sign(det) == CGAL::ZERO );
}

//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------

template<class K, class Method_tag>
inline
Oriented_side
svd_oriented_side_ftC2(const typename K::Site_2& s1,
		       const typename K::Site_2& s2,
		       const typename K::Site_2& s3,
		       const typename K::Site_2& s,
		       const typename K::Site_2& p,
		       Method_tag mtag)
{
  typename K::Site_2 site_vec[] = {s1, s2, s3, s, p};
  return svd_predicate_C2<Svd_oriented_side_C2,Oriented_side,K,
    Method_tag,5>(site_vec);
}

//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------


CGAL_END_NAMESPACE


#ifdef CGAL_ARITHMETIC_FILTER_H
#ifndef CGAL_ARITHMETIC_FILTER_SVD_PREDICATES_FTC2_H
#include <CGAL/Arithmetic_filter/predicates/svd_predicates_ftC2.h>
#endif // CGAL_ARITHMETIC_FILTER_SVD_PREDICATES_FTC2_H
#endif

#endif // CGAL_SEGMENT_VORONOI_DIAGRAM_PREDICATES_FTC2_H

