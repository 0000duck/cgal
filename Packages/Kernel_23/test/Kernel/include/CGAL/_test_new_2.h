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
// file          : test/Kernel/include/CGAL/_test_new_2.h
// revision      : $Revision$
// revision_date : $Date$
// author(s)     : Michael Seel
//                 Stefan Schirra
//
// coordinator   : MPI, Saarbruecken  (<Stefan.Schirra@mpi-sb.mpg.de>)
// ============================================================================
 

#ifndef CGAL__TEST_NEW_2_H
#define CGAL__TEST_NEW_2_H

#include <CGAL/Point_2.h>
#include <CGAL/Vector_2.h>
#include <CGAL/Direction_2.h>
#include <CGAL/Segment_2.h>
#include <CGAL/Ray_2.h>
#include <CGAL/Line_2.h>
#include <CGAL/Circle_2.h>
#include <CGAL/Triangle_2.h>
#include <CGAL/Aff_transformation_2.h>
#include <CGAL/intersections.h>
#include <CGAL/predicates_on_points_2.h>
#include <CGAL/predicates_on_lines_2.h>
#include <CGAL/distance_predicates_2.h>
#include <CGAL/squared_distance_2.h>

void use(bool) {}
void use(CGAL::Sign) {}
void use(CGAL::Bounded_side) {}
void use(CGAL::Oriented_side) {}
void use(CGAL::Comparison_result) {}


