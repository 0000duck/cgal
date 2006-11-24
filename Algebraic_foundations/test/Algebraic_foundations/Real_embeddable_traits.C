#include <CGAL/basic.h>
#include <CGAL/Real_embeddable_traits.h>
#include <CGAL/Testsuite/assert.h>


#define CGAL_IS_RET_NULL_FUNCTOR(NAME)                                  \
    {                                                                   \
        typedef RET::NAME NAME;                                         \
        BOOST_STATIC_ASSERT(                                            \
                (::boost::is_same<CGAL::Null_functor,NAME>::value));    \
    }      

int main(){
    typedef CGAL::Real_embeddable_traits<void> RET;
    
    typedef RET::Type Type;
    BOOST_STATIC_ASSERT((::boost::is_same<void,Type>::value)); 

    typedef RET::Is_real_embeddable Is_real_embeddable;
    BOOST_STATIC_ASSERT((::boost::is_same<CGAL::Tag_false,Is_real_embeddable>::value)); 
    
    CGAL_IS_RET_NULL_FUNCTOR(Abs);
    CGAL_IS_RET_NULL_FUNCTOR(Sign);
    CGAL_IS_RET_NULL_FUNCTOR(Is_finite);
    CGAL_IS_RET_NULL_FUNCTOR(Is_positive);
    CGAL_IS_RET_NULL_FUNCTOR(Is_negative);
    CGAL_IS_RET_NULL_FUNCTOR(Is_zero);
    CGAL_IS_RET_NULL_FUNCTOR(Compare);
    CGAL_IS_RET_NULL_FUNCTOR(To_double);
    CGAL_IS_RET_NULL_FUNCTOR(To_interval);
}
