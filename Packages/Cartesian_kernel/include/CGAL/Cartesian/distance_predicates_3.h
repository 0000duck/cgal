// ======================================================================
//
// Copyright (c) 2000 The CGAL Consortium
//
// This software and related documentation is part of an INTERNAL release
// of the Computational Geometry Algorithms Library (CGAL). It is not
// intended for general use.
//
// ----------------------------------------------------------------------
//
// release       :
// release_date  :
//
// file          : include/CGAL/Cartesian/distance_predicates_3.h
// revision      : $Revision$
// revision_date : $Date$
// author(s)     : Herve Bronnimann
// coordinator   : INRIA Sophia-Antipolis (Mariette.Yvinec@sophia.inria.fr)
//
// ======================================================================

#ifndef CGAL_CARTESIAN_DISTANCE_PREDICATES_3_H
#define CGAL_CARTESIAN_DISTANCE_PREDICATES_3_H

#include <CGAL/Cartesian/Point_3.h>
#include <CGAL/Cartesian/Plane_3.h>
#include <CGAL/predicates/kernel_ftC3.h>

CGAL_BEGIN_NAMESPACE

template < class K >
inline
Comparison_result
compare_distance_to_point(const PointC3<K> &p,
                          const PointC3<K> &q,
                          const PointC3<K> &r)
{
  return K().compare_distance_3_object()(p, q, r);
}

template < class K >
CGAL_KERNEL_LARGE_INLINE
bool
has_larger_distance_to_point(const PointC3<K> &p,
                             const PointC3<K> &q,
                             const PointC3<K> &r)
{
  return has_larger_dist_to_pointC3(p.x(), p.y(), p.z(),
                                    q.x(), q.y(), q.z(),
                                    r.x(), r.y(), r.z());
}

template < class K >
inline
bool
has_smaller_distance_to_point(const PointC3<K> &p,
                              const PointC3<K> &q,
                              const PointC3<K> &r)
{
  return K().less_distance_to_point_3_object()(p, q, r);
}

template < class K >
CGAL_KERNEL_LARGE_INLINE
Comparison_result
compare_signed_distance_to_plane(const PlaneC3<K> &h,
                                 const PointC3<K> &p,
                                 const PointC3<K> &q)
{ // FIXME : probably not compiled by the test-suite.
  return cmp_signed_dist_to_directionC3(h.a(), h.b(), h.c(),
                                        p.x(), p.y(), p.z(),
                                        q.x(), q.y(), q.z());
}

template < class K >
CGAL_KERNEL_LARGE_INLINE
bool
has_larger_signed_distance_to_plane(const PlaneC3<K> &h,
                                    const PointC3<K> &p,
                                    const PointC3<K> &q)
{ // FIXME : probably not compiled by the test-suite.
  return has_larger_signed_dist_to_directionC3(h.a(), h.b(), h.c(),
                                               p.x(), p.y(), p.z(),
                                               q.x(), q.y(), q.z());
}

template < class K >
inline
bool
has_smaller_signed_distance_to_plane(const PlaneC3<K> &h,
                                     const PointC3<K> &p,
                                     const PointC3<K> &q)
{ 
  return K().less_signed_distance_to_plane_3_object()(h, p, q);
}

template < class K >
CGAL_KERNEL_LARGE_INLINE
Comparison_result
compare_signed_distance_to_plane(const PointC3<K> &hp,
                                 const PointC3<K> &hq,
                                 const PointC3<K> &hr,
                                 const PointC3<K> &p,
                                 const PointC3<K> &q)
{ // FIXME : probably not compiled by the test-suite.
  return cmp_signed_dist_to_planeC3(hp.x(), hp.y(), hp.z(),
                                    hq.x(), hq.y(), hq.z(),
                                    hr.x(), hr.y(), hr.z(),
                                    p.x(),  p.y(),  p.z(),
                                    q.x(),  q.y(),  q.z());
}

template < class K >
CGAL_KERNEL_LARGE_INLINE
bool
has_larger_signed_distance_to_plane(const PointC3<K> &hp,
                                    const PointC3<K> &hq,
                                    const PointC3<K> &hr,
                                    const PointC3<K> &p,
                                    const PointC3<K> &q)
{ // FIXME : probably not compiled by the test-suite.
  return has_larger_signed_dist_to_planeC3(hp.x(), hp.y(), hp.z(),
                                           hq.x(), hq.y(), hq.z(),
                                           hr.x(), hr.y(), hr.z(),
                                           p.x(),  p.y(),  p.z(),
                                           q.x(),  q.y(),  q.z());
}

template < class K >
CGAL_KERNEL_LARGE_INLINE
bool
has_smaller_signed_distance_to_plane(const PointC3<K> &hp,
                                     const PointC3<K> &hq,
                                     const PointC3<K> &hr,
                                     const PointC3<K> &p,
                                     const PointC3<K> &q)
{ // FIXME : probably not compiled by the test-suite.
  return has_smaller_signed_dist_to_planeC3(hp.x(), hp.y(), hp.z(),
                                            hq.x(), hq.y(), hq.z(),
                                            hr.x(), hr.y(), hr.z(),
                                            p.x(),  p.y(),  p.z(),
                                            q.x(),  q.y(),  q.z());
}

