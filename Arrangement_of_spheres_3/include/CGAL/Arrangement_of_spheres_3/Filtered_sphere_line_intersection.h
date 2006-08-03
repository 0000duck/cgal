#ifndef FILTERED_SPHERE_LINE_INTERSECTION_H
#define FILTERED_SPHERE_LINE_INTERSECTION_H

#include <CGAL/Arrangement_of_spheres_3/Sphere_line_intersection.h>
#include <iostream>
//#include <CGAL/Arrangement_of_spheres_3/utilities.h>



//using doubles for ease of implementation, may not be the fastest
/*
  cases to handle:
  - tangent line -- fall back to algebraic since interval is small
  - tiny sphere--fall back to algebraic since interval is small
  - missing line
  - vertical line-- asserted
   
  I want to make sure that the interval is open. Now it is not. 
*/






//int num_exact_tests_=0;
//int num_tests_=0;


template <class K, int C>
struct Filtered_sphere_line_intersection: public Sphere_line_intersection<K> {
  enum Extended_comparison_result {UNKNOWN = -2, SMALLER= CGAL::SMALLER, 
				   EQUAL= CGAL::EQUAL, LARGER= CGAL::LARGER};
  typedef Sphere_line_intersection<K> P;
  typedef Filtered_sphere_line_intersection<K,C> This;
  
  //typedef Arrangement_of_spheres_traits_3 T;
  
  //typedef K::Point_3 Point_3;
  //typedef K::Vector_3 Vector_3;
  //typedef K::Line_3 Line_3;
  //typedef K::Sphere_3 Sphere_3;


  Filtered_sphere_line_intersection(): lb_(1), ub_(-1){
  }

  
  /*Sphere_line_intersection(NT n):  first_(false){
    initialize_const(n);
    }*/
  Filtered_sphere_line_intersection(typename K::Point_3 p3,
				    typename K::Line_3 l): 
    P(p3, l){
    std::pair<double,double> i= CGAL::to_interval(p3[C]);
    lb_=i.first;
    ub_=i.second;
  }


  // NOTE add a constructor where the line is known to point along C
  // this saves many computations for closest point

  /*Filtered_sphere_line_intersection(typename K::Point_3 p3, 
				    typename K::Line_3 l): P(p3, l){
    std::pair<double,double> i= CGAL::to_interval(p3[C]);
    lb_=i.first;
    ub_=i.second;
    }*/
  
  // assume the line hits
  /*Filtered_sphere_line_intersection( typename K::Sphere_3 s, 
				     typename K::Line_3 l, 
				     CGAL::Tag_true ): P(s,l, CGAL::Tag_true()){
    initialize();
    }*/
 
  Filtered_sphere_line_intersection(typename K::Sphere_3 s, 
				    typename K::Line_3 l): P(s,l){
    // hope the compiler collapses the duplicate work
    if (!P::is_valid()){
      lb_=std::numeric_limits<double>::infinity();
      ub_=lb_;
    } else {
      typename K::Point_3 cp= l.projection(s.center()); //K::closest_point(l, s.center());
      initialize(cp[C]);
    }
  }

  /*Filtered_sphere_line_intersection(typename K::Circle_2 s, 
				    typename K::Line_2 l): P(s,l){
    // hope the compiler collapses the duplicate work
    CGAL_precondition(C!= 2);
    if (!P::is_valid()){
      lb_=1;
      ub_=-1;
    } else {
      typename K::Point_2 cp= l.projection(s.center()); 
      initialize(cp[C]);
    }
    }*/

  /*Filtered_sphere_line_intersection( typename K::Circle_2 s, 
				     typename K::Line_2 l, 
				     CGAL::Tag_true ): P(s,l, CGAL::Tag_true()){
    initialize();
    }*/
  /*Filtered_sphere_line_intersection(typename K::Point_2 p3, 
				    typename K::Line_2 l): P(p3, l){
    CGAL_precondition(C==0 || C==1);
    std::pair<double,double> i= CGAL::to_interval(p3[C]);
    lb_=i.first;
    ub_=i.second;
    }*/

  Filtered_sphere_line_intersection(typename P::NT p): P(typename K::Point_3(p,p,p),
							 typename K::Line_3(typename K::Point_3(p,p,p), 
									    typename K::Vector_3(1,1,1))){
    std::pair<double,double> i= CGAL::to_interval(p);
    lb_=i.first;
    ub_=i.second;
  }

  //Sphere_line_intersection(double lb, double ub):lb_(lb), ub_(ub){}

  double lb() const {
    return lb_;
  }

  double ub() const {
    return ub_;
  }

  static Filtered_sphere_line_intersection infinity_rep() {
    typename K::Point_3 p(0,0,0);
    typename P::NT vs[]={0,0,0};
    vs[C]=1;
    typename P::Vector_3 v(vs[0], vs[1], vs[2]);
    typename P::Line_3 l(p,v);
    return Filtered_sphere_line_intersection(l, true);
  }

