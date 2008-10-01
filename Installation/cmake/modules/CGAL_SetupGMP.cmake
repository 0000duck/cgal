if ( NOT GMP_FOUND )
  
  find_package( GMP REQUIRED )
  
  message( STATUS "GMP include:     ${GMP_INCLUDE_DIR}" )
  message( STATUS "GMP libraries:   ${GMP_LIBRARIES}" )
  message( STATUS "GMP definitions: ${GMP_DEFINITIONS}" )
  
  include(CGAL_Macros)
  
  get_dependency_version(GMP)
  
  cache_set(CGAL_3RD_PARTY_INCLUDE_DIRS   ${CGAL_3RD_PARTY_INCLUDE_DIRS}   ${GMP_INCLUDE_DIR}   "" )
  cache_set(CGAL_3RD_PARTY_LIBRARIES_DIRS ${CGAL_3RD_PARTY_LIBRARIES_DIRS} ${GMP_LIBRARIES_DIR} "" )
  cache_set(CGAL_3RD_PARTY_DEFINITIONS    ${CGAL_3RD_PARTY_DEFINITIONS}    ${GMP_DEFINITIONS}   "" )
  
  if ( NOT MSVC )
    cache_set(CGAL_3RD_PARTY_LIBRARIES ${CGAL_3RD_PARTY_LIBRARIES} ${GMP_LIBRARIES})
  endif()
  
endif()

