#include <CGAL/basic.h>
#include "test_configuration.h"
#include <iostream>

#if ((TEST_GEOM_TRAITS == CORE_CONIC_GEOM_TRAITS) || \
     (TEST_GEOM_TRAITS == BEZIER_GEOM_TRAITS) || \
     (TEST_GEOM_TRAITS == RATIONAL_ARC_GEOM_TRAITS)) && !defined(CGAL_USE_CORE)

int main()
{
//  bool   UNTESTED_GEOM_TRAITS_AS_CORE_IS_NOT_INSTALLED;
  std::cout << std::endl
            << "NOTE: Core is not installed, "
            << "skipping the test ..."
            << std::endl;
  return 0;
}
#elif (TEST_GEOM_TRAITS == ALGEBRAIC_GEOM_TRAITS) && \
  (TEST_NT == LEDA_INT_NT || TEST_NT == LEDA_RAT_NT) && \
  (! CGAL_USE_LEDA)

int main()
{
//  bool   UNTESTED_GEOM_TRAITS_AS_LEDA_IS_NOT_INSTALLED;
  std::cout << std::endl
	    << "NOTE: LEDA is not installed, "
            << "skipping the test ..."
            << std::endl;
  return 0;
}

#elif (TEST_GEOM_TRAITS == ALGEBRAIC_GEOM_TRAITS) && \
  (TEST_NT == CGAL_GMPZ_NT || TEST_NT == CGAL_GMPQ_NT) && \
  ! (CGAL_USE_GMP && CGAL_USE_MPFI)

int main()
{

//  bool   UNTESTED_GEOM_TRAITS_AS_GMP_OR_MPFI_IS_NOT_INSTALLED;
  std::cout << std::endl
	    << "NOTE: GMP and/or MPFI are not installed, "
            << "skipping the test ..."
            << std::endl;
  return 0;
}

#elif (TEST_GEOM_TRAITS == ALGEBRAIC_GEOM_TRAITS) && \
  (TEST_NT == CORE_INT_NT) && \
  !CGAL_USE_CORE

int main()
{
//  bool   UNTESTED_GEOM_TRAITS_AS_CORE_IS_NOT_INSTALLED;
  std::cout << std::endl
	    << "NOTE: CORE is not installed, "
            << "skipping the test ..."
            << std::endl;
  return 0;
}


#else

#include "test_geom_traits.h"
// Define Base_geom_traits to be the geometry traits, namely, Geom_traits.
typedef Base_geom_traits                Geom_traits;
typedef Geom_traits::Point_2            Point_2;
typedef Geom_traits::Curve_2            Curve_2;
typedef Geom_traits::X_monotone_curve_2 X_monotone_curve_2;
#include "Traits_test.h"

int main(int argc, char* argv[])
{
#if TEST_GEOM_TRAITS == ALGEBRAIC_GEOM_TRAITS
  CGAL::set_pretty_mode(std::cout);
  CGAL::set_pretty_mode(std::cerr);
#endif
  std::cout<< "**** 1 Geom_traits type: " << GEOM_TRAITS_TYPE << std::endl;
  std::cout<< "Test_geom_trait = " << TEST_GEOM_TRAITS << std:: endl;
  Geom_traits traits;

  Traits_test<Geom_traits> test(traits);
  
  //if (!test.parse(argc, argv)) return -1;
  
  //if (!test.init()) return -1;  //polycurves are made somewhere here.
  
  //if (!test.perform()) return -1;
  // std::cout<< "**** 2 Geom_traits type: " << GEOM_TRAITS_TYPE << std::endl;
  return 0;
}

#endif