  bool operator<(const This &o) const {
    return compare_C(o) == CGAL::SMALLER;
  }

  bool operator>(const This &o) const {
    return compare_C(o) == CGAL::LARGER;
  }
  bool operator==(const This &o) const {
    return compare_C(o) == CGAL::EQUAL;
  }
  bool operator!=(const This &o) const {
    return compare_C(o) != CGAL::EQUAL;
  }
  bool operator>=(const This &o) const {
    return compare_C(o) != CGAL::SMALLER;
  }
  bool operator<=(const This &o) const {
    return compare_C(o) != CGAL::LARGER;
  }

  void initialize() {
    // fix this
    typename P::NT t= (P::line().to_vector()*(P::sphere().center()-CGAL::ORIGIN) 
		       - P::line().to_vector()*(P::line().point()-CGAL::ORIGIN))/(P::line().to_vector()*P::line().to_vector());
    typename P::NT r= P::line().point()[C]+t*P::line().to_vector()[C];
    initialize(r);
  }

  void initialize(typename P::NT cc) {
    //NT cc =closest_coord<C>(s_.center(), P::line());
    CGAL_precondition_code(typename P::NT t=(cc-P::line().point()[C])/P::line().to_vector()[C]);
    CGAL_precondition_code(typename P::Point_3 pol=P::line().point()+t*P::line().to_vector());
    CGAL_precondition(P::sphere().bounded_side(pol) != CGAL::ON_UNBOUNDED_SIDE);
    
    
    std::pair<double,double> i=CGAL::to_interval(cc);
    if (P::line().to_vector()[C]>0) {
      ub_=i.second;
      lb_=CGAL::to_interval(P::sphere().center()[C] - (std::max)(P::sphere().squared_radius(), typename P::NT(1))).first;
    } else {
      lb_=i.first;
      ub_=CGAL::to_interval(P::sphere().center()[C] + (std::max)(P::sphere().squared_radius(), typename P::NT(1))).second;
    }
  }

  /*void initialize_const(NT n) {
    std::pair<double, double> i= CGAL::to_interval(n);
    lb_=i.first;
    ub_=i.second;
    if (lb_ != ub_) {
    // initialize sphere and line
    NT v[3]={0,0,0};
    v[C]=1;
    l_=Line_3(Point_3(0,0,0),Vector_3(v[0], v[1], v[2]));
    NT r= n-NT(ub_);
    v[C]=n+r;
    s_=Sphere(Point_3(v[0], v[1], v[2]), r*r);
    }
    }*/

  std::ostream &write(std::ostream &out) const {
    //out << "(" << s_ << ", " << l_ << ", (" << lb_ << "..." << ub_ << "))";
    P::write(out) << " [" << lb_ << "..." << ub_ << "] = " << double_value();
    return out;
  }

  double double_value() const {
    if (ub_== lb_) return ub_;
    else if (ub_-lb_ < .00001) return .5*(lb_+ub_);
    else {
      double d=CGAL::to_double(P::exact_coordinate(typename P::Coordinate_index(C)));
      CGAL_assertion(d>=lb_ && d<= ub_);
      return d;
    }
  }

  std::pair<double, double> interval_value() const {
    return std::make_pair(lb_, ub_);
  }

  void refine(double bm) const {
    if (bm==lb_ || bm==ub_) return;
    CGAL_precondition( bm > lb_ && bm < ub_);
    if (P::line().to_vector()[C] == 0){
      if (typename P::NT(bm) < P::line().point()[C]){
	lb_=bm;
      } else {
	ub_=bm;
      }
    } else {
      typename P::NT t=(typename P::NT(bm)-P::line().point()[C])
	/P::line().to_vector()[C];
      typename K::Point_3 p=  P::line().point()+t*P::line().to_vector();

      //= point_on_line<K, C>(P::line(), typename P::NT(bm));
      CGAL::Bounded_side bs= P::sphere().bounded_side(p);
      switch(bs) {
      case CGAL::ON_BOUNDED_SIDE: if (P::line().to_vector()[C] > 0) ub_=bm; else lb_=bm; break;
      case CGAL::ON_UNBOUNDED_SIDE: if (P::line().to_vector()[C] > 0) lb_=bm; else ub_=bm; break;
      case CGAL::ON_BOUNDARY: lb_=ub_=bm; break;
      };
    }
    {
      CGAL_assertion(P::exact_coordinate(typename P::Coordinate_index(C)) 
		     >= typename P::Quadratic_NT(typename P::NT(lb_)));
      CGAL_assertion(P::exact_coordinate(typename P::Coordinate_index(C)) 
		     <= typename P::Quadratic_NT(typename P::NT(ub_)));
    }
  }

  

