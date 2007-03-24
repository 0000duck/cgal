#include <CGAL/Cartesian.h>
#ifndef CGAL_USE_LAPACK
int main()
{
  std::cerr << "Skip since LAPACK is not installed" << std::endl;
  std::cerr << std::endl;
  return 0;
}
#else

#include <fstream>
#include <vector>
#include <CGAL/Monge_via_jet_fitting.h>

typedef double                   DFT;
typedef CGAL::Cartesian<DFT>     Data_Kernel;
typedef Data_Kernel::Point_3     DPoint;
typedef CGAL::Monge_via_jet_fitting<Data_Kernel> My_Monge_via_jet_fitting;
typedef My_Monge_via_jet_fitting::Monge_form     My_Monge_form;

int main()
{
  //open the input file
  std::ifstream inFile( "data/in_points_d4.txt", std::ios::in);
  if ( !inFile ) 
    {
      std::cerr << "cannot open file for input\n";
      exit(-1);
    }
  //initalize the in_points container
  double x, y, z;
  std::vector<DPoint> in_points;
  while (inFile >> x) { 
    inFile >> y >> z;
    DPoint p(x,y,z);
    in_points.push_back(p);
  }
  inFile.close();

  // fct parameters
  int d_fitting = 4;
  int d_monge = 4;
  My_Monge_form monge_form;
  My_Monge_via_jet_fitting monge_fit; 
  monge_form = monge_fit(in_points.begin(), in_points.end(), d_fitting, d_monge);

  monge_form.comply_wrt_given_normal( -monge_form.normal_direction() );
  //OUTPUT on std::cout
  std::cout << monge_form
	    << "check access fct"  << std::endl;
  if ( monge_form.coefficients().size() >= 2) 
    std::cout << "d1 : " << monge_form.maximal_principal_direction() << std::endl 
	      << "d2 : " << monge_form.minimal_principal_direction() << std::endl
	      << "k1 : " << monge_form.principal_curvatures(0) << std::endl 
	      << "k2 : " << monge_form.principal_curvatures(1) << std::endl;	      
  if ( monge_form.coefficients().size() >= 6) 
     std::cout << "b0 : " << monge_form.third_order_coefficients(0) << std::endl 
	       << "b1 : " << monge_form.third_order_coefficients(1) << std::endl 
 	       << "b2 : " << monge_form.third_order_coefficients(2) << std::endl 
 	       << "b3 : " << monge_form.third_order_coefficients(3) << std::endl;
  if ( monge_form.coefficients().size() >= 11) 
     std::cout << "c0 : " << monge_form.fourth_order_coefficients(0) << std::endl 
	       << "c1 : " << monge_form.fourth_order_coefficients(1) << std::endl 
 	       << "c2 : " << monge_form.fourth_order_coefficients(2) << std::endl 
 	       << "c3 : " << monge_form.fourth_order_coefficients(3) << std::endl 
 	       << "c4 : " << monge_form.fourth_order_coefficients(4) << std::endl;
 
  monge_form.dump_4ogl( std::cout, 1 );
  double precision = 0.01;
  assert(monge_form.coefficients()[0] >= -0.2 - precision);
  assert(monge_form.coefficients()[0] <= -0.2 + precision);
  assert(monge_form.coefficients()[1] >= -0.4 - precision);
  assert(monge_form.coefficients()[1] <= -0.4 + precision);
  std::cout << "success\n";
 return 0;
}
 
#endif // CGAL_USE_LAPACK
 
