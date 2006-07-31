#ifndef CGAL_SDG_TEST_H
#define CGAL_SDG_TEST_H 1

#include "IO/io_aux.h"
#include "test_types.h"

CGAL_BEGIN_NAMESPACE

template<class InputStream>
bool test_x(InputStream& is, char* ifname, bool test_remove = true)
{
  SDG2 sdg;
  return test_base(is, sdg, ifname, "sdg.tmp", test_remove);
}


template<class InputStream>
bool test_no_x(InputStream& is, char* ifname, bool test_remove = true)
{
  SDG2_wi sdg;
  return test_base(is, sdg, ifname, "sdg_wi.tmp", test_remove);
}

//=====================================================================

template<class InputStream>
bool test_hierarchy_x(InputStream& is, char* ifname, bool test_remove = true)
{
  SDG2 sdg;
  return test_base(is, sdg, ifname, "sdgh.tmp", test_remove);
}


template<class InputStream>
bool test_hierarchy_no_x(InputStream& is, char* ifname, bool test_remove = true)
{
  SDG2_wi sdg;
  return test_base(is, sdg, ifname, "sdgh_wi.tmp", test_remove);
}


//=====================================================================


template<class SDG, class InputStream>
bool test_base(InputStream& is, const SDG& sdg, char* ifname, char* ofname,
	       bool test_remove)
{
  print_separator();

  bool types_ok = CGAL::test_sdg(std::cin, sdg, ifname, ofname, test_remove);

  assert_no_warning( types_ok );

  print_separator();

  std::cout << std::endl;

  return types_ok;
}




CGAL_END_NAMESPACE


#endif // CGAL_SDG_TEST_H
