// Copyright (c) 2007-2008  INRIA (France).
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
// Author(s)     : Laurent RINEAU, Laurent Saboret

#ifndef CGAL_POISSON_REFINEMENT_3_H
#define CGAL_POISSON_REFINEMENT_3_H

// CGAL
#include <CGAL/Mesher_level.h>
#include <CGAL/Mesh_3/Refine_tets.h>
#include <CGAL/Mesh_criteria_3.h>
#include <CGAL/Iso_cuboid_oracle_3.h>
#include <CGAL/surface_reconstruction_assertions.h>

CGAL_BEGIN_NAMESPACE


/// Utility class for poisson_refinement_3():
/// implements Delaunay refinement (break bad tetrahedra, where
/// bad means badly shaped or too big).
///
/// This class must be derived to inherit from Mesher_level.
template <class Tr,
          class Criteria,
          class Surface,
          class Oracle,
          class Container = Meshes::Double_map_container<typename Tr::Cell_handle,
                                                         typename Criteria::Quality>
>
class Poisson_mesher_level_impl_base :
    public Mesh_3::Refine_tets_with_oracle_base<Tr, Criteria, Surface, Oracle, Container>
{
protected:
  typedef Mesh_3::Refine_tets_with_oracle_base<Tr, Criteria, Surface, Oracle, Container> Base;

public:
  typedef typename Tr::Geom_traits Geom_traits;
  typedef typename Tr::Vertex_handle Vertex_handle;
  typedef typename Tr::Cell_handle Cell_handle;
  typedef typename Tr::Point Point;
  typedef typename Base::Quality Quality;

  using Base::triangulation_ref_impl;  
  
public:
  /** \name CONSTRUCTORS */

  Poisson_mesher_level_impl_base(Tr& t, Criteria crit, unsigned int max_vert, Surface& surface, Oracle& oracle)
    : Base(t, crit, surface, oracle), 
      max_vertices(max_vert), ///< number of vertices bound (ignored if zero)
      max_memory(CGAL::Memory_sizer().virtual_size()) ///< max memory allocated by this algorithm
  {
  }

protected:
  /* --- protected functions --- */

  bool test_if_cell_is_bad(const Cell_handle c)
  {
    Quality q;
    if( is_in_domain(c) && should_be_refined(c, q) )
    {
      this->add_bad_element(c, q);
      return true;
    }
    return false;
  }

  bool is_in_domain(const Cell_handle c)
  {
    return oracle.is_in_volume(surface, triangulation_ref_impl().dual(c));
  }

public:
  /* Overriden functions of this level: */
  /* we override all methods that call test_if_cell_is_bad() */

  void scan_triangulation_impl()
  {
    for(typename Tr::Finite_cells_iterator cit = triangulation_ref_impl().finite_cells_begin();
        cit != triangulation_ref_impl().finite_cells_end();
        ++cit)
      test_if_cell_is_bad(cit);
  }

  void after_insertion_impl(const Vertex_handle& v)
  {
    update_star(v);

    // Update used memory
    long memory = CGAL::Memory_sizer().virtual_size(); 
    max_memory = (std::max)(max_memory, memory); 
  }

  void update_star(const Vertex_handle& v)
  {
    // scan refiner
    typedef std::vector<Cell_handle> Cells;
    typedef typename Cells::iterator Cell_iterator;
    Cells incident_cells;

    triangulation_ref_impl().incident_cells(v, std::back_inserter(incident_cells));

    for(Cell_iterator cit = incident_cells.begin();
        cit != incident_cells.end();
        ++cit)
    {
      if( ! triangulation_ref_impl().is_infinite(*cit) )
        test_if_cell_is_bad(*cit);
    }
  }

  /// Tells if the algorithm is done.
  bool no_longer_element_to_refine_impl() const
  {
    return Base::no_longer_element_to_refine_impl() || 
           (max_vertices > 0 && triangulation_ref_impl().number_of_vertices() >= max_vertices);
  }

  /// Give max memory allocated by this algorithm.
  long max_memory_allocated() const
  {
    return max_memory;
  }

private:
  /* --- private datas --- */
  unsigned int max_vertices; ///< number of vertices bound (ignored if zero)
  long max_memory; ///< max memory allocated by this algorithm

}; // end Poisson_mesher_level_impl_base


