// ======================================================================
//
// Copyright (c) 2003 The CGAL Consortium
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
// file          : include/CGAL/Apollonius_site_2.h
// package       : Apollonius_graph_2
// source        : $RCSfile$
// revision      : $Revision$
// revision_date : $Date$
// author(s)     : Menelaos Karavelas <mkaravel@cse.nd.edu>
//
// coordinator   :
//
// ======================================================================

#ifndef CGAL_APOLLONIUS_SITE_2_H
#define CGAL_APOLLONIUS_SITE_2_H

#include <iostream>
#include <CGAL/basic.h>

#include <CGAL/Apollonius_graph_short_names_2.h>

CGAL_BEGIN_NAMESPACE


template < class K >
class Apollonius_site_2
{
public:
  typedef K                       Kernel;
  typedef typename K::Point_2     Point_2;
  typedef Apollonius_site_2<K>    Self;
  typedef typename K::FT          Weight;
  typedef typename K::RT          RT;
  typedef Weight                  FT;


public:
  Apollonius_site_2(const Point_2& p = Point_2(),
		    const Weight& w = Weight(0))
    : _p(p), _w(w) {}

  Apollonius_site_2(const Apollonius_site_2& other)
    : _p(other._p), _w(other._w) {}

  
  const Point_2& point() const { return _p; }
  const Weight&  weight() const { return _w; }
  RT      x() const { return _p.x(); }
  RT      y() const { return _p.y(); }

  bool operator==(const Apollonius_site_2& other) const
  {
    return ( _p == other._p && _w == other._w );
  }

private:
  Point_2 _p;
  Weight  _w;

};

template <class K>
std::ostream&
operator<<(std::ostream &os, const Apollonius_site_2<K>& wp)
{
  return os << wp.point() << " " << wp.weight();
}

template <class K>
std::istream&
operator>>(std::istream& is, Apollonius_site_2<K>& wp)
{
  typename Apollonius_site_2<K>::Weight   weight;
  typename Apollonius_site_2<K>::Point_2  p;
  is >> p >> weight;
  wp = Apollonius_site_2<K>(p, weight);
  return is;
}


CGAL_END_NAMESPACE

#ifdef CGAL_USE_QT

#include <CGAL/IO/Qt_widget.h>

CGAL_BEGIN_NAMESPACE

template <class K>
Qt_widget&
operator<<(Qt_widget &qt_w, const Apollonius_site_2<K>& wp)
{
  typedef typename K::Circle_2    Circle_2;
  typedef typename K::Point_2     Point_2;

  Point_2 p(wp.point());
  Circle_2 c(p, CGAL::square(wp.weight()));
  return qt_w << p << c;
}

CGAL_END_NAMESPACE


#endif




#endif // CGAL_APOLLONIUS_SITE_2_H

