

// ============================================================================
//
// Copyright (c) 1997 The CGAL Consortium
//
// This software and related documentation is part of an INTERNAL release
// of the Computational Geometry Algorithms Library (CGAL). It is not
// intended for general use.
//
// ----------------------------------------------------------------------------
//
// release       : $CGAL_Revision: CGAL-0.9-I-06 $
// release_date  : $CGAL_Date: 1998/03/11 $
//
// file          : include/CGAL/polygon_assertions.h
// source        : Generated from script create_assertions.sh
// author(s)     : script by Geert-Jan Giezeman and Sven Sch�nherr 
//
// coordinator   : Utrecht University
//
// ============================================================================



// macro definitions
// =================
// assertions
// ----------


#if defined(CGAL_POLYGON_NO_ASSERTIONS) || defined(CGAL_NO_ASSERTIONS) \
  || defined(NDEBUG)
#  define CGAL_polygon_assertion(EX) ((void)0)
#  define CGAL_polygon_assertion_msg(EX,MSG) ((void)0)
#  define CGAL_polygon_assertion_code(CODE)
#else
#  define CGAL_polygon_assertion(EX) \
   ((EX)?((void)0):CGAL_assertion_fail( # EX , __FILE__, __LINE__, 0))
#  define CGAL_polygon_assertion_msg(EX,MSG) \
   ((EX)?((void)0):CGAL_assertion_fail( # EX , __FILE__, __LINE__, MSG))
#  define CGAL_polygon_assertion_code(CODE) CODE
#endif // CGAL_POLYGON_NO_ASSERTIONS

#if defined(CGAL_POLYGON_NO_ASSERTIONS) || defined(CGAL_NO_ASSERTIONS) \
  || (!defined(CGAL_POLYGON_CHECK_EXACTNESS) && !defined(CGAL_CHECK_EXACTNESS))\
  || defined(NDEBUG)
#  define CGAL_polygon_exactness_assertion(EX) ((void)0)
#  define CGAL_polygon_exactness_assertion_msg(EX,MSG) ((void)0)
#  define CGAL_polygon_exactness_assertion_code(CODE)
#else
#  define CGAL_polygon_exactness_assertion(EX) \
   ((EX)?((void)0):CGAL_assertion_fail( # EX , __FILE__, __LINE__, 0))
#  define CGAL_polygon_exactness_assertion_msg(EX,MSG) \
   ((EX)?((void)0):CGAL_assertion_fail( # EX , __FILE__, __LINE__, MSG))
#  define CGAL_polygon_exactness_assertion_code(CODE) CODE
#endif // CGAL_POLYGON_NO_ASSERTIONS

#if defined(CGAL_POLYGON_NO_ASSERTIONS) \
  || defined(CGAL_NO_ASSERTIONS) \
  || (!defined(CGAL_POLYGON_CHECK_EXPENSIVE) && !defined(CGAL_CHECK_EXPENSIVE)) \
  || defined(NDEBUG)
#  define CGAL_polygon_expensive_assertion(EX) ((void)0)
#  define CGAL_polygon_expensive_assertion_msg(EX,MSG) ((void)0)
#  define CGAL_polygon_expensive_assertion_code(CODE)
#else
#  define CGAL_polygon_expensive_assertion(EX) \
   ((EX)?((void)0):CGAL_assertion_fail( # EX , __FILE__, __LINE__, 0))
#  define CGAL_polygon_expensive_assertion_msg(EX,MSG) \
   ((EX)?((void)0):CGAL_assertion_fail( # EX , __FILE__, __LINE__, MSG))
#  define CGAL_polygon_expensive_assertion_code(CODE) CODE
#endif // CGAL_POLYGON_NO_ASSERTIONS

#if defined(CGAL_POLYGON_NO_ASSERTIONS) || defined(CGAL_NO_ASSERTIONS) \
  || (!defined(CGAL_POLYGON_CHECK_EXACTNESS) && !defined(CGAL_CHECK_EXACTNESS))\
  || (!defined(CGAL_POLYGON_CHECK_EXPENSIVE) && !defined(CGAL_CHECK_EXPENSIVE)) \
  || defined(NDEBUG)
#  define CGAL_polygon_expensive_exactness_assertion(EX) ((void)0)
#  define CGAL_polygon_expensive_exactness_assertion_msg(EX,MSG) ((void)0)
#  define CGAL_polygon_expensive_exactness_assertion_code(CODE)
#else
#  define CGAL_polygon_expensive_exactness_assertion(EX) \
   ((EX)?((void)0):CGAL_assertion_fail( # EX , __FILE__, __LINE__, 0))
#  define CGAL_polygon_expensive_exactness_assertion_msg(EX,MSG) \
   ((EX)?((void)0):CGAL_assertion_fail( # EX , __FILE__, __LINE__, MSG))
#  define CGAL_polygon_expensive_exactness_assertion_code(CODE) CODE
#endif // CGAL_POLYGON_NO_ASSERTIONS


// preconditions
// -------------

#if defined(CGAL_POLYGON_NO_PRECONDITIONS) || defined(CGAL_NO_PRECONDITIONS) \
  || defined(NDEBUG)
#  define CGAL_polygon_precondition(EX) ((void)0)
#  define CGAL_polygon_precondition_msg(EX,MSG) ((void)0)
#  define CGAL_polygon_precondition_code(CODE)
#else
#  define CGAL_polygon_precondition(EX) \
   ((EX)?((void)0):CGAL_precondition_fail( # EX , __FILE__, __LINE__, 0))
#  define CGAL_polygon_precondition_msg(EX,MSG) \
   ((EX)?((void)0):CGAL_precondition_fail( # EX , __FILE__, __LINE__, MSG))
#  define CGAL_polygon_precondition_code(CODE) CODE
#endif // CGAL_POLYGON_NO_PRECONDITIONS

#if defined(CGAL_POLYGON_NO_PRECONDITIONS) || defined(CGAL_NO_PRECONDITIONS) \
  || (!defined(CGAL_POLYGON_CHECK_EXACTNESS) && !defined(CGAL_CHECK_EXACTNESS))\
  || defined(NDEBUG)
#  define CGAL_polygon_exactness_precondition(EX) ((void)0)
#  define CGAL_polygon_exactness_precondition_msg(EX,MSG) ((void)0)
#  define CGAL_polygon_exactness_precondition_code(CODE)
#else
#  define CGAL_polygon_exactness_precondition(EX) \
   ((EX)?((void)0):CGAL_precondition_fail( # EX , __FILE__, __LINE__, 0))
#  define CGAL_polygon_exactness_precondition_msg(EX,MSG) \
   ((EX)?((void)0):CGAL_precondition_fail( # EX , __FILE__, __LINE__, MSG))
#  define CGAL_polygon_exactness_precondition_code(CODE) CODE
#endif // CGAL_POLYGON_NO_PRECONDITIONS

#if defined(CGAL_POLYGON_NO_PRECONDITIONS) || defined(CGAL_NO_PRECONDITIONS) \
  || (!defined(CGAL_POLYGON_CHECK_EXPENSIVE) && !defined(CGAL_CHECK_EXPENSIVE)) \
  || defined(NDEBUG)
#  define CGAL_polygon_expensive_precondition(EX) ((void)0)
#  define CGAL_polygon_expensive_precondition_msg(EX,MSG) ((void)0)
#  define CGAL_polygon_expensive_precondition_code(CODE)
#else
#  define CGAL_polygon_expensive_precondition(EX) \
   ((EX)?((void)0):CGAL_precondition_fail( # EX , __FILE__, __LINE__, 0))
#  define CGAL_polygon_expensive_precondition_msg(EX,MSG) \
   ((EX)?((void)0):CGAL_precondition_fail( # EX , __FILE__, __LINE__, MSG))
#  define CGAL_polygon_expensive_precondition_code(CODE) CODE
#endif // CGAL_POLYGON_NO_PRECONDITIONS

#if defined(CGAL_POLYGON_NO_PRECONDITIONS) || defined(CGAL_NO_PRECONDITIONS) \
  || (!defined(CGAL_POLYGON_CHECK_EXACTNESS) && !defined(CGAL_CHECK_EXACTNESS))\
  || (!defined(CGAL_POLYGON_CHECK_EXPENSIVE) && !defined(CGAL_CHECK_EXPENSIVE)) \
  || defined(NDEBUG)
#  define CGAL_polygon_expensive_exactness_precondition(EX) ((void)0)
#  define CGAL_polygon_expensive_exactness_precondition_msg(EX,MSG) ((void)0)
#  define CGAL_polygon_expensive_exactness_precondition_code(CODE)
#else
#  define CGAL_polygon_expensive_exactness_precondition(EX) \
   ((EX)?((void)0):CGAL_precondition_fail( # EX , __FILE__, __LINE__, 0))
#  define CGAL_polygon_expensive_exactness_precondition_msg(EX,MSG) \
   ((EX)?((void)0):CGAL_precondition_fail( # EX , __FILE__, __LINE__, MSG))
#  define CGAL_polygon_expensive_exactness_precondition_code(CODE) CODE
#endif // CGAL_POLYGON_NO_PRECONDITIONS


// postconditions
// --------------

#if defined(CGAL_POLYGON_NO_POSTCONDITIONS) || defined(CGAL_NO_POSTCONDITIONS) \
  || defined(NDEBUG)
#  define CGAL_polygon_postcondition(EX) ((void)0)
#  define CGAL_polygon_postcondition_msg(EX,MSG) ((void)0)
#  define CGAL_polygon_postcondition_code(CODE)
#else
#  define CGAL_polygon_postcondition(EX) \
   ((EX)?((void)0):CGAL_postcondition_fail( # EX , __FILE__, __LINE__, 0))
#  define CGAL_polygon_postcondition_msg(EX,MSG) \
   ((EX)?((void)0):CGAL_postcondition_fail( # EX , __FILE__, __LINE__, MSG))
#  define CGAL_polygon_postcondition_code(CODE) CODE
#endif // CGAL_POLYGON_NO_POSTCONDITIONS

#if defined(CGAL_POLYGON_NO_POSTCONDITIONS) || defined(CGAL_NO_POSTCONDITIONS) \
  || (!defined(CGAL_POLYGON_CHECK_EXACTNESS) && !defined(CGAL_CHECK_EXACTNESS))\
  || defined(NDEBUG)
#  define CGAL_polygon_exactness_postcondition(EX) ((void)0)
#  define CGAL_polygon_exactness_postcondition_msg(EX,MSG) ((void)0)
#  define CGAL_polygon_exactness_postcondition_code(CODE)
#else
#  define CGAL_polygon_exactness_postcondition(EX) \
   ((EX)?((void)0):CGAL_postcondition_fail( # EX , __FILE__, __LINE__, 0))
#  define CGAL_polygon_exactness_postcondition_msg(EX,MSG) \
   ((EX)?((void)0):CGAL_postcondition_fail( # EX , __FILE__, __LINE__, MSG))
#  define CGAL_polygon_exactness_postcondition_code(CODE) CODE
#endif // CGAL_POLYGON_NO_POSTCONDITIONS

#if defined(CGAL_POLYGON_NO_POSTCONDITIONS) || defined(CGAL_NO_POSTCONDITIONS) \
  || (!defined(CGAL_POLYGON_CHECK_EXPENSIVE) && !defined(CGAL_CHECK_EXPENSIVE)) \
  || defined(NDEBUG)
#  define CGAL_polygon_expensive_postcondition(EX) ((void)0)
#  define CGAL_polygon_expensive_postcondition_msg(EX,MSG) ((void)0)
#  define CGAL_polygon_expensive_postcondition_code(CODE)
#else
#  define CGAL_polygon_expensive_postcondition(EX) \
   ((EX)?((void)0):CGAL_postcondition_fail( # EX , __FILE__, __LINE__, 0))
#  define CGAL_polygon_expensive_postcondition_msg(EX,MSG) \
   ((EX)?((void)0):CGAL_postcondition_fail( # EX , __FILE__, __LINE__, MSG))
#  define CGAL_polygon_expensive_postcondition_code(CODE) CODE
#endif // CGAL_POLYGON_NO_POSTCONDITIONS

#if defined(CGAL_POLYGON_NO_POSTCONDITIONS) || defined(CGAL_NO_POSTCONDITIONS) \
  || (!defined(CGAL_POLYGON_CHECK_EXACTNESS) && !defined(CGAL_CHECK_EXACTNESS))\
  || (!defined(CGAL_POLYGON_CHECK_EXPENSIVE) && !defined(CGAL_CHECK_EXPENSIVE)) \
  || defined(NDEBUG)
#  define CGAL_polygon_expensive_exactness_postcondition(EX) ((void)0)
#  define CGAL_polygon_expensive_exactness_postcondition_msg(EX,MSG) ((void)0)
#  define CGAL_polygon_expensive_exactness_postcondition_code(CODE)
#else
#  define CGAL_polygon_expensive_exactness_postcondition(EX) \
   ((EX)?((void)0):CGAL_postcondition_fail( # EX , __FILE__, __LINE__, 0))
#  define CGAL_polygon_expensive_exactness_postcondition_msg(EX,MSG) \
   ((EX)?((void)0):CGAL_postcondition_fail( # EX , __FILE__, __LINE__, MSG))
#  define CGAL_polygon_expensive_exactness_postcondition_code(CODE) CODE
#endif // CGAL_POLYGON_NO_POSTCONDITIONS


// warnings
// --------

#if defined(CGAL_POLYGON_NO_WARNINGS) || defined(CGAL_NO_WARNINGS) \
  || defined(NDEBUG)
#  define CGAL_polygon_warning(EX) ((void)0)
#  define CGAL_polygon_warning_msg(EX,MSG) ((void)0)
#  define CGAL_polygon_warning_code(CODE)
#else
#  define CGAL_polygon_warning(EX) \
   ((EX)?((void)0):CGAL_warning_fail( # EX , __FILE__, __LINE__, 0))
#  define CGAL_polygon_warning_msg(EX,MSG) \
   ((EX)?((void)0):CGAL_warning_fail( # EX , __FILE__, __LINE__, MSG))
#  define CGAL_polygon_warning_code(CODE) CODE
#endif // CGAL_POLYGON_NO_WARNINGS

#if defined(CGAL_POLYGON_NO_WARNINGS) || defined(CGAL_NO_WARNINGS) \
  || (!defined(CGAL_POLYGON_CHECK_EXACTNESS) && !defined(CGAL_CHECK_EXACTNESS))\
  || defined(NDEBUG)
#  define CGAL_polygon_exactness_warning(EX) ((void)0)
#  define CGAL_polygon_exactness_warning_msg(EX,MSG) ((void)0)
#  define CGAL_polygon_exactness_warning_code(CODE)
#else
#  define CGAL_polygon_exactness_warning(EX) \
   ((EX)?((void)0):CGAL_warning_fail( # EX , __FILE__, __LINE__, 0))
#  define CGAL_polygon_exactness_warning_msg(EX,MSG) \
   ((EX)?((void)0):CGAL_warning_fail( # EX , __FILE__, __LINE__, MSG))
#  define CGAL_polygon_exactness_warning_code(CODE) CODE
#endif // CGAL_POLYGON_NO_WARNINGS

#if defined(CGAL_POLYGON_NO_WARNINGS) || defined(CGAL_NO_WARNINGS) \
  || (!defined(CGAL_POLYGON_CHECK_EXPENSIVE) && !defined(CGAL_CHECK_EXPENSIVE)) \
  || defined(NDEBUG)
#  define CGAL_polygon_expensive_warning(EX) ((void)0)
#  define CGAL_polygon_expensive_warning_msg(EX,MSG) ((void)0)
#  define CGAL_polygon_expensive_warning_code(CODE)
#else
#  define CGAL_polygon_expensive_warning(EX) \
   ((EX)?((void)0):CGAL_warning_fail( # EX , __FILE__, __LINE__, 0))
#  define CGAL_polygon_expensive_warning_msg(EX,MSG) \
   ((EX)?((void)0):CGAL_warning_fail( # EX , __FILE__, __LINE__, MSG))
#  define CGAL_polygon_expensive_warning_code(CODE) CODE
#endif // CGAL_POLYGON_NO_WARNINGS

#if defined(CGAL_POLYGON_NO_WARNINGS) || defined(CGAL_NO_WARNINGS) \
  || (!defined(CGAL_POLYGON_CHECK_EXACTNESS) && !defined(CGAL_CHECK_EXACTNESS))\
  || (!defined(CGAL_POLYGON_CHECK_EXPENSIVE) && !defined(CGAL_CHECK_EXPENSIVE)) \
  || defined(NDEBUG)
#  define CGAL_polygon_expensive_exactness_warning(EX) ((void)0)
#  define CGAL_polygon_expensive_exactness_warning_msg(EX,MSG) ((void)0)
#  define CGAL_polygon_expensive_exactness_warning_code(CODE)
#else
#  define CGAL_polygon_expensive_exactness_warning(EX) \
   ((EX)?((void)0):CGAL_warning_fail( # EX , __FILE__, __LINE__, 0))
#  define CGAL_polygon_expensive_exactness_warning_msg(EX,MSG) \
   ((EX)?((void)0):CGAL_warning_fail( # EX , __FILE__, __LINE__, MSG))
#  define CGAL_polygon_expensive_exactness_warning_code(CODE) CODE
#endif // CGAL_POLYGON_NO_WARNINGS


