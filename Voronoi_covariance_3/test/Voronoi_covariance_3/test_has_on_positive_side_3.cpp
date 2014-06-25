#include <cassert>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

#include <CGAL/Voronoi_covariance_3/Convex_hull_traits_dual_3.h>
#include <CGAL/Convex_hull_traits_3.h>

#include "include/to_dual.h"

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Plane_3 Plane;
typedef CGAL::Convex_hull_3::Plane_dual<K> Plane_dual;

typedef CGAL::Convex_hull_3::Convex_hull_traits_dual_3<K> Hull_traits_dual;
typedef Hull_traits_dual::Has_on_positive_side_3 Has_on_positive_side_3_dual;

typedef CGAL::Convex_hull_traits_3<K> Hull_traits;
typedef Hull_traits::Has_on_positive_side_3 Has_on_positive_side_3;

int main () {
    Hull_traits_dual traits_dual;
    Hull_traits traits;

    Has_on_positive_side_3 has_on = traits.has_on_positive_side_3_object();
    Has_on_positive_side_3_dual has_on_dual = traits_dual.has_on_positive_side_3_object();

    Plane a(1, 1, 0, -1), b(1, 2, 1, -1), c(1, 3, 0, -1); // dual plane : x = 1
    Plane_dual p(a,b,c);
    Plane q(0, 0, 0, -1), qq(2, 0, 0, -1), qqq(1, 1, 0, -1);

    // True
    /* std::cout << has_on_dual(p, q) << std::endl; */
    /* std::cout << has_on(to_dual_plane<K>(p), to_dual<K>(q)) << std::endl; */
    assert(has_on_dual(p, q) == true);
    assert(has_on_dual(p, q) == has_on(to_dual_plane<K>(p), to_dual<K>(q)));

    // False
    /* std::cout << has_on_dual(p, qq) << std::endl; */
    /* std::cout << has_on(to_dual_plane<K>(p), to_dual<K>(qq)) << std::endl; */
    assert(has_on_dual(p, q) == false);
    assert(has_on_dual(p, q) == has_on(to_dual_plane<K>(p), to_dual<K>(qq)));

    // False : on the plane
    /* std::cout << has_on_dual(p, qqq) << std::endl; */
    /* std::cout << has_on(to_dual_plane<K>(p), to_dual<K>(qqq)) << std::endl; */
    assert(has_on_dual(p, q) == false);
    assert(has_on_dual(p, q) == has_on(to_dual_plane<K>(p), to_dual<K>(qqq)));

    return 0;
}

