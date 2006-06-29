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
#ifndef CGAL_SURFACE_MESH_SIMPLIFICATION__VERTEX_PAIR_COLLAPSE_H
#define CGAL_SURFACE_MESH_SIMPLIFICATION__VERTEX_PAIR_COLLAPSE_H 1

#include <vector>

#include <boost/config.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/iterator_adaptors.hpp>
#include <boost/graph/adjacency_list.hpp>

#include <CGAL/Unique_hash_map.h>
#include <CGAL/Handle_hash_function.h>

#include <CGAL/Surface_mesh_simplification/TSMS_common.h>
#include <CGAL/Surface_mesh_simplification/Collapse_operator.h>
#include <CGAL/Surface_mesh_simplification/Polyhedron_is_vertex_fixed_map.h>
#include <CGAL/Modifiable_priority_queue.h>

CGAL_BEGIN_NAMESPACE

namespace Triangulated_surface_mesh { namespace Simplification 
{

//
// Implementation of the vertex-pair collapse triangulated surface mesh simplification algorithm
//
template<class TSM_
        ,class GetCollapseData_
        ,class GetCost_
        ,class GetNewVertexPoint_
        ,class ShouldStop_
        ,class VisitorT_
        >
class VertexPairCollapse
{
public:

  typedef TSM_               TSM ;
  typedef GetCollapseData_   GetCollapseData ;
  typedef GetCost_           GetCost ;
  typedef GetNewVertexPoint_ GetNewVertexPoint ;
  typedef ShouldStop_        ShouldStop ;
  typedef VisitorT_          VisitorT ;
  
  typedef VertexPairCollapse Self ;
  
  typedef boost::graph_traits           <TSM>       GraphTraits ;
  typedef boost::undirected_graph_traits<TSM>       UndirectedGraphTraits ;
  typedef boost::graph_traits           <TSM const> ConstGraphTraits ;
  
  typedef typename GraphTraits::vertex_descriptor  vertex_descriptor ;
  typedef typename GraphTraits::vertex_iterator    vertex_iterator ;
  typedef typename GraphTraits::edge_descriptor    edge_descriptor ;
  typedef typename GraphTraits::edge_iterator      edge_iterator ;
  typedef typename GraphTraits::out_edge_iterator  out_edge_iterator ;
  typedef typename GraphTraits::in_edge_iterator   in_edge_iterator ;
  typedef typename GraphTraits::traversal_category traversal_category ;
  typedef typename GraphTraits::edges_size_type    size_type ;
  
  typedef typename ConstGraphTraits::vertex_descriptor const_vertex_descriptor ;
  
  typedef typename UndirectedGraphTraits::edge_iterator undirected_edge_iterator ;

  typedef typename GetCollapseData::Params        ParamsToGetCollapseData ;
  
  typedef typename GetCollapseData::Collapse_data Collapse_data ;
  typedef typename GetCollapseData::result_type   Collapse_data_ptr ; 
  
  typedef typename GetCost          ::result_type Optional_cost_type ;
  typedef typename GetNewVertexPoint::result_type Optional_vertex_point_type ;
     
  typedef Surface_geometric_traits<TSM> Traits ;
  
  typedef typename Traits::Point_3 Point_3 ;

  typedef typename Kernel_traits<Point_3>::Kernel Kernel ;
  
  typedef typename Kernel::Equal_3 Equal_3 ;
  
  class vertex_pair ;
    
  typedef shared_ptr<vertex_pair> vertex_pair_ptr ;
  
  typedef Modifiable_priority_queue<vertex_pair_ptr> PQ ;
  
  typedef typename PQ::iterator pq_handle ;
  
  // The algoritm is centered around vertex-pairs, encapsulated in this type.
  // For each pair there is a cost value provided by the external GetCost functor.
  // Vertex-pairs are stored in a priority queue based on their cost.
  // 
  // Each edge in the TSM contributes one vertex_pair.
  // If the user chooses to collapse non-edge pairs too, some vertex-pairs won't be edges.
  //
  // The algortithm needs to update the costs of certain vertex-pairs (for those edges affected by a collapse).
  // For that reason, the vertex-pairs are kept in a relaxed_queue data structure that supports the update operation.
  // This relaxed_queue data structure needs an index property map. For that reason, an integer ID is stored in each
  // pair.
  //
  class vertex_pair
  {
  public :
  
    vertex_pair( size_type aID, Collapse_data_ptr const& aData, GetCost const& aGet_cost )
       : 
       mID(aID)
     , mData(aData)
     , Get_cost(addressof(aGet_cost))
     , mCostStored(false)
     , mMark(0)
    {}
    
    size_type id() const { return mID ; } 

