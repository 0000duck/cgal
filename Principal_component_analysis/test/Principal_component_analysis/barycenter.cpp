// Test program for the barycenter() and centroid() functions.
// Sylvain Pion.

#include <vector>
#include <CGAL/Testsuite/assert.h>

#include <CGAL/Cartesian.h>

#include <CGAL/barycenter.h>
#include <CGAL/centroid.h>

typedef double               FT;
typedef CGAL::Cartesian<FT>  K;
typedef K::Point_2           Point_2;
typedef K::Point_3           Point_3;

void test_2()
{
  std::vector<std::pair<Point_2, FT> > wpts;

  Point_2 p0 (1, 2);
  wpts.push_back(std::make_pair(p0, 2.));

  CGAL_test_assert( CGAL::barycenter(wpts.begin(), wpts.end(), K()) == p0 );

  wpts.push_back(std::make_pair(p0, 3.));

  CGAL_test_assert( CGAL::barycenter(wpts.begin(), wpts.end()) == p0 );

  Point_2 p1 (2, 1);
  wpts.push_back(std::make_pair(p1, 5.));

  CGAL_test_assert( CGAL::barycenter(wpts.begin(), wpts.end())
          == CGAL::midpoint(p0, p1) );

  // Test the version with separate Point/weight ranges.
  std::vector<Point_2> pts;
  pts.push_back(p0);
  pts.push_back(p0);
  pts.push_back(p1);
  std::vector<FT> weights;
  weights.push_back(2);
  weights.push_back(3);
  weights.push_back(5);

  CGAL_test_assert( CGAL::barycenter(pts.begin(), pts.end(), weights.begin(), 0)
          == CGAL::midpoint(p0, p1) );

  CGAL_test_assert( CGAL::barycenter(pts.begin(), pts.end(), weights.begin(), K())
          == CGAL::midpoint(p0, p1) );

  CGAL_test_assert( CGAL::centroid(pts.begin(), pts.begin()+1, K(),CGAL::PCA_dimension_0_tag()) == p0);
  CGAL_test_assert( CGAL::centroid(pts.begin(), pts.begin()+2,CGAL::PCA_dimension_0_tag()) == p0);
  CGAL_test_assert( CGAL::centroid(pts.begin()+1, pts.begin()+3,CGAL::PCA_dimension_0_tag())
          == CGAL::midpoint(p0, p1) );
}

void test_3()
{
  std::vector<std::pair<Point_3, FT> > wpts;

  Point_3 p0 (1, 2, 3);
  wpts.push_back(std::make_pair(p0, 2.));

  CGAL_test_assert( CGAL::barycenter(wpts.begin(), wpts.end(), K()) == p0 );

  wpts.push_back(std::make_pair(p0, 3.));

  CGAL_test_assert( CGAL::barycenter(wpts.begin(), wpts.end()) == p0 );

  Point_3 p1 (3, 2, 1);
  wpts.push_back(std::make_pair(p1, 5.));

  CGAL_test_assert( CGAL::barycenter(wpts.begin(), wpts.end())
           == CGAL::midpoint(p0, p1) );

  // Test the version with separate Point/weight ranges.
  std::vector<Point_3> pts;
  pts.push_back(p0);
  pts.push_back(p0);
  pts.push_back(p1);
  std::vector<FT> weights;
  weights.push_back(2);
  weights.push_back(3);
  weights.push_back(5);

  CGAL_test_assert( CGAL::barycenter(pts.begin(), pts.end(), weights.begin(), 0)
          == CGAL::midpoint(p0, p1) );

  CGAL_test_assert( CGAL::barycenter(pts.begin(), pts.end(), weights.begin(), K())
          == CGAL::midpoint(p0, p1) );

  CGAL_test_assert( CGAL::centroid(pts.begin(), pts.begin()+1, K(),CGAL::PCA_dimension_0_tag()) == p0);
  CGAL_test_assert( CGAL::centroid(pts.begin(), pts.begin()+2,CGAL::PCA_dimension_0_tag()) == p0);
  CGAL_test_assert( CGAL::centroid(pts.begin()+1, pts.begin()+3,CGAL::PCA_dimension_0_tag())
          == CGAL::midpoint(p0, p1) );
}


int main()
{
  test_2();
  test_3();
  return 0;
}
