
/*!
\ingroup PkgAlgebraicFoundationsMiscellaneousConcepts
\cgalconcept

A model of the concept `FromIntConstructible` is required 
to be constructible from int. 

\hasModel int 
\hasModel long 
\hasModel double 

*/

class FromIntConstructible {
public:

/// \name Creation 
/// @{

/*! 

*/ 
FromIntConstructible(int& i); 

/// @}

}; /* end FromIntConstructible */

