// Copyright (c) 2006 Inria Lorraine (France). All rights reserved.
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
// $URL:  $
// $Id:  $
// 
//
// Author(s)     : Luis Peñaranda <penarand@loria.fr>

#ifndef CGAL_MPFIINTERVAL_H
#define CGAL_MPFIINTERVAL_H

#include <CGAL/basic.h>
#include <CGAL/assertions.h>
#include <CGAL/Gmpz.h>
#include <CGAL/Gmpq.h>
#include <iostream>
#include <mpfi.h>
#include <mpfi_io.h>
#include <mpfr.h>

CGAL_BEGIN_NAMESPACE

// The representation of intervals. TODO: think about reference counting
class MpfiInterval_rep {
public:
	mpfi_t mpfI;

	MpfiInterval_rep () { mpfi_init (mpfI); }
	~MpfiInterval_rep () { mpfi_clear (mpfI); }

private:
	// Make sure it does not get accidentally copied.
	MpfiInterval_rep (const MpfiInterval_rep &);
	MpfiInterval_rep & operator= (const MpfiInterval_rep &);
};

// The class of the MPFI intervals. It's a model of the RingNumberType concept
class MpfiInterval
: Handle_for<MpfiInterval_rep>,
	boost::field_operators1< MpfiInterval ,
		boost::field_operators2< MpfiInterval, int > > {

	typedef Handle_for<MpfiInterval_rep> Base;
public:
	typedef CGAL::Tag_false	Has_gcd;
	typedef CGAL::Tag_true	Has_division;
	typedef CGAL::Tag_true	Has_sqrt;

	typedef CGAL::Tag_true	Has_exact_ring_operations;
	typedef CGAL::Tag_true	Has_exact_division;
	typedef CGAL::Tag_false	Has_exact_sqrt;

	// constructors
	MpfiInterval ();
	MpfiInterval (int);
	MpfiInterval (unsigned int);
	MpfiInterval (long int);
	MpfiInterval (unsigned long int);
	MpfiInterval (double);
	MpfiInterval (const CGAL::Gmpq &);
	MpfiInterval (const CGAL::Gmpz &);
	MpfiInterval (const mpfi_t &);
	MpfiInterval (const MpfiInterval &);

	MpfiInterval operator= (const long int);
	MpfiInterval operator= (const CGAL::Gmpz &);
	MpfiInterval operator= (const CGAL::Gmpq &);
	MpfiInterval operator= (const MpfiInterval &);

	// destructor
	/* not needed
	~MpfiInterval ();
	*/

	// functions related to the member data
	inline const mpfi_t & mpfi () const;
	inline mpfi_t & mpfi ();
	inline void set_prec (mp_prec_t);
	inline mp_prec_t get_prec ();
	inline void get_left (mpfr_t &);
	inline void get_right (mpfr_t &);

	// Arithmetic functions required by RingNumberType:
	// 1. comparisons between MpfiInterval's
	// 2. comparisons with int's
	// 3. arithmetic between MpfiInterval's
	// 4. arithmetic with int's
	// 5. accesory functions
	// 6. extra arithmetic functions (not required)
	//
	// Arithmetic functions required by FieldNumberType:
	// 7. division functions
	//
	// Arithmetic functions required by SqrtFieldNumberType:
	// 8. square root
	//
	// I/O functions:
	// 9. <<
	//
	// 10. misc functions
	// 11. all functions with mpfr_t

	// 1
	// 2
	bool operator== (const int) const;
	bool operator!= (const int) const;
	bool operator< (const int) const;
	bool operator> (const int) const;
	bool operator<= (const int) const;
	bool operator>= (const int) const;
	bool operator== (const CGAL::Gmpz &) const;
	bool operator!= (const CGAL::Gmpz &) const;
	bool operator< (const CGAL::Gmpz &) const;
	bool operator> (const CGAL::Gmpz &) const;
	bool operator<= (const CGAL::Gmpz &) const;
	bool operator>= (const CGAL::Gmpz &) const;
	bool operator== (const CGAL::Gmpq &) const;
	bool operator!= (const CGAL::Gmpq &) const;
	bool operator< (const CGAL::Gmpq &) const;
	bool operator> (const CGAL::Gmpq &) const;
	bool operator<= (const CGAL::Gmpq &) const;
	bool operator>= (const CGAL::Gmpq &) const;
	// 3
	MpfiInterval operator+ (const MpfiInterval &) const;
	MpfiInterval operator- (const MpfiInterval &) const;
	MpfiInterval operator* (const MpfiInterval &) const;
	MpfiInterval operator- () const;
	MpfiInterval operator+= (const MpfiInterval &);
	MpfiInterval operator-= (const MpfiInterval &);
	MpfiInterval operator*= (const MpfiInterval &);
	// 4
	MpfiInterval operator+ (const int) const;
	MpfiInterval operator- (const int) const;
	MpfiInterval operator* (const int) const;
	MpfiInterval operator+= (const int);
	MpfiInterval operator-= (const int);
	MpfiInterval operator*= (const int);
	MpfiInterval operator+ (const CGAL::Gmpz &) const;
	MpfiInterval operator- (const CGAL::Gmpz &) const;
	MpfiInterval operator* (const CGAL::Gmpz &) const;
	MpfiInterval operator+= (const CGAL::Gmpz &);
	MpfiInterval operator-= (const CGAL::Gmpz &);
	MpfiInterval operator*= (const CGAL::Gmpz &);
	MpfiInterval operator+ (const CGAL::Gmpq &) const;
	MpfiInterval operator- (const CGAL::Gmpq &) const;
	MpfiInterval operator* (const CGAL::Gmpq &) const;
	MpfiInterval operator+= (const CGAL::Gmpq &);
	MpfiInterval operator-= (const CGAL::Gmpq &);
	MpfiInterval operator*= (const CGAL::Gmpq &);
	// 5: the required functions are outside the class 
	// 6
	bool is_valid () const;
	bool is_finite () const;
	double to_double () const;
	std::pair <double,double> to_interval () const;
	// 7
	MpfiInterval operator/ (const MpfiInterval &) const;
	MpfiInterval operator/ (const int) const;
	MpfiInterval operator/= (const MpfiInterval &);
	MpfiInterval operator/= (const int);
	// 8
	MpfiInterval sqrt () const;
	// 9
	std::ostream& show (std::ostream &);
	// 10
	MpfiInterval operator+ (const mpz_t &) const;
	MpfiInterval operator- (const mpz_t &) const;
	MpfiInterval operator* (const mpz_t &) const;
	MpfiInterval operator+= (const mpz_t &);
	MpfiInterval operator-= (const mpz_t &);
	MpfiInterval operator*= (const mpz_t &);
	MpfiInterval operator+ (const mpq_t &) const;
	MpfiInterval operator- (const mpq_t &) const;
	MpfiInterval operator* (const mpq_t &) const;
	MpfiInterval operator+= (const mpq_t &);
	MpfiInterval operator-= (const mpq_t &);
	MpfiInterval operator*= (const mpq_t &);

	// 11
	MpfiInterval (const mpfr_t &);	// constructor
	MpfiInterval operator= (const mpfr_t &);	// assigning
	// comparison
	bool operator== (const mpfr_t &) const;
	bool operator!= (const mpfr_t &) const;
	bool operator< (const mpfr_t &) const;
	bool operator> (const mpfr_t &) const;
	bool operator<= (const mpfr_t &) const;
	bool operator>= (const mpfr_t &) const;
	// arithmetics
	MpfiInterval operator+ (const mpfr_t &) const;
	MpfiInterval operator- (const mpfr_t &) const;
	MpfiInterval operator* (const mpfr_t &) const;
	MpfiInterval operator/ (const mpfr_t &);
	MpfiInterval operator+= (const mpfr_t &);
	MpfiInterval operator-= (const mpfr_t &);
	MpfiInterval operator*= (const mpfr_t &);
	MpfiInterval operator/= (const mpfr_t &);
};

std::ostream& operator<< (std::ostream &, MpfiInterval &);

CGAL_END_NAMESPACE

#ifdef CGAL_CFG_NO_AUTOMATIC_TEMPLATE_INCLUSION
#include <CGAL/MpfiInterval.C>
#endif	// CGAL_CFG_NO_AUTOMATIC_TEMPLATE_INCLUSION

#endif	// CGAL_MPFIINTERVAL_H
