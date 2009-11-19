// Copyright (c) 2009 Inria Lorraine (France). All rights reserved.
// 
// This file is part of CGAL (www.cgal.org); you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License as
// published by the Free Software Foundation; version 2.1 of the License.
// See the file LICENSE.LGPL distributed with CGAL.
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
// Author: Luis Pe�aranda <luis.penaranda@loria.fr>

#ifndef CGAL_TEST_IO_H
#define CGAL_TEST_IO_H

#include <CGAL/basic.h>
#ifdef NDEBUG
#  undef NDEBUG
#  include <cassert>
#  define NDEBUG 1
#endif

namespace CGAL{

// construct a _NT from a value of type _CT, output it to a stream, read
// it again and check that the number is the same
template <class _NT,class _CT>
void test_io(_CT x){
        typedef _NT     NT;
        typedef _CT     CT;

        NT a(x);
        std::stringstream ss;
        CGAL::set_ascii_mode(ss);
        ss<<CGAL::oformat(a);
        ss>>a;
        assert(a==NT(x));
}

// construct by default, write it to stdout and read it again
// (use with caution: when constructing by default, the value of the
// number may be undefined and the test may fail, what does not mean
// that the i/o functions are incorrect)
template <class _NT>
void test_io(){
        typedef _NT     NT;

        NT a;
        std::stringstream ss;
        CGAL::set_ascii_mode(ss);
        ss<<CGAL::oformat(a);
        ss>>a;
        assert(a==NT());
}

} // namespace CGAL

#endif

// vim: tabstop=8: softtabstop=8: smarttab: shiftwidth=8: expandtab
