//#undef CGAL_LINKED_WITH_TBB

//#define CHECK_MEMORY_LEAKS_ON_MSVC

#if defined(CHECK_MEMORY_LEAKS_ON_MSVC) && defined(_MSC_VER)
  //#include <vld.h> // CJTODO: test it
  #define _CRTDBG_MAP_ALLOC
  #include <stdlib.h>
  #include <crtdbg.h>
#endif

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <ctime>

#include <boost/program_options.hpp>
namespace po = boost::program_options;

#ifdef WINVER
  const char * const BENCHMARK_CONFIG_FILENAME =
    "D:/INRIA/CGAL/workingcopy/Mesh_3/benchmark/Mesh_3/concurrency_config.cfg";

  const char * const BENCHMARK_SCRIPT_FILENAME =
    "D:/INRIA/CGAL/workingcopy/Mesh_3/benchmark/Mesh_3/concurrency_script.txt";
#else
  const char * const BENCHMARK_CONFIG_FILENAME =
    "/home/cjamin/CGAL/Mesh_3-parallel-cjamin/Mesh_3/benchmark/Mesh_3/concurrency_config.cfg";

  const char * const BENCHMARK_SCRIPT_FILENAME =
    "/home/cjamin/CGAL/Mesh_3-parallel-cjamin/Mesh_3/benchmark/Mesh_3/concurrency_script.txt";
#endif

// ==========================================================================
// BENCHMARK GENERAL PARAMETERS
// ==========================================================================

//#define MESH_3_POLYHEDRON_WITH_FEATURES
//#define MESH_3_IMPLICIT_WITH_FEATURES
//#define MESH_3_BENCHMARK_EXPORT_TO_MAYA
//#define MESH_3_BENCHMARK_EXPORT_TO_MESH

// ==========================================================================
// MESH_3 GENERAL PARAMETERS
// ==========================================================================

//#define CGAL_MESH_3_USE_OLD_SURFACE_RESTRICTED_DELAUNAY_UPDATE // WARNING: VERY SLOW
//#define CGAL_MESH_3_VERBOSE
//#define CGAL_MESH_3_VERY_VERBOSE
//#define CGAL_MESHES_DEBUG_REFINEMENT_POINTS
#define CGAL_MESH_3_INITIAL_POINTS_NO_RANDOM_SHOOTING

#define MESH_3_PROFILING
//#define CHECK_AND_DISPLAY_THE_NUMBER_OF_BAD_ELEMENTS_IN_THE_END

const int     FACET_ANGLE              = 25;
const double  FACET_APPROX             = 0.0068;
const double  FACET_APPROX_3D_IMAGES   = 0.5;
const int     TET_SHAPE                = 3;

// ==========================================================================
// CONCURRENCY
// ==========================================================================

#ifdef CONCURRENT_MESH_3

# ifndef CGAL_LINKED_WITH_TBB
#   Warning("CGAL_LINKED_WITH_TBB not defined: EVERYTHING WILL BE SEQUENTIAL.")
# endif

# define CGAL_MESH_3_USE_LAZY_SORTED_REFINEMENT_QUEUE
//# define CGAL_MESH_3_USE_LAZY_UNSORTED_REFINEMENT_QUEUE // CJTODO: TEST performance avec et sans
//# define CGAL_MESH_3_IF_UNSORTED_QUEUE_JUST_SORT_AFTER_SCAN

  // ==========================================================================
  // Concurrency activation
  // ==========================================================================

  // In case some code uses CGAL_PROFILE, it needs to be concurrent
# define CGAL_CONCURRENT_PROFILE
# define CGAL_CONCURRENT_MESH_3_VERBOSE
//# define CGAL_CONCURRENT_MESH_3_VERY_VERBOSE

  // ==========================================================================
  // Concurrency config
  // ==========================================================================

#ifdef WINVER
  const char * const CONFIG_FILENAME =
    "D:/INRIA/CGAL/workingcopy/Mesh_3/demo/Mesh_3/concurrent_mesher_config.cfg";
#else
  const char * const CONFIG_FILENAME =
    "/home/cjamin/CGAL/Mesh_3-parallel-cjamin/Mesh_3/demo/Mesh_3/concurrent_mesher_config.cfg";
#endif

# define CGAL_MESH_3_ADD_OUTSIDE_POINTS_ON_A_FAR_SPHERE
//# define CGAL_MESH_3_ACTIVATE_GRID_INDEX_CACHE_IN_VERTEX // DOES NOT WORK YET

  // =================
  // Locking strategy
  // =================

# define CGAL_MESH_3_LOCKING_STRATEGY_SIMPLE_GRID_LOCKING

