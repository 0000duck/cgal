// Copyright (c) 2001-2004  Utrecht University (The Netherlands),
// ETH Zurich (Switzerland), Freie Universitaet Berlin (Germany),
// INRIA Sophia-Antipolis (France), Martin-Luther-University Halle-Wittenberg
// (Germany), Max-Planck-Institute Saarbruecken (Germany), RISC Linz (Austria),
// and Tel-Aviv University (Israel).  All rights reserved.
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
//
// Author(s)     : Sylvain Pion

#ifndef CGAL_MP_FLOAT_H
#define CGAL_MP_FLOAT_H

#include <CGAL/basic.h>
#include <CGAL/Interval_nt.h>
#include <CGAL/Root_of_2.h> // needed because of the [] operation on Root_of_2 
#include <CGAL/MP_Float_fwd.h>
#include <iostream>
#include <vector>
#include <algorithm>

// MP_Float : multiprecision scaled integers.

// Some invariants on the internal representation :
// - zero is represented by an empty vector, and whatever exp.
// - no leading or trailing zero in the vector => unique

// The main algorithms are :
// - Addition/Subtraction
// - Multiplication
// - Comparison
// - to_double() / to_interval()
// - Construction from a double.
// - IOs

// TODO :
// - The exponent really overflows sometimes -> make it multiprecision.
// - Write a generic wrapper that adds an exponent to be used by MP integers.
// - Karatsuba (or other) ?  Would be fun to implement at least.
// - Division, sqrt... : different options :
//   - nothing
//   - convert to double, take approximation, compute over double, reconstruct
//   - exact division as separate function (for Bareiss...)
//   - returns the quotient of the division, forgetting the rest.

CGAL_BEGIN_NAMESPACE

class MP_Float;

MP_Float operator+(const MP_Float &a, const MP_Float &b);
MP_Float operator-(const MP_Float &a, const MP_Float &b);
MP_Float operator*(const MP_Float &a, const MP_Float &b);
#ifdef CGAL_MP_FLOAT_ALLOW_INEXACT
MP_Float operator/(const MP_Float &a, const MP_Float &b);
#endif

Comparison_result
compare (const MP_Float & a, const MP_Float & b);

class MP_Float
{
public:
  typedef Tag_false  Has_gcd;
#ifdef CGAL_MP_FLOAT_ALLOW_INEXACT
  typedef Tag_true   Has_division;
  typedef Tag_true   Has_sqrt;
#else
  typedef Tag_false  Has_division;
  typedef Tag_false  Has_sqrt;
#endif

  typedef Tag_true   Has_exact_ring_operations;
  typedef Tag_false  Has_exact_division;
  typedef Tag_false  Has_exact_sqrt;

  typedef short      limb;
  typedef int        limb2;
  typedef double     exponent_type;

  typedef std::vector<limb>  V;
  typedef V::const_iterator  const_iterator;
  typedef V::iterator        iterator;

private:

  void remove_leading_zeros()
  {
    while (!v.empty() && v.back() == 0)
      v.pop_back();
  }

  void remove_trailing_zeros()
  {
    if (v.empty() || v.front() != 0)
      return;

    iterator i = v.begin();
    for (++i; *i == 0; ++i)
      ;
    exp += i-v.begin();
    v.erase(v.begin(), i);
  }

  // This union is used to convert an unsigned short to a short with
  // the same binary representation, without invoking implementation-defined
  // behavior (standard 4.7.3).
  // It is needed by PGCC, which behaves differently from the others.
  union to_signed {
      unsigned short us;
      short s;
  };

  // The constructors from float/double/long_double are factorized in the
  // following template :
  template < typename T >
  void construct_from_builtin_fp_type(T d);

public:

  // Splits a limb2 into 2 limbs (high and low).
  static
  void split(limb2 l, limb & high, limb & low)
  {
    to_signed l2 = {l};
    low = l2.s;
    high = (l - low) >> (8*sizeof(limb));
  }

  // Given a limb2, returns the higher limb.
  static
  limb higher_limb(limb2 l)
  {
      limb high, low;
      split(l, high, low);
      return high;
  }

