// Copyright (c) 1997-2002  Max-Planck-Institute Saarbrucken (Germany).
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
// $Source$
// $Revision$ $Date$
// $Name$
//
// Author(s)     : Michael Seel    <seel@mpi-sb.mpg.de>
//                 Miguel Granados <granados@mpi-sb.mpg.de>
//                 Susan Hert      <hert@mpi-sb.mpg.de>
//                 Lutz Kettner    <kettner@mpi-sb.mpg.de>
#ifndef CGAL_SNC_VISUALIZOR_OGL_H
#define CGAL_SNC_VISUALIZOR_OGL_H

#include <CGAL/basic.h>
#include <CGAL/IO/Color.h>
#include <GL/glut.h>
#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <string>
#include <vector>
#include <CGAL/Simple_cartesian.h>
#include <CGAL/Nef_3/SNC_structure.h>
#include <CGAL/Nef_3/SNC_decorator.h>
#include <CGAL/Nef_3/SNC_FM_decorator.h>

#undef _DEBUG
#define _DEBUG 53
#include <CGAL/Nef_3/debug.h>

#define CGAL_NEF3_MARKED_VERTEX_COLOR 221,221,255
#define CGAL_NEF3_UNMARKED_VERTEX_COLOR 180,180,214
#define CGAL_NEF3_MARKED_EDGE_COLOR 221,221,255
#define CGAL_NEF3_UNMARKED_EDGE_COLOR 180,180,214
#define CGAL_NEF3_MARKED_FACET_COLOR 221,255,255
#define CGAL_NEF3_UNMARKED_FACET_COLOR 154,188,188

CGAL_BEGIN_NAMESPACE
namespace OGL {

// ----------------------------------------------------------------------------
// Drawable double types:
// ----------------------------------------------------------------------------

  typedef CGAL::Simple_cartesian<double> DKernel;  
  typedef DKernel::Point_3               Double_point;
  typedef DKernel::Vector_3              Double_vector;
  typedef DKernel::Segment_3             Double_segment;
  typedef DKernel::Aff_transformation_3  Affine_3;

  // DPoint = a double point including a mark
  class DPoint : public Double_point {
    bool m_;
  public:
    DPoint() {}
    DPoint(const Double_point& p, bool m) : Double_point(p) { m_ = m; }
    DPoint(const DPoint& p) : Double_point(p) { m_ = p.m_; }
    DPoint& operator=(const DPoint& p)
    { Double_point::operator=(p); m_ = p.m_; return *this; }
    bool mark() const { return m_; }
  };

  // DSegment = a double segment including a mark
  class DSegment : public Double_segment {
    bool m_;
  public:
    DSegment() {}
    DSegment(const Double_segment& s, bool m) : Double_segment(s) { m_ = m; }
    DSegment(const DSegment& s) : Double_segment(s) { m_ = s.m_; }
    DSegment& operator=(const DSegment& s)
    { Double_segment::operator=(s); m_ = s.m_; return *this; }
    bool mark() const { return m_; }
  };

  // Double_triple = a class that stores a triple of double
  // coordinates; we need a pointer to the coordinates in a C array
  // for OpenGL
  class Double_triple {
    typedef double*       double_ptr;
    typedef const double* const_double_ptr;
    double coords_[3];
  public:
    Double_triple() 
    { coords_[0]=coords_[1]=coords_[2]=0.0; }
    Double_triple(double x, double y, double z)
    { coords_[0]=x; coords_[1]=y; coords_[2]=z; }
    Double_triple(const Double_triple& t)
    { coords_[0]=t.coords_[0];
      coords_[1]=t.coords_[1];
      coords_[2]=t.coords_[2];
    }
    Double_triple& operator=(const Double_triple& t)
    { coords_[0]=t.coords_[0];
      coords_[1]=t.coords_[1];
      coords_[2]=t.coords_[2];
      return *this; }
    operator double_ptr() const 
    { return const_cast<Double_triple&>(*this).coords_; }
    double operator[](unsigned i) 
    { CGAL_nef3_assertion(i<3); return coords_[i]; }
  }; // Double_triple

  static std::ostream& operator << (std::ostream& os,
				    const Double_triple& t)
  { os << "(" << t[0] << "," << t[1] << "," << t[2] << ")";
    return os; }


