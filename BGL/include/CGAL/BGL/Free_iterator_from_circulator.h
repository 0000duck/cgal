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
// Author(s)     : Philipp Moeller

#ifndef CGAL_FREE_ITERATOR_FROM_CIRCULATOR_H
#define CGAL_FREE_ITERATOR_FROM_CIRCULATOR_H


#include <iostream>
#include <cstddef>

#include <CGAL/circulator.h>

#include <boost/operators.hpp>
#include <boost/concept_check.hpp>

#include <boost/mpl/if.hpp>
#include <boost/utility/enable_if.hpp>

namespace CGAL {

// adapted from circulator.h, does not support
// random_access_circulators and returns the underlying circulator
// instead of dereferencing it
template <class C, bool Prevent_deref = true>
class Free_iterator_from_circulator {
private:
  // The m_anchor is normalized to be a minimal circulator.
  C         m_anchor;
  C         current;
  int       m_winding;

  typedef  std::iterator_traits<C>                       I_traits;
  typedef  typename  I_traits::iterator_category         I_Iter_cat;
  typedef  I_Iterator_from_circulator_traits<I_Iter_cat> I__traits;

public:
  typedef C                                               Circulator;
  typedef Free_iterator_from_circulator<C, Prevent_deref> Self;

  typedef typename I__traits::iterator_category iterator_category;

  typedef typename 
  boost::mpl::if_c<  Prevent_deref
                   , C
                   , typename C::value_type
                  >::type             value_type;

  typedef typename C::difference_type difference_type;
  typedef typename 
  boost::mpl::if_c<  Prevent_deref
                   , C&
                   , typename C::reference
                  >::type             reference;
  typedef typename 
  boost::mpl::if_c<  Prevent_deref
                   , C*
                   , typename C::reference
                  >::type             pointer;

  Free_iterator_from_circulator(){}

  Free_iterator_from_circulator(const C circ, int n)
    : m_anchor(circ), current(circ), m_winding(n) {}

  bool operator==( const Self& i) const {
    CGAL_assertion( m_anchor == i.m_anchor);  // same anchor?
    return ( current == i.current) && ( m_winding == i.m_winding);
  }
  
  bool operator!=( const Self& i) const {
    return !(*this == i);
  }


// we cannot enable_if on operator* and operator-> because they do not
// depend on template parameters directly and default template
// arguments for functions are C++11. we redirect on helper member
// templates as a work-around.
private:
  template <bool Prevent_deref_>
  typename boost::enable_if_c<Prevent_deref_, reference>::type
  indirection() const {
    CGAL_assertion( current  != NULL);
    return const_cast<Self*>(this)->current;
  }
  template <bool Prevent_deref_>
  typename boost::disable_if_c<Prevent_deref_, reference>::type
  indirection() const {
    CGAL_assertion( current  != NULL);
    return *current;
  }
public:
  reference operator*() const {
    return indirection<Prevent_deref>();
  }
  
private:
  template <bool Prevent_deref_>
  typename boost::disable_if_c<Prevent_deref_, pointer>::type
  structure_dereference() {
    CGAL_assertion( current  != NULL);
    return &(*current);
  }
  template <bool Prevent_deref_>
  typename boost::enable_if_c<Prevent_deref_, pointer>::type
  structure_dereference() {
    CGAL_assertion( current  != NULL);
    return &current;
  }
public:
  pointer operator->() const {
    return structure_dereference<Prevent_deref>();
  }

  Self& operator++() {
    CGAL_assertion( current  != NULL);
    ++current;
    if ( current == m_anchor)
      ++m_winding;
    return *this;
  }
  Self  operator++(int) {
    Self tmp = *this;
    ++*this;
    return tmp;
  }
  Self& operator--() {
    CGAL_assertion( current != NULL);
    if ( current == m_anchor)
      --m_winding;
    --current;
    return *this;
  }
  Self  operator--(int) {
    Self tmp = *this;
    --*this;
    return tmp;
  }
  Self& operator+=( difference_type n) {
    CGAL_assertion( current != NULL);
    if ( n < 0 && current == m_anchor)  // We are leaving the anchor.
      --m_winding;
    current += n;
    if ( n > 0 && current == m_anchor)  // Back again at the anchor.
      ++m_winding;
    return *this;
  }

  bool operator<( const Self& i) const {
    CGAL_assertion( m_anchor  != NULL);
    CGAL_assertion( current != NULL);
    CGAL_assertion( m_anchor  == i.m_anchor);
    return (     (m_winding < i.m_winding)
                 || (    (m_winding == i.m_winding)
                         && (current - m_anchor) < (i.current - m_anchor)
                   )
      );
  }
  bool operator> ( const Self& i) const { return i < *this; }
  bool operator<=( const Self& i) const { return !(i < *this); }
  bool operator>=( const Self& i) const { return !(*this < i); }

  const C*    anchor()             const { return m_anchor;}
  int         winding()            const { return m_winding;}
  Circulator  current_circulator() const { return current;}
};

} // CGAL

#endif /* CGAL_FREE_ITERATOR_FROM_CIRCULATOR_H */
