#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Voronoi_covariance_3/halfspaces_intersection.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel         K;
typedef K::Plane_3 Plane;
typedef CGAL::Polyhedron_3<K> Polyhedron_3;

int main (void) {
    std::list<Plane> planes;
    planes.push_back(Plane(1, 0, 0, -1));
    planes.push_back(Plane(-1, 0, 0, -1));
    planes.push_back(Plane(0, 1, 0, -1));
    planes.push_back(Plane(0, -1, 0, -1));
    planes.push_back(Plane(0, 0, 1, -1));
    planes.push_back(Plane(0, 0, -1, -1));

    Polyhedron_3 P;

    CGAL::Convex_hull_3::halfspaces_intersection(planes.begin(),
                                                 planes.end(),
                                                 P,
                                                 K());

    return 0;
}

