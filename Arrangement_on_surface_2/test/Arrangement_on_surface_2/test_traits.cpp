#include <CGAL/basic.h>
#include "test_configuration.h"
#include <iostream>

#if (TEST_TRAITS == CORE_CONIC_TRAITS) && !defined(CGAL_USE_CORE)

int main ()
{
  bool   UNTESTED_TRAITS_AS_CORE_IS_NOT_ISTALLED;
  std::cout << std::endl
            << "WARNING: Core is not installed, "
            << "skipping the test of the conic traits ..."
            << std::endl;
  return 0;
}
#else

#include <CGAL/assertions.h>
#include <CGAL/Arrangement_2.h>

#include "test_traits.h"
#include "Traits_test.h"

int main (int argc, char * argv[])
{
  CGAL::set_error_behaviour(CGAL::THROW_EXCEPTION);
  CGAL::set_warning_behaviour(CGAL::THROW_EXCEPTION);
  Traits_test<Traits> test(argc, argv);
  bool rc  = test.start();
  return (rc) ? 0 : -1;
}

#endif
