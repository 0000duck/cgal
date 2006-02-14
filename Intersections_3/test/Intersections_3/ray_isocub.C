/*
 * 2D Iso_rectangle Ray intersection.
 */

#include "numrep1.h"
#include <CGAL/Object.h>
#include <CGAL/Iso_cuboid_3.h> 
#include <CGAL/intersection_3_1.h>
#include "numrep2.h"
#include <iostream>

typedef CGAL::Point_3<TestR> point_t;
typedef CGAL::Ray_3<TestR> ray_t;
typedef CGAL::Segment_3<TestR> segment_t;
typedef CGAL::Iso_cuboid_3< TestR > rect_t;

void print(const point_t &pt)
{
    double xd = CGAL::to_double(pt.x());
    double yd = CGAL::to_double(pt.y());
    double zd = CGAL::to_double(pt.z());
    // force 0 to be positive zero.
    if (xd == 0.0)
	xd = 0.0;
    if (yd == 0.0)
	yd = 0.0;
    if (zd == 0.0)
	zd = 0.0;
    std::cout.setf(std::ios::showpos, std::ios::showpos);
    std::cout << xd <<' '<< yd <<' '<< zd;
    std::cout.unsetf(std::ios::showpos);
}


void one_pair(ray_t const & ray, rect_t const & rect)
{
    segment_t seg;
    point_t point;

    CGAL::Object result = CGAL::intersection(ray, rect);
    if (CGAL::assign(point, result)) {
	std::cout << "Point intersection.\n";
	print(point);
	std::cout << '\n';
    }
    if (CGAL::assign(seg, result)) {
	std::cout << "Segment intersection.\n";
	print(seg.source());
	std::cout << "  ";
	print(seg.target());
	std::cout << '\n';
    }
    if (!CGAL::assign(point, result) && !CGAL::assign(seg, result)) {
	std::cout << "No intersection.\n";
    }
}

int main()
{
    randomint ri;
    int x1, x2, y1, y2, z1, z2, w1, w2;
    std::cin >> x1 >> y1 >> z1 >> x2 >> y2 >> z2;
    if (!std::cin)
	return 1;
    point_t p1, p2;
    w1 = ri.next();
    w2 = ri.next();
    p1 = point_t(to_nt(w1*x1), to_nt(w1*y1), to_nt(w1*z1), to_nt(w1));
    p2 = point_t(to_nt(w2*x2), to_nt(w2*y2), to_nt(w2*z2), to_nt(w2));
    ray_t ray(p1, p2);
    std::cin >> x1 >> y1 >> z1 >> x2 >> y2 >> z2;
    if (!std::cin)
	return 1;
    w1 = ri.next();
    w2 = ri.next();
    p1 = point_t(to_nt(w1*x1), to_nt(w1*y1), to_nt(w1*z1), to_nt(w1));
    p2 = point_t(to_nt(w2*x2), to_nt(w2*y2), to_nt(w2*z2), to_nt(w2));
    rect_t rect(p1, p2);
    one_pair(ray, rect);
    return 0;
}
