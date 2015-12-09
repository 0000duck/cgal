
namespace CGAL {

/*!
\ingroup PkgTriangulationsTriangulationClasses

This class is used to maintain the
regular triangulation -- also known as weighted Delaunay triangulation --
of a set of weighted points in \f$ \mathbb{R}^D \f$.
The dimension \f$ D\f$ can be specified at compile-time or
run-time. It should be kept reasonably small -- see the performance 
section in the user manual for what reasonable means.

\warning The removal of points is not supported yet.

A regular triangulation is defined as follows.

Let \f$ {S}^{(w)}\f$ be a set of weighted points in \f$ \mathbb{R}^D\f$. Let
\f$ {p}^{(w)}=(p,w_p), p\in\mathbb{R}^D, w_p\in\mathbb{R}\f$ and
\f$ {z}^{(w)}=(z,w_z), z\in\mathbb{R}^D, w_z\in\mathbb{R}\f$ 
be two weighted points.
A weighted point
\f$ {p}^{(w)}=(p,w_p)\f$ can also be seen as a sphere of center \f$ p\f$ and
radius \f$ \sqrt{w_p}\f$.
The <I>power product</I> (or <I>power distance</I> )
between \f$ {p}^{(w)}\f$ and \f$ {z}^{(w)}\f$ is
defined as
\f[ \Pi({p}^{(w)},{z}^{(w)}) = {\|{p-z}\|^2-w_p-w_z} \f]
where \f$ \|{p-z}\|\f$ is the Euclidean distance between \f$ p\f$ and \f$ z\f$.
\f$ {p}^{(w)}\f$ and \f$ {z}^{(w)}\f$
are said to be <I>orthogonal</I> if \f$ \Pi{({p}^{(w)}-{z}^{(w)})}
= 0\f$.

\f$D + 1\f$ weighted points have a unique common orthogonal weighted point 
called the <I>power sphere</I>. A sphere \f$ {z}^{(w)}\f$ is said to be
<I>regular</I> if \f$ \forall {p}^{(w)}\in{S}^{(w)},
\Pi{({p}^{(w)}-{z}^{(w)})}\geq 0\f$.

A triangulation of \f$ {S}^{(w)}\f$ is <I>regular</I> if the power spheres
of all simplices are regular.

Parameters
--------------

\tparam RegularTriangulationTraits is the geometric traits class that provides the
geometric types and predicates needed by regular triangulations. 
`RegularTriangulationTraits` must be a model of the concept 
`RegularTriangulationTraits`.

\tparam TriangulationDataStructure must be a model of the concept
`TriangulationDataStructure`. This model is used to store 
the faces of the triangulation. The parameter `TriangulationDataStructure` 
defaults to `Triangulation_data_structure` whose template parameters are 
instantiated as follows:
<UL>
<LI>`RegularTriangulationTraits::Dimension`</LI>
<LI>`Triangulation_vertex<CGAL::Regular_triangulation_euclidean_traits<RegularTriangulationTraits> >`</LI>
<LI>`Triangulation_full_cell<CGAL::Regular_triangulation_euclidean_traits<RegularTriangulationTraits> >`.</LI>
</UL>

\tparam Regular_triangulation can
be defined by specifying only the first parameter, or by using the
tag `CGAL::Default` as the second parameter. 

\sa `Delaunay_triangulation`
\sa `Triangulation_data_structure`

*/
template< typename RegularTriangulationTraits, typename TriangulationDataStructure >
class Regular_triangulation
  : public Triangulation<RegularTriangulationTraits, TriangulationDataStructure>
{
public:

/// \name Types
/// @{

/*!
A point in Euclidean space.
*/
typedef RegularTriangulationTraits::Weighted_point Weighted_point;

/// @}

/// \name Creation
/// @{

/*!
Instantiates a regular triangulation with one vertex (the vertex
at infinity). See the description of the inherited nested type
`Triangulation::Maximal_dimension` for an explanation of
the use of the parameter `dim`. The complex stores a copy of the geometric
traits `gt`.
*/
Regular_triangulation(const int dim, const Geom_traits gt = Geom_traits());

/// @}

/// \name Point insertion
/// @{

/*!
Inserts weighted point `p` in the triangulation and returns the corresponding
vertex. Returns a handle to the (possibly newly created) vertex at that 
position.
Prior to the actual insertion, `p` is located in the triangulation;
`hint` is used as a starting place for locating `p`.
*/
Vertex_handle insert(const Weighted_point & p, Full_cell_handle hint
  = Full_cell_handle());

/*!
Same as above but uses a vertex as starting place for the search.
*/
Vertex_handle insert(const Weighted_point & p, Vertex_handle hint);

/*!
\cgalAdvancedBegin
Inserts weighted point `p` in the triangulation and returns the corresponding
vertex. Similar to the above `insert()` function, but takes as additional
parameter the return values of a previous location query. See description of
`Triangulation::locate()`.
\cgalAdvancedEnd
*/
Vertex_handle insert(const Weighted_point & p, const Locate_type lt,
  const Face & f, const Facet & ft, const Full_cell_handle c);

/*!
Inserts the weighted points found in range `[s,e)` in the regular triangulation.
Returns the difference of the number of vertices between after and
before the insertions (it may be negative due to hidden points).
Note that this function is not guaranteed to insert the points
following the order of `InputIterator`, as `spatial_sort()`
is used to improve efficiency.

\tparam ForwardIterator must be an input iterator with the value
type `Weighted_point`. 
*/
template< typename ForwardIterator >
std::ptrdiff_t insert(ForwardIterator s, ForwardIterator e);

/*!
\cgalAdvancedBegin
Inserts the point `p` in the regular triangulation. Returns a handle to the
(possibly newly created) vertex at that position. 
\pre The point `p`
must lie outside the affine hull of the regular triangulation. This implies that
`rt`.`current_dimension()` must be less than `rt`.`maximal_dimension()`.
\cgalAdvancedEnd
*/
Vertex_handle insert_outside_affine_hull(const Weighted_point & p);

/*!
\cgalAdvancedBegin
Inserts the point `p` in the regular triangulation. Returns a handle to the
(possibly newly created) vertex at that position.
\pre The point `p` must be in conflict with the full cell `c`.
\cgalAdvancedEnd
*/
Vertex_handle insert_in_conflicting_cell(const Weighted_point & p, const
Full_cell_handle c);

/// @}

/// \name Queries
/// @{

/*!
Returns `true` if and only if the point `p` is in
conflict with full cell `c` (A weighted point `p` is said to be in conflict 
with a cell `c` if it has a negative power distance to the power sphere of `c`.)
*/
bool is_in_conflict(const Weighted_point & p, Full_cell_const_handle c)
const;

/*!
\cgalAdvancedBegin
Outputs handles to the full cells in conflict with
point `p` into the `OutputIterator out`. The full cell `c` is used
as a starting point for gathering the full cells in conflict with
`p`.
A facet `(cc,i)` on the boundary of the conflict zone with
`cc` in conflict is returned.
\pre `c` is in conflict with `p` and `rt`.`current_dimension()`\f$ \geq2\f$.
\cgalAdvancedEnd
*/
template< typename OutputIterator >
Facet compute_conflict_zone(const Weighted_point & p, const Full_cell_handle c,
OutputIterator out) const;

/// @}

}; /* end regular_triangulation */
} /* end namespace CGAL */