  // DFacet stores the facet cycle vertices in a continuus C array
  // of three double components, this is necessary due to the OpenGL
  // tesselator input format !
  class DFacet {
    typedef std::vector<Double_triple>   Coord_vector;
    typedef std::vector<unsigned>        Cycle_vector;
    Coord_vector    coords_;  // stores all vertex coordinates
    Cycle_vector    fc_ends_; // stores entry points of facet cycles
    Double_triple   normal_; // stores normal and mark of facet
    bool            mark_;

  public:
    typedef Coord_vector::iterator Coord_iterator;
    typedef Coord_vector::const_iterator Coord_const_iterator;

    DFacet() {}

    void push_back_vertex(double x, double y, double z)
    { coords_.push_back(Double_triple(x,y,z)); }

    DFacet(const DFacet& f) 
    { coords_  = f.coords_;
      fc_ends_ = f.fc_ends_;
      normal_  = f.normal_;
      mark_    = f.mark_;
    }

    DFacet& operator=(const DFacet& f) 
    { coords_ =  f.coords_;
      fc_ends_ = f.fc_ends_;
      normal_ =  f.normal_;
      mark_    = f.mark_;
      return *this; 
    }

    ~DFacet()
    { coords_.clear(); fc_ends_.clear(); }

    void push_back_vertex(const Double_point& p)
    { push_back_vertex(p.x(),p.y(),p.z()); }
   
    void set_normal(double x, double y, double z, bool m)
    { double l = sqrt(x*x + y*y + z*z);
      normal_ = Double_triple(x/l,y/l,z/l); mark_ = m; }

    double dx() const { return normal_[0]; }
    double dy() const { return normal_[1]; }
    double dz() const { return normal_[2]; }
    bool mark() const { return mark_; }
    double* normal() const 
    { return static_cast<double*>(normal_); }

    void new_facet_cycle()
    { fc_ends_.push_back(coords_.size()); }
    
    unsigned number_of_facet_cycles() const
    { return fc_ends_.size(); }

    Coord_iterator facet_cycle_begin(unsigned i) 
    { CGAL_nef3_assertion(i<number_of_facet_cycles());
      if (i==0) return coords_.begin();
      else return coords_.begin()+fc_ends_[i]; }

    Coord_iterator facet_cycle_end(unsigned i) 
    { CGAL_nef3_assertion(i<number_of_facet_cycles());
      if (i<fc_ends_.size()-1) return coords_.begin()+fc_ends_[i+1];
      else return coords_.end(); }

    Coord_const_iterator facet_cycle_begin(unsigned i) const
    { CGAL_nef3_assertion(i<number_of_facet_cycles());
      if (i==0) return coords_.begin();
      else return coords_.begin()+fc_ends_[i]; }

    Coord_const_iterator facet_cycle_end(unsigned i) const
    { CGAL_nef3_assertion(i<number_of_facet_cycles());
      if (i<fc_ends_.size()-1) return coords_.begin()+fc_ends_[i+1];
      else return coords_.end(); }

    void debug(std::ostream& os = std::cerr) const
    { os << "DFacet, normal=" << normal_ << ", mark=" << mark() << std::endl;
      for(unsigned i=0; i<number_of_facet_cycles(); ++i) {
	os << "  facet cycle ";
	// put all vertices in facet cycle into contour:
	Coord_const_iterator cit;
	for(cit = facet_cycle_begin(i); cit != facet_cycle_end(i); ++cit)
	  os << *cit;
	os << std::endl;
      }
    }
    
  }; // DFacet


// ----------------------------------------------------------------------------
// OGL Drawable Polyhedron:
// ----------------------------------------------------------------------------

  inline void beginCallback(GLenum which)
  { glBegin(which); }

  inline void endCallback(void)
  { glEnd(); }

  inline void errorCallback(GLenum errorCode)
  { const GLubyte *estring;
    estring = gluErrorString(errorCode);
    fprintf(stderr, "Tessellation Error: %s\n", estring);
    exit (0);
  }

