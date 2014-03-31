
/*!
\ingroup PkgTriangulationsConcepts
\cgalConcept

The `TriangulationDataStructure` concept describes objects responsible for 
storing and maintaining the combinatorial part of a
\f$ d\f$-dimensional pure simplicial complex that has the topology
of the \f$ d\f$-dimensional sphere \f$ \mathcal S^d\f$ with \f$ d\in[-2,D]\f$.
Since the simplicial \f$ d\f$-complex is pure, all
faces are sub-faces of some \f$ d\f$-simplex. And since it has the 
topology of the sphere \f$ \mathcal S^d\f$, it is manifold, thus 
any \f$ d-1\f$-face belongs to exactly two \f$ d\f$-dimensional full cells.

Possible values for the current dimension \f$ d\f$ include

<DL>
<DT><B>-2</B><DD> This corresponds to the non-existence of any object in
the triangulation.
<DT><B>-1</B><DD> This corresponds to a single vertex and a single full cell,
which is also the unique vertex and the unique full cell in the
`TriangulationDataStructure`.
In a
geometric realization of the `TriangulationDataStructure` (<I>e.g.</I>, in a
`Triangulation<TriangulationTraits, TriangulationDataStructure>` or a
`Delaunay_triangulation<DelaunayTriangulationTraits, TriangulationDataStructure>`), this vertex
corresponds to <I>the vertex at infinity</I>.

<DT><B>0</B><DD> This corresponds to two vertices, each incident to one \f$ 0\f$-face;
the two full cells being neighbors of each other. This is the unique
triangulation of the \f$ 0\f$-sphere.

<DT><B>\f$ d>0\f$</B><DD> This corresponds to a standard triangulation of the sphere
\f$ \mathcal S^d\f$.
</DL>

An \f$ i\f$-simplex is a simplex with \f$ i+1\f$ vertices. An \f$ i\f$-simplex \f$ \sigma\f$ is
incident to a \f$ j\f$-simplex \f$ \sigma'\f$, \f$ j<i\f$, if and only if \f$ \sigma'\f$
is a proper face of \f$ \sigma\f$.

We call a \f$ 0\f$-simplex a <I>vertex</I>, a \f$ (d-1)\f$-simplex a <I>facet</I> and a
\f$ d\f$-simplex a <I>full cell</I>. A <I>face</I> can have any dimension.
Two full cells are <I>neighbors</I> if they share a facet. Two faces are
<I>incident</I> if one is included in the other.

Input/Output
--------------

The information stored in the `iostream` is:

- the current dimension (which must be `<=``tds`.`maximal_dimension()`),

- the number of vertices,

- for each vertex the information of that vertex,

- the number of full cells,

- for each full cell the indices of its vertices and extra information for that full cell,

- for each full cell the indices of its neighbors.

The indices of vertices and full cells correspond to the order in the
file; the user cannot control it.
The classes `Vertex` and
`Full_cell` have to provide the relevant I/O operators
(possibly empty).

\cgalHasModel `CGAL::Triangulation_data_structure<Dimensionality, TriangulationDSVertex, TriangulationDSFullCell>`

\sa `TriangulationDSVertex`
\sa `TriangulationDSFullCell`
\sa `TriangulationDSFace`
\sa `Triangulation`

*/

