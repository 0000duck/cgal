// Copyright (c) 2012 GeometryFactory (France). All rights reserved.
// All rights reserved.
//
// Licensees holding a valid commercial license may use this file in
// accordance with the commercial license agreement provided with the software.
//
// This file is part of CGAL (www.cgal.org); you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License as
// published by the Free Software Foundation; version 2.1 of the License.
// See the file LICENSE.LGPL distributed with CGAL.
//
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
//
// $URL$
// $Id$
// 
//
// Author(s)     : Philipp Möller


#ifndef CGAL_PROPERTIES_POLYMESH_ARRAYKERNELT_H
#define CGAL_PROPERTIES_POLYMESH_ARRAYKERNELT_H

#include <CGAL/assertions.h>
#include <OpenMesh/Core/Mesh/PolyMesh_ArrayKernelT.hh>
#include <CGAL/boost/graph/properties.h>


namespace CGAL {


template <typename K>
class OM_edge_weight_pmap 
  : public boost::put_get_helper<typename OpenMesh::PolyMesh_ArrayKernelT<K>::Scalar , OM_edge_weight_pmap<K> >
{
public:
  typedef boost::readable_property_map_tag                         category;
  typedef typename OpenMesh::PolyMesh_ArrayKernelT<K>::Scalar      value_type;
  typedef value_type                                               reference;
  typedef typename boost::graph_traits<OpenMesh::PolyMesh_ArrayKernelT<K> >::edge_descriptor key_type;

  OM_edge_weight_pmap(const OpenMesh::PolyMesh_ArrayKernelT<K>& sm)
    : sm_(sm)
    {}

  value_type operator[](const key_type& e) const
  {
    return sm_.calc_edge_length(e.halfedge());
  }

private:
  const OpenMesh::PolyMesh_ArrayKernelT<K>& sm_;
};
  

template<typename K, typename Key>
class OM_is_border_pmap
{
public:
  typedef boost::read_write_property_map_tag category;
  typedef bool                             value_type;
  typedef bool                             reference;
  typedef Key                              key_type;
    
  OM_is_border_pmap(const OpenMesh::PolyMesh_ArrayKernelT<K>& sm)
    : sm_(sm)
    {}

  friend reference get(OM_is_border_pmap pm, key_type k)
  {
    return pm.sm_.is_boundary(k); 
  }

  friend void put(OM_is_border_pmap pm, key_type k , bool b)
  {
    if(b){
      const_cast<OpenMesh::PolyMesh_ArrayKernelT<K>& >(pm.sm_).set_face_handle(k, typename OpenMesh::PolyMesh_ArrayKernelT<K>::FaceHandle());
    }
  }

private:
  const OpenMesh::PolyMesh_ArrayKernelT<K>& sm_;
};

template <typename K, typename VEF>
class OM_index_pmap : public boost::put_get_helper<std::size_t, OM_index_pmap<K,VEF> >
{
public:
  typedef boost::readable_property_map_tag category;
  typedef std::size_t                      value_type;
  typedef std::size_t                      reference;
  typedef VEF                              key_type;

  value_type operator[](const key_type& vd) const
  {
    return vd.idx();
  }
};


template<typename K>
class OM_point_pmap //: public boost::put_get_helper<bool, OM_point_pmap<K> >
{
public:
  typedef boost::read_write_property_map_tag category;
#if !defined(CGAL_BGL_TESTSUITE)
  typedef typename OpenMesh::PolyMesh_ArrayKernelT<K>::Point             value_type;
  typedef typename const OpenMesh::PolyMesh_ArrayKernelT<K>::Point&      reference;
#else
  typedef typename CGAL::Exact_predicates_inexact_constructions_kernel::Point_3               value_type;
  typedef typename CGAL::Exact_predicates_inexact_constructions_kernel::Point_3               reference;
#endif
  typedef typename boost::graph_traits< OpenMesh::PolyMesh_ArrayKernelT<K> >::vertex_descriptor key_type;
    
  OM_point_pmap()
    : sm_(NULL)
  {}

  OM_point_pmap(const OpenMesh::PolyMesh_ArrayKernelT<K>& sm)
    : sm_(&sm)
    {}
    
  OM_point_pmap(const OM_point_pmap& pm)
    : sm_(pm.sm_)
    {}

  inline friend reference get(const OM_point_pmap<K>& pm, key_type v)
  {
#if !defined(CGAL_BGL_TESTSUITE)
    return pm.sm->.point(v);
#else
    typename OpenMesh::PolyMesh_ArrayKernelT<K>::Point const& omp = pm.sm_->point(v);
    return value_type(omp[0], omp[1], omp[2]);
#endif
  }

