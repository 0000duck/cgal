// Copyright (c) 2006-2008 Fernando Luis Cacciola Carballal. All rights reserved.
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
#ifndef CGAL_CREATE_OFFSET_POLYGONS_2_H
#define CGAL_CREATE_OFFSET_POLYGONS_2_H

#include <CGAL/Create_straight_skeleton_2.h>
#include <CGAL/Polygon_offset_builder_2.h>
#include <CGAL/Straight_skeleton_converter_2.h>
#include <CGAL/compute_outer_frame_margin.h>
#include <CGAL/Polygon_2.h>

CGAL_BEGIN_NAMESPACE

namespace CGAL_SS_i
{

template<class U, class V> struct Is_same_type { typedef Tag_false type ; } ;

template<class U> struct Is_same_type<U,U> { typedef Tag_true type ; } ;


//
// Kernel != Skeleton::kernel. The skeleton is converted to Straight_skeleton_2<Kernel>
//
template<class OutPolygon, class FT, class Skeleton, class K>
std::vector< boost::shared_ptr<OutPolygon> > 
create_offset_polygons_2 ( FT aOffset, Skeleton const& aSs, K const& k, Tag_false )
{
  typedef boost::shared_ptr<OutPolygon> OutPolygonPtr ; 
  typedef std::vector<OutPolygonPtr>    OutPolygonPtrVector ;
   
  typedef Straight_skeleton_2<K> OfSkeleton ;
   
  typedef Polygon_offset_builder_traits_2<K>                                  OffsetBuilderTraits;
  typedef Polygon_offset_builder_2<OfSkeleton,OffsetBuilderTraits,OutPolygon> OffsetBuilder;
  
  OutPolygonPtrVector rR ;
  
  boost::shared_ptr<OfSkeleton> lConvertedSs = convert_straight_skeleton_2<OfSkeleton>(aSs);
  OffsetBuilder ob( *lConvertedSs );
  ob.construct_offset_contours(aOffset, std::back_inserter(rR) ) ;
    
  return rR ;
}

//
// Kernel == Skeleton::kernel, no convertion
//
template<class OutPolygon, class FT, class Skeleton, class K>
std::vector< boost::shared_ptr<OutPolygon> > 
create_offset_polygons_2 ( FT aOffset, Skeleton const& aSs, K const& k, Tag_true )
{
  typedef boost::shared_ptr<OutPolygon> OutPolygonPtr ; 
  typedef std::vector<OutPolygonPtr>    OutPolygonPtrVector ;
   
  typedef Polygon_offset_builder_traits_2<K>                                OffsetBuilderTraits;
  typedef Polygon_offset_builder_2<Skeleton,OffsetBuilderTraits,OutPolygon> OffsetBuilder;
  
  OutPolygonPtrVector rR ;
  
  OffsetBuilder ob(aSs);
  ob.construct_offset_contours(aOffset, std::back_inserter(rR) ) ;
    
  return rR ;
}

// Allow failure due to invalid straight skeletons to go through the users
template<class Skeleton>
Skeleton const& dereference ( boost::shared_ptr<Skeleton> const& ss )
{
  if ( !ss )
    CGAL_error_msg("Failed to construct straight skeleton  for polygon offseting");
    
  return *ss;
}

}

template<class Polygon, class FT, class Skeleton, class K>
std::vector< boost::shared_ptr<Polygon> > 
inline
create_offset_polygons_2 ( FT aOffset, Skeleton const& aSs, K const& k )
{
  typedef typename Skeleton::Traits SsKernel ;
  
  typename CGAL_SS_i::Is_same_type<K,SsKernel>::type same_kernel ;
  
  return CGAL_SS_i::create_offset_polygons_2<Polygon>(aOffset,aSs,k,same_kernel);
}

template<class Polygon, class FT, class Skeleton>
std::vector< boost::shared_ptr<Polygon> > 
inline
create_offset_polygons_2 ( FT aOffset, Skeleton const& aSs )
{
  return create_offset_polygons_2(aOffset, aSs, typename Polygon::Traits() ) ;
}

template<class FT, class Skeleton>
std::vector< boost::shared_ptr< Polygon_2< typename Skeleton::Traits::Kernel > > > 
inline
create_offset_polygons_2 ( FT aOffset, Skeleton const& aSs )
{
  typedef typename Skeleton::Traits::Kernel K ;
  
  typedef Polygon_2<K> Polygon ;
  
  return create_offset_polygons_2<Polygon>(aOffset, aSs, K() ) ;
}

