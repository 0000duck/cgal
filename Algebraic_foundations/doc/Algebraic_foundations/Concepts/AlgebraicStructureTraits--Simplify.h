
/*!
\ingroup PkgAlgebraicFoundationsAlgebraicStructuresConcepts
\cgalconcept

This `AdaptableUnaryFunction` may simplify a given object. 

\refines ::AdaptableUnaryFunction 

\sa ::AlgebraicStructureTraits 

*/

class AlgebraicStructureTraits::Simplify {
public:

/// \name Types 
/// @{

/*! 
Is void. 
*/ 
typedef Hidden_type result_type; 

/*! 
Is `AlgebraicStructureTraits::Type`. 
*/ 
typedef Hidden_type argument_type; 

/// @} 

/// \name Operations 
/// @{

/*! 
may simplify \f$ x\f$. 
*/ 
result_type operator()(argument_type x); 

/// @}

}; /* end AlgebraicStructureTraits::Simplify */

