#include <fstream>
#include <iostream>

#include <CGAL/Scale_space_surface_reconstruction_3.h>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

#include <CGAL/IO/read_off_points.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel     Kernel;

typedef CGAL::Scale_space_surface_reconstruction_3< Kernel >    Reconstruction;

typedef Reconstruction::Point                                   Point;
typedef std::vector< Point >                                    Pointset;

typedef Reconstruction::Const_triple_iterator                   Triple_iterator;

int main(void) {
    // Read the data.
	Pointset points;
	//std::ifstream in("kitten.off");
    std::ifstream in("C:\\Recherche\\Data\\ball_30.off");
    std::cout << "Reading " << std::flush;
    if( !in || !CGAL::read_off_points( in, std::back_inserter( points ) ) ) {
        std::cerr << "Error: cannot read file" << std::endl;
        return EXIT_FAILURE;
    }
	std::cout << "done: " << points.size() << " points." << std::endl;

	// Construct the mesh in a scale space.
	//Reconstruction reconstruct( 30, 200 );
	//reconstruct.reconstruct_surface( points.begin(), points.end(), 4 );
    Reconstruction reconstruct( 5, 200 );
	reconstruct.reconstruct_surface( points.begin(), points.end(), 1 );
    std::cout << "Reconstruction done:" << std::endl;

    // Write the reconstruction.
    std::cout << "Neighborhood radius^2 = " << reconstruct.neighborhood_squared_radius() << std::endl;
    for( std::size_t shell = 0; shell < reconstruct.number_of_shells(); ++shell ) {
        std::cout << "Shell " << shell << std::endl;
        for( Triple_iterator it = reconstruct.shell_begin( shell ); it != reconstruct.shell_end( shell ); ++it )
            std::cout << *it << std::endl;
    }

	std::cout << "Done." << std::endl;
    return EXIT_SUCCESS;
}