  void canonicalize()
  {
    remove_leading_zeros();
    remove_trailing_zeros();
  }

  MP_Float()
      : exp(0)
  {
    CGAL_assertion(sizeof(limb2) == 2*sizeof(limb));
    CGAL_assertion(v.empty());
    // Creates zero.
  }

#if 0
  // Causes ambiguities
  MP_Float(limb i)
  : v(1,i), exp(0)
  {
    remove_leading_zeros();
  }
#endif

  MP_Float(limb2 i)
  : v(2), exp(0)
  {
    split(i, v[1], v[0]);
    canonicalize();
  }

  MP_Float(float d);

  MP_Float(double d);

  MP_Float(long double d);

  MP_Float operator-() const
  {
    return MP_Float() - *this;
  }

  MP_Float& operator+=(const MP_Float &a) { return *this = *this + a; }
  MP_Float& operator-=(const MP_Float &a) { return *this = *this - a; }
  MP_Float& operator*=(const MP_Float &a) { return *this = *this * a; }
#ifdef CGAL_MP_FLOAT_ALLOW_INEXACT
  MP_Float& operator/=(const MP_Float &a) { return *this = *this / a; }
#endif

  exponent_type max_exp() const
  {
    return v.size() + exp;
  }

  exponent_type min_exp() const
  {
    return exp;
  }

  limb of_exp(exponent_type i) const
  {
    if (i < exp || i >= max_exp())
      return 0;
    return v[static_cast<int>(i-exp)];
  }

  bool is_zero() const
  {
    return v.empty();
  }

  Sign sign() const
  {
    if (v.empty())
      return ZERO;
    if (v.back()>0)
      return POSITIVE;
    CGAL_assertion(v.back()<0);
    return NEGATIVE;
  }

  void swap(MP_Float &m)
  {
    std::swap(v, m.v);
    std::swap(exp, m.exp);
  }

  // Converts to a rational type (e.g. Gmpq).
  template < typename T >
  T to_rational() const
  {
    const unsigned log_limb = 8 * sizeof(MP_Float::limb);

    if (is_zero())
      return 0;

    MP_Float::const_iterator i;
    exponent_type exp = min_exp() * log_limb;
    T res = 0;

    for (i = v.begin(); i != v.end(); i++)
    {
      res += CGAL_CLIB_STD::ldexp(static_cast<double>(*i),
                                  static_cast<int>(exp));
      exp += log_limb;
    }

    return res;
  }

  V v;
  exponent_type exp;
};

inline
void swap(MP_Float &m, MP_Float &n)
{ m.swap(n); }

inline
bool operator<(const MP_Float &a, const MP_Float &b)
{ return CGAL_NTS compare(a, b) == SMALLER; }

inline
bool operator>(const MP_Float &a, const MP_Float &b)
{ return b < a; }

inline
bool operator>=(const MP_Float &a, const MP_Float &b)
{ return ! (a < b); }

inline
bool operator<=(const MP_Float &a, const MP_Float &b)
{ return ! (a > b); }

inline
bool operator==(const MP_Float &a, const MP_Float &b)
{ return (a.v == b.v) && (a.v.empty() || (a.exp == b.exp)); }

inline
bool operator!=(const MP_Float &a, const MP_Float &b)
{ return ! (a == b); }

inline
Sign
sign (const MP_Float &a)
{
  return a.sign();
}

MP_Float
square(const MP_Float&);

MP_Float
approximate_sqrt(const MP_Float &d);

MP_Float
approximate_division(const MP_Float &n, const MP_Float &d);

#ifdef CGAL_MP_FLOAT_ALLOW_INEXACT
inline
MP_Float
operator/(const MP_Float &a, const MP_Float &b)
{
  return approximate_division(a, b);
}

inline
MP_Float
sqrt(const MP_Float &d)
{
  return approximate_sqrt(d);
}
#endif

// to_double() returns, not the closest double, but a one bit error is allowed.
// We guarantee : to_double(MP_Float(double d)) == d.
double
to_double(const MP_Float &b);

