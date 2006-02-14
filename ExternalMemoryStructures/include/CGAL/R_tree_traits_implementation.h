// Copyright (c) 1998  ETH Zurich (Switzerland).
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
// $Source$
// $Revision$ $Date$
// $Name$
//
// Author(s)     : Gabriele Neyer<neyer@inf.ethz.ch>

#ifndef __R_tree_traits_implementation_H__
#define __R_tree_traits_implementation_H__

#include <CGAL/basic.h>
#include <fstream>
#include <vector>
#include <iostream>

CGAL_BEGIN_NAMESPACE 


// R_Tree_Interface class
template<class R_tree_data>
class R_tree_traits
{

public:
  typedef R_tree_data Data;
  typedef typename R_tree_data::Key Key;
  typedef R_tree_traits<Data> Traits;


  R_tree_traits(){}

  ~R_tree_traits(){}
  Key build(const Data &d){
    return d.key;
  }
  
  Key unify(const Key &p, const Key &q)  {
    Key t;
    t.unify(p,q); 
    return t;
  }

  //returns true if x includes y
  bool include(const Key& x, const Key& y){
    return x.include(y);
  }

  bool intersect(const Key& x, const Key& y){
    return x.intersect(y);
  }

  
  double cost(const Key &p) const {
    return p.cost();
  }

  //overlap of areas due to Beckmann et al.
  // area(p\cap q)
  double cost(const Key &p, const Key &q) const {
    Key k;
    k.unify(p,q); 
    double area1, area2, area3;
    area1 = p.cost();
    area2 = q.cost();
    area3 = k.cost();
    return (area3 - area1 - area2);
  }

  //compare function. can be used to do arbitrary compares 
  //only used in window query begin_compare, end_compare
  //returns true if x includes y
  bool compare(const Key& x, const Key& y) const{
    return x.compare(y);
  }

  Key intersection(const Key &p, const Key &q)  {
    Key t;
    t.intersection(p,q); 
    return t;
  }
  
  size_t size(const Data &d) const {
    return d.size();
  }

  size_t written_size(const Data &d) const {
    return d.size();
  }

  size_t size_key(const Key &k) const {
    return k.size();
  }

  size_t written_size_key(const Key &k) const {
    return k.size();
  }
  
//  Key clone_key(const Key &k){
//    Key k2=k;
//    return k2;
//  }

//  Data clone_key(const Key &k){
//    Key k2=k;
//    return k2;
//  }

  double center_dist(const Key &p, const Key &q) const {
    return p.center_dist(q);
  }

  void read(char **s, Data& d) {
    d.read(s);
  }
    
  void write(char **s, Data& d) {
    d.write(s);
  }    


  void read_key(char **s, Key& k) {
    k.read(s);
  }
    
  void write_key(char **s, Key& k) {
    k.write(s);
  }    


  void dump(const Key &k, int depth=0)	const {
    k.dump(depth);
  }
  
  //if 
  bool equal_data(const Data &d, const Data &e){
    if(d.key == e.key)
      return true;
    else
      return false;
  } 
  
  bool equal_key(const Key &d, const Key &e){
    if(d == e)
      return true;
    else
      return false;
  }
};



CGAL_END_NAMESPACE

#endif




