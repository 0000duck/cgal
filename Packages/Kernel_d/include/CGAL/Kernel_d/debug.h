// ======================================================================
//
// Copyright (c) 1997-2000 The CGAL Consortium
//
// This software and related documentation is part of an INTERNAL release
// of the Computational Geometry Algorithms Library (CGAL). It is not
// intended for general use.
//
// ----------------------------------------------------------------------
//
// release       : $CGAL_Revision: CGAL-2.4-I-64 $
// release_date  : $CGAL_Date: 2002/03/18 $
//
// file          : include/CGAL/Kernel_d/debug.h
// package       : Kernel_d (0.9.54)
// maintainer    : Michael Hoffmann <hoffmann@inf.ethz.ch>
// chapter       : Basic
//
// revision      : $Revision$
// revision_date : $Date$
//
// author(s)     : Michael Seel <seel@mpi-sb.mpg.de>
// coordinator   : ETH Zurich
//
// implementation: debugging stuff
// ======================================================================
#ifndef CGAL_DEBUG_H
#define CGAL_DEBUG_H

#include <iostream>
#include <string>
#include <sstream>

#undef TRACE
#undef TRACEN
#undef TRACEV
#undef CTRACE
#undef CTRACEN
#undef ASSERT

static int debugthread=3141592;
namespace {
    struct Avoid_warning_for_unused_debugthread { static int x; };
    int Avoid_warning_for_unused_debugthread::x = debugthread;
}

#if _DEBUG>0
#define SETDTHREAD(l) debugthread=l
#else
#define SETDTHREAD(l)
#endif

#if _DEBUG>0
#define TRACE(t)   if((debugthread%_DEBUG)==0)\
 std::cerr<<" "<<t;std::cerr.flush()
#else
#define TRACE(t) 
#endif

#if _DEBUG>0
#define TRACEV(t)  if((debugthread%_DEBUG)==0)\
 std::cerr<<" "<<#t<<" = "<<(t)<<std::endl;std::cerr.flush()
#else
#define TRACEV(t) 
#endif

#if _DEBUG>0
#define TRACEN(t)  if((debugthread%_DEBUG)==0)\
 std::cerr<<" "<<t<<std::endl;std::cerr.flush()
#else
#define TRACEN(t) 
#endif

#if _DEBUG>0
#define CTRACE(b,t)  if(b) std::cerr << " " << t; else std::cerr << " 0"
#else
#define CTRACE(b,t) 
#endif

#if _DEBUG>0
#define CTRACEN(b,t)  if(b) std::cerr<< " " <<t<<"\n"; else std::cerr<<" 0\n"
#else
#define CTRACEN(b,t) 
#endif

#ifndef _ASSERT
#define  ASSERT(cond,fstr) 
#else
#define ASSERT(cond,fstr)   \
  if (!(cond)) {       \
    std::cerr << "   ASSERT:   " << #fstr << endl; \
    std::cerr << "   COND:     " << #cond << endl; \
    std::cerr << "   POSITION: " << __FILE__ << " at line "<< __LINE__ \
              <<std::endl; \
    abort();           \
  }
#endif

#define forall_iterators(x,S)\
for(x = S.begin(); x != S.end(); ++x) 

namespace MSDEBUG {

template <typename C>
void print_elements(const C& container)
{ typename C::const_iterator it;
  forall_iterators(it,container)
    std::cerr << *it << " ";
}

template <typename I>
void print(I s, I e, std::ostream& os = std::cerr)
{ while(s!=e) os<<*s++<<" "; }

template <class T>
std::string make_std_string(const T& t)
{ std::ostringstream os;
  os << t;
  std::string res(os.str()); 
  return res; 
}
} // MSDEBUG


#endif //CGAL_DEBUG_H