//# define CGAL_MESH_3_CONCURRENT_REFINEMENT_LOCK_ADJ_CELLS // USELESS, FOR TESTS ONLY

  // =====================
  // Worksharing strategy
  // =====================

//# define CGAL_MESH_3_WORKSHARING_USES_PARALLEL_FOR
//# define CGAL_MESH_3_WORKSHARING_USES_PARALLEL_DO
# define CGAL_MESH_3_WORKSHARING_USES_TASK_SCHEDULER
# ifdef CGAL_MESH_3_WORKSHARING_USES_TASK_SCHEDULER
//#   define CGAL_MESH_3_TASK_SCHEDULER_WITH_LOCALIZATION_IDS
//#   ifdef CGAL_MESH_3_TASK_SCHEDULER_WITH_LOCALIZATION_IDS
//#     define CGAL_MESH_3_TASK_SCHEDULER_WITH_LOCALIZATION_IDS_SHARED // optional
//#   endif
//#   define CGAL_MESH_3_LOAD_BASED_WORKSHARING // Not recommended
//#   define CGAL_MESH_3_TASK_SCHEDULER_SORTED_BATCHES_WITH_MULTISET
#   define CGAL_MESH_3_TASK_SCHEDULER_SORTED_BATCHES_WITH_SORT
# endif

  // ==========================================================================
  // Profiling
  // ==========================================================================

  // For profiling, etc.
# define CGAL_CONCURRENT_MESH_3_PROFILING

  // ==========================================================================
  // TBB
  // ==========================================================================

# include <tbb/tbb.h>
# include <tbb/compat/thread>
# ifndef _DEBUG
    // Use TBB malloc proxy (for all new/delete/malloc/free calls)
    // Highly recommended
#   include <tbb/tbbmalloc_proxy.h>
# endif


// ==========================================================================
// SEQUENTIAL
// ==========================================================================

#else // !CONCURRENT_MESH_3

# define CGAL_MESH_3_USE_LAZY_SORTED_REFINEMENT_QUEUE
//# define CGAL_MESH_3_USE_LAZY_UNSORTED_REFINEMENT_QUEUE
# define CGAL_MESH_3_IF_UNSORTED_QUEUE_JUST_SORT_AFTER_SCAN

  // For better performance on meshes like fandisk
# define CGAL_MESH_3_ADD_OUTSIDE_POINTS_ON_A_FAR_SPHERE

#endif // CONCURRENT_MESH_3

// ==========================================================================
// ==========================================================================

#ifdef WINVER
  const char * const DEFAULT_INPUT_FILE_NAME = "D:/INRIA/CGAL/workingcopy/Mesh_3/examples/Mesh_3/data/elephant.off";
#else
  const char * const DEFAULT_INPUT_FILE_NAME = "/home/cjamin/CGAL/Mesh_3-parallel-cjamin/Mesh_3/examples/Mesh_3/data/elephant.off";
#endif

// ==========================================================================
// ==========================================================================

#include "../../test/Mesh_3/XML_exporter.h"
#define CGAL_MESH_3_EXPORT_PERFORMANCE_DATA
#define CGAL_MESH_3_SET_PERFORMANCE_DATA(value_name, value) \
        XML_perf_data::set(value_name, value);

class XML_perf_data
{
public:
  typedef Streaming_XML_exporter<std::string> XML_exporter;

  XML_perf_data(const std::string &filename)
    : m_xml(filename, "ContainerPerformance", "Perf",
            construct_subelements_names())
  {}

  ~XML_perf_data()
  {
  }

  static XML_perf_data &get()
  {
    static XML_perf_data singleton(build_filename());
    return singleton;
  }

  template <typename Value_type>
  static void set(const std::string &name, Value_type value)
  {
    get().set_data(name, value);
  }

  static void commit()
  {
    get().commit_current_element();
  }

protected:
  static std::string build_filename()
  {
    std::stringstream sstr;
    sstr << "Performance_log_" << time(0) << ".xml";
    return sstr.str();
  }

  static std::vector<std::string> construct_subelements_names()
  {
    std::vector<std::string> subelements;
    subelements.push_back("Domain");
    subelements.push_back("Facet_angle");
    subelements.push_back("Facet_size");
    subelements.push_back("Facet_approx");
    subelements.push_back("Cell_size");
    subelements.push_back("Cell_shape");
    subelements.push_back("Technique");
    subelements.push_back("Num_threads");
    subelements.push_back("Lockgrid_size");
    subelements.push_back("Lock_radius");
    subelements.push_back("Statgrid_size");
    subelements.push_back("Num_work_items_per_batch");
    subelements.push_back("V");
    subelements.push_back("F");
    subelements.push_back("T");
    subelements.push_back("Facets_time");
    subelements.push_back("Cells_scan_time");
    subelements.push_back("Cells_refin_time");

    return subelements;
  }

