// ======================================================================
//
// Copyright (c) 1999 The CGAL Consortium
//
// This software and related documentation is part of an INTERNAL release
// of the Computational Geometry Algorithms Library (CGAL). It is not
// intended for general use.
//
// ----------------------------------------------------------------------
// 
// release       : 
// release_date  : 
// 
// file          : Quotient.h
// package       : Number_types
// revision      : $Revision$
// revision_date : $Date$
//
// author(s)     :
//
// The template class Quotient<NumberType> is based on the LEDA class
// leda_rational written by Stefan Naeher and Christian Uhrig.
// It is basically a templated version with restricted functionality
// of the version of rational in LEDA release 3.3.
// The modification was done by Stefan.Schirra@mpi-sb.mpg.de
//
// coordinator   : MPI, Saarbruecken  (<Stefan.Schirra@mpi-sb.mpg.de>)
// ======================================================================

#ifndef CGAL_QUOTIENT_H
#define CGAL_QUOTIENT_H

#include <CGAL/basic.h>

#include <utility>

#ifndef CGAL_CFG_NO_LOCALE
#  include <locale>
#else
#  include <cctype>
#endif

#include <CGAL/IO/io_tags.h>

CGAL_BEGIN_NAMESPACE

template <class NumberType> class Quotient;
CGAL_TEMPLATE_NULL  class Quotient<int>;

template <class NumberType>
double
to_double(const Quotient<NumberType>& q);

template <class NumberType>
CGAL_KERNEL_INLINE
bool
is_valid(const Quotient<NumberType>& q);

template <class NumberType>
CGAL_KERNEL_INLINE
bool
is_finite(const Quotient<NumberType>& q);

template <class NumberType>
CGAL_KERNEL_INLINE
NumberType
numerator(const Quotient<NumberType>& q);

template <class NumberType>
CGAL_KERNEL_INLINE
NumberType
denominator(const Quotient<NumberType>& q);

template <class NumberType>
inline
Quotient_tag
number_type_tag(const Quotient<NumberType>& q);

template < class NumberType >
inline
io_Operator
io_tag(const Quotient<NumberType>&);

template <class NumberType>
CGAL_KERNEL_MEDIUM_INLINE
Comparison_result
quotient_cmp(const Quotient<NumberType>& q, const Quotient<NumberType>& r);

template <class NumberType>
CGAL_KERNEL_INLINE
Quotient<NumberType>
operator+ (const Quotient<NumberType>& q, const Quotient<NumberType>& r);

template <class NumberType>
CGAL_KERNEL_INLINE
Quotient<NumberType>
operator+ (const NumberType& q, const Quotient<NumberType>& r);

template <class NumberType>
CGAL_KERNEL_INLINE
Quotient<NumberType>
operator+ (const Quotient<NumberType>& q, const NumberType& r);

template <class NumberType>
CGAL_KERNEL_INLINE
Quotient<NumberType>
operator- (const Quotient<NumberType>& q, const Quotient<NumberType>& r);

template <class NumberType>
CGAL_KERNEL_INLINE
Quotient<NumberType>
operator- (const NumberType& q, const Quotient<NumberType>& r);

template <class NumberType>
CGAL_KERNEL_INLINE
Quotient<NumberType>
operator- (const Quotient<NumberType>& q, const NumberType& r);

template <class NumberType>
CGAL_KERNEL_INLINE
Quotient<NumberType>
operator* (const Quotient<NumberType>& q, const Quotient<NumberType>& r);

template <class NumberType>
CGAL_KERNEL_INLINE
Quotient<NumberType>
operator* (const NumberType& q, const Quotient<NumberType>& r);

template <class NumberType>
CGAL_KERNEL_INLINE
Quotient<NumberType>
operator* (const Quotient<NumberType>& q, const NumberType& r);

template <class NumberType>
CGAL_KERNEL_INLINE
Quotient<NumberType>
operator/ (const Quotient<NumberType>& q, const Quotient<NumberType>& r);

template <class NumberType>
CGAL_KERNEL_INLINE
Quotient<NumberType>
operator/ (const NumberType& q, const Quotient<NumberType>& r);

template <class NumberType>
CGAL_KERNEL_INLINE
Quotient<NumberType>
operator/ (const Quotient<NumberType>& q, const NumberType& r);

template <class NumberType>
inline
Quotient<NumberType>
operator- (const Quotient<NumberType>&);

template <class NumberType>
CGAL_KERNEL_INLINE
bool
operator==(const Quotient<NumberType>& q, const Quotient<NumberType>& r);

template <class NumberType>
CGAL_KERNEL_INLINE
bool
operator==(const Quotient<NumberType>& q, const NumberType& a);

template <class NumberType>
CGAL_KERNEL_INLINE
bool
operator==(const NumberType& n, const Quotient<NumberType>& q);

template <class NumberType>
CGAL_KERNEL_INLINE
bool
operator< (const Quotient<NumberType>& q, const Quotient<NumberType>& r);

template <class NumberType>
CGAL_KERNEL_INLINE
bool
operator< (const Quotient<NumberType>& q, const NumberType& r);

template <class NumberType>
CGAL_KERNEL_INLINE
bool
operator< (const NumberType& q, const Quotient<NumberType>& r);

template <class NumberType>
CGAL_KERNEL_INLINE
bool
operator!=(const Quotient<NumberType>& q, const Quotient<NumberType>& r);

template <class NumberType>
CGAL_KERNEL_INLINE
bool
operator<=(const Quotient<NumberType>& q, const Quotient<NumberType>& r);