std::pair<double,double>
to_interval(const MP_Float &b);

template < typename > class Quotient;

// Overloaded in order to protect against overflow.
double
to_double(const Quotient<MP_Float> &b);

std::pair<double, double>
to_interval(const Quotient<MP_Float> &b);

std::pair<double, int>
to_double_exp(const MP_Float &b);

// Returns (first * 2^second), an interval surrounding b.
std::pair<std::pair<double, double>, int>
to_interval_exp(const MP_Float &b);

inline
void
simplify_quotient(MP_Float & numerator, MP_Float & denominator)
{
  // Currently only simplifies the two exponents.
#if 0
  // This better version causes problems as the I/O is changed for
  // Quotient<MP_Float>, which then does not appear as rational 123/345,
  // 1.23/3.45, this causes problems in the T2 test-suite (to be investigated).
  numerator.exp -= denominator.exp 
                    + (MP_Float::exponent_type) denominator.v.size();
  denominator.exp = - (MP_Float::exponent_type) denominator.v.size();
#else
  numerator.exp -= denominator.exp;
  denominator.exp = 0;
#endif
}

inline void simplify_root_of_2(MP_Float &a, MP_Float &b, MP_Float&c) {
#if 0
  if(CGAL::is_zero(a)) {
  	simplify_quotient(b,c); return;
  } else if(CGAL::is_zero(b)) {
  	simplify_quotient(a,c); return;
  } else if(CGAL::is_zero(c)) {
  	simplify_quotient(a,b); return;
  }  	
  MP_Float::exponent_type va = a.exp + 
    (MP_Float::exponent_type) a.v.size();	
  MP_Float::exponent_type vb = b.exp + 
    (MP_Float::exponent_type) b.v.size();
  MP_Float::exponent_type vc = c.exp + 
    (MP_Float::exponent_type) c.v.size();
  MP_Float::exponent_type min = std::min(std::min(va,vb),vc);	
  MP_Float::exponent_type max = std::max(std::max(va,vb),vc);
  MP_Float::exponent_type med = (min+max)/2.0;
  a.exp -= med;
  b.exp -= med;
  c.exp -= med;  
#endif    	
}

namespace CGALi {
  inline void simplify_3_exp(int &a, int &b, int &c) {
    int min = std::min(std::min(a,b),c);	
    int max = std::max(std::max(a,b),c);
    int med = (min+max)/2;
    a -= med;
    b -= med;
    c -= med;
  }
}

inline
double
to_double(const Root_of_2<MP_Float> &r)
{
  const MP_Float &ra = r[2];
  const MP_Float &rb = r[1];

  if(r.is_rational()) {
    std::pair<double, int> n = to_double_exp(rb);
    std::pair<double, int> d = to_double_exp(ra);
    double scale = CGAL_CLIB_STD::ldexp(1.0, n.second - d.second);
    return (n.first / d.first) * scale;
  }

  const MP_Float &rc = r[0];

  if(is_zero(rc)) {
    if(is_negative(rb)) {
      if(r.is_smaller()) return 0.0;
      std::pair<double, int> pa = to_double_exp(ra);
      std::pair<double, int> pb = to_double_exp(rb);
      double scale = CGAL_CLIB_STD::ldexp(1.0, pb.second - pa.second);  
      return -(pb.first / pa.first)*scale;
    } 
    if(r.is_smaller()) {
      std::pair<double, int> pa = to_double_exp(ra);
      std::pair<double, int> pb = to_double_exp(rb);
      double scale = CGAL_CLIB_STD::ldexp(1.0, pb.second - pa.second);  
      return -(pb.first / pa.first)*scale;
    } return 0.0;
  }

  if(is_zero(rb)) {
    std::pair<double, int> pa = to_double_exp(ra);
    std::pair<double, int> pc = to_double_exp(rc);
    double scale = CGAL_CLIB_STD::ldexp(1.0, pc.second - pa.second);
    double m_c_a = -(pc.first / pa.first)*scale;
    if(r.is_smaller()) return -std::sqrt(m_c_a);
    else return std::sqrt(m_c_a);
  }

  std::pair<double, int> pa = to_double_exp(ra);
  std::pair<double, int> pb = to_double_exp(rb);
  std::pair<double, int> pc = to_double_exp(rc);

  CGALi::simplify_3_exp(pa.second,pb.second,pc.second);

  double a = CGAL_CLIB_STD::ldexp(pa.first,pa.second);
  double b = CGAL_CLIB_STD::ldexp(pb.first,pb.second);
  double c = CGAL_CLIB_STD::ldexp(pc.first,pc.second);

  // Maybe it is better to calculate the delta in Exact Computation
  double d = std::sqrt(square(b) - 4*a*c);
  if (r.is_smaller())
    d = -d;

  return (d-b)/(a*2);
}

