// ======================================================================
//
// Copyright (c) 2001 The CGAL Consortium
//
// This software and related documentation is part of an INTERNAL release
// of the Computational Geometry Algorithms Library (CGAL). It is not
// intended for general use.
//
// ----------------------------------------------------------------------
//
// release       :
// release_date  :
//
// file          : include/CGAL/Leaf_node.h
// package       : APSPAS
// revision      : 1.0 
// revision_date : 2001/06/12 
// maintainer    : Hans Tangelder (<hanst@cs.uu.nl>)
//
// ======================================================================

#ifndef CGAL_LEAF_NODE_H
#define CGAL_LEAF_NODE_H
#include <CGAL/Base_node.h>
#include <CGAL/Cartesian.h>
#include <iomanip>
#include <CGAL/IO/PS_stream.h>

namespace CGAL {

template <class Traits> // = Kd_tree_traits_Point>
class Leaf_node: public Base_node<Traits>  {
public:
  typedef Traits::Item Item;
  
private:
  int n;
  Item** data;
public:
  typedef typename Item::FT NT;
  typedef CGAL::Point_2< CGAL::Cartesian<NT> > Point_2D;
  typedef Item** Item_iterator;
  const bool is_leaf() const { return 1;}
  const int size() const { return n;}
  
  Item_iterator const begin() const  {return data;}
  Item_iterator const end() const {return data + n;}
  Leaf_node(Points_container<Item>& c) :
    n(c.size()), data(new Item*[n]) {
    std::copy(c.begin(), c.end(), data);
    // std::cout << "Boxtree_leaf_node_d called" << std::endl;
  }

  void data_to_postscript(PS_Stream& PS,
	const int i, const int j,
	const NT mini, const NT maxi,
	const NT minj, const NT maxj) {
	  // PS << border_color(RED);  works only for visual
	  for (Item_iterator it=begin(); it != end(); it++) { //Item_iterator replaced Item**
	  Point_2D p ( (*(*it))[i], (*(*it))[j] ); 
	  PS << p;
	}
  } 

  // removed default constructor !!
  ~Leaf_node() {
    // std::cout << "~Leaf_node called" << std::endl;
    delete []data;
  }
};


} // namespace CGAL
#endif // CGAL_LEAF_NODE_H
