// ======================================================================
//
// Copyright (c) 2001 The CGAL Consortium
//
// This software and related documentation is part of an INTERNAL release
// of the Computational Geometry Algorithms Library (CGAL). It is not
// intended for general use.
//
// ----------------------------------------------------------------------
// release       : 
// release_date  : 
//
// file          : include/CGAL/convex_hull_3_from_d.h
// package       : Convex_hull_3 
// revision      : $Revision$ 
// revision_date : $Date$
// author(s)     : Stefan Schirra <Stefan.Schirra@mpi-sb.mpg.de>
//               : Michael Seel <Michael.Seel@mpi-sb.mpg.de>
// coordinator   : MPI, Saarbruecken
// ======================================================================

#ifndef CGAL_CONVEX_HULL_3_FROM_D_H
#define CGAL_CONVEX_HULL_3_FROM_D_H

#include <CGAL/Convex_hull_d.h>
#include <CGAL/Convex_hull_d_traits_3.h>
#include <CGAL/Convex_hull_d_to_polyhedron_3.h>

CGAL_BEGIN_NAMESPACE
template <class InputIterator, class Polyhedron>
void
convex_hull_3_from_d(InputIterator first, InputIterator beyond, Polyhedron& P,
	             bool test_correctness = false)
{
  typedef typename Polyhedron::Traits       PolyTraits;
  typedef typename PolyTraits::R            R;
  typedef typename PolyTraits::Point        Point;
  typedef Convex_hull_d_traits_3<R>         ChullTraits;
  typedef Convex_hull_d< ChullTraits >      ChullType;

  ChullType CH(3);
  for ( ; first != beyond ; ++first)  CH.insert(*first);
  if ( test_correctness ) assert(CH.is_valid());
  CGAL::convex_hull_d_to_polyhedron_3(CH,P);
}
CGAL_END_NAMESPACE

#endif // CGAL_CONVEX_HULL_3_FROM_D_H