inline
std::pair<double,double>
to_interval(const Root_of_2<MP_Float> &r)
{

  const MP_Float &ra = r[2];
  const MP_Float &rb = r[1];

  if(r.is_rational()) {
    std::pair<std::pair<double, double>, int> n = to_interval_exp(rb);
    std::pair<std::pair<double, double>, int> d = to_interval_exp(ra);
    return ldexp(Interval_nt<>(n.first) / Interval_nt<>(d.first),
               n.second - d.second).pair();
  }

  const MP_Float &rc = r[0];

  if(is_zero(rc)) {
    if(is_negative(rb)) {
      if(r.is_smaller()) return std::make_pair(0.0,0.0);
      std::pair<std::pair<double, double>, int> pa = to_interval_exp(ra);
      std::pair<std::pair<double, double>, int> pb = to_interval_exp(rb);
      return (-ldexp(Interval_nt<>(pb.first) / Interval_nt<>(pa.first),
               pb.second - pa.second)).pair();
    } 
    if(r.is_smaller()) {
      std::pair<std::pair<double, double>, int> pa = to_interval_exp(ra);
      std::pair<std::pair<double, double>, int> pb = to_interval_exp(rb);
      return (-ldexp(Interval_nt<>(pb.first) / Interval_nt<>(pa.first),
               pb.second - pa.second)).pair();
    } return std::make_pair(0.0,0.0);
  }

  if(is_zero(rb)) {
    std::pair<std::pair<double, double>, int> pa = to_interval_exp(ra);
    std::pair<std::pair<double, double>, int> pc = to_interval_exp(rc);
    const Interval_nt<> m_c_a = -ldexp(Interval_nt<>(pc.first) / Interval_nt<>(pa.first),
               pc.second - pa.second);
    if(r.is_smaller()) return (-CGAL::sqrt(m_c_a)).pair();
    else return (CGAL::sqrt(m_c_a)).pair();
  }

  std::pair<std::pair<double, double>, int> pa = to_interval_exp(ra);
  std::pair<std::pair<double, double>, int> pb = to_interval_exp(rb);
  std::pair<std::pair<double, double>, int> pc = to_interval_exp(rc);

  CGALi::simplify_3_exp(pa.second,pb.second,pc.second);

  Interval_nt<> a = ldexp(Interval_nt<>(pa.first),pa.second);
  Interval_nt<> b = ldexp(Interval_nt<>(pb.first),pb.second);
  Interval_nt<> c = ldexp(Interval_nt<>(pc.first),pc.second);
  Interval_nt<> d = sqrt(square(b) - 4*a*c);
  if (r.is_smaller())
    d = -d;

  return ((d-b)/(a*2)).pair();
}

inline
bool
is_finite(const MP_Float &)
{
  return true;
}

inline
bool
is_valid(const MP_Float &)
{
  return true;
}

inline
io_Operator
io_tag(const MP_Float &)
{
  return io_Operator();
}

std::ostream &
operator<< (std::ostream & os, const MP_Float &b);

// This one is for debug.
std::ostream &
print (std::ostream & os, const MP_Float &b);

std::istream &
operator>> (std::istream & is, MP_Float &b);

CGAL_END_NAMESPACE

#endif // CGAL_MP_FLOAT_H
