// ============================================================================
//
// Copyright (c) 2001-2006 Max-Planck-Institut Saarbruecken (Germany).
// All rights reserved.
//
// This file is part of EXACUS (http://www.mpi-inf.mpg.de/projects/EXACUS/);
// you may redistribute it under the terms of the Q Public License version 1.0.
// See the file LICENSE.QPL distributed with EXACUS.
//
// Licensees holding a valid commercial license may use this file in
// accordance with the commercial license agreement provided with the software.
//
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
//
// ----------------------------------------------------------------------------
//
// Library       : LiS
// File          : test/io.C
// LiS_release   : $Name:  $
// Revision      : $Revision$
// Revision_date : $Date$
//
// Author(s)     : Lutz Kettner  <kettner@mpi-inf.mpg.de>
//
// ============================================================================

#include <CGAL/basic.h>
#include <CGAL/IO/io.h>
#include <CGAL/Testsuite/assert.h>
#include <iostream>
#include <sstream>
#include <cstdlib>


template <class NT>
void test_io(const NT& x){
    NT tmp;
    {
        std::ostringstream os;
        os << x;
        std::istringstream is(os.str());
        is >> tmp;
        CGAL_test_assert_msg( x == tmp, "IO_TEST failed");
    }{
        std::ostringstream os;
        os << ::CGAL::oformat(x);
        std::istringstream is(os.str());
        is >> ::CGAL::iformat(tmp);
        CGAL_test_assert_msg( x == tmp, "IO_TEST failed");
    }{
        std::ostringstream os;
        ::CGAL::write(os,x);
        std::istringstream is(os.str());
        ::CGAL::read(is,tmp);
        CGAL_test_assert_msg( x == tmp, "IO_TEST failed");
    }
}

int main() {

    std::cout << "test_io: short "<< std::endl;
    test_io<short>(12);
    test_io<short>(0);
    test_io<short>(-12);

    std::cout << "test_io: int "<< std::endl;
    test_io<int>(12);
    test_io<int>(0);
    test_io<int>(-12);

    std::cout << "test_io: long int "<< std::endl;
    test_io<long int>(12);
    test_io<long int>(0);
    test_io<long int>(-12);

    std::cout << "test_io: long long int "<< std::endl;
    test_io<long long int>(12);
    test_io<long long int>(0);
    test_io<long long int>(-12);

    std::cout << "test_io: float "<< std::endl;
    test_io<float>(12);
    test_io<float>(0);
    test_io<float>(-12);

    std::cout << "test_io: double "<< std::endl;
    test_io<double>(12);
    test_io<double>(0);
    test_io<double>(-12);

#ifdef CGAL_CFG_NO_LONG_DOUBLE_IO
    std::cout << "test_io: long double is not performed due to compiler bug"<< std::endl;
#else
    std::cout << "test_io: long double "<< std::endl;
    test_io<long double>(12);
    test_io<long double>(0);
    test_io<long double>(-12);
#endif

    //TODO: add more
    std::cout << "ok" <<std::endl;

    return 0;
}
