// Copyright (c) 2007  INRIA (France).
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
//
// Author(s)     : Laurent Saboret, Pierre Alliez


#ifndef CGAL_POISSON_IMPLICIT_FUNCTION_H
#define CGAL_POISSON_IMPLICIT_FUNCTION_H

#include <queue>
#include <list>
#include <vector>
#include <algorithm>
#include <cmath>
#include <fstream>

#include <CGAL/Implicit_fct_delaunay_triangulation_3.h>
#include <CGAL/spatial_sort.h>
#include <CGAL/taucs_solver.h>
#include <CGAL/k_nearest_neighbor.h>
#include <CGAL/centroid.h>
#include <CGAL/surface_reconstruction_assertions.h>
#include <CGAL/Memory_sizer.h>
#include <CGAL/Peak_memory_sizer.h>
#include <CGAL/poisson_refinement_3.h>

CGAL_BEGIN_NAMESPACE


// functor for priority queue
template<class Candidate>
struct less // read more priority
{
  bool operator()(const Candidate& c1,
                  const Candidate& c2) const
  {
    return (c1.score() < c2.score());
  }
};

// functor for priority queue
template<class Candidate>
struct more // read more priority
{
  bool operator()(const Candidate& c1,
                  const Candidate& c2) const
  {
    return (c1.score() > c2.score());
  }
};

template <class Handle, class Point>
class Candidate
{
private:
  Handle m_v0;
  Handle m_v1;
  Handle m_v2;
  Handle m_v3;
  float m_score;

public:

  Candidate(Handle v0,
            Handle v1,
            Handle v2,
            Handle v3,
            float score)
  {
    m_v0 = v0;
    m_v1 = v1;
    m_v2 = v2;
    m_v3 = v3;
    m_score = score;
  }
  ~Candidate() {}

public:

  float score() const { return m_score; }
  float& score() { return m_score; }

  Handle v0() { return m_v0; }
  Handle v1() { return m_v1; }
  Handle v2() { return m_v2; }
  Handle v3() { return m_v3; }

};

/// Poisson_implicit_function computes an indicator function f() piecewise-linear
/// over the tetrahedra. We solve the Poisson equation
/// Laplacian(f) = divergent(normals field) at each vertex
/// of the triangulation via the TAUCS sparse linear
/// solver. One vertex must be constrained.
///
/// @heading Is Model for the Concepts:
/// Model of the ReconstructionImplicitFunction concept.
///
/// @heading Design Pattern:
/// A model of ReconstructionImplicitFunction is a
/// Strategy [GHJV95]: it implements a strategy of surface mesh reconstruction.
///
/// @heading Parameters:
/// @param Gt Geometric traits class
/// @param ImplicitFctDelaunayTriangulation_3 3D Delaunay triangulation,
///        model of ImplicitFctDelaunayTriangulation_3 concept.

template <class Gt, class ImplicitFctDelaunayTriangulation_3>
class Poisson_implicit_function
{
// Public types
public:

  typedef ImplicitFctDelaunayTriangulation_3 Triangulation;

  typedef Gt Geom_traits; ///< Kernel's geometric traits

  typedef typename Geom_traits::FT FT;
  typedef typename Geom_traits::Point_3 Point;
  typedef typename Geom_traits::Vector_3 Vector;
  typedef typename Geom_traits::Iso_cuboid_3 Iso_cuboid;
  typedef typename Geom_traits::Sphere_3 Sphere;

  typedef typename Triangulation::Point_with_normal Point_with_normal;
                                                     ///< Model of PointWithNormal_3
  typedef typename Point_with_normal::Normal Normal; ///< Model of Kernel::Vector_3 concept.

// Private types
private:

  // Repeat ImplicitFctDelaunayTriangulation_3 types
  typedef typename Triangulation::Triangulation_data_structure Triangulation_data_structure;
  typedef typename Geom_traits::Ray_3 Ray;
  typedef typename Geom_traits::Plane_3 Plane;
  typedef typename Geom_traits::Segment_3 Segment;
  typedef typename Geom_traits::Triangle_3 Triangle;
  typedef typename Geom_traits::Tetrahedron_3 Tetrahedron;
  typedef typename Triangulation::Cell_handle   Cell_handle;
  typedef typename Triangulation::Vertex_handle Vertex_handle;
  typedef typename Triangulation::Cell   Cell;
  typedef typename Triangulation::Vertex Vertex;
  typedef typename Triangulation::Facet  Facet;
  typedef typename Triangulation::Edge   Edge;
  typedef typename Triangulation::Cell_circulator  Cell_circulator;
  typedef typename Triangulation::Facet_circulator Facet_circulator;
  typedef typename Triangulation::Cell_iterator    Cell_iterator;
  typedef typename Triangulation::Facet_iterator   Facet_iterator;
  typedef typename Triangulation::Edge_iterator    Edge_iterator;
  typedef typename Triangulation::Vertex_iterator  Vertex_iterator;
  typedef typename Triangulation::Point_iterator   Point_iterator;
  typedef typename Triangulation::Finite_vertices_iterator Finite_vertices_iterator;
  typedef typename Triangulation::Finite_cells_iterator    Finite_cells_iterator;
  typedef typename Triangulation::Finite_facets_iterator   Finite_facets_iterator;
  typedef typename Triangulation::Finite_edges_iterator    Finite_edges_iterator;
  typedef typename Triangulation::All_cells_iterator       All_cells_iterator;
  typedef typename Triangulation::Locate_type Locate_type;

// Data members
private:

  Triangulation& m_dt; // f() is pre-computed on vertices of m_dt by solving
                       // the Poisson equation Laplacian(f) = divergent(normals field)

  // neighbor search
  typedef typename CGAL::K_nearest_neighbor<Geom_traits,Vertex_handle> K_nearest_neighbor;
  typedef typename CGAL::Point_vertex_handle_3<Vertex_handle> Point_vertex_handle_3;
  K_nearest_neighbor m_nn_search;

  // contouring and meshing
  Point m_sink; // Point with the minimum value of f()
  mutable Cell_handle m_hint; // last cell found = hint for next search

// Public methods
public:

  /// Create a Poisson indicator function f() piecewise-linear
  /// over the tetrahedra of pdt.
  /// If pdt is empty, create an empty implicit function.
  ///
  /// @param pdt ImplicitFctDelaunayTriangulation_3 base of the Poisson indicator function.
  Poisson_implicit_function(Triangulation& pdt)
  : m_dt(pdt)
  {
  }

