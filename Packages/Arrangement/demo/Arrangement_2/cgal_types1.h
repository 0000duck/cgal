#ifndef CGAL_TYPES_HEADER
#define CGAL_TYPES_HEADER

#include <CGAL/basic.h>
#include <CGAL/Cartesian.h>
#include <CGAL/MP_Float.h>
#include <CGAL/Quotient.h>
#include <CGAL/Pm_default_dcel.h>
#include <CGAL/Arr_segment_cached_traits_2.h>
#include <CGAL/Arr_curve_data_traits_2.h>
#include <CGAL/Arr_curve_origin_traits_2.h>
#include <CGAL/Arr_polyline_traits_2.h>
#include <CGAL/Pm_with_intersections.h>
#include <CGAL/squared_distance_2.h>
#include <CGAL/Polygon_2.h>
#include <CGAL/Polygon_traits_2.h>
#include <CGAL/IO/Arr_iostream.h>
#include <CGAL/point_generators_2.h>
#include <CGAL/Arr_conic_traits_2.h>
#include <CGAL/IO/write_pm.h>
#include <CGAL/IO/Pm_iostream.h>
#include <CGAL/leda_real.h>

#include <iostream>

// Coordinate related typedef - using inexact number type
typedef float                                                             Coord_type;
typedef CGAL::Cartesian<Coord_type>                                       Coord_kernel;
typedef Coord_kernel::Point_2                                             Coord_point;
typedef Coord_kernel::Segment_2                                           Coord_segment;
typedef CGAL::Polygon_traits_2<Coord_kernel>                              PT;
typedef std::vector<Coord_point>                                          Container;
typedef CGAL::Polygon_2<PT, Container>                                    Cgal_polygon;

// Planar map typedef - using rational exact number type
typedef CGAL::Quotient<CGAL::MP_Float>								      NT;
typedef CGAL::Cartesian<NT>												  Kernel;

typedef leda_real														  CONIC_NT;
typedef CGAL::Cartesian<CONIC_NT>										  Conic_kernel;

// Segments:
typedef CGAL::Arr_segment_cached_traits_2<Kernel>                         Base_seg_traits;
typedef Base_seg_traits::Curve_2                                          Pm_base_seg_2;

struct Curve_data {
  enum Type {LEAF, INTERNAL};
  int m_index;
  Type m_type;
  union Pointer {
    Pm_base_seg_2 * m_curve;
    void * m_x_motonote_curve;
  } m_ptr;
};

typedef CGAL::Arr_curve_data_traits_2<Base_seg_traits,Curve_data>         Seg_traits;
typedef Seg_traits::Curve_2                                               Pm_seg_2;
typedef Seg_traits::X_monotone_curve_2                                    Pm_xseg_2;
typedef Seg_traits::Point_2					        				      Pm_seg_point_2;
typedef CGAL::Pm_default_dcel<Seg_traits>                                 Seg_dcel;
typedef CGAL::Planar_map_2<Seg_dcel, Seg_traits>                          Seg_pm;
typedef CGAL::Planar_map_with_intersections_2<Seg_pm>		              Seg_arr;
typedef Seg_arr::Locate_type                                              Seg_locate_type;
typedef Seg_arr::Halfedge_handle                                          Seg_halfedge_handle;

typedef std::list<Pm_seg_2*>						                      Pm_seg_list;
typedef Pm_seg_list::const_iterator							              Pm_seg_const_iter;
typedef Pm_seg_list::iterator								              Pm_seg_iter;

// Polyline
typedef CGAL::Arr_polyline_traits_2<Base_seg_traits>				      Base_pol_traits;
typedef Base_pol_traits::Curve_2								      	  Pm_base_pol_2;

struct Curve_pol_data {
  enum Type {LEAF, INTERNAL};
  int m_index;
  Type m_type;
  union Pointer {
    Pm_base_pol_2 * m_curve;
    void * m_x_motonote_curve;
  } m_ptr;
};

typedef CGAL::Arr_curve_data_traits_2<Base_pol_traits,Curve_pol_data>     Pol_traits;
//typedef CGAL::Arr_curve_origin_traits_2<Pol_data_traits>			      Pol_traits;
typedef Pol_traits::Curve_2											      Pm_pol_2;
typedef Pol_traits::X_monotone_curve_2							    	  Pm_xpol_2;
typedef Pol_traits::Point_2									    		  Pm_pol_point_2;
typedef CGAL::Pm_default_dcel<Pol_traits>				    			  Pol_dcel;
typedef CGAL::Planar_map_2<Pol_dcel, Pol_traits>					      Pol_pm;
typedef CGAL::Planar_map_with_intersections_2<Pol_pm>				      Pol_arr;
typedef Pol_arr::Locate_type									    	  Pol_locate_type;
typedef Pol_arr::Halfedge_handle							    		  Pol_halfedge_handle;
typedef Pol_arr::Face_handle							    			  Pol_face_handle;
typedef Pol_arr::Ccb_halfedge_circulator			    				  Pol_ccb_halfedge_circulator;
typedef Pol_arr::Holes_iterator                                           Pol_holes_iterator;

typedef std::list<Pm_pol_2*>								              Pm_pol_list;
typedef Pm_pol_list::const_iterator							              Pm_pol_const_iter;
typedef Pm_pol_list::iterator							                  Pm_pol_iter;

// Conics
typedef CGAL::Arr_conic_traits_2<Conic_kernel>                            Base_conic_traits;
typedef Base_conic_traits::Curve_2                                        Pm_base_conic_2;

struct Curve_conic_data {
  enum Type {LEAF, INTERNAL};
  int m_index;
  Type m_type;
  union Pointer {
    Pm_base_conic_2 * m_curve;
    void * m_x_motonote_curve;
  } m_ptr;
};

typedef CGAL::Arr_curve_data_traits_2<Base_conic_traits,Curve_conic_data> Conic_traits;
//typedef CGAL::Arr_curve_origin_traits_2<Conic_data_traits>			  Conic_traits;
typedef Conic_traits::Curve_2											  Pm_conic_2;
typedef Conic_traits::X_monotone_curve_2                                  Pm_xconic_2;
typedef Conic_traits::Point_2                                             Pm_conic_point_2;
typedef Conic_traits::Segment_2                                           Pm_conic_segment_2;
typedef Conic_traits::Circle_2                                            Pm_conic_circle_2;
typedef CGAL::Pm_default_dcel<Conic_traits>                               Conic_dcel;
typedef CGAL::Planar_map_2<Conic_dcel, Conic_traits>                      Conic_pm;
typedef CGAL::Planar_map_with_intersections_2<Conic_pm>                   Conic_arr;
typedef Conic_arr::Locate_type                                            Conic_locate_type;
typedef Conic_arr::Halfedge_handle                                        Conic_halfedge_handle;
typedef CGAL::Pm_file_scanner<Conic_arr>                                  Pm_scanner;

typedef std::list<Pm_xconic_2*>								              Pm_xconic_list;
typedef Pm_xconic_list::const_iterator						              Pm_xconic_const_iter;
typedef Pm_xconic_list::iterator							              Pm_xconic_iter;

#endif
