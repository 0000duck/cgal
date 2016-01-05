#include <iostream>
#include <fstream>
#include <algorithm>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Point_with_normal_3.h>
#include <CGAL/Shape_detection_3.h>
#include <CGAL/structure_point_set.h>
#include <CGAL/Delaunay_triangulation_3.h>
#include <CGAL/Triangulation_vertex_base_with_info_3.h>
#include <CGAL/Advancing_front_surface_reconstruction.h>
#include <CGAL/IO/read_xyz_points.h>

#include <boost/lexical_cast.hpp>

typedef CGAL::Exact_predicates_inexact_constructions_kernel  Kernel;
typedef Kernel::Point_3  Point;
typedef std::pair<Kernel::Point_3, Kernel::Vector_3>         Point_with_normal;
typedef std::vector<Point_with_normal>                       Pwn_vector;
typedef CGAL::First_of_pair_property_map<Point_with_normal>  Point_map;
typedef CGAL::Second_of_pair_property_map<Point_with_normal> Normal_map;

// Efficient RANSAC types
typedef CGAL::Shape_detection_3::Efficient_RANSAC_traits
  <Kernel, Pwn_vector, Point_map, Normal_map>                Traits;
typedef CGAL::Shape_detection_3::Efficient_RANSAC<Traits>    Efficient_ransac;
typedef CGAL::Shape_detection_3::Plane<Traits>               Plane;

typedef CGAL::internal::Point_set_structuring<Traits>        Structuring;

typedef CGAL::Advancing_front_surface_reconstruction_vertex_base_3<Kernel> LVb;
typedef CGAL::Advancing_front_surface_reconstruction_cell_base_3<Kernel> LCb;

typedef CGAL::Triangulation_data_structure_3<LVb,LCb> Tds;
typedef CGAL::Delaunay_triangulation_3<Kernel,Tds> Triangulation_3;
typedef Triangulation_3::Vertex_handle Vertex_handle;

typedef CGAL::cpp11::array<std::size_t,3> Facet;


struct On_the_fly_pair{
  const Pwn_vector& points;
  typedef std::pair<Point, std::size_t> result_type;

  On_the_fly_pair(const Pwn_vector& points) : points(points) {}
  
  result_type
  operator()(std::size_t i) const
  {
    return result_type(points[i].first,i);
  }
};

template <typename Structuring>
struct Priority_with_structure_coherence {

  Structuring& structuring;
  double bound;
  
  Priority_with_structure_coherence(Structuring& structuring,
                                    double bound)
    : structuring (structuring), bound (bound)
  {}

  template <typename AdvancingFront, typename Cell_handle>
  double operator() (AdvancingFront& adv, Cell_handle& c,
                     const int& index) const
  {
    // If perimeter > bound, return infinity so that facet is not used
    if (bound != 0)
      {
        double d  = 0;
        d = sqrt(squared_distance(c->vertex((index+1)%4)->point(),
                                  c->vertex((index+2)%4)->point()));
        if(d>bound) return adv.infinity();
        d += sqrt(squared_distance(c->vertex((index+2)%4)->point(),
                                   c->vertex((index+3)%4)->point()));
        if(d>bound) return adv.infinity();
        d += sqrt(squared_distance(c->vertex((index+1)%4)->point(),
                                   c->vertex((index+3)%4)->point()));
        if(d>bound) return adv.infinity();
      }

    Facet f = {{ c->vertex ((index + 1) % 4)->info (),
                 c->vertex ((index + 2) % 4)->info (),
                 c->vertex ((index + 3) % 4)->info () }};

    double weight = 100. * (5 - structuring.facet_coherence (f));

    return weight * adv.smallest_radius_delaunay_sphere (c, index);
  }

};

typedef CGAL::Advancing_front_surface_reconstruction<Triangulation_3,
                                                     Priority_with_structure_coherence<Structuring> > Reconstruction;



int main (int argc, char* argv[])
{
  // Points with normals.
  Pwn_vector points;

  const char* fname = (argc>1) ? argv[1] : "data/cube.pwn";
  // Loading point set from a file. 
  std::ifstream stream(fname);

  if (!stream || 
    !CGAL::read_xyz_points_and_normals(stream,
      std::back_inserter(points),
      Point_map(),
      Normal_map()))
  {
      std::cerr << "Error: cannot read file" << std::endl;
      return EXIT_FAILURE;
  }

  std::cerr << "Shape detection... ";
  // Shape detection
  Efficient_ransac ransac;
  ransac.set_input(points);
  ransac.add_shape_factory<Plane>();

  Efficient_ransac::Parameters op;
  op.probability = 0.05;
  op.min_points = 100;
  op.epsilon = (argc>2 ? boost::lexical_cast<double>(argv[2]) : 0.002);
  op.cluster_epsilon = (argc>3 ? boost::lexical_cast<double>(argv[3]) : 0.02);
  op.normal_threshold = 0.7;

  ransac.detect(op);
  std::cerr << "done\nPoint set structuring... ";
  Pwn_vector structured_pts;
  
  Structuring pss (points.begin (), points.end (), ransac);

  pss.run (op.cluster_epsilon);

  pss.get_output (std::back_inserter (structured_pts));
  std::cerr << "done\nAdvancing front... ";
  std::vector<std::size_t> point_indices(boost::counting_iterator<std::size_t>(0),
                                         boost::counting_iterator<std::size_t>(structured_pts.size()));

  Triangulation_3 dt (boost::make_transform_iterator(point_indices.begin(), On_the_fly_pair(structured_pts)),
                      boost::make_transform_iterator(point_indices.end(), On_the_fly_pair(structured_pts)));


  Priority_with_structure_coherence<Structuring> priority (pss, 1000. * op.cluster_epsilon);
  Reconstruction R(dt, priority);

  R.run (5., 0.52);
  std::cerr << "done\nWriting result... ";
  std::vector<Facet> output;
  CGAL::write_triple_indices (std::back_inserter (output), R);

  std::ofstream f ("out.off");
  f << "OFF\n" << structured_pts.size () << " " << output.size() << " 0\n";
  for (std::size_t i = 0; i < structured_pts.size (); ++ i)
    f << structured_pts[i].first << std::endl;
  for (std::size_t i = 0; i < output.size (); ++ i)
    f << "3 "
      << output[i][0] << " "
      << output[i][1] << " "
      << output[i][2] << std::endl;
  std::cerr << "all done\n" << std::endl;

  f.close();
  
  std::vector<Facet> out[3];
  for (std::size_t i = 0; i < output.size (); ++ i)
    {
      unsigned int coherence = pss.facet_coherence (output[i]);
      if (coherence > 2) coherence = 2;
      out[coherence].push_back (output[i]);
    }
  
  for (std::size_t n = 0; n < 3; ++ n)
    {
      std::stringstream ss;
      ss << "out" << n << ".off";
      std::ofstream f (ss.str().c_str());
      f << "OFF\n" << structured_pts.size () << " " << out[n].size() << " 0\n";
      for (std::size_t i = 0; i < structured_pts.size (); ++ i)
        f << structured_pts[i].first << std::endl;
      for (std::size_t i = 0; i < out[n].size (); ++ i)
        f << "3 "
          << out[n][i][0] << " "
          << out[n][i][1] << " "
          << out[n][i][2] << std::endl;
      std::cerr << "all done\n" << std::endl;

      f.close();
    }
  
  return 0;
}
