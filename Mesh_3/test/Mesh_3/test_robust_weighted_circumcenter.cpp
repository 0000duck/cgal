// Copyright (c) 2009 INRIA Sophia-Antipolis (France).
// All rights reserved.
//
// This file is part of CGAL (www.cgal.org); you may redistribute it under
// the terms of the Q Public License version 1.0.
// See the file LICENSE.QPL distributed with CGAL.
//
// Licensees holding a valid commercial license may use this file in
// accordance with the commercial license agreement provided with the software.
//
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
//
// $URL$
// $Id$
//
//
// Author(s)     : Stephane Tayeb
//
//******************************************************************************
// File Description :
//
//******************************************************************************

#include "test_utilities.h"
#include <CGAL/Mesh_3/Creator_weighted_point_3.h>
#include <CGAL/Polyhedral_mesh_domain_3.h>
#include <CGAL/Polyhedron_3.h>
#include <CGAL/Timer.h>

template <typename K>
struct Tester
{
  typedef CGAL::Polyhedron_3<K> Polyhedron;
  typedef CGAL::Polyhedral_mesh_domain_3<Polyhedron, K> Mesh_traits;

  typedef typename CGAL::Mesh_triangulation_3<Mesh_traits>::type Tr;
  
  typedef typename Tr::Geom_traits Gt;
  typedef typename Gt::FT FT;
  typedef typename Gt::Point_3 Point;
  typedef CGAL::Mesh_3::Creator_weighted_point_3<FT, Point> Point_creator;

  typedef CGAL::Regular_triangulation_3<Gt> Triangulation;

  void operator()() const
  {
    //-------------------------------------------------------
    // Data generation : get 4 nearly coplanar points
    //-------------------------------------------------------
    Point_creator creator;
    FT little(1e-35);
    FT tiny(1e-65);
    
    Point p1 = creator(little,1,tiny);
    Point p2 = creator(1,little,0);
    Point p3 = creator(-1*little,1,0);
    Point p4 = creator(1,-1*little,0);
    Point p5 = creator(0,0,1);
    Point p6 = creator(0,1,0);

    std::cerr << "Using points: p1[" << p1 << "]\tp2[" << p2
              << "]\tp3[" << p3 << "]\tp4[" << p4 << "]\tp5[" << p5
              << "]\tp6[" << p6 << "]\n";

    //-------------------------------------------------------
    // Test correctness
    //-------------------------------------------------------
    typename Gt::Construct_weighted_circumcenter_3 circumcenter =
        Gt().construct_weighted_circumcenter_3_object();

    Point center = circumcenter(p1,p2);
    std::cerr << "\tcircumcenter(p1,p2)=[" << center << "]\n";

    center = circumcenter(p1,p3,p6);
    std::cerr << "\tcircumcenter(p1,p3,p6)=[" << center << "]\n";

    center = circumcenter(p1,p2,p5);
    std::cerr << "\tcircumcenter(p1,p3,p5)=[" << center << "]\n";

    center = circumcenter(p1,p2,p3,p4);
    std::cerr << "\tcircumcenter(p1,p2,p3,p4)=[" << center << "]\n";

    center = circumcenter(p1,p2,p3,p5);
    std::cerr << "\tcircumcenter(p1,p2,p3,p5)=[" << center << "]\n";


    //-------------------------------------------------------
    // Test speed
    //-------------------------------------------------------
    std::cerr << "Test speed: compute loops of: 999*c(p1,p2,p3,p5) "
              << "and 1*c(p1,p2,p3,p4)\n";

    CGAL::Timer timer;
    timer.start();
    int nb_loop = 0;
    while ( timer.time() < 0.5 )
    {
      // Compute 1000 fast queries
      for ( int i = 0 ; i < 999 ; ++i)
        circumcenter(p1,p2,p3,p5);

      // Compute 1 exact query
      circumcenter(p1,p2,p3,p4);
      ++nb_loop;
    }
    timer.stop();
    std::cerr << "\t" << nb_loop*1000/timer.time()
              << " circumcenter computation / second\n";

    
    std::cerr << "Test speed: compute loops of: 999*c(p1,p2,p3,p4) "
              << "and 1*c(p1,p2,p3,p5)\n";
    
    timer.reset();
    timer.start();
    nb_loop = 0;
    while ( timer.time() < 0.5 )
    {
      // Compute 1000 fast queries
      for ( int i = 0 ; i < 999 ; ++i)
        circumcenter(p1,p2,p3,p4);
      
      // Compute 1 exact query
      circumcenter(p1,p2,p3,p5);
      ++nb_loop;
    }
    timer.stop();
    std::cerr << "\t" << nb_loop*1000/timer.time()
              << " circumcenter computation / second\n";
    
  }
};


int main()
{
  std::cerr << "TESTING WITH Exact_predicates_inexact_constructions_kernel...\n";
  Tester<K_e_i> test_epic;
  test_epic();

  std::cerr << "\nTESTING WITH Exact_predicates_exact_constructions_kernel...\n";
  Tester<K_e_e> test_epec;
  test_epec();
  
//  std::cerr << "\nTESTING WITH Filtered_kernel<Simple_cartesian<float> > kernel...\n";
//  Tester<Filtered_kernel<CGAL::Simple_cartesian<float> > > test_scf;
//  test_scf();

  return EXIT_SUCCESS;
}


