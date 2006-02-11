// ======================================================================
//
// Copyright (c) 2005 by Le-Jeng Shiue.  All Rights Reserved.
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
// $Source$
// $Revision$ $Date$
// $Name$
//
// Author(s): Le-Jeng Shiue <sle-jeng@cise.ufl.edu>
//                          <SurfLab, CISE, University of Florida>
//
// ======================================================================

#ifndef _POLYHEDRON_SUBDIVISION_H_01292002
#define _POLYHEDRON_SUBDIVISION_H_01292002

#include <CGAL/basic.h>

#include <vector>

#include <CGAL/circulator.h>

#include <CGAL/Polyhedron_decorator_3.h>
#include <CGAL/Subdivision_surfaces_masks_3.h>

CGAL_BEGIN_NAMESPACE

// ======================================================================
//
template <class Poly>
class Subdivision_method_3 : public Polyhedron_decorator_3<Poly> {
  typedef Poly                                        Polyhedron;

  typedef typename Polyhedron::Halfedge_data_structure HDS;
  typedef typename Polyhedron::Vertex                  Vertex;
  typedef typename Polyhedron::Halfedge                Halfedge;
  typedef typename Polyhedron::Facet                   Facet;
  
  typedef typename Polyhedron::Vertex_handle           Vertex_handle;
  typedef typename Polyhedron::Halfedge_handle         Halfedge_handle;
  typedef typename Polyhedron::Facet_handle            Facet_handle;

  typedef typename Polyhedron::Vertex_iterator         Vertex_iterator;
  typedef typename Polyhedron::Halfedge_iterator       Halfedge_iterator;
  typedef typename Polyhedron::Edge_iterator           Edge_iterator;
  typedef typename Polyhedron::Facet_iterator          Facet_iterator;

  typedef typename Polyhedron::Halfedge_around_facet_circulator  
                                            Halfedge_around_facet_circulator;
  typedef typename Polyhedron::Halfedge_around_vertex_circulator 
                                            Halfedge_around_vertex_circulator;

  typedef typename Polyhedron::Traits                  Traits;
  typedef typename Traits::Kernel                      Kernel;

  typedef typename Kernel::FT                          FT;
  typedef typename Kernel::Point_3                     Point;
  typedef typename Kernel::Vector_3                    Vector;

public:
  //
  static void CatmullClark_subdivision(Polyhedron& p, int step = 1) {
    PQQ(p, CatmullClark_mask_3<Polyhedron>(), step);
  }
  //static void PQQ_subdivision(Polyhedron& p, int step = 1) {
  //  PQQ(p, Linear_mask_3<Polyhedron>(), step);
  //}

  //
  static void Loop_subdivision(Polyhedron& p, int step = 1) {
    PTQ(p, Loop_mask_3<Polyhedron>() , step);
  }
  //static void PTQ_subdivision(Polyhedron& p, int step = 1) {
  //  PTQ(p, Linear_mask_3<Polyhedron>(), step);
  //}

  //
  static void DooSabin_subdivision(Polyhedron& p, int step = 1) {
    DQQ(p, DooSabin_mask_3<Polyhedron>(), step);
  }

  //
  static void Sqrt3_subdivision(Polyhedron& p, int step = 1) {
    Sqrt3(p, Sqrt3_mask_3<Polyhedron>(), step);
  }

public:
  // Stencil is the stencil policy
  template <template <typename> class Stencil>
  static void PQQ(Polyhedron& p, Stencil<Polyhedron> rule, int step = 1) {
    for (int i = 0; i < step; i++) PQQ_1step(p, rule);
  }
  // Stencil is the stencil policy
  template <template <typename> class Stencil>
  static void PTQ(Polyhedron& p, Stencil<Polyhedron> rule, int step = 1) {
    for (int i = 0; i < step; i++) PTQ_1step(p, rule);
  }

  // Stencil is the stencil policy
  template <template <typename> class Stencil>
  static void DQQ(Polyhedron& p, Stencil<Polyhedron> rule, int step = 1) {
    for (int i = 0; i < step; ++i) DQQ_1step(p, rule);
  }

