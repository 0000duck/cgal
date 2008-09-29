// Copyright (c) 2007  INRIA (France).
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
// Author(s)     : Laurent Saboret, Pierre Alliez

#ifndef GYROVIZ_POINT_3_H
#define GYROVIZ_POINT_3_H

#include <CGAL/Point_with_normal_3.h>
#include <CGAL/Iterator_project.h>

#include <set>
#include <algorithm>


/// The Gyroviz_point_3 class represents a 3D point with:
/// - a position,
/// - a normal (oriented or not),
/// - a list of camera/2D point pairs used to reconstruct the point 
/// from an image sequence.
///
/// @heading Is Model for the Concepts: 
/// Model of the PointWithNormal_3 concept.
///
/// @heading Parameters:
/// @param Gt   Kernel's geometric traits.

template<class Gt>
class Gyroviz_point_3 : public CGAL::Point_with_normal_3<Gt>
{
// Private types
private:

    // Base class
    typedef CGAL::Point_with_normal_3<Gt> Base;

    // Auxiliary class to build a camera iterator
    template <class Node> // Node is Camera_point2_pair
    struct Project_camera {
      typedef Node                  argument_type;
      typedef typename Gt::Point_3  Point_3;
      typedef Point_3               result_type;
      Point_3&       operator()(Node& x)       const { return x.first; }
      const Point_3& operator()(const Node& x) const { return x.first; }
    };

// Public types
public:

    // Base class
    typedef CGAL::Point_with_normal_3<Gt> Point_with_normal; ///< Model of the PointWithNormal_3 concept.

    // Repeat Point_with_normal_3 public types
    typedef Gt Geom_traits; ///< Kernel's geometric traits.
    typedef typename Geom_traits::FT FT;
    typedef typename Geom_traits::Point_2 Point_2; ///< Kernel's Point_2 class.
    typedef typename Geom_traits::Point_3 Point_3; ///< Kernel's Point_3 class.
    typedef typename Point_with_normal::Normal Normal; ///< Model of OrientedNormal_3 concept.

    /// Camera/2D point pair. The 2D point is the 3D point (*this) projection's
    /// in the camera's image plane.
    typedef std::map <Point_3, Point_2> Camera_point2_map;
    typedef std::pair<Point_3, Point_2> Camera_point2_pair;

    /// Iterator over camera/2D point pairs.
    typedef typename Camera_point2_map::const_iterator 
                                        Camera_point2_pair_const_iterator;

    /// Iterator over cameras.
    typedef CGAL::Iterator_project<Camera_point2_pair_const_iterator, 
                                   Project_camera<std::pair<const Point_3,Point_2> > > // warning: const is required
                                        Camera_const_iterator;      

// Public methods
public:

    /// Point is (0,0,0) by default.
    /// Normal is (0,0,0) by default.
    /// Normal is oriented by default.
    /// Camera list is empty by default.
    Gyroviz_point_3(const CGAL::Origin& o = CGAL::ORIGIN)
    : Base(o)
    {
      m_is_selected = false;
    }
    Gyroviz_point_3(FT x, FT y, FT z)
    : Base(x,y,z)
    {
      m_is_selected = false;
    }
    Gyroviz_point_3(const Point_3& point,
                    const Normal& normal = CGAL::NULL_VECTOR)
    : Base(point, normal)
    {
      m_is_selected = false;
    }
    template<class K>
    Gyroviz_point_3(const Point_with_normal_3<K>& pwn)
    : Base(pwn)
    {
      m_is_selected = false;
    }
    template < class InputIterator >
    Gyroviz_point_3(const Point_3& point,
                    InputIterator first_camera_point2_pair, 
                    InputIterator beyond_camera_point2_pair)
    : Base(point, CGAL::NULL_VECTOR)
    {
      m_is_selected = false;
      camera_point2_map.insert(first_camera_point2_pair, beyond_camera_point2_pair); 
    }
    template < class InputIterator >
    Gyroviz_point_3(const Point_3& point,
                    const Normal& normal,
                    InputIterator first_camera_point2_pair, 
                    InputIterator beyond_camera_point2_pair)
    : Base(point, normal)
    {
      m_is_selected = false;
      camera_point2_map.insert(first_camera_point2_pair, beyond_camera_point2_pair); 
    }

    /// Copy constructor
    Gyroviz_point_3(const Gyroviz_point_3& gpt)
    : Base(gpt)
    {
      m_is_selected = gpt.m_is_selected;
      camera_point2_map = gpt.camera_point2_map;
    }
    template<class K>
    Gyroviz_point_3(const Gyroviz_point_3<K>& gpt)
    : Base(gpt)
    {
      m_is_selected = gpt.is_selected();
      camera_point2_map.insert(gpt.camera_point2_pairs_begin(), gpt.camera_point2_pairs_end()); 
    }
    /// Operator =()
    Gyroviz_point_3& operator=(const Gyroviz_point_3& gpt)
    {
      Base::operator=(gpt);
      m_is_selected = gpt.m_is_selected;
      camera_point2_map = gpt.camera_point2_map;
      return *this;
    }

    /// Merge points, including lists of camera/2D point pairs.
    void merge(const Gyroviz_point_3& that)
    { 
      // we assume that both points 3D position is the same
      
      // merge camera/2D point maps
      camera_point2_map.insert(that.camera_point2_pairs_begin(), that.camera_point2_pairs_end()); 
    }

    /// Compare positions.
    bool operator==(const Gyroviz_point_3& that)
    { 
      return Base::operator==(that); 
    }
    bool operator!=(const Gyroviz_point_3& that)
    { 
      return ! (*this == that); 
    }

    // Selection
    bool is_selected() const { return m_is_selected; }
    void select(bool is_selected=true) { m_is_selected = is_selected; }

    /// Get camera/2D point pairs.
    Camera_point2_pair_const_iterator camera_point2_pairs_begin() const 
    { 
      return camera_point2_map.begin(); 
    }
    Camera_point2_pair_const_iterator camera_point2_pairs_end  () const 
    { 
      return camera_point2_map.end(); 
    }

    /// Set camera/2D point pairs.
    template < class InputIterator >
    void set_camera_point2_pairs(InputIterator first_camera_point2_pair, 
                                 InputIterator beyond_camera_point2_pair)
    {
      camera_point2_map.clear();
      camera_point2_map.insert(first_camera_point2_pair, beyond_camera_point2_pair); 
    }

    /// Add camera/2D point pairs.
    void add_camera_point2_pair(Camera_point2_pair camera_and_point2)
    {
      camera_point2_map.insert(camera_and_point2); 
    }
    template < class InputIterator >
    void add_camera_point2_pairs(InputIterator first_camera_point2_pair, 
                                 InputIterator beyond_camera_point2_pair)
    {
      camera_point2_map.insert(first_camera_point2_pair, beyond_camera_point2_pair); 
    }

    /// Get cameras.
    Camera_const_iterator cameras_begin() const 
    { 
      return Camera_const_iterator(camera_point2_pairs_begin()); 
    }
    Camera_const_iterator cameras_end() const   
    { 
      return Camera_const_iterator(camera_point2_pairs_end()); 
    }
// Data
private:

    // selection flag for GUI purpose
    bool m_is_selected;

    // List of cameras
    Camera_point2_map camera_point2_map;
};


namespace boost {

/// Helper type and constant to get a "vertex_cameras" property map.
enum vertex_cameras_t { vertex_cameras } ;
BOOST_INSTALL_PROPERTY(vertex, cameras);

} // namespace boost


#endif //GYROVIZ_POINT_3_H

