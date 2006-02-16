// Copyright (c) 2001-2004  ENS of Paris (France).
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
// Author(s)     : Pierre Angelier, Michel Pocchiola

#ifndef CGAL_BITANGENT_2_BITANGENT_2_INTERSECTION_H
#define CGAL_BITANGENT_2_BITANGENT_2_INTERSECTION_H

#include <CGAL/basic.h>
#include <CGAL/Bitangent_2.h>
#include <CGAL/Segment_2_Segment_2_intersection.h>

CGAL_BEGIN_NAMESPACE

// -----------------------------------------------------------------------------
template < class D_ >
bool do_intersect( const Bitangent_2<D_>& b1, const Bitangent_2<D_>& b2 )
{
    typedef typename Bitangent_2<D_>::Segment_2 Segment_2;
    // -------------------------------------------------------------------------
    if (b1 == b2) return true;
    // -------------------------------------------------------------------------
    if (b1.source_object() == b2.source_object() && 
	b1.target_object() == b2.target_object())
	return (b1.is_internal() && b2.is_internal());
    // -------------------------------------------------------------------------
    if (b1.source_object() == b2.target_object() && 
	b1.target_object() == b2.source_object()) {
	if (b1.is_internal() && b2.is_internal()) return true;
	if (b1.is_internal() || b2.is_internal()) return false;
	return (b1.type() != b2.type());
    }
    // -------------------------------------------------------------------------
    if (b1.source() == b2.source()) {
	if (b1.is_right_xx() == b2.is_right_xx()) return false;
	return ((b1.is_right_xx() && 
		 D_().left_turn_2_object()(b1.source(),b1.target(),b2.target())) || 
		(b2.is_right_xx() && 
		 D_().left_turn_2_object()(b2.source(),b2.target(),b1.target())));
    }
    // -------------------------------------------------------------------------
    if (b1.target() == b2.target()) {
	if (b1.is_xx_right() == b2.is_xx_right()) return false;
	return ((b1.is_xx_right() && 
		 D_().left_turn_2_object()(b1.source(),b1.target(),b2.source())) || 
		(b2.is_xx_right() &&
		 D_().left_turn_2_object()(b2.source(),b2.target(),b1.source())));
    }
    // -------------------------------------------------------------------------
    if (b1.target() == b2.source()) {
	if (b1.is_xx_right() != b2.is_right_xx()) return false;
	return ((b1.is_xx_right() && 
		 D_().left_turn_2_object()(b1.source(),b1.target(),b2.target())) || 
		(b1.is_xx_left()  && 
		 D_().left_turn_2_object()(b2.target(),b1.target(),b1.source())));
    }
    // -------------------------------------------------------------------------
    if (b1.source() == b2.target()) {
	if (b1.is_right_xx() != b2.is_xx_right()) return false;
	return ((b2.is_xx_right() && 
		 D_().left_turn_2_object()(b2.source(),b2.target(),b1.target())) || 
		(b2.is_xx_left()  && 
		 D_().left_turn_2_object()(b1.target(),b2.target(),b2.source())));
    }
    // -------------------------------------------------------------------------
    return D_().do_intersect_2_object()(Segment_2(b1.source(),b1.target()),
					Segment_2(b2.source(),b2.target()));
    // -------------------------------------------------------------------------
}
// -----------------------------------------------------------------------------

CGAL_END_NAMESPACE

#endif // CGAL_BITANGENT_2_BITANGENT_2_INTERSECTION_H