  // Stencil is the stencil policy
  template <template <typename> class Stencil>
  static void Sqrt3(Polyhedron& p, Stencil<Polyhedron> rule, int step = 1) {
    for (int i = 0; i < step; i++) Sqrt3_1step(p, rule);
  }

protected:
  //
  template <template <typename> class Stencil>
  static void PQQ_1step(Polyhedron& p, Stencil<Polyhedron> rule);
  //
  template <template <typename> class Stencil>
  static void PTQ_1step(Polyhedron& p, Stencil<Polyhedron> rule);
  //
  template <template <typename> class Stencil>
  static void DQQ_1step(Polyhedron& p, Stencil<Polyhedron> rule);
  //
  template <template <typename> class Stencil>
  static void Sqrt3_1step(Polyhedron& p, Stencil<Polyhedron> rule);
};


// ======================================================================
//
template <class Poly> template <template <typename> class Stencil>
void Subdivision_method_3<Poly>::PQQ_1step(Poly& p, Stencil<Poly> rule) {
  p.normalize_border();

  // Build a new vertices buffer has the following structure
  //
  // 0 1 ... e_begin ... f_begin ... (end_of_buffer)
  // 0 ... e_begin-1       : store the positions of the vertex-vertices
  // e_begin ... f_begin-1 : store the positions of the edge-vertices
  // f_begin ... (end)     : store the positions of the face-vertices
  // The index of the vertices buffer should 1-1 map to the distance
  // of the corresponding iterator to the begin of the iterator.
  size_t num_vertex = p.size_of_vertices();
  size_t num_edge = p.size_of_halfedges()/2;
  size_t num_facet = p.size_of_facets();

  // If Polyhedron is using vector, we need to reserve the memory to prevent 
  // the CGAL_assertion.
  // This function for polyhedron using list is VOID.
  p.reserve(num_vertex+num_edge+num_facet, 4*2*num_edge, 4*num_edge/2);

  Point* vertex_point_buffer = new Point[num_vertex + num_edge + num_facet];
  Point* edge_point_buffer = vertex_point_buffer + num_vertex;
  Point* face_point_buffer = edge_point_buffer + num_edge;

  std::vector<bool> v_onborder(num_vertex);

  Facet_iterator fitr = p.facets_begin();
  for (size_t i = 0; i < num_facet; i++, ++fitr)
    rule.facet_node(fitr, face_point_buffer[i]);

  size_t sb = p.size_of_border_edges();

  Edge_iterator eitr = p.edges_begin();
  for (size_t i = 0; i < num_edge-sb; i++, ++eitr)
    rule.edge_node(eitr, edge_point_buffer[i]);
  for (size_t i = num_edge-sb; i < num_edge; i++, ++eitr) {
    int v = (int) std::distance(p.vertices_begin(), eitr->vertex());
    v_onborder[v] = true;
    rule.border_node(eitr, edge_point_buffer[i], vertex_point_buffer[v]);
  }

  Vertex_iterator vitr = p.vertices_begin();
  for (size_t i = 0; i < num_vertex; i++, ++vitr)
    if (!v_onborder[i]) rule.vertex_node(vitr, vertex_point_buffer[i]);

  // Build the connectifty using insert_vertex() and insert_edge()
  // 1. insert_vertex() to all edges and set them to new positions
  // 2. insert_edge() between 2 randomly selected neighboring new inserted 
  //    vertices
  // 3. insert_vertex() to the new inserted edge and set them to new positions
  // 4. insert_edge() between all other new inserted vetices of step 1 and
  //    the new inserted vertex of step 3
  // Step 1.
  eitr = p.edges_begin();
  for (size_t i = 0; i < num_edge; i++, ++eitr) {
    Vertex_handle vh = insert_vertex(p, eitr);
    vh->point() = edge_point_buffer[i];
  }
  fitr = p.facets_begin();

  // TODO: the topoloy modification can be done by a template function
  //       and that gives the user a chance to create new topoly rules.
  for (size_t i = 0; i < num_facet; i++, ++fitr) {
    // Step 2.
    Halfedge_around_facet_circulator hcir_begin = fitr->facet_begin();
    Halfedge_around_facet_circulator hcir = hcir_begin;
    
    Halfedge_handle e1 = ++hcir; // e1 points to the newly inserted vertex
    ++hcir; // Skips one original vertex 
    Halfedge_handle e2 = ++hcir; // points to the next newly inserted vertex
    ++hcir; // Must move the cir before inserts the new edge !!
    Halfedge_handle newe = insert_edge(p, e1, e2);
    
    // Step 3.
    Halfedge_handle newv = insert_vertex_return_edge(p, newe);
    newv = newv->opposite()->prev(); // change newv to the larger face and 
                                     // still points to the newly inserted 
                                     // vertex
    // Update the geometry data of the newly inserted face-vertices
    newv->vertex()->point() = face_point_buffer[i];
   
    // Step 4.
    while (hcir != hcir_begin) {
      e1 = ++hcir;
      ++hcir; // Must move the cir before inserts the new edge !!
      insert_edge(p, e1, newv); 
    }
  }
  
  // Update the geometry data of the newly inserted vertices by the 
  // vertices buffer
  vitr = p.vertices_begin();
  for (size_t i = 0; i < num_vertex; i++, ++vitr) 
    vitr->point() = vertex_point_buffer[i];

  delete []vertex_point_buffer;
}

