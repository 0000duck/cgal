// Copyright (c) 2005  INRIA Sophia-Antipolis (France) 
// All rights reserved.
//
// Authors : Monique Teillaud <Monique.Teillaud@sophia.inria.fr>
//           Sylvain Pion     <Sylvain.Pion@sophia.inria.fr>
// 
// Partially supported by the IST Programme of the EU as a Shared-cost
// RTD (FET Open) Project under Contract No  IST-2000-26473 
// (ECG - Effective Computational Geometry for Curves and Surfaces) 
// and a STREP (FET Open) Project under Contract No  IST-006413 
// (ACS -- Algorithms for Complex Shapes)

#ifndef CGAL_CURVED_KERNEL_FUNCTION_OBJECTS_ON_LINE_2_H
#define CGAL_CURVED_KERNEL_FUNCTION_OBJECTS_ON_LINE_2_H

#include <CGAL/Curved_kernel/internal_functions_on_line_2.h>

namespace CGAL {
namespace LinearFunctors {

  template < class CK >
  class Construct_line_2 : public  CK::Linear_kernel::Construct_line_2
  {
    public:

    typedef typename CK::Line_2 result_type;

    result_type
    operator() ( const typename CK::Polynomial_1_2 &eq )
      {
	return construct_line_2<CK>(eq);
      }
  };

  template < class CK >
  class Get_equation
  {
    public:

    typedef typename CK::Polynomial_1_2 result_type;

    result_type
    operator() ( const typename CK::Line_2 & l )
      {
	return get_equation<CK>(l);
      }
  };

} // namespace LinearFunctors
} // namespace CGAL

#endif // CGAL_CURVED_KERNEL_FUNCTION_OBJECTS_ON_LINE_2_H
