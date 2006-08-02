#include <CGAL/basic.h>

#ifdef CGAL_USE_GMP
  // GMP is installed. Use the GMP rational number-type. 
  #include <CGAL/Gmpq.h>
  typedef CGAL::Gmpq                                    NT;
#else
  // GMP is not installed. Use CGAL's exact rational number-type.
  #include <CGAL/MP_Float.h>
  #include <CGAL/Quotient.h>
  typedef CGAL::Quotient<CGAL::MP_Float>                NT;
#endif

#include <CGAL/Cartesian.h>
#include <CGAL/Arr_segment_traits_2.h>
#include <CGAL/Env_default_diagram_1.h>
#include <CGAL/envelope_2.h>

#include <list>
#include <iostream>

typedef CGAL::Gmpq                                      NT;
typedef CGAL::Cartesian<NT>                             Kernel;
typedef CGAL::Arr_segment_traits_2<Kernel>              Traits_2;
typedef Traits_2::Point_2                               Point_2;
typedef Traits_2::Curve_2                               Segment_2;
typedef CGAL::Env_default_diagram_1<Traits_2>           Diagram_1;
typedef std::list<Segment_2>                            Segment_list;

enum Coord_input_format
{
  F_RATIONAL,            // Coordinates given as rational numbers.
  F_INTEGER,             // Coordinates given as integers.
  F_DOUBLE               // Coordinates given as doubles (with decimal points).
};

/*!
 * Read a set of line segments from an input file.
 * \param filename The name of the input file.
 * \param format The coordinate format.
 * \param segs Output: The segments.
 * \return Whether the segments were successfuly read.
 */
bool read_segments (const char *filename,
                    Coord_input_format format,
                    Segment_list& segs)
{
  segs.clear();

  // Open the input file.
  std::ifstream          ifile (filename);

  if (! ifile.is_open())
  {
    std::cerr << "Failed to open <" << filename << ">." << std::endl;
    return (false);
  }

  // Read the segments.
  int                    n_segments;
  int                    ix1, iy1, ix2, iy2;
  double                 dx1, dy1, dx2, dy2;
  const int              denom = 100000;
  NT                     x1, y1, x2, y2;
  Segment_2              seg;
  int                    k;

  ifile >> n_segments;

  for (k = 0; k < n_segments; k++)
  {
    // Read the coordinates of the current segment.
    if (format == F_INTEGER)
    {
      ifile >> ix1 >> iy1 >> ix2 >> iy2;
      x1 = NT (ix1);
      y1 = NT (iy1);
      x2 = NT (ix2);
      y2 = NT (iy2);
    }
    else if (format == F_DOUBLE)
    {
      ifile >> dx1 >> dy1 >> dx2 >> dy2;
      x1 = NT (static_cast<int> (denom * dx1), denom);
      y1 = NT (static_cast<int> (denom * dy1), denom);
      x2 = NT (static_cast<int> (denom * dx2), denom);
      y2 = NT (static_cast<int> (denom * dy2), denom);
    }
    else
    {
      ifile >> x1 >> y1 >> x2 >> y2;
    }
      
    seg = Segment_2 (Point_2 (x1, y1), Point_2 (x2, y2));
    segs.push_back (seg);
  }
  ifile.close();

  return (true);
}

/*!
 * Check the envelope of a given set of segments.
 * \param segs The segments.
 * \param diag The diagram.
 * \param is_lower Does the diagram reprsent the lower or the upper envelope.
 * \return Whether the diagram structure is correct.
 */