// ======================================================================
//
template <class Poly> template <template <typename> class Stencil>
void Subdivision_method_3<Poly>::PTQ_1step(Poly& p, Stencil<Poly> rule) {
  p.normalize_border();

  // Build a new vertices buffer has the following structure
  //
  // 0 1 ... e_begin ... f_begin ... (end_of_buffer)
  // 0 ... e_begin-1       : store the positions of the vertex-vertices
  // e_begin ... (end)     : store the positions of the edge-vertices
  // The index of the vertices buffer should 1-1 map to the distance
  // of the corresponding iterator to the begin of the iterator.
  size_t num_vertex = p.size_of_vertices();
  size_t num_edge = p.size_of_halfedges()/2;
  size_t num_facet = p.size_of_facets();

  // If Polyhedron is using vector, we need to reserve the memory to prevent 
  // the CGAL_assertion.
  // This function for polyhedron using list is VOID.
  p.reserve(num_vertex+num_edge, 2*2*num_edge, 4*num_edge/2);

  Point* vertex_point_buffer = new Point[num_vertex + num_edge];
  Point* edge_point_buffer = vertex_point_buffer + num_vertex;

  std::vector<bool> v_onborder(num_vertex);
  size_t sb = p.size_of_border_edges();

  Edge_iterator eitr = p.edges_begin();
  for (size_t i = 0; i < num_edge-sb; i++, ++eitr)
    rule.edge_node(eitr, edge_point_buffer[i]);
  for (size_t i = num_edge-sb; i < num_edge; i++, ++eitr) {
    int v = (int) std::distance(p.vertices_begin(), eitr->vertex());
    v_onborder[v] = true;
    rule.border_node(eitr, edge_point_buffer[i], vertex_point_buffer[v]);
  }

  Vertex_iterator vitr = p.vertices_begin();
  for (size_t i = 0; i < num_vertex; i++, ++vitr)
    if (!v_onborder[i]) rule.vertex_node(vitr, vertex_point_buffer[i]);

  // Build the connectifty using insert_vertex() and insert_edge()
  // 1. insert_vertex() to all edges and set them to new positions
  // 2. insert_edge() between 2 randomly selected neighboring new inserted 
  //    vertices
  // 3. insert_vertex() to the new inserted edge and set them to new positions
  // 4. insert_edge() between all other new inserted vetices of step 1 and
  //    the new inserted vertex of step 3
  // Step 1.
  eitr = p.edges_begin();
  for (size_t i = 0; i < num_edge; i++, ++eitr) {
    Vertex_handle vh = insert_vertex(p, eitr);
    vh->point() = edge_point_buffer[i];
  }
  Facet_iterator fitr = p.facets_begin();
  for (size_t i = 0; i < num_facet; i++, ++fitr) {
    // Step 2.
    Halfedge_around_facet_circulator hcir_begin = fitr->facet_begin();
    Halfedge_around_facet_circulator hcir = hcir_begin;
    
    // After linsub, the facet valence = 6
    CGAL_assertion(circulator_size(hcir)==6);
    
    Halfedge_handle e1 = ++hcir;
    ++hcir;
    Halfedge_handle e2 = ++hcir;
    ++hcir;
    Halfedge_handle e3 = ++hcir;
    e2 = insert_edge(p, e1, e2);
    e3 = insert_edge(p, e2, e3);
    insert_edge(p, e3, e1);
  }

  // Update the geometry data of the newly inserted vertices by the 
  // vertices buffer
  vitr = p.vertices_begin();
  for (size_t i = 0; i < num_vertex; i++, ++vitr)
    vitr->point() = vertex_point_buffer[i];

  delete []vertex_point_buffer;
}