  void set_data(const std::string &name, const std::string &value)
  {
    m_current_element[name] = value;
  }

  template <typename Value_type>
  void set_data(const std::string &name, Value_type value)
  {
    std::stringstream sstr;
    sstr << value;
    set_data(name, sstr.str());
  }

  void commit_current_element()
  {
    m_xml.add_element(m_current_element);
    m_current_element.clear();
  }

  XML_exporter m_xml;
  XML_exporter::Element_with_map m_current_element;
};


// ==========================================================================
// ==========================================================================



#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

#include <CGAL/Mesh_triangulation_3.h>
#include <CGAL/Mesh_complex_3_in_triangulation_3.h>
#include <CGAL/Mesh_criteria_3.h>

#include <CGAL/Polyhedral_mesh_domain_3.h>
#include <CGAL/Polyhedral_mesh_domain_with_features_3.h>
#include <CGAL/Implicit_mesh_domain_3.h>
#include <CGAL/Mesh_domain_with_polyline_features_3.h>
#include <CGAL/Labeled_image_mesh_domain_3.h>

#include <CGAL/make_mesh_3.h>
#include <CGAL/refine_mesh_3.h>

#ifdef MESH_3_POLYHEDRON_WITH_FEATURES
# include <CGAL/Mesh_polyhedron_3.h>
#endif

// basic types from kernel
typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel;
typedef Kernel::FT FT;
typedef Kernel::Point_3 Point;
typedef Kernel::Sphere_3 Sphere;
// IO
#include <CGAL/IO/Polyhedron_iostream.h>

// To avoid verbose function and named parameters call
using namespace CGAL::parameters;

struct Mesh_parameters
{
  double facet_angle;
  double facet_sizing;
  double facet_approx;

  double tet_shape;
  double tet_sizing;

  std::string log() const
  {
    std::stringstream sstr;
    sstr
      << " * facet min angle: " << facet_angle << std::endl
      << " * facet max size: " << facet_sizing << std::endl
      << " * facet approx error: " << facet_approx << std::endl
      << " * tet shape (radius-edge): " << tet_shape << std::endl
      << " * tet max size: " << tet_sizing << std::endl;

    return sstr.str();
  }
};

struct Klein_function
{
  typedef ::FT           FT;
  typedef ::Point        Point;

  FT operator()(const Point& query) const
  {
	  const FT x = query.x();
	  const FT y = query.y();
	  const FT z = query.z();

    return   (x*x+y*y+z*z+2*y-1)
           * ( (x*x+y*y+z*z-2*y-1) *(x*x+y*y+z*z-2*y-1)-8*z*z)
           + 16*x*z* (x*x+y*y+z*z-2*y-1);
  }
};

struct Tanglecube_function
{
  typedef ::FT           FT;
  typedef ::Point        Point;

  FT operator()(const Point& query) const
  {
	  const FT x = query.x();
	  const FT y = query.y();
	  const FT z = query.z();

    double x2=x*x, y2=y*y, z2=z*z;
    double x4=x2*x2, y4=y2*y2, z4=z2*z2;
    return x4 - 5*x2 + y4 - 5*y2 + z4 - 5*z2 + 11.8;
  }
};

struct Sphere_function
{
  typedef ::FT           FT;
  typedef ::Point        Point;

  Sphere_function(double radius = 1.)
    : m_squared_radius(radius*radius)
  {}

  FT operator()(const Point& query) const
  {
	  const FT x = query.x();
	  const FT y = query.y();
	  const FT z = query.z();

    return (x*x + y*y + z*z - m_squared_radius);
  }

protected:
  FT m_squared_radius;
};

/*const double PANCAKE_HEIGHT = 0.1;
const double PANCAKE_RADIUS = 3.;

struct Pancake_function
{
  typedef ::FT           FT;
  typedef ::Point        Point;

  FT operator()(const Point& query) const
  {
	  const FT x = query.x();
	  const FT y = query.y();
	  const FT z = query.z();

    if (z > 0.5*PANCAKE_HEIGHT)
      return z - 0.5*PANCAKE_HEIGHT;
    else if (z < -0.5*PANCAKE_HEIGHT)
      return -z + 0.5*PANCAKE_HEIGHT;
    else
      return (x*x + y*y - PANCAKE_RADIUS*PANCAKE_RADIUS);
  }
};*/