  inline friend void put(const OM_point_pmap<K>& pm, key_type v, const value_type& p)
  {
#if !defined(CGAL_BGL_TESTSUITE)
    const_cast<OpenMesh::PolyMesh_ArrayKernelT<K>&>(*pm.sm_)->set_point(v,p);
#else
    const_cast<OpenMesh::PolyMesh_ArrayKernelT<K>&>(*pm.sm_)->set_point
      (v, typename OpenMesh::PolyMesh_ArrayKernelT<K>::Point(p[0], p[1], p[2]));
#endif
  }

  private:
  const OpenMesh::PolyMesh_ArrayKernelT<K>* sm_;
};


} // CGAL

// overloads and specializations in the boost namespace
namespace boost {

//
// edge_weight
//


template <typename K>
struct property_map<OpenMesh::PolyMesh_ArrayKernelT<K>, boost::edge_weight_t >
{
  typedef CGAL::OM_edge_weight_pmap<K> type;
  typedef CGAL::OM_edge_weight_pmap<K> const_type;
};



//
// vertex_index
//

template <typename K>
struct property_map<OpenMesh::PolyMesh_ArrayKernelT<K>, boost::vertex_index_t >
{
  typedef CGAL::OM_index_pmap<K, typename boost::graph_traits<OpenMesh::PolyMesh_ArrayKernelT<K> >::vertex_descriptor> type;
  typedef CGAL::OM_index_pmap<K, typename boost::graph_traits<OpenMesh::PolyMesh_ArrayKernelT<K> >::vertex_descriptor> const_type;
};


//
// face_index
//

template <typename K>
struct property_map<OpenMesh::PolyMesh_ArrayKernelT<K>, boost::face_index_t >
{
  typedef CGAL::OM_index_pmap<K, typename boost::graph_traits<OpenMesh::PolyMesh_ArrayKernelT<K> >::face_descriptor> type;
  typedef CGAL::OM_index_pmap<K, typename boost::graph_traits<OpenMesh::PolyMesh_ArrayKernelT<K> >::face_descriptor> const_type;
};

//
// edge_index
//

template <typename K>
struct property_map<OpenMesh::PolyMesh_ArrayKernelT<K>, boost::edge_index_t >
{
  typedef CGAL::OM_index_pmap<K, typename boost::graph_traits<OpenMesh::PolyMesh_ArrayKernelT<K> >::edge_descriptor> type;
  typedef CGAL::OM_index_pmap<K, typename boost::graph_traits<OpenMesh::PolyMesh_ArrayKernelT<K> >::edge_descriptor> const_type;
};

//
// halfedge_index
//

template <typename K>
struct property_map<OpenMesh::PolyMesh_ArrayKernelT<K>, boost::halfedge_index_t >
{
  typedef CGAL::OM_index_pmap<K, typename boost::graph_traits<OpenMesh::PolyMesh_ArrayKernelT<K> >::halfedge_descriptor> type;
  typedef CGAL::OM_index_pmap<K, typename boost::graph_traits<OpenMesh::PolyMesh_ArrayKernelT<K> >::halfedge_descriptor> const_type;
};


template<typename K>
struct property_map<OpenMesh::PolyMesh_ArrayKernelT<K>, boost::vertex_point_t >
{
  typedef CGAL::OM_point_pmap<K> type;
  typedef type const_type;
};


//
// vertex_is_border
//

template<typename K>
struct property_map<OpenMesh::PolyMesh_ArrayKernelT<K>, CGAL::vertex_is_border_t >
{
  typedef CGAL::OM_is_border_pmap<K, typename boost::graph_traits<OpenMesh::PolyMesh_ArrayKernelT<K> >::vertex_descriptor>
    type;
  typedef type const_type;
};

//
// halfedge_is_border
//

template<typename K>
struct property_map<OpenMesh::PolyMesh_ArrayKernelT<K>, CGAL::halfedge_is_border_t >
{
  typedef CGAL::OM_is_border_pmap<K, typename boost::graph_traits<OpenMesh::PolyMesh_ArrayKernelT<K> >::halfedge_descriptor>
    type;
  typedef type const_type;
};

} // namespace boost

