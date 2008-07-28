#ifndef _AABB_TREE_
#define _AABB_TREE_

#include <list>
#include <stack>
#include "AABB_node.h"
#include "knn.h"

CGAL_BEGIN_NAMESPACE

template <class Kernel, class Input, class PSC>
class AABB_tree
{
public:

  typedef typename Kernel::FT FT;
  typedef typename CGAL::Bbox_3 Bbox;
  typedef typename Kernel::Ray_3 Ray;
  typedef typename Kernel::Line_3 Line;
  typedef typename Kernel::Plane_3 Plane;
  typedef typename Kernel::Point_3 Point;
  typedef typename Kernel::Vector_3 Vector;
  typedef typename Kernel::Segment_3 Segment;
  typedef typename Kernel::Triangle_3 Triangle;
  typedef typename Kernel::Iso_cuboid_3 Iso_cuboid;

  typedef AABB_node<Kernel,Input,PSC> Node;
  typedef typename Node::Point_with_input Point_with_input;

  // types for K nearest neighbors search structure
  typedef CNeighbor_search<Kernel> Neighbor_search;

private:

  // set of input primitives (halfedge or face handles)
  std::vector<Input> m_data;

  // single root node
  Node *m_root;

public:
  // life cycle
  AABB_tree() {m_root = NULL;}
  ~AABB_tree()
  {
    cleanup();
  }

  void cleanup()
  {
    int n = std::max<int>(m_data.size(), 2);
    m_data.clear();
    if(m_root != NULL)
      delete [] m_root;
  }

  // build tree when Input = face_handle
  bool build_faces(PSC& psc)
  {
    cleanup();
    set_face_data(psc);
    m_root = NULL;
    if(!empty())
    {
      m_root = new Node[m_data.size()-1]();
      m_root->expand(psc, m_data.begin(),m_data.end(), m_data.size());
      return true;
    }
    return false;
  }

  void set_face_data(PSC& psc)
  {
    unsigned int nbf = psc.size_of_facets();
    m_data.reserve(nbf);
    typename PSC::Facet_iterator f;
    for(f = psc.facets_begin();
      f != psc.facets_end();
      f++)
      m_data.push_back(f);
  }

  bool empty()
  {
    return m_data.size() < 2; // TODO: change this requirement to < 1
  }

  bool first_intersection(const Ray& ray,
                          Point_with_input& pwh)
  {
    std::pair<bool,Point_with_input> result;
    m_root->first_intersection(ray, result, m_data.size());
    if(result.first)
    {
      pwh = result.second;
      return true;
    }
    return false;
  }

  bool first_intersection(const Segment& seg,
                          Point_with_input& pwh)
  {
    std::pair<bool,Point_with_input> result;
    m_root->first_intersection(seg, result, m_data.size());
    if(result.first)
    {
      pwh = result.second;
      return true;
    }
    return false;
  }

  bool first_intersection(const Line& line,
                          Point_with_input& pwh)
  {
    std::pair<bool,Point_with_input> result;
    m_root->first_intersection(line, result, m_data.size());
    if(result.first)
    {
      pwh = result.second;
      return true;
    }
    return false;
  }
};

CGAL_END_NAMESPACE

#endif // _AABB_TREE_
