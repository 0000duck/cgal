// Copyright (c) 2008  GeometryFactory Sarl (France).
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
// Author(s)     : Andreas Fabri <Andreas.Fabri@geometryfactory.com>
//                 Laurent Rineau <Laurent.Rineau@geometryfactory.com>

#ifndef CGAL_QT_DEMOS_MAIN_WINDOW_H
#define CGAL_QT_DEMOS_MAIN_WINDOW_H

#include <QVector>
#include <QMainWindow>
#include <CGAL/auto_link/Qt4.h>

// forward declaration
class QLabel;
class QGraphicsView;
class QAction;
class QMenu;

namespace CGAL {
namespace Qt {

// forward declaration
class GraphicsViewNavigation;

class DemosMainWindow : public QMainWindow 
{
  Q_OBJECT

public:
  unsigned int maxNumberOfRecentFiles() const ;

public slots:
  void setMaxNumberOfRecentFiles(const unsigned int);

private:
  QMenu* getMenu(QString objectName, QString title);
  void popupAboutBox(QString title, QString html_resource_name);
  QMenu* getHelpMenu();

protected:
  DemosMainWindow (QWidget * parent = 0, ::Qt::WindowFlags flags = 0 );
  void setupStatusBar();
  void addNavigation(QGraphicsView*);
  void setupOptionsMenu(QMenu* menu  = 0);
  void addAboutCGAL(QMenu* menu  = 0);
  void addAboutDemo(QString htmlResourceName, QMenu* menu  = 0);

  void addRecentFiles(QMenu* menu, QAction* insertBefore = 0);

protected slots:
  void setUseAntialiasing(bool checked);
  void setUseOpenGL(bool checked);
  void popupAboutCGAL();
  void popupAboutDemo();

  void openRecentFile_aux();
  void addToRecentFiles(QString fileName);
  void updateRecentFileActions();

signals:
  void openRecentFile(QString filename);

protected:
  QGraphicsView* view;
  GraphicsViewNavigation* navigation;
  QLabel* xycoord ;

  QAction *actionUse_OpenGL;
  QAction *actionUse_Antialiasing;
  QAction *actionAbout;
  QAction *actionAboutCGAL;

  QString aboutHtmlResource;

  QAction* recentFilesSeparator;
  unsigned int maxNumRecentFiles;
  QVector<QAction*> recentFileActs;
}; // end class DemosMainWindow
 
} // namespace Qt
} // namespace CGAL

#endif // CGAL_QT_DEMOS_MAIN_WINDOW_H
