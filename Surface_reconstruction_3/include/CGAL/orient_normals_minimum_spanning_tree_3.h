// Copyright (c) 2007-08  INRIA Sophia-Antipolis (France).
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
// Author(s) : Pierre Alliez and Laurent Saboret and Andreas Fabri

#ifndef CGAL_ORIENT_NORMALS_MINIMUM_SPANNING_TREE_3_H
#define CGAL_ORIENT_NORMALS_MINIMUM_SPANNING_TREE_3_H

#include <CGAL/basic.h>
#include <CGAL/Search_traits_3.h>
#include <CGAL/Orthogonal_k_neighbor_search.h>
#include <CGAL/Search_traits_vertex_handle_3.h>
#include <CGAL/Oriented_normal_3.h>
#include <CGAL/Memory_sizer.h>

#include <iterator>
#include <list>
#include <climits>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/prim_minimum_spanning_tree.hpp>

CGAL_BEGIN_NAMESPACE


// Traces?
//#define CGAL_TRACE  printf

#ifndef CGAL_TRACE
  #define CGAL_TRACE  if (false) printf
#endif


/// Helper function: distance_MST().
///
/// This function is used internally by orient_normals_minimum_spanning_tree_3()
/// to compute the distance between 2 integers or 2 iterators.
template <class T>
inline int
distance_MST(T _First, T _Last)
{
  // return distance between iterators
  return std::distance(_First, _Last);
}

template <>
inline int
distance_MST(std::size_t _First, std::size_t _Last)
{
  // return int difference
  return _Last - _First;
}


/// Helper class: Riemannian graph.
///
/// This class is used internally by orient_normals_minimum_spanning_tree_3()
/// to encode:
/// - the adjacency relations of vertices in a K-neighbouring,
/// - the edge weight = edge weight = 1 - | normal1 * normal2 |
///   where normal1 and normal2 are the normal at the edge extremities.
typedef boost::adjacency_list< boost::vecS, boost::vecS,
                               boost::undirectedS,
                               boost::no_property,
                               boost::property<boost::edge_weight_t, float> >
    Riemannian_graph;


/// Helper class: MST graph
///
/// This class is used internally by orient_normals_minimum_spanning_tree_3()
/// to encode the adjacency relations of vertices in a Minimum Spanning Tree.
template <class VertexIterator>
struct MST_graph_vertex_properties {
    VertexIterator vertex; ///< Input vertex
};
template <class VertexIterator,     ///< Input vertex iterator
          class VertexNormalMap>    ///< property map VertexIterator -> Normal (in and out)
class MST_graph
  : public boost::adjacency_list< boost::vecS, boost::vecS,
                                  boost::directedS,
                                  MST_graph_vertex_properties<VertexIterator> >
{
public:
    MST_graph(VertexNormalMap vertex_normal_map) : m_vertex_normal_map(vertex_normal_map) {}

// Public data
    const VertexNormalMap m_vertex_normal_map;
};


/// Helper class: type of the "vertex_normal" property map
/// of a MST graph.
template <class VertexIterator,     ///< Input vertex iterator
          class VertexNormalMap>    ///< property map VertexIterator -> Normal (in and out)
class MST_graph_vertex_normal_map
  : public boost::put_get_helper< typename boost::property_traits<VertexNormalMap>::value_type&,
                                  MST_graph_vertex_normal_map<VertexIterator, VertexNormalMap> >
{
public:
    typedef CGAL::MST_graph<VertexIterator, VertexNormalMap> MST_graph;
    typedef typename boost::property_traits<VertexNormalMap>::value_type Normal;
    typedef typename MST_graph::vertex_descriptor vertex_descriptor;

    // Property maps required types
    typedef boost::lvalue_property_map_tag              category;
    typedef Normal                                      value_type;
    typedef Normal&                                     reference;
    typedef vertex_descriptor                           key_type;

    MST_graph_vertex_normal_map(const MST_graph& graph)
      : m_graph(graph) {}

    /// Access the map elements.
    reference operator[](key_type graph_vertex) const
    {
        VertexIterator input_vertex = m_graph[graph_vertex].vertex;
        Normal& normal = m_graph.m_vertex_normal_map[input_vertex];
        return normal;
    }

private:
    const MST_graph& m_graph;
};

