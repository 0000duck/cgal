#include <CGAL/Barycentric_coordinates_2/Barycentric_traits_2.h>
#include <CGAL/Barycentric_coordinates_2/Generalized_barycentric_coordinates_2.h>
#include <CGAL/Barycentric_coordinates_2/Mean_value_2.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

// Namespace alias.
namespace BC = CGAL::Barycentric_coordinates;

// Some convenient typedefs.
typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel;

typedef CGAL::Barycentric_coordinates::Barycentric_traits_2<Kernel> Barycentric_traits;

typedef Barycentric_traits::FT      Scalar;
typedef Barycentric_traits::Point_2 Point;

typedef std::vector<Scalar> Scalar_vector;
typedef std::vector<Point>  Point_vector;

typedef Point_vector::iterator InputIterator;
typedef std::back_insert_iterator<Scalar_vector> Vector_insert_iterator;
typedef std::pair<Vector_insert_iterator, bool> Output_type;

typedef BC::Mean_value_2<Barycentric_traits> Mean_value;
typedef BC::Generalized_barycentric_coordinates_2<InputIterator, Mean_value, Barycentric_traits> Mean_value_coordinates;

using std::cout; using std::endl; using std::string;

int main()
{    
    // Construct a star-shaped polygon.
    const int number_of_vertices = 10;
    Point_vector vertices;
    vertices.resize(number_of_vertices);
    vertices[0] = Point(0.0, 0.0); vertices[1] = Point(0.1, -0.8); vertices[2] = Point(0.3, 0.0); vertices[4] = Point(0.6, -0.5); vertices[5]  = Point(0.6 , 0.1);
    vertices[6] = Point(1.1, 0.6); vertices[7] = Point(0.3,  0.2); vertices[8] = Point(0.1, 0.8); vertices[9] = Point(0.1,  0.2); vertices[10] = Point(-0.7, 0.0);

    // Create an std::vector to store coordinates.
    Scalar_vector coordinates;

    // Instantiate the class with mean value coordinates for the polygon defined above.
    Mean_value_coordinates mean_value_coordinates(vertices.begin(), vertices.end());

    // Print some information about the polygon and coordinate functions.
    mean_value_coordinates.print_information();

    // Instantiate some interior points in the polygon.
    const int number_of_interior_points = 8;
    const Point interior_points[] = { Point(0.12, -0.45), Point(0.55, -0.3), Point(0.9 , 0.45),
                                      Point(0.15,  0.35), Point(-0.4, 0.04), Point(0.11, 0.11),
                                      Point(0.28,  0.12), // the only point in the kernel of the star shaped polygon
                                      Point(0.55,  0.11)
                                    };

    // Compute mean value coordinates for all the defined interior points.

    // We speed up the computation using the O(n) algorithm called with the parameter CGAL::Barycentric_coordinates::FAST.
    const CGAL::Barycentric_coordinates::Type_of_algorithm type_of_algorithm = CGAL::Barycentric_coordinates::FAST;

    // Use the parameter query_point_location = CGAL::Barycentric_coordinates::ON_BOUNDED_SIDE.
    const CGAL::Barycentric_coordinates::Query_point_location query_point_location = CGAL::Barycentric_coordinates::ON_BOUNDED_SIDE;

    for(int i = 0; i < number_of_interior_points; ++i) {
        const Output_type result = mean_value_coordinates.compute(interior_points[i], std::back_inserter(coordinates), query_point_location, type_of_algorithm);

        // Output the coordinates for each point.
        const string status = (result.second == true ? "SUCCESS." : "FAILURE.");
        cout << endl << "For the point " << i + 1 << " status of the computation: " << status << endl;

        for(int j = 0; j < number_of_vertices; ++j)
            cout << "Coordinate " << j + 1 << " = " << coordinates[i * number_of_vertices + j] << endl;
    }

    // If we need only the unnormalized weights for the last point, we can compute them as follows.

    // Instantiate an std::vector to store weights.
    Scalar_vector weights;

    // Compute mean value weights.
    const int last_point_index = number_of_interior_points - 1;
    const Output_type result = mean_value_coordinates.compute_weights(interior_points[last_point_index], std::back_inserter(weights));

    // Compute their sum.
    Scalar mv_denominator = Scalar(0);
    for(int j = 0; j < number_of_vertices; ++j) mv_denominator += weights[j];

    // Invert this sum.
    const Scalar mv_inverted_denominator = Scalar(1) / mv_denominator;

    // Output mean value weights.
    const string status = (result.second == true ? "SUCCESS." : "FAILURE.");
    cout << endl << "Status of the weights' computation for the point " << last_point_index + 1 << ": " << status << endl;

    for(int j = 0; j < number_of_vertices; ++j)
        cout << "Weight " << j + 1 << " = " << weights[j] << endl;

    // Now, if we normalize the weight functions, we recover values of the mean value coordinates for the last point computed earlier.
    cout << endl << "After normalization, for the point " << last_point_index + 1 << " mean value coordinates are " << endl;
    for(int j = 0; j < number_of_vertices; ++j)
        cout << "Coordinate " << j + 1 << " = " << weights[j] * mv_inverted_denominator << endl;
    cout << endl;

    return EXIT_SUCCESS;
}