  void refine() const {
    refine( .5*(lb_+ub_));
  }

  This operator-() const {
    CGAL_assertion(0);
    return This();
  }

  static void test();

  
  typename P::NT coord_on_line(typename P::NT v,
			       typename P::Coordinate_index CC) const {
    typename P::NT t=(v-P::line().point()[C])/P::line().to_vector()[C];
    return  P::line().point()[CC]+t*P::line().to_vector()[CC];
  }

  /*void assert_is_valid() {
    if (lb_ != ub_ && .5*(lb_+ub_) != lb_ && .5*(lb_+ub_) != ub_) {
    if (first_) {
    CGAL_assertion(bounded_side_of_sphere(s_, point_on_line<C>(NT(ub_)))!= CGAL::ON_UNBOUNDED_SIDE);
    CGAL_assertion(bounded_side_of_sphere(s_, point_on_line<C>(NT(lb_)))!= CGAL::ON_BOUNDED_SIDE);
    } else {
    CGAL_assertion(bounded_side_of_sphere(s_, point_on_line<C>(NT(ub_)))!= CGAL::ON_BOUNDED_SIDE);
    CGAL_assertion(bounded_side_of_sphere(s_, point_on_line<C>(NT(lb_)))!= CGAL::ON_UNBOUNDED_SIDE);
    }
    }
    }*/

  void swap(This &o) {
    std::swap(lb_, o.lb_);
    std::swap(ub_, o.ub_);
    P::swap(o);
  }


  static Extended_comparison_result compare_bounds(const This &a,
						   const This &b,
						   typename P::Coordinate_index CC){
    //typedef Filtered_sphere_line_intersection<K,C> FS;
    if (a.line().to_vector()[CC] == 0 || b.line().to_vector()[CC]== 0){
      // just punt and fall back on exact for now. 
      return UNKNOWN;
    } else {
      typename P::NT tl= a.coord_on_line(typename P::NT(a.lb()), CC);
      typename P::NT tu= a.coord_on_line(typename P::NT(a.ub()), CC);
      typename P::NT ol= b.coord_on_line(typename P::NT(b.lb()), CC);
      typename P::NT ou= b.coord_on_line(typename P::NT(b.ub()), CC);
      if (tl > tu) std::swap(tl,tu);
      if (ol > ou) std::swap(ol,ou);
      if (tu < ol) {
	CGAL_assertion(Coordinate_index(C) != CC 
		       || a.double_value() < b.double_value());
	return SMALLER;
      } else if (ou < tl) {
	CGAL_assertion(Coordinate_index(C) != CC 
		       || a.double_value() > b.double_value());
	return LARGER;
      }
      else if (tl==tu && ol==ou && tl==ol) return EQUAL;
      else return UNKNOWN; 
    }
  }


  static Extended_comparison_result compare_bounds_C(const This  &a,
						     const This &b){
    if (a.ub() < b.lb()) {
      CGAL_assertion(a.double_value() < b.double_value());
      return SMALLER;
    }
    else if (a.lb() > b.ub()){
      CGAL_assertion(a.double_value() > b.double_value());
      return LARGER;
    } else if (a.lb()== b.lb() && a.lb()==a.ub() && b.lb() == b.ub()) return EQUAL;
    else return UNKNOWN;
  }
  

  inline CGAL::Comparison_result compare_2(const This &o,
					   typename P::Coordinate_index CC, 
					   int sct=0) const {
    Extended_comparison_result ecr= compare_bounds(*this, o, CC);
    if (ecr != UNKNOWN) return CGAL::enum_cast<CGAL::Comparison_result>(ecr); 
    else {
      if (sct < 5) {
	refine();
	o.refine();
	return compare_2(o, CC, sct+1);
      } else {
	//++num_exact_tests_;
	return P::compare(o, CC);
      }
    }
  }


