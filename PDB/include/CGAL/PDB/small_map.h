/* Copyright 2004
   Stanford University

   This file is part of the DSR PDB Library.

   The DSR PDB Library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Lesser General Public License as published by
   the Free Software Foundation; either version 2.1 of the License, or (at your
   option) any later version.

   The DSR PDB Library is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
   or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
   License for more details.

   You should have received a copy of the GNU Lesser General Public License
   along with the DSR PDB Library; see the file LICENSE.LGPL.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
   MA 02110-1301, USA. */


#ifndef CGAL_DSR_PDB_SMALL_MAP_H
#define CGAL_DSR_PDB_SMALL_MAP_H
#include <CGAL/PDB/basic.h>
#include <vector>
#include <algorithm>

CGAL_PDB_BEGIN_NAMESPACE

#ifndef NDEBUG
#define CGAL_SMALL_STATIC_MAP_CHECK_LOCK(x) x
#else 
#define CGAL_SMALL_STATIC_MAP_CHECK_LOCK(x)
#endif

template <class K, class D>
class small_map_value_type {
  typedef small_map_value_type<K, D> This;
  K key_;
  D data_;
public:
  typedef K Key;
  typedef D Data;
  small_map_value_type(){}
  explicit small_map_value_type(Key k): key_(k){}
  small_map_value_type(Key k, const Data &d): key_(k), data_(d){}
  bool operator<(const This &o) const {
    return key_ < o.key_;
  }
  bool operator>(const This &o) const {
    return key_ > o.key_;
  }
  bool operator==(const This &o) const {
    return key_ == o.key_;
  }
 bool operator!=(const This &o) const {
    return key_ != o.key_;
  }
  bool operator<(Key k) const {
    return key_ < k;
  }
  bool operator>(Key k) const {
    return key_ > k;
  }
  bool operator==(Key k) const {
    return k == key_;
  }
  bool operator!=(Key k) const {
    return k == key_;
  }


  Key key() const {
    return key_;
  }
  const Data &data() const {
    return data_;
  }
  Data &data() {
    return data_;
  }
  operator Key() const {
    return key();
  }
  /*operator Key&() {
    return key();
    }*/

};


/*CGAL_PDB_END_NAMESPACE

CGAL_BEGIN_NAMESPACE
template <class K, class D>
CGAL::Comparison_result compare(const PDB::small_map_value_type<K,D> &a,
				const PDB::small_map_value_type<K,D> &b) {
  return CGAL::compare(a.key(), b.key());
}
CGAL_END_NAMESPACE

CGAL_PDB_BEGIN_NAMESPACE*/
/*template <class K, class D>
bool operator<(K k, const small_map_value_type<K,D> &vk) {
  return k < vk.key();
}

template <class K, class D>
bool operator>(K k, const small_map_value_type<K,D> &vk) {
  return k > vk.key();
  }*/


#define CGAL_SMALL_MAP_VALUE_TYPE(CName, K, D, name) struct CName : public CGAL::PDB::small_map_value_type<K,D>{ \
typedef CGAL::PDB::small_map_value_type<K,D> P;\
CName(){}\
CName(K k): P(k){}\
CName(K k, const D &d): P(k,d){}\
D &name(){return P::data();}\
const D& name()const {return P::data();}\
}

template <class Storage>
class small_map {
  typedef small_map<Storage> This;
public:
  typedef Storage value_type;
  
  /*struct Compare {
    bool operator()(Key k, const value_type &v) const {
      return k < v.first;
    }
    bool operator()(const value_type &v, Key k) const {
      return v.first < k;
    }
    bool operator()(const value_type &a, const value_type &b) const {
      return a.first < b.first;
    }
    };*/

  typedef std::vector<value_type> container;
  typedef typename container::iterator iterator;
  typedef typename container::const_iterator const_iterator;


  small_map(std::size_t sz=0){
    CGAL_SMALL_STATIC_MAP_CHECK_LOCK(sorted_=true);
    c_.reserve(sz);
  }