    // The cost of collapsing a vertex-pair is cached in this record.
    // When calling cost() for the first time, the cached cost is taken from the external GetCost functor.
    // Subsequent calls to cost() returns the same cached cost, that is, GetCost() IS NOT called each time.
    //
    // Such a cache cost is an optional<> value. This is because the GetCost can return "none" for too high or incomputable costs.
    // Therefore, even if the cost is cached, it might be "absent" (that is, == boost::none)
    //
    // OTOH, the algorithm can invalidate the cached cost of any given pair. 
    // If invalidate_cost() is called, the next call to cost() will take it again from the external GetCost.
    // NOTE: Whether the cost is cached or not is independent from whether it is absent or none.
    // The former is controlled by the algorithm by explicitely calling InvalidateCost() while the later is defined by the GetCost
    // which can retiurn boost::none for incomputable or logically invalid costs.
    //
    Optional_cost_type cost() const { return UpdateCost() ; }
    
    void reset_data( Collapse_data_ptr const& aData ) { mData = aData ; mCostStored = false ; }
    
    void reset_data( vertex_descriptor const& new_p
                   , vertex_descriptor const& new_q
                   , bool                     is_new_p_fixed
                   , bool                     is_new_q_fixed
                   , edge_descriptor   const& edge
                   )
    {
      reset_data( Collapse_data_ptr( new Collapse_data(new_p,new_q,is_new_p_fixed,is_new_q_fixed,edge,surface()) ) );
    }
    
    Collapse_data_ptr        data      () const { return mData ; }
    vertex_descriptor const& p         () const { return mData->p() ; }
    vertex_descriptor const& q         () const { return mData->q() ; }
    bool                     is_p_fixed() const { return mData->is_p_fixed() ; }
    bool                     is_q_fixed() const { return mData->is_q_fixed() ; }
    edge_descriptor   const& edge      () const { return mData->edge() ; }
    TSM&                     surface   () const { return mData->surface() ; }
    
    bool is_edge_fixed() const { return is_p_fixed() && is_q_fixed() ; }
    
    bool is_edge() const
    {
      edge_descriptor null ;
      return edge() != null ;
    }

    int  mark() const { return mMark ; }
    int& mark()       { return mMark ; }

    pq_handle PQ_handle() const { return mPQHandle ;}
    
    bool is_in_PQ() const { return mPQHandle != null_PQ_handle() ; }
    
    void set_PQ_handle( pq_handle h ) { mPQHandle = h ; }
    
    void reset_PQ_handle() { mPQHandle = null_PQ_handle() ; }
    
        
    friend bool operator< ( shared_ptr<vertex_pair> const& a, shared_ptr<vertex_pair> const& b ) 
    {
      // NOTE: cost() is an optional<> value.
      // Absent optionals are ordered first; that is, "none < T" and "T > none" for any defined T != none.
      // In consequence, vertex-pairs with undefined costs will be promoted to the top of the priority queue and poped out first.
      return a->cost() < b->cost() ;
    }
    friend bool operator== ( shared_ptr<vertex_pair> const& a, shared_ptr<vertex_pair> const& b ) 
    {
      return a->cost() == b->cost() ;
    }

#ifdef CGAL_SURFACE_SIMPLIFICATION_ENABLE_TRACE

    bool is_p_in_surface   () const { return handle_exists(surface().vertices_begin (),surface().vertices_end(),p()) ; }
    bool is_q_in_surface   () const { return handle_exists(surface().vertices_begin (),surface().vertices_end(),q()) ; }
    bool is_edge_in_surface() const { return handle_exists(surface().halfedges_begin(),surface().halfedges_end(),edge()) ; }
         
    bool is_valid() const { return is_p_in_surface() && is_q_in_surface() && is_edge_in_surface() ; }
    
    friend std::ostream& operator<< ( std::ostream& out, vertex_pair const& vp ) 
    {
      out << "VP" << vp.mID << " {" ;
      if ( vp.is_p_in_surface() )
           out << "V" << vp.p()->ID << " (" << vp.p()->point().x() << "," << vp.p()->point().y() << "," << vp.p()->point().z() << ")" ;
      else out << "##p() has been erased## " ;
      out << ( vp.is_p_fixed() ? "[FIXED] " : "" ) << "->" ;
      if ( vp.is_q_in_surface() )
           out << " V" << vp.q()->ID << "(" << vp.q()->point().x() << "," << vp.q()->point().y() << "," << vp.q()->point().z() << ")" ;
      else out << "##q() has been erased## " ;
      out << ( vp.is_q_fixed() ? "[FIXED] " : "" );
      if ( vp.is_edge_in_surface() )
           out << " E" << vp.edge()->ID ;
      else out << "##e() has been erased## " ;
          
      if ( vp.mCostStored )
      {
        if ( vp.mCost )
             out << " [" << *vp.mCost << "]" ;
        else out << " [<none>]" ;
      }
      else out << " [<not cached>]" ;
      out << "}" ;
      return out ;
    }
#endif
    
  private:
    
    // Caches the cost if not currently cached, and returns it.
    Optional_cost_type UpdateCost() const
    { 
      if ( !mCostStored )
      {
        mCost = (*Get_cost)(*mData);
        mCostStored = true ;
      }  
      return mCost ;  
    }
    
