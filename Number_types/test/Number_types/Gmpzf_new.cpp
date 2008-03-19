#include <iostream>
#include <cassert>
#include <CGAL/Gmpzf.h>
#include <CGAL/_test_algebraic_structure.h>
#include <CGAL/_test_real_embeddable.h>

int main() {
    {
        typedef CGAL::Gmpzf NT;

        typedef CGAL::Euclidean_ring_tag Tag;
        typedef CGAL::Tag_true Is_exact;
        
        CGAL::test_algebraic_structure<NT,Tag, Is_exact>();
        CGAL::test_real_embeddable<NT>();
        
        assert(CGAL::sqrt(NT(4)) == NT(2));
        
    
    }{
        typedef CGAL::Quotient<CGAL::Gmpzf> NT;

        typedef CGAL::Field_tag Tag;
        typedef CGAL::Tag_true Is_exact;
        
        CGAL::test_algebraic_structure<NT,Tag, Is_exact>();
        CGAL::test_real_embeddable<NT>();

        assert(CGAL::sqrt(NT(4)) == NT(2));
       
    }
 return 0;
}