/// Free function to get the "vertex_normal" property map
/// of a MST graph.
template <class VertexIterator, class VertexNormalMap>
inline
MST_graph_vertex_normal_map<VertexIterator, VertexNormalMap>
get(boost::vertex_normal_t, const MST_graph<VertexIterator, VertexNormalMap>& graph)
{
  MST_graph_vertex_normal_map<VertexIterator, VertexNormalMap> aMap(graph);
  return aMap;
}


/// Helper class: propagate_normal
///
/// This class is used internally by orient_normals_minimum_spanning_tree_3()
/// to propage the normals orientation, starting from a source vertex
/// and following the adjacency relations of vertices in a Minimum Spanning Tree.
template <class VertexIterator,     ///< Input vertex iterator
          class VertexNormalMap>    ///< property map VertexIterator -> Normal (in and out)
struct propagate_normal
  : public boost::base_visitor< propagate_normal<VertexIterator, VertexNormalMap> >
{
    typedef CGAL::MST_graph<VertexIterator, VertexNormalMap> MST_graph;
    typedef boost::on_examine_edge event_filter;

    template <class Edge>
    void operator()(const Edge& edge, const MST_graph& graph)
    {
        typedef typename boost::property_traits<VertexNormalMap>::value_type Normal;
        typedef typename Normal::Vector Vector;
        typedef typename MST_graph::vertex_descriptor vertex_descriptor;

        // Get source normal
        vertex_descriptor vtx1 = boost::source(edge, graph);
        Normal& normal1 = get(get(boost::vertex_normal, graph), vtx1);
        Vector vec1 = normal1.get_vector();

        // Get target normal
        vertex_descriptor vtx2 = boost::target(edge, graph);
        Normal& normal2 = get(get(boost::vertex_normal, graph), vtx2);
        Vector vec2 = normal2.get_vector();

        double dot = vec1 * vec2;

        //CGAL_TRACE("    %d (%1.3lf,%1.3lf,%1.3lf) -> %d (%1.3lf,%1.3lf,%1.3lf): dot=%1.3lf\n",
        //           (int)vtx1, vec1.x(),vec1.y(),vec1.z(),
        //           (int)vtx2, vec2.x(),vec2.y(),vec2.z(),
        //           dot);

        //           ->                 ->
        // Orient normal2 parallel to normal1
        if (dot < 0) {
            //CGAL_TRACE("    flip %d\n", (int)vtx2);
            vec2 = -vec2;
        }

        // Is orientation robust?
        //normal2 = Normal(vec2, true /* oriented */);
        bool oriented = normal1.is_oriented() &&
                        (std::abs(dot) > 1.0/std::sqrt(2.0)); // oriented iff angle < 45�
        normal2 = Normal(vec2, oriented);
    }
};


/// Orient the normals of a point set using the method described in
/// "Hoppe, DeRose, Duchamp, McDonald, Stuetzle,
/// Surface reconstruction from unorganized points,
/// ACM SIGGRAPH Computer Graphics, v.26 n.2, p.71-78, July 1992".
///
/// Preconditions:
/// - VertexIterator is a model of ForwardIterator.
/// - VertexIndexMap is a model of boost::readable_property_map.
/// - VertexPointMap is a model of boost::readable_property_map.
/// - VertexNormalMap is a model of boost::lvalue_property_map.
/// - Normals must be unit vectors.
/// - KNN >= 2.

