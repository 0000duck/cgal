#include <CGAL/basic.h>

#include <iostream>
#include <fstream>
#include <cassert>

#define DONT_USE_FILTERED_EXACT

// choose number type
#include <CGAL/MP_Float.h>
#ifndef DONT_USE_FILTERED_EXACT
#  include <CGAL/Filtered_exact.h>
#endif

typedef double         inexact_type;
typedef CGAL::MP_Float exact_type;

#ifndef DONT_USE_FILTERED_EXACT
typedef CGAL::Filtered_exact<inexact_type,exact_type>  number_t;
#endif

#include <CGAL/Simple_cartesian.h>

#ifndef DONT_USE_FILTERED_EXACT
struct Kernel : public CGAL::Simple_cartesian<number_t> {};
#endif

#include <CGAL/Number_type_traits.h>

typedef CGAL::Ring_tag Method_tag;

#include "./include/test.h"


struct CK : public CGAL::Simple_cartesian<inexact_type> {};
struct EK : public CGAL::Simple_cartesian<exact_type> {};


int main(int argc, char* argv[])
{
#ifndef DONT_USE_FILTERED_EXACT
  {
    std::ifstream ifs_algo("./data/algo.dat");

    assert( ifs_algo );

    std::cout << "testing the Apollonius graph class..." << std::flush;
    bool algo_ok =
      CGAL::test_algo<Kernel,Method_tag,std::ifstream>(ifs_algo);

    assert( algo_ok );
    std::cout << " done!" << std::endl;

    ifs_algo.close();

    std::cout << std::endl;
  }
#endif
  //------------------------------------------------------------------------

  {
    std::ifstream ifs_algo("./data/algo.dat");

    assert( ifs_algo );

    std::cout << "testing the Apollonius graph class"
	      << " with filtered traits..." << std::flush;
    bool algo_ok =
      CGAL::test_filtered_traits_algo<CK,Method_tag,EK,Method_tag,
      std::ifstream>(ifs_algo);

    assert( algo_ok );
    std::cout << " done!" << std::endl;

    ifs_algo.close();

    std::cout << std::endl;
  }

  return 0;
}