class TriangulationDataStructure {
public:

/// \name Types
// @{

/*!

The vertex type. A model of the concept `TriangulationDSVertex`.

*/
typedef Hidden_type Vertex;

/*!

The full cell type. A model of the concept `TriangulationDSFullCell`.

*/
typedef Hidden_type Full_cell;

/*! 
\cgalModifBegin
A model of the concept `FullCellData`.
\cgalModifEnd
*/ 
typedef Hidden_type Full_cell_data; 
/*!

The concept `TriangulationDataStructure` also defines a type for
describing faces of the triangulation with codimension 1.

The constructor `Facet(c,i)` constructs a `Facet` representing the facet of
full cell `c` opposite to its `i`-th vertex. Its dimension is
`current_dimension()-1`.

*/
typedef Hidden_type Facet;

/*!
A model of the concept `TriangulationDSFace`.
*/
typedef Hidden_type Face;

/// @}


/// \name Handles
/// Vertices and full cells are manipulated via
/// <I>handles</I>. Handles support the usual two dereference
/// operators and `operator->`. 

/*!

Handle to a `Vertex`.

*/
typedef Hidden_type Vertex_handle;

/*!

Handle to a `Full_cell`.

*/
typedef Hidden_type Full_cell_handle;

/// @}

/// \name Rebind
/// @{

/*!
This nested template class allows to get the type of a triangulation
data structure that only changes the vertex type. It has to define a type
`Other` which is a <I>rebound</I> triangulation data structure with `Vb2`
as vertex type.
*/
typedef Hidden_type template <typename Vb2> struct Rebind_vertex;

/*!
This nested template class allows to get the type of a triangulation
data structure that only changes the full cell type. It has to define a type
`Other` which is a <I>rebound</I> triangulation data structure with `Fcb2`
as full cell type.
*/
typedef Hidden_type template <typename Fcb2> struct Rebind_full_cell;


/// @}

/// \name Iterators
/// Vertices, facets and full cells can be iterated over using
/// <I>iterators</I>. Iterators support the usual two dereference
/// operators and `operator->`.
/// @{

/*!

Iterator over the list of vertices.

*/
typedef Hidden_type Vertex_iterator;

/*!

Iterator over the list of full cells.

*/
typedef Hidden_type Full_cell_iterator;

/*!

Iterator over the facets of the complex.

*/
typedef Hidden_type Facet_iterator;

/*!
Size type (an unsigned integral type)
*/
typedef Hidden_type size_type;

/*!
Difference type (a signed integral type)
*/
typedef Hidden_type difference_type;

/// @}

/// \name Creation
/// @{

/*!
Creates an instance `tds` of
type `TriangulationDataStructure`. The maximal dimension of its full cells is `dim` and
`tds` is initialized to the empty triangulation. Thus,
`tds`.`current_dimension()` equals `-2`.
The parameter `dim` can be ignored by the constructor if it is already
known at compile-time. Otherwise, the following precondition holds:
\pre `dim>0`.
*/
TriangulationDataStructure(int dim = 0);

/// @}

/// \name Queries
/// @{

/*!
Returns the maximal dimension of
the full dimensional cells that can be stored in the triangulation `tds`. \post the
returned value is positive.
*/
int maximal_dimension() const;

/*!
Returns the dimension of the
full dimensional cells stored in the triangulation. It holds that
`tds`.`current_dimension()=-2` if and only if `tds`.`empty()` is
`true`. \post the returned value `d` satisfies
\f$ -2\leq d \leq\f$`tds`.`maximal_dimension()`.
*/
int current_dimension() const;

/*!
Returns `true` if thetriangulation
contains nothing. Returns `false` otherwise.
*/
bool empty() const;

/*!
Returns the number of vertices in the triangulation.
*/
size_type number_of_vertices() const;

/*!
Returns the number of full cells in the triangulation.
*/
size_type number_of_full_cells() const;

/*!
Tests whether `v` is a vertex of the triangulation.
*/
bool is_vertex(const Vertex_handle & v) const;

/*!
Tests whether `c` is a full cell of the triangulation.
*/
bool is_full_cell(const Full_cell_handle & c) const;

/*!
This function computes (<I>gathers</I>) a connected set of full cells
satifying a common criterion. Call them <I>good</I> full cells. It is assumed
that the argument `c` is a good full cell. The full cells are then
recursively explored by examining if, from a given good full cell, its adjacent
full cells are also good.

The argument `tp` is a predicate, i.e.\ a function or a functor providing 
`operator()`, that takes as argument a `Facet`
whose `Full_cell` is good.
The predicate must return `true`
if the traversal of that `Facet` leads to a good full cell.

All the good full cells are output into the last argument `out`.
\pre `c!=Full_cell_handle()` and `tp(c)==true`.

*/
template< typename TraversalPredicate, typename OutputIterator >
void full_cells(Full_cell_handle c, TraversalPredicate & tp,
OutputIterator & out) const;

/*!
Insert in `out` all the full cells that are incident to the vertex
`v`, i.e., the full cells that have the `Vertex v` as a vertex.
Returns the output iterator.
\pre `v!=Vertex_handle()`.

*/
template< typename OutputIterator > OutputIterator
incident_full_cells(Vertex_handle v, OutputIterator out) const;

/*!
Insert in `out` all the full cells that are incident to the face `f`,
i.e., the full cells that have the `Face f` as a subface.
Returns the output iterator.
\pre `f.full_cell()!=Full_cell_handle()`.

*/
template< typename OutputIterator > OutputIterator
incident_full_cells(const Face & f, OutputIterator out) const;

/*!
Insert in `out` all the full cells that share at least one vertex with the `Face f`. Returns the output iterator.
\pre `f.full_cell()!=Full_cell_handle()`.

*/
template< typename OutputIterator > OutputIterator
star(const Face & f, OutputIterator out) const;

/*!
Constructs all the `Face`s of dimension `dim` incident to
`Vertex` v and inserts them in the `OutputIterator out`. If `dim >=` `tds`.`current_dimension()`, then no `Face` is
constructed.
\pre `0 < dim` and `v!=Vertex_handle()`.

*/
template< typename OutputIterator > OutputIterator
incident_faces(Vertex_handle v, const int dim, OutputIterator
out);

/// @}

/// \name Accessing the vertices
/// @{

/*!
Returns a handle to the `i`-th `Vertex` of the `Full_cell` `c`.
\pre \f$0 \leq i \leq \f$`tds`.`current_dimension()` and `c!=Full_cell_handle()`.
*/
Vertex_handle vertex(Full_cell_handle c, const int i) const;

/*!
Returns the index of `c` as a neighbor of its `i`-th neighbor.
\pre \f$0 \leq i \leq \f$`tds`.`current_dimension`()

and `c!=Full_cell_handle()`.
*/
int mirror_index(Full_cell_handle c, int i) const;

/*!
Returns the vertex of the `i`-th neighbor of `c`
that is not vertex of `c`.
\pre \f$0 \leq i \leq \f$`tds`.`current_dimension`()

and `c!=Full_cell_handle()`.
*/
Vertex_handle mirror_vertex(Full_cell_handle c, int i) const;

/*!

The first vertex of `tds`. User has no control on the order.

*/
Vertex_iterator vertices_begin();

/*!

The beyond vertex of `tds`.

*/
Vertex_iterator vertices_end();

/// @}

/// \name Accessing the full cells
/// @{

/*!
Returns a full cell incident to `Vertex` `v`. Note that this
full cell is
not unique (`v` is typically incident to more than one full cell).
\pre `v != Vertex_handle`
*/
Full_cell_handle full_cell(Vertex_handle v) const;

/*!
Returns a `Full_cell_handle` pointing to the `Full_cell`
opposite to the `i`-th vertex of `c`.
\pre \f$0 \leq i \leq \f$`tds`.`current_dimension()`

and `c != Full_cell_handle()`
*/
Full_cell_handle neighbor(Full_cell_handle c, int i) const;

/*!

The first full cell of `tds`. User has no control on the order.

*/
Full_cell_iterator full_cells_begin();

/*!

The beyond full cell of `tds`.

*/
Full_cell_iterator full_cells_end();

/// @}

/// \name Faces and Facets
/// @{

/*!
Iterator to the first facet of the triangulation.
*/
Facet_iterator facets_begin();

/*!
Iterator to the beyond facet of the triangulation.
*/
Facet_iterator facets_end();

/*!
Returns a full cell containing the facet `f`
*/
Full_cell_handle full_cell(const Facet & f) const;

/*!
Returns the index of vertex of the full cell `c=``tds`.`full_cell(f)`
which does not belong to `c`.
*/
int index_of_covertex(const Facet & f) const;

/// @}

/// \name Vertex insertion
/// @{

/*!
Inserts a new
vertex `v` in the full cell `c` and returns a handle to
it. The full cell
`c` is subdivided into `tds`.`current_dimension()`+1 full cells which
share the vertex `v`.

\cgalFigureBegin{triangulationfiginsertfullcell,insert-in-cell.png}
Insertion in a full cell, \f$ d=2\f$
\cgalFigureEnd

\pre Current dimension is positive and `c` is a full cell of
`tds`.
*/
Vertex_handle insert_in_full_cell(Full_cell_handle c);

/*!
Inserts a vertex in the triangulation data structure by subdividing the
`Face f`. Returns a handle to the newly created `Vertex`.

\cgalFigureBegin{triangulationfiginsertface,insert-in-face.png}
Insertion in face, \f$ d=3\f$
\cgalFigureEnd
*/
Vertex_handle insert_in_face(const Face & f);

/*!
Inserts a vertex in the triangulation data structure by subdividing the
`Facet ft`. Returns a handle to the newly created `Vertex`.
*/
Vertex_handle insert_in_facet(const Facet & ft);

/*!
The
full cells in the range \f$ C=\f$`[start, end)` are removed, thus
forming a hole \f$ H\f$.
A `Vertex` is inserted and connected to the boundary of the hole in order
to ``fill it''. A `Vertex_handle` to the new `Vertex` is returned.
\pre `c` belongs to \f$ C\f$ and `c->neighbor(i)`
does not, with `f=(c,i)`.
\f$ H\f$ the union of full cells in \f$ C\f$ is simply connected and its
boundary \f$ \partial H\f$ is a
combinatorial triangulation of the sphere \f$ \mathcal S^{d-1}\f$.
All vertices of cells of \f$ C\f$ are on \f$ \partial H\f$.

\cgalFigureBegin{triangulationfiginserthole,insert-in-hole.png}
Insertion in a hole, \f$ d=2\f$
\cgalFigureEnd
*/
template< class ForwardIterator > Vertex_handle
insert_in_hole(ForwardIterator start, ForwardIterator end, Facet f);

/*!
Same as above, but handles to the new full cells are
appended to the `out` output iterator.
*/
template< class ForwardIterator, class OutputIterator >
Vertex_handle insert_in_hole(ForwardIterator start, ForwardIterator end, Facet
f, OutputIterator out);

/*!
Transforms a triangulation of the sphere \f$ \mathcal S^d\f$ into the
triangulation of the sphere \f$ \mathcal S^{d+1}\f$ by adding a new vertex
`v`.
`v` is used to triangulate one of the two half-spheres of
\f$ \mathcal S^{d+1}\f$ (\f$ v\f$ is added as \f$ (d+2)^{th}\f$ vertex to all
full cells)
and `star` is used to triangulate the other half-sphere
(all full cells that do not already have star as vertex are duplicated,
and `star` replaces `v` in these full cells).
The indexing of the vertices in the
full cell is such that, if `f` was a full cell of maximal dimension in the
initial complex, then `(f,v)`, in this order, is the corresponding full cell
in the updated triangulation. A handle to `v` is returned
(see Figure \cgalFigureRef{triangulationfiginsertincreasedim}).
\pre
If the current dimension is -2 (empty triangulation), then `star`
can be omitted (it is ignored), otherwise
the current dimension must be strictly less than the maximal dimension
and `star` must be a vertex of `tds`.

\cgalFigureBegin{triangulationfiginsertincreasedim,insert-increase-dim.png}
Insertion, increasing the dimension from \f$ d=1\f$ to \f$ d=2\f$
\cgalFigureEnd

*/
Vertex_handle insert_increase_dimension(Vertex_handle star);

/*!
Adds a new full cell to `tds` and
returns a handle to it. The new full cell has no vertices and no neighbors yet.
*/
Full_cell_handle new_full_cell();

/*!
Adds a new vertex to `tds` and returns a handle to it. The new vertex has
no associated full cell nor index yet.
*/
Vertex_handle new_vertex();

/*!
Sets the `i`-th vertex of `c` to `v` and, if `v != Vertex_handle()`,
sets `c` as the incident full cell of `v`.
*/
void associate_vertex_with_full_cell(Full_cell_handle c, int i,
Vertex_handle v);

/*!
Sets the neighbor opposite to vertex `i` of `Full_cell` `ci` to
`cj`. Sets the neighbor opposite to vertex `j` of `Full_cell`
`cj` to `ci`.
*/
void set_neighbors(Full_cell_handle ci, int i, Full_cell_handle cj, int
j);

/*!
Forces the current dimension
of the complex to `d`.
\pre \f$-1 \leq d \leq \f$`maximal_dimension()`.
*/
void set_current_dimension(int d);

/// @}

/// \name Vertex removal
/// @{

/*!
Reinitializes `tds` to the empty complex.
*/
void clear();

/*!
Contracts the
`Face f` to a single vertex. Returns a handle to that vertex
(see Figure \cgalFigureRef{triangulationfigcollapseface}).
\pre The boundary of the full cells incident to `f`
is a topological sphere of dimension
`tds`.`current_dimension()`-1).

\cgalFigureBegin{triangulationfigcollapseface,collapse-face.png}
Collapsing an edge in dimension \f$ d=3\f$, `v` is returned.
\cgalFigureEnd


*/
Vertex_handle collapse_face(const Face & f);

/*!
This method does exactly the opposite of
`insert_increase_dimension()`:
`v` is removed,
full cells not containing `star` are removed,
full cells containing `star` but not `v` loose vertex `star`,
full cells containing `star` and `v` loose vertex `v`
(see Figure \cgalFigureRef{triangulationfiginsertincreasedim}).
\pre All cells contain either `star` or `v`.
Edge `star-v` exists in the triangulation
and `current_dimension()!=2`.

*/
void remove_decrease_dimension(Vertex_handle v, Vertex_handle star);

/*!
Remove the vertex `v` from the triangulation.

*/
void delete_vertex(Vertex_handle v);

/*!
Remove the full cell `c` from the triangulation.

*/
void delete_full_cell(Full_cell_handle c);

/*!
Remove the full cells in the range `[start,end)` from the triangulation.

*/
template< typename ForwardIterator > void
delete_full_cells(ForwardIterator start, ForwardIterator end);

/// @}

/// \name Validity check
/// @{

/*!
Partially checks whether `tds` is indeed a triangulation.

\cgalDebug It must <I>at least</I>
<UL>
<LI>check the validity of the vertices and full cells of `tds` by calling
their respective `is_valid` method.
<LI>check that each full cell has no duplicate vertices and has as many
neighbors as its number of facets (`current_dimension()+1`).
<LI>check that each full cell share exactly `tds`.`current_dimension()`
vertices with each of its neighbor.
</UL>

When `verbose` is set to `true`, messages are printed to give
a precise indication on the kind of invalidity encountered.

Returns `true` if all the tests pass, `false` if any test fails. See
the documentation for the models of this concept to see the additionnal (if
any) validity checks that they implement.
*/
bool is_valid(bool verbose=false) const;

/*!
Reads a combinatorial triangulation from `is` and assigns it to
`tds`. \pre The dimension of the input complex must be less than or
equal to `tds`.`maximal_dimension()`.
*/
std::istream & operator>>(std::istream & is, TriangulationDataStructure &
tds);

/*!
Writes `tds` into the output stream `os`
*/
std::ostream & operator<<(std::ostream & os, const TriangulationDataStructure
& tds);

/// @}

}; /* end TriangulationDataStructure */