  inline void vertexCallback(GLvoid* vertex,
			     GLvoid* user)
  { GLdouble* pc(static_cast<GLdouble*>(vertex));
    GLdouble* pu(static_cast<GLdouble*>(user));
    TRACEN("vertexCallback coord  "<<pc[0]<<","<<pc[1]<<","<<pc[2]);
    TRACEN("vertexCallback normal "<<pu[0]<<","<<pu[1]<<","<<pu[2]);
    glNormal3dv(pu);
    glVertex3dv(pc); 
  }

  class Polyhedron {
    std::list<DPoint>    vertices_;
    std::list<DSegment>  edges_;
    std::list<DFacet>    halffacets_;

    GLuint         object_list_;
    bool init_, axes_, surface_;

    Bbox_3  bbox_;

    typedef std::list<DPoint>::const_iterator   Vertex_iterator;
    typedef std::list<DSegment>::const_iterator Edge_iterator;
    typedef std::list<DFacet>::const_iterator   Halffacet_iterator;

  public:
    Polyhedron() : bbox_(-1,-1,-1,1,1,1)
    { object_list_ = 0;
      init_ = axes_ = false; surface_ = true; }

    Polyhedron(const Polyhedron& p)
    { object_list_ = 0;
      init_ = axes_ = false; surface_ = true; }

    Polyhedron& operator=(const Polyhedron& p)
    { return *this; }

    ~Polyhedron() 
    { if (object_list_) glDeleteLists(object_list_, 4); }

    void push_back(const Double_point& p, bool m) {
        vertices_.push_back(DPoint(p,m));
    }
    void push_back(const Double_segment& s, bool m) 
    { edges_.push_back(DSegment(s,m)); }
    void push_back(const DFacet& f) 
    { halffacets_.push_back(f); }
 
    void toggle_axes() { axes_ = !axes_; }
    void skeleton_on() { surface_ = false; }
    void boundary_on() { surface_ = true; }
    bool is_initialized() const { return init_; }

    Bbox_3  bbox() const { return bbox_; }
    Bbox_3& bbox()       { return bbox_; }

    void draw(Vertex_iterator v) const
    { TRACEN("drawing vertex "<<*v);
      CGAL::Color cf(CGAL_NEF3_MARKED_VERTEX_COLOR), 
	ct(CGAL_NEF3_UNMARKED_VERTEX_COLOR); // more blue-ish
      CGAL::Color c = v->mark() ? ct : cf;
      glPointSize(10);
      glColor3ub(c.red(), c.green(), c.blue());
      glBegin(GL_POINTS);
      glVertex3d(v->x(),v->y(),v->z());
      glEnd();           
    }

    void draw(Edge_iterator e) const
    { TRACEN("drawing edge "<<*e);
      Double_point p = e->source(), q = e->target();
      CGAL::Color cf(CGAL_NEF3_MARKED_EDGE_COLOR), 
	ct(CGAL_NEF3_UNMARKED_EDGE_COLOR); // more blue-ish
      CGAL::Color c = e->mark() ? ct : cf;
      glLineWidth(5);
      glColor3ub(c.red(),c.green(),c.blue());
      glBegin(GL_LINE_STRIP);
      glVertex3d(p.x(), p.y(), p.z());
      glVertex3d(q.x(), q.y(), q.z());
      glEnd();
    }

    void draw(Halffacet_iterator f) const
    { TRACEN("drawing facet "<<(f->debug(),""));
      GLUtesselator* tess_ = gluNewTess();
      gluTessCallback(tess_, GLenum(GLU_TESS_VERTEX_DATA),
		      (GLvoid (*)()) &vertexCallback);
      gluTessCallback(tess_, GLenum(GLU_TESS_BEGIN),
		      (GLvoid (*)()) &beginCallback);
      gluTessCallback(tess_, GLenum(GLU_TESS_END),
		      (GLvoid (*)()) &endCallback);
      gluTessCallback(tess_, GLenum(GLU_TESS_ERROR),
		      (GLvoid (*)()) &errorCallback);
      gluTessProperty(tess_, GLenum(GLU_TESS_WINDING_RULE),
		      GLU_TESS_WINDING_POSITIVE);

      DFacet::Coord_const_iterator cit;
      CGAL::Color cf(CGAL_NEF3_MARKED_FACET_COLOR),
	ct(CGAL_NEF3_UNMARKED_FACET_COLOR); // more blue-ish
      CGAL::Color c = (f->mark() ? ct : cf);
      glColor3ub(c.red(),c.green(),c.blue());
      gluTessBeginPolygon(tess_,f->normal());
      TRACEN(" ");
      TRACEN("Begin Polygon");
      gluTessNormal(tess_,f->dx(),f->dy(),f->dz());
      // forall facet cycles of f:
      for(unsigned i = 0; i < f->number_of_facet_cycles(); ++i) {
        gluTessBeginContour(tess_);
	TRACEN("  Begin Contour");
	// put all vertices in facet cycle into contour:
	for(cit = f->facet_cycle_begin(i); 
	    cit != f->facet_cycle_end(i); ++cit) {
	  gluTessVertex(tess_, *cit, *cit);
	  TRACEN("    add Vertex");
	}
        gluTessEndContour(tess_);
	TRACEN("  End Contour");
      }
      gluTessEndPolygon(tess_);
      TRACEN("End Polygon");
      gluDeleteTess(tess_);
    }