  /// Create an implicit function from a point set.
  /// Insert the first...beyond point set into pdt and
  /// create a Poisson indicator function f() piecewise-linear
  /// over the tetrahedra of pdt.
  ///
  /// Precondition: the value type of InputIterator must be convertible to Point_with_normal.
  ///
  /// @param pdt ImplicitFctDelaunayTriangulation_3 base of the Poisson indicator function.
  /// @param first First point to add.
  /// @param beyond Past-the-end point to add.
  template < class InputIterator >
  Poisson_implicit_function(Triangulation& pdt,
                            InputIterator first, InputIterator beyond)
  : m_dt(pdt)
  {
    insert(first, beyond);
  }

  /// Insert points.
  ///
  /// Precondition: the value type of InputIterator must be convertible to Point_with_normal.
  ///
  /// @param first First point to add.
  /// @param beyond Past-the-end point to add.
  /// @return the number of inserted points.
  template < class InputIterator >
  int insert(InputIterator first, InputIterator beyond)
  {
    return m_dt.insert(first, beyond);
  }

  /// Remove all points.
  void clear()
  {
    m_dt.clear();
  }

  /// Get embedded triangulation.
  Triangulation& triangulation()
  {
    return m_dt;
  }
  const Triangulation& triangulation() const
  {
    return m_dt;
  }

  /// Get the surface's bounding box.
  Iso_cuboid bounding_box() const
  {
    return m_dt.input_points_bounding_box();
  }

  /// Get the surface's bounding sphere.
  Sphere bounding_sphere() const
  {
    return m_dt.input_points_bounding_sphere();
  }

  /// Get the region of interest, ignoring the outliers.
  /// This method is used to define the OpenGL arcball sphere.
  Sphere region_of_interest() const
  {
    // A good candidate is a sphere containing the dense region of the point cloud:
    // - center point is barycenter
    // - Radius is 2 * standard deviation
    Point barycenter = m_dt.barycenter();
    FT radius = 2.f * (FT)m_dt.diameter_standard_deviation();

    return Sphere(barycenter, radius*radius);
  }

  /// You should call compute_implicit_function() once when points insertion is over.
  /// It computes the Poisson indicator function f()
  /// at each vertex of the triangulation by:
  /// - applying a Delaunay refinement to define the function
  ///   inside and outside the surface.
  /// - solving the Poisson equation
  ///   Laplacian(f) = divergent(normals field) at each vertex
  ///   of the triangulation via the TAUCS sparse linear
  ///   solver. One vertex must be constrained.
  /// - shifting and orienting f() such that f() = 0 on the input points,
  ///   and f() < 0 inside the surface.
  ///
  /// Return false on error.
  /// TODO: add parameters to compute_implicit_function()?
  bool compute_implicit_function()
  {
    CGAL::Timer task_timer; task_timer.start();
    CGAL_TRACE_STREAM << "Delaunay refinement...\n";

    // Delaunay refinement
    const FT radius_edge_ratio_bound = 2.5;
    const unsigned int max_vertices = (unsigned int)1e7; // max 10M vertices
    const FT enlarge_ratio = 1.5;
    const FT size = sqrt(bounding_sphere().squared_radius()); // get triangulation's radius
    const FT cell_radius_bound = size/5.; // large
    unsigned int nb_vertices_added = delaunay_refinement(radius_edge_ratio_bound,cell_radius_bound,max_vertices,enlarge_ratio);

    // Print status
    CGAL_TRACE_STREAM << "Delaunay refinement: " << "added " << nb_vertices_added << " Steiner points, "
                                                 << task_timer.time() << " seconds, "
                                                 << (CGAL::Memory_sizer().virtual_size()>>20) << " Mb allocated"
                                                 << std::endl;
    task_timer.reset();

    // Smooth normals field.
    // Commented out as it shrinks the reconstructed model.
    //extrapolate_normals();

    CGAL_TRACE_STREAM << "Solve Poisson equation...\n";

    /// Compute the Poisson indicator function f()
    /// at each vertex of the triangulation.
    double lambda = 0.1;
    double duration_assembly, duration_factorization, duration_solve;
    if (!solve_poisson(lambda, &duration_assembly, &duration_factorization, &duration_solve))
    {
      std::cerr << "Error: cannot solve Poisson equation" << std::endl;
      return false;
    }

    // Shift and orient f() such that:
    // - f() = 0 on the input points,
    // - f() < 0 inside the surface.
    set_contouring_value(median_value_at_input_vertices());

    // Print status
    CGAL_TRACE_STREAM << "Solve Poisson equation: " << task_timer.time() << " seconds, "
                                                    << (CGAL::Memory_sizer().virtual_size()>>20) << " Mb allocated"
                                                    << std::endl;
    task_timer.reset();

    return true;
  }

  //Calculate and store average spacing at each input point
  void average_spacing_avg_knn_sq_distance_3()
  {
    Finite_vertices_iterator v;
    int number_vertices = 0;
    for(v = m_dt.finite_vertices_begin();
        v != m_dt.finite_vertices_end();
        v++)
		number_vertices++;
    for(v = m_dt.finite_vertices_begin();
        v != m_dt.finite_vertices_end();
        v++)
	{
	  FT sq_distance = 0.0;
	  int counter = 0;
	  // std::stack<Vertex_handle> vertices; // use to walk in 3D Delaunay
	  //  vertices.push(v);
	  std::list<Vertex_handle> v_neighbors;
	  m_dt.incident_vertices(v,std::back_inserter(v_neighbors));
	  typename std::list<Vertex_handle>::iterator it;
	  for(it = v_neighbors.begin();
			  it != v_neighbors.end();
			  it++)
	  {
		  sq_distance = sq_distance +  distance(*it,v)*distance(*it,v);
		  counter++;
	  }

		/*while(!vertices.empty() && counter < 0.001 * number_vertices )
		{
			Vertex_handle v_cur = vertices.top();
			vertices.pop();
			// get incident_vertices
			std::vector<Vertex_handle> v_neighbors;
			sq_distance = sq_distance + distance(v_cur,v)* distance(v_cur,v);
			counter++;
			 m_dt.incident_vertices(v_cur,std::back_inserter(v_neighbors));
			std::vector<Vertex_handle>::iterator it;
			for(it = v_neighbors.begin();
			it != v_neighbors.end();
			it++)
			{
				Vertex_handle nv = *it;
				int tag = nv->tag();
				int index = v_cur->index();
				if (tag != index)
				{
					vertices.push(nv);
					nv->tag() = index;
				}
			}
		*/

	v->average_spacing() = std::sqrt(sq_distance/counter);

	//}
	/*for(v = m_dt.finite_vertices_begin();
        v != m_dt.finite_vertices_end();
        v++)
			v->tag() = -1;*/
	}
  }

