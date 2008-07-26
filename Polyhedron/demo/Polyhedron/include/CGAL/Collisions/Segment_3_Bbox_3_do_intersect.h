#ifndef CGAL_SEGMENT_3_BBOX_3_DO_INTERSECT_H
#define CGAL_SEGMENT_3_BBOX_3_DO_INTERSECT_H

// inspired from http://cag.csail.mit.edu/~amy/papers/box-jgt.pdf

CGAL_BEGIN_NAMESPACE

namespace CGALi {

  template <class K>
  bool do_intersect(const typename K::Segment_3& segment, 
    const CGAL::Bbox_3& bbox,
    const K& kernel)
  {
    typedef typename K::FT FT;
    typedef typename K::Point_3 Point;
    typedef typename K::Vector_3 Vector;
    typedef typename K::Segment_3 Segment;

    //tried checking whether point is inside the box, but that significantly reduces performance
    //bbox-bbox(segment) without precomputing also reduces performance

    Point parameters[2];
    parameters[0] = Point(bbox.xmin(), bbox.ymin(), bbox.zmin());
    parameters[1] = Point(bbox.xmax(), bbox.ymax(), bbox.zmax());

    const Point source = segment.source();
    const Point target = segment.target();

    const Vector direction = target - source;
    // CAREFUL, when 1.0/0.0, this works only with doubles, not with filtered kernels
    const Vector inv_direction(1.0/direction.x(),
      1.0/direction.y(),
      1.0/direction.z()); 
    const int sign_x = inv_direction.x() < (FT)0.0;
    const int sign_y = inv_direction.y() < (FT)0.0;
    const int sign_z = inv_direction.z() < (FT)0.0;

    FT tmin = (parameters[sign_x].x() - source.x()) * inv_direction.x();
    FT tmax = (parameters[1-sign_x].x() - source.x()) * inv_direction.x();

    //faster exit - thanks to camille
    if(tmax < (FT)0.0 || tmin > (FT)1.0)
      return false;
    if(tmin < (FT)0.0)
      tmin = (FT)0.0;
    if(tmax > (FT)1.0)
      tmax = (FT)1.0;

    const FT tymin = (parameters[sign_y].y() - source.y()) * inv_direction.y();
    const FT tymax = (parameters[1-sign_y].y() - source.y()) * inv_direction.y();

    if(tmin > tymax || tymin > tmax) 
      return false;

    if(tymin > tmin)
      tmin = tymin;

    if(tymax < tmax)
      tmax = tymax;

    FT tzmin = (parameters[sign_z].z() - source.z()) * inv_direction.z();
    FT tzmax = (parameters[1-sign_z].z() - source.z()) * inv_direction.z();

    if(tmin > tzmax || tzmin > tmax) 
      return false;

    return true;
  }

} // namespace CGALi

template <class K>
bool do_intersect(const CGAL::Segment_3<K>& segment, 
		  const CGAL::Bbox_3& bbox)
{
  return typename K::Do_intersect_3()(segment, bbox);
}

template <class K>
bool do_intersect(const CGAL::Bbox_3& bbox, 
		  const CGAL::Segment_3<K>& segment)
{
  return typename K::Do_intersect_3()(segment, bbox);
}


CGAL_END_NAMESPACE

#endif  // CGAL_SEGMENT_3_BBOX_3_DO_INTERSECT_H
