// ======================================================================
//
// Copyright (c) 2002 The CGAL Consortium
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
// file          : include/CGAL/Kd_tree.h
// package       : ASPAS
// revision      : 2.4 
// revision_date : 2003/02/01 
// authors       : Hans Tangelder (<hanst@cs.uu.nl>)
// maintainer    : Hans Tangelder (<hanst@cs.uu.nl>)
// coordinator   : Utrecht University
//
// ======================================================================

#ifndef CGAL_KD_TREE_H
#define CGAL_KD_TREE_H
#include <CGAL/Kd_tree_node.h>
#include <cassert>

namespace CGAL {


template <class Traits> 
class Kd_tree {
public:
  
  typedef typename Traits::Item Item;
  typedef typename std::list<Item>::iterator input_iterator;
  typedef typename Traits::NT NT;
  typedef Kd_tree_node<Traits> Node;
  typedef Kd_tree<Traits> Tree;

private:
  Node* tree_root;
  Kd_tree_rectangle<NT>* bbox;
  std::list<Item> pts;
  Traits tr;
  int the_item_number;

  // protected copy constructor
  Kd_tree(const Tree& tree) {};

public:

  
  Kd_tree(input_iterator first, input_iterator beyond,
	    Traits t = Traits()) : tr(t) {
    assert(first != beyond);
    int dim = first->dimension();
    
    std::copy(first, beyond, std::back_inserter(pts));

    Point_container<Item> c(dim, pts.begin(), pts.end());

    bbox = new Kd_tree_rectangle<NT>(c.bounding_box());
    
    the_item_number=c.size();
    if (c.size() <= t.bucket_size())
      tree_root = new Node(c);
    else 
		if (t.use_extended_nodes())
		tree_root = new Node(c,t,true);  
		else
		tree_root = new Node(c,t,false); 
	
  }

  template <class OutputIterator, class Rectangle>
	void search(OutputIterator it, Rectangle& r, NT eps=NT(0)) {
		Kd_tree_rectangle<NT>* b = new Kd_tree_rectangle<NT>(*bbox);
		tree_root->tree_items_in_rectangle(it,r,b,eps);
		delete b;
	}

  template <class OutputIterator>
	void search(OutputIterator it, Item& center, NT radius, NT eps=NT(0)) {
		Kd_tree_rectangle<NT>* b = new Kd_tree_rectangle<NT>(*bbox);
		tree_root->tree_items_in_sphere(it, center,
		(radius-eps)*(radius-eps), radius*radius,
                (radius+eps)*(radius+eps), b);
		delete b;
	}

  template <class OutputIterator>
	void report_all_points(OutputIterator it) 
	{tree_root->tree_items(it);}

    ~Kd_tree() {
                  delete tree_root; 
		  delete bbox;
	};


  Traits traits() const {return tr;} // Returns the traits class;

  Node* root() { return tree_root; }

  Kd_tree_rectangle<NT>* bounding_box() {return bbox; }

  int item_number() {return the_item_number;}

  // Print statistics of the tree.
  void statistics () {
    std::cout << "Tree statistics:" << std::endl;
    std::cout << "Number of items stored: " 
		  << tree_root->num_items() << std::endl;
    std::cout << " Tree depth: " << tree_root->depth() << std::endl;
  }


};

} // namespace CGAL
#endif // CGAL_KD_TREE_H