  void swap_with(This &o) {
    std::swap(c_, o.c_);
    CGAL_SMALL_STATIC_MAP_CHECK_LOCK(std::swap(sorted_, o.sorted_));
  }

  template <class K>
  iterator find(const K k) {
    CGAL_SMALL_STATIC_MAP_CHECK_LOCK(CGAL_precondition(sorted_));
    std::pair<iterator,iterator> ret= std::equal_range(begin(), end(), k/*, Compare()*/);
    CGAL_assertion(ret.first == end() || ret.first ==ret.second || ret.first->key() == k);
    if (ret.first==ret.second) return end();
    else return ret.first;
  }
  template <class K>
  const_iterator find(const K k) const {
    CGAL_SMALL_STATIC_MAP_CHECK_LOCK(CGAL_precondition(sorted_));
    std::pair<const_iterator,const_iterator> ret= std::equal_range(begin(), end(), k/*, Compare()*/);
    CGAL_assertion(ret.first == end() || ret.first ==ret.second || ret.first->key() == k);
    if (ret.first==ret.second) return end();
    else return ret.first;
  }

  iterator begin() {
    CGAL_SMALL_STATIC_MAP_CHECK_LOCK(CGAL_precondition(sorted_));
    return c_.begin();
  }

  iterator end() {
    CGAL_SMALL_STATIC_MAP_CHECK_LOCK(CGAL_precondition(sorted_));
    return c_.end();
  }

  const_iterator begin() const {
    return c_.begin();
  }

  const_iterator end() const {
    return c_.end();
  }

  template <class K>
  iterator lower_bound(K k) {
    return std::lower_bound(begin(), end(), k/*, Compare()*/);
  }

  template <class K>
  typename Storage::Data& operator[](K k){
    CGAL_SMALL_STATIC_MAP_CHECK_LOCK(CGAL_precondition(sorted_));
    iterator it= find(k);
    if (it == end()) {
      it= insert(value_type(k));
    }
    CGAL_postcondition(it->key() == k);
    return it->data();
  }

  iterator insert(const value_type &v) {
    CGAL_SMALL_STATIC_MAP_CHECK_LOCK(CGAL_precondition(sorted_));
    if (c_.empty() || v > c_.back()) {
      c_.insert(end(), v);
      return c_.end()-1;
    } else {
      iterator it= lower_bound(v.key());
      CGAL_assertion(it== end() || *it != v);
      return c_.insert(it, v);
    }
  }
  void lazy_insert(const value_type &v) {
    CGAL_SMALL_STATIC_MAP_CHECK_LOCK(sorted_=false);
    c_.push_back(v);
  }

  std::size_t size() const {
    return c_.size();
  }

  bool empty() const {
    return c_.empty();
  }

  template <class K>
  void erase(K k) {
    iterator it = find(k);
    c_.erase(it);
  }

  /*bool locked() const {
    return locked_;
    }*/
  void sort() {
    CGAL_SMALL_STATIC_MAP_CHECK_LOCK(sorted_=true);
    std::sort(begin(), end()/*, Compare()*/);
  }

  void clear() {
    c_.clear();
    CGAL_SMALL_STATIC_MAP_CHECK_LOCK(sorted_=true);
  }

protected:
  container c_;
  CGAL_SMALL_STATIC_MAP_CHECK_LOCK(bool sorted_;)
};

template <class C>
inline void swap(small_map<C> &a, small_map<C> &b) {
  a.swap_with(b);
}

CGAL_PDB_END_NAMESPACE


namespace std{
  /*template <class K, class D>
  bool operator<(K k, const CGAL_PDB_NS::small_map_value_type<K,D> &vk) {
    return k < vk.key();
  }

  template <class K, class D>
  bool operator>(K k, const CGAL_PDB_NS::small_map_value_type<K,D> &vk) {
    return k > vk.key();
    }*/

};
#endif
