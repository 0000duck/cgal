// Copyright (c) 2005 Rijksuniversiteit Groningen (Netherlands)
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
// $URL$
// $Id$
// 
//
// Author(s)     : Nico Kruithof <Nico@cs.rug.nl>

#ifndef CGAL_MIXED_COMPLEX_TRAITS_3_H
#define CGAL_MIXED_COMPLEX_TRAITS_3_H

#include <CGAL/Regular_triangulation_euclidean_traits_3.h>
#include <CGAL/predicates/predicates_for_mixed_complex_3.h>

CGAL_BEGIN_NAMESPACE 

/** Input: a list of n weighted points p_1...p_n and a query point x.
    There is a plane separating the mixed cell defined by p_1...p_n-1
    and the mixed cell defined by p_1...p_n. The predicate tests
    whether x lies on the same side of this plane as the mixed cell
    defined by p_1...p_n-1 (NEGATIVE), on the plane (ZERO) or on the
    opposite side (POSITIVE).
 **/
template <class K>
class Side_of_mixed_cell_3 {
public:
  typedef typename K::FT             FT;
  typedef typename K::Bare_point     Bare_point;
  typedef typename K::Weighted_point Weighted_point;

  Side_of_mixed_cell_3(const FT &shrink) : s(shrink) {}
  
  typedef CGAL::Arity_tag< 5 > Arity;
  typedef CGAL::Sign           result_type;
  
  result_type operator()(const Weighted_point &p1,
			 const Weighted_point &p2,
			 const Bare_point &x) const {
    return side_of_mixed_cellC3(p1.x(),p1.y(),p1.z(),p1.weight(),
				p2.x(),p2.y(),p2.z(),p2.weight(),
				x.x(),x.y(),x.z(),
				s);
  }
  result_type operator()(const Weighted_point &p1,
			 const Weighted_point &p2,
			 const Weighted_point &p3,
			 const Bare_point &x) const {
    return side_of_mixed_cellC3(p1.x(),p1.y(),p1.z(),p1.weight(),
				p2.x(),p2.y(),p2.z(),p2.weight(),
				p3.x(),p3.y(),p3.z(),p3.weight(),
				x.x(),x.y(),x.z(),
				s);
  }
  result_type operator()(const Weighted_point &p1,
			 const Weighted_point &p2,
			 const Weighted_point &p3,
			 const Weighted_point &p4,
			 const Bare_point &x) const {
    return side_of_mixed_cellC3(p1.x(),p1.y(),p1.z(),p1.weight(),
				p2.x(),p2.y(),p2.z(),p2.weight(),
				p3.x(),p3.y(),p3.z(),p3.weight(),
				p4.x(),p4.y(),p4.z(),p4.weight(),
				x.x(),x.y(),x.z(),
				s);
  }
  
private:
  FT s;
};

/** Input: Two weighted points
    Computes the anchor point of a Delaunay center and a Voronoi center
 **/
template <class K>
class Construct_anchor_point_3 {
public:
  typedef typename K::FT             FT;
  typedef typename K::Bare_point     Bare_point;

  Construct_anchor_point_3(const FT &shrink) : s(shrink) {}
  
  typedef CGAL::Arity_tag< 2 > Arity;
  typedef Bare_point           result_type;
  
  result_type operator()(const Bare_point &p_del,
			 const Bare_point &p_vor) const {
    FT t = 1-s;
    return Bare_point(s*p_del.x() + t*p_vor.x(),
		      s*p_del.y() + t*p_vor.y(),
		      s*p_del.z() + t*p_vor.z());
  }
  
private:
  FT s;
};

template <class K_>
class Mixed_complex_traits_base_3 
  : public Regular_triangulation_euclidean_traits_3<K_>
{
public:
  typedef K_                                  K;
  typedef Mixed_complex_traits_base_3<K>      Self;

  typedef typename K::FT                      FT;
  typedef typename K::Point_3                 Bare_point;
  typedef Weighted_point<Bare_point,FT>       Weighted_point;
  typedef Weighted_point                      Weighted_point_3;

  typedef Side_of_mixed_cell_3<Self>          Side_of_mixed_cell_3;
  typedef Construct_weighted_circumcenter_3<Self> 
                                              Construct_weighted_circumcenter_3;
  typedef Construct_anchor_point_3<Self>      Construct_anchor_point_3;
  
 
  
  Mixed_complex_traits_base_3() : s(-1) {
  }
  Mixed_complex_traits_base_3(FT s) : s(s) {
  }
  void set_shrink(FT s_) { 
    s = s_; 
  }
  FT get_shrink() const {
    return s;
  }

  
  Side_of_mixed_cell_3 
  side_of_mixed_cell_3_object() const { 
    CGAL_assertion((s>0) && (s<1));
    return Side_of_mixed_cell_3(get_shrink()); }

//   Construct_weighted_circumcenter_3
//   construct_weighted_circumcenter_3_object() const
//   { return Construct_weighted_circumcenter_3(); }

  Construct_anchor_point_3
  construct_anchor_point_3_object() const
  { return Construct_anchor_point_3(get_shrink()); }

private:
  FT s;
};

// We need to introduce a "traits_base_3" class in order to get the
// specialization for Exact_predicates_inexact_constructions_kernel to work,
// otherwise there is a cycle in the derivation.
// Similar to Regular_triangulation_euclidean_traits_3
template < class K >
class Mixed_complex_traits_3
  : public Mixed_complex_traits_base_3<K>
{
  typedef Mixed_complex_traits_base_3<K> Base;
public:
  Mixed_complex_traits_3() {}
  Mixed_complex_traits_3(typename Base::FT s) : Base(s) {}
};

CGAL_END_NAMESPACE

// Partial specialization for Exact_predicates_inexact_constructions_kernel.
#include <CGAL/Mixed_complex_filtered_traits_3.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>


CGAL_BEGIN_NAMESPACE

template <>
class Mixed_complex_traits_3
          <Exact_predicates_inexact_constructions_kernel>
  : public Mixed_complex_filtered_traits_3
          <Exact_predicates_inexact_constructions_kernel>
{
  typedef Mixed_complex_filtered_traits_3
    <Exact_predicates_inexact_constructions_kernel> Base;
public:
  Mixed_complex_traits_3() {}
  Mixed_complex_traits_3(Base::FT s) : Base(s) {}
};


CGAL_END_NAMESPACE
#endif // CGAL_MIXED_COMPLEX_TRAITS_3_H
