#include <CGAL/Simple_cartesian.h>
#include <CGAL/intersections.h>
#include <CGAL/iterator.h>
#include <CGAL/point_generators_2.h>

#include <boost/bind.hpp>

using namespace CGAL;

typedef CGAL::Simple_cartesian<double> K;
typedef K::Point_2                     Point;
typedef K::Segment_2                   Segment;

typedef CGAL::Creator_uniform_2<double,Point>        Pt_creator;
typedef Random_points_on_segment_2<Point,Pt_creator> P1;
typedef Random_points_on_circle_2<Point,Pt_creator>  P2;
typedef Creator_uniform_2< Point, Segment>           Seg_creator;
typedef Join_input_iterator_2< P1, P2, Seg_creator>  Seg_iterator;

//This is weird
namespace boost{
  namespace _bi{
  
template<>
struct result_traits<unspecified, K::Intersect_2>
{
    typedef typename  CGAL::cpp11::result_of<K::Intersect_2(Segment,Segment)>::type type;
};

} }

int main()
{
  std::vector<Segment> input;

  // Prepare point generator for the horizontal segment, length 200.
  P1 p1( Point(-100,0), Point(100,0));

  // Prepare point generator for random points on circle, radius 250.
  P2 p2( 250);
  
  // Create segments.
  Seg_iterator g( p1, p2);
  CGAL::copy_n( g, 200, std::back_inserter(input));
  

  // splitting results with Dispatch_output_iterator
  std::vector<Point> points;
  std::vector<Segment> segments;

  typedef Dispatch_output_iterator<
    cpp0x::tuple<Point,Segment>, cpp0x::tuple< std::back_insert_iterator<std::vector<Point> >,
                                               std::back_insert_iterator<std::vector<Segment> > > >
    Dispatcher;
  
  Dispatcher disp = dispatch_output<Point,Segment>( std::back_inserter(points), 
                                                    std::back_inserter(segments) );
  
  // intersections of many objects, directly dispatched
  // intersects the first segment of input with all other segments
  // The resulting points or segments are written in the vectors with the same names

  K::Intersect_2 intersector = K().intersect_2_object();
  std::transform(input.begin(), input.end(), disp,
                 boost::bind(intersector, input.front(), _1));


  std::cout << "Point intersections: " << points.size() << std::endl;
  std::cout << "Segment intersections: " << segments.size() << std::endl;


  return 0;
}