    void construct_axes() const
    { 
      glLineWidth(2.0);
      // red x-axis
      glColor3f(1.0,0.0,0.0);
      glBegin(GL_LINES);
      glVertex3f(0.0,0.0,0.0);
      glVertex3f(5.0,0.0,0.0);
      glEnd();
       // green y-axis 
      glColor3f(0.0,1.0,0.0);
      glBegin(GL_LINES);
      glVertex3f(0.0,0.0,0.0);
      glVertex3f(0.0,5.0,0.0);
      glEnd();
      // blue z-axis and equator
      glColor3f(0.0,0.0,1.0);
      glBegin(GL_LINES);
      glVertex3f(0.0,0.0,0.0);
      glVertex3f(0.0,0.0,5.0);
      glEnd();
      // six coordinate points in pink:
      glPointSize(10);
      glBegin(GL_POINTS);
      glColor3f(1.0,0.0,0.0);
      glVertex3d(5,0,0);
      glColor3f(0.0,1.0,0.0);
      glVertex3d(0,5,0);
      glColor3f(0.0,0.0,1.0);
      glVertex3d(0,0,5);
      glEnd();
    }


    void fill_display_lists()
    {
      glNewList(object_list_, GL_COMPILE);
      Vertex_iterator v;
      CGAL_nef3_forall_iterators(v,vertices_) draw(v);
      glEndList();     

      glNewList(object_list_+1, GL_COMPILE);
      Edge_iterator e;
      CGAL_nef3_forall_iterators(e,edges_) draw(e);
      glEndList();     

      glNewList(object_list_+2, GL_COMPILE);
      Halffacet_iterator f;
      CGAL_nef3_forall_iterators(f,halffacets_) draw(f);
      glEndList();

      glNewList(object_list_+3, GL_COMPILE); // axes:
      construct_axes();
      glEndList();

    }

    void initialize() 
    { if (init_) return;
      init_ = true;
      axes_ = false; 
      object_list_ = glGenLists(4); CGAL_nef3_assertion(object_list_); 
      fill_display_lists();
    }


    void draw( GLdouble z_vec[3]) const
    { 
      if (!is_initialized()) const_cast<Polyhedron&>(*this).initialize();
      double l = std::max( std::max( bbox().xmax() - bbox().xmin(),
                                     bbox().ymax() - bbox().ymin()),
                           bbox().zmax() - bbox().zmin());
      if ( l < 1) // make sure that a single point doesn't screw up here
          l = 1;
      glScaled( 4.0/l, 4.0/l, 4.0/l);
      glTranslated( -(bbox().xmax() + bbox().xmin()) / 2.0,
                    -(bbox().ymax() + bbox().ymin()) / 2.0,
                    -(bbox().zmax() + bbox().zmin()) / 2.0);
      if ( surface_ ) {
	glEnable(GL_LIGHTING); 
	glCallList(object_list_+2); // facets
	glDisable(GL_LIGHTING);
      }
      // move edges and vertices a bit towards the view-point, 
      // i.e., 1/100th of the unit vector in camera space
      double f = l / 4.0 / 100.0;
      glTranslated( z_vec[0] * f, z_vec[1] * f, z_vec[2] * f);
      glCallList(object_list_+1); // edges
      glCallList(object_list_);   // vertices
      if (axes_) glCallList(object_list_+3); // axis
   }