  /// Delaunay refinement (break bad tetrahedra, where
  /// bad means badly shaped or too big). The normal of
  /// Steiner points is set to zero.
  /// Return the number of vertices inserted.
  unsigned int delaunay_refinement(FT radius_edge_ratio_bound, ///< radius edge ratio bound (ignored if zero)
                                   FT cell_radius_bound, ///< cell radius bound (ignored if zero)
                                   unsigned int max_vertices, ///< number of vertices bound
                                   FT enlarge_ratio) ///< bounding box enlarge ratio
  {
    CGAL_TRACE("Call delaunay_refinement(radius_edge_ratio_bound=%lf, cell_radius_bound=%lf, max_vertices=%u, enlarge_ratio=%lf)\n",
               radius_edge_ratio_bound, cell_radius_bound, max_vertices, enlarge_ratio);

#define DELAUNAY_REFINEMENT_USE_BOUNDING_BOX
#ifdef  DELAUNAY_REFINEMENT_USE_BOUNDING_BOX
    Iso_cuboid enlarged_bbox = enlarged_bounding_box(enlarge_ratio);
    unsigned int nb_vertices_added = poisson_refinement_3(m_dt,radius_edge_ratio_bound,cell_radius_bound,max_vertices,enlarged_bbox);
#else
    Sphere enlarged_bbox = enlarged_bounding_sphere(enlarge_ratio);
    unsigned int nb_vertices_added = poisson_refinement_3(m_dt,radius_edge_ratio_bound,cell_radius_bound,max_vertices,enlarged_bbox);
#endif

    m_dt.invalidate_bounding_box();

    CGAL_TRACE("End of delaunay_refinement()\n");

    return nb_vertices_added;
  }

  unsigned int delaunay_refinement_shell(FT size_shell,
                                         FT sizing,
                                         unsigned int max_vertices)
  {
    // make parameters relative to size
    Sphere bounding_sphere = m_dt.bounding_sphere();
    FT size = sqrt(bounding_sphere.squared_radius());
    size_shell *= size;
    sizing *= size;

    init_nn_search_shell();

    typedef typename CGAL::Candidate<Vertex_handle,Point> Candidate;
    typedef typename std::priority_queue<Candidate,
                                         std::vector<Candidate>,
                                         more<Candidate> > PQueue;

    // push all cells to the queue
    PQueue queue;
    Finite_cells_iterator c;
    for(c = m_dt.finite_cells_begin();
        c != m_dt.finite_cells_end();
        c++)
    {
      Point p;
      FT size = 0.0;
      if(is_refinable(c,size_shell,sizing,size,p))
      {
        Vertex_handle v0 = c->vertex(0);
        Vertex_handle v1 = c->vertex(1);
        Vertex_handle v2 = c->vertex(2);
        Vertex_handle v3 = c->vertex(3);
        queue.push(Candidate(v0,v1,v2,v3,(float)size));
      }
    }

    unsigned int nb = 0;
    while(!queue.empty())
    {
      Candidate candidate = queue.top();
      queue.pop();
      Vertex_handle v0 = candidate.v0();
      Vertex_handle v1 = candidate.v1();
      Vertex_handle v2 = candidate.v2();
      Vertex_handle v3 = candidate.v3();

      Cell_handle cell = NULL;
      if(m_dt.is_cell(v0,v1,v2,v3,cell))
      {
        Point circumcenter = m_dt.dual(cell);
        Vertex_handle v = m_dt.insert(circumcenter, Triangulation::STEINER);

        if(nb++ > max_vertices)
          return nb; // premature ending

        // iterate over incident cells and feed queue
        std::list<Cell_handle> cells;
        m_dt.incident_cells(v,std::back_inserter(cells));
        typename std::list<Cell_handle>::iterator it;
        for(it = cells.begin();
            it != cells.end();
            it++)
        {
          Cell_handle c = *it;
          if(m_dt.is_infinite(c))
            continue;

          Point p;
          FT size = 0.0;
          if(is_refinable(c,size_shell,sizing,size,p))
          {
            Vertex_handle v0 = c->vertex(0);
            Vertex_handle v1 = c->vertex(1);
            Vertex_handle v2 = c->vertex(2);
            Vertex_handle v3 = c->vertex(3);
            queue.push(Candidate(v0,v1,v2,v3,(float)size));
          }
        }
      }
    }
		m_nn_search.clear();
    return nb;
  }

  /// Extrapolate the normals field:
  /// compute null normals by averaging neighbour normals.
  void extrapolate_normals()
  {
    // Compute extrapolated normals and store them in extrapolated_normals[]
    std::map<Vertex_handle,Normal> extrapolated_normals; // vector + orientation
    Finite_vertices_iterator v;
    for(v = m_dt.finite_vertices_begin();
        v != m_dt.finite_vertices_end();
        v++)
    {
      if(v->normal() != CGAL::NULL_VECTOR)
        continue;

      Vector normal = CGAL::NULL_VECTOR;  // normal vector to compute

      std::list<Vertex_handle> vertices;
      m_dt.incident_vertices(v,std::back_inserter(vertices));
      for(typename std::list<Vertex_handle>::iterator it = vertices.begin();
          it != vertices.end();
          it++)
      {
        Vertex_handle nv = *it;
        normal = normal + nv->normal();
      }

      FT sq_norm = normal * normal;
      if(sq_norm > 0.0)
        normal = normal / std::sqrt(sq_norm);

      extrapolated_normals[v] = Normal(normal);
    }

    // set normals
    for(v = m_dt.finite_vertices_begin();
      v != m_dt.finite_vertices_end();
      v++)
    {
      if(v->normal() != CGAL::NULL_VECTOR)
        continue;

      typename std::map<Vertex_handle,Normal>::iterator it = extrapolated_normals.find(v);
      if(it != extrapolated_normals.end())
        v->normal() = extrapolated_normals[v];
    }
  }

   FT gaussian_function( FT sigma , FT distance)
   {
	   FT answer = (1 / std::sqrt(2 * 3.14)) * std::exp(-1 * distance * distance /(2 * sigma * sigma));
	   return answer;
   }


  /// Extrapolate the normals field:

