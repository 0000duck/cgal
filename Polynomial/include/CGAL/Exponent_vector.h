// TODO: Add licence
//
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
//
// $URL$
// $Id$
// 
//
// Author(s)     : Sebastian Limbach
//                 Michael Hemmer
//
// ============================================================================

#ifndef CGAL_EXPONENT_VECTOR_H
#define CGAL_EXPONENT_VECTOR_H

#include <deque>
#include <iterator>
#include <algorithm>
#include <vector>
#include <CGAL/assertions.h>


CGAL_BEGIN_NAMESPACE

class Exponent_vector : 
    public std::vector<int>, 
    public boost::less_than_comparable1< Exponent_vector >
{
    typedef std::vector<int> Base;
public:
    Exponent_vector(): Base(){};
    Exponent_vector(Base::size_type i): Base(i){};
    Exponent_vector(Base::size_type i, Base::value_type x): Base(i,x){};
    Exponent_vector(const Exponent_vector& x): Base ( x ){};

    template <class InputIterator>
    Exponent_vector(InputIterator, InputIterator){
        typedef typename InputIterator::value_type value_type;
        BOOST_STATIC_ASSERT(( ::boost::is_same<value_type, int>::value));
    }
       
    bool operator<( const Exponent_vector& ev ) const {
        CGAL_precondition(this->size()==ev.size());
        Base::const_reverse_iterator rit1(this->rbegin());
        Base::const_reverse_iterator rit2(ev.rbegin());
        while(rit1!=this->rend()){
            if(*rit1 < *rit2) return true;
            if(*rit1 > *rit2) return false;
            rit1++; rit2++;
        }
        CGAL_postcondition(rit1 == this->rend());
        CGAL_postcondition(rit2 == ev.rend());
        return false;
    }
    
    void output_benchmark( std::ostream& os ) const {
        os << "( ";
        for( unsigned i = 0; i < size(); ++i ) {
            if( i != 0 )
                os << ", ";
            os << at(i); 
        }
        os << " )";
    }
};


inline bool is_valid(const Exponent_vector& ev) {
    Exponent_vector::const_iterator it;
    for(it = ev.begin(); it != ev.end();it++){
        if (CGAL::is_negative(*it)) return false;
    }
    return true; 
}

inline std::ostream& operator << (std::ostream& os, const Exponent_vector& ev) {
    Exponent_vector::const_iterator it;
    os << "(" ;
    for(it = ev.begin(); it != ev.end();it++){
        if (it == ev.begin()) {
            os << *it ;
        }
        else{
            os <<"," << *it ;
        }
    }
    os << ")" ;
    return os;
}


CGAL_END_NAMESPACE

#endif // CGAL_EXPONENT_VECTOR_H