// ======================================================================
//
//#define _EULER_DQQ_SPLITTING
#define _EULER_DQQ_TILTING   // Tilting is faster
template <class Poly> template <template <typename> class Stencil>
void Subdivision_method_3<Poly>::DQQ_1step(Poly& p, Stencil<Poly> rule) {
  p.normalize_border();

  size_t num_v = p.size_of_vertices();
  size_t num_e = p.size_of_halfedges()/2;
  size_t num_f = p.size_of_facets();

	size_t num_be = p.size_of_border_edges();

  Point* point_buffer = new Point[num_e*2];

  //
#ifdef _EULER_DQQ_SPLITTING
  //
  // Splitting

	// Splitting is not imlemented to support border

  // build the point_buffer
  Facet_iterator fitr, fitr_end = p.facets_end();
  int pi = 0;
  for (fitr = p.facets_begin(); fitr != fitr_end; ++fitr) {
    Halfedge_around_facet_circulator cir = fitr->facet_begin();
    do {
      rule.corner_node(cir, point_buffer[pi++]);
    } while (--cir != fitr->facet_begin());
  }
  
  // If Polyhedron is using vector, we need to reserve the memory to prevent 
  // the CGAL_assertion. This function for polyhedron using list is VOID.
  p.reserve(num_v+num_e+num_f, 2*num_e, (2+4+2)*num_e);

  // Build the connectifty using insert_vertex() and insert_edge()
  // 1. create barycetric centers of each facet
  fitr = p.facets_begin();
  pi = 0;
  for (int i = 0; i < num_f; i++) {
    Facet_handle fh = fitr;
    ++fitr;
    Vertex_handle vh = (p.create_center_vertex(fh->facet_begin()))->vertex();

    // 1.1 add vertex on each new edges
    Halfedge_around_vertex_circulator vcir = vh->vertex_begin();  
    int vn = circulator_size(vcir);
    for (int j = 0; j < vn; ++j) {
      Halfedge_handle e = vcir;
      ++vcir;
      Vertex_handle v = insert_vertex(p, e);
      v->point() = point_buffer[pi++];
    }
    // 1.2 connect new vertices surround each barycentric center
    for (int j = 0; j < vn; ++j) {
      Halfedge_handle e1 = vcir->prev();
      ++vcir;
      Halfedge_handle e2 = vcir->opposite();
      insert_edge(p, e1, e2);
    }
    // 1.3 remove the barycentric centers
    p.erase_center_vertex(vcir);
  }

  // 2. remove old edges
  Edge_iterator eitr = p.edges_begin();
  for (int i = 0; i < num_e; ++i) {
    Halfedge_handle eh = eitr;
    ++eitr;
    p.join_facet(eh);
  }

  // 3. connect new vertices surround old vertices and then remove 
  //    old vertices.
  Vertex_iterator vitr = p.vertices_begin();
  for (int i = 0; i < num_v; ++i) {
    Halfedge_around_vertex_circulator vcir = vitr->vertex_begin();  
    int vn = circulator_size(vcir);
    for (int j = 0; j < vn; ++j) {
      Halfedge_handle e1 = vcir->prev();
      ++vcir;
      Halfedge_handle e2 = vcir->opposite();
      insert_edge(p, e1, e2);
    }
    ++vitr;
    p.erase_center_vertex(vcir);    
  }

  //
#else
  //
  // Tilting

  // build the point_buffer
  Vertex_iterator vitr, vitr_end = p.vertices_end();
  int pi = 0;
  for (vitr = p.vertices_begin(); vitr != vitr_end; ++vitr) {
    Halfedge_around_vertex_circulator cir = vitr->vertex_begin();
    do {
			if (!cir->is_border()) rule.corner_node(cir, point_buffer[pi++]);
    } while (++cir != vitr->vertex_begin());
  }
  
  // If Polyhedron is using vector, we need to reserve the memory to prevent 
  // the CGAL_assertion. This function for polyhedron using list is VOID.
  p.reserve(num_v+num_e+num_f, 2*num_e, (2+4+2)*num_e);

  // Build the connectifty using insert_vertex() and insert_edge()
  vitr = p.vertices_begin();
  pi = 0;
  for (size_t i = 0; i < num_v; ++i) {
    Vertex_handle vh = vitr;
    ++vitr;

    Halfedge_around_vertex_circulator vcir = vh->vertex_begin();  
    size_t vn = circulator_size(vcir);
    for (size_t j = 0; j < vn; ++j) {
      Halfedge_handle e = vcir;
      ++vcir;
			if (!e->is_border()) {
				Vertex_handle v = insert_vertex(p, e);
				v->point() = point_buffer[pi++];
			}
    }

    vcir = vh->vertex_begin();
		for (size_t j = 0; j < vn; ++j) {
			if (!vcir->is_border()) {
				Halfedge_handle e1 = vcir->prev();
				++vcir;
				if (!vcir->is_border()) {
					Halfedge_handle e2 = vcir->opposite();
					insert_edge(p, e1, e2);
				}
			} else ++vcir;
		}
    //p.erase_center_vertex(vh->vertex_begin());
  }

  Edge_iterator eitr = p.edges_begin();
  for (size_t i = 0; i < num_e; ++i) {
		Halfedge_handle eh = eitr;
		++eitr;
		if (!eh->is_border_edge()) {
			insert_edge(p, eh->prev()->prev(), eh);
			eh = eh->opposite();
			insert_edge(p, eh->prev()->prev(), eh);
			p.join_facet(eh);
		} else {
			if (eh->is_border()) {
				eh = eh->opposite();
				insert_edge(p, eh, eh->prev()->prev());
			} else 
				insert_edge(p, eh->prev()->prev(), eh);
		}
  }

	// after this point, the original border edges are in front!
	eitr = p.edges_begin();
	for (size_t i = 0; i < num_be; ++i) {
		Halfedge_handle eh = eitr;
		++eitr;

		if (eh->is_border()) eh = eh->opposite();
		Halfedge_handle ehe = eh;
		eh = eh->prev()->opposite();
		while (!eh->is_border()) {
			p.erase_facet(ehe);
			ehe = eh;
			eh = eh->prev()->opposite();
		}
		p.erase_facet(ehe);
	}

	vitr = p.vertices_begin();
	for (size_t i = 0; i < num_v-num_be; ++i) {
	  Vertex_handle vh = vitr;
    ++vitr;
    p.erase_center_vertex(vh->vertex_begin());
	}

#endif //_EULER_DQQ_SPLITTING

  delete []point_buffer;
}

