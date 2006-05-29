// Copyright (c) 2006 Geometry Factory (France).
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
// $URL: $
// $Id: $
// 
//
// Author(s): Andreas Fabri <andreas.fabri@geometryfactory.com>, Fernando Cacciola <fernando.cacciola@gmail.com>

#ifndef CGAL_POLYHEDRON_BGL_PROPERTIES_H
#define CGAL_POLYHEDRON_BGL_PROPERTIES_H

#include <CGAL/Polyhedron_BGL.h>

#ifndef CGAL_CFG_NO_TMPL_IN_TMPL_PARAM
#  define CGAL_HDS_PARAM_ template < class Traits, class Items, class Alloc> class HDS
#else
#  define CGAL_HDS_PARAM_ class HDS
#endif


namespace boost
{

template<class Gt, class I, CGAL_HDS_PARAM_, class A>
class Polyhedron_edge_weight_map : public put_get_helper<double, Polyhedron_edge_weight_map<Gt, I, HDS, A> >
{
private:

  typedef CGAL::Polyhedron_3<Gt,I,HDS,A> Polyhedron ;
  
public:

  typedef readable_property_map_tag category;
  typedef double value_type;
  typedef double reference;
  typedef typename graph_traits<Polyhedron const>::edge_descriptor  key_type;

  Polyhedron_edge_weight_map( Polyhedron const& p_) 
  {}

  reference operator[](key_type const& e) const {
    return CGAL::squared_distance(e->vertex()->point(), e->opposite()->vertex()->point());
  }
};

template<class Gt, class I, CGAL_HDS_PARAM_, class A>
class Polyhedron_edge_is_border_map : public put_get_helper<double, Polyhedron_edge_is_border_map<Gt, I, HDS, A> >
{
private:

  typedef CGAL::Polyhedron_3<Gt,I,HDS,A> Polyhedron ;
  
public:

  typedef readable_property_map_tag category;
  typedef bool value_type;
  typedef bool reference;
  typedef typename graph_traits<Polyhedron const>::edge_descriptor  key_type;

  Polyhedron_edge_is_border_map( Polyhedron const& p_) 
  { }

  reference operator[](key_type const& e) const 
  {
    return e->is_border();
  }
};

template<class Gt, class I, CGAL_HDS_PARAM_, class A>
class Polyhedron_vertex_point_map : public put_get_helper< typename Gt::Point_3, Polyhedron_vertex_point_map<Gt, I, HDS, A> >
{
private:

  typedef CGAL::Polyhedron_3<Gt,I,HDS,A> Polyhedron ;

public:

  typedef typename Gt::Point_3 Point_3 ;
  
  typedef lvalue_property_map_tag category;
  
  typedef Point_3   value_type;
  typedef Point_3 & reference;
  
  typedef typename graph_traits<Polyhedron>::vertex_descriptor key_type;

  Polyhedron_vertex_point_map( Polyhedron& p_) 
  { }

  reference operator[](key_type const& e) const 
  {
    return e->point();
  }
};

struct edge_is_border_t {} ;
struct vertex_point_t   {} ;

template<class Gt, class I, CGAL_HDS_PARAM_, class A>
inline Polyhedron_edge_weight_map<Gt, I, HDS, A>
get(edge_weight_t, const CGAL::Polyhedron_3<Gt,I,HDS,A>& p) 
{
  Polyhedron_edge_weight_map<Gt, I, HDS, A> m(p);
  return m;
}

template<class Gt, class I, CGAL_HDS_PARAM_, class A>
inline Polyhedron_edge_is_border_map<Gt, I, HDS, A>
get(edge_is_border_t, const CGAL::Polyhedron_3<Gt,I,HDS,A>& p) 
{
  Polyhedron_edge_is_border_map<Gt, I, HDS, A> m(p);
  return m;
}

template<class Gt, class I, CGAL_HDS_PARAM_, class A>
inline Polyhedron_vertex_point_map<Gt, I, HDS, A>
get(vertex_point_t, CGAL::Polyhedron_3<Gt,I,HDS,A>& p) 
{
  Polyhedron_vertex_point_map<Gt, I, HDS, A> m(p);
  return m;
}

template <class Tag>
struct Polyhedron_property_map { };

template <>
struct Polyhedron_property_map<edge_weight_t> 
{
  template<class Gt, class I, CGAL_HDS_PARAM_, class A>
  struct bind_ {
    typedef Polyhedron_edge_weight_map<Gt, I, HDS, A> type;
    typedef Polyhedron_edge_weight_map<Gt, I, HDS, A> const_type;
  };
};

template <>
struct Polyhedron_property_map<edge_is_border_t> 
{
  template<class Gt, class I, CGAL_HDS_PARAM_, class A>
  struct bind_ {
    typedef Polyhedron_edge_is_border_map<Gt, I, HDS, A> type;
    typedef Polyhedron_edge_is_border_map<Gt, I, HDS, A> const_type;
  };
};

template <>
struct Polyhedron_property_map<vertex_point_t> 
{
  template<class Gt, class I, CGAL_HDS_PARAM_, class A>
  struct bind_ {
    typedef Polyhedron_vertex_point_map<Gt, I, HDS, A> type;
    typedef Polyhedron_vertex_point_map<Gt, I, HDS, A> const_type;
  };
};

// g++ 'enumeral_type' in template unification not implemented workaround
template<class Gt, class I, CGAL_HDS_PARAM_, class A, class Tag>
struct property_map<CGAL::Polyhedron_3<Gt,I,HDS,A>, Tag> 
{
  typedef typename Polyhedron_property_map<Tag>::template bind_<Gt, I, HDS, A> map_gen;
  typedef typename map_gen::type type;
  typedef typename map_gen::const_type const_type;
};

template<class Gt, class I, CGAL_HDS_PARAM_, class A, class PropertyTag, class Key>
inline
typename property_traits< typename property_map<CGAL::Polyhedron_3<Gt,I,HDS,A>,PropertyTag>::type>::value_type
get(PropertyTag p, CGAL::Polyhedron_3<Gt,I,HDS,A>& g, const Key& key) 
{
  return get(get(p, g), key);
}

template<class Gt, class I, CGAL_HDS_PARAM_, class A, class PropertyTag, class Key>
inline
typename property_traits< typename property_map<CGAL::Polyhedron_3<Gt,I,HDS,A>,PropertyTag>::const_type>::value_type
get(PropertyTag p, const CGAL::Polyhedron_3<Gt,I,HDS,A>& g, const Key& key) 
{
  return get(get(p, g), key);
}

template<class Gt, class I, CGAL_HDS_PARAM_, class A, class PropertyTag, class Key,class Value>
inline void put(PropertyTag p, CGAL::Polyhedron_3<Gt,I,HDS,A>& g, const Key& key, const Value& value)
{
  typedef typename property_map<CGAL::Polyhedron_3<Gt,I,HDS,A>, PropertyTag>::type Map;
  Map pmap = get(p, g);
  put(pmap, key, value);
}


// What are those needed for ???
template<class Gt, class I, CGAL_HDS_PARAM_, class A>
struct edge_property_type<CGAL::Polyhedron_3<Gt,I,HDS,A> > 
{
  typedef edge_weight_t type;
};  

template<class Gt, class I, CGAL_HDS_PARAM_, class A>
struct vertex_property_type<CGAL::Polyhedron_3<Gt,I,HDS,A> > 
{
  typedef vertex_point_t type;
};
        
} // namespace boost

#undef CGAL_HDS_

#endif // CGAL_POLYHEDRON_BGL_PROPERTIES_H
