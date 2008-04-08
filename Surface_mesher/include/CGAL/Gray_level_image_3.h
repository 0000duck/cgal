// Copyright (c) 2005-2007  INRIA Sophia-Antipolis (France).
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
// Author(s)     : Laurent RINEAU

#ifndef CGAL_MESH_3_GRAY_LEVEL_IMAGE_3_H
#define CGAL_MESH_3_GRAY_LEVEL_IMAGE_3_H

#include <CGAL/basic.h>

#include <CGAL/auto_link/ImageIO.h>

#include <boost/shared_ptr.hpp>

#ifdef CGAL_SURFACE_MESHER_DEBUG_GRAY_LEVEL_IMAGE_3_CONSTRUCTOR
#include <boost/format.hpp>
#endif

#include <CGAL/Image_3.h>

namespace CGAL {

template <typename FT, typename Point>
class Gray_level_image_3 : public Image_3<FT, Point>
{
  typedef Image_3<FT, Point> Image;
  float isovalue;
  bool positive_inside;
public:
  Gray_level_image_3(const char* file, float isoval, bool positive_inside_=true)
    : Image(),
      isovalue(isoval),
      positive_inside(positive_inside_)
  {
#ifdef CGAL_SURFACE_MESHER_DEBUG_GRAY_LEVEL_IMAGE_3_CONSTRUCTOR
    std::cerr << 
      ::boost::format("Constructing a Gray_level_image_3(\"%1%\")... ") % file;
#endif
    Image::read(file);
#ifdef CGAL_SURFACE_MESHER_DEBUG_GRAY_LEVEL_IMAGE_3_CONSTRUCTOR
    if( image_ptr.get() != 0 )
    {
      std::cerr << ::boost::format(" = %1%\n") % image_ptr.get();
    }
#endif
  }

  ~Gray_level_image_3()
  {
#ifdef CGAL_SURFACE_MESHER_DEBUG_GRAY_LEVEL_IMAGE_3_CONSTRUCTOR
      std::cerr << ::boost::format("~Gray_level_image_3() image=%1%\n") % image;
#endif
  }

  static void print_supported_file_format()
  {
    ::printSupportedFileFormat();
  }

  bool inside(const float X, const float Y, const float Z) const
  {
    return ( X>=0 && Y>=0 && Z>=0 && 
             X<=this->max_x && Y<=this->max_y && Z<=this->max_z );
  }

  FT operator()(Point p) const
  {
    const float X=static_cast<float>(to_double(p.x()));
    const float Y=static_cast<float>(to_double(p.y()));
    const float Z=static_cast<float>(to_double(p.z()));

    if (!inside(X,Y,Z))
      return FT(1);
    else {
      float value = ::triLinInterp(this->image_ptr.get(), X, Y, Z); 
      if (positive_inside)
      {
         if (value > isovalue) // inside
	   return FT(-1);
         else if (value < isovalue) // outside
	   return FT(1);
         else
	   return FT(0);
      }
      else
      {      
         if (value < isovalue) // inside
	   return FT(-1);
         else if (value > isovalue) // outside
          return FT(1);
         else
	  return FT(0);
      }
    }
  }
}; // end Gray_level_image_3
 
} // end namespace CGAL

#endif // CGAL_MESH_3_GRAY_LEVEL_IMAGE_3_H
