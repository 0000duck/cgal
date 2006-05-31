// Copyright (c) 2005, 2006 Fernando Luis Cacciola Carballal. All rights reserved.
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
// Author(s)     : Fernando Cacciola <fernando_cacciola@ciudad.com.ar>
//
#ifndef CGAL_SURFACE_MESH_SIMPLIFICATION_TSMS_COMMON_H
#define CGAL_SURFACE_MESH_SIMPLIFICATION_TSMS_COMMON_H 1

#include <functional>
#include <utility>
#include <vector>

#include <boost/shared_ptr.hpp>
#include <boost/optional/optional.hpp>
#include <boost/none.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/format.hpp>

namespace boost
{

struct cgal_tsms_is_vertex_fixed_t {} ;

}

CGAL_BEGIN_NAMESPACE

namespace Triangulated_surface_mesh { namespace Simplification 
{

using boost::num_edges ;
using boost::num_vertices ;
using boost::edges ;
using boost::out_edges ;
using boost::in_edges ;
using boost::source ;
using boost::target ;
using boost::edge_is_border_t ;
using boost::vertex_point_t ;
using boost::cgal_tsms_is_vertex_fixed_t ;

using boost::shared_ptr ;
using boost::optional ;
using boost::none ;
using boost::put_get_helper ;
using boost::get ;
using boost::put ;
using boost::addressof ;

using namespace boost::tuples ;


template<class Handle>
inline bool handle_assigned( Handle h ) { Handle null ; return h != null ; }

template<class Iterator, class Handle>
bool handle_exists ( Iterator begin, Iterator end, Handle h )
{
 if ( handle_assigned(h) )
 {
   while ( begin != end )
     if ( begin++ == h )
       return true ;
 }
 return false ;
}

template<class TSM> 
struct Surface_geometric_traits
{ 
  typedef typename TSM::Point_3 Point_3 ;
  
  typedef typename Kernel_traits<Point_3>::Kernel Kernel ;
  
  typedef typename Kernel::FT FT ;
  
} ;

} } // namespace Triangulated_surface_mesh::Simplification

//
// Valid surface predicate
//
template<class TSM>
inline bool is_valid_triangulated_surface_mesh ( TSM const& aTSM ) { return aTSM.is_pure_triangle() ; }

template<class XYZ>
inline std::string xyz_to_string( XYZ const& xyz )
{
  return boost::str( boost::format("(%1%,%2%,%3%)") % xyz.x() % xyz.y() % xyz.z() ) ;   
}

template<class T>
inline std::string optional_to_string( boost::optional<T> const& o )
{
  if ( o )
       return boost::str( boost::format("%1%") % *o ) ;   
  else return std::string("NONE");  
}

CGAL_END_NAMESPACE

#if   defined(CGAL_SURFACE_SIMPLIFICATION_ENABLE_TRACE)    \
   || defined(CGAL_SURFACE_SIMPLIFICATION_ENABLE_LT_TRACE) 
#define CGAL_TSMS_ENABLE_TRACE
#endif

#ifdef CGAL_TSMS_ENABLE_TRACE

#  include<string>
#  include<iostream>
#  include<sstream>
#  define CGAL_TSMS_TRACE_IMPL(m) \
     { \
       std::ostringstream ss ; ss << m ; std::string s = ss.str(); \
       Surface_simplification_external_trace(s); \
     }
#endif

#ifdef CGAL_SURFACE_SIMPLIFICATION_ENABLE_LT_TRACE
#  define CGAL_TSMS_LT_TRACE(l,m) if ( l <= CGAL_SURFACE_SIMPLIFICATION_ENABLE_LT_TRACE ) CGAL_TSMS_TRACE_IMPL(m)
#else
#  define CGAL_TSMS_LT_TRACE(l,m)
#endif

#ifdef CGAL_SURFACE_SIMPLIFICATION_ENABLE_TRACE
#  define CGAL_TSMS_TRACE(l,m) if ( l <= CGAL_SURFACE_SIMPLIFICATION_ENABLE_TRACE ) CGAL_TSMS_TRACE_IMPL(m)
#else
#  define CGAL_TSMS_TRACE(l,m)
#endif




#if defined(CGAL_SURFACE_SIMPLIFICATION_ENABLE_AUDIT) 
#define CGAL_TSMS_ENABLE_AUDIT
#endif

#ifdef CGAL_TSMS_ENABLE_AUDIT

#  include<string>
#  include<iostream>
#  include<sstream>
#  define CGAL_TSMS_AUDIT_IMPL(m) \
     { \
       std::ostringstream ss ; ss << m ; std::string s = ss.str(); \
       Surface_simplification_external_audit(s); \
     }
#endif

#ifdef CGAL_SURFACE_SIMPLIFICATION_ENABLE_AUDIT
#  define CGAL_TSMS_AUDIT(m) CGAL_TSMS_AUDIT_IMPL(m)
#else
#  define CGAL_TSMS_AUDIT(m)
#endif


#undef CGAL_TSMS_ENABLE_TRACE
#undef CGAL_TSMS_ENABLE_AUDIT

#endif // CGAL_SURFACE_MESH_SIMPLIFICATION_TSMS_COMMON_H //
// EOF //
 
