#ifndef CGAL_KD_KERNEL_2_INTERFACE_H
#define CGAL_KD_KERNEL_2_INTERFACE_H

#include <CGAL/functor_tags.h>
#include <CGAL/transforming_iterator.h>
#include <CGAL/marcutils.h>
#include <CGAL/tuple.h>


namespace CGAL {
template <class Base_> struct Kernel_2_interface : public Base_ {
	typedef Base_ Base;
	typedef Kernel_2_interface<Base> Kernel;
	typedef typename Base::Point  Point_2;
	typedef typename Base::Vector Vector_2;
	struct Segment_2 {};
	typedef cpp0x::tuple<Point_2,Point_2,Point_2> Triangle_2; // triangulation insists...
	template <class T,int i> struct Help_2p_i {
		typedef typename Get_functor<Base, T>::type LT;
		typedef typename LT::result_type result_type;
		LT lt;
		Help_2p_i(Kernel const&k):lt(k){}
		result_type operator()(Point_2 const&a, Point_2 const&b) {
			return lt(a,b,i);
		}
	};
	typedef Help_2p_i<Less_point_cartesian_coordinate_tag,0> Less_x_2;
	typedef Help_2p_i<Less_point_cartesian_coordinate_tag,1> Less_y_2;
	typedef Help_2p_i<Compare_point_cartesian_coordinate_tag,0> Compare_x_2;
	typedef Help_2p_i<Compare_point_cartesian_coordinate_tag,1> Compare_y_2;
	struct Compare_distance_2 {
		typedef typename Get_functor<Base, Compare_distance_tag>::type CD;
		typedef typename CD::result_type result_type;
		CD cd;
		Compare_distance_2(Kernel const&k):cd(k){}
		result_type operator()(Point_2 const&a, Point_2 const&b, Point_2 const&c) {
			return cd(a,b,c);
		}
		result_type operator()(Point_2 const&a, Point_2 const&b, Point_2 const&c, Point_2 const&d) {
			return cd(a,b,c,d);
		}
	};
	struct Orientation_2 {
		typedef typename Get_functor<Base, Orientation_of_points_tag>::type O;
		typedef typename O::result_type result_type;
		O o;
		Orientation_2(Kernel const&k):o(k){}
		result_type operator()(Point_2 const&a, Point_2 const&b, Point_2 const&c) {
			//return o(a,b,c);
			Point_2 const* t[3]={&a,&b,&c};
			return o(make_transforming_iterator<Dereference_functor>(t+0),make_transforming_iterator<Dereference_functor>(t+3));

		}
	};
	struct Side_of_oriented_sphere_2 {
		typedef typename Get_functor<Base, Side_of_oriented_sphere_tag>::type SOS;
		typedef typename SOS::result_type result_type;
		SOS sos;
		Side_of_oriented_sphere_2(Kernel const&k):sos(k){}
		result_type operator()(Point_2 const&a, Point_2 const&b, Point_2 const&c, Point_2 const&d) {
			//return sos(a,b,c,d);
			Point_2 const* t[4]={&a,&b,&c,&d};
			return sos(make_transforming_iterator<Dereference_functor>(t+0),make_transforming_iterator<Dereference_functor>(t+4));
		}
	};
	Less_x_2 less_x_2_object()const{ return Less_x_2(*this); }
	Less_y_2 less_y_2_object()const{ return Less_y_2(*this); }
	Compare_x_2 compare_x_2_object()const{ return Compare_x_2(*this); }
	Compare_y_2 compare_y_2_object()const{ return Compare_y_2(*this); }
	Compare_distance_2 compare_distance_2_object()const{ return Compare_distance_2(*this); }
	Orientation_2 orientation_2_object()const{ return Orientation_2(*this); }
	Side_of_oriented_sphere_2 side_of_oriented_sphere_2_object()const{ return Side_of_oriented_sphere_2(*this); }
};
}

#endif