    static pq_handle null_PQ_handle() { pq_handle h ; return h ; }
    
  private:  
    
    size_type         mID ;
    Collapse_data_ptr mData ;
    GetCost const*    Get_cost ;
    
    mutable bool               mCostStored ;
    mutable Optional_cost_type mCost ;
    
    pq_handle mPQHandle ;
    int       mMark ;
    
  } ;
  
  // Mapping from edges to vertex-pairs (as indexed into the Pairs_vector sequence)
  typedef Unique_hash_map<edge_descriptor,vertex_pair_ptr,Handle_hash_function> edges_2_pairs_map ;

  typedef std::vector<vertex_pair_ptr> vertex_pair_vector ;
  
  typedef typename vertex_pair_vector::iterator vertex_pair_vector_iterator ;
    
public:

  VertexPairCollapse( TSM&                           aSurface
                    , GetCollapseData const&         aGetCollapseData
                    , ParamsToGetCollapseData const* aGetCollapseDataParams // Can be NULL
                    , GetCost const&                 aGetCost
                    , GetNewVertexPoint const&       aGetVertexPoint
                    , ShouldStop const&              aShouldStop 
                    , VisitorT*                      aVisitor
                    , bool                           aIncludeNonEdgePairs 
                    ) ;
  
  int run() ;
  
private:
  
  void Collect();
  void Loop();
  bool Is_collapsable( vertex_descriptor const& p, vertex_descriptor const& q, edge_descriptor const& p_q ) ;
  void Collapse( vertex_pair_ptr const& aPair ) ;
  void Update_neighbors( vertex_pair_ptr const& aCollapsingPair ) ;
  
  vertex_pair_ptr get_pair ( edge_descriptor const& e ) 
  {
    CGAL_precondition( mEdgesToPairsMap.is_defined(e) ) ;
    return mEdgesToPairsMap[e] ;  
  }
  
  void set_pair ( edge_descriptor const& e, vertex_pair_ptr aPair )
  {
    edge_descriptor o = opposite_edge(e,mSurface) ;
    
    mEdgesToPairsMap[e] = aPair ;  
    mEdgesToPairsMap[o] = aPair ;  

    CGAL_postcondition( get_pair(e)->id() == aPair->id() ) ;
    CGAL_postcondition( get_pair(o)->id() == aPair->id() ) ;
  }
  
  void insert_in_PQ( vertex_pair_ptr const& aPair ) 
  {
    CGAL_precondition(aPair);
    CGAL_precondition(!aPair->is_in_PQ());
    pq_handle h = mPQ.push(aPair); 
    aPair->set_PQ_handle(h);
  }
  
  void update_in_PQ( vertex_pair_ptr const& aPair )
  {
    CGAL_precondition(aPair);
    CGAL_precondition(aPair->is_in_PQ());
    aPair->set_PQ_handle( mPQ.update(aPair->PQ_handle()) ) ; 
  }   
  
  void remove_from_PQ( vertex_pair_ptr const& aPair )
  {
    CGAL_precondition(aPair);
    CGAL_precondition(aPair->is_in_PQ());
    mPQ.erase(aPair->PQ_handle());
    aPair->reset_PQ_handle();
  }   
  
  vertex_pair_ptr pop_from_PQ() 
  {
    vertex_pair_ptr rR ;
    if ( !mPQ.empty() )
    {
      rR = mPQ.top(); 
      mPQ.pop();
      rR->reset_PQ_handle();
    }
    
    return rR ;
  }

  Point_3 const& get_point ( const_vertex_descriptor const& v )
  {
    vertex_point_t vertex_point ;
    return get(vertex_point,const_cast<TSM const&>(mSurface),v) ;
  }
  
  bool is_vertex_fixed ( vertex_descriptor const& v )
  {
    cgal_tsms_is_vertex_fixed_t is_vertex_fixed ;
    return get(is_vertex_fixed,mSurface,v) ;
  }
  
private:

  TSM& mSurface ;
  ParamsToGetCollapseData const* mParamsToGetCollapseData ; // Can be NULL

  GetCollapseData   const&  Get_collapse_data;   
  GetCost           const&  Get_cost ;
  GetNewVertexPoint const&  Get_new_vertex_point ;
  ShouldStop        const&  Should_stop ;
  VisitorT*                 Visitor ;
  
  bool mIncludeNonEdgePairs;

private:

  Collapse_triangulation_edge<TSM> Collapse_triangulation_edge ;  

  edges_2_pairs_map mEdgesToPairsMap ;
  PQ                mPQ ;
  
  std::size_t mInitialPairCount ;
  std::size_t mCurrentPairCount ; 

} ;

} } // namespace Triangulated_surface_mesh::Simplification

CGAL_END_NAMESPACE

#include <CGAL/Surface_mesh_simplification/Vertex_pair_collapse_impl.h>

#endif // CGAL_SURFACE_MESH_SIMPLIFICATION__VERTEX_PAIR_COLLAPSE_H //
// EOF //
 
