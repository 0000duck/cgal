// Copyright (c) 2003-2006  INRIA Sophia-Antipolis (France).
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
// $URL$
// $Id$
//
// Author(s)     : Monique Teillaud, Sylvain Pion, Julien Hazebrouck

// Partially supported by the IST Programme of the EU as a Shared-cost
// RTD (FET Open) Project under Contract No  IST-2000-26473 
// (ECG - Effective Computational Geometry for Curves and Surfaces) 
// and a STREP (FET Open) Project under Contract No  IST-006413 
// (ACS -- Algorithms for Complex Shapes)

#ifndef CGAL_GLOBAL_FUNCTIONS_ON_LINE_ARCS_2_H
#define CGAL_GLOBAL_FUNCTIONS_ON_LINE_ARCS_2_H

namespace CGAL {

template< class CK >
inline
CGAL::Comparison_result 
compare_y_to_right(const Line_arc_2<CK> &A1,
		   const Line_arc_2<CK> &A2,
		   const bool b)
{
  return CK().compare_y_to_right_2_object()(A1, A2, b);
}

template < class CK >
inline
bool
point_in_x_range(const Line_arc_2<CK> &A, const Circular_arc_point_2<CK> &p) 
{
  return CK().in_x_range_2_object()(A, p);
}

template < class CK >
CGAL::Comparison_result       
compare_y_at_x(const Circular_arc_point_2<CK> &p, const Line_arc_2<CK> &a)
{
  return CK().compare_y_at_x_2_object()(p, a);
}

template < class CK, class OutputIterator >
OutputIterator
make_x_monotone(const Line_arc_2<CK> &A, OutputIterator it)
{
  return CK().make_x_monotone_2_object()(A, it);
}

} // namespace CGAL

#endif // CGAL_GLOBAL_FUNCTIONS_ON_LINE_ARCS_2_H
