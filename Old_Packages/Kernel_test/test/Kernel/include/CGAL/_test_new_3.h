// ============================================================================
//
// Copyright (c) 1999 The CGAL Consortium
//
// This software and related documentation is part of an INTERNAL release
// of the Computational Geometry Algorithms Library (CGAL). It is not
// intended for general use.
//
// ----------------------------------------------------------------------------
// 
// release       :
// release_date  :
// 
// file          : test/Kernel/include/CGAL/_test_new_3.h
// revision      : $Revision$
// revision_date : $Date$
// author(s)     : Michael Seel
//                 Stefan Schirra
//
// coordinator   : MPI, Saarbruecken  (<Stefan.Schirra@mpi-sb.mpg.de>)
// ============================================================================
 

#ifndef CGAL_TEST_NEW_3_H
#define CGAL_TEST_NEW_3_H

#include <CGAL/Point_3.h>
#include <CGAL/Vector_3.h>
#include <CGAL/Direction_3.h>
#include <CGAL/Segment_3.h>
#include <CGAL/Ray_3.h>
#include <CGAL/Line_3.h>
#include <CGAL/Plane_3.h>
#include <CGAL/Sphere_3.h>
#include <CGAL/Triangle_3.h>
#include <CGAL/Tetrahedron_3.h>
#include <CGAL/Iso_cuboid_3.h>
#include <CGAL/Aff_transformation_3.h>
#include <CGAL/intersections.h>
#include <CGAL/predicates_on_points_3.h>
#include <CGAL/distance_predicates_3.h>
#include <CGAL/squared_distance_3.h>

void use(bool) {}
void use(CGAL::Sign) {}
void use(CGAL::Bounded_side) {}
void use(CGAL::Oriented_side) {}
void use(CGAL::Comparison_result) {}


