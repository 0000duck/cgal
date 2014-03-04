 /// \ingroup PkgSurfaceModelingConcepts
 /// \cgalConcept
 ///
 /// @brief Concept describing the set of requirements for calculating weights for edges. 
 ///
 /// \cgalHeading{Example:}
 ///
 /// \code
 /// // a simple model to SurfaceModelingWeights concept, which provides uniform weights
 /// template <class HalfedgeGraph>
 /// struct Identity_weight
 /// {
 ///   typedef HalfedgeGraph Halfedge_graph;
 ///   template<class VertexPointMap>
 ///   double operator()(typename boost::graph_traits<HalfedgeGraph>::edge_descriptor  /*e*/, const HalfedgeGraph& /*p*/, VertexPointMap /*v*/)
 ///   { return 1.0; }
 /// };
 /// \endcode
 ///
 ///
class SurfaceModelingWeights
{
public:
/// \name Types 
/// @{
  /// a model of HalfedgeGraph
  typedef unspecified_type Halfedge_graph;
/// @} 

/// \name Creation 
/// @{
  /// Default constructor. Required only if the default parameter is used in the constructor of `CGAL::Deform_mesh`.
  SurfaceModelingWeights();
/// @} 

/// \name Operations 
/// @{
  /// Function computing the edge weight of edge `e`
  /// \tparam VertexPointMap a model of `ReadWritePropertyMap`</a>  with boost::graph_traits<Halfedge_graph>::vertex_descriptor as key and a 3D point from a \cgal Kernel as value type
  template <class VertexPointMap>
  double operator()(boost::graph_traits<Halfedge_graph>::edge_descriptor  e, const Halfedge_graph& halfedge_graph, VertexPointMap vpm);
/// @}
};


