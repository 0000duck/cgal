// ============================================================================
//
// Copyright (c) 2000 The CGAL Consortium
//
// This software and related documentation is part of an INTERNAL release
// of the Computational Geometry Algorithms Library (CGAL). It is not
// intended for general use.
//
// ----------------------------------------------------------------------------
//
// release       : $CGAL_Revision $
// release_date  : $CGAL_Date $
//
// file          : include/CGAL/random_polygon_2.h
// package       : $CGAL_Package: Generator 2.12 (28 Jul 1999) $
// chapter       : Geometric Object Generators
//
// revision      : $Revision$
// revision_date : $Date$
//
// author(s)     : Susan Hert <hert@mpi-sb.mpg.de>
//
// coordinator   : ETH Zurich (Bernd Gaertner <gaertner@inf.ethz.ch>)
//
// implementation: Random Simple Polygons
// ============================================================================
#ifndef CGAL_RANDOM_POLYGON_2_H
#define CGAL_RANDOM_POLYGON_2_H

#include <list>
#include <set>
#include <vector>
#include <assert.h>
#include <CGAL/Random_polygon_2_sweep.h>
#include <CGAL/Kernel_traits.h>

namespace CGAL {

//
// Using the provided point generator, generates a set of n points and 
// produces  a simple polygon from the unique subset of points within this 
// set.
// 
// Each of the p possible simple polygons for the unique point set is 
// generated with probability greater than 0 but the polygons are not 
// generated with uniform probability.
//
template <class PointGenerator, class OutputIterator, class Traits>
OutputIterator random_polygon_2(int n,  OutputIterator result, 
                                const PointGenerator& pg, const Traits& traits)
{
   typedef typename Traits::Point_2           Point_2;
   typedef std::vector<Point_2>               Vertex_list;
   typedef typename Vertex_list::iterator     Iterator;

   Vertex_list  vertices;

   copy_n_unique(pg, n, std::back_inserter(vertices), traits);
   CGAL_assertion(!duplicate_points(vertices.begin(), vertices.end(), traits));

   std::random_shuffle(vertices.begin(), vertices.end());

   make_simple_polygon(vertices.begin(), vertices.end(), traits);

   if (orientation_2(vertices.begin(), vertices.end()) == CLOCKWISE)
      std::reverse(vertices.begin(), vertices.end());


   CGAL_assertion(is_simple_2(vertices.begin(), vertices.end()));
   for (Iterator it = vertices.begin(); it != vertices.end(); it++)
   {
      *result = *it;
      result++;
   }
   return result;
}

template <class PointGenerator, class OutputIterator>
inline
OutputIterator random_polygon_2( int n,  OutputIterator result, 
                                 const PointGenerator& pg )
{
   typedef typename std::iterator_traits<PointGenerator>::value_type  Point_2;
   typedef typename Kernel_traits<Point_2>::Kernel   K;
   return random_polygon_2(n, result, pg, K());
}

template <class ForwardIterator, class Traits>
bool duplicate_points(ForwardIterator first, ForwardIterator beyond, 
                      const Traits& )
{
   typedef typename Traits::Point_2      Point_2;
   typedef typename Traits::Less_xy_2    Less_xy_2;
   std::set<Point_2,Less_xy_2>  point_set;
   int i = 0;
   for (; first != beyond; first++, i++)
      if (!(point_set.insert(*first)).second) return true;
   return false;
}

template <class ForwardIterator>
bool duplicate_points(ForwardIterator first, ForwardIterator beyond)
{
   typedef typename std::iterator_traits<ForwardIterator>::value_type  Point_2;
   typedef typename Kernel_traits<Point_2>::Kernel   K;
   return duplicate_points(first, beyond, K());
}


// Copies the first n points from the input iterator to the output iterator,
// removing any duplicates.  Thus fewer than n points may be inserted into
// the output iterator.
template <class InputIterator, class Size, class OutputIterator, class Traits>
OutputIterator copy_n_unique(InputIterator first, Size n, 
                             OutputIterator result,
                             const Traits& )
{
   typedef typename Traits::Point_2    Point_2;
   typedef typename Traits::Less_xy_2  Less_xy_2;

   std::set<Point_2, Less_xy_2>    sorted_point_set;
   int i;
   for (i = 0; i < n; i++)
   {
      if (sorted_point_set.insert(*first).second)
      {
          *result = *first;
          result++;
      }   
      first++;
   }
   return result;
}

template <class InputIterator, class Size, class OutputIterator>
inline
OutputIterator copy_n_unique(InputIterator first, Size n, 
                             OutputIterator result)
{
   typedef typename std::iterator_traits<InputIterator>::value_type  Point_2;
   typedef typename Kernel_traits<Point_2>::Kernel  K;
   return copy_n_unique(first, n, result, K());
}


} // namespace CGAL

#endif
