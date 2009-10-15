// Copyright (c) 2009  INRIA Sophia-Antipolis (France).
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
// $URL:  $
// $Id:  $
//
// Author(s)     : S�bastien Loriot

#ifndef CGAL_INTERNAL_STATIC_FILTERS_REGULAR_TRIANGULATION_STATIC_FILTERS_TRAITS_3_H
#define CGAL_INTERNAL_STATIC_FILTERS_REGULAR_TRIANGULATION_STATIC_FILTERS_TRAITS_3_H
  
  
  
#include <CGAL/internal/Static_filters/Power_test_3.h>
  
namespace CGAL{ namespace internal {
  
template <class K_base>  
class Regular_triangulation_static_filters_traits_3:public K_base{

public:  
  typedef Static_filters_predicates::Power_test_3<K_base>                          Power_test_3;

  
 
  Power_test_3 power_test_3_object() const
  { return Power_test_3();}

};
  
  
  
} } //namespace CGAL::internal
  
#endif //CGAL_INTERNAL_STATIC_FILTERS_REGULAR_TRIANGULATION_STATIC_FILTERS_TRAITS_3_H