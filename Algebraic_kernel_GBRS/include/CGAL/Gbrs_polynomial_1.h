#ifndef CGAL_GBRS_POLYNOMIAL_1_H
#define CGAL_GBRS_POLYNOMIAL_1_H

#include <iostream>
#include <CGAL/assertions.h>
#include <CGAL/basic.h>
#include <CGAL/Gmpz.h>
#include <CGAL/Gmpq.h>
#include <CGAL/MpfiInterval.h>
#include <gmp.h>

CGAL_BEGIN_NAMESPACE

// TODO: change the implementation so the type of coefficients are a parameter
// to the class (it won't be so effective, though)

class Rational_polynomial_1 {
	private:
		int degree;	// degree
		int nm;	// number of monomials in the polynomial
		mpq_t* coef;	// the coefficients
		int calc_index (int) const;	// index of the element
	public:
		// construction and destruction
		Rational_polynomial_1 ();
		Rational_polynomial_1 (const Rational_polynomial_1 &);
		Rational_polynomial_1 (unsigned int);
		Rational_polynomial_1 (int);
		~Rational_polynomial_1 ();
		// init functions
		void set_degree (int);
		void set_coef (int, const mpq_t &);
		void set_coef (int, const mpz_t &);
		void set_coef (int, const CGAL::Gmpq &);
		void set_coef (int, const CGAL::Gmpz &);
		void set_coef (int, int);
		void set_coef (int, unsigned int);
		void set_coef (int, long int);
		void set_coef (int, unsigned long int);
		void set_coef_rat_li (int, long int, long int);
		void scale (const int);
		void scale (const CGAL::Gmpz &);
		void scale (const CGAL::Gmpq &);
		// access to the data
		int get_degree () const;
		int get_number_of_monomials () const;
		mpq_t* get_coefs () const;
		void get_coef (int, mpq_t *) const;
		CGAL::MpfiInterval eval (const CGAL::MpfiInterval &) const;
		CGAL::Gmpq eval (const CGAL::Gmpq &) const;
		Rational_polynomial_1 derive () const;
		// assignment
		Rational_polynomial_1 operator= (const Rational_polynomial_1 &);
		// i/o functions
		std::ostream& show (std::ostream &) const;
		// overcharging
		Rational_polynomial_1 operator- () const;
		Rational_polynomial_1 operator+ (const Rational_polynomial_1 &) const;
		void operator+= (const Rational_polynomial_1 &);
		Rational_polynomial_1 operator- (const Rational_polynomial_1 &) const;
		void operator-= (const Rational_polynomial_1 &);
		void shift (int);
		Rational_polynomial_1 operator* (const Rational_polynomial_1 &) const;
		void operator*= (const Rational_polynomial_1 &);
		bool operator== (const Rational_polynomial_1 &) const;
};

std::ostream& operator<< (std::ostream &, const Rational_polynomial_1 &);

CGAL_END_NAMESPACE

#endif	// CGAL_GRBS_POLYNOMIAL_1_H