    void debug(std::ostream& os = std::cerr) const
    {
      os << "OGL::Polyhedron" << std::endl;
      os << "Vertices:" << std::endl;
      Vertex_iterator v;
      CGAL_nef3_forall_iterators(v,vertices_) 
	os << "  "<<*v<<", mark="<<v->mark()<<std::endl;
      os << "Edges:" << std::endl;
      Edge_iterator e;
      CGAL_nef3_forall_iterators(e,edges_) 
	os << "  "<<*e<<", mark="<<e->mark()<<std::endl;
      os << "Facets:" << std::endl;
      Halffacet_iterator f;
      CGAL_nef3_forall_iterators(f,halffacets_) f->debug(); os << std::endl;
      os << std::endl;
    }

  }; // Polyhedron

// ----------------------------------------------------------------------------
// Viewer configuration:
// ----------------------------------------------------------------------------

  enum MenuEntries { ROTATE, SCALE, TRANSLATE, TRANS_Z, RESET_CONTROL, 
		     AXES, BOUNDARY, SKELETON, PERSP, FULLSCREEN, QUIT };

  const double znear = 4.0;
  const double zfar  = 4.0;
  const double eye   = 6.0;
  const double wsize = 3.0;

  int window_width  = 600;           // Breite und
  int window_height = 600;           // Hoehe des Fensters
  int window_radius = 300;           // min(width,height) / 2

  bool perspective  = true;

  int mouse_x, mouse_y;              // Mauskoordinaten linker button
  int interaction;                   // type of interaction in motion fct.
  int motion_mode       = ROTATE;    // Bewegen der Maus bei Mouse1 gedrueckt
  int submenu1, submenu2;
  double dx = 0;                     // Translation
  double dy = 0;                     // Translation
  double dz = 0;                     // Translation in Z
  double s  = 0.4;                   // Skalierung
  Affine_3    rotation( IDENTITY);   // Rotation
                       
  long double factor_s;              // Umrechnungsfaktor fuer Skalierung

  // our draw object:
  static std::vector<Polyhedron>  polyhedra_;
  static std::vector<std::string> titles_;

static Polyhedron& add_polyhedron() 
{ polyhedra_.push_back(Polyhedron()); 
  return polyhedra_.back(); }

static void show (int mode)
{
  std::vector<Polyhedron>::iterator it;
  switch(mode)
  {
    case ROTATE:
    case SCALE:
    case TRANSLATE:
    case TRANS_Z:
      motion_mode = mode;
      break;
    case RESET_CONTROL:
      dx = dy = dz = 0.0;
      s = 0.4;
      rotation = Affine_3( IDENTITY);
      motion_mode = ROTATE;
      CGAL_nef3_forall_iterators(it,polyhedra_) it->initialize();
      glutPostRedisplay();
      break;
    case AXES:
      CGAL_nef3_forall_iterators(it,polyhedra_) it->toggle_axes();
      glutPostRedisplay();
      break;
    case BOUNDARY:
      CGAL_nef3_forall_iterators(it,polyhedra_) it->boundary_on();
      //CGAL_nef3_forall_iterators(it,polyhedra_) it->draw();
      glutPostRedisplay();
      break;
    case SKELETON:
      CGAL_nef3_forall_iterators(it,polyhedra_) it->skeleton_on();
      //CGAL_nef3_forall_iterators(it,polyhedra_) it->draw();
      glutPostRedisplay();
      break;
    case PERSP:
      perspective = ! perspective;
      break;
    case FULLSCREEN:
      glutFullScreen();
      break;
    case QUIT: 
      exit(0);
  }
}



// Mausknopf gedrueckt
static void mouse (int button, int state, int x, int y)
{
    mouse_x = x;
    mouse_y = y;
    interaction = 0;
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        if ( GLUT_ACTIVE_SHIFT & glutGetModifiers())
            interaction = SCALE;
        else
            interaction = motion_mode;
    }
    if (button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN)
        if ( GLUT_ACTIVE_SHIFT & glutGetModifiers())
            interaction = TRANS_Z;
        else
            interaction = TRANSLATE;
}