  inline CGAL::Comparison_result compare_C2(const This &o, int sct=0) const {
    CGAL_precondition(P::is_valid() && o.is_valid());
    Extended_comparison_result ecr= compare_bounds_C(*this, o);
    if (ecr != UNKNOWN) return CGAL::enum_cast<CGAL::Comparison_result>(ecr); 
    else {
      if (sct < 5) {
	refine();
	o.refine();
	return compare_C2(o, sct+1);
      } else {
	//++num_exact_tests_;
	return static_cast<const P*>(this)->compare(o, typename P::Coordinate_index(C));
      }
    }
  }
  inline CGAL::Comparison_result  compare(const This &o, 
					  typename P::Coordinate_index CC) const {
    //++num_tests_;
    // see if intervals overlap
    Extended_comparison_result ecr= compare_bounds(*this, o, CC);
    if (ecr != UNKNOWN) return CGAL::enum_cast<CGAL::Comparison_result>(ecr);
    // question of <= vs <
    // if so cut at shared part
    // not useful if we are comparing at another coordinate than C, but not really harmful either.
    else if (typename P::Coordinate_index(C)==CC && lb_ <= o.lb_) {
      if (ub_ != o.lb_) refine(o.lb_);
      if (ub_ < o.ub_) o.refine(ub_);
    } else if (typename P::Coordinate_index(C)==CC && o.lb_ <= lb_) {
      if (o.ub_ != lb_) o.refine(lb_);
      if (o.ub_ < ub_) refine(o.ub_);
    }
    return compare_2(o, CC);
    // if still, subdivide a few times
    // finally use algebraic numbers
  }

  
  inline CGAL::Comparison_result  compare_C(const This &o) const {
    //++num_tests_;
    // see if intervals overlap
    Extended_comparison_result ecr= compare_bounds_C(*this, o);
    if (ecr != UNKNOWN) return CGAL::enum_cast<CGAL::Comparison_result>(ecr);
    else if (lb_ <= o.lb_) {
      if (ub_ != o.lb_) refine(o.lb_);
      if (ub_ < o.ub_) o.refine(ub_);
    } else if (o.lb_ <= lb_) {
      if (o.ub_ != lb_) o.refine(lb_);
      if (o.ub_ < ub_) refine(o.ub_);
    }
    return compare_C2(o);
  }


  mutable double lb_, ub_;
};

template <class K, int C>
inline double to_double(const Filtered_sphere_line_intersection<K,C> &r) {
  return r.double_value();
}
template <class K, int C>
inline double to_interval(const Filtered_sphere_line_intersection<K,C> &r) {
  return r.interval_value();
}

template <class K, int C>
inline std::ostream &operator<<(std::ostream &out, const Filtered_sphere_line_intersection<K,C> &r){
  return r.write(out);
}

namespace std{
  template <class K, int C>
  class numeric_limits<Filtered_sphere_line_intersection<K, C> >: public numeric_limits<double>
  {
  public:
    typedef numeric_limits<double> P;
    typedef Filtered_sphere_line_intersection<K,C> T;
    static const bool is_specialized = true;
    static T min BOOST_PREVENT_MACRO_SUBSTITUTION () throw() {return T((P::min)());}
    static T max BOOST_PREVENT_MACRO_SUBSTITUTION () throw() {return T((P::max)());}
    static const bool has_infinity=true;
    static T infinity() throw() {return T::infinity_rep();}
  };

  template <class K, int C>
  inline void swap(Filtered_sphere_line_intersection<K,C>  &a, Filtered_sphere_line_intersection<K,C>  &b){
    a.swap(b);
  }
}

namespace CGAL{
  template <class K, int C>
  inline double to_double(const Filtered_sphere_line_intersection<K,C> &r) {
    return r.double_value();
  }
  template <class K, int C>
  inline std::pair<double,double> to_interval(const Filtered_sphere_line_intersection<K,C> &r) {
    return r.interval_value();
  }
};


/*template <int C>
  void Filtered_sphere_line_intersection<C>::test() {
  typedef Filtered_sphere_line_intersection<C> T;
  T inf= infinity_rep();
  T ninf= -inf;
  //std::cout << "Inf " << CGAL::to_double(inf) << " " << CGAL::to_double(ninf) << std::endl;
  T zero(Point_3(0,0,0));
  //std::cout << "Zero " << CGAL::to_double(zero) << " " << CGAL::to_double(-zero) << std::endl;
  T one(Point_3(1,1,1));
  //std::cout << "One " << CGAL::to_double(one) << " " << CGAL::to_double(-one) << std::endl;
  CGAL_assertion(ninf < inf);
  CGAL_assertion(inf > ninf);
  CGAL_assertion(zero > ninf);
  CGAL_assertion(zero < inf);
  CGAL_assertion(one > zero);
  CGAL_assertion(-one < zero);
  NT ant("100000000000000000000000000000000000000000000000000000/100000000000000000000000000000000000000000000000000001");
  T l(Point_3(ant, ant, ant));
  CGAL_assertion(l < one);
  CGAL_assertion(-l > -one);
  CGAL_assertion(l < inf);
  CGAL_assertion(l > zero);
  T root2inv(Sphere(Point_3(0,0,0), 1), Line_3(Point_3(1,1,0), Vector_3(1,1,0)), false);
  //std::cout << "Root2Inv " << CGAL::to_double(root2inv) << " " << CGAL::to_double(-root2inv) << std::endl;
  CGAL_assertion(root2inv > zero);
  CGAL_assertion(root2inv < one);
  CGAL_assertion(-root2inv > -one);
  CGAL_assertion(-root2inv < zero);
  }*/



#endif