bool check_envelope (const Segment_list& segs,
                     const Diagram_1& diag,
                     bool is_lower)
{
  // Go over minimization diagram.
  Diagram_1::Edge_const_handle    e = diag.leftmost();
  Diagram_1::Vertex_const_handle  v;

  if (e == diag.rightmost())
    // If the diagram is empty, the segment set must also be empty.
    return (segs.empty());

  // Start from the first finite edge.
  Kernel                         ker;
  Kernel::Construct_midpoint_2   midpoint = ker.construct_midpoint_2_object();
  Kernel::Construct_min_vertex_2 min_ver = ker.construct_min_vertex_2_object();
  Kernel::Construct_max_vertex_2 max_ver = ker.construct_max_vertex_2_object();
  Kernel::Compare_x_2            comp_x = ker.compare_x_2_object();
  Kernel::Compare_y_at_x_2       comp_y_at_x = ker.compare_y_at_x_2_object();
  Point_2                        p_mid;
  CGAL::Comparison_result        res1, res2;
  CGAL::Comparison_result        y_res;
  Segment_list::const_iterator   sit;

  e = e->right()->right();
  while (e != diag.rightmost())
  {
    // Get the midpoint of the current edge.
    p_mid = midpoint (e->left()->point(), e->right()->point());

    // Go over all segments.
    for (sit = segs.begin(); sit != segs.end(); ++sit)
    {
      // Check if p_mid lies in the x-range of the current segment.
      res1 = compare_x (p_mid, min_ver(*sit));

      if (res1 == CGAL::SMALLER)
        continue;

      res2 = compare_x (p_mid, max_ver(*sit));

      if (res2 == CGAL::LARGER)
        continue;

      // Check the diagram edge.
      if (e->is_empty())
      {
        // If p_mid is in the x-range of the given segment, the current edge
        // cannot by empty.
        std::cerr << "The edge (" << e->left()->point()
                  << ") -> (" << e->right()->point() << ") is empty, "
                  << "but the segment [" << *sit << "] is defined over it."
                  << std::endl;
        return (false);
      }
      else
      {
        // Compare the segment with the segment associated with the edge.
        y_res = comp_y_at_x (p_mid, *sit, e->curve());
        
        // Check for violations of the diagram properties.
        if ((is_lower && y_res == CGAL::SMALLER) ||
            (! is_lower && y_res == CGAL::LARGER))
        {
          std::cerr << "The edge (" << e->left()->point()
                    << ") -> (" << e->right()->point() 
                    << ") is associated with the segment ["
                    << e->curve() << "], but the segment [" 
                    << *sit << "] violates the envelope properties."
                  << std::endl;
          return (false);
        }
      }
    }
    v = e->right();
    e = v->right();
  }

    
  // If we reached here, the diagram is valid.
  return (true);
}

/*!
 * The main program.
 */
int main (int argc, char **argv)
{
  if (argc < 2)
  {
    std::cerr << "Usage: " << argv[0] 
              << "<input file> [ -q | -i | -d ]" << std::endl;
    return (1);
  }

  // Determine the input format.
  Coord_input_format   format = F_RATIONAL;

  if (argc > 2)
  {
    if (strcmp (argv[2], "-i") || strcmp (argv[2], "-I"))
      format = F_INTEGER;
    else if (strcmp (argv[2], "-d") || strcmp (argv[2], "-D"))
      format = F_DOUBLE;
  }

  // Read the input segments.
  Segment_list   segments;

  if (! read_segments (argv[1], format, segments))
    return (1);

  // Compute their lower envelope.
  Diagram_1              min_diag;
  
  lower_envelope_2 (segments.begin(), segments.end(),
                    min_diag);

  // Check the lower envelope.
  if (! check_envelope (segments, min_diag, true))
  {
    std::cerr << "Problems in the lower-envelope computation." << std::endl;
    return (1);
  }
  else
  {
    std::cout << "The lower envelope is valid." << std::endl;
  }

  // Compute the upper envelope.
  Diagram_1              max_diag;
  
  upper_envelope_2 (segments.begin(), segments.end(),
                    max_diag);

  // Check the upper envelope.
  if (! check_envelope (segments, max_diag, false))
  {
    std::cerr << "Problems in the upper-envelope computation." << std::endl;
    return (1);
  }
  else
  {
    std::cout << "The upper envelope is valid." << std::endl;
  }

  return (0);
}
