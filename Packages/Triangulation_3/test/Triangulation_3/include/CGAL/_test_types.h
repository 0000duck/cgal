#ifndef CGAL_TEST_TYPES_H
#define CGAL_TEST_TYPES_H

#include <CGAL/basic.h>
#include <iostream>
#include <cassert>

#ifdef CGAL_USE_LEDA
#include <CGAL/leda_integer.h>
typedef leda_integer my_NT;
#else
#ifdef CGAL_USE_GMP
#include <CGAL/Gmpz.h>
typedef CGAL::Gmpz my_NT;
#else
#include <CGAL/double.h>
typedef double my_NT;
#endif
#endif

#include <CGAL/Cartesian.h>
#include <CGAL/Homogeneous.h>

typedef CGAL::Cartesian<my_NT> Test_rep_cartesian;
typedef CGAL::Homogeneous<my_NT> Test_rep_homogeneous;

class _Triangulation_test_traits;

#endif