  int extrapolate_normals_using_gaussian_kernel()
  {
    int counter = 0;
    Finite_vertices_iterator v;
    for(v = m_dt.finite_vertices_begin();
      v != m_dt.finite_vertices_end();
      v++)
    {
      if(v->type() == Triangulation::INPUT)
      {
        FT limit_distance =  v->average_spacing();
        std::stack<Vertex_handle> vertices; // use to walk in 3D Delaunay
        vertices.push(v);

        while(!vertices.empty())
        {
          Vertex_handle v_cur = vertices.top();
          vertices.pop();
          FT distance_cur = distance(v,v_cur);
          if (distance_cur > limit_distance)
            continue;
          if (v_cur->type() != Triangulation::INPUT)
          {
            FT gf = gaussian_function(limit_distance,distance_cur);
            v_cur->normal() = v_cur->normal() + gf * v->normal();
          }
          // get incident_vertices
          std::vector<Vertex_handle> v_neighbors;
          m_dt.incident_vertices(v_cur,std::back_inserter(v_neighbors));
          typename std::vector<Vertex_handle>::iterator it;
          for(it = v_neighbors.begin(); it != v_neighbors.end(); it++)
          {
            Vertex_handle nv = *it;
            int tag = nv->tag();
            int index = v_cur->index();
            if (tag != index)
            {
              vertices.push(nv);
              nv->tag() = index;
            }
          }
        }
      }

    }

    for(v = m_dt.finite_vertices_begin();
      v != m_dt.finite_vertices_end();
      v++)
    {
      if(v->type() != Triangulation::INPUT )
      {
        FT sq_norm = std::sqrt(v->normal()*v->normal());
        if(sq_norm > 0.0)
        {
          v->normal() = v->normal() / sq_norm;
          counter++;
        }
        //v->type() = Triangulation::INPUT;
      }
    }
    return counter;
  }


  /// Poisson reconstruction.
  /// Return false on error.
  bool solve_poisson(double lambda,
                     double* duration_assembly,
                     double* duration_factorization,
                     double* duration_solve,
                     bool is_normalized = false)
  {
    CGAL_TRACE("Call solve_poisson()\n");

    double time_init = clock();

    *duration_assembly = 0.0;
    *duration_factorization = 0.0;
    *duration_solve = 0.0;

    long old_max_memory = CGAL::Peak_memory_sizer().peak_virtual_size();

    CGAL_TRACE("  %ld Mb allocated, largest free memory block=%ld Mb, #blocks over 100 Mb=%ld\n",
               long(CGAL::Memory_sizer().virtual_size())>>20,
               long(taucs_available_memory_size()/1048576.0),
               long(CGAL::Peak_memory_sizer().count_free_memory_blocks(100*1048576)));
    CGAL_TRACE("  Create matrix\n");

    CGAL_TRACE("  %ld Mb allocated, largest free memory block=%ld Mb, #blocks over 100 Mb=%ld\n", 
               long(CGAL::Memory_sizer().virtual_size())>>20,
               long(taucs_available_memory_size()/1048576.0),
               long(CGAL::Peak_memory_sizer().count_free_memory_blocks(100*1048576)));
    CGAL_TRACE("  Create matrix...\n");

    // get #variables
    unsigned int nb_variables = m_dt.index_unconstrained_vertices();

    // at least one vertex must be constrained
    if(nb_variables == m_dt.number_of_vertices())
    {
      constrain_one_vertex_on_convex_hull();
      nb_variables = m_dt.index_unconstrained_vertices();
    }

    // Assemble linear system
    Taucs_solver solver;
    std::vector<double> X(nb_variables);
    std::vector<double> B(nb_variables);

    Finite_vertices_iterator v;
    for(v = m_dt.finite_vertices_begin();
        v != m_dt.finite_vertices_end();
        v++)
    {
      if(!v->constrained())
      {
        B[v->index()] = is_normalized ? div_normalized(v)
                                      : div(v); // rhs -> divergent
        assemble_poisson_row(solver,v,B,lambda);
      }
    }

    *duration_assembly = (clock() - time_init)/CLOCKS_PER_SEC;
    CGAL_TRACE("  Create matrix: done (%.2lf s)\n", *duration_assembly);
    
    /*
    time_init = clock();
    if(!solver.solve_conjugate_gradient(B,X,10000,1e-15))
      return false;
    *duration_solve = (clock() - time_init)/CLOCKS_PER_SEC;
    */

    CGAL_TRACE("  %ld Mb allocated, largest free memory block=%ld Mb, #blocks over 100 Mb=%ld\n",
               long(CGAL::Memory_sizer().virtual_size())>>20,
               long(taucs_available_memory_size()/1048576.0),
               long(CGAL::Peak_memory_sizer().count_free_memory_blocks(100*1048576)));
    CGAL_TRACE("  Choleschy factorization...\n");

    // Choleschy factorization M = L L^T
    time_init = clock();
    if(!solver.factorize_ooc())
      return false;
    *duration_factorization = (clock() - time_init)/CLOCKS_PER_SEC;
    CGAL_TRACE("  Choleschy factorization: done (%.2lf s)\n", *duration_factorization);

    // Print peak memory (Windows only)
    long max_memory = CGAL::Peak_memory_sizer().peak_virtual_size();
    if (max_memory > old_max_memory)
      CGAL_TRACE("  Max allocation = %ld Mb\n", max_memory>>20);

    CGAL_TRACE("  %ld Mb allocated, largest free memory block=%ld Mb, #blocks over 100 Mb=%ld\n",
               long(CGAL::Memory_sizer().virtual_size())>>20,
               long(taucs_available_memory_size()/1048576.0),
               long(CGAL::Peak_memory_sizer().count_free_memory_blocks(100*1048576)));
    CGAL_TRACE("  Direct solve...\n");

    // Direct solve by forward and backward substitution
    time_init = clock();
    if(!solver.solve_ooc(B,X))
      return false;
    *duration_solve = (clock() - time_init)/CLOCKS_PER_SEC;
    CGAL_TRACE("  Direct solve: done (%.2lf s)\n", *duration_solve);

    /*
    // Choleschy factorization M = L L^T
    time_init = clock();
    if(!solver.factorize(true))
      return false;
    *duration_factorization = (clock() - time_init)/CLOCKS_PER_SEC;

    // Direct solve by forward and backward substitution
    time_init = clock();
    if(!solver.solve(B,X,1))
      return false;
    *duration_solve = (clock() - time_init)/CLOCKS_PER_SEC;
    */

    CGAL_TRACE("  Choleschy factorization + solve: done (%.2lf s)\n", *duration_factorization + *duration_solve);

    // set values to vertices
    unsigned int index = 0;
    for(v = m_dt.finite_vertices_begin();
        v != m_dt.finite_vertices_end();
        v++)
      if(!v->constrained())
        v->f() = X[index++];

    CGAL_TRACE("  %ld Mb allocated, largest free memory block=%ld Mb, #blocks over 100 Mb=%ld\n",
               long(CGAL::Memory_sizer().virtual_size())>>20,
               long(taucs_available_memory_size()/1048576.0),
               long(CGAL::Peak_memory_sizer().count_free_memory_blocks(100*1048576)));
    CGAL_TRACE("End of solve_poisson()\n");

    return true;
  }


