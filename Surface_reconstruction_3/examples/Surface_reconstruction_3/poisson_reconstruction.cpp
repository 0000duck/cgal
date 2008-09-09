// poisson_reconstruction.cpp


// ----------------------------------------------------------------------------
// USAGE EXAMPLES
// ----------------------------------------------------------------------------

//----------------------------------------------------------
// Poisson Delaunay Reconstruction method
// Input file formats are .off and .xyz.
// Output file format is .off.
//----------------------------------------------------------
// poisson_reconstruction file_in file_out [options]


#include <CGAL/basic.h> // include basic.h before testing #defines

#ifdef CGAL_USE_TAUCS


// CGAL
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Timer.h>
#include <CGAL/Memory_sizer.h>
#include <CGAL/IO/Polyhedron_iostream.h>
#include <CGAL/Surface_mesh_default_triangulation_3.h>
#include <CGAL/make_surface_mesh.h>
#include <CGAL/Implicit_surface_3.h>
#include <CGAL/IO/Complex_2_in_triangulation_3_file_writer.h>

// This package
#include <CGAL/Poisson_implicit_function.h>
#include <CGAL/Point_with_normal_3.h>
#include <CGAL/IO/surface_reconstruction_output.h>
#include <CGAL/IO/surface_reconstruction_read_xyz.h>
#include <CGAL/surface_reconstruction_assertions.h>

// This test
#include "enriched_polyhedron.h"

// STL stuff
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <cassert>


// ----------------------------------------------------------------------------
// Private types
// ----------------------------------------------------------------------------

// kernel
typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel;
typedef Kernel::FT FT;
typedef Kernel::Point_3 Point;
typedef Kernel::Vector_3 Vector;
typedef CGAL::Point_with_normal_3<Kernel> Point_with_normal;
typedef Kernel::Sphere_3 Sphere;

typedef std::vector<Point_with_normal> PointList;

// Poisson's Delaunay triangulation 3 and implicit function
typedef CGAL::Implicit_fct_delaunay_triangulation_3<Kernel> Dt3;
typedef CGAL::Poisson_implicit_function<Kernel, Dt3> Poisson_implicit_function;

// Surface mesher
typedef CGAL::Surface_mesh_default_triangulation_3 STr;
typedef CGAL::Surface_mesh_complex_2_in_triangulation_3<STr> C2t3;
typedef CGAL::Implicit_surface_3<Kernel, Poisson_implicit_function&> Surface_3;


// ----------------------------------------------------------------------------
// main()
// ----------------------------------------------------------------------------