/*const double THIN_CYLINDER_HEIGHT = 3.;
const double THIN_CYLINDER_RADIUS = 0.05;

struct Thin_cylinder_function
{
  typedef ::FT           FT;
  typedef ::Point        Point;


  FT operator()(const Point& query) const
  {
	  const FT x = query.x();
	  const FT y = query.y();
	  const FT z = query.z();

    if (z > 0.5*THIN_CYLINDER_HEIGHT)
      return z - 0.5*THIN_CYLINDER_HEIGHT;
    else if (z < -0.5*THIN_CYLINDER_HEIGHT)
      return -z + 0.5*THIN_CYLINDER_HEIGHT;
    else
      return (x*x + y*y - THIN_CYLINDER_RADIUS*THIN_CYLINDER_RADIUS);
  }
};*/

struct Cylinder_function
{
  typedef ::FT           FT;
  typedef ::Point        Point;

  Cylinder_function(double radius = 0.5, double height = 2.)
    : m_radius(radius), m_height(height)
  {}

  FT operator()(const Point& query) const
  {
	  const FT x = query.x();
	  const FT y = query.y();
	  const FT z = query.z();

    if (z > 0.5*m_height)
      return z - 0.5*m_height;
    else if (z < -0.5*m_height)
      return -z + 0.5*m_height;
    else
      return (x*x + y*y - m_radius*m_radius);
  }

protected:
  FT m_radius;
  FT m_height;
};


std::string get_technique()
{
  std::string tech;
#ifdef CONCURRENT_MESH_3

# if defined(CGAL_MESH_3_WORKSHARING_USES_TASK_SCHEDULER)
  tech += "Task-scheduler (auto";
#   ifdef CGAL_MESH_3_TASK_SCHEDULER_SORTED_BATCHES_WITH_SORT
    tech += ", sorted batches with std::sort";
#   elif defined(CGAL_MESH_3_TASK_SCHEDULER_SORTED_BATCHES_WITH_MULTISET)
    tech += ", sorted batches with multiset";
#   elif defined(CGAL_MESH_3_TASK_SCHEDULER_WITH_LOCALIZATION_IDS)
    tech += ", with localization ids";
#   elif defined(CGAL_MESH_3_LOAD_BASED_WORKSHARING)
    tech += ", load-based worksharing";
#endif
  tech += ")";

# elif defined(CGAL_MESH_3_WORKSHARING_USES_PARALLEL_FOR)
  tech += "Parallel_for";
# elif defined(CGAL_MESH_3_WORKSHARING_USES_PARALLEL_DO)
  tech += "Parallel_do";
# else
  tech += "Unknown parallel technique";
# endif

#else // !CONCURRENT_MESH_3

  tech += "Sequential ";
# if defined(CGAL_MESH_3_USE_LAZY_UNSORTED_REFINEMENT_QUEUE)
#   ifdef CGAL_MESH_3_IF_UNSORTED_QUEUE_JUST_SORT_AFTER_SCAN
  tech += "(sort after scan only";
#   else
  tech += "(unsorted";
#   endif
# elif defined(CGAL_MESH_3_USE_LAZY_SORTED_REFINEMENT_QUEUE)
  tech += "(sorted";
# else
  tech += "(NOT LAZY, sorted";
# endif

#ifdef CGAL_MESH_3_ADD_OUTSIDE_POINTS_ON_A_FAR_SPHERE
  tech += ", points on far sphere)";
#else
  tech += ")";
#endif

#endif // CONCURRENT_MESH_3

  return tech;
}

void xml_perf_set_technique()
{
  CGAL_MESH_3_SET_PERFORMANCE_DATA("Technique", get_technique());
}


void display_info(int num_threads)
{
  std::cerr << get_technique() << std::endl;

#ifdef CONCURRENT_MESH_3

  if (num_threads != -1)
    std::cerr << "Num threads = " << num_threads << std::endl;
  else
    std::cerr << "Num threads = AUTO" << std::endl;

#else // !CONCURRENT_MESH_3

# ifdef CGAL_MESH_3_INITIAL_POINTS_NO_RANDOM_SHOOTING
  std::cerr << "NO random shooting)" << std::endl;
# else
  std::cerr << "WITH random shooting)" << std::endl;
# endif

#endif // CONCURRENT_MESH_3
}

// To add a crease (feature)
typedef std::vector<Point> Crease;
typedef std::list<Crease> Creases;
void add_crease(const Point& a,
	const Point& b,
	Creases& creases)
{
  Crease crease;
	crease.push_back(a);
	crease.push_back(b);
	creases.push_back(crease);
}

