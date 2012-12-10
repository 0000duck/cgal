
namespace CGAL {
namespace Kinetic {

/*!
\ingroup PkgKdsSupport

The class `Kinetic::Qt_widget_2` implements a graphical interface for 2D kinetic data structures. 

\code{.cpp} 

#include <CGAL/basic.h> 
#include <CGAL/Kinetic/Exact_simulation_traits.h> 
#include <CGAL/Kinetic/Delaunay_triangulation_2.h> 
#include <CGAL/Kinetic/Delaunay_triangulation_vertex_base_2.h> 
#include <CGAL/Kinetic/Delaunay_triangulation_recent_edges_visitor_2.h> 
#include <CGAL/Kinetic/Enclosing_box_2.h> 
#include <CGAL/Kinetic/IO/Qt_moving_points_2.h> 
#include <CGAL/Kinetic/IO/Qt_triangulation_2.h> 
#include <CGAL/Kinetic/IO/Qt_widget_2.h> 
#include <CGAL/Kinetic/Insert_event.h> 

int main(int argc, char *argv[]) 
{ 
  typedef CGAL::Kinetic::Exact_simulation_traits Traits; 

  typedef CGAL::Triangulation_data_structure_2< 
    CGAL::Kinetic::Delaunay_triangulation_vertex_base_2<Traits::Instantaneous_kernel>, 
    CGAL::Kinetic::Delaunay_triangulation_face_base_2<Traits> > TDS; 
  typedef CGAL::Delaunay_triangulation_2<Traits::Instantaneous_kernel, TDS > Del; 
  // Color edges based on how recently they were created 
  typedef CGAL::Kinetic::Delaunay_triangulation_recent_edges_visitor_2<Del> Visitor; 
  typedef CGAL::Kinetic::Delaunay_triangulation_2<Traits, Visitor, Del> KDel; 
  typedef CGAL::Kinetic::Qt_widget_2<Traits::Simulator> Qt_gui; 
  typedef CGAL::Kinetic::Qt_moving_points_2<Traits, Qt_gui> Qt_mps; 
  typedef CGAL::Kinetic::Qt_triangulation_2<KDel, Traits::Instantaneous_kernel, Qt_gui> Qt_triangulation; 
  // Keeps the points confined 
  typedef CGAL::Kinetic::Enclosing_box_2<Traits> Box; 

  CGAL_SET_LOG_LEVEL(CGAL::Log::LOTS); 

  // Run from time 0 until a really large value of time 
  Traits tr(0, 10000000); 
  Box::Handle box= new Box(tr); 
  KDel::Handle kdel= new KDel(tr); 

  Qt_gui::Handle qtsim= new Qt_gui(argc, argv, tr.simulator_handle()); 

  Qt_mps::Handle qtmps= new Qt_mps(qtsim, tr); 
  Qt_triangulation::Handle qtdel= new Qt_triangulation(kdel, tr.instantaneous_kernel_object(), qtsim); 
  Traits::Kinetic_kernel::Point_2 p; 
  std::cin >> p; 
  tr.active_points_2_table_handle()->insert(p); 
  return qtsim->begin_event_loop(); 
} 

\endcode 

*/
template< typename Simulator >
class Qt_widget_2 {
public:

/// \name Types 
/// @{

/*!
The listener base to listen for when to update the picture. This
class includes an extra method `Qt_widget widget()` which returns the
`Qt_widget` object which can be used for drawing.
*/ 
typedef Hidden_type Listener; 

/// @} 

/// \name Creation 
/// @{

/*! 
default constructor. 
*/ 
Qt_widget_2(int argc, char *argv[], Simulator::Handle); 

/// @}

}; /* end Kinetic::Qt_widget_2 */
} /* end namespace Kinetic */
} /* end namespace CGAL */