static Affine_3 virtual_sphere_transformation( double old_x, double old_y, 
                                               double new_x, double new_y) {
    if ( old_x == new_x && old_y == new_y)// zero rotation.
	return Affine_3( IDENTITY);
    // Determine the projected vectors on the `sphere'.
    double dd = old_x * old_x + old_y * old_y;
    Double_vector v_old( old_x, old_y, 
                         ((dd < 0.5) ? std::sqrt(1-dd) : 0.5 / std::sqrt(dd)));
    dd = new_x * new_x + new_y * new_y;
    Double_vector v_new( new_x, new_y, 
                         ((dd < 0.5) ? std::sqrt(1-dd) : 0.5 / std::sqrt(dd)));
    Double_vector axis  = cross_product( v_old, v_new);
    double angle = 0.0;
    double norm = std::sqrt( (v_old*v_old)*(v_new*v_new));
    if ( norm != 0) {
        double x = v_old*v_new/ norm;
        if ( x <= -1)
        angle = M_PI;
        if ( x < 1)
            angle = std::acos(x);
    }
    double len = std::sqrt( double(axis * axis));
    double s   = std::sin( angle / 2.0) / len;
    double q1 = axis.x() * s; // quaternion
    double q2 = axis.y() * s;
    double q3 = axis.z() * s;
    double q0 = std::cos( angle / 2.0);
    double a   = q1 * q2;
    double b   = q0 * q3;
    double c   = q1 * q3;
    double d   = q0 * q2;
    double e   = q2 * q3;
    double f   = q0 * q1;
    double qq0 = q0 * q0;
    double qq1 = q1 * q1;
    double qq2 = q2 * q2;
    double qq3 = q3 * q3;
    return Affine_3( qq0 + qq1 - qq2 - qq3, 2 * (a-b), 2 * (c+d),
                     2 * (a+b), qq0 - qq1 + qq2 - qq3, 2 * (e-f),
                     2 * (c-d), 2 * (e+f), qq0 - qq1 - qq2 + qq3);
}


// Objekt rotieren, zoomen oder verschieben
static void motion (int x, int y)
{
    switch ( interaction) {
    case SCALE:
        s *= exp( (x - mouse_x + mouse_y -y) * factor_s );
        break;
    case ROTATE: {
        double old_x =   1.2 * (mouse_x -  window_width/2) / window_radius;
        double old_y = - 1.2 * (mouse_y - window_height/2) / window_radius;
        double new_x =   1.2 * (x -  window_width/2) / window_radius;
        double new_y = - 1.2 * (y - window_height/2) / window_radius;
        rotation =  virtual_sphere_transformation( old_x, old_y, new_x, new_y)
            * rotation;
        }
        break;
    case TRANSLATE:
        dx += (x - mouse_x) * 2.0 / window_radius;
        dy -= (y - mouse_y) * 2.0 / window_radius;
        break;
    case TRANS_Z:
        dz += (x - mouse_x + mouse_y -y) * 2.0 / window_radius;
        break;
    default:
        break;
    }
    mouse_x = x;
    mouse_y = y;
    glutPostRedisplay();
}

static void init()
{
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  GLfloat light_diffuse[] =  { 0.5, 0.5, 0.5, 0.7 };    // white diffuse light 
  GLfloat light_position[] = { -4.0, -4.0, 10.0, 0.0 }; // infinite location
 
  glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);
  glEnable(GL_LIGHT0);

  GLfloat mat_diffuse[4] = { 0.3, 0.3, 0.3, 1.0 };
  // GLfloat mat_specular[4] = { 1.0, 1.0, 1.0, 1.0 };
  GLfloat mat_shininess[] = { 50.0 };
  GLfloat ambient_light[] = { 1.0, 1.0, 1.0, 1.0 };
  GLfloat mat_emission[] = { 0.2, 0.2, 0.4, 1.0 };
  
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient_light);
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse );
  //  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular );
  glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess );
  glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, mat_emission);

  glDepthFunc( GL_LEQUAL);
  glShadeModel( GL_FLAT);
  glEnable(GL_AUTO_NORMAL);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_POINT_SMOOTH);
  glEnable(GL_NORMALIZE);

  glColorMaterial(GL_FRONT_AND_BACK,GL_DIFFUSE);
  glEnable(GL_COLOR_MATERIAL);
}

static void enter_leave(int state)
{ glutPostRedisplay(); }