bool make_mesh_polyhedron(const std::string &input_filename,
                 double facet_sizing,
                 double cell_sizing)
{
  // Domain
  typedef Kernel K;
  
#ifdef MESH_3_POLYHEDRON_WITH_FEATURES
  typedef CGAL::Mesh_polyhedron_3<K>::type Polyhedron;
  typedef CGAL::Polyhedral_mesh_domain_with_features_3<K> Mesh_domain;
#else
  typedef CGAL::Polyhedron_3<K> Polyhedron;
  typedef CGAL::Polyhedral_mesh_domain_3<Polyhedron, K> Mesh_domain;
#endif

  // Triangulation
#ifdef CONCURRENT_MESH_3
  typedef CGAL::Parallel_mesh_triangulation_3<Mesh_domain>::type Tr;
#else
  typedef CGAL::Mesh_triangulation_3<Mesh_domain>::type Tr;
#endif
  // C3t3
  typedef CGAL::Mesh_complex_3_in_triangulation_3<Tr> C3t3;
  // Criteria
  typedef CGAL::Mesh_criteria_3<Tr> Mesh_criteria;

  // Create input polyhedron
  Polyhedron polyhedron;
  std::ifstream input(input_filename.c_str());
  if (!input.is_open())
  {
    std::cerr << "Could not open '" << input_filename << "'" << std::endl;
    return false;
  }
  input >> polyhedron;
  
  std::cerr << "Building AABB tree... ";
  // Create domain
  Mesh_domain domain(polyhedron);
  std::cerr << "done." << std::endl;

#ifdef MESH_3_POLYHEDRON_WITH_FEATURES
  std::cerr << "Detecting features... ";
  domain.detect_features();
  std::cerr << "done." << std::endl;
#endif

  Mesh_parameters params;
  params.facet_angle = FACET_ANGLE;
  params.facet_sizing = facet_sizing;
  params.facet_approx = FACET_APPROX;
  params.tet_sizing = cell_sizing;
  params.tet_shape = TET_SHAPE;

  std::cerr
    << "File: " << input_filename << std::endl
    << "Parameters: " << std::endl
    << params.log() << std::endl;

  // Mesh criteria (no cell_size set)
  Mesh_criteria criteria(
    edge_size=params.facet_sizing,
    facet_angle=params.facet_angle,
    facet_size=params.facet_sizing,
    facet_distance=params.facet_approx,
    cell_size=params.tet_sizing,
    cell_radius_edge_ratio=params.tet_shape
  );

  // Mesh generation
  C3t3 c3t3 = CGAL::make_mesh_3<C3t3>(domain, criteria, no_perturb(), no_exude());

  CGAL_MESH_3_SET_PERFORMANCE_DATA("V", c3t3.triangulation().number_of_vertices());
  CGAL_MESH_3_SET_PERFORMANCE_DATA("F", c3t3.number_of_facets_in_complex());
  CGAL_MESH_3_SET_PERFORMANCE_DATA("T", c3t3.number_of_cells_in_complex());

#ifdef MESH_3_BENCHMARK_EXPORT_TO_MAYA
  std::cerr << "Exporting to maya file format (*.maya)... ";
  c3t3.output_to_maya(std::ofstream(input_filename + ".maya"), true);
  std::cerr << "done." << std::endl;
#endif
  
#ifdef MESH_3_BENCHMARK_EXPORT_TO_MESH
  std::cerr << "Exporting to Medit file format (*.mesh)... ";
  c3t3.output_to_medit(std::ofstream(input_filename + ".mesh"), true);
  std::cerr << "done." << std::endl;
#endif

  return true;
}