/// Utility class for poisson_refinement_3():
/// glue class that inherits from both Mesher_level 
/// and Poisson_mesher_level_impl_base.
template <typename Tr,
          typename Criteria,
          typename Surface,
          typename Oracle = typename CGAL::Surface_mesh_traits_generator_3<Surface>::type
 >
class Poisson_mesher_level : 
  public Poisson_mesher_level_impl_base<Tr, Criteria, Surface, Oracle>, 
  public Mesher_level <
    Tr,
    Poisson_mesher_level<Tr, Criteria, Surface, Oracle>,
    typename Tr::Cell_handle,
    Null_mesher_level,
    Triangulation_mesher_level_traits_3<Tr>
  >
{
  typedef Poisson_mesher_level_impl_base<Tr, Criteria, Surface, Oracle> Base;

public:
  typedef Mesher_level <
    Tr,
    Poisson_mesher_level<Tr, Criteria, Surface, Oracle>,
    typename Tr::Cell_handle,
    Null_mesher_level,
    Triangulation_mesher_level_traits_3<Tr>
  > Mesher;
  
  Poisson_mesher_level(Tr& t, Criteria criteria, unsigned int max_vertices, Surface& surface, Oracle& oracle = Oracle())
    : Base(t, criteria, max_vertices, surface, oracle), 
      Mesher(Null_mesher_level()) 
  {
  }

}; // end class Poisson_mesher_level


/// Delaunay refinement (break bad tetrahedra, where
/// bad means badly shaped or too big). 
/// @return the number of vertices inserted.
///
/// Precondition: 
/// convergence is guaranteed if radius_edge_ratio_bound >= 1.0.
///
/// @heading Parameters:
/// @param Tr 3D Delaunay triangulation.
/// @param Surface Sphere_3 or Iso_cuboid_3.
template <typename Tr,
          typename Surface>
unsigned int poisson_refinement_3(Tr& tr,
                                  double radius_edge_ratio_bound, ///< radius edge ratio bound (>= 1.0)
                                  double cell_radius_bound, ///< cell radius bound (ignored if zero)
                                  unsigned int max_vertices, ///< number of vertices bound (ignored if zero)
                                  Surface& enlarged_bbox) ///< new bounding sphere or box
{
  CGAL_TRACE("Call poisson_refinement_3()\n");

  // Convergence is guaranteed if radius_edge_ratio_bound >= 1.0
  CGAL_precondition(radius_edge_ratio_bound >= 1.0);

  // Basic geometric types
  typedef typename Tr::Geom_traits Gt;
  typedef typename Gt::FT FT;
  typedef typename Gt::Point_3 Point;
  
  // Mesher_level types
  typedef Mesh_criteria_3<Tr> Tets_criteria;
  typedef Poisson_mesher_level<Tr, Tets_criteria, Surface> Refiner;

  long memory = CGAL::Memory_sizer().virtual_size(); CGAL_TRACE("  %ld Mb allocated\n", memory>>20);
  CGAL_TRACE("  Create queue\n");

  int nb_vertices = tr.number_of_vertices(); // get former #vertices

  // Delaunay refinement
  Tets_criteria tets_criteria(radius_edge_ratio_bound*radius_edge_ratio_bound, cell_radius_bound);
  Refiner refiner(tr, tets_criteria, max_vertices, enlarged_bbox);  
  refiner.scan_triangulation(); // Push bad cells to the queue
  refiner.refine(Null_mesh_visitor()); // Refine triangulation until queue is empty
  
  int nb_vertices_added = tr.number_of_vertices() - nb_vertices;

  long max_memory = refiner.max_memory_allocated();
  CGAL_TRACE("  Max allocation = %ld Mb\n", max_memory>>20);

  /*long*/ memory = CGAL::Memory_sizer().virtual_size(); CGAL_TRACE("  %ld Mb allocated\n", memory>>20);
  CGAL_TRACE("End of poisson_refinement_3()\n");

  return (unsigned int) nb_vertices_added;
}


CGAL_END_NAMESPACE

#endif // CGAL_POISSON_REFINEMENT_3_H
