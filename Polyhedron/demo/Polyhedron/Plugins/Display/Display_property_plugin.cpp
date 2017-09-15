#include <CGAL/Three/Polyhedron_demo_plugin_helper.h>
#include <QApplication>
#include <QObject>
#include <QAction>
#include <QMainWindow>
#include <QInputDialog>
#include "Messages_interface.h"
#include "Scene_surface_mesh_item.h"
#include "Color_ramp.h"
#include <CGAL/boost/graph/helpers.h>
#include <CGAL/Polygon_mesh_processing/compute_normal.h>
#include "ui_Display_property.h"

#define VERDICT_DBL_MIN 1.0E-30
#define VERDICT_DBL_MAX 1.0E+30

class DockWidget :
    public QDockWidget,
    public Ui::DisplayPropertyWidget
{
public:
  DockWidget(QString name, QWidget *parent)
    :QDockWidget(name,parent)
  {
    setupUi(this);
  }
};

typedef boost::graph_traits<SMesh>::halfedge_descriptor halfedge_descriptor;
typedef boost::graph_traits<SMesh>::face_descriptor face_descriptor;

class DisplayPropertyPlugin :
    public QObject,
    public CGAL::Three::Polyhedron_demo_plugin_helper
{
  Q_OBJECT
  Q_INTERFACES(CGAL::Three::Polyhedron_demo_plugin_interface)
  Q_PLUGIN_METADATA(IID "com.geometryfactory.PolyhedronDemo.PluginInterface/1.0")
public:

  bool applicable(QAction*) const Q_DECL_OVERRIDE
  {
    CGAL::Three::Scene_item* item = scene->item(scene->mainSelectionIndex());
    return qobject_cast<Scene_surface_mesh_item*>(item);
  }

  QList<QAction*> actions() const Q_DECL_OVERRIDE
  {
    return _actions;
  }

  void init(QMainWindow* mw, CGAL::Three::Scene_interface* sc, Messages_interface* mi) Q_DECL_OVERRIDE
  {
    this->scene = sc;
    this->mw = mw;

    QAction *actionDisplayAngles= new QAction(QString("Display Properties"), mw);

    actionDisplayAngles->setProperty("submenuName", "Color");

    if(actionDisplayAngles) {
      connect(actionDisplayAngles, SIGNAL(triggered()),
              this, SLOT(openDialog()));
      _actions << actionDisplayAngles;

    }
    dock_widget = new DockWidget("Property Displaying", mw);
    dock_widget->setVisible(false);
    addDockWidget(dock_widget);
    connect(dock_widget->colorizeButton, SIGNAL(clicked(bool)),
            this, SLOT(colorize()));

    connect(dock_widget->rampButton, SIGNAL(clicked(bool)),
            this, SLOT(replaceRamp()));

    connect(dock_widget->propertyBox, SIGNAL(currentIndexChanged(int)),
            this, SLOT(on_propertyBox_currentIndexChanged(int)));
    on_propertyBox_currentIndexChanged(0);

  }
private Q_SLOTS:
  void openDialog()
  {
    if(dock_widget->isVisible()) { dock_widget->hide(); }
    else                         { replaceRamp(); dock_widget->show(); }
  }

  void colorize()
  {
    Scene_surface_mesh_item* item =
        qobject_cast<Scene_surface_mesh_item*>(scene->item(scene->mainSelectionIndex()));
    if(!item)
      return;
    QApplication::setOverrideCursor(Qt::WaitCursor);
    SMesh& smesh = *item->face_graph();
    smesh.collect_garbage();
    typedef boost::property_map<SMesh, boost::vertex_point_t>::type PMap;
    PMap pmap = get(boost::vertex_point, smesh);
    switch(dock_widget->propertyBox->currentIndex())
    {
    case 0:
      displayScaledJacobian(smesh);
      break;
    default:
      displayAngles(smesh);
      break;
    }
    QApplication::restoreOverrideCursor();
    item->invalidateOpenGLBuffers();
    item->itemChanged();
  }

  void displayScaledJacobian(SMesh& smesh)
  {

    //compute and store the jacobian per face
    std::vector<float> jacobians;
    jacobians.reserve(num_faces(smesh));
    float res_min = VERDICT_DBL_MAX,
        res_max = -VERDICT_DBL_MAX;
    for(boost::graph_traits<SMesh>::face_iterator fit = faces(smesh).begin();
        fit != faces(smesh).end();
        ++fit)
    {
      jacobians.push_back(scaled_jacobian(*fit, smesh));
      if(jacobians.back() > res_max)
        res_max = jacobians.back();
      if(jacobians.back() < res_min)
        res_min = jacobians.back();

      QApplication::processEvents();
    }
    //scale a color ramp between min and max
    float max = maxBox;
    float min = minBox;
    //fill f:color pmap
    SMesh::Property_map<face_descriptor, CGAL::Color> fcolors =
        smesh.add_property_map<face_descriptor, CGAL::Color >("f:color", CGAL::Color()).first;
    int index =0;
    for(boost::graph_traits<SMesh>::face_iterator fit = faces(smesh).begin();
        fit != faces(smesh).end();
        ++fit)
    {
      if(min == max)
        --min;
      float f = (jacobians[index]-min)/(max-min);
      if(f<min)
        f = min;
      if(f>max)
        f = max;
      CGAL::Color color(
            255*color_ramp.r(f),
            255*color_ramp.g(f),
            255*color_ramp.b(f));
      ++index;
      fcolors[*fit] = color;
    }
    dock_widget->minBox->setValue(res_min);
    dock_widget->maxBox->setValue(res_max);
  }

  void displayAngles(SMesh& smesh)
  {
    typedef boost::property_map<SMesh, boost::vertex_point_t>::type PMap;
    PMap pmap = get(boost::vertex_point, smesh);
    //compute and store smallest angle per face
    std::vector<float> angles;
    float res_min = VERDICT_DBL_MAX,
        res_max = -VERDICT_DBL_MAX;
    angles.reserve(num_faces(smesh));
    for(boost::graph_traits<SMesh>::face_iterator fit = faces(smesh).begin();
        fit != faces(smesh).end();
        ++fit)
    {
      bool is_face_triangle = is_triangle(halfedge(*fit, smesh), smesh);
      bool normal_is_ok;
      EPICK::Vector_3 normal(0,0,0);

      EPICK::Orientation orientation;
      if(!is_face_triangle)
      {
        face_descriptor f = *fit;
        CGAL::Halfedge_around_face_circulator<SMesh>
            he(halfedge(f, smesh), smesh),
            he_end(he);
        do{
          normal_is_ok = true;

          //Initializes the facet orientation

          EPICK::Point_3 S,T;
          T = get(pmap, source(*he, smesh));
          S = get(pmap, target(*he, smesh));
          EPICK::Vector_3 V1((T-S).x(), (T-S).y(), (T-S).z());
          S = get(pmap,source(next(*he,smesh), smesh));
          T = get(pmap, target(next(*he,smesh), smesh));
          EPICK::Vector_3 V2((T-S).x(), (T-S).y(), (T-S).z());

          if(normal == EPICK::Vector_3(0,0,0))
            normal_is_ok = false;
          {
            normal = CGAL::cross_product(V1, V2);
          }
          if(normal_is_ok)
          {
            orientation = EPICK::Orientation_3()(V1, V2, normal);
            if( orientation == CGAL::COPLANAR )
              normal_is_ok = false;
          }
        }while( ++he != he_end && !normal_is_ok);
      }

      std::vector<float> local_angles;
      local_angles.reserve(degree(*fit, smesh));
      BOOST_FOREACH(halfedge_descriptor hd,
                    halfedges_around_face(halfedge(*fit, smesh),smesh))
      {
        halfedge_descriptor hdn = next(hd, smesh);
        EPICK::Vector_3 v1(get(pmap, source(hd, smesh)), get(pmap, target(hd, smesh))),
            v2(get(pmap, target(hdn, smesh)), get(pmap, source(hdn, smesh)));
        float norm1(CGAL::approximate_sqrt(v1.squared_length())), norm2(CGAL::approximate_sqrt(v2.squared_length()));
        float dot_prod = v1*v2;
        float angle = std::acos(dot_prod/(norm1*norm2));
        if(is_face_triangle || !normal_is_ok)
          local_angles.push_back(angle * 180/CGAL_PI);
        else
        {
          bool is_convex = true;
          EPICK::Orientation res = EPICK::Orientation_3()(v1, v2, normal) ;
          if(res!= orientation && res != CGAL::ZERO)
            is_convex = false;
          local_angles.push_back(is_convex ? angle * 180/CGAL_PI : 360 - angle * 180/CGAL_PI );
        }
      }
      std::sort(local_angles.begin(), local_angles.end());
      angles.push_back(local_angles.front());
      if(angles.back() > res_max)
        res_max = angles.back();
      if(angles.back() < res_min)
        res_min = angles.back();
      QApplication::processEvents();
    }
    //scale a color ramp between min and max

    float max = maxBox;
    float min = minBox;

    //fill f:color pmap
    SMesh::Property_map<face_descriptor, CGAL::Color> fcolors =
        smesh.add_property_map<face_descriptor, CGAL::Color >("f:color", CGAL::Color()).first;
    int index =0;
    for(boost::graph_traits<SMesh>::face_iterator fit = faces(smesh).begin();
        fit != faces(smesh).end();
        ++fit)
    {
      if(min == max)
        --min;
      float f = (angles[index]-min)/(max-min);
      if(f<0)
        f = 0;
      if(f>1)
        f = 1;
      CGAL::Color color(
            255*color_ramp.r(f),
            255*color_ramp.g(f),
            255*color_ramp.b(f));
      ++index;
      fcolors[*fit] = color;
    }
    dock_widget->minBox->setValue(res_min);
    dock_widget->maxBox->setValue(res_max);
  }

  void replaceRamp()
  {
    double rm = dock_widget->redMinBox->value();
    double rM = dock_widget->redMaxBox->value();

    double gm = dock_widget->greenMinBox->value();
    double gM = dock_widget->greenMaxBox->value();

    double bm = dock_widget->blueMinBox->value();
    double bM = dock_widget->blueMaxBox->value();

    color_ramp = Color_ramp(rm, rM, gm, gM, bm, bM);
    displayLegend();
    minBox = dock_widget->minBox->value();
    maxBox = dock_widget->maxBox->value();
  }

  void on_propertyBox_currentIndexChanged(int)
  {
    switch(dock_widget->propertyBox->currentIndex())
    {
    case 0:
      dock_widget->minBox->setMinimum(-1000);
      dock_widget->minBox->setMaximum(1000);
      dock_widget->minBox->setValue(0);

      dock_widget->maxBox->setMinimum(-1000);
      dock_widget->maxBox->setMaximum(1000);
      dock_widget->maxBox->setValue(2);
      break;
    default:
      dock_widget->minBox->setMinimum(0);
      dock_widget->minBox->setMaximum(360);
      dock_widget->minBox->setValue(0);

      dock_widget->maxBox->setMinimum(0);
      dock_widget->maxBox->setMaximum(360);
      dock_widget->maxBox->setValue(60);
      break;
    }
  }

  void closure()
  {
    dock_widget->hide();
  }

private:
  void displayLegend()
  {
    // Create an legend_ and display it
    const int height = 256;
    const int width = 256;
    const int cell_width = width/3;
    const int top_margin = 5;
    const int left_margin = (width-cell_width)/2;
    const int drawing_height = height - top_margin * 2;
    const int text_height = 20;

    legend_ = QPixmap(width, height + text_height);
    legend_.fill(QColor(200, 200, 200));

    QPainter painter(&legend_);
    painter.setPen(Qt::black);
    painter.setBrush(QColor(200, 200, 200));

    // Build legend_ data
    double min_value(dock_widget->minBox->value()),
        max_value(dock_widget->maxBox->value());
    std::vector<double> graduations(100);
    for(int i=0; i<100; ++i)
      graduations[i] = i/100.0;

    // draw
    int i=0;
    for (std::vector<double>::iterator it = graduations.begin(), end = graduations.end();
         it != end; ++it, i+=2)
    {
      QColor color(255*color_ramp.r(*it),
                   255*color_ramp.g(*it),
                   255*color_ramp.b(*it));
      painter.fillRect(left_margin,
                       drawing_height - top_margin - i,
                       cell_width,
                       2,
                       color);
    }

    // draw right vertical line
    painter.setPen(Qt::blue);

    painter.drawLine(QPoint(left_margin + cell_width+10, drawing_height - top_margin),
                     QPoint(left_margin + cell_width+10,
                            drawing_height - top_margin - static_cast<int>(graduations.size())*2));


    // draw min value and max value
    painter.setPen(Qt::blue);
    QRect min_text_rect(left_margin + cell_width+10,drawing_height - top_margin,
                        50, text_height);
    painter.drawText(min_text_rect, Qt::AlignCenter, tr("%1").arg(min_value, 0, 'f', 1));

    QRect max_text_rect(left_margin + cell_width+10, drawing_height - top_margin - 200,
                        50, text_height);
    painter.drawText(max_text_rect, Qt::AlignCenter, tr("%1").arg(max_value, 0, 'f', 1));

    dock_widget->legendLabel->setPixmap(legend_);
  }
  double scaled_jacobian( const face_descriptor& f , const SMesh mesh);
  QList<QAction*> _actions;
  Color_ramp color_ramp;
  DockWidget* dock_widget;
  float minBox;
  float maxBox;
  QPixmap legend_;
};

  /// Code based on the verdict module of vtk

  /*=========================================================================
  Copyright (c) 2006 Sandia Corporation.
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.
=========================================================================*/

  double DisplayPropertyPlugin::scaled_jacobian( const face_descriptor& f , const SMesh mesh)
  {
    boost::property_map<SMesh, boost::vertex_point_t>::type
        pmap = get(boost::vertex_point, mesh);
    std::vector<double> corner_areas(degree(f, mesh));
    std::vector<EPICK::Vector_3> edges;
    BOOST_FOREACH(halfedge_descriptor hd, CGAL::halfedges_around_face(halfedge(f, mesh), mesh))
    {
      edges.push_back(EPICK::Vector_3(get(pmap, source(hd, mesh)), get(pmap, target(hd, mesh))));
    }
    BOOST_FOREACH(halfedge_descriptor hd, CGAL::halfedges_around_face(halfedge(f, mesh), mesh))
    {
      std::vector<EPICK::Vector_3> corner_normals;
      for(std::size_t i = 0; i < edges.size(); ++i)
      {
        corner_normals.push_back(CGAL::cross_product(edges[i], edges[(i+1)%(edges.size())]));
      }


      EPICK::Vector_3 unit_center_normal = CGAL::Polygon_mesh_processing::compute_face_normal(f, mesh);
      unit_center_normal *= 1.0/CGAL::approximate_sqrt(unit_center_normal.squared_length());

      for(std::size_t i = 0; i < corner_areas.size(); ++i)
      {
        corner_areas[i] =  unit_center_normal*corner_normals[i];
      }
    }
    std::vector<double> length;
    for(std::size_t i=0; i<edges.size(); ++i)
    {
      length.push_back(CGAL::approximate_sqrt(edges[i].squared_length()));
      if( length[i] < VERDICT_DBL_MIN)
        return 0.0;
    }
    double min_scaled_jac = VERDICT_DBL_MAX;
    for(std::size_t i=0; i<edges.size(); ++i)
    {
      double scaled_jac = corner_areas[i] / (length[i] * length[(i+edges.size()-1)%(edges.size())]);
      min_scaled_jac = (std::min)( scaled_jac, min_scaled_jac );
    }

    if( min_scaled_jac > 0 )
      return (double) (std::min)( min_scaled_jac, VERDICT_DBL_MAX );
    return (double) (std::max)( min_scaled_jac, -VERDICT_DBL_MAX );

  }


#include "Display_property_plugin.moc"