template <class R>
bool
test_new_2(const R& rep)
{
  std::cout << "Testing 2 dimensional functionality" << std::endl;

  using namespace CGAL;

  typedef typename R::RT                          RT;
  typedef typename R::FT                          FT;

  typedef typename R::Point_2                     Point_2;
  typedef typename R::Vector_2                    Vector_2;
  typedef typename R::Direction_2                 Direction_2;
  typedef typename R::Segment_2                   Segment_2;
  typedef typename R::Line_2                      Line_2;
  typedef typename R::Ray_2                       Ray_2;
  typedef typename R::Circle_2                    Circle_2;
  typedef typename R::Triangle_2                  Triangle_2;
  typedef typename R::Iso_rectangle_2             Iso_rectangle_2;
  typedef typename R::Aff_transformation_2        Aff_transformation_2;
  typedef typename R::Object_2                    Object2;

  typename R::Construct_point_2 construct_point =
        rep.construct_point_2_object();
  Point_2 p1 = construct_point();
  Point_2 p2 = construct_point(ORIGIN);
  Point_2 p3 = construct_point(1,1);
  Point_2 p4 = construct_point(1,2,2);
  Point_2 p5 = construct_point(3,4,5);
  Point_2 p6 = construct_point(3,4,6);

  typename R::Construct_vector_2 construct_vector =
        rep.construct_vector_2_object();
  Vector_2 v1 = construct_vector();
  Vector_2 v2 = construct_vector(NULL_VECTOR);
  Vector_2 v3 = construct_vector(1,3);
  Vector_2 v4 = construct_vector(1,2,3);

  typename R::Construct_direction_2 construct_direction
        = rep.construct_direction_2_object();
  Direction_2 d1 = construct_direction();
  Direction_2 d2 = construct_direction(v3);
  Direction_2 d3 = construct_direction(1,4);

  typename R::Construct_segment_2 construct_segment
        = rep.construct_segment_2_object();
  Segment_2 s1 = construct_segment();
  Segment_2 s2 = construct_segment(p2,p3);

  typename R::Construct_ray_2 construct_ray =
        rep.construct_ray_2_object();
  Ray_2 r1 = construct_ray();
  Ray_2 r2 = construct_ray(p2,p5);
  Ray_2 r3 = construct_ray(p2,d3);

  typename R::Construct_line_2 construct_line
        = rep.construct_line_2_object();
  Line_2 l1 = construct_line();
  Line_2 l2 = construct_line(1,1,1);
  Line_2 l3 = construct_line(p2,p6);
  Line_2 l4 = construct_line(p2,d3);
  Line_2 l5 = construct_line(s2);
  Line_2 l6 = construct_line(r2);

  typename R::Construct_circle_2 construct_circle
        = rep.construct_circle_2_object();
  Circle_2 c1 = construct_circle(p2,1);
  Circle_2 c11 = construct_circle(p2,1,COUNTERCLOCKWISE);
  Circle_2 c2 = construct_circle(p2,p3,p4);
  Circle_2 c31 = construct_circle(p2,p3,COUNTERCLOCKWISE);
  Circle_2 c4 = construct_circle(p2);
  Circle_2 c41 = construct_circle(p2,COUNTERCLOCKWISE);

  typename R::Construct_triangle_2 construct_triangle
        = rep.construct_triangle_2_object();
  Triangle_2 t1 = construct_triangle();
  Triangle_2 t2 = construct_triangle(p2,p3,p4);

  typename R::Construct_iso_rectangle_2 construct_iso_rectangle
        = rep.construct_iso_rectangle_2_object();
  Iso_rectangle_2 rec2 = construct_iso_rectangle(p4,p5);

  typename R::Construct_aff_transformation_2 construct_aff_transformation
        = rep.construct_aff_transformation_2_object();
  Aff_transformation_2 a1 = construct_aff_transformation(Translation(),v2);
  Aff_transformation_2 a2 = construct_aff_transformation(Rotation(),d2,1);
  Aff_transformation_2 a21 = construct_aff_transformation(Rotation(),d2,1,2);
  Aff_transformation_2 a3 = construct_aff_transformation(Rotation(),1,1);
  Aff_transformation_2 a31 = construct_aff_transformation(Rotation(),3,4,5);
  Aff_transformation_2 a4 = construct_aff_transformation(Scaling(),5);
  Aff_transformation_2 a41 = construct_aff_transformation(Scaling(),5,2);
  Aff_transformation_2 a5 = construct_aff_transformation(1,2,3,4,5,6);
  Aff_transformation_2 a51 = construct_aff_transformation(1,2,3,4,5,6,1);
  Aff_transformation_2 a6 = construct_aff_transformation(1,2,3,4);
  Aff_transformation_2 a61 = construct_aff_transformation(1,2,3,4,1);


  typename R::Construct_point_on_2 construct_point_on
        = rep.construct_point_on_2_object();
  Point_2 tmp1 = construct_point_on(l2);


  typename R::Construct_second_point_on_2 construct_second_point_on
        = rep.construct_second_point_on_2_object();
  Point_2 tmp2 = construct_second_point_on(r2);


  typename R::Construct_source_point_2 construct_source_point
        = rep.construct_source_point_2_object();
  Point_2 tmp3 = construct_source_point(s2);


  typename R::Construct_target_point_2 construct_target_point
        = rep.construct_target_point_2_object();
  Point_2 tmp4 = construct_target_point(s2);


  typename R::Construct_min_point_2 construct_min_point
        = rep.construct_min_point_2_object();
  Point_2 tmp5a = construct_min_point(s2);
  Point_2 tmp5b = construct_min_point(rec2);

  typename R::Construct_max_point_2 construct_max_point
        = rep.construct_max_point_2_object();
  Point_2 tmp6a = construct_max_point(s2);
  Point_2 tmp6b = construct_max_point(rec2);


  typename R::Construct_direction_of_line_2 construct_direction_from
        = rep.construct_direction_of_line_2_object();
  Direction_2 tmp7a = construct_direction_from(l2);

  typename R::Construct_direction_of_ray_2 construct_direction_from_ray
        = rep.construct_direction_of_ray_2_object();
  Direction_2 tmp7b = construct_direction_from_ray(r2);


  typename R::Construct_supporting_line_2 construct_supporting_line
        = rep.construct_supporting_line_2_object();
  Line_2 tmp8a = construct_supporting_line(r2);
  Line_2 tmp8b = construct_supporting_line(s2);

  typename R::Construct_perpendicular_vector_2 construct_perpendicular_vector
        = rep.construct_perpendicular_vector_2_object();
  Vector_2 tmp9 = construct_perpendicular_vector(v2,CLOCKWISE);

  typename R::Construct_perpendicular_direction_2 construct_perpendicular_direction
        = rep.construct_perpendicular_direction_2_object();
  Direction_2 tmp10 = construct_perpendicular_direction(d2,COUNTERCLOCKWISE);

  typename R::Construct_perpendicular_line_2 construct_perpendicular_line
        = rep.construct_perpendicular_line_2_object();
  Line_2 tmp11 = construct_perpendicular_line(l2,p2);

  typename R::Construct_midpoint_2 construct_midpoint_2
        = rep.construct_midpoint_2_object();
  Point_2 tmp12 = construct_midpoint_2(p2,p3);

//  typename R::Construct_midpoint construct_midpoint
//        = rep.construct_midpoint_object();
//  Point_2 tmp12 = construct_midpoint(p2,p3);

  typename R::Construct_circumcenter_2 construct_circumcenter
        = rep.construct_circumcenter_2_object();
  Point_2 tmp13 = construct_circumcenter(p2,p3,p4);


  typename R::Construct_bisector_2 construct_bisector
        = rep.construct_bisector_2_object();
  Line_2 tmp14 = construct_bisector(p2,p3);


  typename R::Construct_opposite_segment_2 construct_opposite_segment
        = rep.construct_opposite_segment_2_object();
  Segment_2 tmp15 = construct_opposite_segment(s2);


  typename R::Construct_opposite_ray_2 construct_opposite_ray
        = rep.construct_opposite_ray_2_object();
  Ray_2 tmp16 = construct_opposite_ray(r2);


  typename R::Construct_opposite_line_2 construct_opposite_line
        = rep.construct_opposite_line_2_object();
  Line_2 tmp17 = construct_opposite_line(l2);


  typename R::Construct_opposite_triangle_2 construct_opposite_triangle
        = rep.construct_opposite_triangle_2_object();
  Triangle_2 tmp18 = construct_opposite_triangle(t2);


  typename R::Construct_opposite_circle_2 construct_opposite_circle
        = rep.construct_opposite_circle_2_object();
  Circle_2 tmp19 = construct_opposite_circle(c2);


  typename R::Transform_2 transform
        = rep.transform_2_object();
  Point_2 tmp20a = transform(p2,a3);
  Line_2 tmp20b = transform(l2,a3);


  typename R::Intersect_2 intersect
        = rep.intersect_2_object();
  Object2 tmp21a = intersect(l2,l3);
  Object2 tmp21b = intersect(l2,r2);

  bool tmp_bool;

  typename R::Do_intersect_2 do_intersect
        = rep.do_intersect_2_object();
   tmp_bool = do_intersect(l2,l3);
   tmp_bool = do_intersect(l2,r2);

  typename R::Assign_2  assign
        = rep.assign_2_object();
       tmp_bool = assign(p1,tmp21a);
       tmp_bool = assign(p1,tmp21b);

  typename R::Compute_y_at_x_2 compute_y_at_x
        = rep.compute_y_at_x_2_object();
  FT tmp22 = compute_y_at_x(l2, FT(11));

  typename R::Compute_area_2 compute_area_2
        = rep.compute_area_2_object();
  FT tmp22a = compute_area_2(tmp18);
  FT tmp22b = compute_area_2(rec2);

  typename R::Compute_squared_distance_2 Compute_squared_distance
        = rep.compute_squared_distance_2_object();
  FT tmp22c = Compute_squared_distance(p1, p2);
     tmp22c = Compute_squared_distance(p1, r2);
     tmp22c = Compute_squared_distance(p1, t2);

  typename R::Compute_squared_length_2 Compute_squared_length
        = rep.compute_squared_length_2_object();
  FT tmp23 = Compute_squared_length(s2);

  typename R::Equal_2 equal
        = rep.equal_2_object();
  bool tmp24 = equal(p2,p3);


  typename R::Equal_x_2 equal_x
        = rep.equal_x_2_object();
  bool tmp25 = equal_x(p2,p3);


  typename R::Equal_y_2 equal_y
        = rep.equal_y_2_object();
  bool tmp26 = equal_y(p2,p3);


  typename R::Equal_xy_2 equal_xy
        = rep.equal_xy_2_object();
  bool tmp27 = equal_xy(p2,p3);

  typename R::Less_x_2 less_x
        = rep.less_x_2_object();
  bool tmp28 = less_x(p2,p3);


  typename R::Less_y_2 less_y
        = rep.less_y_2_object();
  bool tmp29 = less_y(p2,p3);


  typename R::Less_xy_2 less_xy
        = rep.less_xy_2_object();
  bool tmp30 = less_xy(p2,p3);


  typename R::Compare_x_2 compare_x
        = rep.compare_x_2_object();
  Comparison_result tmp31a = compare_x(p2,p3);
  Comparison_result tmp31b = compare_x(p2,l2,l3);
  Comparison_result tmp31c = compare_x(l2,l3,l4);
  Comparison_result tmp31d = compare_x(l2,l3,l4,l5);


  typename R::Compare_y_2 compare_y
        = rep.compare_y_2_object();
  Comparison_result tmp32a = compare_y(p2,p3);
  Comparison_result tmp32b = compare_y(p2,l2,l3);
  Comparison_result tmp32c = compare_y(l2,l3,l4);
  Comparison_result tmp32d = compare_y(l2,l3,l4,l5);


  typename R::Compare_xy_2 compare_xy
        = rep.compare_xy_2_object();
  Comparison_result tmp33a = compare_xy(p2,p3);


  typename R::Compare_y_at_x_2 compare_y_at_x
        = rep.compare_y_at_x_2_object();
  Comparison_result tmp34a = compare_y_at_x(p2,l2);
  Comparison_result tmp34b = compare_y_at_x(p2,l2,l3);
  Comparison_result tmp34c = compare_y_at_x(l2,l3,l4);
  Comparison_result tmp34d = compare_y_at_x(l2,l3,l4,l5);

//
//  the compare_x_at_y function is not yet available in the Cartesian kernel
//
//  typename R::Compare_x_at_y_2 compare_x_at_y
//        = rep.compare_x_at_y_2_object();
//  Comparison_result tmp34aa = compare_x_at_y(p2,l2);
//  Comparison_result tmp34bb = compare_x_at_y(p2,l2,l3);
//  Comparison_result tmp34cc = compare_x_at_y(l2,l3,l4);
//  Comparison_result tmp34dd = compare_x_at_y(l2,l3,l4,l5);

  typename R::Less_distance_to_point_2 less_distance_to_point
        = rep.less_distance_to_point_2_object(p2);
  bool tmp35 = less_distance_to_point(p3,p4);

  typename R::Compare_distance_to_point_2 compare_dist_to_point
        = rep.compare_distance_to_point_2_object(p2);
  Comparison_result tmp34ab = compare_dist_to_point(p2,p3);

  typename R::Less_signed_distance_to_line_2 less_signed_distance_to_line
        = rep.less_signed_distance_to_line_2_object(p4,p5);
  bool tmp36 = less_signed_distance_to_line(p2,p3);

  typename R::Less_rotate_ccw_2 less_rotate_ccw 
        = rep.less_rotate_ccw_2_object(p4);
  bool tmp36a = less_rotate_ccw(p2,p3);

  bool tmp39;

  typename R::Counterclockwise_in_between_2 ccwib
        = rep.counterclockwise_in_between_2_object();
       tmp39 = ccwib(d1,d2,d3);

  typename R::Leftturn_2 leftturn
        = rep.leftturn_2_object();
  bool tmp37 = leftturn(p2,p3,p4);


  typename R::Left_of_line_2 left_of_line
        = rep.left_of_line_2_object(p2,p3);
  bool tmp38 = left_of_line(p4);


  typename R::Collinear_2 collinear
        = rep.collinear_2_object();
  bool tmp39a = collinear(p2,p3,p4);


  typename R::Orientation_2 orientation
        = rep.orientation_2_object();
  Orientation tmp40 = orientation(p2,p3,p4);


  typename R::Side_of_oriented_circle_2 side_of_oriented_circle
        = rep.side_of_oriented_circle_2_object();
  Oriented_side tmp41 = side_of_oriented_circle(p2,p3,p4,p5);


  typename R::Side_of_bounded_circle_2 side_of_bounded_circle
        = rep.side_of_bounded_circle_2_object();
  Bounded_side tmp42 = side_of_bounded_circle(p2,p3,p4,p5);


  typename R::Is_horizontal_2 is_horizontal
        = rep.is_horizontal_2_object();
  bool tmp43 = is_horizontal(l2);


  typename R::Is_vertical_2 is_vertical
        = rep.is_vertical_2_object();
  bool tmp44 = is_vertical(l2);


  typename R::Is_degenerate_2 is_degenerate
        = rep.is_degenerate_2_object();
  bool tmp45 = is_degenerate(l2);


  typename R::Has_on_2 has_on
        = rep.has_on_2_object();
  bool tmp46 = has_on(l2,p2);


  typename R::Collinear_has_on_2 collinear_has_on
        = rep.collinear_has_on_2_object();
  bool tmp47 = collinear_has_on(s2,p2);


  typename R::Has_on_bounded_side_2 has_on_bounded_side
        = rep.has_on_bounded_side_2_object();
  bool tmp48a = has_on_bounded_side(c2,p2);
  bool tmp48b = has_on_bounded_side(t2,p2);


  typename R::Has_on_unbounded_side_2 has_on_unbounded_side
        = rep.has_on_unbounded_side_2_object();
  bool tmp49a = has_on_unbounded_side(c2,p2);
  bool tmp49b = has_on_unbounded_side(t2,p2);


  typename R::Has_on_boundary_2 has_on_boundary
        = rep.has_on_boundary_2_object();
  bool tmp50a = has_on_boundary(c2,p2);
  bool tmp50b = has_on_boundary(t2,p2);


  typename R::Has_on_positive_side_2 has_on_positive_side
        = rep.has_on_positive_side_2_object();
  bool tmp51a = has_on_positive_side(l2,p2);
  bool tmp51b = has_on_positive_side(c2,p2);


  typename R::Has_on_negative_side_2 has_on_negative_side
        = rep.has_on_negative_side_2_object();
  bool tmp52a = has_on_negative_side(l2,p2);
  bool tmp52b = has_on_negative_side(c2,p2);


  typename R::Oriented_side_2 oriented_side
        = rep.oriented_side_2_object();
  Oriented_side tmp53a = oriented_side(l2,p2);
  Oriented_side tmp53b = oriented_side(c2,p2);


  typename R::Are_ordered_along_line_2 are_ordered_along_line
        = rep.are_ordered_along_line_2_object();
  bool tmp54 = are_ordered_along_line(p2,p2,p3);


  typename R::Are_strictly_ordered_along_line_2 are_strictly_ordered_along_line
        = rep.are_strictly_ordered_along_line_2_object();
  bool tmp55 = are_strictly_ordered_along_line(p2,p2,p3);


  typename R::Collinear_are_ordered_along_line_2 collinear_are_ordered_along_line
        = rep.collinear_are_ordered_along_line_2_object();
  bool tmp56 = collinear_are_ordered_along_line(p2,p2,p3);


  typename R::Collinear_are_strictly_ordered_along_line_2 collinear_are_strictly_ordered_along_line
        = rep.collinear_are_strictly_ordered_along_line_2_object();
  bool tmp57 = collinear_are_strictly_ordered_along_line(p2,p2,p3);

  use(tmp57); use(tmp56); use(tmp55); use(tmp54); use(tmp53b); use(tmp53a);
  use(tmp52b); use(tmp52a); use(tmp51b); use(tmp51a); use(tmp50b); use(tmp50a);
  use(tmp49b); use(tmp49a); use(tmp48b); use(tmp48a); use(tmp47); use(tmp46);
  use(tmp45); use(tmp44); use(tmp43); use(tmp42); use(tmp41); use(tmp40);
  use(tmp39); use(tmp38); use(tmp37); use(tmp36); use(tmp35); use(tmp34d);
  use(tmp34c); use(tmp34b); use(tmp34a); use(tmp32d); use(tmp32c); use(tmp32b);
  use(tmp32a); use(tmp31d); use(tmp31c); use(tmp31b); use(tmp31a); use(tmp30);
  use(tmp26); use(tmp25); use(tmp24);
  use(tmp29); use(tmp28); use(tmp27); use(tmp33a); use(tmp34ab); 
// use(tmp34dd); use(tmp34cc); use(tmp34bb); use(tmp34aa); 
  use(tmp39a); use(tmp36a);
  use(tmp_bool);
  

  return true;
}

#endif // CGAL__TEST_NEW_2_H
