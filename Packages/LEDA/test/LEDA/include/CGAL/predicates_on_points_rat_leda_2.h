// Copyright (c) 1999  Utrecht University (The Netherlands),
// ETH Zurich (Switzerland), Freie Universitaet Berlin (Germany),
// INRIA Sophia-Antipolis (France), Martin-Luther-University Halle-Wittenberg
// (Germany), Max-Planck-Institute Saarbrucken (Germany), RISC Linz (Austria),
// and Tel-Aviv University (Israel).  All rights reserved.
//
// This file is part of CGAL (www.cgal.org); you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License as
// published by the Free Software Foundation; version 2.1 of the License.
// See the file LICENSE.LGPL distributed with CGAL.
//
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
//
// $Source$
// $Revision$ $Date$
// $Name$
//
// Author(s)     : Stefan Schirra
 

#ifndef CGAL_PREDICATES_ON_POINTS_RAT_LEDA_2_H
#define CGAL_PREDICATES_ON_POINTS_RAT_LEDA_2_H

#include <CGAL/rat_leda.h>
#include <CGAL/predicates_on_points_2.h>

CGAL_BEGIN_NAMESPACE


template <class use_rat_leda_kernel>
inline
Orientation
orientation(const Point_2<use_rat_leda_kernel>& p,
            const Point_2<use_rat_leda_kernel>& q,
            const Point_2<use_rat_leda_kernel>& r)
{
  typedef typename use_rat_leda_kernel::Point_2_base  RPoint_2;
  return static_cast<Orientation>( CGAL_LEDA_SCOPE::orientation((const RPoint_2&)p,
                                                 (const RPoint_2&)q,
                                                 (const RPoint_2&)r) );
/*
  return static_cast<Orientation>(
      ::orientation((const use_rat_leda_kernel::Point_2_base&)p,
                    (const use_rat_leda_kernel::Point_2_base&)q,
                    (const use_rat_leda_kernel::Point_2_base&)r) );
*/
}

// #ifdef CGAL_CFG_NO_KOENIG_LOOKUP
template <class use_rat_leda_kernel>
inline
bool
collinear(const Point_2<use_rat_leda_kernel>& p,
          const Point_2<use_rat_leda_kernel>& q,
          const Point_2<use_rat_leda_kernel>& r)
{

  typedef typename use_rat_leda_kernel::Point_2_base  RPoint_2;
  return CGAL_LEDA_SCOPE::collinear((const RPoint_2&)p,
                     (const RPoint_2&)q,
                     (const RPoint_2&)r );
/*
  return
      ::collinear((const use_rat_leda_kernel::Point_2_base&)p,
                  (const use_rat_leda_kernel::Point_2_base&)q,
                  (const use_rat_leda_kernel::Point_2_base&)r);
*/
}
// #endif // CGAL_CFG_NO_KOENIG_LOOKUP

CGAL_END_NAMESPACE

#endif // CGAL_PREDICATES_ON_POINTS_RAT_LEDA_2_H
