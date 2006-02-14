// Copyright (c) 2002  Max Planck Institut fuer Informatik (Germany).
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
// $Id$ $Date$
// 
//
// Author(s)     : Radu Ursu

#ifndef CGAL_STRAIGHTSKELETON_2_TOOLBAR_LAYERS_H
#define CGAL_STRAIGHTSKELETON_2_TOOLBAR_LAYERS_H

#include "cgal_types.h"
#include "ss_types.h"
#include <CGAL/IO/Qt_widget.h>

#include <qobject.h>
#include <qmainwindow.h>
#include <qtoolbutton.h>
#include <qtoolbar.h>
#include <qstatusbar.h>
#include <qbuttongroup.h>


template <class T> class Qt_layer_show_polygon;
template <class T> class Qt_layer_show_skeleton;

class Layers_toolbar : public QToolBar
{
  Q_OBJECT
public:
  Layers_toolbar(CGAL::Qt_widget *w
                , QMainWindow *mw
                , PolygonalRegion const& pr
                , Sls const& sls
                , PolygonalRegion const& off
                );
  ~Layers_toolbar();
private:
  QToolButton         *but[3];
  CGAL::Qt_widget     *widget;
  QMainWindow         *window;
  QButtonGroup        *button_group;
  int                 nr_of_buttons;

  Qt_layer_show_polygon <PolygonalRegion> *showP;
  Qt_layer_show_skeleton<Sls>            *showSLS;
  Qt_layer_show_polygon <PolygonalRegion> *showO;

};//end class

#endif