template <class NumberType>
CGAL_KERNEL_INLINE
bool
operator> (const Quotient<NumberType>& q, const Quotient<NumberType>& r);

template <class NumberType>
CGAL_KERNEL_INLINE
bool
operator>=(const Quotient<NumberType>& q, const Quotient<NumberType>& r);

template <class NumberType>
CGAL_KERNEL_INLINE
bool
operator!=(const Quotient<NumberType>& q, const NumberType& a);

template <class NumberType>
CGAL_KERNEL_INLINE
bool
operator!=(const NumberType& n, const Quotient<NumberType>& q);

template <class NumberType>
CGAL_KERNEL_INLINE
bool
operator<=(const Quotient<NumberType>& q, const NumberType& r);

template <class NumberType>
CGAL_KERNEL_INLINE
bool
operator<=(const NumberType& q, const Quotient<NumberType>& r);

template <class NumberType>
CGAL_KERNEL_INLINE
bool
operator> (const Quotient<NumberType>& q, const NumberType& r);

template <class NumberType>
CGAL_KERNEL_INLINE
bool
operator> (const NumberType& q, const Quotient<NumberType>& r);

template <class NumberType>
CGAL_KERNEL_INLINE
bool
operator>=(const Quotient<NumberType>& q, const NumberType& r);

template <class NumberType>
CGAL_KERNEL_INLINE
bool
operator>=(const NumberType& q, const Quotient<NumberType>& r);

template <class NumberType>
std::istream&
operator>>(std::istream& in, Quotient<NumberType>& q);

template <class NumberType>
std::ostream&
operator<<(std::ostream& out, const Quotient<NumberType>& q);

template <class NumberType>
CGAL_KERNEL_INLINE
NumberType
quotient_truncation(const Quotient<NumberType>& r);

template <class NumberType>
NumberType
gcd(const NumberType& , const NumberType&);

template <class NumberType>
class Quotient
{
 public:
  typedef NumberType NT;
  
  Quotient() : num( NT(0) ), den( NT(1) ) {}
  
  Quotient(const NT& n) : num(n), den( NT(1) ) {}
  
  Quotient(int i) : num(NT(i)), den( NT(1) ) {}
  
  Quotient(const NT& n, const NT& d) : num(n), den(d)
  { CGAL_kernel_precondition( d!= NT(0) ); }
  
  Quotient(const Quotient<NT>& r) : num(r.num), den(r.den) {}
  
  ~Quotient() {}
  
  Quotient<NT>& operator+= (const Quotient<NT>& r);
  Quotient<NT>& operator-= (const Quotient<NT>& r);
  Quotient<NT>& operator*= (const Quotient<NT>& r);
  Quotient<NT>& operator/= (const Quotient<NT>& r);
  Quotient<NT>& operator+= (const NT& r);
  Quotient<NT>& operator-= (const NT& r);
  Quotient<NT>& operator*= (const NT& r);
  Quotient<NT>& operator/= (const NT& r);
  
  Quotient<NT>&    normalize();
  
  NT   numerator() const;
  NT   denominator() const;

 public:
  NT   num;
  NT   den;
};

template <class RT>
Interval_base
to_interval (const Quotient<RT>& z)
{
  return Interval_nt<>(CGAL::to_interval(z.numerator())) /
         Interval_nt<>(CGAL::to_interval(z.denominator()));
}

CGAL_END_NAMESPACE

#ifdef CGAL_CFG_NO_AUTOMATIC_TEMPLATE_INCLUSION
#include <CGAL/Quotient.C>
#endif // CGAL_CFG_NO_AUTOMATIC_TEMPLATE_INCLUSION

CGAL_BEGIN_NAMESPACE

CGAL_TEMPLATE_NULL
class Quotient<int>
{
 public:
  Quotient() : num( 0 ), den( 1 ) {}
  Quotient(int i) : num( i ), den( 1 ) {}
  Quotient(const int& n, const int& d) : num(n), den(d)
  { CGAL_kernel_precondition( d!= 0 ); }
  Quotient(const Quotient<int>& r) : num(r.num), den(r.den) {}
  ~Quotient() {}
  
  int
  numerator() const { return num; }
  
  int
  denominator() const { return den; }
  
  Quotient<int>&
  operator+= (const Quotient<int>& r)
  {
    num = num * r.den + r.num * den;
    den *= r.den;
    return (*this);
  }
  
  Quotient<int>&
  operator-= (const Quotient<int>& r)
  {
    num = num * r.den - r.num * den;
    den *= r.den;
    return (*this);
  }
  
  Quotient<int>&
  operator*= (const Quotient<int>& r)
  {
    num *= r.num;
    den *= r.den;
    return (*this);
  }
  
  Quotient<int>&
  operator/= (const Quotient<int>& r)
  {
    CGAL_kernel_precondition( r.num != 0 );
    num *= r.den;
    den *= r.num;
    return (*this);
  }
  Quotient<int>&
  operator+= (const int& r)
  {
    num = num + r * den;
    return (*this);
  }
  
  Quotient<int>&
  operator-= (const int& r)
  {
    num = num - r * den;
    return (*this);
  }
  
  Quotient<int>&
  operator*= (const int& r)
  {
    num *= r ;
    return (*this);
  }
  
  Quotient<int>&
  operator/= (const int& r)
  {
    CGAL_kernel_precondition( r != 0 );
    den *= r ;
    return (*this);
  }
  
 public:
  int num;
  int den;
};

CGAL_END_NAMESPACE

#include <CGAL/iterator_traits_pointer_specs_for_cartesian_quotient.h>

#endif // CGAL_QUOTIENT_H