static void draw()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
  glPushMatrix();
 
  if (window_width > window_height) {
    double w = double( window_width) / double( window_height);
    if ( perspective) {
      double s = (eye - znear) / eye;
      glFrustum( -wsize*w*s, wsize*w*s, -wsize*s, wsize*s, 
		 eye-znear, eye+zfar);
      glTranslated( 0.0, 0.0, -eye);
    } else {
      glOrtho( -wsize*w, wsize*w, -wsize, wsize, -znear, zfar );
    }
  } else {
    double h = double( window_height) / double( window_width);
    if ( perspective) {
      double s = (eye - znear) / eye;
      glFrustum( -wsize*s, wsize*s, -wsize*h*s, wsize*h*s, 
		 eye-znear, eye+zfar);
      glTranslated( 0.0, 0.0, -eye);
    } else {
      glOrtho( -wsize, wsize, -wsize*h, wsize*h, -znear, zfar );
    }
  }

  glTranslated(dx,dy,dz);
  glTranslated(0,0,1);
  GLdouble M[16] = { rotation.m(0,0), rotation.m(1,0), rotation.m(2,0), 0.0,
                     rotation.m(0,1), rotation.m(1,1), rotation.m(2,1), 0.0,
                     rotation.m(0,2), rotation.m(1,2), rotation.m(2,2), 0.0,
                     rotation.m(0,3), rotation.m(1,3), rotation.m(2,3), 1.0};
  glMultMatrixd( M);
  
  glScaled(s,s,s);

 
  GLdouble z_vec[3] = { rotation.m(2,0) / s,
                        rotation.m(2,1) / s,
                        rotation.m(2,2) / s};
 
  //  GLdouble z_vec[3] = {1.0, 1.0, 1.0};
  int win = glutGetWindow();
  polyhedra_[win-1].draw( z_vec);
  glPopMatrix();
  glutSwapBuffers();
}

static void reshape(int width, int height)
{
  window_width  = width;
  window_height = height;
  window_radius = std::min( width, height) / 2;
  factor_s = std::log(2.0) / (window_radius/2.0); // radius == scale factor 2

  glViewport(0, 0, (GLint)width, (GLint)height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}


static void start_viewer()
{ 
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
  glutInitWindowSize(window_width, window_height);
  glutInitWindowPosition(0,0);

  int submenu1 = glutCreateMenu(show);
  glutAddMenuEntry("Reset",RESET_CONTROL);
  glutAddMenuEntry("Rotate",ROTATE);
  glutAddMenuEntry("Scale",SCALE);
  glutAddMenuEntry("Translate in XY",TRANSLATE);
  glutAddMenuEntry("Translate in Z",TRANS_Z);

  int submenu2 = glutCreateMenu(show);
  glutAddMenuEntry("Boundary",BOUNDARY);
  glutAddMenuEntry("Skeleton",SKELETON);
  glutAddMenuEntry("Toggle Axes",AXES);

  for (unsigned i = 0; i < polyhedra_.size(); ++i) {
    if (i > 0 ) glutInitWindowPosition(i*(window_width+12),0);
    if ( i < titles_.size() ) glutCreateWindow(titles_[i].c_str());
    else                      glutCreateWindow(" Polyhedron ");
    glutEntryFunc(enter_leave);
    init();
    glutDisplayFunc(draw);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutCreateMenu(show);
    glutAddSubMenu("Control",submenu1);
    glutAddSubMenu("Render",submenu2);
    glutAddMenuEntry("Persp/Ortho",PERSP);
    glutAddMenuEntry("Fullscreen",FULLSCREEN);
    glutAddMenuEntry("Quit",QUIT);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
  }
  glutMainLoop();
}

} // namespace OGL

// ----------------------------------------------------------------------------
// SNC_visualizor_OGL<SNC_>
// when you define a SNC_visualizor_OGL<SNC_> object then you create a
// viewer window that shows the corresponding SNC_ object. All such viewers
// follow the same visualization motion. Thus visualizing binary operations
// is easy: just display the two input SNCs and the output SNC
//
// for similar viewer code see Nef_S2/demo/Nef_S2/Nef_polyhedron_S2-demo.C
// ----------------------------------------------------------------------------