int main(int argc, char * argv[])
{
    std::cerr << "RECONSTRUCTION" << std::endl;
    std::cerr << "Poisson Delaunay Reconstruction method" << std::endl;

    //***************************************
    // decode parameters
    //***************************************

    if (argc<3)
    {
        std::cerr << "Usage: " << argv[0] << " file_in file_out [options]\n";
        std::cerr << "Input file formats are .off and .xyz.\n";
        std::cerr << "Output file format is .off.\n";
        std::cerr << "Options:\n";
        std::cerr << "  -sm_radius <float>     Radius upper bound (default=0.1 * point set radius)\n";
        std::cerr << "  -sm_distance <float>   Distance upper bound (default=0.002 * point set radius)\n";
        return EXIT_FAILURE;
    }

    // Default Surface Mesher options
    FT sm_angle = 20.0; // theorical guaranty if angle >= 30, but slower
    FT sm_radius = 0.1; // as suggested by LR
    FT sm_distance = 0.002; // AG: was 0.005 (upper bound of distance to surface/Poisson)
    FT sm_error_bound = 1e-3;

    for (int i=3; i+1<argc ; ++i)
    {
        if (std::string(argv[i])=="-sm_radius")
            sm_radius = atof(argv[++i]);
        else if (std::string(argv[i])=="-sm_distance")
            sm_distance = atof(argv[++i]);
        else
            std::cerr << "invalid option " << argv[i] << "\n";
    }

    CGAL::Timer task_timer; task_timer.start();

    //***************************************
    // Load mesh/point set
    //***************************************

    // File name is:
    std::string input_filename  = argv[1];

    // get extension
    std::string extension = input_filename.substr(input_filename.find_last_of('.'));

    PointList pwns;

    if (extension == ".off" || extension == ".OFF")
    {
      // Read the mesh file in a polyhedron
      std::ifstream stream(input_filename.c_str());
      typedef Enriched_polyhedron<Kernel,Enriched_items> Polyhedron;
      Polyhedron input_mesh;
      CGAL::scan_OFF(stream, input_mesh, true /* verbose */);
      if(!stream || !input_mesh.is_valid() || input_mesh.empty())
      {
        std::cerr << "Error: cannot read file " << input_filename << std::endl;
        return EXIT_FAILURE;
      }

      // Compute vertices' normals from connectivity
      input_mesh.compute_normals();

      // Convert vertices and normals to PointList
      Polyhedron::Vertex_iterator v;
      for(v = input_mesh.vertices_begin();
          v != input_mesh.vertices_end();
          v++)
      {
        const Point& p = v->point();
        const Vector& n = v->normal();
        pwns.push_back(Point_with_normal(p,n));
      }
    }
    else if (extension == ".xyz" || extension == ".XYZ")
    {
      // Read the point set file in pwns
      if(!CGAL::surface_reconstruction_read_xyz(input_filename.c_str(),
                                                std::back_inserter(pwns)))
      {
        std::cerr << "Error: cannot read file " << input_filename << std::endl;
        return EXIT_FAILURE;
      }
    }
    else
    {
      std::cerr << "Error: cannot read file " << input_filename << std::endl;
      return EXIT_FAILURE;
    }

    // Print status
    long memory = CGAL::Memory_sizer().virtual_size();
    int nb_vertices = pwns.size();
    std::cerr << "Read file " << input_filename << ": " << nb_vertices << " vertices, "
                                                        << task_timer.time() << " seconds, "
                                                        << (memory>>20) << " Mb allocated"
                                                        << std::endl;
    task_timer.reset();

    //***************************************
    // Check requirements
    //***************************************

    if (nb_vertices == 0)
    {
      std::cerr << "Error: empty file" << std::endl;
      return EXIT_FAILURE;
    }

    assert(pwns.begin() != pwns.end());
    bool points_have_normals = (pwns.begin()->normal().get_vector() != CGAL::NULL_VECTOR);
    bool normals_are_oriented = pwns.begin()->normal().is_oriented();
    if ( ! (points_have_normals && normals_are_oriented) )
    {
      std::cerr << "Input point set not supported: this reconstruction method requires oriented normals" << std::endl;
      return EXIT_FAILURE;
    }

    //***************************************
    // Compute implicit function
    //***************************************

    std::cerr << "Create triangulation...\n";

    // Create implicit function and triangulation.
    // Insert vertices and normals in triangulation.
    Dt3 dt;
    Poisson_implicit_function poisson_function(dt, pwns.begin(), pwns.end());

    // Print status
    /*long*/ memory = CGAL::Memory_sizer().virtual_size();
    std::cerr << "Create triangulation: " << task_timer.time() << " seconds, "
                                          << (memory>>20) << " Mb allocated"
                                          << std::endl;
    task_timer.reset();

    std::cerr << "Compute implicit function...\n";

    /// Compute the Poisson indicator function f()
    /// at each vertex of the triangulation
    if ( ! poisson_function.compute_implicit_function() )
    {
      std::cerr << "Error: cannot compute implicit function" << std::endl;
      return EXIT_FAILURE;
    }

    // Print status
    /*long*/ memory = CGAL::Memory_sizer().virtual_size();
    std::cerr << "Compute implicit function: " << task_timer.time() << " seconds, "
                                               << (memory>>20) << " Mb allocated"
                                               << std::endl;
    task_timer.reset();

    //***************************************
    // Surface mesh generation
    //***************************************

    std::cerr << "Surface meshing...\n";

    STr tr;           // 3D-Delaunay triangulation
    C2t3 c2t3 (tr);   // 2D-complex in 3D-Delaunay triangulation

    // Get inner point
    Point inner_point = poisson_function.get_inner_point();
    FT inner_point_value = poisson_function(inner_point);
    if(inner_point_value >= 0.0)
    {
      std::cerr << "Error: unable to seed (" << inner_point_value << " at inner_point)" << std::endl;
      return EXIT_FAILURE;
    }

    // Get implicit surface's radius
    Sphere bounding_sphere = poisson_function.bounding_sphere();
    FT size = sqrt(bounding_sphere.squared_radius());

    // defining the surface
    Point sm_sphere_center = inner_point; // bounding sphere centered at inner_point
    FT    sm_sphere_radius = 2 * size;
    sm_sphere_radius *= 1.1; // <= the Surface Mesher fails if the sphere does not contain the surface
    Surface_3 surface(poisson_function,
                      Sphere(sm_sphere_center,sm_sphere_radius*sm_sphere_radius),
                      sm_error_bound*size/sm_sphere_radius); // dichotomy stops when segment < sm_error_bound*size

    // defining meshing criteria
    CGAL::Surface_mesh_default_criteria_3<STr> criteria(sm_angle,  // lower bound of facets angles (degrees)
                                                        sm_radius*size,  // upper bound of Delaunay balls radii
                                                        sm_distance*size); // upper bound of distance to surface

    CGAL_TRACE_STREAM << "  make_surface_mesh(dichotomy error="<<sm_error_bound<<" * point set radius,\n"
                      << "                    sphere center=("<<sm_sphere_center << "),\n"
                      << "                    sphere radius="<<sm_sphere_radius/size<<" * p.s.r.,\n"
                      << "                    angle="<<sm_angle << " degrees,\n"
                      << "                    radius="<<sm_radius<<" * p.s.r.,\n"
                      << "                    distance="<<sm_distance<<" * p.s.r.,\n"
                      << "                    Non_manifold_tag)\n";

    // meshing surface
    CGAL::make_surface_mesh(c2t3, surface, criteria, CGAL::Non_manifold_tag());

    // Print status
    /*long*/ memory = CGAL::Memory_sizer().virtual_size();
    std::cerr << "Surface meshing: " << task_timer.time() << " seconds, "
                                     << tr.number_of_vertices() << " output vertices, "
                                     << (memory>>20) << " Mb allocated"
                                     << std::endl;
    task_timer.reset();

    // save the mesh
    std::ofstream out(argv[2]);
    CGAL::output_surface_facets_to_off(out, c2t3);

    // Print status
    std::cerr << "Write file " << argv[2] << std::endl << std::endl;

    return EXIT_SUCCESS;
}


#else // CGAL_USE_TAUCS


#include <iostream>
#include <cstdlib>

// ----------------------------------------------------------------------------
// Empty main() if TAUCS is not installed
// ----------------------------------------------------------------------------

int main()
{
  std::cerr << "Skip test as TAUCS is not installed" << std::endl;
  return EXIT_SUCCESS;
}

#endif // CGAL_USE_TAUCS

