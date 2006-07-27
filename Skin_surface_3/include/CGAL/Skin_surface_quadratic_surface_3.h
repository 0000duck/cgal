// Copyright (c) 2005 Rijksuniversiteit Groningen (Netherlands)
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
// Author(s)     : Nico Kruithof <Nico@cs.rug.nl>

#ifndef CGAL_SKIN_SURFACE_QUADRATIC_SURFACE_H
#define CGAL_SKIN_SURFACE_QUADRATIC_SURFACE_H

CGAL_BEGIN_NAMESPACE

template < class SkinSurfaceQuadraticSurfaceTraits_3 >
class Skin_surface_quadratic_surface_3 {
public:
  typedef SkinSurfaceQuadraticSurfaceTraits_3 K;
  typedef typename K::RT                  RT;
  typedef typename K::Point_3             Point;
  typedef typename K::Vector_3            Vector;
  typedef typename K::Segment_3           Segment;

  Skin_surface_quadratic_surface_3(RT Qinput[], Point p, RT c)
    : p(p), c(c) 
  {
    for (int i=0; i<6; i++) Q[i] = Qinput[i];
  }

  template <class Input_point>
  RT value(Input_point const &x) const {
    typedef Cartesian_converter<typename Input_point::R, K> Converter;
    
    RT vx = Converter()(x.x()) - p.x();
    RT vy = Converter()(x.y()) - p.y();
    RT vz = Converter()(x.z()) - p.z();

    return 
      vx*(Q[0]*vx) +
      vy*(Q[1]*vx+Q[2]*vy) +
      vz*(Q[3]*vx+Q[4]*vy+Q[5]*vz) +
      c;
  }
  template <class Input_point>
  Sign sign(Input_point const &x) const {
    return CGAL_NTS sign(value(x));
  }


  template <class Input_point>
  typename Input_point::R::Vector_3 gradient(Input_point const &x) {
    // NGHK: We use the kernel trick again: DOCUMENT!!!!
    typedef Cartesian_converter<typename Input_point::R, K> Converter;
    typedef Cartesian_converter<K, typename Input_point::R> Inv_converter;
    Converter conv;
    Inv_converter inv_conv;
    Point xp = conv(x);
    return inv_conv(compute_gradient(xp));
  }

  /// Construct the intersection point with the segment (p0,p1)
  template <class Input_point>
  Input_point to_surface(const Input_point &p0, const Input_point &p1) {
    // NGHK: We use the kernel trick again: DOCUMENT!!!!
    typedef Cartesian_converter<typename Input_point::R, K> Converter;
    Converter conv;

    Input_point pp0 = p0, pp1 = p1, mid;
    double sq_d = to_double(squared_distance(pp0,pp1));
    
    if (value(conv(pp1)) < value(conv(pp0))) {
      std::swap(pp0, pp1);
    }
    
    while (sq_d > 1.e-10) {
      mid = midpoint(pp0,pp1);
      if (value(conv(mid)) > 0) {
	pp1 = mid;
      } else {
	pp0 = mid;
      }
      sq_d /= 4;
    }
    return midpoint(pp0,pp1);
  }
private:
  template <class Input_point>
  Vector compute_gradient(Input_point const &x) {
    typedef Cartesian_converter<typename Input_point::R, K> Converter;
    
    Vector v = Converter()(x) - p;

    return Vector(2*Q[0]*v.x() + Q[1]*v.y() + Q[3]*v.z(),
		  Q[1]*v.x() + 2*Q[2]*v.y() + Q[4]*v.z(),
		  Q[3]*v.x() + Q[4]*v.y() + 2*Q[5]*v.z());
  }
	
  RT Q[6]; 
  Point p; 
  RT c;
};

CGAL_END_NAMESPACE

#endif // CGAL_SKIN_SURFACE_QUADRATIC_SURFACE_H