template <typename SNC_>
class SNC_visualizor_OGL : public SNC_decorator<SNC_>
{ typedef SNC_ SNC_structure;
  typedef CGAL::SNC_visualizor_OGL<SNC_> Self;
  typedef CGAL::SNC_decorator<SNC_>      Base;
  typedef CGAL::SNC_FM_decorator<SNC_>   FM_decorator;

  CGAL::OGL::Polyhedron* ppoly_;

public:
  #define USING(t) typedef typename SNC_::t t
  USING(Vertex_iterator); 
  USING(Halfedge_iterator); 
  USING(Halffacet_iterator); 
  USING(Halffacet_cycle_iterator);

  USING(Object_handle);
  USING(SObject_handle); 
  USING(SHalfedge_handle); 
  USING(SHalfloop_handle); 

  USING(Vertex_handle); 
  USING(Halfedge_handle); 
  USING(Halffacet_handle);

  USING(Point_3);
  USING(Vector_3);
  USING(Segment_3);
  USING(Plane_3);
  USING(Mark);
  USING(SHalfedge_around_facet_circulator);
  USING(SHalfedge_around_facet_const_circulator);
  #undef USING

  SNC_visualizor_OGL(const SNC_structure& N) : Base(const_cast<SNC_&>(N))
  { ppoly_ = & CGAL::OGL::add_polyhedron(); }

  OGL::Double_point double_point(const Point_3& p) const {
    double x = CGAL::to_double(p.hx().eval_at(1));
    double y = CGAL::to_double(p.hy().eval_at(1));
    double z = CGAL::to_double(p.hz().eval_at(1));
    double w = CGAL::to_double(p.hw().eval_at(1));
    return OGL::Double_point(x/w,y/w,z/w);
  }

  OGL::Double_segment double_segment(const Segment_3& s) const
  { return OGL::Double_segment(double_point(s.source()),
			       double_point(s.target())); 
  }

  void draw(Vertex_handle v) const
    { 
    Point_3 bp = Infi_box::box_point(point(v));
    TRACEN("vertex " << bp);
    ppoly_->push_back(double_point(bp), mark(v)); 
  }

  void draw(Halfedge_handle e) const
  { 
    Point_3 s = point(source(e));
    Point_3 t = point(target(e));
    Segment_3 seg(Infi_box::box_point(s),Infi_box::box_point(t));
    TRACEN("edge " << seg);
    ppoly_->push_back(double_segment(seg), mark(e)); 
  }

  void draw(Halffacet_handle f) const
  { OGL::DFacet g;
    FM_decorator D(*sncp(),f);
    Halffacet_cycle_iterator fc; // all facet cycles:
    for (fc = D.facet_cycles_begin(); fc != D.facet_cycles_end(); ++ fc)
      if ( fc.is_shalfedge() ) { // non-trivial facet cycle
	g.new_facet_cycle();
	SHalfedge_handle h = fc;
	SHalfedge_around_facet_circulator hc(h), he(hc);
	CGAL_For_all(hc,he){ // all vertex coordinates in facet cycle
	  Point_3 sp = Infi_box::box_point(point(source(hc)));
	      TRACEN(" ");TRACEN("facet" << sp);
	  g.push_back_vertex(double_point(sp));
	}
      }
    Vector_3 v = orthogonal_vector(f);
    double x = CGAL::to_double(v.hx().eval_at(1));
    double y = CGAL::to_double(v.hy().eval_at(1));
    double z = CGAL::to_double(v.hz().eval_at(1));
    double w = CGAL::to_double(v.hw().eval_at(1));    
    g.set_normal(x/w, y/w, z/w, mark(f));
    ppoly_->push_back(g);
  }

  void draw() const
  { 
    //    SETDTHREAD(53);
    Vertex_iterator v;
    CGAL_nef3_forall_vertices(v,*sncp()) draw(v);
    ppoly_->bbox() = sncp()->bounded_bbox();
    Halfedge_iterator e;
    CGAL_nef3_forall_edges(e,*sncp()) draw(e);
    Halffacet_iterator f;
    CGAL_nef3_forall_facets(f,*sncp()) draw(f);
  }

}; // SNC_visualizor_OGL<SNC_>





CGAL_END_NAMESPACE
#endif //CGAL_SNC_VISUALIZOR_OGL_H

