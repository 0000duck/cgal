// Copyright (c) 2003,2004  INRIA Sophia-Antipolis (France) and
// Notre Dame University (U.S.A.).  All rights reserved.
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
// $Source$
// $Revision$ $Date$
// $Name$
//
// Author(s)     : Menelaos Karavelas <mkaravel@cse.nd.edu>




#ifndef CGAL_SQUARE_ROOT_1_H
#define CGAL_SQUARE_ROOT_1_H

#include <CGAL/basic.h>
#include <CGAL/enum.h>

#include <iostream>

#define CHECK_CGAL_PRECONDITIONS 0

CGAL_BEGIN_NAMESPACE


template<class NT>
class Square_root_1
{
private:
  NT x, y, r;

private:
  typedef Square_root_1<NT>   Self;

public:
  typedef NT                  FT;
  typedef NT                  RT;

  Square_root_1() : x(0), y(0), r(0) {}
  Square_root_1(int i) : x(i), y(0), r(0) {}
  Square_root_1(const NT& a) : x(a), y(0), r(0) {}
  Square_root_1(const NT& a, const NT& b, const NT& c)
    : x(a), y(b), r(c)
  {
#if CHECK_CGAL_PRECONDITIONS
    CGAL_assertion( !(CGAL::is_negative(r)) );
#endif
  }

  Square_root_1(const Square_root_1<NT>& other)
    : x(other.x), y(other.y), r(other.r) {}


  NT a() const { return x; }
  NT b() const { return y; }
  NT c() const { return r; }

  Self square() const
  {
    NT xy = x * y;

    return Self(CGAL::square(x) + CGAL::square(y) * r,
		xy + xy,
		r);
  }

  Sign sign() const
  {
    Sign sx = CGAL::sign(x);

    if ( CGAL::sign(r) == ZERO )  { return sx; }

    Sign sy = CGAL::sign(y);

    if ( sx == sy )  { return sx; }
    if ( sx == ZERO )  { return sy; }

    return Sign( sx * CGAL::compare( CGAL::square(x),
				     r * CGAL::square(y) )
		 );
  }

  Self operator+() const
  {
    return (*this);
  }

  Self operator-() const
  {
    return Self(-x, -y, r);
  }

  double to_double() const
  {
    // THIS MUST BE CHECK WITH SYLVAIN FOR CORRECTNESS
    double xd = CGAL::to_double(x);
    double yd = CGAL::to_double(y);
    double rd = CGAL::to_double(r);

    return (xd + yd * CGAL::sqrt(rd));
  }

  std::pair<double,double> to_interval() const
  {
    // THIS MUST BE CHECK WITH SYLVAIN FOR CORRECTNESS
    std::pair<double,double> x_ivl = CGAL::to_interval(x);
    std::pair<double,double> y_ivl = CGAL::to_interval(y);
    std::pair<double,double> r_ivl = CGAL::to_interval(r);

    std::pair<double,double> sqrt_r_ivl(CGAL::sqrt(r_ivl.first),
					CGAL::sqrt(r_ivl.second));

    std::pair<double,double>
      ivl(x_ivl.first + y_ivl.first * sqrt_r_ivl.first,
	  x_ivl.second + y_ivl.second * sqrt_r_ivl.second);

    return ivl;
  }
};


// operator *
template<class NT>
inline
Square_root_1<NT>
operator*(const Square_root_1<NT>& x, const NT& n)
{
  return Square_root_1<NT>(x.a() * n, x.b() * n, x.c());
}


template<class NT>
inline
Square_root_1<NT>
operator*(const NT& n, const Square_root_1<NT>& x)
{
  return (x * n);
}

template<class NT>
inline
Square_root_1<NT>
operator*(const Square_root_1<NT>& x, const Square_root_1<NT>& y)
{
#if CHECK_CGAL_PRECONDITIONS
  CGAL_precondition( CGAL::compare(x.c(), y.c()) == EQUAL );
#endif

  NT a = x.a() * y.a() + x.b() * y.b() * x.c();
  NT b = x.a() * y.b() + x.b() * y.a();

  return Square_root_1<NT>(a, b, x.c());
}


// operator +
template<class NT>
inline
Square_root_1<NT>
operator+(const Square_root_1<NT>& x, const NT& n)
{
  return Square_root_1<NT>(x.a() + n, x.b(), x.c());
}


template<class NT>
inline
Square_root_1<NT>
operator+(const NT& n, const Square_root_1<NT>& x)
{
  return (x + n);
}

template<class NT>
inline
Square_root_1<NT>
operator+(const Square_root_1<NT>& x, const Square_root_1<NT>& y)
{
#if CHECK_CGAL_PRECONDITIONS
  CGAL_precondition( CGAL::compare(x.c(), y.c()) == EQUAL );
#endif

  return Square_root_1<NT>(x.a() + y.a(), x.b() + y.b(), x.c());
}



// operator -
template<class NT>
inline
Square_root_1<NT>
operator-(const Square_root_1<NT>& x, const NT& n)
{
  return x + (-n);
}


template<class NT>
inline
Square_root_1<NT>
operator-(const NT& n, const Square_root_1<NT>& x)
{
  return -(x - n);
}

template<class NT>
inline
Square_root_1<NT>
operator-(const Square_root_1<NT>& x, const Square_root_1<NT>& y)
{
  return (x + (-y));
}





template<class NT>
inline
std::pair<double,double>
to_interval(const Square_root_1<NT>& x)
{
  return x.to_interval();
}


template<class NT>
inline
bool
is_positive(const Square_root_1<NT>& x)
{
  return sign(x) == POSITIVE;
}

template<class NT>
inline
bool
is_negative(const Square_root_1<NT>& x)
{
  return sign(x) == NEGATIVE;
}

template<class NT>
inline
bool
is_zero(const Square_root_1<NT>& x)
{
  return sign(x) == ZERO;
}


template<class NT>
inline
Sign
sign(const Square_root_1<NT>& x)
{
  return x.sign();
}

template<class NT>
inline
Square_root_1<NT>
square(const Square_root_1<NT>& x)
{
  return x.square();
}

template<class NT>
inline
Comparison_result
compare(const Square_root_1<NT>& x, const Square_root_1<NT>& y)
{
#if CHECK_CGAL_PRECONDITIONS
  CGAL_precondition( CGAL::compare(x.c(), y.c()) == EQUAL );
#endif

  Sign s = CGAL::sign(x - y);

  if ( s == ZERO ) { return EQUAL; }
  return (s == POSITIVE) ? LARGER : SMALLER;
}

template<class NT>
inline
double
to_double(const Square_root_1<NT>& x)
{
  return x.to_double();
}

// operator <<
template<class Stream, class NT>
inline
Stream&
operator<<(Stream& os, const Square_root_1<NT>& x)
{
  os << "(" << x.a()  << ")+(" << x.b() << ") sqrt{" << x.c() << "}";
  return os;
}



CGAL_END_NAMESPACE



#endif // CGAL_SQUARE_ROOT_1_H
