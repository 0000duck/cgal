// Copyright (c) 1997  Tel-Aviv University (Israel).
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
// Author(s)     : Baruch Zukerman <baruchzu@post.tau.ac.il>

#ifndef CGAL_SWEEP_LINE_2_DEBUG_H
#define CGAL_SWEEP_LINE_2_DEBUG_H


#include <CGAL/Basic_sweep_line_2.h>


////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
//                         DEBUG UTILITIES                                //
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////


template <class Tr, class Visit, class Crv, class Evnt, class Alloc> 
void Basic_sweep_line_2<Tr, Visit, Crv, Evnt, Alloc>::PrintEventQueue()
{
  CGAL_SL_DEBUG(std::cout << std::endl << "Event queue: " << std::endl;)
  Event_queue_iterator iter = m_queue->begin();
  while ( iter != m_queue->end() )
  {
    CGAL_SL_DEBUG(std::cout << "Point (" << iter->first << ")" << std::endl;)
    Event *e = iter->second;
    e->Print();
    ++iter;
  }
  CGAL_SL_DEBUG(std::cout << "--------------------------------" << std::endl;)
}

template <class Tr, class Visit, class Crv, class Evnt, class Alloc> 
void Basic_sweep_line_2<Tr, Visit, Crv, Evnt, Alloc>::PrintSubCurves()
{
  CGAL_SL_DEBUG(std::cout << std::endl << "Sub curves: " << std::endl;)
  for(unsigned int i=0 ; i < m_num_of_subCurves ; ++i)
  {
    m_subCurves[i].Print();
  }
}

template <class Tr, class Visit, class Crv, class Evnt, class Alloc> 
void Basic_sweep_line_2<Tr, Visit, Crv, Evnt, Alloc>::PrintStatusLine()
{
  if ( m_statusLine.size() == 0) {
    std::cout << std::endl << "Status line: empty" << std::endl;
    return;
  }
  std::cout << std::endl << "Status line: (" ;
  if(m_currentEvent->is_finite())
    std::cout << m_currentEvent->point() << ")" << std::endl;
  else
  {
    Boundary_type x = m_currentEvent->boundary_in_x(),
                  y = m_currentEvent->boundary_in_y();

    PrintInfinityType(x, y);
  }
  Status_line_iterator iter = m_statusLine.begin();
  while ( iter != m_statusLine.end() )
  {
    (*iter)->Print();
    ++iter;
  }
  std::cout << "Status line - end" << std::endl;
}

template <class Tr, class Visit, class Crv, class Evnt, class Alloc> 
void Basic_sweep_line_2<Tr, Visit, Crv, Evnt, Alloc>::PrintInfinityType
    (Boundary_type x, Boundary_type y)
{
  switch(x)
  {
  case MINUS_INFINITY:
    std::cout<<" X = -oo ";
    return;
  case PLUS_INFINITY:
    std::cout<<" X = +oo ";
    return;
  case NO_BOUNDARY:
  default:
    break;
  }

  switch(y)
  {
  case MINUS_INFINITY:
    std::cout<<" Y = -oo ";
    return;
  case PLUS_INFINITY:
    std::cout<<" Y = +oo ";
    return;
  case NO_BOUNDARY:
  default:
    CGAL_assertion(false);
  }
}

template <class Tr, class Visit, class Crv, class Evnt, class Alloc> 
void Basic_sweep_line_2<Tr, Visit, Crv, Evnt, Alloc>::PrintEvent
    (const Event* e)
{
  if(e->is_finite())
    std::cout<< e->point();
  else
  {
    Boundary_type x = e->boundary_in_x(),
                  y = e->boundary_in_y();
    PrintInfinityType(x, y);
    std::cout<<" with unbounded curve: " << e->unbounded_curve();
  } 
}

#endif