template <class R>
bool
test_new_3(const R& rep)
{
  std::cout << "Testing 3 dimensional functionality" << std::endl;

  using namespace CGAL;

  typedef typename R::RT                          RT;
  typedef typename R::FT                          FT;

  typedef typename R::Point_3                     Point_3;
  typedef typename R::Vector_3                    Vector_3;
  typedef typename R::Direction_3                 Direction_3;
  typedef typename R::Segment_3                   Segment_3;
  typedef typename R::Line_3                      Line_3;
  typedef typename R::Ray_3                       Ray_3;
  typedef typename R::Plane_3                     Plane_3;
  typedef typename R::Sphere_3                    Sphere_3;
  typedef typename R::Triangle_3                  Triangle_3;
  typedef typename R::Tetrahedron_3               Tetrahedron_3;
  typedef typename R::Iso_cuboid_3                Iso_cuboid_3;
  typedef typename R::Aff_transformation_3        Aff_transformation_3;
  typedef typename R::Object_3                    Object3;

  typename R::Construct_point_3 construct_point
        = rep.construct_point_3_object();
  Point_3 p1 = construct_point();
  Point_3 p2 = construct_point(ORIGIN);
  Point_3 p3 = construct_point(1,1,1);
  Point_3 p4 = construct_point(1,1,2,2);
  Point_3 p5 = construct_point(1,2,3,4);
  Point_3 p6 = construct_point(4,2,1,2);

  typename R::Construct_vector_3 construct_vector
        = rep.construct_vector_3_object();
  Vector_3 v1 = construct_vector();
  Vector_3 v2 = construct_vector(NULL_VECTOR);
  Vector_3 v3 = construct_vector(1,1,1);
  Vector_3 v4 = construct_vector(1,1,2,2);
  Vector_3 v5 = construct_vector(p5, p6);

  typename R::Construct_direction_3 construct_direction
        = rep.construct_direction_3_object();
  Direction_3 d1 = construct_direction();
  Direction_3 d2 = construct_direction(v3);
  Direction_3 d3 = construct_direction(1,1,5);
  Direction_3 d4 = construct_direction(1,5,5);

  typename R::Construct_segment_3 construct_segment
        = rep.construct_segment_3_object();
  Segment_3 s1 = construct_segment();
  Segment_3 s2 = construct_segment(p2,p3);

  typename R::Construct_ray_3 construct_ray =
        rep.construct_ray_3_object();
  Ray_3 r1 = construct_ray();
  Ray_3 r2 = construct_ray(p2,p4);
  Ray_3 r3 = construct_ray(p2,d3);

  typename R::Construct_line_3 construct_line
        = rep.construct_line_3_object();
  Line_3 l1 = construct_line();
  Line_3 l2 = construct_line(p5,p6);
  Line_3 l3 = construct_line(p2,p3);
  Line_3 l4 = construct_line(p2,d4);
  Line_3 l5 = construct_line(s2);
  Line_3 l6 = construct_line(r2);

  typename R::Construct_plane_3 construct_plane
        = rep.construct_plane_3_object();
  Plane_3 h1 = construct_plane();
  Plane_3 h2 = construct_plane(1,1,1,1);
  Plane_3 h3 = construct_plane(p2,p3,p4);
  Plane_3 h4 = construct_plane(p2,d4);
  Plane_3 h5 = construct_plane(l2,p4);
  Plane_3 h6 = construct_plane(r2,p4);
  Plane_3 h7 = construct_plane(s2,p4);

  typename R::Construct_sphere_3 construct_sphere
        = rep.construct_sphere_3_object();
  Sphere_3 sp1 = construct_sphere(p2,1);
  Sphere_3 sp2 = construct_sphere(p2,1,COUNTERCLOCKWISE);
  Sphere_3 sp3 = construct_sphere(p2,p3,p4,p5);
  Sphere_3 sp4 = construct_sphere(p2,p3,p4);
  Sphere_3 sp5 = construct_sphere(p2,p3,p4,CLOCKWISE);
  Sphere_3 sp6 = construct_sphere(p2,p3);
  Sphere_3 sp7 = construct_sphere(p2,p3,CLOCKWISE);
  Sphere_3 sp8 = construct_sphere(p3);
  Sphere_3 sp9 = construct_sphere(p3,CLOCKWISE);


  typename R::Construct_triangle_3 construct_triangle
        = rep.construct_triangle_3_object();
  Triangle_3 t1 = construct_triangle();
  Triangle_3 t2 = construct_triangle(p2,p3,p4);

  typename R::Construct_tetrahedron_3 construct_tetrahedron
        = rep.construct_tetrahedron_3_object();
  Tetrahedron_3 th1 = construct_tetrahedron();
  Tetrahedron_3 th2 = construct_tetrahedron(p2,p3,p4,p5);

  typename R::Construct_iso_cuboid_3 construct_iso_cuboid
        = rep.construct_iso_cuboid_3_object();
  Iso_cuboid_3 iso1 = construct_iso_cuboid(p3,p6);

  typename R::Construct_aff_transformation_3 construct_aff_transformation
        = rep.construct_aff_transformation_3_object();
  Aff_transformation_3 a1 = construct_aff_transformation();
  Aff_transformation_3 a2 = construct_aff_transformation(Translation(),v2);
  Aff_transformation_3 a4 = construct_aff_transformation(Scaling(),5);
  Aff_transformation_3 a41 = construct_aff_transformation(Scaling(),5,2);
  Aff_transformation_3 a5 =
        construct_aff_transformation(1,2,3,4,5,6,7,8,9,10,11,12);
  Aff_transformation_3 a51 =
        construct_aff_transformation(1,2,3,4,5,6,7,8,9,10,11,12,1);
  Aff_transformation_3 a6 =
        construct_aff_transformation(1,2,3,4,5,6,7,8,9);
  Aff_transformation_3 a61 =
        construct_aff_transformation(1,2,3,4,5,6,7,8,9,10);


  typename R::Construct_point_on_3 construct_point_on
        = rep.construct_point_on_3_object();
  Point_3 tmp1 = construct_point_on(l2);


  typename R::Construct_second_point_on_3 construct_second_point_on
        = rep.construct_second_point_on_3_object();
  Point_3 tmp2 = construct_second_point_on(r2);

  typename R::Construct_source_point_3 construct_source_point
        = rep.construct_source_point_3_object();
  Point_3 tmp2b = construct_source_point(s2);

  typename R::Construct_target_point_3 construct_target_point
        = rep.construct_target_point_3_object();
  Point_3 tmp2c = construct_target_point(s2);

  typename R::Construct_min_point_3 construct_min_point
        = rep.construct_min_point_3_object();
  Point_3 tmp2d = construct_min_point(s2);
  Point_3 tmp2e = construct_min_point(iso1);

  typename R::Construct_max_point_3 construct_max_point
        = rep.construct_max_point_3_object();
          tmp2d = construct_max_point(s2);
          tmp2e = construct_max_point(iso1);

  typename R::Construct_direction_of_line_3 construct_direction_from
        = rep.construct_direction_of_line_3_object();
  Direction_3 tmp7a = construct_direction_from(l2);

  typename R::Construct_direction_of_ray_3 construct_direction_from_ray
        = rep.construct_direction_of_ray_3_object();
  Direction_3 tmp7b = construct_direction_from_ray(r2);

  typename R::Construct_supporting_line_3 construct_supporting_line
        = rep.construct_supporting_line_3_object();
  Line_3 tmp8a = construct_supporting_line(r2);
  Line_3 tmp8b = construct_supporting_line(s2);

  typename R::Construct_perpendicular_plane_3 construct_perpendicular_plane
        = rep.construct_perpendicular_plane_3_object();
  Plane_3 tmp3 = construct_perpendicular_plane(l2,p2);

  typename R::Construct_perpendicular_line_3 construct_perpendicular_line
        = rep.construct_perpendicular_line_3_object();
  Line_3 tmp3a = construct_perpendicular_line(h2,p5);

  typename R::Construct_midpoint_3 construct_midpoint
        = rep.construct_midpoint_3_object();
  Point_3 tmp4 = construct_midpoint(p2,p3);

  typename R::Construct_center_3 construct_center
        = rep.construct_center_3_object();
          tmp4 = construct_center(sp2);

  typename R::Construct_circumcenter_3 construct_circumcenter
        = rep.construct_circumcenter_3_object();
          tmp4 = construct_circumcenter(p2,p3,p4,p5);

  typename R::Construct_cross_product_vector_3 construct_cross_product
        = rep.construct_cross_product_vector_3_object();
  Vector_3 tmp9 = construct_cross_product(v3,v4);

  typename R::Construct_opposite_segment_3 construct_opposite_segment
        = rep.construct_opposite_segment_3_object();
  Segment_3 tmp5 = construct_opposite_segment(s2);


  typename R::Construct_opposite_ray_3 construct_opposite_ray
        = rep.construct_opposite_ray_3_object();
  Ray_3 tmp6 = construct_opposite_ray(r2);


  typename R::Construct_opposite_line_3 construct_opposite_line
        = rep.construct_opposite_line_3_object();
  Line_3 tmp7 = construct_opposite_line(l2);

  typename R::Construct_opposite_plane_3 construct_opposite_plane
        = rep.construct_opposite_plane_3_object();
  Plane_3 tmp71 = construct_opposite_plane(h2);

  typename R::Construct_supporting_plane_3 construct_supporting_plane
        = rep.construct_supporting_plane_3_object();
  Plane_3 tmp8 = construct_supporting_plane(t2);


  typename R::Transform_3 transform
        = rep.transform_3_object();
  Point_3 tmp9a = transform(p2,a2);
  Triangle_3 tmp9b = transform(t2,a2);


  typename R::Intersect_3 intersect
        = rep.intersect_3_object();
  Object3 tmp10a = intersect(l2,h2);
  Object3 tmp10b = intersect(r2,h2);

  bool tmp12a;
  bool tmp12b;

  typename R::Do_intersect_3 do_intersect
        = rep.do_intersect_3_object();
     tmp12a = do_intersect(l2,h2);
     tmp12b = do_intersect(r2,h2);

  typename R::Assign_3  assign
        = rep.assign_3_object();
       tmp12a = assign(p1,tmp10a);
       tmp12b = assign(p1,tmp10b);

  typename R::Compute_squared_distance_3 Compute_squared_distance
        = rep.compute_squared_distance_3_object();
  FT tmp12c = Compute_squared_distance(p1, p2);
     tmp12c = Compute_squared_distance(p1, r2);
     tmp12c = Compute_squared_distance(p1, h2);

  typename R::Compute_squared_length_3 compute_squared_length
        = rep.compute_squared_length_3_object();
  FT tmp11 = compute_squared_length(s2);

  
  typename R::Compute_squared_radius_3 Compute_squared_radius
        = rep.compute_squared_radius_3_object();
  FT tmp11aa = Compute_squared_radius(sp1);
     tmp11aa = Compute_squared_radius(p3, p4, p5);
     tmp11aa = Compute_squared_radius(p3, p4, p5, p6);

  typename R::Compute_squared_area_3 compute_squared_area
        = rep.compute_squared_area_3_object();
  FT tmp11a = compute_squared_area(tmp9b);

  typename R::Compute_volume_3 compute_volume
        = rep.compute_volume_3_object();
  FT tmp11b = compute_volume(th2);
  FT tmp11c = compute_volume(iso1);


  typename R::Equal_3 equal
        = rep.equal_3_object();
       tmp12a = equal(p2,p3);
       tmp12b = equal(l2,l3);


  typename R::Equal_x_3 equal_x
        = rep.equal_x_3_object();
  bool tmp13 = equal_x(p2,p3);


  typename R::Equal_y_3 equal_y
        = rep.equal_y_3_object();
  bool tmp14 = equal_y(p2,p3);


  typename R::Equal_z_3 equal_z
        = rep.equal_z_3_object();
  bool tmp15 = equal_z(p2,p3);


  typename R::Equal_xy_3 equal_xy
        = rep.equal_xy_3_object();
  bool tmp16 = equal_xy(p2,p3);


  typename R::Equal_xyz_3 equal_xyz
        = rep.equal_xyz_3_object();
  bool tmp17 = equal_xyz(p2,p3);


  typename R::Less_x_3 less_x
        = rep.less_x_3_object();
  bool tmp18 = less_x(p2,p3);


  typename R::Less_y_3 less_y
        = rep.less_y_3_object();
  bool tmp19 = less_y(p2,p3);


  typename R::Less_z_3 less_z
        = rep.less_z_3_object();
  bool tmp20 = less_z(p2,p3);


  typename R::Less_xy_3 less_xy
        = rep.less_xy_3_object();
  bool tmp21 = less_xy(p2,p3);


  typename R::Less_xyz_3 less_xyz
        = rep.less_xyz_3_object();
  bool tmp22 = less_xyz(p2,p3);


  typename R::Compare_x_3 compare_x
        = rep.compare_x_3_object();
  Comparison_result tmp23 = compare_x(p2,p3);


  typename R::Compare_y_3 compare_y
        = rep.compare_y_3_object();
  Comparison_result tmp24 = compare_y(p2,p3);


  typename R::Compare_z_3 compare_z
        = rep.compare_z_3_object();
  Comparison_result tmp25 = compare_z(p2,p3);


  typename R::Compare_xy_3 compare_xy
        = rep.compare_xy_3_object();
  Comparison_result tmp26 = compare_xy(p2,p3);


  typename R::Compare_xyz_3 compare_xyz
        = rep.compare_xyz_3_object();
  Comparison_result tmp27 = compare_xyz(p2,p3);

  typename R::Less_distance_to_point_3 less_distance_to_point
        = rep.less_distance_to_point_3_object(p4);
  bool tmp28 = less_distance_to_point(p2,p3);

  typename R::Less_signed_distance_to_plane_3 less_signed_distance_to_plane
        = rep.less_signed_distance_to_plane_3_object();
  bool tmp28a = less_signed_distance_to_plane(tmp8,p2,p3);

  typename R::Compare_distance_3 compare_dist
        = rep.compare_distance_3_object();
  Comparison_result tmp34ab = compare_dist(p2,p3,p4);

  typename R::Collinear_3 collinear
        = rep.collinear_3_object();
  bool tmp29 = collinear(p2,p3,p4);

  typename R::Coplanar_3 coplanar
        = rep.coplanar_3_object();
  bool tmp30 = coplanar(p2,p3,p4,p5);

  Point_3 p7(0,0,0);
  Point_3 p8(1,0,0);
  Point_3 p9(1,1,0);
  Point_3 p10(0,1,0);
  typename R::Coplanar_orientation_3 coplanar_orientation
        = rep.coplanar_orientation_3_object();
  Orientation tmp30a = coplanar_orientation(p7,p8,p9,p10);

  typename R::Coplanar_side_of_bounded_circle_3
           coplanar_side_of_bounded_circle
        = rep.coplanar_side_of_bounded_circle_3_object();
  Bounded_side tmp30b = coplanar_side_of_bounded_circle(p7,p8,p9,p10);

  typename R::Orientation_3 orientation
        = rep.orientation_3_object();
  Orientation tmp31 = orientation(p2,p3,p4,p5);


  typename R::Is_degenerate_3 is_degenerate
        = rep.is_degenerate_3_object();
  bool tmp32 = is_degenerate(l2);


  typename R::Has_on_3 has_on
        = rep.has_on_3_object();
  bool tmp33a = has_on(l2,p2);
  bool tmp33b = has_on(t2,p2);
  bool tmp33c = has_on(tmp8,p2);


  typename R::Has_on_bounded_side_3 has_on_bounded_side
        = rep.has_on_bounded_side_3_object();
  bool tmp34 = has_on_bounded_side(th2,p2);


  typename R::Has_on_unbounded_side_3 has_on_unbounded_side
        = rep.has_on_unbounded_side_3_object();
  bool tmp35 = has_on_unbounded_side(th2,p2);


  typename R::Has_on_boundary_3 has_on_boundary
        = rep.has_on_boundary_3_object();
  bool tmp36a = has_on_boundary(h2,p2);
  bool tmp36b = has_on_boundary(th2,p2);


  typename R::Has_on_positive_side_3 has_on_positive_side
        = rep.has_on_positive_side_3_object();
  bool tmp37 = has_on_positive_side(h2,p2);


  typename R::Has_on_negative_side_3 has_on_negative_side
        = rep.has_on_negative_side_3_object();
  bool tmp38 = has_on_negative_side(h2,p2);


  typename R::Oriented_side_3 oriented_side
        = rep.oriented_side_3_object();
  Oriented_side tmp39 = oriented_side(h2,p2);


  typename R::Are_ordered_along_line_3 are_ordered_along_line
        = rep.are_ordered_along_line_3_object();
  bool tmp40 = are_ordered_along_line(p2,p3,p4);


  typename R::Are_strictly_ordered_along_line_3 are_strictly_ordered_along_line
        = rep.are_strictly_ordered_along_line_3_object();
  bool tmp41 = are_strictly_ordered_along_line(p2,p3,p4);


  typename R::Collinear_are_ordered_along_line_3 collinear_are_ordered_along_line
        = rep.collinear_are_ordered_along_line_3_object();
  bool tmp42 = collinear_are_ordered_along_line(p2,p2,p3);


  typename R::Collinear_are_strictly_ordered_along_line_3 collinear_are_strictly_ordered_along_line
        = rep.collinear_are_strictly_ordered_along_line_3_object();
  bool tmp43 = collinear_are_strictly_ordered_along_line(p2,p2,p3);


  typename R::Side_of_oriented_sphere_3 side_of_oriented_sphere
        = rep.side_of_oriented_sphere_3_object();
  Oriented_side tmp44 = side_of_oriented_sphere(p2,p3,p4,p5,p6);


  typename R::Side_of_bounded_sphere_3 side_of_bounded_sphere
        = rep.side_of_bounded_sphere_3_object();
  Bounded_side tmp45 = side_of_bounded_sphere(p2,p3,p4,p5,p6);
               tmp45 = side_of_bounded_sphere(p2,p3,p4,p6);
               tmp45 = side_of_bounded_sphere(p2,p3,p6);

  typename R::Angle_3 angle
        = rep.angle_3_object();
  Angle tmp46 = angle(p2,p3,p4);

  use(tmp46);
  use(tmp45); use(tmp44); use(tmp43); use(tmp42); use(tmp41); use(tmp40);
  use(tmp39); use(tmp38); use(tmp37); use(tmp36b); use(tmp36a); use(tmp35);
  use(tmp34); use(tmp33b); use(tmp33a); use(tmp32); use(tmp31); use(tmp30);
  use(tmp28a); use(tmp33c);
  use(tmp29); use(tmp27); use(tmp25); use(tmp24); use(tmp23); use(tmp22);
  use(tmp15); use(tmp14); use(tmp13); use(tmp12b); use(tmp12a);
  use(tmp16); use(tmp17); use(tmp18); use(tmp19); use(tmp20); use(tmp21);
  use(tmp26); use(tmp28); use(tmp30a); use(tmp30b); use(tmp34ab);

  return true;
}

#endif // CGAL_TEST_NEW_3_H
