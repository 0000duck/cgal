#ifndef MARCHING_TETRAHEDRA_OBSERVER_DEFAULT_3_H
#define MARCHING_TETRAHEDRA_OBSERVER_DEFAULT_3_H

CGAL_BEGIN_NAMESPACE

template <class MarchingTetrahedraTraits_3>
class Marching_tetrahedra_observer_default_3 {
public:
  typedef MarchingTetrahedraTraits_3            Traits;
  typedef typename Traits::Triangulation        Triangulation;
  typedef typename Traits::Halfedge_DS          Halfedge_DS;
  
  typedef typename Triangulation::Cell_handle   Triangulation_cell_handle;
  typedef typename Halfedge_DS::Vertex_handle   Polyhedron_vertex_handle; 
  typedef typename Halfedge_DS::Facet_handle    Polyhedron_facet_handle; 

  Marching_tetrahedra_observer_default_3() {
  }

  Marching_tetrahedra_observer_default_3(
    const  Marching_tetrahedra_observer_default_3& traits2) {
  }

  void after_vertex_insertion(
    Triangulation_cell_handle ch,  int i, int j,
    Polyhedron_vertex_handle vh) {
  }
  
  void after_facet_insertion(
    Triangulation_cell_handle ch,
    Polyhedron_facet_handle fh) {
  }

};

CGAL_END_NAMESPACE

#endif // MARCHING_TETRAHEDRA_OBSERVER_DEFAULT_3_H