// ======================================================================
//
template <class Poly> template <template <typename> class Stencil>
void Subdivision_method_3<Poly>::Sqrt3_1step(Poly& p, Stencil<Poly> rule) {
  //
  p.normalize_border();

  //
  size_t num_v = p.size_of_vertices();
  size_t num_e = p.size_of_halfedges()/2;
  size_t num_f = p.size_of_facets();

  p.reserve(num_v+num_f, (num_e+3*num_f)*2, 3*num_f);
 
  // prepare the smoothed center points
  Point* cpt = new Point[num_f]; 
  Facet_iterator fitr = p.facets_begin();
  for (size_t i = 0; i < num_f; ++i, ++fitr) {
    //ASSERTION_MSG(circulator_size(fitr->facet_begin())==3, "(ERROR) Non-triangle facet!");
    rule.facet_node(fitr, cpt[i]);
  }

  // smooth the vertex points
  Vertex_iterator vitr = p.vertices_begin();
  for (size_t i = 0; i < num_v; ++i, ++vitr)
    rule.vertex_node(vitr, vitr->point());
  
  // insert the facet points
  fitr = p.facets_begin();
  for (size_t i = 0; i < num_f; ++i, ++fitr) {
    Halfedge_handle center = p.create_center_vertex(fitr->halfedge());
    center->vertex()->point() = cpt[i];
  }
  
  delete []cpt;

  // flip the old edges except the border edges
  Edge_iterator eitr = p.edges_begin();
  for (size_t i = 0; i < num_e; ++i) {
    Halfedge_handle e = eitr;
    ++eitr; // move to next edge before flip since flip destroys current edge
    if (!e->is_border_edge()) {
      Halfedge_handle h = p.join_facet(e);
      p.split_facet(h->prev(), h->next());
    }
  }

  // TODO: border ...
  
  CGAL_postcondition(p.is_valid());
}

CGAL_END_NAMESPACE

#endif //_POLYHEDRON_SUBDIVISION_H_01292002
