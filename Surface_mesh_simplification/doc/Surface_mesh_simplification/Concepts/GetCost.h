
/*!
\ingroup PkgSurfaceMeshSimplificationConcepts
\cgalconcept

The concept `GetCost` describes the requirements for the <I>policy function object</I> 
which gets the <I>collapse cost</I> of an edge. 

The cost returned is a `boost::optional` value (i.e. it can be absent). 
An <I>absent</I> cost indicates that the edge should not be collapsed. 
This could be the result of a computational limitation (such as overflow), 
or can be intentionally returned to prevent the edge from being collapsed. 

\refines ::DefaultConstructible 
\refines ::CopyConstructible 

\hasModel `CGAL::Surface_mesh_simplification::Edge_length_cost<ECM>`
\hasModel `CGAL::Surface_mesh_simplification::LindstromTurk_cost<ECM>`

*/

class GetCost {
public:

/// \name Types 
/// @{

/*! 
The type of the edge profile cache. Must be a model of the `EdgeProfile` concept. 
*/ 
typedef Hidden_type Profile; 

/*! 
A field type representing the collapse cost 
*/ 
typedef Hidden_type FT; 

/*! 
The point type for the surface vertex. Must be a model of `Point_3`. 
*/ 
typename CGAL::halfedge_graph_traits<ECM>::Point Point; 

/*! 
The type of the result (an optional cost value). 
*/ 
boost::optional<FT> result_type; 

/// @} 

/// \name Operations 
/// @{

/*! 
Computes and returns the cost of collapsing the edge (represented by its profile), 
using the calculated placement. 

*/ 
result_type operator()( Profile const& edge_profile 
, boost::optional<Point> const& placement ) const; 

/// @}

}; /* end GetCost */