template<class VertexIterator, class VertexPointMap, class VertexIndexMap, class VertexNormalMap>
void
orient_normals_minimum_spanning_tree_3(VertexIterator first, ///< first input vertex
                                       VertexIterator beyond, ///< past-the-end input vertex
                                       VertexIndexMap vertex_index_map, ///< property map VertexIterator -> index
                                       VertexPointMap vertex_point_map, ///< property map VertexIterator -> Point_3
                                       VertexNormalMap vertex_normal_map, ///< property map VertexIterator -> Normal (in and out)
                                       unsigned int KNN) ///< number of neighbors
{
    CGAL_TRACE("Call orient_normals_minimum_spanning_tree_3()\n");

    // Input mesh's types
    typedef typename boost::property_traits<VertexPointMap>::value_type Point;
    typedef typename boost::property_traits<VertexNormalMap>::value_type Normal;
    typedef typename Normal::Vector Vector;

    // Types for K nearest neighbors search structure
    typedef Point_vertex_handle_3<VertexIterator> Point_vertex_handle_3;
    typedef Search_traits_vertex_handle_3<VertexIterator> Traits;
    typedef Euclidean_distance_vertex_handle_3<VertexIterator> KDistance;
    typedef Orthogonal_k_neighbor_search<Traits,KDistance> Neighbor_search;
    typedef typename Neighbor_search::Tree Tree;
    typedef typename Neighbor_search::iterator Search_iterator;

    // Riemannian_graph types
    typedef boost::property_map<Riemannian_graph, boost::edge_weight_t>::type Riemannian_graph_weight_map;

    // MST_graph types
    typedef MST_graph<VertexIterator, VertexNormalMap> MST_graph;

    // Precondition: at least one element in the container.
    CGAL_surface_reconstruction_precondition(first != beyond);

    // Precondition: at least 2 nearest neighbors
    CGAL_surface_reconstruction_precondition(KNN >= 2);

    // Number of input vertices
    const int num_input_vertices = distance_MST(first, beyond);

    // Orient source normal: the normal of the vertex
    // with maximum Z is oriented towards +Z axis.
    //
    // Find vertex with maximum Z
    double z_max = -1e30;
    VertexIterator source_vertex = first; // 'first' initial value is just to workaround a gcc warning
    for (VertexIterator it = first; it != beyond; it++)
    {
        double z = get(vertex_point_map,it).z();
        if (z_max < z) {
            z_max = z;
            source_vertex = it;
        }
    }
    unsigned int source_vertex_index = get(vertex_index_map, source_vertex);
    //
    // Orient its normal towards +Z axis
    const Vector Z(0, 0, 1);
    Normal& normal = vertex_normal_map[source_vertex];
    Vector vec = normal.get_vector();
    if (Z * vec < 0) {
        //CGAL_TRACE("  Flip source vertex %d\n", (int)source_vertex_index);
        vec = -vec;
    }
    normal = Normal(vec, true /* oriented */);

    long memory = CGAL::Memory_sizer().virtual_size(); CGAL_TRACE("  %ld Mb allocated\n", memory>>20);
    CGAL_TRACE("  Create KD-tree\n");

    // Instanciate a KD-tree search.
    // Notes: We have to wrap each input vertex by a Point_vertex_handle_3.
    //        The KD-tree is allocated dynamically to recover RAM as soon as possible.
    std::vector<Point_vertex_handle_3> kd_tree_points;
    for (VertexIterator it = first; it != beyond; it++)
    {
        Point point = get(vertex_point_map, it);
        Point_vertex_handle_3 point_wrapper(point.x(), point.y(), point.z(), it);
        kd_tree_points.push_back(point_wrapper);
    }
    std::auto_ptr<Tree> tree( new Tree(kd_tree_points.begin(), kd_tree_points.end()) );

    // Recover RAM
    kd_tree_points.clear();

    /*long*/ memory = CGAL::Memory_sizer().virtual_size(); CGAL_TRACE("  %ld Mb allocated\n", memory>>20);
    CGAL_TRACE("  Create Riemannian Graph\n");

    // Iterate over input points and create Riemannian Graph:
    // - vertices are numbered like the input vertices' index.
    // - vertices are empty.
    // - we add the edge (i, j) if either vertex i is in the KNN-neighborhood of vertex j,
    //   or vertex j is in the KNN-neighborhood of vertex i.
    Riemannian_graph riemannian_graph(num_input_vertices);
    Riemannian_graph_weight_map riemannian_graph_weight_map = get(boost::edge_weight, riemannian_graph);
    //
    // add edges
    for (VertexIterator it = first; it != beyond; it++)
    {
        unsigned int it_index = get(vertex_index_map,it);
        Vector it_normal_vector = vertex_normal_map[it].get_vector();

        // Gather set of (KNN+1) neighboring points.
        // Perform KNN+1 queries (as in point set, the query point is
        // output first). Search may be aborted when KNN is greater
        // than number of input points.
        Point point = get(vertex_point_map, it);
        Point_vertex_handle_3 point_wrapper(point.x(), point.y(), point.z(), it);
        Neighbor_search search(*tree, point_wrapper, KNN+1);
        Search_iterator search_iterator = search.begin();
        for(unsigned int i=0;i<(KNN+1);i++)
        {
            if(search_iterator == search.end())
                break; // premature ending

            VertexIterator neighbour = search_iterator->first;
            unsigned int neighbour_index = get(vertex_index_map,neighbour);
            if (neighbour_index > it_index) // undirected graph
            {
                // Add edge
                boost::graph_traits<Riemannian_graph>::edge_descriptor e;
                bool inserted;
                boost::tie(e, inserted) = boost::add_edge(boost::vertex(it_index, riemannian_graph),
                                                          boost::vertex(neighbour_index, riemannian_graph),
                                                          riemannian_graph);
                CGAL_surface_reconstruction_assertion(inserted);

                //                               ->        ->
                // Compute edge weight = 1 - | normal1 * normal2 |
                // where normal1 and normal2 are the normal at the edge extremities.
                Vector neighbour_normal_vector = vertex_normal_map[neighbour].get_vector();
                double weight = 1.0 - std::abs(it_normal_vector * neighbour_normal_vector);
                if (weight < 0)
                    weight = 0; // safety check
                //CGAL_TRACE("    %d (%1.3lf,%1.3lf,%1.3lf) -> %d (%1.3lf,%1.3lf,%1.3lf): weight=%1.3lf\n",
                //           (int)it_index, it_normal_vector.x(),it_normal_vector.y(),it_normal_vector.z(),
                //           (int)neighbour_index, neighbour_normal_vector.x(),neighbour_normal_vector.y(),neighbour_normal_vector.z(),
                //           weight);
                riemannian_graph_weight_map[e] = (float)weight;
            }

            search_iterator++;
        }
    }

    // Recover RAM
    tree.reset();

    /*long*/ memory = CGAL::Memory_sizer().virtual_size(); CGAL_TRACE("  %ld Mb allocated\n", memory>>20);
    CGAL_TRACE("  Call boost::prim_minimum_spanning_tree()\n");

    // Compute Minimum Spanning Tree.
    typedef std::vector<Riemannian_graph::vertex_descriptor> PredecessorMap;
    PredecessorMap predecessor(num_input_vertices);
    boost::prim_minimum_spanning_tree(riemannian_graph, &predecessor[0],
                                      weight_map( riemannian_graph_weight_map )
                                     .root_vertex( boost::vertex(source_vertex_index, riemannian_graph) ));

    // Recover RAM
    riemannian_graph.clear();

    /*long*/ memory = CGAL::Memory_sizer().virtual_size(); CGAL_TRACE("  %ld Mb allocated\n", memory>>20);
    CGAL_TRACE("  Create MST Graph\n");

    // Convert predecessor map to a MST graph:
    // - vertices are numbered like the input vertices' index.
    // - vertices contain the corresponding input vertex handle.
    // - we add the edge (predecessor[i], i) for each element of the predecessor map.
    MST_graph mst_graph(vertex_normal_map);
    //
    // add vertices
    for (VertexIterator it = first; it != beyond; it++)
    {
        typename MST_graph::vertex_descriptor v = add_vertex(mst_graph);
        CGAL_surface_reconstruction_assertion(v == get(vertex_index_map,it));
        mst_graph[v].vertex = it;
    }
    // add edges
    for (unsigned int i=0; i < predecessor.size(); i++) // add edges
    {
        if (i != predecessor[i])
        {
            // check that bi-directed graph is useless
            CGAL_surface_reconstruction_assertion(predecessor[predecessor[i]] != i);

            boost::add_edge(boost::vertex(predecessor[i], mst_graph),
                            boost::vertex(i,     mst_graph),
                            mst_graph);
        }
    }

    // Recover RAM
    predecessor.clear();

    /*long*/ memory = CGAL::Memory_sizer().virtual_size(); CGAL_TRACE("  %ld Mb allocated\n", memory>>20);
    CGAL_TRACE("  Call boost::breadth_first_search()\n");

    // Orient normals
    boost::breadth_first_search(mst_graph,
                                boost::vertex(source_vertex_index, mst_graph), // source
                                visitor(boost::make_bfs_visitor(propagate_normal<VertexIterator, VertexNormalMap>())));

    /*long*/ memory = CGAL::Memory_sizer().virtual_size(); CGAL_TRACE("  %ld Mb allocated\n", memory>>20);
    CGAL_TRACE("End of orient_normals_minimum_spanning_tree_3()\n");
}


// Avoid clash with other header
#undef CGAL_TRACE

CGAL_END_NAMESPACE

#endif // CGAL_ORIENT_NORMALS_MINIMUM_SPANNING_TREE_3_H