  void SaveAsMeshFile()
  {
    std::ofstream os("function.mesh");
    Finite_vertices_iterator v;
    int counter = 0;
    for(v = m_dt.finite_vertices_begin();
        v != m_dt.finite_vertices_end();
        v++)
  	{
  		Point& p = v->point();
  		if (std::abs(f(p) - 0) < 0.001)
  		  counter++;
  	}
    os << "MeshVersionFormatted 1\n"
       << "Dimension\n"
       << "3 \n\n"
       << "Vertices\n"
       << counter << " \n";
    for(v = m_dt.finite_vertices_begin();
      v != m_dt.finite_vertices_end();
      v++)
    {
      Point& p = v->point();
      if (std::abs(f(p) - 0) < 0.01)
        os << p.x() << " " << p.y() << " " << p.z() << " " << 0 << std::endl;
    }

    os << "\n" << "End\n";

    os.close();
  }


  /// Shift and orient the implicit function such that:
  /// - the implicit function = 0 for points / f() = contouring_value,
  /// - the implicit function < 0 inside the surface.
  ///
  /// Return the minimum value of the implicit function.
  FT set_contouring_value(FT contouring_value)
  {
    // median value set to 0.0
    shift_f(-contouring_value);

    // check value on convex hull (should be positive)
    Vertex_handle v = any_vertex_on_convex_hull();
    if(v->f() < 0.0)
      flip_f();

    // Update m_sink
    FT sink_value = find_sink();
    return sink_value;
  }

  /// Evaluate implicit function for any 3D point.
  FT f(const Point& p) const
  {
    m_hint = m_dt.locate(p,m_hint);

    if(m_hint == NULL)
      return 1e38;

    if(m_dt.is_infinite(m_hint))
      return 1e38;

    FT a,b,c,d;
    barycentric_coordinates(p,m_hint,a,b,c,d);
    return a * m_hint->vertex(0)->f() +
           b * m_hint->vertex(1)->f() +
           c * m_hint->vertex(2)->f() +
           d * m_hint->vertex(3)->f();
  }

  /// [ImplicitFunction interface]
  ///
  /// Evaluate implicit function for any 3D point.
  FT operator()(const Point& p) const
  {
    return f(p);
  }

  /// Get point inside the surface.
  Point get_inner_point() const
  {
    // Get point / the implicit function is minimum
    return m_sink;
  }

  /// Get average value of the implicit function over input vertices.
  FT average_value_at_input_vertices() const
  {
    FT sum = 0.0;
    unsigned int nb = 0;
    Finite_vertices_iterator v;
    for(v = m_dt.finite_vertices_begin();
        v != m_dt.finite_vertices_end();
        v++)
    {
      if(v->type() == Triangulation::INPUT)
      {
        sum += v->f();
        nb++;
      }
    }
    if(nb > 0)
      return sum / (FT)nb;
    else
    {
      std::cerr << "Contouring: no input points\n";
      return (FT)0.0;
    }
  }

  /// Get median value of the implicit function over input vertices.
  FT median_value_at_input_vertices() const
  {
    std::vector<FT> values;
    Finite_vertices_iterator v;
    for(v = m_dt.finite_vertices_begin();
        v != m_dt.finite_vertices_end();
        v++)
      if(v->type() == Triangulation::INPUT)
        values.push_back(v->f());

    int size = values.size();
    if(size == 0)
    {
      std::cerr << "Contouring: no input points\n";
      return 0.0;
    }

    std::sort(values.begin(),values.end());
    int index = size/2;
    // return values[size/2];
    return 0.5 * (values[index] + values[index+1]); // avoids singular cases
  }

  /// Get min value of the implicit function over input vertices.
  FT min_value_at_input_vertices() const
  {
    FT min_value = 1e38;
    Finite_vertices_iterator v;
    for(v = m_dt.finite_vertices_begin();
        v != m_dt.finite_vertices_end();
        v++)
    {
      if(v->type() == Triangulation::INPUT)
        min_value = (std::min)(min_value, v->f());
    }
    if (m_dt.number_of_vertices() > 0)
    {
      return min_value;
    }
    else
    {
      std::cerr << "Contouring: no input points\n";
      return (FT)0.0;
    }
  }

  /// Get max value of the implicit function over input vertices.
  FT max_value_at_input_vertices() const
  {
    FT max_value = -1e38;
    Finite_vertices_iterator v;
    for(v = m_dt.finite_vertices_begin();
        v != m_dt.finite_vertices_end();
        v++)
    {
      if(v->type() == Triangulation::INPUT)
        max_value = (std::max)(max_value, v->f());
    }
    if (m_dt.number_of_vertices() > 0)
    {
      return max_value;
    }
    else
    {
      std::cerr << "Contouring: no input points\n";
      return (FT)0.0;
    }
  }

  /// Get median value of the implicit function over convex hull vertices.
  FT median_value_at_convex_hull() const
  {
    // Get convex hull vertices
    std::list<Vertex_handle> convex_hull_vertices;
    m_dt.incident_vertices(m_dt.infinite_vertex(),std::back_inserter(convex_hull_vertices));

    // Get values of the implicit function over convex hull vertices
    std::vector<FT> values;
    typename std::list<Vertex_handle>::iterator it;
    for(it = convex_hull_vertices.begin();
        it != convex_hull_vertices.end();
        it++)
    {
      Vertex_handle v = *it;
      values.push_back(v->f());
    }

    int size = values.size();
    if(size == 0)
    {
      std::cerr << "Contouring: no input points\n";
      return 0.0;
    }

    std::sort(values.begin(),values.end());
    int index = size/2;
    // return values[size/2];
    return 0.5 * (values[index] + values[index+1]); // avoids singular cases
  }