namespace boost {


template <typename K>
typename boost::property_map<OpenMesh::PolyMesh_ArrayKernelT<K>, boost::edge_weight_t>::const_type
get(boost::edge_weight_t, const OpenMesh::PolyMesh_ArrayKernelT<K>& sm)
{
  return CGAL::OM_edge_weight_pmap<K>(sm);
}

template <typename K>
typename OpenMesh::PolyMesh_ArrayKernelT<K>::Scalar
get(boost::edge_weight_t, const OpenMesh::PolyMesh_ArrayKernelT<K>& sm, 
    const typename boost::graph_traits<OpenMesh::PolyMesh_ArrayKernelT<K> >::edge_descriptor& e)
{
  return CGAL::OM_edge_weight_pmap<K>(sm)[e];
}


template <typename K>
CGAL::OM_index_pmap<K, typename boost::graph_traits<OpenMesh::PolyMesh_ArrayKernelT<K> >::vertex_descriptor>
get(const boost::vertex_index_t&, const OpenMesh::PolyMesh_ArrayKernelT<K>&)
{
  return CGAL::OM_index_pmap<K, typename boost::graph_traits<OpenMesh::PolyMesh_ArrayKernelT<K> >::vertex_descriptor>();
}

template <typename K>
typename boost::property_map<OpenMesh::PolyMesh_ArrayKernelT<K>, boost::face_index_t>::const_type
get(const boost::face_index_t&, const OpenMesh::PolyMesh_ArrayKernelT<K>&)
{
  return CGAL::OM_index_pmap<K, typename boost::graph_traits<OpenMesh::PolyMesh_ArrayKernelT<K> >::face_descriptor>();
}

template <typename K>
CGAL::OM_index_pmap<K, typename boost::graph_traits<OpenMesh::PolyMesh_ArrayKernelT<K> >::edge_descriptor>
get(const boost::edge_index_t&, const OpenMesh::PolyMesh_ArrayKernelT<K>&)
{
  return CGAL::OM_index_pmap<K, typename boost::graph_traits<OpenMesh::PolyMesh_ArrayKernelT<K> >::edge_descriptor>();
}

template <typename K>
CGAL::OM_index_pmap<K, typename boost::graph_traits<OpenMesh::PolyMesh_ArrayKernelT<K> >::halfedge_descriptor>
get(const boost::halfedge_index_t&, const OpenMesh::PolyMesh_ArrayKernelT<K>&)
{
  return CGAL::OM_index_pmap<K, typename boost::graph_traits<OpenMesh::PolyMesh_ArrayKernelT<K> >::halfedge_descriptor>();
}

template<typename K>
CGAL::OM_point_pmap<K>
get(boost::vertex_point_t, const OpenMesh::PolyMesh_ArrayKernelT<K>& g) 
{
 return CGAL::OM_point_pmap<K>(g);
}


template<typename K>
CGAL::OM_is_border_pmap<K, typename boost::graph_traits<OpenMesh::PolyMesh_ArrayKernelT<K> >::vertex_descriptor>
get(boost::vertex_is_border_t, const OpenMesh::PolyMesh_ArrayKernelT<K>& g) 
{
  return CGAL::OM_is_border_pmap<K, typename boost::graph_traits<OpenMesh::PolyMesh_ArrayKernelT<K> >::vertex_descriptor>(g);
}

template<typename K>
CGAL::OM_is_border_pmap<K, typename boost::graph_traits<OpenMesh::PolyMesh_ArrayKernelT<K> >::halfedge_descriptor>
get(boost::halfedge_is_border_t, const OpenMesh::PolyMesh_ArrayKernelT<K>& g) 
{
  return CGAL::OM_is_border_pmap<K, typename boost::graph_traits<OpenMesh::PolyMesh_ArrayKernelT<K> >::halfedge_descriptor>(g);
}

// get for intrinsic properties
#define CGAL_OM_INTRINSIC_PROPERTY(RET, PROP, TYPE)                     \
  template<typename K>                                              \
  RET                                                                   \
  get(PROP p, const OpenMesh::PolyMesh_ArrayKernelT<K>& sm,                      \
      typename boost::graph_traits< OpenMesh::PolyMesh_ArrayKernelT<K> >::TYPE x) \
  { return get(get(p, sm), x); }                                        \

  CGAL_OM_INTRINSIC_PROPERTY(std::size_t, boost::vertex_index_t, vertex_descriptor)
  CGAL_OM_INTRINSIC_PROPERTY(std::size_t, boost::edge_index_t, edge_descriptor)
  CGAL_OM_INTRINSIC_PROPERTY(int, boost::halfedge_index_t, halfedge_descriptor)
  CGAL_OM_INTRINSIC_PROPERTY(std::size_t, boost::face_index_t, face_descriptor)
  //  CGAL_OM_INTRINSIC_PROPERTY(std::size_t, boost::halfedge_index_t, face_descriptor)
  CGAL_OM_INTRINSIC_PROPERTY(typename OpenMesh::PolyMesh_ArrayKernelT<K>::Point const&, boost::vertex_point_t, vertex_descriptor)
  CGAL_OM_INTRINSIC_PROPERTY(bool, boost::vertex_is_border_t, vertex_descriptor)
  CGAL_OM_INTRINSIC_PROPERTY(bool, boost::halfedge_is_border_t, halfedge_descriptor)

#undef CGAL_OM_INTRINSIC_PROPERTY

// put for intrinsic properties
// only available for vertex_point

template<typename K>
void
put(boost::vertex_point_t p, OpenMesh::PolyMesh_ArrayKernelT<K>& g,
    typename boost::graph_traits< OpenMesh::PolyMesh_ArrayKernelT<K> >::vertex_descriptor vd,
    const typename K::Point& point) 
{
  put(get(p,g), vd, point);
}


} // namespace boost



#endif /* CGAL_PROPERTIES_POLYMESH_ARRAYKERNELT_H */
