// Copyright (c) 2011 GeometryFactory (France). All rights reserved.
// All rights reserved.
//
// Licensees holding a valid commercial license may use this file in
// accordance with the commercial license agreement provided with the software.
//
// This file is part of CGAL (www.cgal.org); you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License as
// published by the Free Software Foundation; version 2.1 of the License.
// See the file LICENSE.LGPL distributed with CGAL.
//
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
//
// $URL$
// $Id$
// 
//
// Author(s)     : Philipp Möller

#ifndef CGAL_INTERSECTION_TRAITS_H
#define CGAL_INTERSECTION_TRAITS_H

#include <CGAL/Kernel_traits.h>
#include <CGAL/Object.h>

// The macro CGAL_INTERSECTION_VERSION controls which version of the
// intersection is used.
// Currently two values are supported:
// - 1, which means intersections with CGAL::Object
// - 2, which means intersections with Intersection_traits and the 
//      corresponding APIs in other modules
// The default value is 2.

#if !defined(CGAL_INTERSECTION_VERSION)
#define CGAL_INTERSECTION_VERSION 2
#endif

#define CGAL_INTERSECTION_TRAITS_2(A, B, R1, R2, DIMTAG)                \
  template<typename K>     \
  struct Intersection_traits<K, typename K::A, typename K::B>  { \
    typedef typename boost::variant<typename K::R1, typename K::R2 >    \
                     variant_type;                                      \
    typedef typename boost::optional< variant_type > result_type;       \
    typedef internal::DIMTAG Dim_tag;                                   \
  };  

#define CGAL_INTERSECTION_TRAITS_3(A, B, R1, R2, R3, DIMTAG)            \
  template<typename K>     \
  struct Intersection_traits<K, typename K::A, typename K::B>  { \
    typedef typename boost::variant<typename K::R1, typename K::R2,     \
                                    typename K::R3> variant_type;       \
    typedef typename boost::optional< variant_type > result_type;       \
    typedef internal::DIMTAG Dim_tag;                                   \
  };

namespace CGAL {

// only declarationn
template<typename, typename, typename>
struct Intersection_traits {};

// alias
template<typename K, typename A, typename B>
struct IT : public Intersection_traits<K, A, B> {};

// The version to cover ternary intersections of the Spherical_kernel
template<typename K>
struct Intersection_traits_spherical {
  typedef boost::variant< 
    typename K::Circle_3, typename K::Plane_3, typename K::Sphere_3, std::pair< typename K::Circular_arc_point_3, unsigned > >
  result_type;
};

template<typename K>
struct ITs : public Intersection_traits_spherical<K> {};

namespace internal {

  // this function is used to call either make_object or a
  // Intersection_traits::result_type constructor to create return
  // values. The Object version takes some dummy template arguments
  // that are needed for the return of the Intersection_traits. In
  // theory a one parameter variant could be returned, but this
  // _could_ come with conversion overhead and so we rather go for
  // the real type.
  // Overloads for empty returns are also provided.
#if CGAL_INTERSECTION_VERSION < 2
  #if defined(CGAL_CFG_NO_CPP0X_RVALUE_REFERENCE)
    template<typename K, typename A, typename B, typename T>
    inline
    CGAL::Object intersection_return(const T& t) { return CGAL::make_object(t); }
  #else
    template<typename K, typename A, typename B, typename T>
    inline
    CGAL::Object intersection_return(T&& t) { return CGAL::make_object(std::forward<T>(t)); }
  #endif // CGAL_CFG_NO_CPP0X_RVALUE_REFERENCE
  template<typename K, typename A, typename B>
  inline
  CGAL::Object intersection_return() { return CGAL::Object(); }
#else
  #if defined(CGAL_CFG_NO_CPP0X_RVALUE_REFERENCE)
    template<typename K, typename A, typename B, typename T>
    inline
    typename Intersection_traits<K, A, B>::result_type
    intersection_return(const T& t) { return typename CGAL::Intersection_traits<K, A, B>::result_type(t); }
  #else
    template<typename K, typename A, typename B, typename T>
    inline
    typename Intersection_traits<K, A, B>::result_type
    intersection_return(T&& t) { return typename CGAL::Intersection_traits<K, A, B>::result_type(std::forward<T>(t)); }
  #endif // CGAL_CFG_NO_CPP0X_RVALUE_REFERENCE
  template<typename K, typename A, typename B>
  inline
  typename Intersection_traits<K, A, B>::result_type
  intersection_return() { return typename CGAL::Intersection_traits<K, A, B>::result_type(); }
#endif // CGAL_INTERSECTION_VERSION < 2

// tags for dispatch
struct Intersection_dim_two {};
struct Intersection_dim_three {};
struct Intersection_dim_d {};

template<typename A, typename B>
#if CGAL_INTERSECTION_VERSION < 2
CGAL::Object
#else
typename IT< typename CGAL::Kernel_traits<A>::Kernel, A, B>::result_type
#endif
intersection_impl(const A& a, const B& b, Intersection_dim_two) {
  typedef typename CGAL::Kernel_traits<A>::Kernel Kernel;
  return Kernel().intersect_2_object()(a, b);
}

template<typename A, typename B>
#if CGAL_INTERSECTION_VERSION < 2
CGAL::Object
#else
typename IT< typename CGAL::Kernel_traits<A>::Kernel, A, B>::result_type
#endif
intersection_impl(const A& a, const B& b, Intersection_dim_three) {
  typedef typename CGAL::Kernel_traits<A>::Kernel Kernel;
  return Kernel().intersect_3_object()(a, b);
}

template<typename A, typename B>
#if CGAL_INTERSECTION_VERSION < 2
CGAL::Object
#else
typename IT< typename CGAL::Kernel_traits<A>::Kernel, A, B>::result_type
#endif
intersection_impl(const A& a, const B& b, Intersection_dim_d) {
  typedef typename CGAL::Kernel_traits<A>::Kernel Kernel;
  return Kernel().intersect_d_object()(a, b);
}

template<typename A, typename B>
inline bool
do_intersect_impl(const A& a, const B& b, Intersection_dim_two) {
  typedef typename CGAL::Kernel_traits<A>::Kernel Kernel;
  return Kernel().do_intersect_2_object()(a, b);
}

template<typename A, typename B>
inline bool
do_intersect_impl(const A& a, const B& b, Intersection_dim_three) {
  typedef typename CGAL::Kernel_traits<A>::Kernel Kernel;
  return Kernel().do_intersect_3_object()(a, b);
}

template<typename A, typename B>
inline bool
do_intersect_impl(const A& a, const B& b, Intersection_dim_d) {
  typedef typename CGAL::Kernel_traits<A>::Kernel Kernel;
  return Kernel().do_intersect_d_object()(a, b);
}

} // internal

template<typename A, typename B>
inline
#if CGAL_INTERSECTION_VERSION < 2
CGAL::Object
#else
typename IT< typename Kernel_traits<A>::Kernel, A, B>::result_type
#endif
intersection(const A& a, const B& b) {
  typedef typename Kernel_traits<A>::Kernel Kernel;
  typedef IT<Kernel, A, B> Traits;
  return internal::intersection_impl(a, b, typename Traits::Dim_tag());
}

template<typename A, typename B>
inline bool
do_intersect(const A& a, const B& b) {
  typedef typename Kernel_traits<A>::Kernel Kernel;
  typedef IT<Kernel, A, B> Traits;
  return internal::do_intersect_impl(a, b, typename Traits::Dim_tag());
}

} // CGAL

#endif /* CGAL_INTERSECTION_TRAITS_H */