  /// Get average value of the implicit function over convex hull vertices.
  FT average_value_at_convex_hull() const
  {
    std::list<Vertex_handle> convex_hull_vertices;
    m_dt.incident_vertices(m_dt.infinite_vertex(),std::back_inserter(convex_hull_vertices));

    FT sum = 0.0;
    unsigned int nb = 0;
    typename std::list<Vertex_handle>::iterator it;
    for(it = convex_hull_vertices.begin();
        it != convex_hull_vertices.end();
        it++,nb++)
    {
      Vertex_handle v = *it;
      sum += v->f();
    }
    if(nb != 0)
      return sum / (FT)nb;
    else
      return 0.0;
  }

// Private methods:
private:

  // PA: todo change type (FT)
  // check if this is in CGAL already
  void barycentric_coordinates(const Point& p,
                               Cell_handle cell,
                               double& a,
                               double& b,
                               double& c,
                               double& d) const
  {
    const Point& pa = cell->vertex(0)->point();
    const Point& pb = cell->vertex(1)->point();
    const Point& pc = cell->vertex(2)->point();
    const Point& pd = cell->vertex(3)->point();
    Tetrahedron ta(pb,pc,pd,p);
    Tetrahedron tb(pa,pc,pd,p);
    Tetrahedron tc(pb,pa,pd,p);
    Tetrahedron td(pb,pc,pa,p);
    Tetrahedron tet(pa,pb,pc,pd);
    double v = tet.volume();
    a = std::fabs(ta.volume() / v);
    b = std::fabs(tb.volume() / v);
    c = std::fabs(tc.volume() / v);
    d = std::fabs(td.volume() / v);
  }

  FT distance(Vertex_handle v1, Vertex_handle v2) const
  {
    const Point& a = v1->point();
    const Point& b = v2->point();
    return std::sqrt(CGAL::squared_distance(a,b));
  }

  FT find_sink()
  {
    m_sink = CGAL::ORIGIN;
    FT min_f = 1e38;
    Finite_vertices_iterator v;
    for(v = m_dt.finite_vertices_begin();
        v != m_dt.finite_vertices_end();
        v++)
    {
      if(v->f() < min_f)
      {
        m_sink = v->point();
        min_f = v->f();
      }
    }
    return min_f;
  }

  void shift_f(const FT shift)
  {
    Finite_vertices_iterator v;
    for(v = m_dt.finite_vertices_begin();
        v != m_dt.finite_vertices_end();
        v++)
      v->f() += shift;
  }

  void flip_f()
  {
    Finite_vertices_iterator v;
    for(v = m_dt.finite_vertices_begin();
        v != m_dt.finite_vertices_end();
        v++)
      v->f() = -v->f();
  }

  Vertex_handle any_vertex_on_convex_hull()
  {
    // TODO: return NULL if none and assert
    std::list<Vertex_handle> vertices;
    m_dt.incident_vertices(m_dt.infinite_vertex(),std::back_inserter(vertices));
    typename std::list<Vertex_handle>::iterator it = vertices.begin();
    return *it;
  }

  void constrain_one_vertex_on_convex_hull(const FT value = 0.0)
  {
    Vertex_handle v = any_vertex_on_convex_hull();
    v->constrained() = true;
    v->f() = value;
  }

  //void constrain_input_vertices_on_convex_hull(const FT value = 0.0)
  //{
  //  for(Finite_vertices_iterator v = m_dt.finite_vertices_begin();
  //    v != m_dt.finite_vertices_end();
  //    v++)
	 // if (v->type() == Triangulation::INPUT)
	 // {
		//  v->constrained() = true;
		//  v->f() = value;
	 // }
  //}

  // divergent
  FT div(Vertex_handle v)
  {
    std::list<Cell_handle> cells;
    m_dt.incident_cells(v,std::back_inserter(cells));
    if(cells.size() == 0)
      return 0.0;

    FT div = 0.0;
    typename std::list<Cell_handle>::iterator it;
    for(it = cells.begin();
        it != cells.end();
        it++)
    {
      Cell_handle cell = *it;
      if(m_dt.is_infinite(cell))
        continue;

      // compute average normal per cell
      Vector n = cell_normal(cell);

      // zero normal - no need to compute anything else
      if(n == CGAL::NULL_VECTOR)
        continue;

      // compute n'
      int index = cell->index(v);
      const Point& a = cell->vertex((index+1)%4)->point();
      const Point& b = cell->vertex((index+2)%4)->point();
      const Point& c = cell->vertex((index+3)%4)->point();
      Vector nn = (index%2==0) ? CGAL::cross_product(b-a,c-a) : CGAL::cross_product(c-a,b-a);
      nn = nn / std::sqrt(nn*nn); // normalize

      Triangle face(a,b,c);
      FT area = std::sqrt(face.squared_area());

      div += n * nn * area;
    }
    return div;
  }

 FT div_normalized(Vertex_handle v)
  {
    std::list<Cell_handle> cells;
    m_dt.incident_cells(v,std::back_inserter(cells));
    if(cells.size() == 0)
      return 0.0;
	FT length = 100000;
	int counter = 0;
    FT div = 0.0;
    typename std::list<Cell_handle>::iterator it;
    for(it = cells.begin();
        it != cells.end();
        it++)
    {
      Cell_handle cell = *it;
      if(m_dt.is_infinite(cell))
        continue;

      // compute average normal per cell
      Vector n = cell_normal(cell);

      // zero normal - no need to compute anything else
      if(n == CGAL::NULL_VECTOR)
        continue;

      // compute n'
      int index = cell->index(v);
	  const Point& x = cell->vertex(index)->point();
      const Point& a = cell->vertex((index+1)%4)->point();
      const Point& b = cell->vertex((index+2)%4)->point();
      const Point& c = cell->vertex((index+3)%4)->point();
      Vector nn = (index%2==0) ? CGAL::cross_product(b-a,c-a) : CGAL::cross_product(c-a,b-a);
	  nn = nn / std::sqrt(nn*nn); // normalize
	  Vector p = a - x;
      Vector q = b - x;
	  Vector r = c - x;
	  FT p_n = std::sqrt(p*p);
	  FT q_n = std::sqrt(q*q);
	  FT r_n = std::sqrt(r*r);
	  FT solid_angle = p*(CGAL::cross_product(q,r));
	  solid_angle = std::abs(solid_angle * 1.0 / (p_n*q_n*r_n + (p*q)*r_n + (q*r)*p_n + (r*p)*q_n));
	  Triangle face(a,b,c);
      FT area = std::sqrt(face.squared_area());
	  length = std::sqrt((x-a)*(x-a)) + std::sqrt((x-b)*(x-b)) + std::sqrt((x-c)*(x-c));
	  counter++;
		div += n * nn * area * 3 / length ;
     }
    return div;
  }

