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
// release       : $CGAL_Revision: CGAL-2.4-I-11 $
// release_date  : $CGAL_Date: 2001/09/21 $
//
// file          : include/CGAL/config.h
// package       : Configuration (2.14)
// maintainer    : Geert-Jan Giezeman <geert@cs.uu.nl>
// source        :
// revision      : 1.11
// revision_date : 30 Mar 1998
// author(s)     : Wieger Wesselink <wieger@cs.ruu.nl>
//                 Michael Hoffmann
//
// coordinator   : Utrecht University
//
// ======================================================================

#ifndef CGAL_CONFIG_H
#define CGAL_CONFIG_H

#define CGAL_VERSION 2.4-I-11
#define CGAL_VERSION_NR 1002004011

#define CGAL_CFG_NO_ADVANCED_KERNEL 1

//----------------------------------------------------------------------//
//             STLport fix for MSVC
//----------------------------------------------------------------------//


#ifdef _MSC_VER
#   define CGAL_SCOPE
#   define CGAL_LIMITED_ITERATOR_TRAITS_SUPPORT 1
#   include <stl_config.h>
#   include <stl_iterator_base.h>
#else  // not _MSC_VER
#   define CGAL_SCOPE CGAL::
#   define CGAL_DEFINE_ITERATOR_TRAITS_POINTER_SPEC(a)
#endif // _MSC_VER


//----------------------------------------------------------------------//
//             include platform specific workaround flags (CGAL_CFG_...)
//----------------------------------------------------------------------//

#include <CGAL/compiler_config.h>

//----------------------------------------------------------------------//
//             do some post processing for the flags
//----------------------------------------------------------------------//


#ifdef CGAL_CFG_TYPENAME_BUG
#   define CGAL_TYPENAME_MSVC_NULL
#else
#   define CGAL_TYPENAME_MSVC_NULL typename
#endif

#ifdef CGAL_CFG_NO_NAMESPACE
#  define CGAL_USING_NAMESPACE_STD
#  define CGAL_STD
#  define CGAL std
#else
#  define CGAL_USING_NAMESPACE_STD using namespace std;
#  define CGAL_STD std
#  ifndef CGAL_USE_NAMESPACE
#    define CGAL_USE_NAMESPACE 1
#  endif
#endif

#if CGAL_USE_NAMESPACE
#  define CGAL_BEGIN_NAMESPACE namespace CGAL {
#  define CGAL_END_NAMESPACE }
#else
#  define CGAL_BEGIN_NAMESPACE
#  define CGAL_END_NAMESPACE
#endif

#ifdef CGAL_CFG_VC7_PRIVATE_TYPE_BUG
#  define CGAL_VC7_BUG_PROTECTED protected:
#else
#  define CGAL_VC7_BUG_PROTECTED
#endif

#ifdef CGAL_CFG_NO_TEMPLATE_FRIEND_DISTINCTION
#  define CGAL_NULL_TMPL_ARGS
#else
#  define CGAL_NULL_TMPL_ARGS <>
#endif

#ifdef CGAL_CFG_NO_EXPLICIT_CLASS_TEMPLATE_SPECIALISATION
#  define CGAL_TEMPLATE_NULL
#else
#  define CGAL_TEMPLATE_NULL template <>
#endif

#ifdef CGAL_CFG_NO_STDC_NAMESPACE
#define CGAL_CLIB_STD
#else
#define CGAL_CLIB_STD std
#endif

//----------------------------------------------------------------------//
//             include separate workaround files
//----------------------------------------------------------------------//

#ifdef _MSC_VER
#  include <CGAL/MSVC_standard_header_fixes.h>
#endif
#if defined(__BORLANDC__) && __BORLANDC__ > 0x520
#include <CGAL/Borland_fixes.h>
#endif

//----------------------------------------------------------------------//
//             select old or new style headers
//----------------------------------------------------------------------//

#ifndef CGAL_USE_NEWSTYLE_HEADERS
#  ifndef CGAL_CFG_NO_STANDARD_HEADERS
#    ifndef CGAL_NO_NEWSTYLE_HEADERS
#      define CGAL_USE_NEWSTYLE_HEADERS
#    endif // ! CGAL_NO_NEWSTYLE_HEADERS
#  endif // ! CGAL_CFG_NO_STANDARD_HEADERS
#endif // ! CGAL_USE_NEWSTYLE_HEADERS

//----------------------------------------------------------------------//
//             fake iterator_traits for sunpro
//----------------------------------------------------------------------//

#if defined(__sun) && defined(__SUNPRO_CC)
#include <iterator>
namespace std {
  template <class Iterator> struct iterator_traits
  {
    typedef typename Iterator::value_type value_type;
    typedef typename Iterator::difference_type difference_type;
    typedef typename Iterator::pointer pointer;
    typedef typename Iterator::reference reference;
    typedef typename Iterator::iterator_category iterator_category;
  };
  template <class T> struct iterator_traits<T*>
  {
    typedef T value_type;
    typedef ptrdiff_t difference_type;
    typedef T* pointer;
    typedef T& reference;
    typedef random_access_iterator_tag iterator_category;
  };
  template <class T> struct iterator_traits<const T*>
  {
    typedef T value_type;
    typedef ptrdiff_t difference_type;
    typedef const T* pointer;
    typedef const T& reference;
    typedef random_access_iterator_tag iterator_category;
  };
  template <class ForwardIterator>
  inline ptrdiff_t
  distance (ForwardIterator first, ForwardIterator last)
  {
    ptrdiff_t n = 0;
    __distance(first, last, n, 
               iterator_traits<ForwardIterator>::iterator_category());
    return n;
  }

  template < class T >
  inline typename T::value_type*
  __value_type (const T&)
  { return (typename T::value_type*)(0); }

  template < class T >
  inline typename T::difference_type*
  __distance_type(const T&)
  { return (typename T::difference_type*)(0); }

  template < class T >
  inline typename T::iterator_category
  __iterator_category (const T&)
  {
    typename T::iterator_category tmp;
    return tmp;
  }
} // namespace std
#endif // if defined(__sun) && defined(__SUNPRO_CC)

#endif // CGAL_CONFIG_H