#ifndef CGAL_NO_DEPRECATED_CODE
template < class K >
CGAL_KERNEL_LARGE_INLINE
Comparison_result
cmp_dist_to_point(const PointC3<K> &p,
                  const PointC3<K> &q,
                  const PointC3<K> &r)
{
  return cmp_dist_to_pointC3(p.x(), p.y(), p.z(),
                             q.x(), q.y(), q.z(),
                             r.x(), r.y(), r.z());
}

template < class K >
CGAL_KERNEL_LARGE_INLINE
bool
has_larger_dist_to_point(const PointC3<K> &p,
                         const PointC3<K> &q,
                         const PointC3<K> &r)
{
  return has_larger_dist_to_pointC3(p.x(), p.y(), p.z(),
                                    q.x(), q.y(), q.z(),
                                    r.x(), r.y(), r.z());
}

template < class K >
CGAL_KERNEL_LARGE_INLINE
bool
has_smaller_dist_to_point(const PointC3<K> &p,
                          const PointC3<K> &q,
                          const PointC3<K> &r)
{
  return has_smaller_dist_to_pointC3(p.x(), p.y(), p.z(),
                                     q.x(), q.y(), q.z(),
                                     r.x(), r.y(), r.z());
}

template < class K >
CGAL_KERNEL_LARGE_INLINE
Comparison_result
cmp_signed_dist_to_plane(const PlaneC3<K> &h,
                         const PointC3<K> &p,
                         const PointC3<K> &q)
{ // FIXME : probably not compiled by the test-suite.
  return cmp_signed_dist_to_directionC3(h.a(), h.b(), h.c(),
                                        p.x(), p.y(), p.z(),
                                        q.x(), q.y(), q.z());
}

template < class K >
CGAL_KERNEL_LARGE_INLINE
bool
has_larger_signed_dist_to_plane(const PlaneC3<K> &h,
                                const PointC3<K> &p,
                                const PointC3<K> &q)
{ // FIXME : probably not compiled by the test-suite.
  return has_larger_signed_dist_to_directionC3(h.a(), h.b(), h.c(),
                                               p.x(), p.y(), p.z(),
                                               q.x(), q.y(), q.z());
}

template < class K >
CGAL_KERNEL_LARGE_INLINE
bool
has_smaller_signed_dist_to_plane(const PlaneC3<K> &h,
                                 const PointC3<K> &p,
                                 const PointC3<K> &q)
{ // FIXME : probably not compiled by the test-suite.
  return has_smaller_signed_dist_to_directionC3(h.a(), h.b(), h.c(),
                                                p.x(), p.y(), p.z(),
                                                q.x(), q.y(), q.z());
}

template < class K >
CGAL_KERNEL_LARGE_INLINE
Comparison_result
cmp_signed_dist_to_plane(const PointC3<K> &hp,
                         const PointC3<K> &hq,
                         const PointC3<K> &hr,
                         const PointC3<K> &p,
                         const PointC3<K> &q)
{ // FIXME : probably not compiled by the test-suite.
  return cmp_signed_dist_to_planeC3(hp.x(), hp.y(), hp.z(),
                                    hq.x(), hq.y(), hq.z(),
                                    hr.x(), hr.y(), hr.z(),
                                    p.x(),  p.y(),  p.z(),
                                    q.x(),  q.y(),  q.z());
}

template < class K >
CGAL_KERNEL_LARGE_INLINE
bool
has_larger_signed_dist_to_plane(const PointC3<K> &hp,
                                const PointC3<K> &hq,
                                const PointC3<K> &hr,
                                const PointC3<K> &p,
                                const PointC3<K> &q)
{ // FIXME : probably not compiled by the test-suite.
  return has_larger_signed_dist_to_planeC3(hp.x(), hp.y(), hp.z(),
                                           hq.x(), hq.y(), hq.z(),
                                           hr.x(), hr.y(), hr.z(),
                                           p.x(),  p.y(),  p.z(),
                                           q.x(),  q.y(),  q.z());
}

template < class K >
CGAL_KERNEL_LARGE_INLINE
bool
has_smaller_signed_dist_to_plane(const PointC3<K> &hp,
                                 const PointC3<K> &hq,
                                 const PointC3<K> &hr,
                                 const PointC3<K> &p,
                                 const PointC3<K> &q)
{ // FIXME : probably not compiled by the test-suite.
  return has_smaller_signed_dist_to_planeC3(hp.x(), hp.y(), hp.z(),
                                            hq.x(), hq.y(), hq.z(),
                                            hr.x(), hr.y(), hr.z(),
                                            p.x(),  p.y(),  p.z(),
                                            q.x(),  q.y(),  q.z());
}

#endif

CGAL_END_NAMESPACE

#endif // CGAL_CARTESIAN_DISTANCE_PREDICATES_3_H
