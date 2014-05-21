#include <CGAL/Simple_cartesian.h>
#include <CGAL/convex_hull_2.h>
#include <CGAL/point_generators_2.h>
#include <CGAL/Barycentric_traits_2.h>
#include <CGAL/Wachspress_coordinates_2.h>
#include <CGAL/Barycentric_coordinates_2.h>

// Namespace alias.
namespace BC = CGAL::Barycentric_coordinates;

// Some convenient typedefs.
typedef CGAL::Simple_cartesian<double> Kernel;

typedef BC::Barycentric_traits_2<Kernel> Barycentric_traits;

typedef Barycentric_traits::FT      Scalar;
typedef Barycentric_traits::Point_2 Point;

typedef std::vector<Scalar> Scalar_vector;
typedef std::vector<Point>  Point_vector;

typedef CGAL::Creator_uniform_2<double,Point> Creator;

typedef Point_vector::iterator InputIterator;

typedef BC::Wachspress_coordinates_2<Barycentric_traits> Wachspress;
typedef BC::Barycentric_coordinates_2<InputIterator, Wachspress, Barycentric_traits> Wachspress_coordinates;

using std::cout; using std::endl; using std::string;

int main()
{
    // Choose how many random points we want to generate.
    const int number_of_points = 1000;

    // Create vectors to store generated points and vertices of a convex polygon.
    Point_vector points, vertices;

    // Generate a set of random points.
    CGAL::Random_points_in_square_2<Point,Creator> point_generator(1.0);
    CGAL::cpp11::copy_n(point_generator, number_of_points, std::back_inserter(points));

    // Find the convex hull of the generated set of points.
    // This convex hull gives us vertices of a convex polygon that contains all the generated points.
    CGAL::convex_hull_2(points.begin(), points.end(), std::back_inserter(vertices));

    const size_t number_of_vertices = vertices.size();

    // Instantiate the class Wachspress_coordinates_2 for the convex polygon defined above.
    Wachspress_coordinates wachspress_coordinates(vertices.begin(), vertices.end());

    // Print some information about the polygon and coordinate functions.
    wachspress_coordinates.print_information();
    
    // Compute Wachspress coordinates for all the randomly defined points.
    cout << endl << "Computed Wachspress coordinates are " << endl << endl;
    for(int i = 0; i < number_of_points; ++i) {
        // Compute coordinates.
        Scalar_vector coordinates;
        coordinates.reserve(number_of_vertices);
        wachspress_coordinates.compute(points[i], std::back_inserter(coordinates));

        // Output the computed coordinates.
        cout << "Point " << i + 1 << ": " << endl;
        for(int j = 0; j < int(number_of_vertices); ++j)  cout << "Coordinate " << j + 1 << " = " << coordinates[j] << "; " << endl;
        cout << endl;
    }

    return EXIT_SUCCESS;
}