 FT mesh_size(Vertex_handle v) {
	  std::list<Cell_handle> cells;
	  int counter = 0;
	  FT length_total = 100000.0;
    m_dt.incident_cells(v,std::back_inserter(cells));
    if(cells.size() == 0)
      return 0.0;
	    typename std::list<Cell_handle>::iterator it;
    for(it = cells.begin();
        it != cells.end();
        it++)
    {
      Cell_handle cell = *it;
      if(m_dt.is_infinite(cell))
        continue;
	  int index = cell->index(v);
	  const Point& x = cell->vertex(index)->point();
      const Point& a = cell->vertex((index+1)%4)->point();
      const Point& b = cell->vertex((index+2)%4)->point();
      const Point& c = cell->vertex((index+3)%4)->point();
     if (length_total > std::sqrt((x-a)*(x-a)) + std::sqrt((x-b)*(x-b)) + std::sqrt((x-c)*(x-c)))
	  length_total = std::sqrt((x-a)*(x-a)) + std::sqrt((x-b)*(x-b)) + std::sqrt((x-c)*(x-c));
	  counter++;
	  }
	 return length_total / 3 ;
  }

  Vector cell_normal(Cell_handle cell)
  {
    const Vector& n0 = cell->vertex(0)->normal();
    const Vector& n1 = cell->vertex(1)->normal();
    const Vector& n2 = cell->vertex(2)->normal();
    const Vector& n3 = cell->vertex(3)->normal();
    Vector n = n0 + n1 + n2 + n3;
    FT sq_norm = n*n;
    if(sq_norm != 0.0)
      return n / std::sqrt(sq_norm); // normalize
    else
      return CGAL::NULL_VECTOR;
  }

  // cotan formula as area(voronoi face) / len(primal edge)
  FT cotan_geometric(Edge& edge)
  {
    Cell_handle cell = edge.first;
    Vertex_handle vi = cell->vertex(edge.second);
    Vertex_handle vj = cell->vertex(edge.third);

    // primal edge
    const Point& pi = vi->point();
    const Point& pj = vj->point();
    Vector primal = pj - pi;
    FT len_primal = std::sqrt(primal * primal);
    return area_voronoi_face(edge) / len_primal;
  }

 FT area_normal_ratio(Cell_handle cell, Edge& edge)
 {
      Vertex_handle vi = cell->vertex(edge.second);
      Vertex_handle vj = cell->vertex(edge.third);
      int index1 = cell->index(vi);
      int index2 = cell->index(vj);
	  Point& p_vi = cell->vertex(index1)->point();
	  Point& p_vj = cell->vertex(index2)->point();
	  Point& a = cell->vertex(index1)->point();
	  Point& c = cell->vertex(index1)->point();
	  if ((index1+1)%4 == index2)
	  {
	   a = cell->vertex((index1+2)%4)->point();
       c = cell->vertex((index1+3)%4)->point();
	  }
	  if ((index1+2)%4 == index2)
	  {
	   a = cell->vertex((index1+1)%4)->point();
       c = cell->vertex((index1+3)%4)->point();
	  }
	  if ((index1+3)%4 == index2)
	  {
	   a = cell->vertex((index1+1)%4)->point();
       c = cell->vertex((index1+2)%4)->point();
	  }
      Triangle face(p_vi,a,c);
      FT area = std::sqrt(face.squared_area());

      Vector x = p_vj - a;
      Vector n1 = CGAL::cross_product(a-c,p_vi-c);
      FT sq_norm1  = std::sqrt(n1*n1);
      FT normal = std::abs((x*n1)/sq_norm1);

      Vector n2 = CGAL::cross_product(a-c,p_vj-c);
      FT sq_norm2  = std::sqrt(n2*n2);
      FT cos = std::abs(n1 * n2 / (sq_norm1 * sq_norm2));


      return (area * cos / normal);
 }



 FT cotan_FEM(Edge& edge)
  {
     FT answer = 0.0;
     Cell_circulator circ = m_dt.incident_cells(edge);
     Cell_circulator done = circ;
     do
     {
      Cell_handle cell = circ;
      if(!m_dt.is_infinite(cell))
       {
          answer = answer + area_normal_ratio(cell,edge);
       }
      else return cotan_geometric(edge);
     }
     while ( circ != done);
    return answer;
  }

  // spin around edge
  // return area(voronoi face)
  FT area_voronoi_face(Edge& edge)
  {
    // circulate around edge
    Cell_circulator circ = m_dt.incident_cells(edge);
    Cell_circulator done = circ;
    std::vector<Point> voronoi_points;
    do
    {
      Cell_handle cell = circ;
      if(!m_dt.is_infinite(cell))
        voronoi_points.push_back(m_dt.dual(cell));
      else // one infinite tet, switch to another calculation
        return area_voronoi_face_boundary(edge);
      circ++;
    }
    while(circ != done);

    if(voronoi_points.size() < 3)
    {
      CGAL_surface_reconstruction_assertion(false);
      return 0.0;
    }

    // sum up areas
    FT area = 0.0;
    const Point& a = voronoi_points[0];
    unsigned int nb_triangles = voronoi_points.size() - 2;
    for(unsigned int i=1;i<nb_triangles;i++)
    {
      const Point& b = voronoi_points[i];
      const Point& c = voronoi_points[i+1];
      Triangle triangle(a,b,c);
      area += std::sqrt(triangle.squared_area());
    }
    return area;
  }

