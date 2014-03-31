
/*!
\ingroup PkgTriangulationsConcepts
\cgalConcept

A `TriangulationDSFace` describes a face `f` with dimension `k` 
(a `k`-face) in a triangulation.
It gives access to a handle to a full cell `c` containing the face
`f` in its boundary, as well as the indices of the vertices of `f` in
`c`. It must hold that `f` is a <I>proper</I> face of full cell
`c`, i.e., the dimension of `f` is strictly less than
the dimension of `c`.
\cgalModifBegin
The dimension of a face is automatically set when 
`TriangulationDSFace::set_index` is called. 
\cgalModifEnd

\cgalHasModel `CGAL::Triangulation_face<TriangulationDataStructure>`

\sa `TriangulationDSFullCell`
\sa `TriangulationDSVertex`
\sa `TriangulationDataStructure`
\sa `Triangulation`

*/

class TriangulationDSFace {
public:

/// \name Types
/// @{

/*!
\cgalModifBegin
The `Triangulation_data_structure` in which the `TriangulationDSFace` is
defined/used.
Must be a model of the `TriangulationDataStructure` concept.
\cgalModifEnd
*/
typedef Hidden_type Triangulation_data_structure;

/*!
Must be the same as the nested type
`Triangulation_data_structure::Full_cell_handle`.
*/
typedef Hidden_type Full_cell_handle;

/*!
Must be the same as the nested type
`Triangulation_data_structure::Vertex_handle`.
*/
typedef Hidden_type Vertex_handle;

/// @}

/// \name Creation
/// There is no default constructor, since the maximal dimension (of
/// the full cells) must be known by the constructors of a
/// `TriangulationDSFace`.
/// @{

/*!
Copy constructor.
*/
Triangulation_face(Triangulation_face g);

/*!
Sets the `Face`'s
full cell to `c` and the maximal dimension to
`c.maximal_dimension()`.
\pre `c!=Full_cell_handle()`

*/
Triangulation_face(Full_cell_handle c);

/*!
Setup the `Face` knowing
the maximal dimension `md`. Sets the `Face`'s full cell to the
default-constructed one.
*/
Triangulation_face(const int md);

/// @}

/// \name Access functions
/// @{

/*!
Returns a handle to a cell that
has the face in its boundary.
*/
Full_cell_handle full_cell() const;

/*!
Returns the dimension of the face
(one less than the number of vertices).
*/
int face_dimension() const;

/*!
Returns the index of the `i`-th vertex
of the face in the cell `f`.`full_cell()`. \pre \f$0 \leq i \leq \f$`f`.`face_dimension()`.
*/
int index(int i) const;

/*!
Returns a handle to the
`i`-th `Vertex` of the face in the cell `f`.`full_cell()`.
\pre \f$0 \leq i \leq \f$ `f.face_dimension()`.
*/
Vertex_handle vertex(int i) const;

/// @}

/// \name Update functions
/// @{

/*!
Sets the facet to the empty set. Maximal
dimension remains unchanged.
*/
void clear();

/*!
Sets the cell of the face to
`c`.
\pre `c!=Full_cell_handle()`

*/
void set_full_cell(Full_cell_handle c);

/*!
Sets the index of the `i`-th
vertex of the face to be the `j`-th vertex of the full cell.
\pre \f$0 \leq i \leq \f$ `f.full_cell()->face_dimension()`.
\pre \f$0 \leq j \leq \f$ `f.full_cell()->maximal_dimension()`.
*/
void set_index(int i, int j);

/// @}

}; /* end TriangulationDSFace */