bool make_mesh_3D_images(const std::string &input_filename,
                 double facet_sizing,
                 double cell_sizing)
{
  // Domain
  typedef Kernel K;
  
  typedef CGAL::Labeled_image_mesh_domain_3<CGAL::Image_3, K> Mesh_domain;

  // Triangulation
#ifdef CONCURRENT_MESH_3
  typedef CGAL::Parallel_mesh_triangulation_3<Mesh_domain>::type Tr;
#else
  typedef CGAL::Mesh_triangulation_3<Mesh_domain>::type Tr;
#endif
  // C3t3
  typedef CGAL::Mesh_complex_3_in_triangulation_3<Tr> C3t3;
  // Criteria
  typedef CGAL::Mesh_criteria_3<Tr> Mesh_criteria;

  // Load image
  CGAL::Image_3 image;
  image.read(input_filename.c_str());
  
  // Create domain
  Mesh_domain domain(image);
  std::cerr << "done." << std::endl;
  
  Mesh_parameters params;
  params.facet_angle = FACET_ANGLE;
  params.facet_sizing = facet_sizing;
  params.facet_approx = FACET_APPROX_3D_IMAGES;
  params.tet_sizing = cell_sizing;
  params.tet_shape = TET_SHAPE;

  std::cerr
    << "File: " << input_filename << std::endl
    << "Parameters: " << std::endl
    << params.log() << std::endl;

  // Mesh criteria (no cell_size set)
  Mesh_criteria criteria(
    edge_size=params.facet_sizing,
    facet_angle=params.facet_angle,
    facet_size=params.facet_sizing,
    facet_distance=params.facet_approx,
    cell_size=params.tet_sizing,
    cell_radius_edge_ratio=params.tet_shape
  );

  // Mesh generation
  C3t3 c3t3 = CGAL::make_mesh_3<C3t3>(domain, criteria, no_perturb(), no_exude());

  CGAL_MESH_3_SET_PERFORMANCE_DATA("V", c3t3.triangulation().number_of_vertices());
  CGAL_MESH_3_SET_PERFORMANCE_DATA("F", c3t3.number_of_facets_in_complex());
  CGAL_MESH_3_SET_PERFORMANCE_DATA("T", c3t3.number_of_cells_in_complex());

#ifdef MESH_3_BENCHMARK_EXPORT_TO_MAYA
  std::cerr << "Exporting to maya file format (*.maya)... ";
  c3t3.output_to_maya(std::ofstream(input_filename + ".maya"), true);
  std::cerr << "done." << std::endl;
#endif
  
#ifdef MESH_3_BENCHMARK_EXPORT_TO_MESH
  std::cerr << "Exporting to Medit file format (*.mesh)... ";
  c3t3.output_to_medit(std::ofstream(input_filename + ".mesh"), true);
  std::cerr << "done." << std::endl;
#endif

  return true;
}


template <class ImplicitFunction>
bool make_mesh_implicit(double facet_sizing, double cell_sizing, ImplicitFunction func, 
                        const std::string &function_name)
{
  // Domain
#ifdef MESH_3_IMPLICIT_WITH_FEATURES
  typedef CGAL::Implicit_mesh_domain_3<const ImplicitFunction, Kernel> Implicit_domain;
  typedef CGAL::Mesh_domain_with_polyline_features_3<Implicit_domain> Mesh_domain;
#else
  typedef CGAL::Implicit_mesh_domain_3<ImplicitFunction, Kernel> Mesh_domain;
#endif

  // Triangulation
#ifdef CONCURRENT_MESH_3
  typedef typename CGAL::Parallel_mesh_triangulation_3<Mesh_domain>::type Tr;
#else
  typedef typename CGAL::Mesh_triangulation_3<Mesh_domain>::type Tr;
#endif
  // C3t3
  typedef CGAL::Mesh_complex_3_in_triangulation_3<Tr> C3t3;
  // Criteria
  typedef CGAL::Mesh_criteria_3<Tr> Mesh_criteria;

  // Create domain
	Sphere bounding_sphere(CGAL::ORIGIN, 10.0 * 10.0);
  Mesh_domain domain(func, bounding_sphere/*, 1e-7*/);

#ifdef MESH_3_IMPLICIT_WITH_FEATURES
	// Add 12 feature creases
	Creases creases;
	Point p1(-1.0, -1.0, -1.0);
	Point p2(-1.0, -1.0,  1.0);
	Point p3(-1.0,  1.0,  1.0);
	Point p4(-1.0,  1.0, -1.0);
	Point p5( 1.0, -1.0, -1.0);
	Point p6( 1.0, -1.0,  1.0);
	Point p7( 1.0,  1.0,  1.0);
	Point p8( 1.0,  1.0, -1.0);

	add_crease(p1, p2, creases);
	add_crease(p2, p3, creases);
	add_crease(p3, p4, creases);
	add_crease(p4, p1, creases);

	add_crease(p5, p6, creases);
	add_crease(p6, p7, creases);
	add_crease(p7, p8, creases);
	add_crease(p8, p5, creases);

	add_crease(p5, p1, creases);
	add_crease(p6, p2, creases);
	add_crease(p7, p3, creases);
	add_crease(p8, p4, creases);

	domain.add_features(creases.begin(), creases.end());
#endif

  Mesh_parameters params;
  params.facet_angle = FACET_ANGLE;
  params.facet_sizing = facet_sizing;
  params.facet_approx = FACET_APPROX;
  params.tet_sizing = cell_sizing;
  params.tet_shape = TET_SHAPE;

  std::cerr
    << "Implicit function" << std::endl
    << "Parameters: " << std::endl
    << params.log() << std::endl;

  // Mesh criteria (no cell_size set)
  Mesh_criteria criteria(
    edge_size=params.facet_sizing,
    facet_angle=params.facet_angle,
    facet_size=params.facet_sizing,
    facet_distance=params.facet_approx,
    cell_size=params.tet_sizing,
    cell_radius_edge_ratio=params.tet_shape
  );

  // Mesh generation
  C3t3 c3t3 = CGAL::make_mesh_3<C3t3>(domain, criteria, no_perturb(), no_exude());

  CGAL_MESH_3_SET_PERFORMANCE_DATA("V", c3t3.triangulation().number_of_vertices());
  CGAL_MESH_3_SET_PERFORMANCE_DATA("F", c3t3.number_of_facets_in_complex());
  CGAL_MESH_3_SET_PERFORMANCE_DATA("T", c3t3.number_of_cells_in_complex());
  
#ifdef MESH_3_BENCHMARK_EXPORT_TO_MAYA
  std::cerr << "Exporting to maya file format (*.maya)... ";
  c3t3.output_to_maya(std::ofstream(function_name + ".maya"), true);
  std::cerr << "done." << std::endl;
#endif

#ifdef MESH_3_BENCHMARK_EXPORT_TO_MESH
  std::cerr << "Exporting to Medit file format (*.mesh)... ";
  c3t3.output_to_medit(std::ofstream(function_name + ".mesh"), true);
  std::cerr << "done." << std::endl;
#endif

  return true;
}