  // approximate area when a cell is infinite
  FT area_voronoi_face_boundary(Edge& edge)
  {
    FT area = 0.0;
    Vertex_handle vi = edge.first->vertex(edge.second);
    Vertex_handle vj = edge.first->vertex(edge.third);

    const Point& pi = vi->point();
    const Point& pj = vj->point();
    Point m = CGAL::midpoint(pi,pj);

    // circulate around each incident cell
    Cell_circulator circ = m_dt.incident_cells(edge);
    Cell_circulator done = circ;
    do
    {
      Cell_handle cell = circ;
      if(!m_dt.is_infinite(cell))
      {
        // circumcenter of cell
        Point c = m_dt.dual(cell);
        Tetrahedron tet = m_dt.tetrahedron(cell);

        int i = cell->index(vi);
        int j = cell->index(vj);
        int k = -1, l = -1;
        other_two_indices(i,j, &k,&l);
        Vertex_handle vk = cell->vertex(k);
        Vertex_handle vl = cell->vertex(l);

        const Point& pk = vk->point();
        const Point& pl = vl->point();

        // if circumcenter is outside tet
        // pick barycenter instead
        if(tet.has_on_unbounded_side(c))
        {
          Point cell_points[4] = {pi,pj,pk,pl};
          c = CGAL::centroid(cell_points, cell_points+4);
        }

        Point ck = CGAL::circumcenter(pi,pj,pk);
        Point cl = CGAL::circumcenter(pi,pj,pl);

        Triangle mcck(m,m,ck);
        Triangle mccl(m,m,cl);

        area += std::sqrt(mcck.squared_area());
        area += std::sqrt(mccl.squared_area());
      }
      circ++;
    }
    while(circ != done);
    return area;
  }

  // Get indices different from i and j
  void other_two_indices(int i, int j, int* k, int* l)
  {
    CGAL_surface_reconstruction_assertion(i != j);
    bool k_done = false;
    bool l_done = false;
    for(int index=0;index<4;index++)
    {
      if(index != i && index != j)
      {
        if(!k_done)
        {
          *k = index;
          k_done = true;
        }
        else
        {
          *l = index;
          l_done = true;
        }
      }
    }
    CGAL_surface_reconstruction_assertion(k_done);
    CGAL_surface_reconstruction_assertion(l_done);
  }

  void assemble_poisson_row(Taucs_solver& solver,
                            Vertex_handle vi,
                            std::vector<double>& B,
                            double lambda)
  {
    // assemble new row
    solver.begin_row();

    // for each vertex vj neighbor of vi
    std::list<Vertex_handle> vertices;
    m_dt.incident_vertices(vi,std::back_inserter(vertices));
    double diagonal = 0.0;
    for(typename std::list<Vertex_handle>::iterator it = vertices.begin();
        it != vertices.end();
        it++)
    {
      Vertex_handle vj = *it;
      if(m_dt.is_infinite(vj))
        continue;

      // get corresponding edge
      Edge edge = sorted_edge(vi,vj);

     // double cij = cotan_FEM(edge);
      double cij = cotan_geometric(edge);
      if(vj->constrained())
        B[vi->index()] -= cij * vj->f(); // change rhs
      else
        // off-diagonal coefficient
        solver.add_value(vj->index(),-cij);

      diagonal += cij;
    }

    // diagonal coefficient
    if (vi->type() == Triangulation::INPUT)
      solver.add_value(vi->index(),diagonal + lambda) ;
    else
      solver.add_value(vi->index(),diagonal);

    // end matrix row
    solver.end_row();
  }

  Edge sorted_edge(Vertex_handle vi,
                   Vertex_handle vj)
  {
    int i1 = 0;
    int i2 = 0;
    Cell_handle cell = NULL;
    bool success;
    if(vi->index() > vj->index())
      success = m_dt.is_edge(vi,vj,cell,i1,i2);
    else
      success = m_dt.is_edge(vj,vi,cell,i1,i2);
    CGAL_surface_reconstruction_assertion(success);
    return Edge(cell,i1,i2);
  }

  /// Compute enlarged geometric bounding box of the embedded triangulation.
  Iso_cuboid enlarged_bounding_box(FT ratio) const
  {
    // Get triangulation's bounding box
    Iso_cuboid bbox = bounding_box();

    // Its center point is:
    FT mx = 0.5 * (bbox.xmax() + bbox.xmin());
    FT my = 0.5 * (bbox.ymax() + bbox.ymin());
    FT mz = 0.5 * (bbox.zmax() + bbox.zmin());
    Point c(mx,my,mz);

    // Compute enlarged bounding box
    FT sx = 0.5 * ratio * (bbox.xmax() - bbox.xmin());
    FT sy = 0.5 * ratio * (bbox.ymax() - bbox.ymin());
    FT sz = 0.5 * ratio * (bbox.zmax() - bbox.zmin());
    Point p(c.x() - sx, c.y() - sy, c.z() - sz);
    Point q(c.x() + sx, c.y() + sy, c.z() + sz);
    return Iso_cuboid(p,q);
  }

  /// Compute enlarged geometric bounding sphere of the embedded triangulation.
  Sphere enlarged_bounding_sphere(FT ratio) const
  {
    Sphere bbox = bounding_sphere(); // triangulation's bounding sphere
    return Sphere(bbox.center(), bbox.squared_radius() * ratio*ratio);
  }

  void init_nn_search_shell()
  {
    // Instanciate a KD-tree search.
    // We have to wrap each input vertex by a Point_vertex_handle_3.
    std::list<Point_vertex_handle_3> kvertices;
    for(Finite_vertices_iterator v = m_dt.finite_vertices_begin();
      v != m_dt.finite_vertices_end();
      v++)
    {
      if(v->type() != Triangulation::INPUT)
        continue;
      const Point& p = v->point();
      Point_vertex_handle_3 kv(p.x(),p.y(),p.z(),v);
      kvertices.push_back(kv);
    }
    m_nn_search = K_nearest_neighbor(kvertices.begin(), kvertices.end());
  }

  bool is_refinable(Cell_handle cell,
                    const FT size_shell,
                    const FT sizing,
                    FT& size,
                    Point& p)
  {
    size = circumradius(cell);
    if(size <= sizing)
      return false;

    // try circumcenter
    p = m_dt.dual(cell);
    if(distance_to_input_points(p) < size_shell)
      return true;

    return false;
  }

  FT distance_to_input_points(const Point& p)
  {
    // Get nearest neighbour
    std::list<Vertex_handle> nearest_vertices;
    m_nn_search.get_k_nearest_neighbors(p,1,nearest_vertices);
    Vertex_handle nv = *nearest_vertices.begin();
    if(nv != NULL)
      return distance(nv->point(),p);
    else
      return 0.0; // default
  }

  FT distance(const Point& a, const Point& b) const
  {
    return std::sqrt(CGAL::squared_distance(a,b));
  }

  FT circumradius(Cell_handle c) const
  {
    Point center = m_dt.dual(c);
    const Point& p = c->vertex(0)->point();
    return std::sqrt((p-center)*((p-center)));
  }

}; // end of Poisson_implicit_function


CGAL_END_NAMESPACE

#endif // CGAL_POISSON_IMPLICIT_FUNCTION_H