template<class FT, class Polygon, class HoleIterator, class OfK, class SsK>
std::vector< boost::shared_ptr<Polygon> >
inline
create_interior_skeleton_and_offset_polygons_2 ( FT             aOffset
                                               , Polygon const& aOuterBoundary
                                               , HoleIterator   aHolesBegin
                                               , HoleIterator   aHolesEnd
                                               , OfK const&     ofk
                                               , SsK const&     ssk
                                               )
{
  return create_offset_polygons_2<Polygon>(aOffset
                                          ,CGAL_SS_i::dereference( create_interior_straight_skeleton_2(CGAL_SS_i::vertices_begin(aOuterBoundary)
                                                                                                      ,CGAL_SS_i::vertices_end  (aOuterBoundary)
                                                                                                      ,aHolesBegin
                                                                                                      ,aHolesEnd
                                                                                                      ,ssk
                                                                                                      ) 
                                                                 )
                                          ,ofk
                                          );
    
}

template<class FT, class Polygon, class HoleIterator, class OfK>
std::vector< boost::shared_ptr<Polygon> >
inline
create_interior_skeleton_and_offset_polygons_2 ( FT             aOffset
                                               , Polygon const& aOuterBoundary
                                               , HoleIterator   aHolesBegin
                                               , HoleIterator   aHolesEnd
                                               , OfK const&     ofk
                                               )
{
  return create_interior_skeleton_and_offset_polygons_2(aOffset
                                                       ,aOuterBoundary
                                                       ,aHolesBegin
                                                       ,aHolesEnd
                                                       ,ofk
                                                       ,Exact_predicates_inexact_constructions_kernel()
                                                       );
}

template<class FT, class Polygon, class OfK, class SsK>
std::vector< boost::shared_ptr<Polygon> >
inline
create_interior_skeleton_and_offset_polygons_2 ( FT aOffset, Polygon const& aPoly, OfK const& ofk, SsK const& ssk )
{
  std::vector<Polygon> no_holes ;
  return create_interior_skeleton_and_offset_polygons_2(aOffset
                                                       ,aPoly
                                                       ,no_holes.begin()
                                                       ,no_holes.end()
                                                       ,ofk
                                                       ,ssk
                                                       );
}

template<class FT, class Polygon, class OfK>
std::vector< boost::shared_ptr<Polygon> >
inline
create_interior_skeleton_and_offset_polygons_2 ( FT aOffset, Polygon const& aPoly, OfK const& ofk )
{
  std::vector<Polygon> no_holes ;
  return create_interior_skeleton_and_offset_polygons_2(aOffset
                                                       ,aPoly
                                                       ,no_holes.begin()
                                                       ,no_holes.end()
                                                       ,ofk
                                                       ,Exact_predicates_inexact_constructions_kernel()
                                                       );
}

template<class FT, class Polygon>
std::vector< boost::shared_ptr<Polygon> >
inline
create_interior_skeleton_and_offset_polygons_2 ( FT aOffset, Polygon const& aPoly )
{
  return create_interior_skeleton_and_offset_polygons_2(aOffset, aPoly, typename Polygon::Traits() );
}

template<class FT, class Polygon, class OfK, class SsK>
std::vector< boost::shared_ptr<Polygon> >
inline
create_exterior_skeleton_and_offset_polygons_2 ( FT aOffset, Polygon const& aPoly, OfK const& ofk , SsK const&  ssk)
{
  return create_offset_polygons_2<Polygon>(aOffset
                                          ,CGAL_SS_i::dereference(create_exterior_straight_skeleton_2(aOffset
                                                                                                     ,CGAL_SS_i::vertices_begin(aPoly)
                                                                                                     ,CGAL_SS_i::vertices_end  (aPoly)
                                                                                                     ,ssk
                                                                                                    )
                                                                  )
                                          ,ofk                                    
                                          );
}

template<class FT, class Polygon, class OfK>
std::vector< boost::shared_ptr<Polygon> >
inline
create_exterior_skeleton_and_offset_polygons_2 ( FT aOffset, Polygon const& aPoly, OfK const& ofk )
{
  return create_exterior_skeleton_and_offset_polygons_2(aOffset
                                                       ,aPoly
                                                       ,ofk
                                                       ,Exact_predicates_inexact_constructions_kernel()
                                                       );
                                               
}

template<class FT, class Polygon>
std::vector< boost::shared_ptr<Polygon> >
inline
create_exterior_skeleton_and_offset_polygons_2 ( FT aOffset, Polygon const& aOuterBoundary )
{
  return create_exterior_skeleton_and_offset_polygons_2(aOffset, aOuterBoundary, typename Polygon::Traits() );
                                               
}

CGAL_END_NAMESPACE


#endif
// EOF //
