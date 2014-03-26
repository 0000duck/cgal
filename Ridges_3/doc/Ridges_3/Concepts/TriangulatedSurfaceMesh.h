
/*!
  \ingroup PkgRidges_3Concepts
  \cgalConcept

  The concept `TriangulatedSurfaceMesh` describes an oriented surface (possibly with 
  boundary) embedded in the 3 space. It is combinatorially based on a 
  halfedge data structure with triangular faces, geometrically 3d points 
  associated to the vertices define the embedding. 

  \cgalHasModel `CGAL::Polyhedron_3` with the restriction that faces are triangular.

  \cgalHeading{Creation}

  Construction and destruction are undefined. 
*/
class TriangulatedSurfaceMesh {
public:

  /// \name Types 
  /// @{

  /*!

    geometric Traits, this is a CGAL::Kernel concept. 

  */ 
  typedef unspecified_type Traits; 

  /*!
    \cgalConcept
    Opaque type representing a facet of the `TriangulatedSurfaceMesh` . 
  */ 
  class Facet {
    /*!
      an incident halfedge that points to `facet`. 
    */ 
    Halfedge_const_handle halfedge() const; 
  };

  /*!
    \cgalConcept
    Opaque type representing a vertex of the `TriangulatedSurfaceMesh` . 
  */ 
  class Vertex {
  public:
    /*!
      The point associated to the vertex 
    */ 
    Traits::Point_3& point(); 
  };


  /*!
    \cgalConcept
    Opaque type representing a halfedge of the `TriangulatedSurfaceMesh` . 
  */ 
  class Halfedge {
  public:
    /*!
      the opposite halfedge. 
    */ 
    Halfedge_const_handle opposite() const; 

    /*!
      the next halfedge around the facet. 
    */ 
    Halfedge_const_handle next() const; 

    /*!
      the previous halfedge around the facet. 
    */ 
    Halfedge_const_handle prev() const; 

    /*!
      is true if `halfedge` or `halfedge.opposite()` is a border halfedge. 
    */ 
    bool is_border_edge() const; 

    /*!
      the incident vertex of `halfedge`. 
    */ 
    Vertex_const_handle vertex() const; 

    /*!
      the incident facet of `halfedge`. If `halfedge` is a border halfedge 
      the result is default construction of the handle. 
    */ 
    Facet_const_handle facet() const; 
  };

  /*!

    Handle to a facet. Model of the Handle concept. 

  */ 
  typedef unspecified_type Facet_handle; 

  /*!

   */ 
  typedef unspecified_type Facet_const_handle; 

  /*!

    Iterator over all mesh facets. Model of the ForwardIterator concept. 

  */ 
  typedef unspecified_type Facet_iterator; 

  /*!

   */ 
  typedef unspecified_type Facet_const_iterator; 

  /*!

    Handle to a vertex. Model of the Handle concept. 

  */ 
  typedef unspecified_type Vertex_handle; 

  /*!

   */ 
  typedef unspecified_type Vertex_const_handle; 

  /*!

    Iterator over all vertices of a mesh. Model of the ForwardIterator concept. 

  */ 
  typedef unspecified_type Vertex_iterator; 

  /*!

   */ 
  typedef unspecified_type Vertex_const_iterator; 

  /*!

    Handle to a halfedge. Model of the Handle concept. 

  */ 
  typedef unspecified_type Halfedge_handle; 

  /*!

   */ 
  typedef unspecified_type Halfedge_const_handle; 

  /*!

    Iterator over all halfedges of a `TriangulatedSurfaceMesh` . Model of the ForwardIterator concept. 

  */ 
  typedef unspecified_type Halfedge_iterator; 

  /*!

   */ 
  typedef unspecified_type Halfedge_const_iterator; 

  /*!
    Iterator over all points of a `TriangulatedSurfaceMesh` , its
    value type is `Traits::Point_3`. Model of the `ForwardIterator`
    concept.
  */ 
  typedef unspecified_type Point_iterator; 

  /*!

   */ 
  typedef unspecified_type Point_const_iterator; 

  /// @} 

  /// \name Operations 
  /// @{

  /*!
    iterator over all facets 
    (excluding holes). 
  */ 
  Facet_const_iterator facets_begin(); 

  /*!
    past-the-end iterator. 
  */ 
  Facet_const_iterator facets_end(); 

  /*!
    iterator over all points. 
  */ 
  Point_iterator points_begin(); 

  /*!
    past-the-end iterator. 
  */ 
  Point_iterator points_end(); 

  /// @}

}; /* end TriangulatedSurfaceMesh */