int main()
{
  // Program options
  po::variables_map vm;
  try
  {
    // Declare the supported options.
    po::options_description desc("Allowed options");
    desc.add_options()
      ("filename", po::value<std::string>()->default_value(DEFAULT_INPUT_FILE_NAME), "")
      ("facet_sizing", po::value<double>()->default_value(0.005), "")
      ("cell_sizing", po::value<double>()->default_value(0.005), "")
      ("numthreads", po::value<int>()->default_value(-1), "");

    std::ifstream in(BENCHMARK_CONFIG_FILENAME);
    po::store(po::parse_config_file<char>(in, desc), vm);
    po::notify(vm);
  }
  catch (std::exception &e)
  {
    std::cerr << "Config file error: " << e.what() << std::endl;
    return false;
  }
  int num_threads = vm["numthreads"].as<int>();
  double facet_sizing = vm["facet_sizing"].as<double>();
  double cell_sizing = vm["cell_sizing"].as<double>();
  std::string filename = vm["filename"].as<std::string>();

#ifdef CONCURRENT_MESH_3
  Concurrent_mesher_config::load_config_file(CONFIG_FILENAME, true);

  tbb::task_scheduler_init init(tbb::task_scheduler_init::deferred);
  if (num_threads > 0)
    init.initialize(num_threads);
  else
    init.initialize();

#endif

  std::ifstream script_file;
  script_file.open(BENCHMARK_SCRIPT_FILENAME);
  // Script?
  // Script file format: each line gives
  //    - Filename (polyhedron) or "XXX_function" (implicit)
  //    - Facet sizing
  //    - Cell sizing
  //    - Number of iterations with these parameters
  if (script_file.is_open())
  {
    int i = 1;
#ifdef CONCURRENT_MESH_3
    //for(num_threads = 1 ; num_threads <= 12 ; ++num_threads)
    /*for (Concurrent_mesher_config::get().num_work_items_per_batch = 5 ;
      Concurrent_mesher_config::get().num_work_items_per_batch < 100 ;
      Concurrent_mesher_config::get().num_work_items_per_batch += 5)*/
#endif
    {
#ifdef CONCURRENT_MESH_3
      init.terminate();
      if (num_threads > 0)
        init.initialize(num_threads);
      else
        init.initialize();
#endif

      std::cerr << "Script file '" << BENCHMARK_SCRIPT_FILENAME << "' found." << std::endl;
      script_file.seekg(0);
      while (script_file.good())
      {
        std::string line;
        std::getline(script_file, line);
        if (line.size() > 1 && line[0] != '#')
        {
          std::cerr << std::endl << std::endl;
          std::cerr << "*****************************************" << std::endl;
          std::cerr << "******* " << line << std::endl;
          std::cerr << "*****************************************" << std::endl;
          std::stringstream sstr(line);

          std::string input;
          double facet_sizing;
          double cell_sizing;
          int num_iteration;
          sstr >> input;
          sstr >> facet_sizing;
          sstr >> cell_sizing;
          sstr >> num_iteration;

          for (int j = 0 ; j < num_iteration ; ++j)
          {
            std::string domain = input;
            size_t slash_index = domain.find_last_of('/');
            if (slash_index == std::string::npos)
              slash_index = domain.find_last_of('\\');
            if (slash_index == std::string::npos)
              slash_index = 0;
            else
              ++slash_index;
            domain = domain.substr(
              slash_index, domain.find_last_of('.') - slash_index);

            CGAL_MESH_3_SET_PERFORMANCE_DATA("Domain", domain);
            CGAL_MESH_3_SET_PERFORMANCE_DATA("Facet_angle", FACET_ANGLE);
            CGAL_MESH_3_SET_PERFORMANCE_DATA("Facet_size", facet_sizing);
            CGAL_MESH_3_SET_PERFORMANCE_DATA("Facet_approx", FACET_APPROX);
            CGAL_MESH_3_SET_PERFORMANCE_DATA("Cell_size", cell_sizing);
            CGAL_MESH_3_SET_PERFORMANCE_DATA("Cell_shape", TET_SHAPE);
            xml_perf_set_technique();
#ifdef CONCURRENT_MESH_3
            CGAL_MESH_3_SET_PERFORMANCE_DATA(
              "Num_threads",
              (num_threads == -1 ? std::thread::hardware_concurrency() : num_threads));
            CGAL_MESH_3_SET_PERFORMANCE_DATA(
              "Lockgrid_size",
              Concurrent_mesher_config::get().locking_grid_num_cells_per_axis);
            CGAL_MESH_3_SET_PERFORMANCE_DATA(
              "Lock_radius",
              Concurrent_mesher_config::get().first_grid_lock_radius);
            CGAL_MESH_3_SET_PERFORMANCE_DATA(
              "Statgrid_size",
              Concurrent_mesher_config::get().work_stats_grid_num_cells_per_axis);
            CGAL_MESH_3_SET_PERFORMANCE_DATA(
              "Num_work_items_per_batch",
              Concurrent_mesher_config::get().num_work_items_per_batch);
#else
            CGAL_MESH_3_SET_PERFORMANCE_DATA("Num_threads", "N/A");
            CGAL_MESH_3_SET_PERFORMANCE_DATA("Lockgrid_size", "N/A");
            CGAL_MESH_3_SET_PERFORMANCE_DATA("Lock_radius", "N/A");
            CGAL_MESH_3_SET_PERFORMANCE_DATA("Statgrid_size", "N/A");
            CGAL_MESH_3_SET_PERFORMANCE_DATA("Num_work_items_per_batch", "N/A");
#endif

            std::cerr << std::endl << "Refinement #" << i << "..." << std::endl;
            
            display_info(num_threads);

            if (input == "Klein_function")
              make_mesh_implicit(facet_sizing, cell_sizing, Klein_function(), input);
            else if (input == "Tanglecube_function")
              make_mesh_implicit(facet_sizing, cell_sizing, Tanglecube_function(), input);
            /*else if (input == "Sphere_function")
              make_mesh_implicit(facet_sizing, cell_sizing, Sphere_function(1.), input);
            else if (input == "Thin_cylinder_function")
            {
              Cylinder_function f(0.05, 3.);
              make_mesh_implicit(facet_sizing, cell_sizing, f, input);
            }
            else if (input == "Pancake_function")
            {
              Cylinder_function f(3., 0.1);
              make_mesh_implicit(facet_sizing, cell_sizing, f, input);
            }*/
            else 
            {
              size_t dot_position = input.find_last_of('.');
              std::string extension = input.substr(dot_position + 1);
              if (extension == "off")
                make_mesh_polyhedron(input, facet_sizing, cell_sizing);
              else if (extension == "inr")
                make_mesh_3D_images(input, facet_sizing, cell_sizing);
            }

            std::cerr << "Refinement #" << i++ << " done." << std::endl;
            std::cerr << std::endl << "---------------------------------" << std::endl << std::endl;

            XML_perf_data::commit();
          }
        }
      }
      script_file.seekg(0);
      script_file.clear();
    }

    script_file.close();
  }
  // Or not script?
  else
  {
    std::cerr << "Script file '" << BENCHMARK_SCRIPT_FILENAME << "' NOT found." << std::endl;
    for(int i = 1 ; ; ++i)
    {
      std::cerr << "Refinement #" << i << "..." << std::endl;
      display_info(num_threads);
      //make_mesh_polyhedron(filename, facet_sizing, cell_sizing);
      make_mesh_implicit(facet_sizing, cell_sizing, Klein_function(), "Klein_function");
      std::cerr << "Refinement #" << i << " done." << std::endl;
      std::cerr << std::endl << "---------------------------------" << std::endl << std::endl;
    }
  }

#if defined(CHECK_MEMORY_LEAKS_ON_MSVC) && defined(_MSC_VER)
  _CrtDumpMemoryLeaks();
#endif

  return 0;
}
